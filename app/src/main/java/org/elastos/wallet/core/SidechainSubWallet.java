package org.elastos.wallet.core;

/**
 * ISidechainSubWallet jni
 */
public class SidechainSubWallet extends SubWallet {
    private long mInstance;

    public String CreateWithdrawTransaction(
            String fromAddress, String toAddress, long amount, String mainchainAccounts,
            String mainchainAmounts, String mainchainIndexs, String memo, String remark)
            throws WalletException {

        return CreateWithdrawTransaction(mInstance, fromAddress, toAddress, amount,
                mainchainAccounts, mainchainAmounts, mainchainIndexs, memo, remark);
    }

    public String GetGenesisAddress() throws WalletException {
        return GetGenesisAddress(mInstance);
    }

    public SidechainSubWallet(long instance) {
        super(instance);
        mInstance = instance;
    }

    private native String CreateWithdrawTransaction(
            long instance, String fromAddress, String toAddress, long amount, String mainchainAccounts,
            String mainchainAmounts, String mainchainIndexs, String memo, String remark);

    private native String GetGenesisAddress(long instance);
}
