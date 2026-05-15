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
//<ID> 309
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 5
//<Quality> {"density":24,"unique_branches":{"ssl_set_new_record_layer":[[1347,9,1347,18,0,0,4,1],[1350,16,1350,26,0,0,4,0],[1350,30,1350,70,0,0,4,0],[1350,30,1350,70,0,0,4,1]],"ossl_ssl_connection_new_int":[[910,17,910,63,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","OSSL_IETF_ATTR_SYNTAX_VALUE_it","X509_STORE_new","X509_STORE_CTX_new","X509_STORE_CTX_init","BIO_new","BIO_s_mem","BIO_write","d2i_X509_PUBKEY_fp","X509_PUBKEY_get","EVP_PKEY_get_id","EVP_rc4_hmac_md5","EVP_CIPHER_CTX_new","EVP_CIPHER_get_key_length","EVP_CIPHER_get_iv_length","EVP_EncryptInit_ex","EVP_EncryptUpdate","EVP_EncryptFinal_ex","SSL_CTX_new","TLS_client_method","SSL_new","SSL_set_cipher_list","BIO_new","BIO_s_mem","BIO_new","BIO_s_mem","BIO_write","SSL_set_bio","BIO_free","BIO_free","SSL_shutdown_ex","OSSL_STORE_LOADER_new","OSSL_STORE_LOADER_set_open_ex","ossl_check_OSSL_CRMF_CERTID_compfunc_type","EVP_CIPHER_CTX_free","EVP_PKEY_free","X509_PUBKEY_free","SSL_free","SSL_CTX_free","OSSL_STORE_LOADER_free","BIO_free","X509_STORE_CTX_free","X509_STORE_free"],"critical_calls":["OPENSSL_init_ssl","OSSL_IETF_ATTR_SYNTAX_VALUE_it","X509_STORE_new","X509_STORE_CTX_new","X509_STORE_CTX_init","BIO_new","BIO_s_mem","BIO_write","d2i_X509_PUBKEY_fp","X509_PUBKEY_get","EVP_PKEY_get_id","EVP_rc4_hmac_md5","EVP_CIPHER_CTX_new","EVP_CIPHER_get_key_length","EVP_CIPHER_get_iv_length","EVP_EncryptInit_ex","EVP_EncryptUpdate","EVP_EncryptFinal_ex","SSL_CTX_new","TLS_client_method","SSL_new","SSL_set_cipher_list","BIO_new","BIO_s_mem","BIO_new","BIO_s_mem","BIO_write","SSL_set_bio","SSL_shutdown_ex","OSSL_STORE_LOADER_new","OSSL_STORE_LOADER_set_open_ex","ossl_check_OSSL_CRMF_CERTID_compfunc_type","EVP_CIPHER_CTX_free","EVP_PKEY_free","X509_PUBKEY_free","SSL_free","SSL_CTX_free","OSSL_STORE_LOADER_free","BIO_free","X509_STORE_CTX_free","X509_STORE_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL library
    OPENSSL_init_ssl(0, NULL);
    
    // Get ASN1 item for OSSL_IETF_ATTR_SYNTAX_VALUE
    const ASN1_ITEM *asn1_item = OSSL_IETF_ATTR_SYNTAX_VALUE_it();
    (void)asn1_item; // Use to avoid unused variable warning
    
    // Create X509 store and context for certificate verification
    X509_STORE *store = X509_STORE_new();
    X509_STORE_CTX *store_ctx = X509_STORE_CTX_new();
    
    // Initialize store context (even without actual certificates)
    if (store && store_ctx) {
        X509_STORE_CTX_init(store_ctx, store, NULL, NULL);
    }
    
    // Create memory BIO from input data
    BIO *mem_bio = BIO_new(BIO_s_mem());
    if (mem_bio && size > 0) {
        BIO_write(mem_bio, data, (int)size);
    }
    
    // Create FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Try to read X509 public key from file
    X509_PUBKEY *pubkey = NULL;
    if (in_file) {
        pubkey = d2i_X509_PUBKEY_fp(in_file, &pubkey);
        assert_file_closed(&in_file);;
    }
    
    // Get EVP_PKEY from X509_PUBKEY if available
    EVP_PKEY *pkey = NULL;
    if (pubkey) {
        pkey = X509_PUBKEY_get(pubkey);
        if (pkey) {
            // Get key ID
            int key_id = EVP_PKEY_get_id(pkey);
            (void)key_id; // Use to avoid warning
        }
    }
    
    // Get RC4-HMAC-MD5 cipher
    const EVP_CIPHER *cipher = EVP_rc4_hmac_md5();
    
    // Create and use cipher context for encryption
    EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new();
    if (cipher_ctx && cipher && size >= 16) {
        int key_len = EVP_CIPHER_get_key_length(cipher);
        int iv_len = EVP_CIPHER_get_iv_length(cipher);
        
        // Use first part of data as key/iv if enough data
        unsigned char key[EVP_MAX_KEY_LENGTH] = {0};
        unsigned char iv[EVP_MAX_IV_LENGTH] = {0};
        
        size_t key_size = (size_t)key_len < size ? (size_t)key_len : size;
        size_t iv_size = (size_t)iv_len < (size - key_size) ? (size_t)iv_len : (size - key_size);
        
        if (key_size > 0) memcpy(key, data, key_size);
        if (iv_size > 0 && key_size + iv_size <= size) {
            memcpy(iv, data + key_size, iv_size);
        }
        
        unsigned char out_buf[256];
        int out_len = 0;
        
        // Try encryption if we have data
        if (size > key_size + iv_size) {
            size_t encrypt_size = size - (key_size + iv_size);
            if (encrypt_size > sizeof(out_buf)) {
                encrypt_size = sizeof(out_buf);
            }
            
            EVP_EncryptInit_ex(cipher_ctx, cipher, NULL, key, iv);
            EVP_EncryptUpdate(cipher_ctx, out_buf, &out_len, 
                            data + key_size + iv_size, (int)encrypt_size);
            int final_len = 0;
            EVP_EncryptFinal_ex(cipher_ctx, out_buf + out_len, &final_len);
        }
    }
    
    // Create SSL context and SSL object
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_client_method());
    SSL *ssl = SSL_new(ssl_ctx);
    
    if (ssl) {
        // Set cipher list
        SSL_set_cipher_list(ssl, "RC4-MD5");
        
        // Set BIO for SSL
        BIO *rbio = BIO_new(BIO_s_mem());
        BIO *wbio = BIO_new(BIO_s_mem());
        if (rbio && wbio) {
            // Write some data to read BIO
            if (size > 0) {
                BIO_write(rbio, data, (int)(size > 1024 ? 1024 : size));
            }
            SSL_set_bio(ssl, rbio, wbio);
        } else {
            if (rbio) BIO_free(rbio);
            if (wbio) BIO_free(wbio);
        }
        
        // Create SSL shutdown arguments
        SSL_SHUTDOWN_EX_ARGS shutdown_args = {0};
        shutdown_args.quic_error_code = 0;
        shutdown_args.quic_reason = "test";
        
        // Try SSL shutdown
        SSL_shutdown_ex(ssl, 0, &shutdown_args, sizeof(shutdown_args));
    }
    
    // Create OSSL_STORE_LOADER
    OSSL_STORE_LOADER *loader = OSSL_STORE_LOADER_new(NULL, "file");
    if (loader) {
        // Set open_ex function (dummy implementation)
        OSSL_STORE_LOADER_set_open_ex(loader, 
            [](const OSSL_STORE_LOADER *loader, const char *uri, 
               OSSL_LIB_CTX *libctx, const char *propq, 
               const UI_METHOD *ui_method, void *ui_data) -> OSSL_STORE_LOADER_CTX* {
                return nullptr;
            });
    }
    
    // Use OSSL_CRMF_CERTID comparison function type
    sk_OSSL_CRMF_CERTID_compfunc cmp_func = nullptr;
    OPENSSL_sk_compfunc checked_cmp = ossl_check_OSSL_CRMF_CERTID_compfunc_type(cmp_func);
    (void)checked_cmp; // Use to avoid warning
    
    // Cleanup
    if (cipher_ctx) EVP_CIPHER_CTX_free(cipher_ctx);
    if (pkey) EVP_PKEY_free(pkey);
    if (pubkey) X509_PUBKEY_free(pubkey);
    if (ssl) SSL_free(ssl);
    if (ssl_ctx) SSL_CTX_free(ssl_ctx);
    if (loader) OSSL_STORE_LOADER_free(loader);
    if (mem_bio) BIO_free(mem_bio);
    if (store_ctx) X509_STORE_CTX_free(store_ctx);
    if (store) X509_STORE_free(store);
    
    assert_file_closed(&in_file);
	return 0;
}