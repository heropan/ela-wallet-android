// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "Utils.h"
#include "idid.h"
#include "nlohmann/json.hpp"

using namespace Elastos::DID;

#define JNI_GetDIDName "(J)Ljava/lang/String;"

static jstring JNICALL GetDIDName(JNIEnv *env, jobject clazz, jlong instance) {
    jstring idName = NULL;

    try {
        IDID *did = (IDID *) instance;
        std::string value = did->GetDIDName();
        idName = env->NewStringUTF(value.c_str());
    } catch (std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return idName;
}

#define JNI_SetValue "(JLjava/lang/String;Ljava/lang/String;)V"

static void JNICALL SetValue(JNIEnv *env, jobject clazz, jlong instance,
                             jstring jkeyPath,
                             jstring jvalueJson) {
    bool exception = false;
    std::string msgException;

    const char *keyPath = env->GetStringUTFChars(jkeyPath, NULL);
    const char *valueJson = env->GetStringUTFChars(jvalueJson, NULL);

    try {
        IDID *did = (IDID *) instance;
        did->SetValue(keyPath, nlohmann::json::parse(valueJson));
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jkeyPath, keyPath);
    env->ReleaseStringUTFChars(jvalueJson, valueJson);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }
}

#define JNI_GetValue "(JLjava/lang/String;)Ljava/lang/String;"

static /*nlohmann::json*/ jstring JNICALL GetValue(JNIEnv *env, jobject clazz, jlong instance,
                                                   jstring jpath) {
    bool exception = false;
    std::string msgException;

    const char *path = env->GetStringUTFChars(jpath, NULL);
    jstring value = NULL;

    try {
        IDID *did = (IDID *) instance;
        nlohmann::json jsonValue = did->GetValue(path);
        value = env->NewStringUTF(jsonValue.dump().c_str());
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jpath, path);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return value;
}

#define JNI_GetHistoryValue "(JLjava/lang/String;)Ljava/lang/String;"

static /*nlohmann::json*/ jstring JNICALL GetHistoryValue(JNIEnv *env, jobject clazz,
                                                          jlong instance, jstring jkeyPath) {
    bool exception = false;
    std::string msgException;

    const char *keyPath = env->GetStringUTFChars(jkeyPath, NULL);
    jstring value = NULL;

    try {
        IDID *did = (IDID *) instance;
        nlohmann::json jsonValue = did->GetHistoryValue(keyPath);
        value = env->NewStringUTF(jsonValue.dump().c_str());
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jkeyPath, keyPath);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return value;
}

#define JNI_GetAllKeys "(JII)Ljava/lang/String;"

static /*nlohmann::json*/ jstring JNICALL GetAllKeys(JNIEnv *env, jobject clazz, jlong instance,
                                                     jint jstart, jint jcount) {
    jstring keys = NULL;

    try {
        IDID *did = (IDID *) instance;
        nlohmann::json jsonValue = did->GetAllKeys(jstart, jcount);
        keys = env->NewStringUTF(jsonValue.dump().c_str());
    } catch (std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return keys;
}

#define JNI_Sign "(JLjava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL Sign(JNIEnv *env, jobject clazz, jlong instance, jstring jmessage,
                            jstring jpassword) {
    bool exception = false;
    std::string msgException;

    const char *message = env->GetStringUTFChars(jmessage, NULL);
    const char *password = env->GetStringUTFChars(jpassword, NULL);
    jstring result = NULL;

    try {
        IDID *did = (IDID *) instance;
        std::string r = did->Sign(message, password);
        result = env->NewStringUTF(r.c_str());
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jmessage, message);
    env->ReleaseStringUTFChars(jpassword, password);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_CheckSign "(JLjava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static /*nlohmann::json*/ jstring JNICALL CheckSign(JNIEnv *env, jobject clazz, jlong instance,
                                                    jstring jmessage, jstring jsignature) {
    bool exception = false;
    std::string msgException;

    const char *message = env->GetStringUTFChars(jmessage, NULL);
    const char *signature = env->GetStringUTFChars(jsignature, NULL);

    jstring result = NULL;

    try {
        IDID *did = (IDID *) instance;
        nlohmann::json r = did->CheckSign(message, signature);
        result = env->NewStringUTF(r.dump().c_str());
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jmessage, message);
    env->ReleaseStringUTFChars(jsignature, signature);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_GetPublicKey "(J)Ljava/lang/String;"

static jstring JNICALL GetPublicKey(JNIEnv *env, jobject clazz, jlong instance) {
    jstring key = NULL;

    try {
        IDID *did = (IDID *) instance;
        std::string value = did->GetPublicKey();
        key = env->NewStringUTF(value.c_str());
    } catch (std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return key;
}

#define JNI_GenerateProgram "(JLjava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL GenerateProgram(JNIEnv *env, jobject clazz, jlong instance,
                                       jstring jmessage, jstring jpassword) {
    bool exception = false;
    std::string msgException;

    const char *message = env->GetStringUTFChars(jmessage, NULL);
    const char *password = env->GetStringUTFChars(jpassword, NULL);
    jstring program = NULL;

    try {
        IDID *did = (IDID *) instance;
        nlohmann::json jsonValue = did->GenerateProgram(message, password);
        program = env->NewStringUTF(jsonValue.dump().c_str());
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jmessage, message);
    env->ReleaseStringUTFChars(jpassword, password);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return program;
}

static const JNINativeMethod methods[] = {
        REGISTER_METHOD(GetDIDName),
        REGISTER_METHOD(SetValue),
        REGISTER_METHOD(GetValue),
        REGISTER_METHOD(GetHistoryValue),
        REGISTER_METHOD(GetAllKeys),
        REGISTER_METHOD(Sign),
        REGISTER_METHOD(CheckSign),
        REGISTER_METHOD(GetPublicKey),
        REGISTER_METHOD(GenerateProgram),
};

int registerDID(JNIEnv *env, const std::string &path) {
    return registerNativeMethods(env, path + "DID", methods, NELEM(methods));
}

