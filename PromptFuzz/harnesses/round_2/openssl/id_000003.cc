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
//<ID> 58
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["OSSL_LIB_CTX_new","SRP_create_verifier_ex","CRYPTO_free","CRYPTO_free","CRYPTO_free","ASN1_item_d2i_fp","ASN1_SEQUENCE_it","ASN1_item_free","ASN1_SEQUENCE_it","SSL_CTX_new","TLS_method","SSL_CTX_get_record_padding_callback_arg","SSL_new","SSL_set1_host","X509_STORE_new","X509_STORE_add_lookup","X509_LOOKUP_file","X509_LOOKUP_get_store","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_const_void_sk_type","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_const_UI_STRING_sk_type","OSSL_FUNC_rand_set_callbacks","ASN1_item_d2i_fp","X509_it","X509_free","OPENSSL_sk_free","ossl_check_void_sk_type","OPENSSL_sk_free","ossl_check_UI_STRING_sk_type","SSL_free","SSL_CTX_free","X509_STORE_free","OSSL_LIB_CTX_free"],"critical_calls":["OSSL_LIB_CTX_new","SRP_create_verifier_ex","CRYPTO_free","CRYPTO_free","CRYPTO_free","ASN1_item_d2i_fp","ASN1_SEQUENCE_it","ASN1_item_free","ASN1_SEQUENCE_it","SSL_CTX_new","TLS_method","SSL_CTX_get_record_padding_callback_arg","SSL_new","SSL_set1_host","X509_STORE_new","X509_STORE_add_lookup","X509_LOOKUP_file","X509_LOOKUP_get_store","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_const_void_sk_type","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_const_UI_STRING_sk_type","OSSL_FUNC_rand_set_callbacks","ASN1_item_d2i_fp","X509_it","X509_free","OPENSSL_sk_free","ossl_check_void_sk_type","OPENSSL_sk_free","ossl_check_UI_STRING_sk_type","SSL_free","SSL_CTX_free","X509_STORE_free","OSSL_LIB_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Create and free OSSL_LIB_CTX for SRP operations
    OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
    
    // 2. Use SRP_create_verifier_ex with dummy parameters
    char *salt = nullptr;
    char *verifier = nullptr;
    // Using safe SRP parameters (dummy values since we don't have real user/pass)
    const char *N = "EEAF0AB9ADB38DD69C33F80AFA8FC5E86072618775FF3C0B9EA2314C"
                    "9C256576D674DF7496EA81D3383B4813D692C6E0E0D5D8E250B98BE4"
                    "8E495C1D6089DAD15DC7D7B46154D6B6CE8EF4AD69B15D4982559B29"
                    "7BCF1885C529F566660E57EC68EDBC3C05726CC02FD4CBF4976EAA9A"
                    "FD5138FE8376435B9FC61D2FC0EB06E3";
    const char *g = "2";
    
    // Try to create verifier with dummy user/pass
    char *result = SRP_create_verifier_ex("testuser", "testpass", &salt, &verifier, 
                                          N, g, libctx, nullptr);
    
    // Clean up SRP results
    OPENSSL_free(result);
    OPENSSL_free(salt);
    OPENSSL_free(verifier);
    
    // 3. Use ASN1_item_d2i_fp to read ASN.1 data from input
    // Try to read as generic ASN.1 sequence
    void *asn1_data = ASN1_item_d2i_fp(ASN1_SEQUENCE_it(), in_file, nullptr);
    
    // Clean up ASN.1 data if read successfully
    if (asn1_data) {
        ASN1_item_free(static_cast<ASN1_VALUE*>(asn1_data), ASN1_SEQUENCE_it());
    }
    
    // Reset file pointer for potential reuse
    rewind(in_file);
    
    // 4. Create SSL_CTX and use SSL_CTX_get_record_padding_callback_arg
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    if (ssl_ctx) {
        void *padding_arg = SSL_CTX_get_record_padding_callback_arg(ssl_ctx);
        (void)padding_arg; // Use variable to avoid unused warning
    }
    
    // 5. Create SSL and use SSL_set1_host
    SSL *ssl = nullptr;
    if (ssl_ctx) {
        ssl = SSL_new(ssl_ctx);
        if (ssl) {
            SSL_set1_host(ssl, "example.com");
        }
    }
    
    // 6. Create X509_STORE and X509_LOOKUP, then use X509_LOOKUP_get_store
    X509_STORE *store = X509_STORE_new();
    X509_LOOKUP *lookup = nullptr;
    if (store) {
        lookup = X509_STORE_add_lookup(store, X509_LOOKUP_file());
        if (lookup) {
            X509_STORE *retrieved_store = X509_LOOKUP_get_store(lookup);
            (void)retrieved_store; // Use variable
        }
    }
    
    // 7. Create stacks and use type-checking functions
    struct stack_st_void *void_stack = sk_void_new_null();
    if (void_stack) {
        const OPENSSL_STACK *checked_void_stack = 
            ossl_check_const_void_sk_type(void_stack);
        (void)checked_void_stack; // Use variable
    }
    
    struct stack_st_UI_STRING *ui_string_stack = sk_UI_STRING_new_null();
    if (ui_string_stack) {
        const OPENSSL_STACK *checked_ui_stack = 
            ossl_check_const_UI_STRING_sk_type(ui_string_stack);
        (void)checked_ui_stack; // Use variable
    }
    
    // 8. Try to use OSSL_FUNC_rand_set_callbacks
    // This requires an OSSL_DISPATCH structure - create minimal one
    OSSL_DISPATCH dispatch_table[] = {
        {0, nullptr}
    };
    OSSL_FUNC_rand_set_callbacks_fn *rand_cb_func = 
        OSSL_FUNC_rand_set_callbacks(dispatch_table);
    (void)rand_cb_func; // Use variable
    
    // 9. Read more ASN.1 data with different item type
    rewind(in_file);
    void *x509_data = ASN1_item_d2i_fp(X509_it(), in_file, nullptr);
    if (x509_data) {
        X509_free(static_cast<X509*>(x509_data));
    }
    
    // Clean up resources
    if (void_stack) sk_void_free(void_stack);
    if (ui_string_stack) sk_UI_STRING_free(ui_string_stack);
    if (ssl) SSL_free(ssl);
    if (ssl_ctx) SSL_CTX_free(ssl_ctx);
    if (store) X509_STORE_free(store);
    
    // Free OSSL_LIB_CTX (this corresponds to OSSL_LIB_CTX_free)
    if (libctx) OSSL_LIB_CTX_free(libctx);
    
    // Close file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}