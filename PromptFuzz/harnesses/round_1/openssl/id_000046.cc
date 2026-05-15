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
//<ID> 553
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 4
//<Quality> {"density":7,"unique_branches":{"RSA_meth_new":[[24,9,24,21,0,0,4,0],[28,13,28,31,0,0,4,0]],"CMS_get0_signers":[[848,17,848,42,0,0,4,1]],"RSA_meth_free":[[39,9,39,21,0,0,4,0]]},"library_calls":["OSSL_LIB_CTX_new","sk_OCSP_RESPONSE_new_null","OSSL_LIB_CTX_free","sk_OCSP_RESPONSE_zero","d2i_OCSP_RESPONSE","sk_OCSP_RESPONSE_push","TLS_method","SSL_CTX_new","SSL_new","EC_KEY_METHOD_new","EC_KEY_METHOD_set_keygen","EC_KEY_METHOD_free","EVP_RAND_fetch","EVP_RAND_CTX_new","EVP_PKEY_CTX_new_id","EVP_sha256","RSA_meth_new","RSA_meth_set_mod_exp","d2i_RSAPrivateKey_fp","RSA_free","OPENSSL_sk_new_null","OPENSSL_sk_set_thunks","ossl_check_SRP_gN_freefunc_type","BIO_new_mem_buf","d2i_X509_bio","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_X509_type","ossl_check_X509_sk_type","OPENSSL_sk_push","OSSL_CMP_ITAV_new_caCerts","ossl_check_const_OPENSSL_CSTRING_lh_plain_type","CMS_ContentInfo_new","CMS_get0_signers","CMS_ContentInfo_free","OSSL_CMP_ITAV_free","OPENSSL_sk_pop_free","ossl_check_X509_sk_type","ossl_check_X509_freefunc_type","X509_free","X509_free","X509_free","BIO_free","sk_OCSP_RESPONSE_pop_free","OCSP_RESPONSE_free","OCSP_RESPONSE_free","EVP_PKEY_CTX_free","EVP_RAND_CTX_free","EVP_RAND_free","OPENSSL_sk_free","RSA_meth_free","SSL_free","SSL_CTX_free","OSSL_LIB_CTX_free"],"critical_calls":["OSSL_LIB_CTX_new","sk_OCSP_RESPONSE_new_null","sk_OCSP_RESPONSE_zero","d2i_OCSP_RESPONSE","sk_OCSP_RESPONSE_push","TLS_method","SSL_CTX_new","SSL_new","EC_KEY_METHOD_new","EC_KEY_METHOD_set_keygen","EC_KEY_METHOD_free","EVP_RAND_fetch","EVP_RAND_CTX_new","EVP_PKEY_CTX_new_id","EVP_sha256","RSA_meth_new","RSA_meth_set_mod_exp","d2i_RSAPrivateKey_fp","RSA_free","OPENSSL_sk_new_null","OPENSSL_sk_set_thunks","ossl_check_SRP_gN_freefunc_type","BIO_new_mem_buf","d2i_X509_bio","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_X509_type","ossl_check_X509_sk_type","OPENSSL_sk_push","OSSL_CMP_ITAV_new_caCerts","ossl_check_const_OPENSSL_CSTRING_lh_plain_type","CMS_ContentInfo_new","CMS_get0_signers","CMS_ContentInfo_free","OSSL_CMP_ITAV_free","OPENSSL_sk_pop_free","ossl_check_X509_freefunc_type","X509_free","X509_free","ossl_check_X509_sk_type","X509_free","BIO_free","sk_OCSP_RESPONSE_pop_free","OCSP_RESPONSE_free","OCSP_RESPONSE_free","EVP_PKEY_CTX_free","EVP_RAND_CTX_free","EVP_RAND_free","OPENSSL_sk_free","RSA_meth_free","SSL_free","SSL_CTX_free","OSSL_LIB_CTX_free"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified OpenSSL APIs to achieve an event involving parsing and processing OCSP responses, SSL/TLS setup, RSA key handling, and certificate management:

*/


extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data
    
    // 1. Create OSSL_LIB_CTX
    OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
    if (!libctx) {
	return 0;
	}
	
    
    // 2. Create stack for OCSP responses
    struct stack_st_OCSP_RESPONSE *ocsp_stack = sk_OCSP_RESPONSE_new_null();
    if (!ocsp_stack) {
        OSSL_LIB_CTX_free(libctx);
        return 0;
    }
    
    // 3. Zero the OCSP stack
    sk_OCSP_RESPONSE_zero(ocsp_stack);
    
    // 4. Parse OCSP response from input data
    const unsigned char *p = data;
    long len = static_cast<long>(size);
    OCSP_RESPONSE *ocsp_resp = d2i_OCSP_RESPONSE(nullptr, &p, len);
    if (ocsp_resp) {
        // 5. Push OCSP response to stack
        sk_OCSP_RESPONSE_push(ocsp_stack, ocsp_resp);
    }
    
    // 6. Create SSL context with TLS method
    const SSL_METHOD *meth = TLS_method();
    SSL_CTX *ssl_ctx = SSL_CTX_new(meth);
    
    // 7. Create SSL connection
    SSL *ssl = nullptr;
    if (ssl_ctx) {
        ssl = SSL_new(ssl_ctx);
    }
    
    // 8. Setup EC key method
    EC_KEY_METHOD *ec_method = EC_KEY_METHOD_new(nullptr);
    if (ec_method) {
        EC_KEY_METHOD_set_keygen(ec_method, [](EC_KEY *key) -> int { return 0; });
        EC_KEY_METHOD_free(ec_method);
    }
    
    // 9. Fetch random number generator
    EVP_RAND *rand = EVP_RAND_fetch(libctx, "CTR-DRBG", nullptr);
    EVP_RAND_CTX *rand_ctx = nullptr;
    if (rand) {
        rand_ctx = EVP_RAND_CTX_new(rand, nullptr);
    }
    
    // 10. Create EVP_PKEY context
    EVP_PKEY_CTX *pkey_ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
    
    // 11. Get SHA256 digest
    const EVP_MD *sha256 = EVP_sha256();
    
    // 12. Setup RSA method
    RSA_METHOD *rsa_method = RSA_meth_new("Fuzz RSA Method", 0);
    if (rsa_method) {
        RSA_meth_set_mod_exp(rsa_method, [](BIGNUM *r0, const BIGNUM *i, RSA *rsa, BN_CTX *ctx) -> int { return 0; });
    }
    
    // 13. Read RSA private key from input data using FILE
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        RSA *rsa = d2i_RSAPrivateKey_fp(in_file, nullptr);
        if (rsa) {
            // 14. Free RSA key
            RSA_free(rsa);
        }
        assert_file_closed(&in_file);;
    }
    
    // 15. Create null stack
    OPENSSL_STACK *generic_stack = OPENSSL_sk_new_null();
    
    // 16. Setup stack thunks with SRP free function
    if (generic_stack) {
        OPENSSL_sk_freefunc_thunk thunk = [](OPENSSL_sk_freefunc fr, void *data) {
            // Empty thunk function
        };
        OPENSSL_sk_set_thunks(generic_stack, thunk);
        
        // Cast SRP free function type
        OPENSSL_sk_freefunc srp_free = ossl_check_SRP_gN_freefunc_type([](SRP_gN *a) {});
    }
    
    // 17. Create BIO from input data
    BIO *mem_bio = BIO_new_mem_buf(data, static_cast<int>(size));
    
    // 18. Read X509 certificate from BIO
    X509 *x509 = nullptr;
    if (mem_bio) {
        x509 = d2i_X509_bio(mem_bio, nullptr);
    }
    
    // 19. Create X509 stack and push certificate
    struct stack_st_X509 *x509_stack = sk_X509_new_null();
    if (x509_stack && x509) {
        // Type checking
        X509 *checked_x509 = ossl_check_X509_type(x509);
        OPENSSL_STACK *checked_stack = ossl_check_X509_sk_type(x509_stack);
        
        // Push to stack
        OPENSSL_sk_push(checked_stack, checked_x509);
    }
    
    // 20. Create CMP ITAV from CA certificates
    OSSL_CMP_ITAV *itav = nullptr;
    if (x509_stack) {
        itav = OSSL_CMP_ITAV_new_caCerts(x509_stack);
    }
    
    // 21. Type checking for OPENSSL_CSTRING
    const OPENSSL_CSTRING *cstr_check = ossl_check_const_OPENSSL_CSTRING_lh_plain_type(nullptr);
    
    // 22. Simulate CMS operation (even though we don't have CMS content)
    struct stack_st_X509 *cms_signers = nullptr;
    if (ssl_ctx) {
        // Create dummy CMS content info
        CMS_ContentInfo *cms = CMS_ContentInfo_new();
        if (cms) {
            cms_signers = CMS_get0_signers(cms);
            CMS_ContentInfo_free(cms);
        }
    }
    
    // 23. Cleanup: Free all allocated resources in reverse order
    
    // Free OSSL_CMP_ITAV
    if (itav) {
        OSSL_CMP_ITAV_free(itav);
    }
    
    // Free X509 stack with pop_free
    if (x509_stack) {
        OPENSSL_sk_pop_free(ossl_check_X509_sk_type(x509_stack), 
                           ossl_check_X509_freefunc_type([](X509 *a) { X509_free(a); }));
    }
    
    // Free X509 certificate
    if (x509) {
        X509_free(x509);
    }
    
    // Free BIO
    if (mem_bio) {
        BIO_free(mem_bio);
    }
    
    // Free OCSP stack with pop_free
    if (ocsp_stack) {
        sk_OCSP_RESPONSE_pop_free(ocsp_stack, [](OCSP_RESPONSE *a) { OCSP_RESPONSE_free(a); });
    }
    
    // Free EVP_PKEY_CTX
    if (pkey_ctx) {
        EVP_PKEY_CTX_free(pkey_ctx);
    }
    
    // Free EVP_RAND_CTX
    if (rand_ctx) {
        EVP_RAND_CTX_free(rand_ctx);
    }
    
    // Free EVP_RAND
    if (rand) {
        EVP_RAND_free(rand);
    }
    
    // Free generic stack
    if (generic_stack) {
        OPENSSL_sk_free(generic_stack);
    }
    
    // Free RSA method
    if (rsa_method) {
        RSA_meth_free(rsa_method);
    }
    
    // Free SSL
    if (ssl) {
        SSL_free(ssl);
    }
    
    // Free SSL context
    if (ssl_ctx) {
        SSL_CTX_free(ssl_ctx);
    }
    
    // Free OSSL_LIB_CTX
    OSSL_LIB_CTX_free(libctx);
    
    assert_file_closed(&in_file);
	return 0;
}