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
//<ID> 628
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["OPENSSL_init_ssl","OPENSSL_issetugid","OPENSSL_version_pre_release","TLS_method","SSL_CTX_new","SSL_CTX_use_PrivateKey_ASN1","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_set1_ip_asc","X509_VERIFY_PARAM_free","SSL_CTX_free","d2i_ADMISSION_SYNTAX","GENERAL_NAME_new","ADMISSION_SYNTAX_set0_admissionAuthority","ADMISSION_SYNTAX_free","d2i_OSSL_ATTRIBUTE_MAPPING","OSSL_ATTRIBUTE_MAPPING_free","SSL_new","SSL_get_rpoll_descriptor","EVP_sha3_256","ossl_check_OSSL_TIME_PERIOD_type","CRYPTO_aligned_alloc_array","CRYPTO_secure_clear_free","SSL_free","SSL_CTX_free"],"critical_calls":["OPENSSL_init_ssl","OPENSSL_issetugid","OPENSSL_version_pre_release","TLS_method","SSL_CTX_new","SSL_CTX_use_PrivateKey_ASN1","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_set1_ip_asc","X509_VERIFY_PARAM_free","d2i_ADMISSION_SYNTAX","GENERAL_NAME_new","ADMISSION_SYNTAX_set0_admissionAuthority","ADMISSION_SYNTAX_free","d2i_OSSL_ATTRIBUTE_MAPPING","OSSL_ATTRIBUTE_MAPPING_free","SSL_new","SSL_get_rpoll_descriptor","EVP_sha3_256","ossl_check_OSSL_TIME_PERIOD_type","CRYPTO_aligned_alloc_array","CRYPTO_secure_clear_free","SSL_free","SSL_CTX_free"],"visited":0}
/*Here's a C++ fuzzing driver that uses the specified OpenSSL APIs to simulate an SSL/TLS connection setup with certificate verification and attribute processing:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL library
    uint64_t opts = OPENSSL_INIT_LOAD_SSL_STRINGS | 
                   OPENSSL_INIT_LOAD_CRYPTO_STRINGS |
                   OPENSSL_INIT_ADD_ALL_CIPHERS |
                   OPENSSL_INIT_ADD_ALL_DIGESTS;
    
    OPENSSL_init_ssl(opts, NULL);
    
    // Check if running with elevated privileges
    OPENSSL_issetugid();
    
    // Get OpenSSL version info
    const char* version = OPENSSL_version_pre_release();
    (void)version; // Use to avoid unused variable warning
    
    // Create SSL context using TLS method
    const SSL_METHOD* method = TLS_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) {
        return 0;
    }
    
    // Try to load private key from input data
    if (size > 0) {
        SSL_CTX_use_PrivateKey_ASN1(EVP_PKEY_RSA, ctx, data, (long)size);
    }
    
    // Create and configure verification parameters
    X509_VERIFY_PARAM* param = X509_VERIFY_PARAM_new();
    if (param) {
        X509_VERIFY_PARAM_set1_ip_asc(param, "192.168.1.1");
        X509_VERIFY_PARAM_free(param);
    }
    
    // Create FILE pointer for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        SSL_CTX_free(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Parse ADMISSION_SYNTAX structure from input
    const unsigned char* p = data;
    ADMISSION_SYNTAX* admission = d2i_ADMISSION_SYNTAX(NULL, &p, (long)(size > 100 ? 100 : size));
    
    if (admission) {
        // Create and set admission authority
        GENERAL_NAME* gn = GENERAL_NAME_new();
        if (gn) {
            ADMISSION_SYNTAX_set0_admissionAuthority(admission, gn);
        }
        ADMISSION_SYNTAX_free(admission);
    }
    
    // Parse OSSL_ATTRIBUTE_MAPPING from input
    p = data;
    OSSL_ATTRIBUTE_MAPPING* attr_map = d2i_OSSL_ATTRIBUTE_MAPPING(NULL, &p, (long)(size > 50 ? 50 : size));
    if (attr_map) {
        OSSL_ATTRIBUTE_MAPPING_free(attr_map);
    }
    
    // Create SSL connection
    SSL* ssl = SSL_new(ctx);
    if (ssl) {
        // Get poll descriptor (not actually used for polling in fuzzing)
        BIO_POLL_DESCRIPTOR desc;
        SSL_get_rpoll_descriptor(ssl, &desc);
        
        // Get SHA3-256 digest method
        const EVP_MD* sha3_256 = EVP_sha3_256();
        (void)sha3_256; // Use to avoid unused variable warning
        
        // Type check OSSL_TIME_PERIOD (dummy operation)
        OSSL_TIME_PERIOD dummy_period;
        ossl_check_OSSL_TIME_PERIOD_type(&dummy_period);
        
        // Allocate aligned memory for testing
        void* freeptr = NULL;
        size_t alloc_size = size > 1024 ? 1024 : size;
        if (alloc_size > 0) {
            void* aligned_mem = CRYPTO_aligned_alloc_array(1, alloc_size, 16, &freeptr, __FILE__, __LINE__);
            if (aligned_mem) {
                // Clear and free the memory
                CRYPTO_secure_clear_free(aligned_mem, alloc_size, __FILE__, __LINE__);
            }
        }
    }
    
    // Cleanup
    if (ssl) {
        SSL_free(ssl);
    }
    
    // Close input file
    assert_file_closed(&in_file);;
    
    // Free SSL context
    SSL_CTX_free(ctx);
    
    assert_file_closed(&in_file);
	return 0;
}