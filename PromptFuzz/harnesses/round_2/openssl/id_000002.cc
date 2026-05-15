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
//<ID> 55
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"check_default_context":[[470,35,470,57,0,0,4,1]],"init_thread_stop":[[355,9,355,22,0,0,4,0]]},"library_calls":["OSSL_LIB_CTX_new","OSSL_LIB_CTX_free","OSSL_LIB_CTX_free","SRP_create_verifier_ex","CRYPTO_free","CRYPTO_free","OSSL_LIB_CTX_free","ASN1_item_d2i_fp","X509_it","SSL_CTX_new","TLS_method","SSL_CTX_get_record_padding_callback_arg","SSL_new","SSL_set1_host","SSL_free","X509_STORE_new","X509_STORE_add_lookup","X509_LOOKUP_file","X509_LOOKUP_get_store","OPENSSL_sk_new_null","ossl_check_const_void_sk_type","OPENSSL_sk_push","OPENSSL_sk_pop","OPENSSL_sk_free","OPENSSL_sk_new_null","ossl_check_const_UI_STRING_sk_type","OPENSSL_sk_free","OSSL_FUNC_rand_set_callbacks","X509_free","SSL_CTX_free","X509_STORE_free","OSSL_LIB_CTX_free"],"critical_calls":["OSSL_LIB_CTX_new","SRP_create_verifier_ex","CRYPTO_free","CRYPTO_free","ASN1_item_d2i_fp","X509_it","SSL_CTX_new","TLS_method","SSL_CTX_get_record_padding_callback_arg","SSL_new","SSL_set1_host","SSL_free","X509_STORE_new","X509_STORE_add_lookup","X509_LOOKUP_file","X509_LOOKUP_get_store","OPENSSL_sk_new_null","ossl_check_const_void_sk_type","OPENSSL_sk_push","OPENSSL_sk_pop","OPENSSL_sk_free","OPENSSL_sk_new_null","ossl_check_const_UI_STRING_sk_type","OPENSSL_sk_free","OSSL_FUNC_rand_set_callbacks","X509_free","SSL_CTX_free","X509_STORE_free","OSSL_LIB_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing
    
    // 1. Create OSSL_LIB_CTX for SRP operation
    OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
    if (!libctx) {
	return 0;
	}
	
    
    // 2. Parse input data for SRP parameters
    // Use first part of data for user/pass/N/g strings
    size_t offset = 0;
    
    // Extract user (first null-terminated string from data)
    const char *user = reinterpret_cast<const char*>(data);
    size_t user_len = strnlen(user, size - offset);
    if (offset + user_len >= size) {
        OSSL_LIB_CTX_free(libctx);
        return 0;
    }
    offset += user_len + 1;
    
    // Extract password
    const char *pass = reinterpret_cast<const char*>(data + offset);
    size_t pass_len = strnlen(pass, size - offset);
    if (offset + pass_len >= size) {
        OSSL_LIB_CTX_free(libctx);
        return 0;
    }
    offset += pass_len + 1;
    
    // Extract N (SRP modulus) - using predefined safe prime
    const char *N = "EEAF0AB9ADB38DD69C33F80AFA8FC5E86072618775FF3C0B9EA2314C"
                    "9C256576D674DF7496EA81D3383B4813D692C6E0E0D5D8E250B98BE4"
                    "8E495C1D6089DAD15DC7D7B46154D6B6CE8EF4AD69B15D4982559B2"
                    "97BCF1885C529F566660E57EC68EDBC3C05726CC02FD4CBF4976EAA"
                    "9AFD5138FE8376435B9FC61D2FC0EB06E3";
    
    // Extract g (generator)
    const char *g = "2";
    
    // 3. Call SRP_create_verifier_ex with parsed parameters
    char *salt = nullptr;
    char *verifier = nullptr;
    char *result = SRP_create_verifier_ex(user, pass, &salt, &verifier, 
                                         N, g, libctx, nullptr);
    
    // Clean up SRP results
    if (salt) OPENSSL_free(salt);
    if (verifier) OPENSSL_free(verifier);
    
    // 4. Use remaining data as ASN.1 input file
    FILE *in_file = fmemopen((void *)(data + offset), size - offset, "rb");
    if (!in_file) {
        OSSL_LIB_CTX_free(libctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Try to parse ASN.1 structure (using X509 as example)
    X509 *x509 = nullptr;
    x509 = (X509 *)ASN1_item_d2i_fp(X509_it(), in_file, nullptr);
    assert_file_closed(&in_file);;
    
    // 5. Create SSL_CTX and get padding callback argument
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    if (ssl_ctx) {
        void *pad_arg = SSL_CTX_get_record_padding_callback_arg(ssl_ctx);
        (void)pad_arg; // Use variable to avoid unused warning
        
        // 6. Set hostname for SSL context
        const char *hostname = "test.example.com";
        SSL *ssl = SSL_new(ssl_ctx);
        if (ssl) {
            SSL_set1_host(ssl, hostname);
            SSL_free(ssl);
        }
    }
    
    // 7. Create X509 store and lookup to get store
    X509_STORE *store = X509_STORE_new();
    X509_LOOKUP *lookup = nullptr;
    if (store) {
        lookup = X509_STORE_add_lookup(store, X509_LOOKUP_file());
        if (lookup) {
            X509_STORE *lookup_store = X509_LOOKUP_get_store(lookup);
            (void)lookup_store; // Use variable
        }
    }
    
    // 8. Create stacks and use cast functions
    OPENSSL_STACK *void_stack = OPENSSL_sk_new_null();
    if (void_stack) {
        const OPENSSL_STACK *cvoid_stack = 
            ossl_check_const_void_sk_type((struct stack_st_void *)void_stack);
        (void)cvoid_stack; // Use variable
        
        // Add some dummy data
        int dummy = 42;
        OPENSSL_sk_push(void_stack, &dummy);
        OPENSSL_sk_pop(void_stack);
        OPENSSL_sk_free(void_stack);
    }
    
    // Create UI_STRING stack
    OPENSSL_STACK *ui_stack = OPENSSL_sk_new_null();
    if (ui_stack) {
        const OPENSSL_STACK *cui_stack = 
            ossl_check_const_UI_STRING_sk_type((struct stack_st_UI_STRING *)ui_stack);
        (void)cui_stack; // Use variable
        OPENSSL_sk_free(ui_stack);
    }
    
    // 9. Get random set callbacks function (even if we don't use it)
    // Create dummy dispatch table
    OSSL_DISPATCH dummy_dispatch[] = {
        {0, nullptr}
    };
    OSSL_FUNC_rand_set_callbacks_fn *rand_cb = 
        OSSL_FUNC_rand_set_callbacks(dummy_dispatch);
    (void)rand_cb; // Use variable
    
    // 10. Cleanup
    if (x509) X509_free(x509);
    if (ssl_ctx) SSL_CTX_free(ssl_ctx);
    if (store) X509_STORE_free(store);
    
    // Finally free the library context
    OSSL_LIB_CTX_free(libctx);
    
    assert_file_closed(&in_file);
	return 0;
}