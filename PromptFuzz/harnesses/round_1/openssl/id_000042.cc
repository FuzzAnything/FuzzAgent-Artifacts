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
//<ID> 500
//<Prompt> []
/*<Combination>: [
*/
//<score> 33, nr_unique_branch: 3
//<Quality> {"density":11,"unique_branches":{"SSL_new":[[685,9,685,20,0,0,4,0]],"OSSL_PROVIDER_get0_default_search_path":[[940,9,940,53,0,0,4,0],[941,12,941,61,0,0,4,0]]},"library_calls":["ERR_load_CRYPTO_strings","BIO_new_fp","BIO_free","BIO_new_fp","PKCS12_SAFEBAG_new","i2d_PKCS12_SAFEBAG","CRYPTO_free","PKCS12_SAFEBAG_free","X509_REVOKED_new","X509_REVOKED_get0_extensions","X509_REVOKED_free","X509_new","X509_keyid_set1","SSL_new","SSL_set_tmp_dh_callback","SSL_free","X509_free","OSSL_PROVIDER_get0_default_search_path","a2i_GENERAL_NAME","OPENSSL_sk_new_null","OPENSSL_sk_set_thunks","OPENSSL_sk_push","ossl_check_GENERAL_NAME_type","ossl_check_GENERAL_NAME_sk_type","OSSL_GENERAL_NAMES_print","OPENSSL_version_minor","OPENSSL_sk_new_null","ossl_check_ESS_CERT_ID_sk_type","OPENSSL_sk_free","sk_X509_LOOKUP_cmpfunc_thunk","OPENSSL_sk_pop_free","ossl_check_GENERAL_NAME_freefunc_type","GENERAL_NAME_free","BIO_free","BIO_free"],"critical_calls":["ERR_load_CRYPTO_strings","BIO_new_fp","BIO_new_fp","PKCS12_SAFEBAG_new","i2d_PKCS12_SAFEBAG","CRYPTO_free","PKCS12_SAFEBAG_free","X509_REVOKED_new","X509_REVOKED_get0_extensions","X509_REVOKED_free","X509_new","X509_keyid_set1","SSL_new","SSL_set_tmp_dh_callback","SSL_free","X509_free","OSSL_PROVIDER_get0_default_search_path","a2i_GENERAL_NAME","OPENSSL_sk_new_null","OPENSSL_sk_set_thunks","OPENSSL_sk_push","ossl_check_GENERAL_NAME_type","ossl_check_GENERAL_NAME_sk_type","OSSL_GENERAL_NAMES_print","OPENSSL_version_minor","OPENSSL_sk_new_null","ossl_check_ESS_CERT_ID_sk_type","OPENSSL_sk_free","sk_X509_LOOKUP_cmpfunc_thunk","OPENSSL_sk_pop_free","ossl_check_GENERAL_NAME_freefunc_type","BIO_free","BIO_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL error strings
    ERR_load_CRYPTO_strings();
    
    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create BIO from FILE stream
    BIO *bio = BIO_new_fp(in_file, 0); // 0 = don't close FILE on BIO free
    if (!bio) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        BIO_free(bio);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    BIO *out_bio = BIO_new_fp(out_file, 1); // 1 = close FILE on BIO free
    
    // Create and manipulate PKCS12_SAFEBAG
    PKCS12_SAFEBAG *bag = PKCS12_SAFEBAG_new();
    if (bag) {
        unsigned char *der = nullptr;
        int der_len = i2d_PKCS12_SAFEBAG(bag, &der);
        if (der_len > 0 && der) {
            // Free DER data using CRYPTO_free
            CRYPTO_free(der, __FILE__, __LINE__);
        }
        PKCS12_SAFEBAG_free(bag);
    }
    
    // Create and manipulate X509_REVOKED
    X509_REVOKED *revoked = X509_REVOKED_new();
    if (revoked) {
        // Get extensions (will be empty for new object)
        const struct stack_st_X509_EXTENSION *exts = X509_REVOKED_get0_extensions(revoked);
        X509_REVOKED_free(revoked);
    }
    
    // Create X509 certificate
    X509 *x509 = X509_new();
    if (x509) {
        // Set key ID using input data (limit to 128 bytes for safety)
        int id_len = size > 128 ? 128 : (int)size;
        if (id_len > 0) {
            X509_keyid_set1(x509, data, id_len);
        }
        
        // Set dummy DH callback for SSL
        SSL *ssl = SSL_new(nullptr);
        if (ssl) {
            SSL_set_tmp_dh_callback(ssl, nullptr);
            SSL_free(ssl);
        }
        
        X509_free(x509);
    }
    
    // Get default provider search path
    const char *search_path = OSSL_PROVIDER_get0_default_search_path(nullptr);
    
    // Create and manipulate GENERAL_NAME using a2i_GENERAL_NAME
    GENERAL_NAME *gen_name = nullptr;
    if (size > 0) {
        // Create a null-terminated string from input for testing
        char *value = (char *)malloc(size + 1);
        if (value) {
            memcpy(value, data, size);
            value[size] = '\0';
            
            // Create minimal context for a2i_GENERAL_NAME
            X509V3_CTX ctx;
            memset(&ctx, 0, sizeof(ctx));
            
            // Try to create GENERAL_NAME (using DNS type = 2)
            gen_name = a2i_GENERAL_NAME(nullptr, nullptr, &ctx, 2, value, 0);
            free(value);
        }
    }
    
    // Create stack and manipulate with various stack functions
    OPENSSL_STACK *stack = OPENSSL_sk_new_null();
    if (stack) {
        // Set thunks for stack
        OPENSSL_sk_set_thunks(stack, nullptr);
        
        // Push GENERAL_NAME if created
        if (gen_name) {
            OPENSSL_sk_push(stack, ossl_check_GENERAL_NAME_type(gen_name));
            
            // Print GENERAL_NAMES
            GENERAL_NAMES *gens = (GENERAL_NAMES *)ossl_check_GENERAL_NAME_sk_type(
                (struct stack_st_GENERAL_NAME *)stack);
            OSSL_GENERAL_NAMES_print(out_bio, gens, 0);
        }
        
        // Get OpenSSL minor version
        unsigned int minor_ver = OPENSSL_version_minor();
        
        // Create ESS_CERT_ID stack for type checking
        OPENSSL_STACK *ess_stack = OPENSSL_sk_new_null();
        if (ess_stack) {
            ossl_check_ESS_CERT_ID_sk_type((struct stack_st_ESS_CERT_ID *)ess_stack);
            OPENSSL_sk_free(ess_stack);
        }
        
        // Test comparison thunk function
        int (*dummy_cmp)(const void *, const void *) = [](const void *a, const void *b) { return 0; };
        sk_X509_LOOKUP_cmpfunc_thunk(dummy_cmp, nullptr, nullptr);
        
        // Use pop_free with free function thunk
        OPENSSL_sk_pop_free(stack, 
            ossl_check_GENERAL_NAME_freefunc_type(
                (sk_GENERAL_NAME_freefunc)GENERAL_NAME_free));
    } else if (gen_name) {
        GENERAL_NAME_free(gen_name);
    }
    
    // Cleanup
    if (out_bio) {
        BIO_free(out_bio);  // This will also close out_file
    } else if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    BIO_free(bio);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}