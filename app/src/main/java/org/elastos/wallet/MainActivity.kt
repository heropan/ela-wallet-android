package org.elastos.wallet

import android.content.res.Resources
import android.os.Build
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_main.*
import org.elastos.wallet.core.MasterWalletManager
import org.elastos.wallet.utils.Utils


class MainActivity : AppCompatActivity() {

    val TAG = MainActivity::class.java.name

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val rootPath = this.filesDir.absolutePath
        Utils.copyAssetsTo(this, rootPath)

        root_path.text = rootPath
        val masterWalletManager = MasterWalletManager(rootPath)

        val language = getLanguage()
        tv_language.text = language

        val mnemonic = masterWalletManager.GenerateMnemonic(language)
        tv_mnemonic.text = mnemonic
    }

    private fun getLanguage(): String {
        val locale = when {
            Build.VERSION.SDK_INT >= Build.VERSION_CODES.N ->
                Resources.getSystem().configuration.locales.get(0)

            else -> Resources.getSystem().configuration.locale
        }

        when (locale.language) {
            "en" -> return getString(R.string.ENGLISH)
            "zh" -> return getString(R.string.CHINESE)
            "fr" -> return getString(R.string.FRENCH)
            "it" -> return getString(R.string.ITALIAN)
            "ja" -> return getString(R.string.JAPANESE)
            "es" -> return getString(R.string.SPANISH)
        }
        return getString(R.string.ENGLISH)
    }


    companion object {

        // Used to load the 'wallet' library on application startup.
        init {
            System.loadLibrary("spvsdk_jni")
        }
    }
}
