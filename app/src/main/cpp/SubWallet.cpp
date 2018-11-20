// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <sstream>
#include "Utils.h"
#include "ISubWallet.h"
#include "nlohmann/json.hpp"

#define TAG "SubWallet"

using namespace Elastos::ElaWallet;

#define JNI_GetChainId "(J)Ljava/lang/String;"

static jstring JNICALL GetChainId(JNIEnv *env, jobject clazz, jlong jSubProxy) {
    jstring chainId = NULL;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        std::string result = subWallet->GetChainId();
        chainId = env->NewStringUTF(result.c_str());
    } catch (std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return chainId;
}

#define JNI_GetBasicInfo "(J)Ljava/lang/String;"

static jstring JNICALL GetBasicInfo(JNIEnv *env, jobject clazz, jlong jSubProxy) {
    jstring info = NULL;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        nlohmann::json result = subWallet->GetBasicInfo();
        info = env->NewStringUTF(result.dump().c_str());
    } catch (std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return info;
}

#define JNI_GetBalanceInfo "(J)Ljava/lang/String;"

static jstring JNICALL GetBalanceInfo(JNIEnv *env, jobject clazz, jlong jSubProxy) {
    jstring info = NULL;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        nlohmann::json result = subWallet->GetBalanceInfo();
        info = env->NewStringUTF(result.dump().c_str());
    } catch (std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return info;
}

#define JNI_GetBalance "(J)J"

static jlong JNICALL GetBalance(JNIEnv *env, jobject clazz, jlong jSubProxy) {
    jlong balance = 0;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        balance = (jlong) subWallet->GetBalance();
    } catch (std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return balance;
}

#define JNI_CreateAddress "(J)Ljava/lang/String;"

static jstring JNICALL CreateAddress(JNIEnv *env, jobject clazz, jlong jSubProxy) {
    jstring addr = NULL;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        std::string result = subWallet->CreateAddress();
        addr = env->NewStringUTF(result.c_str());
    } catch (std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return addr;
}

#define JNI_GetAllAddress "(JII)Ljava/lang/String;"

static jstring JNICALL GetAllAddress(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                     jint jStart,
                                     jint jCount) {
    jstring addresses = NULL;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        nlohmann::json addressesJson = subWallet->GetAllAddress(jStart, jCount);
        addresses = env->NewStringUTF(addressesJson.dump().c_str());
    } catch (std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return addresses;
}

#define JNI_GetBalanceWithAddress "(JLjava/lang/String;)J"

static jlong JNICALL GetBalanceWithAddress(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                           jstring jaddress) {
    bool exception = false;
    std::string msgException;

    const char *address = env->GetStringUTFChars(jaddress, NULL);
    jlong result = 0;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        result = (jlong) subWallet->GetBalanceWithAddress(address);
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jaddress, address);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return (jlong) result;
}

class ElaSubWalletCallback : public ISubWalletCallback {
public:
    virtual void OnTransactionStatusChanged(
            const std::string &txid,
            const std::string &status,
            const nlohmann::json &desc,
            uint32_t confirms);

    virtual void OnBlockSyncStarted();

    /**
     * Callback method fired when best block chain height increased. This callback could be used to show progress.
     * @param currentBlockHeight is the of current block when callback fired.
     * @param progress is current progress when block height increased.
     */
    virtual void OnBlockHeightIncreased(uint32_t currentBlockHeight, int progress);

    /**
     * Callback method fired when block end synchronizing with a peer. This callback could be used to show progress.
     */
    virtual void OnBlockSyncStopped();

    virtual void OnBalanceChanged(uint64_t balance);

    ElaSubWalletCallback(
            /* [in] */ JNIEnv *env,
            /* [in] */ jobject jobj);

    ~ElaSubWalletCallback();

private:
    JNIEnv *GetEnv();

    void Detach();

private:
    JavaVM *mVM;
    jobject mObj;
};


static std::map<jlong, ElaSubWalletCallback *> sSubCallbackMap;
#define JNI_AddCallback "(JLorg/elastos/wallet/core/SubWalletCallback;)V"

