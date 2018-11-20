// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "Utils.h"
#include <unistd.h>

#define TAG "Utils"

const std::string classesPackagePath = "org/elastos/wallet/core/";
const std::string exceptionClassPath = classesPackagePath + "WalletException";

extern int registerMasterWalletManager(JNIEnv *env, const std::string &path);

extern int registerMasterWallet(JNIEnv *env, const std::string &path);

extern int registerSubWallet(JNIEnv *env, const std::string &path);

extern int registerMainchainSubWallet(JNIEnv *env, const std::string &path);

extern int registerSidechainSubWallet(JNIEnv *env, const std::string &path);

extern int registerIDChainSubWallet(JNIEnv *env, const std::string &path);

//did
extern int registerDID(JNIEnv *env, const std::string &path);

extern int registerDIDManager(JNIEnv *env, const std::string &path);

extern int registerDIDService(JNIEnv *env, const std::string &path);

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    registerMasterWalletManager(env, classesPackagePath);
    registerMasterWallet(env, classesPackagePath);
    registerSubWallet(env, classesPackagePath);
    registerMainchainSubWallet(env, classesPackagePath);
    registerSidechainSubWallet(env, classesPackagePath);
    registerIDChainSubWallet(env, classesPackagePath);

    //did
    registerDID(env, classesPackagePath);
    registerDIDManager(env, classesPackagePath);
    registerDIDService(env, classesPackagePath);

    return JNI_VERSION_1_6;
}

int registerNativeMethods(JNIEnv *env, const std::string &className, const JNINativeMethod *methods,
                          int numMethods) {
    LOGW(TAG, "Registering %s's %d native methods...", className.c_str(), numMethods);

    jclass cls = env->FindClass(className.c_str());
    return env->RegisterNatives(cls, methods, numMethods);
}

void CheckErrorAndLog(
        /* [in] */ JNIEnv *env,
        /* [in] */ const char *errlog,
        /* [in] */ int line) {
    if (env->ExceptionCheck() != 0) {
        LOGW(TAG, errlog, line);
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
}

void CheckErrorAndLog(
        /* [in] */ JNIEnv *env,
        /* [in] */ const char *errlog,
        /* [in] */ const char *paramname,
        /* [in] */ int line) {
    if (env->ExceptionCheck() != 0) {
        LOGW(TAG, errlog, paramname, line);
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
}

jlong GetJavaLongField(JNIEnv *env, jclass klass, jobject jobj, const std::string &fieldName) {
    if (env == NULL || jobj == NULL || fieldName == "") {
        LOGW(TAG, "GetJavaLongField() invalid param while get field:%s  : %d!\n", fieldName.c_str(),
             __LINE__);
        return 0;
    }

    jfieldID field = env->GetFieldID(klass, fieldName.c_str(), "J");
    CheckErrorAndLog(env, "Fail get long field id:%s  : %d!\n", fieldName.c_str(), __LINE__);
    jlong value = env->GetLongField(jobj, field);
    CheckErrorAndLog(env, "Fail get long field: %s : %d!\n", fieldName.c_str(), __LINE__);
    return value;
}

void ThrowLogicException(JNIEnv *env, const char *errorInfo) {
    jclass walletException = env->FindClass(exceptionClassPath.c_str());
    env->ExceptionClear();
    env->ThrowNew(walletException, errorInfo);
}

void ThrowWalletExceptionWithECode(JNIEnv *env, int errorcode, const char *errorInfo) {
    jclass walletException = env->FindClass(exceptionClassPath.c_str());
    jmethodID methodId = env->GetMethodID(walletException, "<init>", "(ILjava/lang/String;)V");
    jstring arg = env->NewStringUTF(errorInfo);
    jthrowable throwable = (jthrowable) env->NewObject(walletException, methodId, errorcode, arg);
    env->ExceptionClear();
    env->Throw(throwable);
}

void ThrowWalletException(JNIEnv *env, const char *errorInfo) {
    env->ExceptionClear();
    jclass walletException = env->FindClass(exceptionClassPath.c_str());
    jmethodID methodId = env->GetMethodID(walletException, "<init>", "(Ljava/lang/String;)V");
    jstring arg = env->NewStringUTF(errorInfo);
    jthrowable throwable = (jthrowable) env->NewObject(walletException, methodId, arg);
    env->Throw(throwable);
}

int UTF82UnicodeOne(const char *utf8, wchar_t &wch) {
    unsigned char firstCh = utf8[0];
    if (firstCh >= 0xC0) {
        int afters, code;
        if ((firstCh & 0xE0) == 0xC0) {
            afters = 2;
            code = firstCh & 0x1F;
        } else if ((firstCh & 0xF0) == 0xE0) {
            afters = 3;
            code = firstCh & 0xF;
        } else if ((firstCh & 0xF8) == 0xF0) {
            afters = 4;
            code = firstCh & 0x7;
        } else if ((firstCh & 0xFC) == 0xF8) {
            afters = 5;
            code = firstCh & 0x3;
        } else if ((firstCh & 0xFE) == 0xFC) {
            afters = 6;
            code = firstCh & 0x1;
        } else {
            wch = firstCh;
            return 1;
        }

        for (int k = 1; k < afters; ++k) {
            if ((utf8[k] & 0xC0) != 0x80) {
                wch = firstCh;
                return 1;
            }

            code <<= 6;
            code |= (unsigned char) utf8[k] & 0x3F;
        }

        wch = code;
        return afters;
    } else {
        wch = firstCh;
    }

    return 1;
}

int UTF82Unicode(const char *utf8Buf, wchar_t *pUniBuf, int utf8Leng) {
    int i = 0, count = 0;
    while (i < utf8Leng) {
        i += UTF82UnicodeOne(utf8Buf + i, pUniBuf[count]);
        count++;
    }

    return count;
}

jstring stringTojstring(JNIEnv *env, std::string str) {
    int len = str.length();
    wchar_t *wcs = new wchar_t[len * 2];
    int nRet = UTF82Unicode(str.c_str(), wcs, len);
    jchar *jcs = new jchar[nRet];
    for (int i = 0; i < nRet; i++) {
        jcs[i] = (jchar) wcs[i];
    }

    jstring retString = env->NewString(jcs, nRet);
    delete[] wcs;
    delete[] jcs;
    return retString;
}
