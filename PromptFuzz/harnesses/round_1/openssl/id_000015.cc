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
//<ID> 214
//<Prompt> []
/*<Combination>: [
*/
//<score> 64, nr_unique_branch: 10
//<Quality> {"density":16,"unique_branches":{"ossl_pkcs7_ctx_get0_propq":[[524,12,524,23,0,0,4,0]],"d2i_PKCS7":[[71,9,71,18,0,0,4,1],[78,9,78,20,0,0,4,0],[78,9,78,20,0,0,4,1]],"ossl_pkcs7_get0_ctx":[[486,12,486,22,0,0,4,0]],"ossl_pkcs7_resolve_libctx":[[460,9,460,20,0,0,4,1],[460,24,460,41,0,0,4,0]],"ossl_pkcs7_ctx_get0_libctx":[[520,12,520,23,0,0,4,0]],"CRYPTO_secure_malloc_array":[[23,26,23,52,1,0,4,1]],"EVP_RAND_CTX_gettable_params":[[475,9,475,47,0,0,4,1]]},"library_calls":["OPENSSL_init_crypto","CRYPTO_secure_malloc_array","CRYPTO_secure_clear_free","d2i_ASN1_UTCTIME","ASN1_UTCTIME_free","d2i_TS_REQ","TS_REQ_free","d2i_TS_ACCURACY","TS_ACCURACY_free","d2i_PKCS7","X509_STORE_new","X509_STORE_set_flags","BIO_new_fd","BIO_new_fd","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","X509_ALGOR_new","ossl_check_X509_ALGOR_type","X509_ALGOR_free","PKCS7_verify","OPENSSL_sk_free","ossl_check_X509_sk_type","BIO_free","BIO_free","X509_STORE_free","PKCS7_free","EVP_RAND_fetch","EVP_RAND_CTX_new","EVP_RAND_CTX_gettable_params","EVP_RAND_CTX_free","EVP_RAND_free","d2i_TS_REQ","TS_REQ_free"],"critical_calls":["OPENSSL_init_crypto","CRYPTO_secure_malloc_array","CRYPTO_secure_clear_free","d2i_ASN1_UTCTIME","ASN1_UTCTIME_free","d2i_TS_REQ","TS_REQ_free","d2i_TS_ACCURACY","TS_ACCURACY_free","d2i_PKCS7","X509_STORE_new","X509_STORE_set_flags","BIO_new_fd","BIO_new_fd","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","X509_ALGOR_new","ossl_check_X509_ALGOR_type","X509_ALGOR_free","PKCS7_verify","OPENSSL_sk_free","ossl_check_X509_sk_type","BIO_free","BIO_free","X509_STORE_free","PKCS7_free","EVP_RAND_fetch","EVP_RAND_CTX_new","EVP_RAND_CTX_gettable_params","EVP_RAND_CTX_free","EVP_RAND_free","d2i_TS_REQ","TS_REQ_free"],"visited":1}
/**/



extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL if needed
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | OPENSSL_INIT_ADD_ALL_CIPHERS | 
                       OPENSSL_INIT_ADD_ALL_DIGESTS, nullptr);
    
    if (size < 10) {
        // Too small to be meaningful
        return 0;
    }
    
    // Create a secure memory array using CRYPTO_secure_malloc_array
    size_t secure_array_size = size > 100 ? 100 : size;
    char *secure_array = (char *)CRYPTO_secure_malloc_array(secure_array_size, 1, __FILE__, __LINE__);
    if (secure_array) {
        memcpy(secure_array, data, secure_array_size);
        CRYPTO_secure_clear_free(secure_array, secure_array_size, __FILE__, __LINE__);
    }
    
    // Use FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output FILE*
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Reset file position
    rewind(in_file);
    
    // Parse ASN1_UTCTIME from the beginning of the data
    const unsigned char *p = data;
    ASN1_UTCTIME *utctime = d2i_ASN1_UTCTIME(nullptr, &p, size);
    if (utctime) {
        ASN1_UTCTIME_free(utctime);
    }
    
    // Reset pointer for other parsing
    p = data;
    
    // Try to parse TS_REQ structure
    TS_REQ *ts_req = d2i_TS_REQ(nullptr, &p, size);
    if (ts_req) {
        // Free TS_REQ using provided API
        TS_REQ_free(ts_req);
    }
    
    // Reset pointer again
    p = data;
    
    // Try to parse TS_ACCURACY structure
    TS_ACCURACY *ts_accuracy = d2i_TS_ACCURACY(nullptr, &p, size);
    if (ts_accuracy) {
        // Free TS_ACCURACY using provided API
        TS_ACCURACY_free(ts_accuracy);
    }
    
    // Reset pointer for PKCS7 parsing
    p = data;
    PKCS7 *p7 = d2i_PKCS7(nullptr, &p, size);
    
    if (p7) {
        // Create X509_STORE for verification
        X509_STORE *store = X509_STORE_new();
        if (store) {
            // Set some basic verification flags
            X509_STORE_set_flags(store, X509_V_FLAG_ALLOW_PROXY_CERTS);
            
            // Create input BIO from file descriptor
            int in_fd = fuzz_fileno(in_file);
            BIO *indata = BIO_new_fd(in_fd, BIO_NOCLOSE);
            
            // Create output BIO from file descriptor
            int out_fd = fuzz_fileno(out_file);
            BIO *out = BIO_new_fd(out_fd, BIO_NOCLOSE);
            
            // Create an empty certificate stack
            struct stack_st_X509 *certs = sk_X509_new_null();
            
            if (indata && out && certs) {
                // Use the provided API for type checking
                X509_ALGOR *dummy_algor = X509_ALGOR_new();
                if (dummy_algor) {
                    // This function call is for demonstration - in reality,
                    // ossl_check_X509_ALGOR_type is used internally by OpenSSL
                    // We'll just call it to satisfy the requirement
                    X509_ALGOR *checked = ossl_check_X509_ALGOR_type(dummy_algor);
                    (void)checked;  // Avoid unused variable warning
                    X509_ALGOR_free(dummy_algor);
                }
                
                // Try to verify the PKCS7 structure using provided API
                // We use minimal flags for basic verification
                int verify_flags = PKCS7_NOVERIFY | PKCS7_NOCHAIN;
                int verify_result = PKCS7_verify(p7, certs, store, indata, out, verify_flags);
                (void)verify_result;  // Result not important for fuzzing
            }
            
            // Cleanup
            if (certs) sk_X509_free(certs);
            if (indata) BIO_free(indata);
            if (out) BIO_free(out);
            X509_STORE_free(store);
        }
        
        PKCS7_free(p7);
    }
    
    // Create EVP_RAND_CTX and use EVP_RAND_CTX_gettable_params
    // First get a RAND method
    EVP_RAND *rand = EVP_RAND_fetch(nullptr, "CTR-DRBG", nullptr);
    if (rand) {
        EVP_RAND_CTX *rand_ctx = EVP_RAND_CTX_new(rand, nullptr);
        if (rand_ctx) {
            // Use the provided API to get table of parameters
            const OSSL_PARAM *gettable = EVP_RAND_CTX_gettable_params(rand_ctx);
            (void)gettable;  // Result not important for fuzzing
            
            EVP_RAND_CTX_free(rand_ctx);
        }
        EVP_RAND_free(rand);
    }
    
    // Additional fuzzing: Try to parse the data as various structures
    p = data;
    long remaining = size;
    
    // Try multiple parsing attempts with different offsets
    for (size_t offset = 0; offset < size && offset < 100; offset += 10) {
        const unsigned char *current_p = data + offset;
        long current_len = size - offset;
        
        if (current_len > 0) {
            // Try parsing different structures
            TS_REQ *temp_req = d2i_TS_REQ(nullptr, &current_p, current_len);
            if (temp_req) {
                TS_REQ_free(temp_req);
                break;
            }
        }
    }
    
    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file if created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}