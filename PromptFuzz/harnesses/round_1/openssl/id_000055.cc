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
//<ID> 627
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["OPENSSL_init_ssl","OPENSSL_issetugid","TLS_method","SSL_CTX_new","SSL_CTX_use_PrivateKey_ASN1","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_set1_ip_asc","X509_VERIFY_PARAM_free","d2i_ADMISSION_SYNTAX","GENERAL_NAME_new","ADMISSION_SYNTAX_set0_admissionAuthority","ADMISSION_SYNTAX_free","d2i_OSSL_ATTRIBUTE_MAPPING","OSSL_ATTRIBUTE_MAPPING_free","OPENSSL_version_pre_release","SSL_new","SSL_get_rpoll_descriptor","EVP_sha3_256","ossl_check_OSSL_TIME_PERIOD_type","CRYPTO_aligned_alloc_array","CRYPTO_secure_clear_free","SSL_free","SSL_CTX_free"],"critical_calls":["OPENSSL_init_ssl","OPENSSL_issetugid","TLS_method","SSL_CTX_new","SSL_CTX_use_PrivateKey_ASN1","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_set1_ip_asc","X509_VERIFY_PARAM_free","d2i_ADMISSION_SYNTAX","GENERAL_NAME_new","ADMISSION_SYNTAX_set0_admissionAuthority","ADMISSION_SYNTAX_free","d2i_OSSL_ATTRIBUTE_MAPPING","OSSL_ATTRIBUTE_MAPPING_free","OPENSSL_version_pre_release","SSL_new","SSL_get_rpoll_descriptor","EVP_sha3_256","ossl_check_OSSL_TIME_PERIOD_type","CRYPTO_aligned_alloc_array","CRYPTO_secure_clear_free","SSL_free","SSL_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Initialize OpenSSL
    uint64_t opts = OPENSSL_INIT_LOAD_SSL_STRINGS | OPENSSL_INIT_ADD_ALL_CIPHERS | 
                   OPENSSL_INIT_ADD_ALL_DIGESTS | OPENSSL_INIT_LOAD_CONFIG;
    OPENSSL_init_ssl(opts, nullptr);
    
    // 2. Check if running with elevated privileges
    OPENSSL_issetugid();
    
    // 3. Create SSL context with TLS method
    const SSL_METHOD *method = TLS_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (!ctx) {
        return 0;
    }
    
    // 4. Try to load private key from input data
    // Use first part of input for private key (maximum 1024 bytes)
    size_t key_len = size > 1024 ? 1024 : size;
    SSL_CTX_use_PrivateKey_ASN1(EVP_PKEY_RSA, ctx, data, static_cast<long>(key_len));
    
    // 5. Create and configure X509 verification parameters
    X509_VERIFY_PARAM *param = X509_VERIFY_PARAM_new();
    if (param) {
        // Try to set IP address from a sample string
        X509_VERIFY_PARAM_set1_ip_asc(param, "192.168.1.1");
        X509_VERIFY_PARAM_free(param);
    }
    
    // 6. Parse ADMISSION_SYNTAX from input data
    const unsigned char *p = data;
    long remaining = static_cast<long>(size);
    ADMISSION_SYNTAX *admission_syntax = d2i_ADMISSION_SYNTAX(nullptr, &p, remaining);
    
    if (admission_syntax) {
        // Create a GENERAL_NAME and set it as admission authority
        GENERAL_NAME *general_name = GENERAL_NAME_new();
        if (general_name) {
            ADMISSION_SYNTAX_set0_admissionAuthority(admission_syntax, general_name);
        }
        ADMISSION_SYNTAX_free(admission_syntax);
    }
    
    // 7. Parse OSSL_ATTRIBUTE_MAPPING from input data
    // Reset pointer to start of data
    p = data;
    remaining = static_cast<long>(size);
    OSSL_ATTRIBUTE_MAPPING *attr_mapping = d2i_OSSL_ATTRIBUTE_MAPPING(nullptr, &p, remaining);
    
    if (attr_mapping) {
        OSSL_ATTRIBUTE_MAPPING_free(attr_mapping);
    }
    
    // 8. Get OpenSSL version information
    const char *pre_release = OPENSSL_version_pre_release();
    (void)pre_release; // Use variable to avoid unused warning
    
    // 9. Create SSL connection
    SSL *ssl = SSL_new(ctx);
    if (ssl) {
        // 10. Get read poll descriptor
        BIO_POLL_DESCRIPTOR desc;
        SSL_get_rpoll_descriptor(ssl, &desc);
        
        // 11. Get SHA3-256 digest method
        const EVP_MD *sha3_256 = EVP_sha3_256();
        (void)sha3_256; // Use variable to avoid unused warning
        
        // 12. Type check for OSSL_TIME_PERIOD (cast operation)
        OSSL_TIME_PERIOD dummy_period;
        OSSL_TIME_PERIOD *period_ptr = ossl_check_OSSL_TIME_PERIOD_type(&dummy_period);
        (void)period_ptr; // Use variable to avoid unused warning
        
        // 13. Allocate aligned memory
        void *freeptr = nullptr;
        size_t num = 1;
        size_t elem_size = 64;
        size_t align = 16;
        void *aligned_mem = CRYPTO_aligned_alloc_array(num, elem_size, align, &freeptr, __FILE__, __LINE__);
        
        // 14. Securely clear and free the allocated memory
        if (aligned_mem) {
            CRYPTO_secure_clear_free(aligned_mem, num * elem_size, __FILE__, __LINE__);
        }
        
        SSL_free(ssl);
    }
    
    // 15. Clean up SSL context
    SSL_CTX_free(ctx);
    
    // 16. Try to use the input data with FILE operations as specified in instructions
    // Create input FILE from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Get file descriptor
        int fd_in = fuzz_fileno(in_file);
        (void)fd_in; // Use variable to avoid unused warning
        
        // Create output FILE
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            int fd_out = fuzz_fileno(out_file);
            (void)fd_out; // Use variable to avoid unused warning
            
            // Read from input and write to output (simple copy)
            uint8_t buffer[1024];
            size_t bytes_read;
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
                fwrite(buffer, 1, bytes_read, out_file);
            }
            
            assert_file_closed(&out_file);;
            remove("output_file"); // Clean up output file
        }
        assert_file_closed(&in_file);;
    }
    
    assert_file_closed(&in_file);
	return 0;
}