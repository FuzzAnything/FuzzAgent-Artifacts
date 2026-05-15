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
//<ID> 517
//<Prompt> []
/*<Combination>: [
*/
//<score> 77, nr_unique_branch: 21
//<Quality> {"density":14,"unique_branches":{"dh_cb":[[30,9,30,37,0,0,4,0],[30,9,30,37,0,0,4,1],[32,13,32,26,0,0,4,0],[35,16,35,45,0,0,4,0],[35,16,35,45,0,0,4,1],[39,16,39,45,0,0,4,0],[39,16,39,45,0,0,4,1]],"SCT_new":[[26,9,26,20,0,0,4,1]],"EVP_CIPHER_CTX_reset":[[33,9,33,28,0,0,4,0]],"evp_md_init_internal":[[189,13,189,27,0,0,4,1]],"SCT_set0_log_id":[[80,9,80,39,0,0,4,1]],"SCT_free":[[36,9,36,20,0,0,4,1]],"ossl_ffc_numbers_to_dh_named_group":[[121,17,121,31,0,0,4,0],[121,17,121,31,0,0,4,1],[123,13,123,49,0,0,4,1]],"EVP_KDF_names_do_all":[[316,9,316,26,0,0,4,0]],"ossl_dh_cache_named_group":[[60,9,60,19,0,0,4,1],[66,9,66,29,0,0,4,1],[67,12,67,32,0,0,4,1],[70,9,73,16,0,0,4,1]],"EVP_CipherInit":[[403,9,403,23,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","ERR_clear_error","SSL_CTX_new","TLS_method","SSL_CTX_set_client_hello_cb","SSL_CTX_free","PEM_read_X509","OSSL_CMP_CTX_new","OSSL_CMP_CTX_set1_cert","OSSL_CMP_CTX_get1_extraCertsIn","OPENSSL_sk_free","ossl_check_X509_sk_type","OCSP_SINGLERESP_new","OCSP_SINGLERESP_get0_id","OCSP_SINGLERESP_free","OSSL_FUNC_keymgmt_import","EVP_KDF_fetch","EVP_KDF_names_do_all","EVP_KDF_free","SCT_new","CRYPTO_malloc","SCT_set0_log_id","SCT_free","BIO_new_fp","d2i_PKCS8_PRIV_KEY_INFO_bio","BIO_free","BIO_new_fp","ASN1_d2i_bio","BIO_free","CRYPTO_malloc","d2i_X509_ATTRIBUTE","X509_ATTRIBUTE_free","CRYPTO_free","EVP_CIPHER_CTX_new","EVP_aes_128_cbc","EVP_CipherInit","EVP_CipherUpdate","EVP_CIPHER_CTX_free","EVP_MD_CTX_new","EVP_sha256","EVP_DigestInit","EVP_DigestUpdate","EVP_DigestFinal","EVP_MD_CTX_free","X509_free","OSSL_CMP_CTX_free","PKCS8_PRIV_KEY_INFO_free","DH_free","SSL_CTX_free","ERR_clear_error"],"critical_calls":["OPENSSL_init_ssl","ERR_clear_error","SSL_CTX_new","TLS_method","SSL_CTX_set_client_hello_cb","PEM_read_X509","OSSL_CMP_CTX_new","OSSL_CMP_CTX_set1_cert","OSSL_CMP_CTX_get1_extraCertsIn","OPENSSL_sk_free","ossl_check_X509_sk_type","OCSP_SINGLERESP_new","OCSP_SINGLERESP_get0_id","OCSP_SINGLERESP_free","OSSL_FUNC_keymgmt_import","EVP_KDF_fetch","EVP_KDF_names_do_all","EVP_KDF_free","SCT_new","CRYPTO_malloc","SCT_set0_log_id","SCT_free","BIO_new_fp","d2i_PKCS8_PRIV_KEY_INFO_bio","BIO_free","BIO_new_fp","ASN1_d2i_bio","BIO_free","CRYPTO_malloc","d2i_X509_ATTRIBUTE","X509_ATTRIBUTE_free","CRYPTO_free","EVP_CIPHER_CTX_new","EVP_aes_128_cbc","EVP_CipherInit","EVP_CipherUpdate","EVP_CIPHER_CTX_free","EVP_MD_CTX_new","EVP_sha256","EVP_DigestInit","EVP_DigestUpdate","EVP_DigestFinal","EVP_MD_CTX_free","X509_free","OSSL_CMP_CTX_free","PKCS8_PRIV_KEY_INFO_free","DH_free","SSL_CTX_free","ERR_clear_error"],"visited":1}
/*Looking at the APIs, I need to create a fuzz driver that uses them together meaningfully. The APIs suggest working with SSL/TLS, OCSP responses, key management, certificate management, and SCT (Signed Certificate Timestamp) operations. Let me create a fuzz driver that processes a certificate chain with OCSP and SCT data.

*/


