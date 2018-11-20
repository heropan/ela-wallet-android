// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "Utils.h"
#include "DIDManagerSupervisor.h"

using namespace Elastos::ElaWallet;
using namespace Elastos::DID;

#define JNI_NewDIDService "(Ljava/lang/String;)J"

static jlong JNICALL NewDIDService(JNIEnv *env, jobject clazz, jstring jrootPath) {
    bool exception = false;
    std::string msgException;

    const char *rootPath = env->GetStringUTFChars(jrootPath, NULL);
    DIDManagerSupervisor *supervisor = NULL;

    try {
        supervisor = new DIDManagerSupervisor(rootPath);
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return (jlong) supervisor;
}

#define JNI_DisposeNative "(J)V"

static void JNICALL DisposeNative(JNIEnv *env, jobject clazz, jlong instance) {
    bool exception = false;
    std::string msgException;

    try {
        DIDManagerSupervisor *supervisor = (DIDManagerSupervisor *) instance;
        delete supervisor;
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }
}

#define JNI_CreateDIDManager "(JJLjava/lang/String;)J"

static jlong JNICALL CreateDIDManager(JNIEnv *env, jobject clazz, jlong jSupervisor,
                                      jlong jMasterWallet, jstring jrootPath) {
    bool exception = false;
    std::string msgException;

    const char *rootPath = env->GetStringUTFChars(jrootPath, NULL);
    IDIDManager *manager = NULL;

    try {
        DIDManagerSupervisor *supervisor = (DIDManagerSupervisor *) jSupervisor;
        IMasterWallet *masterWallet = (IMasterWallet *) jMasterWallet;

        manager = supervisor->CreateDIDManager(masterWallet, rootPath);
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jrootPath, rootPath);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return (jlong) manager;
}

#define JNI_DestroyDIDManager "(JJ)V"

static void JNICALL DestroyDIDManager(JNIEnv *env, jobject clazz, jlong jSupervisor,
                                      jlong jIDManager) {
    bool exception = false;
    std::string msgException;

    try {
        DIDManagerSupervisor *supervisor = (DIDManagerSupervisor *) jSupervisor;
        IDIDManager *manager = (IDIDManager *) jIDManager;

        supervisor->DestroyDIDManager(manager);
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }
}

static const JNINativeMethod methods[] = {
        REGISTER_METHOD(NewDIDService),
        REGISTER_METHOD(DisposeNative),
        REGISTER_METHOD(CreateDIDManager),
        REGISTER_METHOD(DestroyDIDManager),
};

int registerDIDService(JNIEnv *env, const std::string &path) {
    return registerNativeMethods(env, path + "DIDService", methods, NELEM(methods));
}

