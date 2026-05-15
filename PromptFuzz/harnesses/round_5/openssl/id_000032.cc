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
//<ID> 305
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["OPENSSL_init_ssl","OSSL_IETF_ATTR_SYNTAX_VALUE_it","X509_STORE_CTX_new","X509_STORE_new","d2i_X509","X509_STORE_CTX_init","OSSL_STORE_LOADER_new","OSSL_STORE_LOADER_set_open_ex","OSSL_STORE_LOADER_free","SSL_CTX_new","TLS_method","SSL_new","BIO_new","BIO_s_mem","BIO_write","SSL_set_bio","BIO_free","SSL_set_cipher_list","SSL_shutdown_ex","d2i_X509_PUBKEY_fp","X509_PUBKEY_get","EVP_PKEY_get_id","EVP_rc4_hmac_md5","EVP_CIPHER_CTX_new","EVP_CIPHER_get_key_length","EVP_CIPHER_get_iv_length","EVP_EncryptInit_ex","EVP_EncryptUpdate","EVP_EncryptFinal_ex","EVP_CIPHER_CTX_free","EVP_PKEY_free","X509_PUBKEY_free","ossl_check_OSSL_CRMF_CERTID_compfunc_type","X509_free","X509_STORE_CTX_free","X509_STORE_free","SSL_free","SSL_CTX_free"],"critical_calls":["OPENSSL_init_ssl","OSSL_IETF_ATTR_SYNTAX_VALUE_it","X509_STORE_CTX_new","X509_STORE_new","d2i_X509","X509_STORE_CTX_init","OSSL_STORE_LOADER_new","OSSL_STORE_LOADER_set_open_ex","OSSL_STORE_LOADER_free","SSL_CTX_new","TLS_method","SSL_new","BIO_new","BIO_s_mem","BIO_write","SSL_set_bio","SSL_set_cipher_list","SSL_shutdown_ex","d2i_X509_PUBKEY_fp","X509_PUBKEY_get","EVP_PKEY_get_id","EVP_rc4_hmac_md5","EVP_CIPHER_CTX_new","EVP_CIPHER_get_key_length","EVP_CIPHER_get_iv_length","EVP_EncryptInit_ex","EVP_EncryptUpdate","EVP_EncryptFinal_ex","EVP_CIPHER_CTX_free","EVP_PKEY_free","X509_PUBKEY_free","ossl_check_OSSL_CRMF_CERTID_compfunc_type","X509_free","X509_STORE_CTX_free","X509_STORE_free","SSL_free","SSL_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 256) {
        return 0; // Need minimum data
    }

    // 1. Initialize OpenSSL
    OPENSSL_init_ssl(0, nullptr);
    const ASN1_ITEM *item = OSSL_IETF_ATTR_SYNTAX_VALUE_it();
    (void)item; // Use to avoid unused warning

    // 2. Create X509 store context and store
    X509_STORE_CTX *store_ctx = X509_STORE_CTX_new();
    X509_STORE *store = X509_STORE_new();
    
    // 3. Create dummy X509 certificate from input data
    // Use first 128 bytes as certificate data
    const unsigned char *cert_data = data;
    long cert_len = size > 128 ? 128 : size;
    X509 *cert = d2i_X509(nullptr, &cert_data, cert_len);
    
    if (cert) {
        // Initialize store context with certificate
        X509_STORE_CTX_init(store_ctx, store, cert, nullptr);
    }
    
    // 4. Create OSSL_STORE_LOADER
    OSSL_STORE_LOADER *loader = OSSL_STORE_LOADER_new(nullptr, "file");
    if (loader) {
        // Set open_ex function (dummy implementation)
        OSSL_STORE_LOADER_set_open_ex(loader, 
            [](const OSSL_STORE_LOADER *, const char *, OSSL_LIB_CTX *, 
               const char *, const UI_METHOD *, void *) -> OSSL_STORE_LOADER_CTX* {
                return nullptr;
            });
        OSSL_STORE_LOADER_free(loader);
    }

    // 5. Create SSL context and SSL object
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = SSL_new(ssl_ctx);
    
    // 6. Create memory BIO and write input data to it
    BIO *bio = BIO_new(BIO_s_mem());
    if (bio && size > 0) {
        BIO_write(bio, data, size);
        // Set BIO for SSL
        SSL_set_bio(ssl, bio, bio);
    } else {
        if (bio) BIO_free(bio);
    }
    
    // 7. Set cipher list
    SSL_set_cipher_list(ssl, "RC4-HMAC-MD5:ALL");
    
    // 8. Prepare SSL shutdown with extended arguments
    SSL_SHUTDOWN_EX_ARGS shutdown_args = {0};
    shutdown_args.quic_error_code = 0;
    shutdown_args.quic_reason = "fuzz_test";
    SSL_shutdown_ex(ssl, 0, &shutdown_args, sizeof(shutdown_args));
    
    // 9. Read public key from file pointer (using input data)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        X509_PUBKEY *pubkey = nullptr;
        pubkey = d2i_X509_PUBKEY_fp(in_file, &pubkey);
        
        if (pubkey) {
            // Get EVP_PKEY from X509_PUBKEY
            EVP_PKEY *pkey = X509_PUBKEY_get(pubkey);
            if (pkey) {
                // Get key ID
                int key_id = EVP_PKEY_get_id(pkey);
                (void)key_id; // Use to avoid warning
                
                // 10. Perform encryption using RC4-HMAC-MD5
                const EVP_CIPHER *cipher = EVP_rc4_hmac_md5();
                EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new();
                
                if (cipher_ctx) {
                    int key_len = EVP_CIPHER_get_key_length(cipher);
                    int iv_len = EVP_CIPHER_get_iv_length(cipher);
                    
                    // Extract key and IV from input data
                    // Use different offsets to avoid overlap
                    size_t key_offset = size > 128 ? 128 : 0;
                    size_t iv_offset = size > 192 ? 192 : 0;
                    
                    const unsigned char *key = data + key_offset;
                    const unsigned char *iv = data + iv_offset;
                    
                    // Ensure we have enough data for key and IV
                    if (size - key_offset >= (size_t)key_len && 
                        size - iv_offset >= (size_t)iv_len) {
                        
                        // Initialize encryption
                        EVP_EncryptInit_ex(cipher_ctx, cipher, nullptr, key, iv);
                        
                        // Encrypt some data (use remaining input)
                        size_t encrypt_len = size > 256 ? size - 256 : 0;
                        if (encrypt_len > 0) {
                            unsigned char *out_buf = (unsigned char *)malloc(encrypt_len + EVP_MAX_BLOCK_LENGTH);
                            int out_len1 = 0, out_len2 = 0;
                            
                            if (out_buf) {
                                const unsigned char *in_data = data + 256;
                                EVP_EncryptUpdate(cipher_ctx, out_buf, &out_len1, 
                                                 in_data, encrypt_len);
                                EVP_EncryptFinal_ex(cipher_ctx, out_buf + out_len1, &out_len2);
                                free(out_buf);
                            }
                        }
                    }
                    
                    EVP_CIPHER_CTX_free(cipher_ctx);
                }
                
                EVP_PKEY_free(pkey);
            }
            
            X509_PUBKEY_free(pubkey);
        }
        
        assert_file_closed(&in_file);;
    }
    
    // 11. Use comparison function (dummy usage)
    sk_OSSL_CRMF_CERTID_compfunc cmp_func = nullptr;
    OPENSSL_sk_compfunc casted_cmp = ossl_check_OSSL_CRMF_CERTID_compfunc_type(cmp_func);
    (void)casted_cmp; // Use to avoid warning
    
    // 12. Cleanup
    if (cert) X509_free(cert);
    if (store_ctx) X509_STORE_CTX_free(store_ctx);
    if (store) X509_STORE_free(store);
    if (ssl) SSL_free(ssl);
    if (ssl_ctx) SSL_CTX_free(ssl_ctx);
    
    assert_file_closed(&in_file);
	return 0;
}