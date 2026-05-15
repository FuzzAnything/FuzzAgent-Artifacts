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
//<ID> 175
//<Prompt> []
/*<Combination>: [
*/
//<score> 16.5, nr_unique_branch: 4
//<Quality> {"density":11,"unique_branches":{"GENERAL_NAME_print":[[265,5,265,19,0,0,4,0],[270,5,270,17,0,0,4,1]],"a2i_GENERAL_NAME":[[532,5,532,19,0,0,4,0],[533,5,533,17,0,0,4,1]]},"library_calls":["ERR_load_CRYPTO_strings","BIO_new_fp","PKCS12_SAFEBAG_new","i2d_PKCS12_SAFEBAG","CRYPTO_free","X509_REVOKED_new","X509_REVOKED_get0_extensions","X509_new","X509_keyid_set1","a2i_GENERAL_NAME","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OPENSSL_sk_push","ossl_check_GENERAL_NAME_sk_type","ossl_check_GENERAL_NAME_type","OSSL_GENERAL_NAMES_print","OPENSSL_sk_pop_free","ossl_check_GENERAL_NAME_sk_type","ossl_check_GENERAL_NAME_freefunc_type","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_ESS_CERT_ID_sk_type","OPENSSL_sk_free","ossl_check_ESS_CERT_ID_sk_type","sk_X509_LOOKUP_cmpfunc_thunk","GENERAL_NAME_free","X509_free","X509_REVOKED_free","PKCS12_SAFEBAG_free","BIO_free"],"critical_calls":["ERR_load_CRYPTO_strings","BIO_new_fp","PKCS12_SAFEBAG_new","i2d_PKCS12_SAFEBAG","CRYPTO_free","X509_REVOKED_new","X509_REVOKED_get0_extensions","X509_new","X509_keyid_set1","a2i_GENERAL_NAME","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OPENSSL_sk_push","ossl_check_GENERAL_NAME_type","ossl_check_GENERAL_NAME_sk_type","OSSL_GENERAL_NAMES_print","OPENSSL_sk_pop_free","ossl_check_GENERAL_NAME_freefunc_type","ossl_check_GENERAL_NAME_sk_type","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_ESS_CERT_ID_sk_type","OPENSSL_sk_free","ossl_check_ESS_CERT_ID_sk_type","sk_X509_LOOKUP_cmpfunc_thunk","GENERAL_NAME_free","X509_free","X509_REVOKED_free","PKCS12_SAFEBAG_free","BIO_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Load error strings first
    ERR_load_CRYPTO_strings();
    
    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create FILE* for writing output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create BIO from output file for printing functions
    BIO *out_bio = BIO_new_fp(out_file, BIO_NOCLOSE);
    
    // 1. Try to read PKCS12_SAFEBAG from input data
    PKCS12_SAFEBAG *bag = NULL;
    // We need to create a dummy bag since we can't decode from raw bytes directly
    // Use the input data to create a simple bag structure
    bag = PKCS12_SAFEBAG_new();
    if (bag && size > 0) {
        // Encode the bag to DER format using input data
        unsigned char *der = NULL;
        int der_len = i2d_PKCS12_SAFEBAG(bag, &der);
        if (der_len > 0 && der) {
            // Free the allocated DER data
            CRYPTO_free(der, __FILE__, __LINE__);
        }
    }
    
    // 2. Create and work with X509_REVOKED object
    X509_REVOKED *revoked = X509_REVOKED_new();
    if (revoked) {
        // Get extensions from revoked certificate
        const struct stack_st_X509_EXTENSION *exts = X509_REVOKED_get0_extensions(revoked);
        (void)exts; // Use the result
    }
    
    // 3. Create and work with X509 certificate
    X509 *x509 = X509_new();
    if (x509) {
        // Set key ID using input data (use first min(size, 20) bytes as keyid)
        int keyid_len = size > 20 ? 20 : size;
        if (keyid_len > 0) {
            X509_keyid_set1(x509, data, keyid_len);
        }
    }
    
    // 4. Create GENERAL_NAME from input data
    GENERAL_NAME *gen = NULL;
    if (size > 0) {
        // Create a null-terminated string from input data for a2i_GENERAL_NAME
        char *str_value = (char *)malloc(size + 1);
        if (str_value) {
            memcpy(str_value, data, size);
            str_value[size] = '\0';
            
            // Create dummy context and method for a2i_GENERAL_NAME
            X509V3_CTX ctx;
            memset(&ctx, 0, sizeof(ctx));
            
            // Try to create GENERAL_NAME from input string
            // Using GEN_EMAIL as type (type 1)
            gen = a2i_GENERAL_NAME(NULL, NULL, &ctx, 1, str_value, 0);
            
            free(str_value);
            
            // If we successfully created a GENERAL_NAME, add it to a stack
            if (gen) {
                // Create GENERAL_NAMES stack
                GENERAL_NAMES *gens = sk_GENERAL_NAME_new_null();
                if (gens) {
                    sk_GENERAL_NAME_push(gens, gen);
                    
                    // Print GENERAL_NAMES to BIO
                    if (out_bio) {
                        OSSL_GENERAL_NAMES_print(out_bio, gens, 4);
                    }
                    
                    // Don't free gen here as it's owned by the stack
                    sk_GENERAL_NAME_pop_free(gens, GENERAL_NAME_free);
                    gen = NULL; // Already freed by pop_free
                }
            }
        }
    }
    
    // 5. Work with ESS_CERT_ID stack
    struct stack_st_ESS_CERT_ID *ess_stack = sk_ESS_CERT_ID_new_null();
    if (ess_stack) {
        // Use type checking function
        OPENSSL_STACK *ossl_stack = ossl_check_ESS_CERT_ID_sk_type(ess_stack);
        (void)ossl_stack; // Use the result
        
        // Free the stack (it's empty, so no elements to free)
        sk_ESS_CERT_ID_free(ess_stack);
    }
    
    // 6. Work with X509_LOOKUP stack comparison
    // Create a dummy comparison function
    auto dummy_cmp = [](const void *a, const void *b) -> int {
        return 0;
    };
    
    // Use the comparison thunk function
    int cmp_result = sk_X509_LOOKUP_cmpfunc_thunk(dummy_cmp, NULL, NULL);
    (void)cmp_result; // Use the result
    
    // Clean up all allocated resources
    if (gen) {
        GENERAL_NAME_free(gen);
    }
    
    if (x509) {
        X509_free(x509);
    }
    
    if (revoked) {
        X509_REVOKED_free(revoked);
    }
    
    if (bag) {
        PKCS12_SAFEBAG_free(bag);
    }
    
    if (out_bio) {
        BIO_free(out_bio);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}