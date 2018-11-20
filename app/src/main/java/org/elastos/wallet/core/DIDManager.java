package org.elastos.wallet.core;

public class DIDManager {
    private long mInstance;

    DIDManager(long instance) {
        mInstance = instance;
    }

    long getInstance() {
        return mInstance;
    }

    public DID CreateDID(String password) {
        long instance = CreateDID(mInstance, password);
        return new DID(instance);
    }

    public DID GetDID(String didName) {
        long instance = GetDID(mInstance, didName);
        return new DID(instance);
    }

    public String GetDIDList() {
        return GetDIDList(mInstance);
    }

    public void DestroyDID(String didName) {
        DestroyDID(mInstance, didName);
    }

    public boolean RegisterCallback(String id, IDManagerCallback callback) {
        return RegisterCallback(mInstance, id, callback);
    }

    public boolean UnregisterCallback(String id) {
        return UnregisterCallback(mInstance, id);
    }

    private native long CreateDID(long instance, String password);

    private native long GetDID(long instance, String didName);

    private native String GetDIDList(long instance);

    private native void DestroyDID(long instance, String didName);

    private native boolean RegisterCallback(long instance, String id, IDManagerCallback callback);

    private native boolean UnregisterCallback(long instance, String id);
}
