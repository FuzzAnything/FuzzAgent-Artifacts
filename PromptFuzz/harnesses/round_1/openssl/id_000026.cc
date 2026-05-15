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
//<ID> 292
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 4
//<Quality> {"density":13,"unique_branches":{"SSL_set_bio":[[1663,25,1663,37,0,0,4,0],[1664,13,1664,30,0,0,4,1]],"BIO_free_all":[[881,13,881,20,0,0,4,0]],"X509_policy_node_get0_policy":[[86,9,86,14,0,0,4,0]]},"library_calls":["OPENSSL_version_patch","BIO_new_fp","BN_new","BN_bin2bn","EVP_PKEY_new_raw_public_key","X509_ALGOR_new","ASN1_STRING_new","OBJ_nid2obj","ASN1_STRING_set","X509_ALGOR_set0","BIO_new_fp","X509_signature_print","BIO_free","X509_STORE_new","X509_STORE_get0_param","X509_policy_node_get0_policy","CRYPTO_cts128_decrypt_block","SSL_CTX_new","TLS_method","SSL_new","SSL_set_bio","SSL_read_ex","SSL_free","SSL_CTX_free","BIO_free","OSSL_FUNC_cipher_pipeline_decrypt_init","EVP_MAC_fetch","EVP_MAC_CTX_new","OSSL_PARAM_construct_utf8_string","OSSL_PARAM_construct_end","EVP_MAC_init","EVP_MAC_update","EVP_MAC_update","EVP_MAC_final","EVP_MAC_CTX_free","EVP_MAC_free","ASN1_STRING_free","X509_ALGOR_free","EVP_PKEY_free","X509_STORE_free","BN_free","BIO_free"],"critical_calls":["OPENSSL_version_patch","BIO_new_fp","BN_new","BN_bin2bn","EVP_PKEY_new_raw_public_key","X509_ALGOR_new","ASN1_STRING_new","OBJ_nid2obj","ASN1_STRING_set","X509_ALGOR_set0","BIO_new_fp","X509_signature_print","BIO_free","X509_STORE_new","X509_STORE_get0_param","X509_policy_node_get0_policy","CRYPTO_cts128_decrypt_block","SSL_CTX_new","TLS_method","SSL_new","SSL_set_bio","SSL_read_ex","SSL_free","SSL_CTX_free","OSSL_FUNC_cipher_pipeline_decrypt_init","EVP_MAC_fetch","EVP_MAC_CTX_new","OSSL_PARAM_construct_utf8_string","OSSL_PARAM_construct_end","EVP_MAC_init","EVP_MAC_update","EVP_MAC_final","EVP_MAC_CTX_free","EVP_MAC_free","ASN1_STRING_free","X509_ALGOR_free","EVP_PKEY_free","X509_STORE_free","BN_free"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified APIs in a coherent scenario:

