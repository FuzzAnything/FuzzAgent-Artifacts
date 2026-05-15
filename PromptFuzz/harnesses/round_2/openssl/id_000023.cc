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
//<ID> 513
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"PKCS12_free":[[34,9,34,20,0,0,4,1]],"int_ctx_new":[[122,13,122,28,0,0,4,0]]},"library_calls":["OPENSSL_init_crypto","ERR_clear_error","OPENSSL_sk_new_null","OPENSSL_sk_set_thunks","OSSL_CMP_CTX_new","X509_STORE_new","X509_STORE_CTX_new","X509_STORE_CTX_set_error_depth","ossl_check_ASN1_STRING_TABLE_copyfunc_type","PKCS12_AUTHSAFES_it","d2i_PKCS12_fp","PKCS12_free","OSSL_ENCODER_fetch","X509_REQ_new","EVP_MD_CTX_new","EVP_PKEY_new","EVP_DigestSignInit","EVP_sha256","X509_REQ_sign_ctx","EVP_PKEY_free","EVP_MD_CTX_free","OPENSSL_store_u16_le","DSA_meth_new","DSA_meth_set_mod_exp","OSSL_STORE_LOADER_new","OSSL_STORE_LOADER_get0_provider","OPENSSL_LH_new","ossl_check_CONF_VALUE_lh_hashfunc_type","ossl_check_CONF_VALUE_lh_compfunc_type","OPENSSL_LH_set_thunks","ossl_check_CONF_VALUE_lh_type","ossl_check_ASN1_STRING_TABLE_sk_type","ossl_check_X509_sk_type","OSSL_CMP_CTX_build_cert_chain","OPENSSL_sk_free","OSSL_ENCODER_free","X509_REQ_free","DSA_meth_free","OSSL_STORE_LOADER_free","OPENSSL_LH_free","OSSL_CMP_CTX_free","X509_STORE_CTX_free","X509_STORE_free"],"critical_calls":["OPENSSL_init_crypto","ERR_clear_error","OPENSSL_sk_new_null","OPENSSL_sk_set_thunks","OSSL_CMP_CTX_new","X509_STORE_new","X509_STORE_CTX_new","X509_STORE_CTX_set_error_depth","ossl_check_ASN1_STRING_TABLE_copyfunc_type","PKCS12_AUTHSAFES_it","d2i_PKCS12_fp","PKCS12_free","OSSL_ENCODER_fetch","X509_REQ_new","EVP_MD_CTX_new","EVP_PKEY_new","EVP_DigestSignInit","EVP_sha256","X509_REQ_sign_ctx","EVP_PKEY_free","EVP_MD_CTX_free","OPENSSL_store_u16_le","DSA_meth_new","DSA_meth_set_mod_exp","OSSL_STORE_LOADER_new","OSSL_STORE_LOADER_get0_provider","OPENSSL_LH_new","ossl_check_CONF_VALUE_lh_compfunc_type","ossl_check_CONF_VALUE_lh_hashfunc_type","OPENSSL_LH_set_thunks","ossl_check_CONF_VALUE_lh_type","ossl_check_ASN1_STRING_TABLE_sk_type","ossl_check_X509_sk_type","OSSL_CMP_CTX_build_cert_chain","OPENSSL_sk_free","OSSL_ENCODER_free","X509_REQ_free","DSA_meth_free","OSSL_STORE_LOADER_free","OPENSSL_LH_free","OSSL_CMP_CTX_free","X509_STORE_CTX_free","X509_STORE_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL
    if (!OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, nullptr)) {
        return 0;
    }
    
    // Clear any previous errors
    ERR_clear_error();
    
    // Create and configure OPENSSL_STACK with thunks
    OPENSSL_STACK *stack = OPENSSL_sk_new_null();
    if (stack) {
        OPENSSL_sk_set_thunks(stack, nullptr); // No thunk function for this test
    }
    
    // Create OSSL_CMP context
    OSSL_CMP_CTX *cmp_ctx = OSSL_CMP_CTX_new(nullptr, nullptr);
    
    // Create X509_STORE for certificate chain building
    X509_STORE *store = X509_STORE_new();
    
    // Create X509_STORE_CTX for certificate verification
    X509_STORE_CTX *store_ctx = X509_STORE_CTX_new();
    if (store_ctx) {
        X509_STORE_CTX_set_error_depth(store_ctx, 1); // Set error depth to 1
    }
    
    // Use type checking functions (no actual objects needed for these)
    OPENSSL_sk_copyfunc copy_func = ossl_check_ASN1_STRING_TABLE_copyfunc_type(nullptr);
    const ASN1_ITEM *pkcs12_item = PKCS12_AUTHSAFES_it();
    
    // Read PKCS12 from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        PKCS12 *p12 = d2i_PKCS12_fp(in_file, nullptr);
        assert_file_closed(&in_file);;
        
        // Clean up PKCS12 if allocated
        PKCS12_free(p12);
    }
    
    // Fetch an encoder (for potential encoding operations)
    OSSL_ENCODER *encoder = OSSL_ENCODER_fetch(nullptr, "DER", nullptr);
    
    // Create X509_REQ and sign it
    X509_REQ *req = X509_REQ_new();
    if (req) {
        EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
        if (md_ctx) {
            EVP_PKEY_CTX *pkey_ctx = nullptr;
            // Create a temporary RSA key for signing
            EVP_PKEY *pkey = EVP_PKEY_new();
            if (pkey) {
                // Note: In real usage, we would set up key parameters
                // Initialize digest signing context
                if (EVP_DigestSignInit(md_ctx, &pkey_ctx, EVP_sha256(), nullptr, pkey)) {
                    // Sign the X509 request
                    X509_REQ_sign_ctx(req, md_ctx);
                }
                EVP_PKEY_free(pkey);
            }
            EVP_MD_CTX_free(md_ctx);
        }
    }
    
    // Use OPENSSL_store_u16_le for little-endian storage
    unsigned char buffer[2];
    OPENSSL_store_u16_le(buffer, 0x1234);
    
    // Create DSA method and set mod_exp function
    DSA_METHOD *dsa_meth = DSA_meth_new("Test DSA", 0);
    if (dsa_meth) {
        DSA_meth_set_mod_exp(dsa_meth, nullptr); // Using nullptr for this test
    }
    
    // Create OSSL_STORE_LOADER
    OSSL_STORE_LOADER *loader = OSSL_STORE_LOADER_new(nullptr, "file");
    if (loader) {
        const OSSL_PROVIDER *provider = OSSL_STORE_LOADER_get0_provider(loader);
        (void)provider; // Prevent unused variable warning
    }
    
    // Create and configure OPENSSL_LHASH with thunks
    OPENSSL_LHASH *lh = OPENSSL_LH_new(
        ossl_check_CONF_VALUE_lh_hashfunc_type(nullptr),
        ossl_check_CONF_VALUE_lh_compfunc_type(nullptr)
    );
    if (lh) {
        OPENSSL_LH_set_thunks(lh, nullptr, nullptr, nullptr, nullptr);
    }
    
    // Type checking for lhash
    struct lhash_st_CONF_VALUE *conf_lh = nullptr;
    OPENSSL_LHASH *checked_lh = ossl_check_CONF_VALUE_lh_type(conf_lh);
    (void)checked_lh; // Prevent unused variable warning
    
    // Type checking for stacks
    struct stack_st_ASN1_STRING_TABLE *astack = nullptr;
    OPENSSL_STACK *checked_astack = ossl_check_ASN1_STRING_TABLE_sk_type(astack);
    (void)checked_astack; // Prevent unused variable warning
    
    struct stack_st_X509 *x509stack = nullptr;
    OPENSSL_STACK *checked_x509stack = ossl_check_X509_sk_type(x509stack);
    (void)checked_x509stack; // Prevent unused variable warning
    
    // Build certificate chain (using empty candidates for this test)
    if (cmp_ctx && store) {
        OSSL_CMP_CTX_build_cert_chain(cmp_ctx, store, nullptr);
    }
    
    // Free all allocated resources
    if (stack) OPENSSL_sk_free(stack);
    if (encoder) OSSL_ENCODER_free(encoder);
    if (req) X509_REQ_free(req);
    if (dsa_meth) DSA_meth_free(dsa_meth);
    if (loader) OSSL_STORE_LOADER_free(loader);
    if (lh) OPENSSL_LH_free(lh);
    if (cmp_ctx) OSSL_CMP_CTX_free(cmp_ctx);
    if (store_ctx) X509_STORE_CTX_free(store_ctx);
    if (store) X509_STORE_free(store);
    
    assert_file_closed(&in_file);
	return 0;
}