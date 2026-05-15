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
//<ID> 489
//<Prompt> []
/*<Combination>: [
*/
//<score> 371, nr_unique_branch: 156
//<Quality> {"density":14,"unique_branches":{"EC_KEY_priv2buf":[[1047,9,1047,17,0,0,4,1],[1049,9,1049,44,0,0,4,1],[1052,9,1052,17,0,0,4,1]],"ossl_prov_get_entropy":[[92,9,92,35,0,0,4,0]],"EC_POINT_get_affine_coordinates":[[936,9,936,58,0,0,4,1],[940,9,940,42,0,0,4,1],[944,9,944,46,0,0,4,1],[948,9,948,24,0,0,4,1]],"ossl_prov_ctx_get0_handle":[[59,9,59,20,0,0,4,1]],"EC_KEY_up_ref":[[179,9,179,47,0,0,4,1],[184,13,184,20,0,0,4,0],[293,12,293,18,2,0,4,1]],"SSL_is_tls":[[996,9,996,44,0,0,4,1],[996,48,996,76,0,0,4,1],[1000,9,1000,19,0,0,4,1],[266,5,266,81,4,0,4,1],[32,6,32,19,5,0,4,1],[33,23,33,61,5,0,4,0]],"EVP_DigestVerify":[[546,9,546,21,0,0,4,0]],"ossl_prov_drbg_generate":[[643,9,643,44,0,0,4,0],[647,13,647,48,0,0,4,0]],"wait_random_seeded":[[441,9,441,16,0,0,4,0],[443,13,443,78,0,0,4,0],[453,17,453,32,0,0,4,0],[456,29,456,38,0,0,4,1],[457,21,457,50,0,0,4,0]],"bn_copy_words":[[157,9,157,23,0,0,4,1],[161,9,161,22,0,0,4,0]],"drbg_ctr_get_ctx_params":[[697,9,697,27,0,0,4,1],[712,9,712,27,0,0,4,1]],"evp_rand_strength_locked":[[625,9,625,53,0,0,4,1]],"ossl_prov_cleanup_entropy":[[104,9,104,39,0,0,4,0]],"EC_POINT_point2buf":[[145,9,145,17,0,0,4,1],[147,9,147,44,0,0,4,1],[150,9,150,17,0,0,4,1]],"ossl_rand_pool_new":[[32,23,32,47,0,0,4,0]],"EC_POINT_point2oct":[[77,9,77,22,0,0,4,1],[81,9,81,36,0,0,4,0],[82,12,82,56,0,0,4,1],[86,9,86,42,0,0,4,1],[90,9,90,50,0,0,4,0],[91,13,91,61,0,0,4,0]],"ossl_rand_pool_bytes_needed":[[257,9,257,34,0,0,4,1]],"ossl_rand_get_user_entropy":[[50,9,50,20,0,0,4,0],[50,24,50,46,0,0,4,0]],"EVP_CIPHER_settable_ctx_params":[[1180,9,1180,23,0,0,4,0],[1180,27,1180,62,0,0,4,1]],"SSL_set_quic_tls_cbs":[[132,9,132,23,0,0,4,1],[137,9,137,55,0,0,4,0],[32,6,32,19,3,0,4,1],[33,23,33,61,3,0,4,0]],"do_EC_KEY_print":[[279,9,279,18,0,0,4,1],[279,22,279,60,0,0,4,1],[284,9,284,36,0,0,4,0],[284,40,284,73,0,0,4,0],[286,13,286,24,0,0,4,1],[290,9,290,38,0,0,4,0],[290,42,290,76,0,0,4,0],[292,13,292,25,0,0,4,1],[296,9,296,38,0,0,4,0],[303,9,303,34,0,0,4,1],[305,9,307,13,0,0,4,0],[328,9,328,13,0,0,4,0]],"bwrite_conv":[[81,9,81,17,0,0,4,0]],"ossl_rand_get0_seed_noncreating":[[579,9,579,21,0,0,4,1],[582,9,582,45,0,0,4,1]],"seed_get_seed":[[194,9,194,30,0,0,4,1]],"evp_rand_nonce_locked":[[646,9,646,33,0,0,4,1]],"EVP_PKEY_set1_EC_KEY":[[67,9,67,28,0,0,4,1],[69,9,69,32,0,0,4,1]],"evp_rand_generate_locked":[[567,13,568,57,0,0,4,0]],"mem_write":[[221,9,221,40,0,0,4,0]],"evp_rand_clear_seed_locked":[[739,9,739,38,0,0,4,0]],"ossl_rand_pool_entropy_needed":[[189,9,189,48,0,0,4,1]],"EC_KEY_key2buf":[[939,9,939,20,0,0,4,1],[939,24,939,44,0,0,4,1],[939,48,939,66,0,0,4,1]],"cleanup_entropy":[[248,9,248,29,0,0,4,0]],"EVP_RAND_nonce":[[655,9,655,20,0,0,4,1],[655,24,655,35,0,0,4,1],[655,39,655,50,0,0,4,1],[660,9,660,28,0,0,4,1]],"EC_KEY_free":[[77,9,77,14,0,0,4,0]],"evp_rand_get_seed":[[725,9,725,28,0,0,4,1]],"ossl_prov_drbg_instantiate":[[385,9,385,31,0,0,4,1],[446,9,447,27,0,0,4,0],[462,9,462,44,0,0,4,1]],"drbg_ctr_instantiate":[[326,9,326,24,0,0,4,0]],"EC_KEY_priv2oct":[[971,9,971,29,0,0,4,1],[971,33,971,59,0,0,4,1],[973,9,973,45,0,0,4,1]],"ossl_pool_acquire_entropy":[[664,9,664,30,0,0,4,1],[669,9,669,29,0,0,4,1]],"EC_KEY_print":[[702,9,702,16,0,0,4,0]],"bio_write_intern":[[372,9,372,16,0,0,4,1]],"ossl_ec_group_simple_order_bits":[[1277,9,1277,29,0,0,4,1]],"ossl_ec_key_simple_priv2oct":[[987,9,987,32,0,0,4,1],[989,9,989,20,0,0,4,0],[989,9,989,20,0,0,4,1],[991,14,991,35,0,0,4,1],[996,9,996,58,0,0,4,1]],"evp_rand_clear_seed":[[746,9,746,28,0,0,4,1]],"rand_drbg_restart":[[734,9,734,44,0,0,4,1],[738,9,738,52,0,0,4,0]],"tls_callbacks_from_dispatch":[[85,12,85,35,0,0,4,1],[114,9,114,37,0,0,4,0]],"ossl_ec_GFp_simple_point2oct":[[169,9,169,46,0,0,4,0],[170,12,170,51,0,0,4,1],[176,9,176,46,0,0,4,1],[190,11,190,48,0,0,4,1],[193,9,193,20,0,0,4,0],[193,9,193,20,0,0,4,1],[194,13,194,22,0,0,4,1],[199,13,199,24,0,0,4,0],[201,17,201,28,0,0,4,1],[209,13,209,22,0,0,4,1],[212,13,212,70,0,0,4,1],[215,14,215,49,0,0,4,1],[216,20,216,51,0,0,4,1],[225,13,225,29,0,0,4,1],[229,16,229,24,0,0,4,0],[229,16,229,24,0,0,4,1],[235,13,235,31,0,0,4,1],[240,13,240,50,0,0,4,0],[243,17,243,33,0,0,4,1],[247,20,247,28,0,0,4,0],[247,20,247,28,0,0,4,1],[255,13,255,21,0,0,4,1],[261,9,261,17,0,0,4,0],[261,9,261,17,0,0,4,1]],"get_entropy":[[199,9,199,29,0,0,4,0]],"evp_rand_get_seed_locked":[[709,9,709,36,0,0,4,0]],"ecp_nistz256_mod_inverse":[[546,17,546,22,0,0,4,0],[546,17,546,22,0,0,4,1],[551,17,551,23,0,0,4,0],[551,17,551,23,0,0,4,1],[556,17,556,23,0,0,4,0],[556,17,556,23,0,0,4,1],[560,17,560,27,0,0,4,0],[560,17,560,27,0,0,4,1],[564,17,564,23,0,0,4,0],[564,17,564,23,0,0,4,1],[568,17,568,23,0,0,4,0],[568,17,568,23,0,0,4,1],[572,17,572,22,0,0,4,0],[572,17,572,22,0,0,4,1]],"ossl_rand_cleanup_user_entropy":[[68,9,68,20,0,0,4,0],[68,24,68,46,0,0,4,0]],"ecp_nistz256_get_affine":[[1157,9,1157,46,0,0,4,1],[1162,9,1162,62,0,0,4,1],[1162,66,1162,119,0,0,4,1],[1162,123,1162,176,0,0,4,1],[1171,9,1171,18,0,0,4,0],[1173,13,1173,48,0,0,4,1],[1177,9,1177,18,0,0,4,0],[1181,13,1181,48,0,0,4,1]]},"library_calls":["EVP_aria_256_ccm","EVP_CIPHER_settable_ctx_params","BIO_new_mem_buf","d2i_PKCS7_ENC_CONTENT","EC_KEY_new_by_curve_name","EC_KEY_generate_key","EC_KEY_print","EVP_MD_CTX_new","EVP_PKEY_new","EVP_PKEY_set1_EC_KEY","EVP_DigestVerify","EVP_PKEY_free","EVP_MD_CTX_free","EC_KEY_free","OSSL_FUNC_decoder_gettable_params","SSL_CTX_new","TLS_method","SSL_new","SSL_set_quic_tls_cbs","SSL_free","SSL_CTX_free","EVP_RAND_CTX_new","EVP_RAND_fetch","EVP_RAND_nonce","EVP_RAND_CTX_free","BIO_free","PKCS7_ENC_CONTENT_free"],"critical_calls":["EVP_aria_256_ccm","EVP_CIPHER_settable_ctx_params","BIO_new_mem_buf","d2i_PKCS7_ENC_CONTENT","EC_KEY_new_by_curve_name","EC_KEY_generate_key","EC_KEY_print","EVP_MD_CTX_new","EVP_PKEY_new","EVP_PKEY_set1_EC_KEY","EVP_DigestVerify","EVP_PKEY_free","EVP_MD_CTX_free","EC_KEY_free","OSSL_FUNC_decoder_gettable_params","SSL_CTX_new","TLS_method","SSL_new","SSL_set_quic_tls_cbs","SSL_free","SSL_CTX_free","EVP_RAND_CTX_new","EVP_RAND_fetch","EVP_RAND_nonce","EVP_RAND_CTX_free","BIO_free","PKCS7_ENC_CONTENT_free"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified OpenSSL APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for operations
    
    // 1. Setup cipher context with ARIA-256-CCM
    const EVP_CIPHER *cipher = EVP_aria_256_ccm();
    if (!cipher) {
	return 0;
	}
	
    
    // Get settable parameters for the cipher
    const OSSL_PARAM *cipher_params = EVP_CIPHER_settable_ctx_params(cipher);
    
    // 2. Create a memory BIO for input data
    BIO *bio = BIO_new_mem_buf(data, size);
    if (!bio) {
	return 0;
	}
	
    
    // 3. Decode PKCS7 encrypted content
    const unsigned char *p = data;
    PKCS7_ENC_CONTENT *enc_content = d2i_PKCS7_ENC_CONTENT(NULL, &p, size);
    
    // 4. Create and print EC key (dummy operations)
    EC_KEY *ec_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if (ec_key) {
        // Generate a key for printing
        EC_KEY_generate_key(ec_key);
        EC_KEY_print(bio, ec_key, 0);
        
        // 5. Setup digest verification context
        EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
        if (md_ctx) {
            EVP_PKEY *pkey = EVP_PKEY_new();
            if (pkey && EVP_PKEY_set1_EC_KEY(pkey, ec_key)) {
                unsigned char sig[256];
                size_t siglen = size > 256 ? 256 : size;
                memcpy(sig, data, siglen);
                
                // Verify signature (will likely fail due to random data)
                EVP_DigestVerify(md_ctx, sig, siglen, data + siglen, 
                                size - siglen > siglen ? siglen : size - siglen);
                
                EVP_PKEY_free(pkey);
            }
            EVP_MD_CTX_free(md_ctx);
        }
        EC_KEY_free(ec_key);
    }
    
    // 6. Get decoder gettable params (dummy decoder dispatch)
    OSSL_DISPATCH dummy_dispatch[] = {
        {0, NULL}
    };
    OSSL_FUNC_decoder_gettable_params_fn *gettable_fn = 
        OSSL_FUNC_decoder_gettable_params(dummy_dispatch);
    
    // 7. Setup SSL for QUIC (dummy operations)
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    if (ssl_ctx) {
        SSL *ssl = SSL_new(ssl_ctx);
        if (ssl) {
            OSSL_DISPATCH quic_dispatch[] = {
                {0, NULL}
            };
            SSL_set_quic_tls_cbs(ssl, quic_dispatch, NULL);
            SSL_free(ssl);
        }
        SSL_CTX_free(ssl_ctx);
    }
    
    // 8. Generate nonce using random context
    EVP_RAND_CTX *rand_ctx = EVP_RAND_CTX_new(EVP_RAND_fetch(NULL, "CTR-DRBG", NULL), NULL);
    if (rand_ctx) {
        unsigned char nonce[32];
        EVP_RAND_nonce(rand_ctx, nonce, sizeof(nonce));
        EVP_RAND_CTX_free(rand_ctx);
    }
    
    // Cleanup
    BIO_free(bio);
    PKCS7_ENC_CONTENT_free(enc_content);
    
    return 0;
}