*/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 64) {
        // Need minimum data for operations
        return 0;
    }

    // 1. Get OpenSSL patch version
    unsigned int patch_version = OPENSSL_version_patch();
    
    // 2. Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create BIO from FILE*
    BIO *bio = BIO_new_fp(in_file, BIO_CLOSE);
    if (!bio) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create BIGNUM from part of input data
    BIGNUM *bn = BN_new();
    if (bn) {
        size_t bn_data_len = size > 32 ? 32 : size;
        BN_bin2bn(data, bn_data_len, bn);
        
        // 5. Create EVP_PKEY from raw public key (using first 32 bytes)
        EVP_PKEY *pkey = EVP_PKEY_new_raw_public_key(EVP_PKEY_ED25519, NULL, 
                                                     data, 32);
        
        // 6. Create X509_ALGOR and ASN1_STRING for signature operations
        X509_ALGOR *algor = X509_ALGOR_new();
        ASN1_STRING *asn1_str = ASN1_STRING_new();
        
        if (algor && asn1_str) {
            // 7. Create ASN1_OBJECT from a known NID
            ASN1_OBJECT *obj = OBJ_nid2obj(NID_sha256);
            
            // 8. Set ASN1_STRING with data
            size_t str_data_len = size > 48 ? 16 : size / 4;
            ASN1_STRING_set(asn1_str, data + 32, str_data_len);
            
            // 9. Set X509_ALGOR with the object
            X509_ALGOR_set0(algor, obj, V_ASN1_NULL, NULL);
            
            // 10. Create output file for signature printing
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                BIO *out_bio = BIO_new_fp(out_file, BIO_CLOSE);
                if (out_bio) {
                    // 11. Print signature (even if empty/not valid)
                    X509_signature_print(out_bio, algor, asn1_str);
                    BIO_free(out_bio);
                } else {
                    assert_file_closed(&out_file);;
                }
            }
        }
        
        // 12. Create X509_STORE and get parameters
        X509_STORE *store = X509_STORE_new();
        if (store) {
            X509_VERIFY_PARAM *param = X509_STORE_get0_param(store);
            // Dummy use of param to avoid unused variable warning
            if (param) {
                // Just reference it
            }
        }
        
        // 13. Try to get policy from a dummy node (NULL in this case)
        const ASN1_OBJECT *policy_obj = X509_policy_node_get0_policy(NULL);
        // Dummy use
        if (policy_obj) {
            // Just reference it
        }
        
        // 14. Attempt CTS128 decryption (using part of input as key/IV)
        if (size >= 48) {
            unsigned char key[16];
            unsigned char ivec[16];
            unsigned char out_buf[32];
            
            memcpy(key, data, 16);
            memcpy(ivec, data + 16, 16);
            
            // Use a simple block function (we'll create a dummy one)
            block128_f dummy_block = [](const unsigned char *in, 
                                        unsigned char *out, 
                                        const void *key) {
                memcpy(out, in, 16); // Just copy for dummy operation
            };
            
            size_t decrypt_len = size > 64 ? 32 : 16;
            CRYPTO_cts128_decrypt_block(data + 32, out_buf, decrypt_len, 
                                       key, ivec, dummy_block);
        }
        
        // 15. Attempt SSL read (create minimal SSL context)
        SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
        if (ssl_ctx) {
            SSL *ssl = SSL_new(ssl_ctx);
            if (ssl) {
                // Set bio for reading
                SSL_set_bio(ssl, bio, bio);
                
                size_t readbytes;
                unsigned char read_buf[1024];
                
                // Try to read (may fail but we call the API)
                SSL_read_ex(ssl, read_buf, sizeof(read_buf), &readbytes);
                
                SSL_free(ssl);
            }
            SSL_CTX_free(ssl_ctx);
        } else {
            // If SSL context creation failed, still need to free BIO
            BIO_free(bio);
        }
        
        // 16. Create OSSL_DISPATCH for pipeline decrypt init
        OSSL_DISPATCH dummy_dispatch[] = {
            {0, NULL}
        };
        OSSL_FUNC_cipher_pipeline_decrypt_init_fn *pipeline_decrypt_init = 
            OSSL_FUNC_cipher_pipeline_decrypt_init(dummy_dispatch);
        // Dummy use
        if (pipeline_decrypt_init) {
            // Just reference it
        }
        
        // 17. MAC operations
        EVP_MAC *mac = EVP_MAC_fetch(NULL, "HMAC", NULL);
        if (mac) {
            EVP_MAC_CTX *mac_ctx = EVP_MAC_CTX_new(mac);
            if (mac_ctx) {
                // Set up parameters
                char digest_name[] = "SHA256";
                OSSL_PARAM params[2];
                params[0] = OSSL_PARAM_construct_utf8_string("digest", 
                                                            digest_name, 
                                                            sizeof(digest_name));
                params[1] = OSSL_PARAM_construct_end();
                
                // Initialize MAC with key from input
                size_t key_len = size > 64 ? 32 : 16;
                EVP_MAC_init(mac_ctx, data, key_len, params);
                
                // Update with more data
                if (size > 96) {
                    EVP_MAC_update(mac_ctx, data + 64, size - 64);
                } else {
                    EVP_MAC_update(mac_ctx, data, size);
                }
                
                // Finalize MAC
                unsigned char mac_out[64];
                size_t mac_out_len;
                EVP_MAC_final(mac_ctx, mac_out, &mac_out_len, sizeof(mac_out));
                
                EVP_MAC_CTX_free(mac_ctx);
            }
            EVP_MAC_free(mac);
        }
        
        // 18. Cleanup
        if (asn1_str) ASN1_STRING_free(asn1_str);
        if (algor) X509_ALGOR_free(algor);
        if (pkey) EVP_PKEY_free(pkey);
        if (store) X509_STORE_free(store);
        
        BN_free(bn);
    } else {
        BIO_free(bio);
    }
    
    // Note: bio is freed by SSL_free or explicitly above
    // in_file is closed by BIO_free with BIO_CLOSE flag
    
    assert_file_closed(&in_file);
	return 0;
}