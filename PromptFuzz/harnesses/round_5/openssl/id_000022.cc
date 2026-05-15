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
//<ID> 230
//<Prompt> []
/*<Combination>: [
*/
//<score> 11.5, nr_unique_branch: 0
//<Quality> {"density":23,"unique_branches":{},"library_calls":["OPENSSL_init_ssl","OSSL_IETF_ATTR_SYNTAX_VALUE_it","X509_STORE_CTX_new","X509_STORE_new","X509_STORE_CTX_init","X509_STORE_CTX_free","X509_STORE_free","d2i_X509_PUBKEY_fp","EVP_rc4_hmac_md5","ossl_check_OSSL_CRMF_CERTID_compfunc_type","SSL_CTX_new","TLS_method","SSL_new","BIO_new","BIO_s_mem","BIO_new","BIO_s_mem","BIO_write","SSL_set_bio","SSL_set_cipher_list","SSL_shutdown_ex","BIO_free","BIO_free","SSL_free","SSL_CTX_free","X509_PUBKEY_get","EVP_PKEY_get_id","EVP_CIPHER_CTX_new","EVP_CIPHER_get_key_length","EVP_CIPHER_get_iv_length","EVP_EncryptInit_ex","EVP_EncryptUpdate","EVP_EncryptFinal_ex","EVP_CIPHER_CTX_free","EVP_PKEY_free","X509_PUBKEY_free","X509_PUBKEY_free"],"critical_calls":["OPENSSL_init_ssl","OSSL_IETF_ATTR_SYNTAX_VALUE_it","X509_STORE_CTX_new","X509_STORE_new","X509_STORE_CTX_init","X509_STORE_CTX_free","X509_STORE_free","d2i_X509_PUBKEY_fp","EVP_rc4_hmac_md5","ossl_check_OSSL_CRMF_CERTID_compfunc_type","SSL_CTX_new","TLS_method","SSL_new","BIO_new","BIO_s_mem","BIO_new","BIO_s_mem","BIO_write","SSL_set_bio","SSL_set_cipher_list","SSL_shutdown_ex","SSL_free","SSL_CTX_free","X509_PUBKEY_get","EVP_PKEY_get_id","EVP_CIPHER_CTX_new","EVP_CIPHER_get_key_length","EVP_CIPHER_get_iv_length","EVP_EncryptInit_ex","EVP_EncryptUpdate","EVP_EncryptFinal_ex","EVP_CIPHER_CTX_free","EVP_PKEY_free","X509_PUBKEY_free","X509_PUBKEY_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL
    OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS | OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    
    // 1. Get ASN1 item for IETF attribute syntax
    const ASN1_ITEM *attr_item = OSSL_IETF_ATTR_SYNTAX_VALUE_it();
    if (!attr_item) {
        // Continue anyway - this is fuzzing
    }
    
    // 2. Create X509_STORE_CTX and free it
    X509_STORE_CTX *store_ctx = X509_STORE_CTX_new();
    if (store_ctx) {
        // Initialize with dummy values for fuzzing
        X509_STORE *store = X509_STORE_new();
        if (store) {
            X509_STORE_CTX_init(store_ctx, store, NULL, NULL);
        }
        X509_STORE_CTX_free(store_ctx);
        if (store) X509_STORE_free(store);
    }
    
    // 3. Create FILE* from input data for d2i_X509_PUBKEY_fp
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0; // Cannot proceed without file
    }
    
    // 4. Try to read X509_PUBKEY from the file
    X509_PUBKEY *xpk = NULL;
    X509_PUBKEY *read_xpk = d2i_X509_PUBKEY_fp(in_file, &xpk);
    
    // Rewind file for potential reuse
    rewind(in_file);
    
    // 5. Get RC4-HMAC-MD5 cipher
    const EVP_CIPHER *rc4_hmac_md5 = EVP_rc4_hmac_md5();
    if (!rc4_hmac_md5) {
        // Cipher not available, but continue fuzzing
    }
    
    // 6. Get comparison function for OSSL_CRMF_CERTID stack
    OPENSSL_sk_compfunc cmp_func = ossl_check_OSSL_CRMF_CERTID_compfunc_type(
        (int (*)(const OSSL_CRMF_CERTID *const *, const OSSL_CRMF_CERTID *const *))NULL);
    // Note: We pass NULL as we don't have an actual comparison function
    
    // 7. Create SSL context and connection for SSL_shutdown_ex
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = NULL;
    
    if (ssl_ctx) {
        ssl = SSL_new(ssl_ctx);
        if (ssl) {
            // Create memory BIOs for SSL
            BIO *rbio = BIO_new(BIO_s_mem());
            BIO *wbio = BIO_new(BIO_s_mem());
            
            if (rbio && wbio) {
                // Write some data to read BIO to simulate received data
                if (size > 0) {
                    BIO_write(rbio, data, size < 1024 ? size : 1024);
                }
                
                SSL_set_bio(ssl, rbio, wbio);
                
                // Try to set cipher if available
                if (rc4_hmac_md5) {
                    SSL_set_cipher_list(ssl, "RC4-MD5");
                }
                
                // Create shutdown arguments
                SSL_SHUTDOWN_EX_ARGS shutdown_args = {0};
                shutdown_args.quic_error_code = 0;
                shutdown_args.quic_reason = "fuzzing";
                
                // Call SSL_shutdown_ex with minimal flags
                SSL_shutdown_ex(ssl, 0, &shutdown_args, sizeof(shutdown_args));
                
                // Don't free BIOs here - SSL_free will do it
            } else {
                if (rbio) BIO_free(rbio);
                if (wbio) BIO_free(wbio);
            }
            
            SSL_free(ssl);
        }
        SSL_CTX_free(ssl_ctx);
    }
    
    // 8. If we successfully read a X509_PUBKEY, try to use it
    if (read_xpk) {
        // Extract public key from X509_PUBKEY
        EVP_PKEY *pkey = X509_PUBKEY_get(read_xpk);
        if (pkey) {
            // Try to get key type
            int key_type = EVP_PKEY_id(pkey);
            
            // Try to encrypt something with RC4-HMAC-MD5 if available
            if (rc4_hmac_md5 && size > 0) {
                EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new();
                if (cipher_ctx) {
                    unsigned char key[EVP_MAX_KEY_LENGTH] = {0};
                    unsigned char iv[EVP_MAX_IV_LENGTH] = {0};
                    unsigned char out[1024];
                    int outlen = 0;
                    
                    // Use first bytes of data as key/iv (truncated to appropriate sizes)
                    size_t key_len = EVP_CIPHER_key_length(rc4_hmac_md5);
                    size_t iv_len = EVP_CIPHER_iv_length(rc4_hmac_md5);
                    
                    if (key_len > 0 && size >= key_len) {
                        memcpy(key, data, key_len);
                    }
                    if (iv_len > 0 && size >= key_len + iv_len) {
                        memcpy(iv, data + key_len, iv_len);
                    }
                    
                    // Try encryption
                    if (EVP_EncryptInit_ex(cipher_ctx, rc4_hmac_md5, NULL, key, iv)) {
                        size_t encrypt_size = size > 512 ? 512 : size;
                        if (EVP_EncryptUpdate(cipher_ctx, out, &outlen, 
                                             data + (size > 100 ? 100 : 0), 
                                             encrypt_size)) {
                            // Encryption succeeded
                        }
                        EVP_EncryptFinal_ex(cipher_ctx, out + outlen, &outlen);
                    }
                    
                    EVP_CIPHER_CTX_free(cipher_ctx);
                }
            }
            
            EVP_PKEY_free(pkey);
        }
        X509_PUBKEY_free(read_xpk);
    }
    
    // 9. Clean up file
    assert_file_closed(&in_file);;
    
    // 10. Additional cleanup
    if (xpk) {
        X509_PUBKEY_free(xpk);
    }
    
    assert_file_closed(&in_file);
	return 0;
}