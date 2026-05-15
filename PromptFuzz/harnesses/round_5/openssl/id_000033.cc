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
//<ID> 306
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 5
//<Quality> {"density":18,"unique_branches":{"ssl_cipher_process_rulestr":[[1110,17,1110,34,0,0,4,0]],"ssl_cipher_apply_rule":[[802,17,802,31,0,0,4,0],[802,35,802,56,0,0,4,0],[802,35,802,56,0,0,4,1],[815,20,815,74,0,0,4,1]]},"library_calls":["OPENSSL_init_ssl","X509_STORE_CTX_new","X509_STORE_new","X509_new","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","X509_STORE_CTX_init","d2i_X509_PUBKEY_fp","X509_PUBKEY_get","EVP_PKEY_get_id","SSL_CTX_new","TLS_method","SSL_new","BIO_new","BIO_s_mem","BIO_new","BIO_s_mem","SSL_set_bio","SSL_set_cipher_list","BIO_write","OSSL_STORE_LOADER_new","OSSL_STORE_LOADER_set_open_ex","EVP_rc4_hmac_md5","EVP_CIPHER_CTX_new","EVP_CIPHER_get_key_length","EVP_CIPHER_get_iv_length","EVP_EncryptInit_ex","EVP_EncryptUpdate","EVP_EncryptFinal_ex","SSL_shutdown_ex","OSSL_IETF_ATTR_SYNTAX_VALUE_it","ossl_check_OSSL_CRMF_CERTID_compfunc_type","EVP_CIPHER_CTX_free","OSSL_STORE_LOADER_free","SSL_free","SSL_CTX_free","EVP_PKEY_free","X509_PUBKEY_free","X509_STORE_CTX_free","X509_STORE_free","X509_free","OPENSSL_sk_free","ossl_check_X509_sk_type"],"critical_calls":["OPENSSL_init_ssl","X509_STORE_CTX_new","X509_STORE_new","X509_new","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","X509_STORE_CTX_init","d2i_X509_PUBKEY_fp","X509_PUBKEY_get","EVP_PKEY_get_id","SSL_CTX_new","TLS_method","SSL_new","BIO_new","BIO_s_mem","BIO_new","BIO_s_mem","SSL_set_bio","SSL_set_cipher_list","BIO_write","OSSL_STORE_LOADER_new","OSSL_STORE_LOADER_set_open_ex","EVP_rc4_hmac_md5","EVP_CIPHER_CTX_new","EVP_CIPHER_get_key_length","EVP_CIPHER_get_iv_length","EVP_EncryptInit_ex","EVP_EncryptUpdate","EVP_EncryptFinal_ex","SSL_shutdown_ex","OSSL_IETF_ATTR_SYNTAX_VALUE_it","ossl_check_OSSL_CRMF_CERTID_compfunc_type","EVP_CIPHER_CTX_free","OSSL_STORE_LOADER_free","SSL_free","SSL_CTX_free","EVP_PKEY_free","X509_PUBKEY_free","X509_STORE_CTX_free","X509_STORE_free","X509_free","OPENSSL_sk_free","ossl_check_X509_sk_type"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified OpenSSL APIs to achieve a comprehensive event:

*/


extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for operations
    
    // Initialize OpenSSL
    OPENSSL_init_ssl(0, NULL);
    
    // Create and initialize X509_STORE_CTX and X509_STORE
    X509_STORE_CTX *store_ctx = X509_STORE_CTX_new();
    X509_STORE *store = X509_STORE_new();
    
    // Create a dummy X509 certificate for context initialization
    X509 *cert = X509_new();
    struct stack_st_X509 *untrusted = sk_X509_new_null();
    
    if (store_ctx && store && cert && untrusted) {
        X509_STORE_CTX_init(store_ctx, store, cert, untrusted);
    }
    
    // Read X509_PUBKEY from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    X509_PUBKEY *pubkey = NULL;
    if (in_file) {
        pubkey = d2i_X509_PUBKEY_fp(in_file, &pubkey);
        assert_file_closed(&in_file);;
    }
    
    // Get EVP_PKEY from X509_PUBKEY
    EVP_PKEY *pkey = NULL;
    if (pubkey) {
        pkey = X509_PUBKEY_get(pubkey);
    }
    
    // Get key ID if we have a pkey
    int key_id = 0;
    if (pkey) {
        key_id = EVP_PKEY_get_id(pkey);
    }
    
    // Create SSL context and SSL object
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = SSL_new(ssl_ctx);
    
    // Set up BIO for SSL
    BIO *rbio = BIO_new(BIO_s_mem());
    BIO *wbio = BIO_new(BIO_s_mem());
    
    if (ssl && rbio && wbio) {
        SSL_set_bio(ssl, rbio, wbio);
        
        // Set cipher list
        SSL_set_cipher_list(ssl, "RC4-SHA:AES128-SHA");
        
        // Write some data to BIO for testing
        if (size > 0) {
            BIO_write(rbio, data, size > 1024 ? 1024 : size);
        }
    }
    
    // Create and use OSSL_STORE_LOADER
    OSSL_STORE_LOADER *loader = OSSL_STORE_LOADER_new(NULL, "file");
    if (loader) {
        // Set open_ex function (dummy implementation)
        OSSL_STORE_LOADER_set_open_ex(loader, [](const OSSL_STORE_LOADER *,
                                                 const char *,
                                                 OSSL_LIB_CTX *,
                                                 const char *,
                                                 const UI_METHOD *,
                                                 void *) -> OSSL_STORE_LOADER_CTX* {
            return NULL;
        });
    }
    
    // Create encryption context for RC4-HMAC-MD5
    const EVP_CIPHER *cipher = EVP_rc4_hmac_md5();
    EVP_CIPHER_CTX *enc_ctx = EVP_CIPHER_CTX_new();
    
    if (enc_ctx && cipher) {
        int key_len = EVP_CIPHER_get_key_length(cipher);
        int iv_len = EVP_CIPHER_get_iv_length(cipher);
        
        // Generate key and IV from input data
        unsigned char key[EVP_MAX_KEY_LENGTH];
        unsigned char iv[EVP_MAX_IV_LENGTH];
        
        // Use input data for key/IV, pad if necessary
        size_t key_offset = 0;
        size_t iv_offset = key_len % size;
        
        for (int i = 0; i < key_len; i++) {
            key[i] = data[(key_offset + i) % size];
        }
        for (int i = 0; i < iv_len; i++) {
            iv[i] = data[(iv_offset + i) % size];
        }
        
        // Initialize encryption
        EVP_EncryptInit_ex(enc_ctx, cipher, NULL, key, iv);
        
        // Perform encryption on part of input data
        unsigned char encrypted[1024];
        int out_len = 0;
        
        if (size > 50) {
            // Encrypt first 50 bytes
            EVP_EncryptUpdate(enc_ctx, encrypted, &out_len, 
                             data, size > 50 ? 50 : size);
            
            int final_len = 0;
            EVP_EncryptFinal_ex(enc_ctx, encrypted + out_len, &final_len);
        }
    }
    
    // Setup SSL shutdown with extended arguments
    if (ssl) {
        SSL_SHUTDOWN_EX_ARGS shutdown_args = {0};
        shutdown_args.quic_error_code = 0;
        shutdown_args.quic_reason = "fuzzing";
        
        SSL_shutdown_ex(ssl, 0, &shutdown_args, sizeof(shutdown_args));
    }
    
    // Get ASN1 item for attribute syntax
    const ASN1_ITEM *attr_item = OSSL_IETF_ATTR_SYNTAX_VALUE_it();
    (void)attr_item; // Use to avoid unused variable warning
    
    // Get comparison function for OSSL_CRMF_CERTID
    OPENSSL_sk_compfunc compfunc = ossl_check_OSSL_CRMF_CERTID_compfunc_type(
        [](const OSSL_CRMF_CERTID *const *a, const OSSL_CRMF_CERTID *const *b) -> int {
            return 0; // Dummy comparison
        });
    (void)compfunc; // Use to avoid unused variable warning
    
    // Cleanup - free all allocated resources
    if (enc_ctx) EVP_CIPHER_CTX_free(enc_ctx);
    if (loader) OSSL_STORE_LOADER_free(loader);
    if (ssl) SSL_free(ssl);
    if (ssl_ctx) SSL_CTX_free(ssl_ctx);
    if (pkey) EVP_PKEY_free(pkey);
    if (pubkey) X509_PUBKEY_free(pubkey);
    if (store_ctx) X509_STORE_CTX_free(store_ctx);
    if (store) X509_STORE_free(store);
    if (cert) X509_free(cert);
    if (untrusted) sk_X509_free(untrusted);
    
    assert_file_closed(&in_file);
	return 0;
}