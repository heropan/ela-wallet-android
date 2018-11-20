package org.elastos.wallet.core;

/**
 * MainchainSubWallet jni
 */
public class MainchainSubWallet extends SubWallet {
    private long mMainchainProxy;


    public String CreateDepositTransaction(
            String fromAddress, String toAddress, long amount, String sidechainAccounts,
            String sidechainAmounts, String sidechainIndexs, String memo, String remark)
            throws WalletException {

        return CreateDepositTransaction(mMainchainProxy, fromAddress, toAddress, amount,
                sidechainAccounts, sidechainAmounts, sidechainIndexs, memo, remark);
    }

    public MainchainSubWallet(long instance) {
        super(instance);
        mMainchainProxy = instance;
    }

    private native String CreateDepositTransaction(
            long instance, String fromAddress, String toAddress, long amount,
            String sidechainAccounts, String sidechainAmounts, String sidechainIndexs,
            String memo, String remark);
}
