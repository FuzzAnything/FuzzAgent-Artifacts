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
//<ID> 492
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["OSSL_LIB_CTX_new","sk_OCSP_RESPONSE_new_null","sk_OCSP_RESPONSE_zero","d2i_OCSP_RESPONSE","sk_OCSP_RESPONSE_push","ossl_check_const_OPENSSL_CSTRING_lh_plain_type","SSL_CTX_new","TLS_method","SSL_new","EVP_RAND_fetch","EVP_RAND_CTX_new","EVP_RAND_free","EVP_PKEY_CTX_new_id","EVP_PKEY_CTX_set_ecdh_kdf_md","EVP_sha256","d2i_RSAPrivateKey_fp","RSA_get_multi_prime_extra_count","RSA_free","OPENSSL_sk_new_null","ossl_check_SRP_gN_freefunc_type","OPENSSL_sk_set_thunks","BIO_new_mem_buf","d2i_X509_bio","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_X509_type","ossl_check_X509_sk_type","OPENSSL_sk_push","OSSL_CMP_ITAV_new_caCerts","OSSL_CMP_ITAV_free","OPENSSL_sk_pop_free","ossl_check_X509_sk_type","ossl_check_X509_freefunc_type","OPENSSL_sk_free","ossl_check_X509_sk_type","X509_free","BIO_free","sk_OCSP_RESPONSE_pop_free","EVP_PKEY_CTX_free","EVP_RAND_CTX_free","OPENSSL_sk_free","SSL_free","SSL_CTX_free","OSSL_LIB_CTX_free"],"critical_calls":["OSSL_LIB_CTX_new","sk_OCSP_RESPONSE_new_null","sk_OCSP_RESPONSE_zero","d2i_OCSP_RESPONSE","sk_OCSP_RESPONSE_push","ossl_check_const_OPENSSL_CSTRING_lh_plain_type","SSL_CTX_new","TLS_method","SSL_new","EVP_RAND_fetch","EVP_RAND_CTX_new","EVP_RAND_free","EVP_PKEY_CTX_new_id","EVP_PKEY_CTX_set_ecdh_kdf_md","EVP_sha256","d2i_RSAPrivateKey_fp","RSA_get_multi_prime_extra_count","RSA_free","OPENSSL_sk_new_null","ossl_check_SRP_gN_freefunc_type","OPENSSL_sk_set_thunks","BIO_new_mem_buf","d2i_X509_bio","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_X509_type","ossl_check_X509_sk_type","OPENSSL_sk_push","OSSL_CMP_ITAV_new_caCerts","OSSL_CMP_ITAV_free","OPENSSL_sk_pop_free","ossl_check_X509_freefunc_type","ossl_check_X509_sk_type","BIO_free","sk_OCSP_RESPONSE_pop_free","EVP_PKEY_CTX_free","EVP_RAND_CTX_free","OPENSSL_sk_free","SSL_free","SSL_CTX_free","OSSL_LIB_CTX_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Create and free OSSL_LIB_CTX
    OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
    
    // 2. Create OCSP response stack
    struct stack_st_OCSP_RESPONSE *ocsp_stack = sk_OCSP_RESPONSE_new_null();
    if (ocsp_stack) {
        // 3. Zero the stack
        sk_OCSP_RESPONSE_zero(ocsp_stack);
        
        // 4. Parse OCSP response from input data
        const unsigned char *p = data;
        OCSP_RESPONSE *ocsp_resp = d2i_OCSP_RESPONSE(NULL, &p, (long)size);
        if (ocsp_resp) {
            // 5. Push OCSP response to stack
            sk_OCSP_RESPONSE_push(ocsp_stack, ocsp_resp);
        }
    }
    
    // 6. Use the lhash type check function
    const OPENSSL_CSTRING dummy_string = "test";
    const OPENSSL_CSTRING *checked_string = 
        ossl_check_const_OPENSSL_CSTRING_lh_plain_type(&dummy_string);
    (void)checked_string; // Prevent unused warning
    
    // 7. Create SSL context and SSL object
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = SSL_new(ssl_ctx);
    
    // 8. Create EVP_RAND_CTX (use default random method if available)
    EVP_RAND *rand = EVP_RAND_fetch(libctx, "CTR-DRBG", NULL);
    EVP_RAND_CTX *rand_ctx = NULL;
    if (rand) {
        rand_ctx = EVP_RAND_CTX_new(rand, NULL);
        EVP_RAND_free(rand);
    }
    
    // 9. Create EVP_PKEY_CTX for ECDH and set KDF MD
    EVP_PKEY_CTX *pkey_ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
    if (pkey_ctx) {
        EVP_PKEY_CTX_set_ecdh_kdf_md(pkey_ctx, EVP_sha256());
    }
    
    // 10. Parse RSA private key from FILE* created from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    RSA *rsa = NULL;
    if (in_file) {
        rsa = d2i_RSAPrivateKey_fp(in_file, NULL);
        if (rsa) {
            // 11. Get multi-prime extra count
            int extra_count = RSA_get_multi_prime_extra_count(rsa);
            (void)extra_count; // Prevent unused warning
            
            // 12. Free RSA
            RSA_free(rsa);
        }
        assert_file_closed(&in_file);;
    }
    
    // 13-14. Create and configure OPENSSL_STACK with thunks
    OPENSSL_STACK *generic_stack = OPENSSL_sk_new_null();
    if (generic_stack) {
        // Create a dummy free function thunk
        OPENSSL_sk_freefunc_thunk dummy_thunk = 
            (OPENSSL_sk_freefunc_thunk)ossl_check_SRP_gN_freefunc_type(
                (sk_SRP_gN_freefunc)OPENSSL_sk_free);
        
        OPENSSL_sk_set_thunks(generic_stack, dummy_thunk);
    }
    
    // 15-16. Create BIO from input data and parse X509
    BIO *bio = BIO_new_mem_buf(data, (int)size);
    X509 *x509 = NULL;
    if (bio) {
        x509 = d2i_X509_bio(bio, NULL);
        
        // 17-20. Create X509 stack and push certificate
        struct stack_st_X509 *x509_stack = sk_X509_new_null();
        if (x509_stack && x509) {
            X509 *checked_x509 = ossl_check_X509_type(x509);
            OPENSSL_STACK *x509_ossl_stack = ossl_check_X509_sk_type(x509_stack);
            OPENSSL_sk_push(x509_ossl_stack, checked_x509);
            
            // 21-22. Create and free OSSL_CMP_ITAV with caCerts
            OSSL_CMP_ITAV *itav = OSSL_CMP_ITAV_new_caCerts(x509_stack);
            if (itav) {
                OSSL_CMP_ITAV_free(itav);
            }
            
            sk_X509_pop_free(x509_stack, X509_free);
        } else {
            if (x509_stack) sk_X509_free(x509_stack);
            if (x509) X509_free(x509);
        }
        
        // 23. Free BIO
        BIO_free(bio);
    }
    
    // 24. Use SRP_gN freefunc type check (already used in dummy_thunk)
    
    // 25. Free OCSP response stack with pop_free
    if (ocsp_stack) {
        sk_OCSP_RESPONSE_pop_free(ocsp_stack, OCSP_RESPONSE_free);
    }
    
    // 26. Free EVP_PKEY_CTX
    if (pkey_ctx) {
        EVP_PKEY_CTX_free(pkey_ctx);
    }
    
    // 27. Free EVP_RAND_CTX
    if (rand_ctx) {
        EVP_RAND_CTX_free(rand_ctx);
    }
    
    // 28. Free generic stack
    if (generic_stack) {
        OPENSSL_sk_free(generic_stack);
    }
    
    // Free SSL objects
    if (ssl) {
        SSL_free(ssl);
    }
    if (ssl_ctx) {
        SSL_CTX_free(ssl_ctx);
    }
    
    // 29. Free OSSL_LIB_CTX
    if (libctx) {
        OSSL_LIB_CTX_free(libctx);
    }
    
    assert_file_closed(&in_file);
	return 0;
}