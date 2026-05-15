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
//<ID> 176
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 5
//<Quality> {"density":12,"unique_branches":{"X509V3_EXT_get_nid":[[59,9,59,16,0,0,4,1],[63,9,63,12,0,0,4,0]],"asn1_item_embed_new":[[89,13,89,26,0,0,4,0],[90,17,90,71,0,0,4,1]],"X509V3_set_ctx":[[513,9,513,20,0,0,4,1]]},"library_calls":["ERR_load_CRYPTO_strings","BIO_new_fp","X509V3_set_ctx","X509_new","X509_keyid_set1","CRYPTO_malloc","X509V3_EXT_get_nid","a2i_GENERAL_NAME","CRYPTO_free","PKCS12_SAFEBAG_new","i2d_PKCS12_SAFEBAG","CRYPTO_free","X509_REVOKED_new","X509_REVOKED_get0_extensions","GENERAL_NAMES_new","OPENSSL_sk_push","ossl_check_GENERAL_NAME_sk_type","ossl_check_GENERAL_NAME_type","OSSL_GENERAL_NAMES_print","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_ESS_CERT_ID_sk_type","sk_X509_LOOKUP_cmpfunc_thunk","GENERAL_NAMES_free","GENERAL_NAME_free","PKCS12_SAFEBAG_free","X509_REVOKED_free","X509_free","OPENSSL_sk_free","ossl_check_ESS_CERT_ID_sk_type","BIO_free"],"critical_calls":["ERR_load_CRYPTO_strings","BIO_new_fp","X509V3_set_ctx","X509_new","X509_keyid_set1","CRYPTO_malloc","X509V3_EXT_get_nid","a2i_GENERAL_NAME","CRYPTO_free","PKCS12_SAFEBAG_new","i2d_PKCS12_SAFEBAG","CRYPTO_free","X509_REVOKED_new","X509_REVOKED_get0_extensions","GENERAL_NAMES_new","OPENSSL_sk_push","ossl_check_GENERAL_NAME_type","ossl_check_GENERAL_NAME_sk_type","OSSL_GENERAL_NAMES_print","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_ESS_CERT_ID_sk_type","sk_X509_LOOKUP_cmpfunc_thunk","GENERAL_NAMES_free","PKCS12_SAFEBAG_free","X509_REVOKED_free","X509_free","OPENSSL_sk_free","ossl_check_ESS_CERT_ID_sk_type","BIO_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Load OpenSSL error strings
    ERR_load_CRYPTO_strings();
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a BIO for output
    BIO *out_bio = BIO_new_fp(out_file, BIO_NOCLOSE);
    if (!out_bio) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create and initialize variables
    X509 *x509 = NULL;
    GENERAL_NAME *gen_name = NULL;
    GENERAL_NAMES *gen_names = NULL;
    PKCS12_SAFEBAG *bag = NULL;
    X509_REVOKED *revoked = NULL;
    X509V3_CTX ctx;
    struct stack_st_ESS_CERT_ID *ess_stack = NULL;
    
    // Initialize X509V3 context
    X509V3_set_ctx(&ctx, NULL, NULL, NULL, NULL, 0);
    
    // 1. Create a dummy X509 certificate for testing
    x509 = X509_new();
    if (x509) {
        // Set keyid using fuzzer data (first 20 bytes or less)
        int keyid_len = size > 20 ? 20 : size;
        if (keyid_len > 0) {
            X509_keyid_set1(x509, data, keyid_len);
        }
        
        // 2. Create a GENERAL_NAME from string using fuzzer data
        // Use a portion of data as string (up to 100 bytes)
        char *name_str = NULL;
        if (size > 0) {
            size_t str_len = size > 100 ? 100 : size;
            name_str = (char *)CRYPTO_malloc(str_len + 1, __FILE__, __LINE__);
            if (name_str) {
                memcpy(name_str, data, str_len);
                name_str[str_len] = '\0';
                
                // Create GENERAL_NAME from the string
                const X509V3_EXT_METHOD *method = X509V3_EXT_get_nid(NID_subject_alt_name);
                if (method) {
                    gen_name = a2i_GENERAL_NAME(NULL, method, &ctx, GEN_DNS, name_str, 0);
                }
                
                CRYPTO_free(name_str, __FILE__, __LINE__);
            }
        }
        
        // 3. Create a PKCS12_SAFEBAG (dummy certificate bag)
        bag = PKCS12_SAFEBAG_new();
        if (bag) {
            // Try to encode the bag using i2d
            unsigned char *out_buf = NULL;
            int out_len = i2d_PKCS12_SAFEBAG(bag, &out_buf);
            if (out_buf) {
                // Write to output file
                fwrite(out_buf, 1, out_len, out_file);
                // Free the buffer using CRYPTO_free
                CRYPTO_free(out_buf, __FILE__, __LINE__);
            }
        }
        
        // 4. Create X509_REVOKED and get extensions
        revoked = X509_REVOKED_new();
        if (revoked) {
            const struct stack_st_X509_EXTENSION *exts = X509_REVOKED_get0_extensions(revoked);
            // exts could be NULL for empty revoked entry
        }
        
        // 5. Create GENERAL_NAMES stack if we have a GENERAL_NAME
        if (gen_name) {
            gen_names = GENERAL_NAMES_new();
            if (gen_names) {
                sk_GENERAL_NAME_push(gen_names, gen_name);
                
                // Print GENERAL_NAMES to BIO
                OSSL_GENERAL_NAMES_print(out_bio, gen_names, 4);
            }
        }
    }
    
    // 6. Use stack functions
    // Create an ESS_CERT_ID stack
    ess_stack = sk_ESS_CERT_ID_new_null();
    if (ess_stack) {
        // Get OPENSSL_STACK type
        OPENSSL_STACK *ossl_stack = ossl_check_ESS_CERT_ID_sk_type(ess_stack);
        // ossl_stack can be used for stack operations
    }
    
    // 7. Demonstrate sk_X509_LOOKUP_cmpfunc_thunk usage
    // Create a dummy comparison function
    int (*cmp_func)(const void *, const void *) = [](const void *a, const void *b) -> int {
        return 0;
    };
    
    // Call the thunk function
    int cmp_result = sk_X509_LOOKUP_cmpfunc_thunk(cmp_func, NULL, NULL);
    
    // Clean up all allocated resources
    if (gen_names) {
        // Don't free individual items as they're owned by the stack
        GENERAL_NAMES_free(gen_names);
    } else if (gen_name) {
        // If gen_names wasn't created, free the individual GENERAL_NAME
        GENERAL_NAME_free(gen_name);
    }
    
    if (bag) {
        PKCS12_SAFEBAG_free(bag);
    }
    
    if (revoked) {
        X509_REVOKED_free(revoked);
    }
    
    if (x509) {
        X509_free(x509);
    }
    
    if (ess_stack) {
        sk_ESS_CERT_ID_free(ess_stack);
    }
    
    // Clean up BIO and files
    BIO_free(out_bio);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}