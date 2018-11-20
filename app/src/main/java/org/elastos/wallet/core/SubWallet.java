
package org.elastos.wallet.core;

/**
 * SubWallet
 */
public class SubWallet {
    private long mInstance;

    public String GetChainId() throws WalletException {
        return GetChainId(mInstance);
    }

    public String GetBasicInfo() throws WalletException {
        return GetBasicInfo(mInstance);
    }

    public String GetBalanceInfo() throws WalletException {
        return GetBalanceInfo(mInstance);
    }

    public long GetBalance() throws WalletException {
        return GetBalance(mInstance);
    }

    public String CreateAddress() throws WalletException {
        return CreateAddress(mInstance);
    }

    public String GetAllAddress(int start, int count) throws WalletException {
        return GetAllAddress(mInstance, start, count);
    }

    public long GetBalanceWithAddress(String address) throws WalletException {
        return GetBalanceWithAddress(mInstance, address);
    }

    public void AddCallback(SubWalletCallback subCallback) throws WalletException {
        AddCallback(mInstance, subCallback);
    }

    public void RemoveCallback() throws WalletException {
        RemoveCallback(mInstance);
    }

    public String CreateTransaction(String fromAddress, String toAddress, long amount, String memo, String remark) throws WalletException {
        return CreateTransaction(mInstance, fromAddress, toAddress, amount, memo, remark);
    }

    public String CreateMultiSignTransaction(String fromAddress, String toAddress, long amount, String memo) throws WalletException {
        return CreateMultiSignTransaction(mInstance, fromAddress, toAddress, amount, memo);
    }

    public long CalculateTransactionFee(String rawTransaction, long feePerKb) throws WalletException {
        return CalculateTransactionFee(mInstance, rawTransaction, feePerKb);
    }

    public String UpdateTransactionFee(String rawTransaction, long fee) throws WalletException {
        return UpdateTransactionFee(mInstance, rawTransaction, fee);
    }

    public String SignTransaction(String rawTransaction, String payPassword) throws WalletException {
        return SignTransaction(mInstance, rawTransaction, payPassword);
    }

    public String GetTransactionSignedSigners(String rawTransaction) throws WalletException {
        return GetTransactionSignedSigners(mInstance, rawTransaction);
    }

    public String PublishTransaction(String rawTransaction) throws WalletException {
        return PublishTransaction(mInstance, rawTransaction);
    }

    public String GetAllTransaction(int start, int count, String addressOrTxId) throws WalletException {
        return GetAllTransaction(mInstance, start, count, addressOrTxId);
    }

    public String Sign(String message, String payPassword) throws WalletException {
        return Sign(mInstance, message, payPassword);
    }

    public String CheckSign(String publicKey, String message, String signature) throws WalletException {
        return CheckSign(mInstance, publicKey, message, signature);
    }

    public String GetPublicKey() throws WalletException {
        return GetPublicKey(mInstance);
    }

    public SubWallet(long instance) {
        mInstance = instance;
    }

    protected long getProxy() {
        return mInstance;
    }

    private native String GetChainId(long subProxy);

    private native String GetBasicInfo(long subProxy);

    private native String GetBalanceInfo(long subProxy);

    private native long GetBalance(long subProxy);

    private native String CreateAddress(long subProxy);

    private native String GetAllAddress(long subProxy, int start, int count);

    private native long GetBalanceWithAddress(long subProxy, String address);

    private native void AddCallback(long subProxy, SubWalletCallback subCallback);

    private native void RemoveCallback(long subProxy);

    private native String CreateTransaction(long subProxy, String fromAddress, String toAddress, long amount, String memo, String remark);

    private native String CreateMultiSignTransaction(long subProxy, String fromAddress, String toAddress, long amount, String memo);

    private native long CalculateTransactionFee(long subProxy, String rawTransaction, long feePerKb);

    private native String UpdateTransactionFee(long subProxy, String rawTransaction, long fee);

    private native String SignTransaction(long subProxy, String rawTransaction, String payPassword);

    private native String GetTransactionSignedSigners(long subProxy, String rawTransaction);

    private native String PublishTransaction(long subProxy, String rawTransaction);

    private native String GetAllTransaction(long subProxy, int start, int count, String addressOrTxId);

    private native String Sign(long subProxy, String message, String payPassword);

    private native String CheckSign(long subProxy, String publicKey, String message, String signature);

    private native String GetPublicKey(long jSubProxy);
}
