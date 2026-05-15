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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"ASN1_mbstring_ncopy":[[116,26,116,43,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","ERR_clear_error","SSL_CTX_new","TLS_method","X509_new","SSL_CTX_use_certificate","X509_free","X509_NAME_new","X509_NAME_add_entry_by_txt","X509_REQ_new","X509_REQ_set_subject_name","X509_NAME_free","BN_new","BN_bin2bn","BN_new","BN_new","BN_new","BN_set_word","BN_set_word","BN_set_word","SRP_Calc_B_ex","BN_free","BN_free","BN_free","BN_free","BN_free","EVP_MD_CTX_new","EVP_MD_CTX_new","EVP_MD_CTX_gettable_params","BIO_dgram_non_fatal_error","OCSP_SIGNATURE_it","OPENSSL_sk_new_null","OPENSSL_sk_set_thunks","OPENSSL_sk_push","OSSL_TARGET_new","sk_OSSL_TARGETS_cmpfunc_thunk","OSSL_TARGET_free","ossl_check_OSSL_TARGET_sk_type","EVP_PKEY_CTX_new_id","EVP_PKEY_decapsulate","EVP_PKEY_CTX_free","X509_REQ_free","EVP_MD_CTX_free","EVP_MD_CTX_free","OPENSSL_sk_free","SSL_CTX_free"],"critical_calls":["OPENSSL_init_ssl","ERR_clear_error","SSL_CTX_new","TLS_method","X509_new","SSL_CTX_use_certificate","X509_free","X509_NAME_new","X509_NAME_add_entry_by_txt","X509_REQ_new","X509_REQ_set_subject_name","X509_NAME_free","BN_new","BN_bin2bn","BN_new","BN_new","BN_new","BN_set_word","BN_set_word","BN_set_word","SRP_Calc_B_ex","BN_free","BN_free","BN_free","BN_free","BN_free","EVP_MD_CTX_new","EVP_MD_CTX_new","EVP_MD_CTX_gettable_params","BIO_dgram_non_fatal_error","OCSP_SIGNATURE_it","OPENSSL_sk_new_null","OPENSSL_sk_set_thunks","OPENSSL_sk_push","OSSL_TARGET_new","sk_OSSL_TARGETS_cmpfunc_thunk","OSSL_TARGET_free","ossl_check_OSSL_TARGET_sk_type","EVP_PKEY_CTX_new_id","EVP_PKEY_decapsulate","EVP_PKEY_CTX_free","X509_REQ_free","EVP_MD_CTX_free","EVP_MD_CTX_free","OPENSSL_sk_free","SSL_CTX_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize SSL
    OPENSSL_init_ssl(0, nullptr);
    
    // 2. Clear any previous errors
    ERR_clear_error();
    
    // 3. Create SSL context
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    if (!ctx) {
	return 0;
	}
	
    
    // 4. Create and use certificate (simulated)
    // We'll create a minimal X509 structure for testing
    X509 *cert = X509_new();
    if (cert) {
        SSL_CTX_use_certificate(ctx, cert);
        X509_free(cert);
    }
    
    // 5. Create X509_NAME and add entries from input data
    X509_NAME *name = X509_NAME_new();
    if (name && size > 10) {
        // Use part of input data for the name entry
        size_t name_len = size > 100 ? 100 : size;
        X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_UTF8, 
                                   data, name_len, -1, 0);
    }
    
    // 6. Create X509_REQ and set subject name
    X509_REQ *req = X509_REQ_new();
    if (req && name) {
        X509_REQ_set_subject_name(req, name);
    }
    
    // 7. Clean up name
    X509_NAME_free(name);
    
    // 8. Create BIGNUM from input data
    BIGNUM *bn = BN_new();
    if (bn && size > 0) {
        size_t bn_len = size > 32 ? 32 : size;
        BN_bin2bn(data, bn_len, bn);
    }
    
    // 9. Perform SRP calculation
    BIGNUM *N = BN_new();
    BIGNUM *g = BN_new();
    BIGNUM *v = BN_new();
    BIGNUM *srp_result = nullptr;
    
    if (bn && N && g && v) {
        // Initialize with some dummy values for SRP
        BN_set_word(N, 0x10001);
        BN_set_word(g, 2);
        BN_set_word(v, 3);
        
        srp_result = SRP_Calc_B_ex(bn, N, g, v, nullptr, nullptr);
    }
    
    // 10. Clean up BIGNUMs
    BN_free(bn);
    BN_free(N);
    BN_free(g);
    BN_free(v);
    BN_free(srp_result);
    
    // 11. Create EVP_MD_CTX and get table params
    EVP_MD_CTX *md_ctx1 = EVP_MD_CTX_new();
    EVP_MD_CTX *md_ctx2 = EVP_MD_CTX_new();
    
    if (md_ctx1) {
        const OSSL_PARAM *gettable = EVP_MD_CTX_gettable_params(md_ctx1);
        (void)gettable; // Use to avoid unused variable warning
    }
    
    // 12. Check BIO dgram error
    int dummy_error = 0;
    BIO_dgram_non_fatal_error(dummy_error);
    
    // 13. Get OCSP signature ASN1 item
    const ASN1_ITEM *ocsp_item = OCSP_SIGNATURE_it();
    (void)ocsp_item;
    
    // 14. Create and manipulate OPENSSL_STACK with thunks
    OPENSSL_STACK *stack = OPENSSL_sk_new_null();
    if (stack) {
        // Create a dummy thunk function
        auto dummy_thunk = [](OPENSSL_sk_freefunc freefunc_arg, void *ptr) {
            if (freefunc_arg && ptr) {
                freefunc_arg(ptr);
            }
        };
        
        OPENSSL_sk_set_thunks(stack, dummy_thunk);
        
        // Add some dummy data to stack
        int dummy_data = 42;
        OPENSSL_sk_push(stack, &dummy_data);
    }
    
    // 15. Create and manipulate OSSL_TARGET stack
    OSSL_TARGET *target = OSSL_TARGET_new();
    if (target) {
        // Create comparison thunk
        auto cmp_thunk = [](int (*cmp)(const void *, const void *), 
                           const void *a, const void *b) {
            return cmp ? cmp(a, b) : 0;
        };
        
        // Use the thunk
        int cmp_result = sk_OSSL_TARGETS_cmpfunc_thunk(
            [](const void *a, const void *b) { return 0; }, 
            target, target);
        (void)cmp_result;
        
        OSSL_TARGET_free(target);
    }
    
    // 16. Type check for OSSL_TARGET stack
    struct stack_st_OSSL_TARGET *target_stack = nullptr;
    OPENSSL_STACK *checked_stack = ossl_check_OSSL_TARGET_sk_type(target_stack);
    (void)checked_stack;
    
    // 17. Create EVP_PKEY_CTX and attempt decapsulation
    EVP_PKEY_CTX *pkey_ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
    if (pkey_ctx && size > 0) {
        unsigned char *unwrapped = nullptr;
        size_t unwrapped_len = 0;
        
        // Use input data as wrapped data
        size_t wrapped_len = size > 256 ? 256 : size;
        EVP_PKEY_decapsulate(pkey_ctx, unwrapped, &unwrapped_len, 
                            data, wrapped_len);
        
        EVP_PKEY_CTX_free(pkey_ctx);
    }
    
    // 18. Clean up remaining resources
    X509_REQ_free(req);
    EVP_MD_CTX_free(md_ctx1);
    EVP_MD_CTX_free(md_ctx2);
    OPENSSL_sk_free(stack);
    SSL_CTX_free(ctx);
    
    return 0;
}