static void JNICALL AddCallback(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                jobject jsubCallback) {
    try {
        if (sSubCallbackMap.find(jSubProxy) == sSubCallbackMap.end()) {
            ElaSubWalletCallback *subCallback = new ElaSubWalletCallback(env, jsubCallback);
            ISubWallet *subWallet = (ISubWallet *) jSubProxy;
            subWallet->AddCallback(subCallback);
            sSubCallbackMap[jSubProxy] = subCallback;
        } else {
            LOGE(TAG, "Sub wallet callback already exist");
        }
    } catch (std::exception &e) {
        ThrowWalletException(env, e.what());
    }
}

#define JNI_RemoveCallback "(J)V"

static void JNICALL RemoveCallback(JNIEnv *env, jobject clazz, jlong jSubProxy) {
    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        if (sSubCallbackMap.find(jSubProxy) != sSubCallbackMap.end()) {
            ElaSubWalletCallback *callback = sSubCallbackMap[jSubProxy];
            delete callback;
            sSubCallbackMap.erase(jSubProxy);
        } else {
            LOGW(TAG, "Sub wallet callback already removed");
        }
    } catch (std::exception &e) {
        ThrowWalletException(env, e.what());
    }
}

#define JNI_CreateTransaction "(JLjava/lang/String;Ljava/lang/String;JLjava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL CreateTransaction(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                         jstring jfromAddress,
                                         jstring jtoAddress,
                                         jlong amount,
                                         jstring jmemo,
                                         jstring jremark) {
    bool exception = false;
    std::string msgException;

    const char *fromAddress = env->GetStringUTFChars(jfromAddress, NULL);
    const char *toAddress = env->GetStringUTFChars(jtoAddress, NULL);
    const char *memo = env->GetStringUTFChars(jmemo, NULL);
    const char *remark = env->GetStringUTFChars(jremark, NULL);

    ISubWallet *subWallet = (ISubWallet *) jSubProxy;

    jstring tx = NULL;
    try {
        nlohmann::json result = subWallet->CreateTransaction(fromAddress, toAddress, amount, memo,
                                                             remark);
        tx = env->NewStringUTF(result.dump().c_str());
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jfromAddress, fromAddress);
    env->ReleaseStringUTFChars(jtoAddress, toAddress);
    env->ReleaseStringUTFChars(jmemo, memo);
    env->ReleaseStringUTFChars(jremark, remark);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}

#define JNI_CreateMultiSignTransaction "(JLjava/lang/String;Ljava/lang/String;JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL CreateMultiSignTransaction(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                                  jstring jfromAddress,
                                                  jstring jtoAddress,
                                                  jlong amount,
                                                  jstring jmemo) {
    bool exception = false;
    std::string msgException;

    const char *fromAddress = env->GetStringUTFChars(jfromAddress, NULL);
    const char *toAddress = env->GetStringUTFChars(jtoAddress, NULL);
    const char *memo = env->GetStringUTFChars(jmemo, NULL);

    ISubWallet *subWallet = (ISubWallet *) jSubProxy;
    jstring tx = NULL;

    try {
        nlohmann::json result = subWallet->CreateMultiSignTransaction(fromAddress, toAddress,
                                                                      amount, memo);
        tx = env->NewStringUTF(result.dump().c_str());
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jfromAddress, fromAddress);
    env->ReleaseStringUTFChars(jtoAddress, toAddress);
    env->ReleaseStringUTFChars(jmemo, memo);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}

#define JNI_CalculateTransactionFee "(JLjava/lang/String;J)J"

static jlong JNICALL CalculateTransactionFee(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                             jstring jrawTransaction,
                                             jlong feePerKb) {
    bool exception = false;
    std::string msgException;

    const char *rawTransaction = env->GetStringUTFChars(jrawTransaction, NULL);

    ISubWallet *subWallet = (ISubWallet *) jSubProxy;
    jlong fee = 0;

    try {
        nlohmann::json tx = nlohmann::json::parse(rawTransaction);
        fee = (jlong) subWallet->CalculateTransactionFee(tx, feePerKb);
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jrawTransaction, rawTransaction);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return fee;
}

#define JNI_UpdateTransactionFee "(JLjava/lang/String;J)Ljava/lang/String;"

static jstring JNICALL UpdateTransactionFee(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                            jstring jrawTransaction,
                                            jlong fee) {
    bool exception = false;
    std::string msgException;

    const char *rawTransaction = env->GetStringUTFChars(jrawTransaction, NULL);

    ISubWallet *subWallet = (ISubWallet *) jSubProxy;
    jstring tx = NULL;

    try {
        nlohmann::json txJson = subWallet->UpdateTransactionFee(
                nlohmann::json::parse(rawTransaction), fee);
        tx = env->NewStringUTF(txJson.dump().c_str());
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jrawTransaction, rawTransaction);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}