// Dummy callback function for SSL_CTX_set_client_hello_cb
static int  id_000045_client_hello_cb(SSL *s, int *al, void *arg) {
    (void)s; (void)al; (void)arg;
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Initialize OpenSSL
    OPENSSL_init_ssl(0, NULL);
    ERR_clear_error();
    
    // 1. Create SSL_CTX and set client hello callback
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    if (!ssl_ctx) {
	return 0;
	}
	
    
    SSL_CTX_set_client_hello_cb(ssl_ctx,  id_000045_client_hello_cb, NULL);
    
    // 2. Create a file from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        SSL_CTX_free(ssl_ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Try to read an X509 certificate from the input
    X509 *cert = PEM_read_X509(in_file, NULL, NULL, NULL);
    rewind(in_file);
    
    // 4. Create OSSL_CMP_CTX and use certificate
    OSSL_CMP_CTX *cmp_ctx = OSSL_CMP_CTX_new(NULL, NULL);
    if (cmp_ctx && cert) {
        // Set certificate in CMP context
        OSSL_CMP_CTX_set1_cert(cmp_ctx, cert);
        
        // Get extra certificates (returns stack)
        struct stack_st_X509 *extra_certs = OSSL_CMP_CTX_get1_extraCertsIn(cmp_ctx);
        if (extra_certs) {
            sk_X509_free(extra_certs);
        }
    }
    
    // 5. Create a dummy OCSP_SINGLERESP and get its ID
    OCSP_SINGLERESP *single_resp = OCSP_SINGLERESP_new();
    if (single_resp) {
        const OCSP_CERTID *cert_id = OCSP_SINGLERESP_get0_id(single_resp);
        (void)cert_id; // Use cert_id to avoid unused variable warning
        OCSP_SINGLERESP_free(single_resp);
    }
    
    // 6. Create a dummy OSSL_DISPATCH structure and get keymgmt_import function
    // We'll create a minimal dispatch table
    OSSL_DISPATCH dispatch_table[2] = {{0, NULL}};
    OSSL_FUNC_keymgmt_import_fn *import_fn = OSSL_FUNC_keymgmt_import(dispatch_table);
    (void)import_fn; // Use import_fn to avoid unused variable warning
    
    // 7. Create EVP_KDF and iterate over names
    EVP_KDF *kdf = EVP_KDF_fetch(NULL, "TLS13-KDF", NULL);
    if (kdf) {
        // Create callback function for names iteration
        auto name_callback = [](const char *name, void *data) {
            (void)name; (void)data;
            // Just consume the name
        };
        
        EVP_KDF_names_do_all(kdf, name_callback, NULL);
        EVP_KDF_free(kdf);
    }
    
    // 8. Create SCT and set log ID using input data
    SCT *sct = SCT_new();
    if (sct) {
        // Use part of input data as log ID (ensure we don't overflow)
        size_t log_id_len = size > 32 ? 32 : size;
        unsigned char *log_id = (unsigned char *)OPENSSL_malloc(log_id_len);
        if (log_id) {
            memcpy(log_id, data, log_id_len);
            SCT_set0_log_id(sct, log_id, log_id_len);
            // Note: SCT_set0_log_id takes ownership of log_id, no need to free
        }
        SCT_free(sct);
    }
    
    // 9. Try to parse more data from input file
    // Read PKCS8 private key info
    PKCS8_PRIV_KEY_INFO *p8inf = NULL;
    BIO *bio = BIO_new_fp(in_file, BIO_NOCLOSE);
    if (bio) {
        p8inf = d2i_PKCS8_PRIV_KEY_INFO_bio(bio, NULL);
        BIO_free(bio);
    }
    
    // 10. Try to parse DH parameters
    DH *dh = NULL;
    rewind(in_file);
    bio = BIO_new_fp(in_file, BIO_NOCLOSE);
    if (bio) {
        dh = d2i_DHparams_bio(bio, NULL);
        BIO_free(bio);
    }
    
    // 11. Try to parse X509 attribute
    rewind(in_file);
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    rewind(in_file);
    
    if (file_size > 0) {
        unsigned char *file_data = (unsigned char *)OPENSSL_malloc(file_size);
        if (file_data) {
            fread(file_data, 1, file_size, in_file);
            const unsigned char *p = file_data;
            X509_ATTRIBUTE *attr = d2i_X509_ATTRIBUTE(NULL, &p, file_size);
            if (attr) {
                X509_ATTRIBUTE_free(attr);
            }
            OPENSSL_free(file_data);
        }
    }
    
    // 12. Try various encryption/decryption operations
    EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new();
    if (cipher_ctx) {
        // Use part of input as key and IV
        unsigned char key[EVP_MAX_KEY_LENGTH] = {0};
        unsigned char iv[EVP_MAX_IV_LENGTH] = {0};
        size_t key_len = size > EVP_MAX_KEY_LENGTH ? EVP_MAX_KEY_LENGTH : size;
        size_t iv_len = size > EVP_MAX_IV_LENGTH ? EVP_MAX_IV_LENGTH : size;
        
        memcpy(key, data, key_len);
        if (size > key_len) {
            memcpy(iv, data + key_len, iv_len);
        }
        
        const EVP_CIPHER *cipher = EVP_aes_128_cbc();
        EVP_CipherInit(cipher_ctx, cipher, key, iv, 1);
        
        // Try to encrypt some data
        unsigned char in[16] = {0};
        unsigned char out[32] = {0};
        int outlen = 0;
        
        if (size > key_len + iv_len + 16) {
            memcpy(in, data + key_len + iv_len, 16);
        }
        
        EVP_CipherUpdate(cipher_ctx, out, &outlen, in, sizeof(in));
        EVP_CIPHER_CTX_free(cipher_ctx);
    }
    
    // 13. Try digest operations
    if (size > 50) {
        unsigned char md[EVP_MAX_MD_SIZE];
        unsigned int md_len = 0;
        EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
        if (md_ctx) {
            const EVP_MD *md_type = EVP_sha256();
            EVP_DigestInit(md_ctx, md_type);
            EVP_DigestUpdate(md_ctx, data, size > 1000 ? 1000 : size);
            EVP_DigestFinal(md_ctx, md, &md_len);
            EVP_MD_CTX_free(md_ctx);
        }
    }
    
    // Cleanup
    if (cert) X509_free(cert);
    if (cmp_ctx) OSSL_CMP_CTX_free(cmp_ctx);
    if (p8inf) PKCS8_PRIV_KEY_INFO_free(p8inf);
    if (dh) DH_free(dh);
    
    assert_file_closed(&in_file);;
    SSL_CTX_free(ssl_ctx);
    
    // Clear any remaining errors
    ERR_clear_error();
    
    assert_file_closed(&in_file);
	return 0;
}