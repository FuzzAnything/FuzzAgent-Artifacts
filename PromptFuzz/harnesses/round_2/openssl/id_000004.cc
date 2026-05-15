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
//<ID> 59
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["ASN1_ANY_it","ASN1_item_d2i_fp","OSSL_LIB_CTX_new","SRP_create_verifier_ex","SSL_CTX_new","TLS_method","SSL_new","SSL_set1_host","SSL_CTX_get_record_padding_callback_arg","X509_STORE_new","X509_STORE_add_lookup","X509_LOOKUP_file","X509_LOOKUP_get_store","OPENSSL_sk_new","OPENSSL_sk_push","ossl_check_const_void_sk_type","OPENSSL_sk_new","ossl_check_const_UI_STRING_sk_type","OSSL_FUNC_rand_set_callbacks","CRYPTO_free","CRYPTO_free","CRYPTO_free","ASN1_item_free","OPENSSL_sk_free","OPENSSL_sk_free","SSL_free","SSL_CTX_free","X509_STORE_free","OSSL_LIB_CTX_free"],"critical_calls":["ASN1_ANY_it","ASN1_item_d2i_fp","OSSL_LIB_CTX_new","SRP_create_verifier_ex","SSL_CTX_new","TLS_method","SSL_new","SSL_set1_host","SSL_CTX_get_record_padding_callback_arg","X509_STORE_new","X509_STORE_add_lookup","X509_LOOKUP_file","X509_LOOKUP_get_store","OPENSSL_sk_new","OPENSSL_sk_push","ossl_check_const_void_sk_type","OPENSSL_sk_new","ossl_check_const_UI_STRING_sk_type","OSSL_FUNC_rand_set_callbacks","CRYPTO_free","CRYPTO_free","CRYPTO_free","ASN1_item_free","OPENSSL_sk_free","OPENSSL_sk_free","SSL_free","SSL_CTX_free","X509_STORE_free","OSSL_LIB_CTX_free"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if no data
    if (size == 0) {
        return 0;
    }

    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create FILE* for writing output (though we won't actually write in this fuzzer)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Variables to be used with the APIs
    OSSL_LIB_CTX *libctx = nullptr;
    SSL_CTX *ssl_ctx = nullptr;
    SSL *ssl = nullptr;
    X509_LOOKUP *lookup = nullptr;
    struct stack_st_void *void_stack = nullptr;
    struct stack_st_UI_STRING *ui_string_stack = nullptr;
    char *salt = nullptr;
    char *verifier = nullptr;
    const OSSL_DISPATCH *dispatch_table = nullptr;
    
    // 1. Use ASN1_item_d2i_fp to read ASN.1 data from input
    // We'll try to read a generic ASN.1 structure
    const ASN1_ITEM *it = ASN1_ITEM_ptr(ASN1_ANY_it);
    void *asn1_data = ASN1_item_d2i_fp(it, in_file, nullptr);
    
    // Rewind the file for potential reuse
    rewind(in_file);
    
    // 2. Create and free OSSL_LIB_CTX
    libctx = OSSL_LIB_CTX_new();
    
    // 3. Use SRP_create_verifier_ex (with some dummy parameters)
    // We'll use parts of input data for user and password
    const char *user = "testuser";
    const char *pass = "testpass";
    const char *N = "EEAF0AB9ADB38DD69C33F80AFA8FC5E86072618775FF3C0B9EA2314C"
                    "9C256576D674DF7496EA81D3383B4813D692C6E0E0D5D8E250B98BE4"
                    "8E495C1D6089DAD15DC7D7B46154D6B6CE8EF4AD69B15D4982559B29"
                    "7BCF1885C529F566660E57EC68EDBC3C05726CC02FD4CBF4976EAA9A"
                    "FD5138FE8376435B9FC61D2FC0EB06E3";
    const char *g = "2";
    
    char *srp_result = SRP_create_verifier_ex(user, pass, &salt, &verifier, 
                                              N, g, libctx, nullptr);
    
    // 4. Create SSL_CTX and SSL for SSL_set1_host
    ssl_ctx = SSL_CTX_new(TLS_method());
    if (ssl_ctx != nullptr) {
        ssl = SSL_new(ssl_ctx);
        
        // Use SSL_set1_host with a dummy hostname
        if (ssl != nullptr) {
            SSL_set1_host(ssl, "example.com");
        }
        
        // 5. Use SSL_CTX_get_record_padding_callback_arg
        void *padding_arg = SSL_CTX_get_record_padding_callback_arg(ssl_ctx);
        (void)padding_arg; // Mark as used
    }
    
    // 6. Create X509_STORE and X509_LOOKUP for X509_LOOKUP_get_store
    X509_STORE *store = X509_STORE_new();
    if (store != nullptr) {
        lookup = X509_STORE_add_lookup(store, X509_LOOKUP_file());
        if (lookup != nullptr) {
            // 7. Use X509_LOOKUP_get_store
            X509_STORE *retrieved_store = X509_LOOKUP_get_store(lookup);
            (void)retrieved_store; // Mark as used
        }
    }
    
    // 8. Create and use ossl_check_const_void_sk_type
    void_stack = (struct stack_st_void *)OPENSSL_sk_new(nullptr);
    if (void_stack != nullptr) {
        // Add some dummy data
        int dummy = 42;
        OPENSSL_sk_push((OPENSSL_STACK *)void_stack, &dummy);
        
        // 9. Use ossl_check_const_void_sk_type
        const OPENSSL_STACK *checked_void_stack = 
            ossl_check_const_void_sk_type(void_stack);
        (void)checked_void_stack; // Mark as used
    }
    
    // 10. Create and use ossl_check_const_UI_STRING_sk_type
    ui_string_stack = (struct stack_st_UI_STRING *)OPENSSL_sk_new(nullptr);
    if (ui_string_stack != nullptr) {
        // 11. Use ossl_check_const_UI_STRING_sk_type
        const OPENSSL_STACK *checked_ui_stack = 
            ossl_check_const_UI_STRING_sk_type(ui_string_stack);
        (void)checked_ui_stack; // Mark as used
    }
    
    // 12. Use OSSL_FUNC_rand_set_callbacks (simulated usage)
    // Note: This requires a valid OSSL_DISPATCH table, which we don't have.
    // We'll create a minimal dispatch table for demonstration.
    OSSL_DISPATCH dummy_dispatch[] = {
        {0, nullptr}
    };
    dispatch_table = dummy_dispatch;
    
    // Get the function pointer (will return nullptr with our dummy table)
    OSSL_FUNC_rand_set_callbacks_fn *set_callbacks = 
        OSSL_FUNC_rand_set_callbacks(dispatch_table);
    (void)set_callbacks; // Mark as used
    
    // Cleanup
    
    // Free SRP resources
    OPENSSL_free(srp_result);
    OPENSSL_free(salt);
    OPENSSL_free(verifier);
    
    // Free ASN.1 data if allocated
    if (asn1_data != nullptr) {
        ASN1_item_free((ASN1_VALUE *)asn1_data, it);
    }
    
    // Free stacks
    if (void_stack != nullptr) {
        OPENSSL_sk_free((OPENSSL_STACK *)void_stack);
    }
    if (ui_string_stack != nullptr) {
        OPENSSL_sk_free((OPENSSL_STACK *)ui_string_stack);
    }
    
    // Free SSL/SSL_CTX
    if (ssl != nullptr) {
        SSL_free(ssl);
    }
    if (ssl_ctx != nullptr) {
        SSL_CTX_free(ssl_ctx);
    }
    
    // Free X509 store
    if (store != nullptr) {
        X509_STORE_free(store);
    }
    
    // Free library context
    if (libctx != nullptr) {
        OSSL_LIB_CTX_free(libctx);
    }
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}