#define JNI_SignTransaction "(JLjava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL SignTransaction(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                       jstring jRawTransaction,
                                       jstring jPayPassword) {
    bool exception = false;
    std::string msgException;

    const char *rawTransaction = env->GetStringUTFChars(jRawTransaction, NULL);
    const char *payPassword = env->GetStringUTFChars(jPayPassword, NULL);

    ISubWallet *subWallet = (ISubWallet *) jSubProxy;
    jstring tx = NULL;

    try {
        nlohmann::json result = subWallet->SignTransaction(nlohmann::json::parse(rawTransaction),
                                                           payPassword);
        tx = env->NewStringUTF(result.dump().c_str());
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jRawTransaction, rawTransaction);
    env->ReleaseStringUTFChars(jPayPassword, payPassword);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}

#define JNI_GetTransactionSignedSigners "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL GetTransactionSignedSigners(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                                   jstring jTransactionJson) {
    bool exception = false;
    std::string msgException;

    const char *transactionJson = env->GetStringUTFChars(jTransactionJson, NULL);
    jstring result = NULL;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        nlohmann::json signers = subWallet->GetTransactionSignedSigners(
                nlohmann::json::parse(transactionJson));
        result = env->NewStringUTF(signers.dump().c_str());
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jTransactionJson, transactionJson);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_PublishTransaction "(JLjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL PublishTransaction(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                          jstring jTransactionJson) {
    bool exception = false;
    std::string msgException;

    const char *transactionJson = env->GetStringUTFChars(jTransactionJson, NULL);

    jstring result = NULL;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        nlohmann::json r = subWallet->PublishTransaction(nlohmann::json::parse(transactionJson));
        result = env->NewStringUTF(r.dump().c_str());
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jTransactionJson, transactionJson);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_GetAllTransaction "(JIILjava/lang/String;)Ljava/lang/String;"

static jstring JNICALL GetAllTransaction(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                         jint start,
                                         jint count,
                                         jstring jaddressOrTxid) {
    bool exception = false;
    std::string msgException;

    const char *addressOrTxid = env->GetStringUTFChars(jaddressOrTxid, NULL);
    jstring tx = NULL;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        nlohmann::json result = subWallet->GetAllTransaction(start, count, addressOrTxid);
        tx = env->NewStringUTF(result.dump().c_str());
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jaddressOrTxid, addressOrTxid);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return tx;
}


#define JNI_Sign "(JLjava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL Sign(JNIEnv *env, jobject clazz, jlong jSubProxy,
                            jstring jmessage,
                            jstring jpayPassword) {
    bool exception = false;
    std::string msgException;

    const char *message = env->GetStringUTFChars(jmessage, NULL);
    const char *payPassword = env->GetStringUTFChars(jpayPassword, NULL);

    ISubWallet *subWallet = (ISubWallet *) jSubProxy;
    jstring result = NULL;

    try {
        std::string r = subWallet->Sign(message, payPassword);
        result = env->NewStringUTF(r.c_str());
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jmessage, message);
    env->ReleaseStringUTFChars(jpayPassword, payPassword);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_CheckSign "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"

static jstring JNICALL CheckSign(JNIEnv *env, jobject clazz, jlong jSubProxy,
                                 jstring jPublicKey,
                                 jstring jMessage,
                                 jstring jSignature) {
    bool exception = false;
    std::string msgException;

    const char *publicKey = env->GetStringUTFChars(jPublicKey, NULL);
    const char *message = env->GetStringUTFChars(jMessage, NULL);
    const char *signature = env->GetStringUTFChars(jSignature, NULL);

    jstring result = NULL;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        nlohmann::json r = subWallet->CheckSign(publicKey, message, signature);
        result = env->NewStringUTF(r.dump().c_str());
    } catch (std::exception &e) {
        exception = true;
        msgException = e.what();
    }

    env->ReleaseStringUTFChars(jPublicKey, publicKey);
    env->ReleaseStringUTFChars(jMessage, message);
    env->ReleaseStringUTFChars(jSignature, signature);

    if (exception) {
        ThrowWalletException(env, msgException.c_str());
    }

    return result;
}

