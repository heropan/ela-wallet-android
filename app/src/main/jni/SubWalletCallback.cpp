// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "SubWalletCallback.h"

namespace Elastos {
    namespace ElaWallet {

        SubWalletCallback::SubWalletCallback(
                /* [in] */ JNIEnv *env,
                /* [in] */ jobject jobj) {
            mObj = env->NewGlobalRef(jobj);
            env->GetJavaVM(&mVM);
        }

        SubWalletCallback::~SubWalletCallback() {
            if (mObj) {
                GetEnv()->DeleteGlobalRef(mObj);
            }
        }

        JNIEnv *SubWalletCallback::GetEnv() {
            JNIEnv *env;
            assert(mVM != NULL);
            mVM->AttachCurrentThread(&env, NULL);
            return env;
        }

        void SubWalletCallback::Detach() {
            assert(mVM != NULL);
            mVM->DetachCurrentThread();
        }

        void SubWalletCallback::OnTransactionStatusChanged(const std::string &txid,
                                                           const std::string &status,
                                                           const nlohmann::json &desc,
                                                           uint32_t confirms) {
            JNIEnv *env = GetEnv();

            jclass clazz = env->GetObjectClass(mObj);
            jmethodID methodId = env->GetMethodID(clazz, "OnTransactionStatusChanged",
                                                  "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
            jstring jtxid = env->NewStringUTF(txid.c_str());
            jstring jstatus = env->NewStringUTF(status.c_str());
            jstring jdesc = env->NewStringUTF(desc.dump().c_str());

            env->CallVoidMethod(mObj, methodId, jtxid, jstatus, jdesc, confirms);

            Detach();
        }

        void SubWalletCallback::OnBlockSyncStarted() {
            JNIEnv *env = GetEnv();

            jclass clazz = env->GetObjectClass(mObj);
            jmethodID methodId = env->GetMethodID(clazz, "OnBlockSyncStarted", "()V");
            env->CallVoidMethod(mObj, methodId);

            Detach();
        }

        void SubWalletCallback::OnBlockSyncProgress(uint32_t currentBlockHeight,
                                                    uint32_t estimatedHeight,
                                                    time_t lastBlockTime) {
            JNIEnv *env = GetEnv();

            jclass clazz = env->GetObjectClass(mObj);
            jmethodID methodId = env->GetMethodID(clazz, "OnBlockHeightIncreased", "(IIJ)V");
            env->CallVoidMethod(mObj, methodId, currentBlockHeight, estimatedHeight, lastBlockTime);

            Detach();
        }

        void SubWalletCallback::OnBlockSyncStopped() {
            JNIEnv *env = GetEnv();

            jclass clazz = env->GetObjectClass(mObj);
            jmethodID methodId = env->GetMethodID(clazz, "OnBlockSyncStopped", "()V");
            env->CallVoidMethod(mObj, methodId);

            Detach();
        }

        void SubWalletCallback::OnBalanceChanged(const std::string &asset, uint64_t balance) {
            JNIEnv *env = GetEnv();

            jstring assetID = env->NewStringUTF(asset.c_str());

            jclass clazz = env->GetObjectClass(mObj);
            jmethodID methodId = env->GetMethodID(clazz, "OnBalanceChanged",
                                                  "(Ljava/lang/String;J)V");
            env->CallVoidMethod(mObj, methodId, assetID, balance);

            Detach();
        }

        void
        SubWalletCallback::OnTxPublished(const std::string &hash, const nlohmann::json &result) {
            JNIEnv *env = GetEnv();

            jstring jResult = env->NewStringUTF(result.dump().c_str());
            jstring jHash = env->NewStringUTF(hash.c_str());

            jclass clazz = env->GetObjectClass(mObj);
            jmethodID methodId = env->GetMethodID(clazz, "OnTxPublished",
                                                  "(Ljava/lang/String;Ljava/lang/String;)V");
            env->CallVoidMethod(mObj, methodId, jHash, jResult);

            Detach();
        }

        void SubWalletCallback::OnTxDeleted(const std::string &hash, bool notifyUser,
                                            bool recommendRescan) {
            JNIEnv *env = GetEnv();

            jstring jHash = env->NewStringUTF(hash.c_str());

            jclass clazz = env->GetObjectClass(mObj);
            jmethodID methodId = env->GetMethodID(clazz, "OnTxDeleted", "(Ljava/lang/String;ZZ)V");
            env->CallVoidMethod(mObj, methodId, jHash, notifyUser, recommendRescan);

            Detach();
        }

    }
}

using namespace Elastos::ElaWallet;

#define JNI_InitSubWalletCallback "()J"

static jlong JNICALL InitSubWalletCallback(JNIEnv *env, jobject clazz) {
    SubWalletCallback *callback = NULL;

    try {
        callback = new SubWalletCallback(env, clazz);
    } catch (const std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return (jlong) callback;
}

#define JNI_DisposeNative "(J)V"

static void JNICALL DisposeNative(JNIEnv *env, jobject clazz, jlong instance) {
    SubWalletCallback *callback = (SubWalletCallback *) instance;
    delete callback;
}

static const JNINativeMethod methods[] = {
        REGISTER_METHOD(InitSubWalletCallback),
        REGISTER_METHOD(DisposeNative),
};

jint RegisterSubWalletCallback(JNIEnv *env, const std::string &path) {
    return RegisterNativeMethods(env, path + "SubWalletCallback", methods, NELEM(methods));
}
