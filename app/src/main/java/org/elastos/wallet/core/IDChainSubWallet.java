package org.elastos.wallet.core;

/**
 * IDChainSubWallet jni
 */
public class IDChainSubWallet extends SidechainSubWallet {
    private long mInstance;

    public String CreateIdTransaction(
            String fromAddress, String payloadJson, String programJson, String memo, String remark)
            throws WalletException {
        return CreateIdTransaction(mInstance, fromAddress, payloadJson, programJson, memo, remark);
    }

    public IDChainSubWallet(long instance) {
        super(instance);
        mInstance = instance;
    }

    private native String CreateIdTransaction(
            long instance, String fromAddress, String payloadJson, String programJson, String memo, String remark);
}
