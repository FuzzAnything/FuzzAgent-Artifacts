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
//<ID> 629
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 5
//<Quality> {"density":12,"unique_branches":{"OPENSSL_hexchar2int":[[185,5,185,13,0,0,4,0]],"ipv4_from_asc":[[1252,12,1252,44,0,0,4,0],[1252,48,1252,66,0,0,4,0],[1253,12,1253,44,0,0,4,0],[1253,48,1253,59,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","OPENSSL_issetugid","TLS_method","SSL_CTX_new","SSL_CTX_free","SSL_CTX_use_PrivateKey_ASN1","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_set1_ip_asc","X509_VERIFY_PARAM_free","d2i_ADMISSION_SYNTAX","GENERAL_NAME_new","ADMISSION_SYNTAX_set0_admissionAuthority","ADMISSION_SYNTAX_free","d2i_OSSL_ATTRIBUTE_MAPPING","OSSL_ATTRIBUTE_MAPPING_free","OPENSSL_version_pre_release","SSL_new","SSL_get_rpoll_descriptor","EVP_sha3_256","ossl_check_OSSL_TIME_PERIOD_type","CRYPTO_aligned_alloc_array","CRYPTO_secure_clear_free","SSL_free","SSL_CTX_free"],"critical_calls":["OPENSSL_init_ssl","OPENSSL_issetugid","TLS_method","SSL_CTX_new","SSL_CTX_use_PrivateKey_ASN1","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_set1_ip_asc","X509_VERIFY_PARAM_free","d2i_ADMISSION_SYNTAX","GENERAL_NAME_new","ADMISSION_SYNTAX_set0_admissionAuthority","ADMISSION_SYNTAX_free","d2i_OSSL_ATTRIBUTE_MAPPING","OSSL_ATTRIBUTE_MAPPING_free","OPENSSL_version_pre_release","SSL_new","SSL_get_rpoll_descriptor","EVP_sha3_256","ossl_check_OSSL_TIME_PERIOD_type","CRYPTO_aligned_alloc_array","CRYPTO_secure_clear_free","SSL_free","SSL_CTX_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Initialize SSL library
    OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS | OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);

    // 2. Check if running with elevated privileges
    OPENSSL_issetugid();

    // 3. Create SSL context with TLS method
    const SSL_METHOD *method = TLS_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (!ctx) {
        return 0;
    }

    // 4. Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        SSL_CTX_free(ctx);
        assert_file_closed(&in_file);
	return 0;
    }

    // 5. Try to load private key from input data
    // We'll use the first portion of data as potential ASN.1 private key
    size_t key_len = size > 1024 ? 1024 : size;
    SSL_CTX_use_PrivateKey_ASN1(EVP_PKEY_RSA, ctx, data, key_len);

    // 6. Create X509 verification parameters
    X509_VERIFY_PARAM *param = X509_VERIFY_PARAM_new();
    if (param) {
        // Use portion of data as IP address string (null-terminated)
        size_t ip_len = size > 15 ? 15 : size;  // Max for IPv4 address
        char ip_buf[16] = {0};
        for (size_t i = 0; i < ip_len && i < 15; i++) {
            ip_buf[i] = data[i % size];
        }
        ip_buf[15] = '\0';
        X509_VERIFY_PARAM_set1_ip_asc(param, ip_buf);
        X509_VERIFY_PARAM_free(param);
    }

    // 7. Parse ADMISSION_SYNTAX from input data
    const uint8_t *admission_ptr = data;
    long admission_len = size > 100 ? 100 : size;
    ADMISSION_SYNTAX *admission = d2i_ADMISSION_SYNTAX(NULL, &admission_ptr, admission_len);
    
    if (admission) {
        GENERAL_NAME *gen_name = GENERAL_NAME_new();
        if (gen_name) {
            ADMISSION_SYNTAX_set0_admissionAuthority(admission, gen_name);
        }
        ADMISSION_SYNTAX_free(admission);
    }

    // 8. Parse OSSL_ATTRIBUTE_MAPPING from input data
    const uint8_t *attr_ptr = data + (size > 50 ? 50 : 0);
    long attr_len = size > 50 ? size - 50 : 0;
    if (attr_len > 0) {
        OSSL_ATTRIBUTE_MAPPING *attr_map = d2i_OSSL_ATTRIBUTE_MAPPING(NULL, &attr_ptr, attr_len);
        if (attr_map) {
            OSSL_ATTRIBUTE_MAPPING_free(attr_map);
        }
    }

    // 9. Get OpenSSL version information
    OPENSSL_version_pre_release();

    // 10. Create SSL connection
    SSL *ssl = SSL_new(ctx);
    if (ssl) {
        // 11. Get read poll descriptor
        BIO_POLL_DESCRIPTOR desc;
        SSL_get_rpoll_descriptor(ssl, &desc);

        // 12. Get SHA3-256 hash function
        const EVP_MD *sha3_256 = EVP_sha3_256();

        // 13. Check OSSL_TIME_PERIOD type (dummy operation)
        OSSL_TIME_PERIOD *time_period = NULL;
        ossl_check_OSSL_TIME_PERIOD_type(time_period);

        // 14. Allocate aligned memory using CRYPTO_aligned_alloc_array
        size_t alloc_size = size > 256 ? 256 : size;
        if (alloc_size > 0) {
            void *freeptr = NULL;
            void *aligned_mem = CRYPTO_aligned_alloc_array(1, alloc_size, 64, &freeptr, __FILE__, __LINE__);
            
            if (aligned_mem) {
                // Copy some data into allocated memory
                size_t copy_len = alloc_size > size ? size : alloc_size;
                memcpy(aligned_mem, data, copy_len);
                
                // 15. Securely clear and free the memory
                CRYPTO_secure_clear_free(aligned_mem, alloc_size, __FILE__, __LINE__);
            }
        }

        SSL_free(ssl);
    }

    // 16. Create output FILE* for writing
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Get file descriptor
        int out_fd = fuzz_fileno(out_file);
        
        // Write some data to output file
        size_t write_len = size > 100 ? 100 : size;
        if (write_len > 0) {
            fwrite(data, 1, write_len, out_file);
        }
        assert_file_closed(&out_file);;
    }

    // 17. Get input file descriptor
    int in_fd = fuzz_fileno(in_file);
    
    // Cleanup
    assert_file_closed(&in_file);;
    SSL_CTX_free(ctx);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}