package org.elastos.wallet.core;

public class DID {
    private long mInstance;

    public DID(long instance) {
        mInstance = instance;
    }

    public String GetDIDName() {
        return GetDIDName(mInstance);
    }

    public void SetValue(String keyPath, String valueJson) {
        SetValue(mInstance, keyPath, valueJson);
    }

    public String GetValue(String path) {
        return GetValue(mInstance, path);
    }

    public String GetHistoryValue(String keyPath) {
        return GetHistoryValue(mInstance, keyPath);
    }

    public String GetAllKeys(int start, int count) {
        return GetAllKeys(mInstance, start, count);
    }

    public String Sign(String message, String password) {
        return Sign(mInstance, message, password);
    }

    public String CheckSign(String message, String signature) {
        return CheckSign(mInstance, message, signature);
    }

    public String GetPublicKey() {
        return GetPublicKey(mInstance);
    }

    public String GenerateProgram(String message, String password) {
        return GenerateProgram(mInstance, message, password);
    }

    private native String GetDIDName(long instance);

    private native void SetValue(long instance, String keyPath, String valueJson);

    private native String GetValue(long instance, String path);

    private native String GetHistoryValue(long instance, String keyPath);

    private native String GetAllKeys(long instance, int start, int count);

    private native String Sign(long instance, String message, String password);

    private native String CheckSign(long instance, String message, String signature);

    private native String GetPublicKey(long instance);

    private native String GenerateProgram(long instance, String message, String password);
}
