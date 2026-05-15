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
//<ID> 225
//<Prompt> []
/*<Combination>: [
*/
//<score> 80, nr_unique_branch: 8
//<Quality> {"density":20,"unique_branches":{"null_ctrl":[[55,5,55,24,0,0,4,0],[68,5,68,12,0,0,4,1]],"file_puts":[[389,9,389,20,0,0,4,1]],"file_ctrl":[[188,5,188,25,0,0,4,0],[190,13,190,49,0,0,4,1],[357,5,357,23,0,0,4,0],[358,5,358,22,0,0,4,0],[359,5,359,12,0,0,4,0]]},"library_calls":["OPENSSL_init_crypto","ERR_clear_error","BIO_new_fp","ERR_clear_error","d2i_ISSUING_DIST_POINT","DIST_POINT_it","ISSUING_DIST_POINT_free","ASN1_d2i_bio","OCSP_request_onereq_count","OCSP_REQUEST_free","BIO_ctrl","d2i_RSAPrivateKey_bio","RSA_get0_multi_prime_factors","RSA_free","BIO_ctrl","d2i_PKCS7_bio","BIO_new_fp","ASN1_PCTX_new","PKCS7_print_ctx","ASN1_PCTX_free","BIO_free","PKCS7_free","X509_STORE_new","X509_VERIFY_PARAM_new","X509_STORE_set1_param","X509_STORE_set_ex_data","X509_VERIFY_PARAM_free","X509_STORE_free","OPENSSL_sk_new_null","OPENSSL_sk_set_thunks","X509_EXTENSION_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","X509_EXTENSION_set_data","ASN1_OCTET_STRING_free","OPENSSL_sk_push","ossl_check_X509_EXTENSION_sk_type","ossl_check_X509_EXTENSION_type","i2d_X509_EXTENSIONS","CRYPTO_free","X509_EXTENSION_free","OPENSSL_sk_free","OSSL_PRIVILEGE_POLICY_ID_new","OBJ_nid2obj","OSSL_PRIVILEGE_POLICY_ID_free","EVP_rc2_cbc","BIO_s_null","BIO_new","BIO_ctrl","BIO_free","BIO_ctrl","ASN1_d2i_bio","X509_free","BIO_free","ERR_clear_error","CRYPTO_free"],"critical_calls":["OPENSSL_init_crypto","BIO_new_fp","d2i_ISSUING_DIST_POINT","DIST_POINT_it","ISSUING_DIST_POINT_free","ASN1_d2i_bio","OCSP_request_onereq_count","OCSP_REQUEST_free","BIO_ctrl","d2i_RSAPrivateKey_bio","RSA_get0_multi_prime_factors","RSA_free","BIO_ctrl","d2i_PKCS7_bio","BIO_new_fp","ASN1_PCTX_new","PKCS7_print_ctx","ASN1_PCTX_free","BIO_free","PKCS7_free","X509_STORE_new","X509_VERIFY_PARAM_new","X509_STORE_set1_param","X509_STORE_set_ex_data","X509_VERIFY_PARAM_free","X509_STORE_free","OPENSSL_sk_new_null","OPENSSL_sk_set_thunks","X509_EXTENSION_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","X509_EXTENSION_set_data","ASN1_OCTET_STRING_free","OPENSSL_sk_push","ossl_check_X509_EXTENSION_type","ossl_check_X509_EXTENSION_sk_type","i2d_X509_EXTENSIONS","CRYPTO_free","X509_EXTENSION_free","OPENSSL_sk_free","OSSL_PRIVILEGE_POLICY_ID_new","OBJ_nid2obj","OSSL_PRIVILEGE_POLICY_ID_free","EVP_rc2_cbc","BIO_s_null","BIO_new","BIO_ctrl","BIO_free","BIO_ctrl","ASN1_d2i_bio","X509_free","BIO_free","ERR_clear_error","CRYPTO_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | OPENSSL_INIT_ADD_ALL_CIPHERS, nullptr);

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        ERR_clear_error();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create BIO from FILE*
    BIO *bio = BIO_new_fp(in_file, BIO_NOCLOSE);
    if (!bio) {
        assert_file_closed(&in_file);;
        ERR_clear_error();
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Parse ISSUING_DIST_POINT from input data
    const unsigned char *p = data;
    ISSUING_DIST_POINT *idp = d2i_ISSUING_DIST_POINT(nullptr, &p, size);
    if (idp) {
        // Use DIST_POINT_it() for reference
        const ASN1_ITEM *dist_point_it = DIST_POINT_it();
        (void)dist_point_it; // Reference to satisfy API usage
        ISSUING_DIST_POINT_free(idp);
    }

    // 2. Parse OCSP_REQUEST from BIO
    OCSP_REQUEST *ocsp_req = d2i_OCSP_REQUEST_bio(bio, nullptr);
    if (ocsp_req) {
        int count = OCSP_request_onereq_count(ocsp_req);
        (void)count; // Use the count
        OCSP_REQUEST_free(ocsp_req);
    }

    // Reset BIO position
    BIO_seek(bio, 0);

    // 3. Parse RSA private key from BIO
    RSA *rsa = d2i_RSAPrivateKey_bio(bio, nullptr);
    if (rsa) {
        const BIGNUM *primes[10];
        int num_factors = RSA_get0_multi_prime_factors(rsa, primes);
        (void)num_factors; // Use the factor count
        RSA_free(rsa);
    }

    // Reset BIO position
    BIO_seek(bio, 0);

    // 4. Parse PKCS7 structure from BIO
    PKCS7 *p7 = d2i_PKCS7_bio(bio, nullptr);
    if (p7) {
        // Create output FILE* for printing
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            BIO *out_bio = BIO_new_fp(out_file, BIO_NOCLOSE);
            if (out_bio) {
                ASN1_PCTX *pctx = ASN1_PCTX_new();
                if (pctx) {
                    PKCS7_print_ctx(out_bio, p7, 4, pctx);
                    ASN1_PCTX_free(pctx);
                }
                BIO_free(out_bio);
            }
            assert_file_closed(&out_file);;
        }
        PKCS7_free(p7);
    }

    // 5. Create and manipulate X509_STORE with X509_VERIFY_PARAM
    X509_STORE *store = X509_STORE_new();
    if (store) {
        X509_VERIFY_PARAM *param = X509_VERIFY_PARAM_new();
        if (param) {
            X509_STORE_set1_param(store, param);
            // Set ex_data (example index 0 with nullptr)
            X509_STORE_set_ex_data(store, 0, nullptr);
            X509_VERIFY_PARAM_free(param);
        }
        X509_STORE_free(store);
    }

    // 6. Create and manipulate OPENSSL_STACK of X509_EXTENSION
    OPENSSL_STACK *stack = OPENSSL_sk_new_null();
    if (stack) {
        // Set thunks (using a dummy function)
        OPENSSL_sk_set_thunks(stack, [](OPENSSL_sk_freefunc freefunc_arg, void *ptr) {
            // This is a thunk that does nothing special
            if (freefunc_arg && ptr) freefunc_arg(ptr);
        });

        X509_EXTENSION *ext = X509_EXTENSION_new();
        if (ext) {
            ASN1_OCTET_STRING *octet = ASN1_OCTET_STRING_new();
            if (octet && size > 0) {
                // Use first min(10, size) bytes for demonstration
                int len = size > 10 ? 10 : size;
                ASN1_OCTET_STRING_set(octet, data, len);
                X509_EXTENSION_set_data(ext, octet);
                ASN1_OCTET_STRING_free(octet);
            }

            // Push to stack using type-checked functions
            OPENSSL_sk_push(ossl_check_X509_EXTENSION_sk_type(
                reinterpret_cast<struct stack_st_X509_EXTENSION*>(stack)),
                ossl_check_X509_EXTENSION_type(ext));
            
            // Convert stack to DER (just for demonstration)
            unsigned char *der = nullptr;
            int der_len = i2d_X509_EXTENSIONS(
                reinterpret_cast<const X509_EXTENSIONS*>(stack), &der);
            if (der) {
                OPENSSL_free(der);
            }

            X509_EXTENSION_free(ext);
        }
        OPENSSL_sk_free(stack);
    }

    // 7. Create OSSL_PRIVILEGE_POLICY_ID
    OSSL_PRIVILEGE_POLICY_ID *policy_id = OSSL_PRIVILEGE_POLICY_ID_new();
    if (policy_id) {
        ASN1_OBJECT *obj = OBJ_nid2obj(NID_commonName);
        if (obj) {
            policy_id->privilegePolicy = obj;
        }
        OSSL_PRIVILEGE_POLICY_ID_free(policy_id);
    }

    // 8. Use EVP_rc2_cbc() cipher (just for reference)
    const EVP_CIPHER *cipher = EVP_rc2_cbc();
    (void)cipher;

    // 9. Use BIO_s_null() and BIO_ctrl()
    const BIO_METHOD *null_method = BIO_s_null();
    BIO *null_bio = BIO_new(null_method);
    if (null_bio) {
        BIO_ctrl(null_bio, BIO_CTRL_RESET, 0, nullptr);
        BIO_free(null_bio);
    }

    // 10. Use ASN1_d2i_bio() for generic parsing (example with X509)
    BIO_seek(bio, 0);
    X509 *x509 = nullptr;
    ASN1_d2i_bio(
        (void *(*)(void))X509_new,
        (d2i_of_void *)d2i_X509,
        bio,
        (void **)&x509
    );
    if (x509) {
        X509_free(x509);
    }

    // Cleanup
    BIO_free(bio);
    assert_file_closed(&in_file);;
    
    // Clear any remaining errors
    ERR_clear_error();
    
    // Free any remaining allocated memory using CRYPTO_free
    // (Note: Most structures have their own free functions, but we demonstrate CRYPTO_free)
    void *dummy = nullptr;
    CRYPTO_free(dummy, __FILE__, __LINE__);

    assert_file_closed(&in_file);
	return 0;
}