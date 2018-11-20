package org.elastos.wallet.core;

public interface SubWalletCallback {
    /**
     * Callback method fired when status of a transaction changed.
     *
     * @param txID     indicate hash of the transaction.
     * @param status   can be "Added", "Deleted" or "Updated".
     * @param desc     is an detail description of transaction status.
     * @param confirms is confirm count util this callback fired.
     */
    void OnTransactionStatusChanged(String txID, String status, String desc, int confirms);

    /**
     * Callback method fired when block begin synchronizing with a peer. This callback could be used to show progress.
     */
    void OnBlockSyncStarted();

    /**
     * Callback method fired when best block chain height increased. This callback could be used to show progress.
     *
     * @param currentBlockHeight is the of current block when callback fired.
     * @param progress           is current progress when block height increased.
     */
    void OnBlockHeightIncreased(int currentBlockHeight, int progress);

    /**
     * Callback method fired when block end synchronizing with a peer. This callback could be used to show progress.
     */
    void OnBlockSyncStopped();

    void OnBalanceChanged(long balance);
}