#define JNI_GetPublicKey "(J)Ljava/lang/String;"

static jstring JNICALL GetPublicKey(JNIEnv *env, jobject clazz, jlong jSubProxy) {
    bool exception = false;
    std::string msgException;

    jstring key = NULL;

    try {
        ISubWallet *subWallet = (ISubWallet *) jSubProxy;
        std::string result = subWallet->GetPublicKey();
        key = env->NewStringUTF(result.c_str());
    } catch (std::exception &e) {
        ThrowWalletException(env, e.what());
    }

    return key;
}

static const JNINativeMethod methods[] = {
        REGISTER_METHOD(GetChainId),
        REGISTER_METHOD(GetBasicInfo),
        REGISTER_METHOD(GetBalanceInfo),
        REGISTER_METHOD(GetBalance),
        REGISTER_METHOD(CreateAddress),
        REGISTER_METHOD(GetAllAddress),
        REGISTER_METHOD(GetBalanceWithAddress),
        REGISTER_METHOD(AddCallback),
        REGISTER_METHOD(RemoveCallback),
        REGISTER_METHOD(CreateTransaction),
        REGISTER_METHOD(CreateMultiSignTransaction),
        REGISTER_METHOD(CalculateTransactionFee),
        REGISTER_METHOD(UpdateTransactionFee),
        REGISTER_METHOD(SignTransaction),
        REGISTER_METHOD(GetTransactionSignedSigners),
        REGISTER_METHOD(PublishTransaction),
        REGISTER_METHOD(GetAllTransaction),
        REGISTER_METHOD(Sign),
        REGISTER_METHOD(CheckSign),
        REGISTER_METHOD(GetPublicKey),
};

int registerSubWallet(JNIEnv *env, const std::string &path) {
    return registerNativeMethods(env, path + "SubWallet", methods, NELEM(methods));
}

ElaSubWalletCallback::ElaSubWalletCallback(
        /* [in] */ JNIEnv *env,
        /* [in] */ jobject jobj) {
    mObj = env->NewGlobalRef(jobj);
    env->GetJavaVM(&mVM);
}

ElaSubWalletCallback::~ElaSubWalletCallback() {
    if (mObj) {
        GetEnv()->DeleteGlobalRef(mObj);
    }
}

JNIEnv *ElaSubWalletCallback::GetEnv() {
    JNIEnv *env;
    assert(mVM != NULL);
    mVM->AttachCurrentThread(&env, NULL);
    return env;
}

void ElaSubWalletCallback::Detach() {
    assert(mVM != NULL);
    mVM->DetachCurrentThread();
}

void ElaSubWalletCallback::OnTransactionStatusChanged(const std::string &txid, const std::string &status,
                                                      const nlohmann::json &desc, uint32_t confirms) {
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

void ElaSubWalletCallback::OnBlockSyncStarted() {
    JNIEnv *env = GetEnv();

    jclass clazz = env->GetObjectClass(mObj);
    jmethodID methodId = env->GetMethodID(clazz, "OnBlockSyncStarted", "()V");
    env->CallVoidMethod(mObj, methodId);

    Detach();
}

void ElaSubWalletCallback::OnBlockHeightIncreased(uint32_t currentBlockHeight, int progress) {
    JNIEnv *env = GetEnv();

    jclass clazz = env->GetObjectClass(mObj);
    jmethodID methodId = env->GetMethodID(clazz, "OnBlockHeightIncreased", "(II)V");
    env->CallVoidMethod(mObj, methodId, currentBlockHeight, progress);

    Detach();
}

void ElaSubWalletCallback::OnBlockSyncStopped() {
    JNIEnv *env = GetEnv();

    jclass clazz = env->GetObjectClass(mObj);
    jmethodID methodId = env->GetMethodID(clazz, "OnBlockSyncStopped", "()V");
    env->CallVoidMethod(mObj, methodId);

    Detach();
}

void ElaSubWalletCallback::OnBalanceChanged(uint64_t balance) {
    JNIEnv *env = GetEnv();

    jclass clazz = env->GetObjectClass(mObj);
    jmethodID methodId = env->GetMethodID(clazz, "OnBalanceChanged", "(J)V");
    env->CallVoidMethod(mObj, methodId, balance);

    Detach();
}

