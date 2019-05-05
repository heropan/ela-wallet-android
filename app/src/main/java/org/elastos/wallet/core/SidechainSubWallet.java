package org.elastos.wallet.core;

/**
 * ISidechainSubWallet jni
 */
public class SidechainSubWallet extends SubWallet {
    private long mInstance;

    public String CreateWithdrawTransaction(String fromAddress, long amount, String mainChainAddress, String memo, String remark) throws WalletException {
        return CreateWithdrawTransaction(mInstance, fromAddress, amount, mainChainAddress, memo, remark);
    }

    public String GetGenesisAddress() throws WalletException {
        return GetGenesisAddress(mInstance);
    }

    public SidechainSubWallet(long proxy) {
        super(proxy);
        mInstance = proxy;
    }

    private native String CreateWithdrawTransaction(long proxy, String fromAddress, long amount, String mainChainAddress, String memo, String remark);

    private native String GetGenesisAddress(long proxy);
}
