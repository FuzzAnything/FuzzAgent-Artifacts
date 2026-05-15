#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <openssl/aes.h>
}
extern "C" {
#include <openssl/asn1.h>
}
extern "C" {
#include <openssl/asn1err.h>
}
extern "C" {
#include <openssl/asn1t.h>
}
extern "C" {
#include <openssl/async.h>
}
extern "C" {
#include <openssl/asyncerr.h>
}
extern "C" {
#include <openssl/bio.h>
}
extern "C" {
#include <openssl/bioerr.h>
}
extern "C" {
#include <openssl/blowfish.h>
}
extern "C" {
#include <openssl/bn.h>
}
extern "C" {
#include <openssl/bnerr.h>
}
extern "C" {
#include <openssl/buffer.h>
}
extern "C" {
#include <openssl/buffererr.h>
}
extern "C" {
#include <openssl/byteorder.h>
}
extern "C" {
#include <openssl/camellia.h>
}
extern "C" {
#include <openssl/cast.h>
}
extern "C" {
#include <openssl/cmac.h>
}
extern "C" {
#include <openssl/cmp.h>
}
extern "C" {
#include <openssl/cmp_util.h>
}
extern "C" {
#include <openssl/cmperr.h>
}
extern "C" {
#include <openssl/cms.h>
}
extern "C" {
#include <openssl/cmserr.h>
}
extern "C" {
#include <openssl/comp.h>
}
extern "C" {
#include <openssl/comperr.h>
}
extern "C" {
#include <openssl/conf.h>
}
extern "C" {
#include <openssl/conf_api.h>
}
extern "C" {
#include <openssl/conferr.h>
}
extern "C" {
#include <openssl/configuration.h>
}
extern "C" {
#include <openssl/conftypes.h>
}
extern "C" {
#include <openssl/core.h>
}
extern "C" {
#include <openssl/core_dispatch.h>
}
extern "C" {
#include <openssl/core_names.h>
}
extern "C" {
#include <openssl/core_object.h>
}
extern "C" {
#include <openssl/crmf.h>
}
extern "C" {
#include <openssl/crmferr.h>
}
extern "C" {
#include <openssl/crypto.h>
}
extern "C" {
#include <openssl/cryptoerr.h>
}
extern "C" {
#include <openssl/cryptoerr_legacy.h>
}
extern "C" {
#include <openssl/ct.h>
}
extern "C" {
#include <openssl/cterr.h>
}
extern "C" {
#include <openssl/decoder.h>
}
extern "C" {
#include <openssl/decodererr.h>
}
extern "C" {
#include <openssl/des.h>
}
extern "C" {
#include <openssl/dh.h>
}
extern "C" {
#include <openssl/dherr.h>
}
extern "C" {
#include <openssl/dsa.h>
}
extern "C" {
#include <openssl/dsaerr.h>
}
extern "C" {
#include <openssl/dtls1.h>
}
extern "C" {
#include <openssl/e_os2.h>
}
extern "C" {
#include <openssl/e_ostime.h>
}
extern "C" {
#include <openssl/ebcdic.h>
}
extern "C" {
#include <openssl/ec.h>
}
extern "C" {
#include <openssl/ecdh.h>
}
extern "C" {
#include <openssl/ecdsa.h>
}
extern "C" {
#include <openssl/ecerr.h>
}
extern "C" {
#include <openssl/ech.h>
}
extern "C" {
#include <openssl/encoder.h>
}
extern "C" {
#include <openssl/encodererr.h>
}
extern "C" {
#include <openssl/engine.h>
}
extern "C" {
#include <openssl/engineerr.h>
}
extern "C" {
#include <openssl/err.h>
}
extern "C" {
#include <openssl/ess.h>
}
extern "C" {
#include <openssl/esserr.h>
}
extern "C" {
#include <openssl/evp.h>
}
extern "C" {
#include <openssl/evperr.h>
}
extern "C" {
#include <openssl/fips_names.h>
}
extern "C" {
#include <openssl/fipskey.h>
}
extern "C" {
#include <openssl/hmac.h>
}
extern "C" {
#include <openssl/hpke.h>
}
extern "C" {
#include <openssl/http.h>
}
extern "C" {
#include <openssl/httperr.h>
}
extern "C" {
#include <openssl/idea.h>
}
extern "C" {
#include <openssl/indicator.h>
}
extern "C" {
#include <openssl/kdf.h>
}
extern "C" {
#include <openssl/kdferr.h>
}
extern "C" {
#include <openssl/lhash.h>
}
extern "C" {
#include <openssl/macros.h>
}
extern "C" {
#include <openssl/md2.h>
}
extern "C" {
#include <openssl/md4.h>
}
extern "C" {
#include <openssl/md5.h>
}
extern "C" {
#include <openssl/mdc2.h>
}
extern "C" {
#include <openssl/ml_kem.h>
}
extern "C" {
#include <openssl/modes.h>
}
extern "C" {
#include <openssl/obj_mac.h>
}
extern "C" {
#include <openssl/objects.h>
}
extern "C" {
#include <openssl/objectserr.h>
}
extern "C" {
#include <openssl/ocsp.h>
}
extern "C" {
#include <openssl/ocsperr.h>
}
extern "C" {
#include <openssl/opensslconf.h>
}
extern "C" {
#include <openssl/opensslv.h>
}
extern "C" {
#include <openssl/ossl_typ.h>
}
extern "C" {
#include <openssl/param_build.h>
}
extern "C" {
#include <openssl/params.h>
}
extern "C" {
#include <openssl/pem.h>
}
extern "C" {
#include <openssl/pem2.h>
}
extern "C" {
#include <openssl/pemerr.h>
}
extern "C" {
#include <openssl/pkcs12.h>
}
extern "C" {
#include <openssl/pkcs12err.h>
}
extern "C" {
#include <openssl/pkcs7.h>
}
extern "C" {
#include <openssl/pkcs7err.h>
}
extern "C" {
#include <openssl/posix_time.h>
}
extern "C" {
#include <openssl/prov_ssl.h>
}
extern "C" {
#include <openssl/proverr.h>
}
extern "C" {
#include <openssl/provider.h>
}
extern "C" {
#include <openssl/quic.h>
}
extern "C" {
#include <openssl/rand.h>
}
extern "C" {
#include <openssl/randerr.h>
}
extern "C" {
#include <openssl/rc2.h>
}
extern "C" {
#include <openssl/rc4.h>
}
extern "C" {
#include <openssl/rc5.h>
}
extern "C" {
#include <openssl/ripemd.h>
}
extern "C" {
#include <openssl/rsa.h>
}
extern "C" {
#include <openssl/rsaerr.h>
}
extern "C" {
#include <openssl/safestack.h>
}
extern "C" {
#include <openssl/seed.h>
}
extern "C" {
#include <openssl/self_test.h>
}
extern "C" {
#include <openssl/sha.h>
}
extern "C" {
#include <openssl/srp.h>
}
extern "C" {
#include <openssl/srtp.h>
}
extern "C" {
#include <openssl/ssl.h>
}
extern "C" {
#include <openssl/ssl2.h>
}
extern "C" {
#include <openssl/ssl3.h>
}
extern "C" {
#include <openssl/sslerr.h>
}
extern "C" {
#include <openssl/sslerr_legacy.h>
}
extern "C" {
#include <openssl/stack.h>
}
extern "C" {
#include <openssl/store.h>
}
extern "C" {
#include <openssl/storeerr.h>
}
extern "C" {
#include <openssl/symhacks.h>
}
extern "C" {
#include <openssl/thread.h>
}
extern "C" {
#include <openssl/tls1.h>
}
extern "C" {
#include <openssl/trace.h>
}
extern "C" {
#include <openssl/ts.h>
}
extern "C" {
#include <openssl/tserr.h>
}
extern "C" {
#include <openssl/txt_db.h>
}
extern "C" {
#include <openssl/types.h>
}
extern "C" {
#include <openssl/ui.h>
}
extern "C" {
#include <openssl/uierr.h>
}
extern "C" {
#include <openssl/whrlpool.h>
}
extern "C" {
#include <openssl/x509.h>
}
extern "C" {
#include <openssl/x509_acert.h>
}
extern "C" {
#include <openssl/x509_vfy.h>
}
extern "C" {
#include <openssl/x509err.h>
}
extern "C" {
#include <openssl/x509v3.h>
}
extern "C" {
#include <openssl/x509v3err.h>
}
//<ID> 310
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"asn1_ex_c2i":[[935,5,935,30,0,0,4,0]]},"library_calls":["d2i_X509_fp","PEM_read_X509","X509_get0_pubkey","EVP_PKEY_up_ref","DSA_new","EVP_PKEY_new","EVP_PKEY_set1_DSA","TLS_method","SSL_CTX_new","SSL_new","SSL_set_alpn_protos","SSL_get0_alpn_selected","X509_STORE_new","X509_STORE_set_check_revocation","ASN1_TIME_new","X509_set1_notBefore","ASN1_TIME_free","SHA512_Update","EVP_aria_256_cbc","d2i_TS_ACCURACY","OSSL_PARAM_construct_ulong","OSSL_PARAM_construct_end","OSSL_PARAM_get_ulong","ASYNC_WAIT_CTX_new","ASYNC_WAIT_CTX_set_callback","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","EVP_EncryptFinal_ex","EVP_CIPHER_CTX_free","ASYNC_WAIT_CTX_free","TS_ACCURACY_free","X509_STORE_free","SSL_free","SSL_CTX_free","EVP_PKEY_free","DSA_free","EVP_PKEY_free","X509_free"],"critical_calls":["d2i_X509_fp","PEM_read_X509","X509_get0_pubkey","EVP_PKEY_up_ref","DSA_new","EVP_PKEY_new","EVP_PKEY_set1_DSA","TLS_method","SSL_CTX_new","SSL_new","SSL_set_alpn_protos","SSL_get0_alpn_selected","X509_STORE_new","X509_STORE_set_check_revocation","ASN1_TIME_new","X509_set1_notBefore","ASN1_TIME_free","SHA512_Update","EVP_aria_256_cbc","d2i_TS_ACCURACY","OSSL_PARAM_construct_ulong","OSSL_PARAM_construct_end","OSSL_PARAM_get_ulong","ASYNC_WAIT_CTX_new","ASYNC_WAIT_CTX_set_callback","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","EVP_EncryptFinal_ex","EVP_CIPHER_CTX_free","ASYNC_WAIT_CTX_free","TS_ACCURACY_free","X509_STORE_free","SSL_free","SSL_CTX_free","EVP_PKEY_free","DSA_free","EVP_PKEY_free","X509_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Read X509 certificate from input
    X509 *x509 = NULL;
    // Try DER format first
    x509 = d2i_X509_fp(in_file, &x509);
    if (!x509) {
        // Try PEM format
        fseek(in_file, 0, SEEK_SET);
        x509 = PEM_read_X509(in_file, &x509, NULL, NULL);
    }
    
    // 2. Extract public key and manage references
    EVP_PKEY *pubkey = NULL;
    if (x509) {
        pubkey = X509_get0_pubkey(x509);
        if (pubkey) {
            EVP_PKEY_up_ref(pubkey);
        }
    }
    
    // 3. Create DSA key and set it to EVP_PKEY
    DSA *dsa = DSA_new();
    EVP_PKEY *dsa_pkey = EVP_PKEY_new();
    if (dsa && dsa_pkey) {
        EVP_PKEY_set1_DSA(dsa_pkey, dsa);
    }
    
    // 4. Create SSL context and SSL object
    const SSL_METHOD *method = TLS_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    SSL *ssl = NULL;
    if (ctx) {
        ssl = SSL_new(ctx);
    }
    
    // 5. Set and get ALPN protocols
    if (ssl) {
        const unsigned char protos[] = {8, 'h', 't', 't', 'p', '/', '1', '.', '1'};
        SSL_set_alpn_protos(ssl, protos, sizeof(protos));
        
        const unsigned char *selected;
        unsigned int selected_len;
        SSL_get0_alpn_selected(ssl, &selected, &selected_len);
    }
    
    // 6. Create X509 store and set revocation callback
    X509_STORE *store = X509_STORE_new();
    if (store) {
        X509_STORE_set_check_revocation(store, NULL);
    }
    
    // 7. Set certificate validity time (using current time as example)
    if (x509) {
        ASN1_TIME *notBefore = ASN1_TIME_new();
        if (notBefore) {
            X509_set1_notBefore(x509, notBefore);
            ASN1_TIME_free(notBefore);
        }
    }
    
    // 8. Use remaining data for various operations
    long current_pos = ftell(in_file);
    if (current_pos < 0) current_pos = 0;
    size_t remaining = size - (size_t)current_pos;
    const unsigned char *remaining_data = data + current_pos;
    
    // 9. SHA512 hash update
    SHA512_CTX sha512_ctx;
    // Initialize context (not in API list but needed)
    memset(&sha512_ctx, 0, sizeof(sha512_ctx));
    if (remaining > 0) {
        size_t hash_len = remaining > 64 ? 64 : remaining;
        SHA512_Update(&sha512_ctx, remaining_data, hash_len);
    }
    
    // 10. Get ARIA cipher
    const EVP_CIPHER *aria_cipher = EVP_aria_256_cbc();
    
    // 11. Parse TS_ACCURACY structure
    TS_ACCURACY *ts_accuracy = NULL;
    if (remaining > 0) {
        const unsigned char *ptr = remaining_data;
        long ts_len = (long)(remaining > 100 ? 100 : remaining);
        ts_accuracy = d2i_TS_ACCURACY(&ts_accuracy, &ptr, ts_len);
    }
    
    // 12. OSSL_PARAM operations
    unsigned long param_value = 0;
    OSSL_PARAM params[2];
    params[0] = OSSL_PARAM_construct_ulong("test_param", &param_value);
    params[1] = OSSL_PARAM_construct_end();
    
    if (remaining > sizeof(unsigned long)) {
        memcpy(&param_value, remaining_data, sizeof(unsigned long));
        OSSL_PARAM_get_ulong(&params[0], &param_value);
    }
    
    // 13. ASYNC operations
    ASYNC_WAIT_CTX *async_ctx = ASYNC_WAIT_CTX_new();
    if (async_ctx) {
        ASYNC_WAIT_CTX_set_callback(async_ctx, NULL, NULL);
    }
    
    // 14. Encryption with ARIA
    EVP_CIPHER_CTX *evp_ctx = EVP_CIPHER_CTX_new();
    if (evp_ctx && aria_cipher && remaining > 48) {
        // Use first 32 bytes as key, next 16 as IV
        const unsigned char *key = remaining_data;
        const unsigned char *iv = remaining_data + 32;
        size_t plaintext_len = remaining > 48 ? remaining - 48 : 0;
        const unsigned char *plaintext = remaining_data + 48;
        
        unsigned char ciphertext[256];
        int outlen1 = 0, outlen2 = 0;
        
        EVP_EncryptInit_ex(evp_ctx, aria_cipher, NULL, key, iv);
        EVP_EncryptFinal_ex(evp_ctx, ciphertext, &outlen2);
    }
    
    // Cleanup
    if (evp_ctx) EVP_CIPHER_CTX_free(evp_ctx);
    if (async_ctx) ASYNC_WAIT_CTX_free(async_ctx);
    if (ts_accuracy) TS_ACCURACY_free(ts_accuracy);
    if (store) X509_STORE_free(store);
    if (ssl) SSL_free(ssl);
    if (ctx) SSL_CTX_free(ctx);
    if (dsa_pkey) EVP_PKEY_free(dsa_pkey);
    if (dsa) DSA_free(dsa);
    if (pubkey) EVP_PKEY_free(pubkey);
    if (x509) X509_free(x509);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}