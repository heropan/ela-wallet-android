// Copyright (c) 2012-2019 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

package org.elastos.wallet.core;

/**
 * MainchainSubWallet jni
 */
public class MainchainSubWallet extends SubWallet {
    private long mMainchainProxy;

    public MainchainSubWallet(long proxy) {
        super(proxy);
        mMainchainProxy = proxy;
    }

    public String CreateDepositTransaction(String fromAddress, String lockedAddress, long amount, String sideChainAddress, String memo, String remark, boolean useVotedUTXO) throws WalletException {
        return CreateDepositTransaction(mMainchainProxy, fromAddress, lockedAddress, amount, sideChainAddress, memo, remark, useVotedUTXO);
    }

    public String GenerateProducerPayload(String publicKey, String nodePublicKey, String nickName, String url, String IPAddress, long location, String payPasswd) throws WalletException {
        return GenerateProducerPayload(mMainchainProxy, publicKey, nodePublicKey, nickName, url, IPAddress, location, payPasswd);
    }

    public String GenerateCancelProducerPayload(String publicKey, String payPasswd) throws WalletException {
        return GenerateCancelProducerPayload(mMainchainProxy, publicKey, payPasswd);
    }

    public String CreateRegisterProducerTransaction(String fromAddress, String payloadJson, long amount, String memo, String remark, boolean useVotedUTXO) throws WalletException {
        return CreateRegisterProducerTransaction(mMainchainProxy, fromAddress, payloadJson, amount, memo, remark, useVotedUTXO);
    }

    public String CreateUpdateProducerTransaction(String fromAddress, String payloadJson, String memo, String remark, boolean useVotedUTXO) throws WalletException {
        return CreateUpdateProducerTransaction(mMainchainProxy, fromAddress, payloadJson, memo, remark, useVotedUTXO);
    }

    public String CreateCancelProducerTransaction(String fromAddress, String payloadJson, String memo, String remark, boolean useVotedUTXO) throws WalletException {
        return CreateCancelProducerTransaction(mMainchainProxy, fromAddress, payloadJson, memo, remark, useVotedUTXO);
    }

    public String CreateRetrieveDepositTransaction(long amount, String memo, String remark) throws WalletException {
        return CreateRetrieveDepositTransaction(mMainchainProxy, amount, memo, remark);
    }

    public String GetPublicKeyForVote() throws WalletException {
        return GetPublicKeyForVote(mMainchainProxy);
    }

    public String CreateVoteProducerTransaction(String fromAddress, long stake, String publicKeys, String memo, String remark, boolean useVotedUTXO) throws WalletException {
        return CreateVoteProducerTransaction(mMainchainProxy, fromAddress, stake, publicKeys, memo, remark, useVotedUTXO);
    }

    public String GetVotedProducerList() throws WalletException {
        return GetVotedProducerList(mMainchainProxy);
    }

    public String GetRegisteredProducerInfo() throws WalletException {
        return GetRegisteredProducerInfo(mMainchainProxy);
    }


    private native String CreateDepositTransaction(long proxy, String fromAddress, String lockedAddress, long amount,
                                                   String sideChainAddress, String memo, String remark, boolean useVotedUTXO);

    private native String GenerateProducerPayload(long proxy, String publicKey, String nodePublicKey, String nickName,
                                                  String url, String IPAddress, long location, String payPasswd);

    private native String GenerateCancelProducerPayload(long proxy, String publicKey, String payPasswd);

    private native String CreateRegisterProducerTransaction(long proxy, String fromAddress, String payloadJson, long amount,
                                                            String memo, String remark, boolean useVotedUTXO);

    private native String CreateUpdateProducerTransaction(long proxy, String fromAddress, String payloadJson, String memo, String remark, boolean useVotedUTXO);

    private native String CreateCancelProducerTransaction(long proxy, String fromAddress, String payloadJson, String memo, String remark, boolean useVotedUTXO);

    private native String CreateRetrieveDepositTransaction(long proxy, long amount, String memo, String remark);

    private native String GetPublicKeyForVote(long proxy);

    private native String CreateVoteProducerTransaction(long proxy, String fromAddress, long stake, String publicKeys, String memo, String remark, boolean useVotedUTXO);

    private native String GetVotedProducerList(long proxy);

    private native String GetRegisteredProducerInfo(long proxy);

}
