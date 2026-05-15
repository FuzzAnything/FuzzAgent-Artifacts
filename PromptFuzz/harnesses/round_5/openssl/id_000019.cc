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
//<ID> 219
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 4
//<Quality> {"density":40,"unique_branches":{"added_obj_hash":[[148,5,148,19,0,0,4,0]],"OBJ_nid2obj":[[264,37,264,65,0,0,4,1],[270,9,270,30,0,0,4,1],[276,9,276,20,0,0,4,1]]},"library_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","BIO_new_mem_buf","BIO_new","BIO_s_null","BIO_free","ASN1_d2i_bio","OCSP_request_onereq_count","OCSP_REQUEST_free","BIO_ctrl","X509_STORE_new","X509_STORE_set_ex_data","X509_VERIFY_PARAM_new","X509_STORE_set1_param","X509_VERIFY_PARAM_free","X509_STORE_free","d2i_RSAPrivateKey_bio","RSA_get0_multi_prime_factors","RSA_free","BIO_ctrl","DIST_POINT_it","EVP_rc2_cbc","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","X509_EXTENSION_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","X509_EXTENSION_set_data","OPENSSL_sk_push","ossl_check_X509_EXTENSION_sk_type","ossl_check_X509_EXTENSION_type","i2d_X509_EXTENSIONS","CRYPTO_free","ASN1_OCTET_STRING_free","X509_EXTENSION_free","OPENSSL_sk_free","ossl_check_X509_EXTENSION_sk_type","BIO_ctrl","OSSL_PRIVILEGE_POLICY_ID_new","OBJ_nid2obj","OSSL_PRIVILEGE_POLICY_ID_free","d2i_PKCS7_bio","ASN1_PCTX_new","PKCS7_print_ctx","ASN1_PCTX_free","PKCS7_free","BIO_free","BIO_free","ERR_clear_error"],"critical_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","BIO_new_mem_buf","BIO_new","BIO_s_null","ASN1_d2i_bio","OCSP_request_onereq_count","OCSP_REQUEST_free","BIO_ctrl","X509_STORE_new","X509_STORE_set_ex_data","X509_VERIFY_PARAM_new","X509_STORE_set1_param","X509_VERIFY_PARAM_free","X509_STORE_free","d2i_RSAPrivateKey_bio","RSA_get0_multi_prime_factors","RSA_free","BIO_ctrl","DIST_POINT_it","EVP_rc2_cbc","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","X509_EXTENSION_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","X509_EXTENSION_set_data","OPENSSL_sk_push","ossl_check_X509_EXTENSION_type","ossl_check_X509_EXTENSION_sk_type","i2d_X509_EXTENSIONS","CRYPTO_free","ASN1_OCTET_STRING_free","X509_EXTENSION_free","OPENSSL_sk_free","ossl_check_X509_EXTENSION_sk_type","BIO_ctrl","OSSL_PRIVILEGE_POLICY_ID_new","OBJ_nid2obj","OSSL_PRIVILEGE_POLICY_ID_free","d2i_PKCS7_bio","ASN1_PCTX_new","PKCS7_print_ctx","ASN1_PCTX_free","PKCS7_free","BIO_free","BIO_free","ERR_clear_error"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    OPENSSL_init_ssl(0, NULL);
    
    // Create memory BIO from input data
    BIO *mem_bio = BIO_new_mem_buf(data, size);
    if (!mem_bio) {
	return 0;
	}
	
    
    // Create output BIO (null device to avoid side effects)
    BIO *out_bio = BIO_new(BIO_s_null());
    if (!out_bio) {
        BIO_free(mem_bio);
        return 0;
    }
    
    // 1. Parse OCSP_REQUEST from input
    OCSP_REQUEST *ocsp_req = d2i_OCSP_REQUEST_bio(mem_bio, NULL);
    if (ocsp_req) {
        // Count one-request items
        int count = OCSP_request_onereq_count(ocsp_req);
        (void)count; // Use result to avoid compiler warning
        OCSP_REQUEST_free(ocsp_req);
    }
    
    // Reset BIO position
    BIO_reset(mem_bio);
    
    // 2. Create and configure X509_STORE
    X509_STORE *store = X509_STORE_new();
    if (store) {
        // Set ex_data
        int idx = 0; // Using 0 as dummy index
        void *ex_data = (void*)data; // Use input data as dummy ex_data
        X509_STORE_set_ex_data(store, idx, ex_data);
        
        // Set parameters
        X509_VERIFY_PARAM *param = X509_VERIFY_PARAM_new();
        if (param) {
            X509_STORE_set1_param(store, param);
            X509_VERIFY_PARAM_free(param);
        }
        
        X509_STORE_free(store);
    }
    
    // 3. Parse RSA key and get multi-prime factors
    RSA *rsa = d2i_RSAPrivateKey_bio(mem_bio, NULL);
    if (rsa) {
        const BIGNUM *primes[10]; // Assuming max 10 prime factors
        int num_primes = RSA_get0_multi_prime_factors(rsa, primes);
        (void)num_primes; // Use result
        RSA_free(rsa);
    }
    
    // Reset BIO position
    BIO_reset(mem_bio);
    
    // 4. Get DIST_POINT ASN1_ITEM
    const ASN1_ITEM *dist_point_it = DIST_POINT_it();
    (void)dist_point_it; // Use result
    
    // 5. Get RC2-CBC cipher
    const EVP_CIPHER *rc2_cbc = EVP_rc2_cbc();
    (void)rc2_cbc; // Use result
    
    // 6. Create dummy X509_EXTENSIONS and encode
    X509_EXTENSIONS *exts = sk_X509_EXTENSION_new_null();
    if (exts && size > 0) {
        // Create a simple extension using input data
        X509_EXTENSION *ext = X509_EXTENSION_new();
        if (ext) {
            ASN1_OCTET_STRING *os = ASN1_OCTET_STRING_new();
            if (os && size <= INT_MAX) {
                ASN1_OCTET_STRING_set(os, data, (int)size);
                X509_EXTENSION_set_data(ext, os);
                sk_X509_EXTENSION_push(exts, ext);
                
                // Encode extensions
                unsigned char *out = NULL;
                int len = i2d_X509_EXTENSIONS(exts, &out);
                if (out) OPENSSL_free(out);
                (void)len; // Use result
            }
            if (os) ASN1_OCTET_STRING_free(os);
            X509_EXTENSION_free(ext);
        }
        sk_X509_EXTENSION_free(exts);
    }
    
    // Reset BIO position
    BIO_reset(mem_bio);
    
    // 7. Create and free OSSL_PRIVILEGE_POLICY_ID
    OSSL_PRIVILEGE_POLICY_ID *policy_id = OSSL_PRIVILEGE_POLICY_ID_new();
    if (policy_id) {
        // Initialize with dummy data
        if (size > 0) {
            // Create dummy ASN1_OBJECT from first byte
            ASN1_OBJECT *obj = OBJ_nid2obj(data[0] % 1000);
            if (obj) {
                policy_id->privilegePolicy = obj;
            }
        }
        OSSL_PRIVILEGE_POLICY_ID_free(policy_id);
    }
    
    // 8. Parse PKCS7 and print
    PKCS7 *pkcs7 = d2i_PKCS7_bio(mem_bio, NULL);
    if (pkcs7) {
        // Create ASN1_PCTX for printing
        ASN1_PCTX *pctx = ASN1_PCTX_new();
        if (pctx) {
            PKCS7_print_ctx(out_bio, pkcs7, 4, pctx);
            ASN1_PCTX_free(pctx);
        }
        PKCS7_free(pkcs7);
    }
    
    // Cleanup
    BIO_free(mem_bio);
    BIO_free(out_bio);
    
    // Clear any remaining errors
    ERR_clear_error();
    
    return 0;
}