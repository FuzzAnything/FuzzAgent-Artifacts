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
//<ID> 475
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 2
//<Quality> {"density":13,"unique_branches":{"EVP_PKEY_get_field_type":[[2449,13,2449,23,0,0,4,0]],"evp_pkey_get0_EC_KEY_int":[[78,9,78,50,0,0,4,0]]},"library_calls":["OPENSSL_init_crypto","ERR_clear_error","d2i_OSSL_ATTRIBUTE_VALUE_MAPPING","ASN1_BIT_STRING_new","ASN1_BIT_STRING_set","BN_get_rfc3526_prime_1536","DSA_new","BN_new","BN_new","BN_set_word","BN_set_word","DSA_set0_pqg","BN_dup","EVP_PKEY_new","EVP_PKEY_set1_DSA","BN_free","BN_free","DSA_free","EVP_PKEY_get_field_type","ossl_check_OSSL_TARGETS_compfunc_type","CRYPTO_free","ASN1_BIT_STRING_free","BN_free","EVP_PKEY_free"],"critical_calls":["OPENSSL_init_crypto","ERR_clear_error","d2i_OSSL_ATTRIBUTE_VALUE_MAPPING","ASN1_BIT_STRING_new","ASN1_BIT_STRING_set","BN_get_rfc3526_prime_1536","DSA_new","BN_new","BN_new","BN_set_word","BN_set_word","DSA_set0_pqg","BN_dup","EVP_PKEY_new","EVP_PKEY_set1_DSA","DSA_free","EVP_PKEY_get_field_type","ossl_check_OSSL_TARGETS_compfunc_type","CRYPTO_free","ASN1_BIT_STRING_free","BN_free","EVP_PKEY_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    ERR_clear_error();

    // 1. Parse OSSL_ATTRIBUTE_VALUE_MAPPING from input data
    const unsigned char *p = data;
    long remaining = static_cast<long>(size);
    OSSL_ATTRIBUTE_VALUE_MAPPING *mapping = NULL;
    mapping = d2i_OSSL_ATTRIBUTE_VALUE_MAPPING(&mapping, &p, remaining);
    if (mapping) {
        // Update remaining bytes after parsing
        remaining -= (p - data);
    }

    // 2. Create and set ASN1_BIT_STRING with remaining data
    ASN1_BIT_STRING *bit_string = ASN1_BIT_STRING_new();
    if (bit_string && remaining > 0) {
        // Use remaining data for ASN1_BIT_STRING
        // Ensure we don't exceed available data
        int bytes_to_use = remaining > INT_MAX ? INT_MAX : static_cast<int>(remaining);
        if (bytes_to_use > 0) {
            ASN1_BIT_STRING_set(bit_string, const_cast<unsigned char*>(p), bytes_to_use);
        }
    }

    // 3. Generate RFC3526 1536-bit prime
    BIGNUM *prime = BN_get_rfc3526_prime_1536(NULL);
    
    // 4. Create EVP_PKEY from the prime (for DSA parameters)
    EVP_PKEY *pkey = NULL;
    if (prime) {
        // Create DSA parameters with the prime
        DSA *dsa = DSA_new();
        if (dsa) {
            // Set the prime in DSA parameters
            // Note: This is simplified - real DSA needs more parameters
            BIGNUM *q = BN_new();
            BIGNUM *g = BN_new();
            if (q && g) {
                // Generate some reasonable values for testing
                BN_set_word(q, 0xFFFFFFFF); // Dummy value
                BN_set_word(g, 2);          // Common generator
                
                DSA_set0_pqg(dsa, BN_dup(prime), q, g);
                
                // Create EVP_PKEY from DSA
                pkey = EVP_PKEY_new();
                if (pkey) {
                    EVP_PKEY_set1_DSA(pkey, dsa);
                }
            } else {
                if (q) BN_free(q);
                if (g) BN_free(g);
            }
            DSA_free(dsa);
        }
    }

    // 5. Get field type of EVP_PKEY if available
    int field_type = -1;
    if (pkey) {
        field_type = EVP_PKEY_get_field_type(pkey);
    }

    // 6. Use ossl_check_OSSL_TARGETS_compfunc_type
    // Create a dummy comparison function
    int (*dummy_cmp)(const OSSL_TARGETS *const *, const OSSL_TARGETS *const *) = 
        [](const OSSL_TARGETS *const *a, const OSSL_TARGETS *const *b) {
            return 0;
        };
    
    // Use the type checking function
    OPENSSL_sk_compfunc checked_cmp = ossl_check_OSSL_TARGETS_compfunc_type(dummy_cmp);
    (void)checked_cmp; // Prevent unused variable warning

    // 7. Also demonstrate file operations as specified
    FILE *in_file = NULL;
    if (size > 0) {
        in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            // Read a byte to demonstrate file usage
            int fd = fuzz_fileno(in_file);
            (void)fd; // Prevent unused variable warning
            
            unsigned char buffer[1];
            if (fread(buffer, 1, 1, in_file) == 1) {
                // Successfully read one byte
            }
            
            assert_file_closed(&in_file);;
        }
    }
    
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some dummy output
        const char *dummy = "fuzz test output\n";
        fwrite(dummy, 1, strlen(dummy), out_file);
        
        int fd = fuzz_fileno(out_file);
        (void)fd; // Prevent unused variable warning
        
        assert_file_closed(&out_file);;
    }

    // Cleanup
    if (mapping) {
        // Note: No direct free function in provided APIs, use generic cleanup
        OPENSSL_free(mapping);
    }
    
    if (bit_string) {
        ASN1_BIT_STRING_free(bit_string);
    }
    
    if (prime) {
        BN_free(prime);
    }
    
    if (pkey) {
        EVP_PKEY_free(pkey);
    }
    
    // Remove output file if created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	return 0;
}