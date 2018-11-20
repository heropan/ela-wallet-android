// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "Utils.h"
#include "ididManager.h"

using namespace Elastos::DID;

#define JNI_CreateDID "(JLjava/lang/String;)J"

static jlong JNICALL CreateDID(JNIEnv *env, jobject clazz, jlong instance, jstring jpassword) {
    bool exception = false;
    std::string msgException;

    const char *password = env->GetStringUTFChars(jpassword, NULL);
    IDID *did = NULL;

    try {
        IDIDManager *didMgr = (IDIDManager *) instance;
        did = didMgr->CreateDID(password);
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jpassword, password);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return (jlong) did;
}

#define JNI_GetDID "(JLjava/lang/String;)J"

static jlong JNICALL GetDID(JNIEnv *env, jobject clazz, jlong instance, jstring jdidName) {
    bool exception = false;
    std::string msgException;

    const char *didName = env->GetStringUTFChars(jdidName, NULL);
    IDID *did = NULL;

    try {
        IDIDManager *didMgr = (IDIDManager *) instance;
        did = didMgr->GetDID(didName);
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jdidName, didName);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return (jlong) did;
}

#define JNI_GetDIDList "(J)Ljava/lang/String;"

static /*nlohmann::json*/ jstring JNICALL
GetDIDList(JNIEnv *env, jobject clazz, jlong jDidMgrProxy) {
    jstring list = NULL;

    try {
        IDIDManager *didMgr = (IDIDManager *) jDidMgrProxy;
        nlohmann::json jsonValue = didMgr->GetDIDList();
        list = env->NewStringUTF(jsonValue.dump().c_str());
    } catch (std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return list;
}

#define JNI_DestroyDID "(JLjava/lang/String;)V"

static void JNICALL DestroyDID(JNIEnv *env, jobject clazz, jlong instance, jstring jdidName) {
    bool exception = false;
    std::string msgException;

    const char *didName = env->GetStringUTFChars(jdidName, NULL);

    try {
        IDIDManager *didMgr = (IDIDManager *) instance;
        didMgr->DestoryDID(didName);
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jdidName, didName);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }
}

class ElaIdManagerCallback : public IIdManagerCallback {
public:
    virtual void OnIdStatusChanged(
            const std::string &id,
            const std::string &path,
            const nlohmann::json &value);

    ElaIdManagerCallback(
            /* [in] */ JNIEnv *env,
            /* [in] */ jobject jobj);

    ~ElaIdManagerCallback();

private:
    JNIEnv *GetEnv();

    void Detach();

private:
    JavaVM *mVM;
    jobject mObj;
};

#define JNI_RegisterCallback "(JLjava/lang/String;Lorg/elastos/wallet/core/IDManagerCallback;)Z"
static std::map<jstring, ElaIdManagerCallback *> sIdCallbackMap;

static jboolean JNICALL
RegisterCallback(JNIEnv *env, jobject clazz, jlong instance, jstring jdidName,
                 jobject jidCallback) {
    bool exception = false;
    std::string msgException;

    const char *didName = env->GetStringUTFChars(jdidName, NULL);
    ElaIdManagerCallback *idCallback = new ElaIdManagerCallback(env, jidCallback);
    IDIDManager *didMgr = (IDIDManager *) instance;
    jboolean status = false;

    try {
        status = didMgr->RegisterCallback(didName, idCallback);
        sIdCallbackMap[jdidName] = idCallback;
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jdidName, didName);

    if (exception) {
        delete idCallback;
        ThrowWalletException(env, msgException.c_str());
    }

    return status;
}

#define JNI_UnregisterCallback "(JLjava/lang/String;)Z"

static jboolean JNICALL
UnregisterCallback(JNIEnv *env, jobject clazz, jlong instance, jstring jdidName) {
    bool exception = false;
    std::string msgException;

    const char *didName = env->GetStringUTFChars(jdidName, NULL);

    jboolean status = false;

    try {
        IDIDManager *didMgr = (IDIDManager *) instance;
        std::map<jstring, ElaIdManagerCallback *>::iterator it;
        for (it = sIdCallbackMap.begin(); it != sIdCallbackMap.end(); it++) {
            if (jdidName == it->first) {
                status = didMgr->UnregisterCallback(didName);
                delete it->second;
                sIdCallbackMap.erase(it);
                break;
            }
        }
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jdidName, didName);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return status;
}

static const JNINativeMethod methods[] = {
        REGISTER_METHOD(CreateDID),
        REGISTER_METHOD(GetDID),
        REGISTER_METHOD(GetDIDList),
        REGISTER_METHOD(DestroyDID),
        REGISTER_METHOD(RegisterCallback),
        REGISTER_METHOD(UnregisterCallback),
};

int registerDIDManager(JNIEnv *env, const std::string &path) {
    return registerNativeMethods(env, path + "DIDManager", methods, NELEM(methods));
}

ElaIdManagerCallback::ElaIdManagerCallback(
        /* [in] */ JNIEnv *env,
        /* [in] */ jobject jobj) {
    mObj = env->NewGlobalRef(jobj);
    env->GetJavaVM(&mVM);
}

ElaIdManagerCallback::~ElaIdManagerCallback() {
    if (mObj) {
        GetEnv()->DeleteGlobalRef(mObj);
    }
}

JNIEnv *ElaIdManagerCallback::GetEnv() {
    JNIEnv *env;
    assert(mVM != NULL);
    mVM->AttachCurrentThread(&env, NULL);
    return env;
}

void ElaIdManagerCallback::Detach() {
    assert(mVM != NULL);
    mVM->DetachCurrentThread();
}

void ElaIdManagerCallback::OnIdStatusChanged(const std::string &id,
                                             const std::string &path, const nlohmann::json &value) {
    JNIEnv *env = GetEnv();

    jclass clazz = env->GetObjectClass(mObj);
    jmethodID methodId = env->GetMethodID(clazz, "OnIDStatusChanged",
                                          "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    jstring jid = env->NewStringUTF(id.c_str());
    jstring jpath = env->NewStringUTF(path.c_str());
    jstring jvalue = env->NewStringUTF(value.dump().c_str());

    env->CallVoidMethod(mObj, methodId, jid, jpath, jvalue);

    Detach();
}
