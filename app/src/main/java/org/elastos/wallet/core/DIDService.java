package org.elastos.wallet.core;

public class DIDService {
    private long mInstance;

    public DIDService(String rootPath) {
        mInstance = NewDIDService(rootPath);
    }

    public void finalize() {
        DisposeNative(mInstance);
    }

    public DIDManager CreateDIDManager(MasterWallet masterWallet, String rootPath) throws WalletException {
        long instance = CreateDIDManager(mInstance, masterWallet.GetInstance(), rootPath);
        return new DIDManager(instance);
    }

    public void DestroyDIDManager(DIDManager manager) throws WalletException {
        DestroyDIDManager(mInstance, manager.getInstance());
    }

    private native long NewDIDService(String rootPath);

    private native void DisposeNative(long instance);

    private native long CreateDIDManager(long instance, long masterWallet, String rootPath);

    private native void DestroyDIDManager(long instance, long managerInstance);
}
