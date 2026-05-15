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
//<ID> 255
//<Prompt> []
/*<Combination>: [
*/
//<score> 82.5, nr_unique_branch: 25
//<Quality> {"density":33,"unique_branches":{"ossl_ascii_isdigit":[[265,41,265,50,1,0,4,0]],"ERR_clear_last_mark":[[91,15,91,22,0,0,4,1]],"do_buf":[[171,9,171,15,0,0,4,0],[178,9,178,15,0,0,4,0]],"ossl_asn1_time_to_tm":[[118,13,118,38,0,0,4,1],[122,13,122,21,0,0,4,1],[125,13,125,38,0,0,4,0],[125,13,125,38,0,0,4,1],[129,13,129,21,0,0,4,1],[132,14,132,41,0,0,4,1],[134,13,134,26,0,0,4,1],[134,30,134,43,0,0,4,0],[134,30,134,43,0,0,4,1],[136,17,136,19,0,0,4,1],[137,9,137,15,0,0,4,0],[137,9,137,15,0,0,4,1],[141,9,141,15,0,0,4,0],[141,9,141,15,0,0,4,1],[142,17,142,42,0,0,4,1],[147,9,147,15,0,0,4,0],[147,9,147,15,0,0,4,1],[150,9,150,15,0,0,4,1],[163,9,163,15,0,0,4,1],[166,9,166,15,0,0,4,1],[169,9,169,15,0,0,4,1]]},"library_calls":["OPENSSL_init_crypto","ERR_clear_error","BIO_new_mem_buf","d2i_ISSUING_DIST_POINT","ISSUING_DIST_POINT_free","BIO_ctrl","d2i_RSAPrivateKey_bio","RSA_get0_multi_prime_factors","RSA_free","BIO_ctrl","d2i_PKCS7_bio","BIO_new","BIO_s_null","ASN1_PCTX_new","PKCS7_print_ctx","ASN1_PCTX_free","BIO_free","PKCS7_free","OCSP_REQUEST_new","OCSP_request_onereq_count","OCSP_REQUEST_free","X509_STORE_new","X509_STORE_set_ex_data","X509_VERIFY_PARAM_new","X509_STORE_set1_param","X509_VERIFY_PARAM_free","X509_STORE_free","OPENSSL_sk_new_null","OPENSSL_sk_set_thunks","X509_EXTENSION_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","X509_EXTENSION_set_data","ASN1_OCTET_STRING_free","OPENSSL_sk_push","ossl_check_X509_EXTENSION_type","i2d_X509_EXTENSIONS","CRYPTO_free","X509_EXTENSION_free","OPENSSL_sk_free","OSSL_PRIVILEGE_POLICY_ID_new","OBJ_nid2obj","OSSL_PRIVILEGE_POLICY_ID_free","BIO_ctrl","ASN1_d2i_bio","d2i_ISSUING_DIST_POINT","d2i_ISSUING_DIST_POINT","ISSUING_DIST_POINT_free","EVP_sha224","EVP_rc2_cbc","BIO_free"],"critical_calls":["OPENSSL_init_crypto","ERR_clear_error","BIO_new_mem_buf","d2i_ISSUING_DIST_POINT","ISSUING_DIST_POINT_free","BIO_ctrl","d2i_RSAPrivateKey_bio","RSA_get0_multi_prime_factors","RSA_free","BIO_ctrl","d2i_PKCS7_bio","BIO_new","BIO_s_null","ASN1_PCTX_new","PKCS7_print_ctx","ASN1_PCTX_free","BIO_free","PKCS7_free","OCSP_REQUEST_new","OCSP_request_onereq_count","OCSP_REQUEST_free","X509_STORE_new","X509_STORE_set_ex_data","X509_VERIFY_PARAM_new","X509_STORE_set1_param","X509_VERIFY_PARAM_free","X509_STORE_free","OPENSSL_sk_new_null","OPENSSL_sk_set_thunks","X509_EXTENSION_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","X509_EXTENSION_set_data","ASN1_OCTET_STRING_free","OPENSSL_sk_push","ossl_check_X509_EXTENSION_type","i2d_X509_EXTENSIONS","CRYPTO_free","X509_EXTENSION_free","OPENSSL_sk_free","OSSL_PRIVILEGE_POLICY_ID_new","OBJ_nid2obj","OSSL_PRIVILEGE_POLICY_ID_free","BIO_ctrl","ASN1_d2i_bio","d2i_ISSUING_DIST_POINT","d2i_ISSUING_DIST_POINT","ISSUING_DIST_POINT_free","EVP_sha224","EVP_rc2_cbc","BIO_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL
    OPENSSL_init_crypto(0, NULL);
    ERR_clear_error();
    
    // Create memory BIO from input data
    BIO *mem_bio = BIO_new_mem_buf(data, size);
    if (mem_bio == NULL) {
        return 0;
    }
    
    // Parse ISSUING_DIST_POINT from input
    const unsigned char *p = data;
    ISSUING_DIST_POINT *idp = d2i_ISSUING_DIST_POINT(NULL, &p, size);
    if (idp != NULL) {
        ISSUING_DIST_POINT_free(idp);
    }
    
    // Reset BIO position
    BIO_ctrl(mem_bio, BIO_CTRL_RESET, 0, NULL);
    
    // Parse RSA private key from BIO
    RSA *rsa = d2i_RSAPrivateKey_bio(mem_bio, NULL);
    if (rsa != NULL) {
        // Get multi-prime factors
        const BIGNUM *primes[10];
        RSA_get0_multi_prime_factors(rsa, primes);
        RSA_free(rsa);
    }
    
    // Reset BIO position again
    BIO_ctrl(mem_bio, BIO_CTRL_RESET, 0, NULL);
    
    // Parse PKCS7 structure from BIO
    PKCS7 *p7 = d2i_PKCS7_bio(mem_bio, NULL);
    if (p7 != NULL) {
        // Create output BIO and print PKCS7
        BIO *out_bio = BIO_new(BIO_s_null());
        ASN1_PCTX *pctx = ASN1_PCTX_new();
        PKCS7_print_ctx(out_bio, p7, 0, pctx);
        
        ASN1_PCTX_free(pctx);
        BIO_free(out_bio);
        PKCS7_free(p7);
    }
    
    // Create OCSP request (dummy) and check count
    OCSP_REQUEST *ocsp_req = OCSP_REQUEST_new();
    if (ocsp_req != NULL) {
        OCSP_request_onereq_count(ocsp_req);
        OCSP_REQUEST_free(ocsp_req);
    }
    
    // Create X509_STORE with verify parameters
    X509_STORE *store = X509_STORE_new();
    if (store != NULL) {
        X509_STORE_set_ex_data(store, 0, NULL);
        
        X509_VERIFY_PARAM *param = X509_VERIFY_PARAM_new();
        if (param != NULL) {
            X509_STORE_set1_param(store, param);
            X509_VERIFY_PARAM_free(param);
        }
        X509_STORE_free(store);
    }
    
    // Create stack and push X509_EXTENSION
    OPENSSL_STACK *stack = OPENSSL_sk_new_null();
    if (stack != NULL) {
        // Set thunks (dummy function)
        OPENSSL_sk_set_thunks(stack, [](OPENSSL_sk_freefunc freefunc_arg, void *ptr) {});
        
        X509_EXTENSION *ext = X509_EXTENSION_new();
        if (ext != NULL) {
            ASN1_OCTET_STRING *os = ASN1_OCTET_STRING_new();
            if (os != NULL && size > 0) {
                // Use first min(100, size) bytes for data
                int len = size > 100 ? 100 : size;
                ASN1_OCTET_STRING_set(os, data, len);
                X509_EXTENSION_set_data(ext, os);
                ASN1_OCTET_STRING_free(os);
            }
            
            // Push extension to stack
            OPENSSL_sk_push(stack, ossl_check_X509_EXTENSION_type(ext));
            
            // Convert stack to DER (dummy output)
            unsigned char *der = NULL;
            i2d_X509_EXTENSIONS((X509_EXTENSIONS*)stack, &der);
            if (der != NULL) {
                CRYPTO_free(der, __FILE__, __LINE__);
            }
            
            X509_EXTENSION_free(ext);
        }
        OPENSSL_sk_free(stack);
    }
    
    // Create OSSL_PRIVILEGE_POLICY_ID
    OSSL_PRIVILEGE_POLICY_ID *policy = OSSL_PRIVILEGE_POLICY_ID_new();
    if (policy != NULL) {
        // Use NID_commonName as example
        policy->privilegePolicy = OBJ_nid2obj(NID_commonName);
        OSSL_PRIVILEGE_POLICY_ID_free(policy);
    }
    
    // Reset BIO and parse using ASN1_d2i_bio with DIST_POINT
    BIO_ctrl(mem_bio, BIO_CTRL_RESET, 0, NULL);
    void *dist_point = ASN1_d2i_bio(
        []() -> void* { return NULL; },
        [](void **a, const unsigned char **in, long len) -> void* {
            return d2i_ISSUING_DIST_POINT((ISSUING_DIST_POINT**)a, in, len);
        },
        mem_bio,
        NULL
    );
    if (dist_point != NULL) {
        ISSUING_DIST_POINT_free((ISSUING_DIST_POINT*)dist_point);
    }
    
    // Get EVP digest and cipher (not used, just called)
    const EVP_MD *sha224 = EVP_sha224();
    const EVP_CIPHER *rc2_cbc = EVP_rc2_cbc();
    
    // Cleanup
    BIO_free(mem_bio);
    
    return 0;
}