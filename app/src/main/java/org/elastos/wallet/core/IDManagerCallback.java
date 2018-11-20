package org.elastos.wallet.core;

public interface IDManagerCallback {
    void OnIDStatusChanged(String id, String path, String value);
}
