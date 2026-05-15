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
//<ID> 597
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["OPENSSL_init_ssl","OPENSSL_issetugid","OPENSSL_version_pre_release","TLS_method","SSL_CTX_new","SSL_CTX_use_PrivateKey_ASN1","EVP_sha3_256","CRYPTO_aligned_alloc_array","d2i_OSSL_ATTRIBUTE_MAPPING","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_set1_ip_asc","X509_VERIFY_PARAM_free","OCSP_SINGLERESP_new","OCSP_SINGLERESP_get_ext_by_critical","OCSP_SINGLERESP_free","ADMISSION_SYNTAX_new","GENERAL_NAME_new","X509_NAME_new","ADMISSION_SYNTAX_set0_admissionAuthority","ADMISSION_SYNTAX_set0_admissionAuthority","ADMISSION_SYNTAX_free","OSSL_TIME_PERIOD_new","ossl_check_OSSL_TIME_PERIOD_type","OSSL_TIME_PERIOD_free","CRYPTO_secure_clear_free","OSSL_ATTRIBUTE_MAPPING_free","SSL_CTX_free"],"critical_calls":["OPENSSL_init_ssl","OPENSSL_issetugid","OPENSSL_version_pre_release","TLS_method","SSL_CTX_new","SSL_CTX_use_PrivateKey_ASN1","EVP_sha3_256","CRYPTO_aligned_alloc_array","d2i_OSSL_ATTRIBUTE_MAPPING","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_set1_ip_asc","X509_VERIFY_PARAM_free","OCSP_SINGLERESP_new","OCSP_SINGLERESP_get_ext_by_critical","OCSP_SINGLERESP_free","ADMISSION_SYNTAX_new","GENERAL_NAME_new","X509_NAME_new","ADMISSION_SYNTAX_set0_admissionAuthority","ADMISSION_SYNTAX_free","OSSL_TIME_PERIOD_new","ossl_check_OSSL_TIME_PERIOD_type","OSSL_TIME_PERIOD_free","CRYPTO_secure_clear_free","OSSL_ATTRIBUTE_MAPPING_free","SSL_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize SSL with default settings
    OPENSSL_init_ssl(0, NULL);
    
    // Check if running with elevated privileges
    OPENSSL_issetugid();
    
    // Get OpenSSL pre-release version info
    const char *pre_release = OPENSSL_version_pre_release();
    
    // Create SSL context
    const SSL_METHOD *method = TLS_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    
    if (ctx) {
        // Use part of input data as private key (assuming first 512 bytes contain key material)
        size_t key_len = size > 512 ? 512 : size;
        if (key_len > 0) {
            // Try to use the input as an RSA private key (type 6)
            SSL_CTX_use_PrivateKey_ASN1(6, ctx, data, key_len);
        }
        
        // Get SHA3-256 digest for potential use
        const EVP_MD *sha3_256 = EVP_sha3_256();
        
        // Create a file from input data for reading
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            // Allocate aligned memory using input parameters
            void *freeptr = NULL;
            size_t alloc_size = size > 16 ? 16 : size;
            void *aligned_mem = CRYPTO_aligned_alloc_array(
                1, alloc_size, 64, &freeptr, __FILE__, __LINE__);
            
            if (aligned_mem && size > 0) {
                // Copy some input data to aligned memory
                size_t copy_len = size > alloc_size ? alloc_size : size;
                memcpy(aligned_mem, data, copy_len);
            }
            
            // Parse OSSL_ATTRIBUTE_MAPPING from input data
            const unsigned char *p = data;
            long remaining = size;
            OSSL_ATTRIBUTE_MAPPING *attr_mapping = d2i_OSSL_ATTRIBUTE_MAPPING(
                NULL, &p, remaining);
            
            // Set X509 verify parameter with an IP address from string "127.0.0.1"
            X509_VERIFY_PARAM *param = X509_VERIFY_PARAM_new();
            if (param) {
                X509_VERIFY_PARAM_set1_ip_asc(param, "127.0.0.1");
                X509_VERIFY_PARAM_free(param);
            }
            
            // Create and work with OCSP single response
            OCSP_SINGLERESP *single_resp = OCSP_SINGLERESP_new();
            if (single_resp) {
                // Check for critical extensions
                OCSP_SINGLERESP_get_ext_by_critical(single_resp, 1, -1);
                OCSP_SINGLERESP_free(single_resp);
            }
            
            // Create and work with ADMISSION_SYNTAX
            ADMISSION_SYNTAX *admission_syntax = ADMISSION_SYNTAX_new();
            if (admission_syntax) {
                // Create a GENERAL_NAME for admission authority
                GENERAL_NAME *gen_name = GENERAL_NAME_new();
                if (gen_name) {
                    // Set type to directoryName (4)
                    gen_name->type = 4;
                    gen_name->d.directoryName = X509_NAME_new();
                    
                    // Set admission authority
                    ADMISSION_SYNTAX_set0_admissionAuthority(
                        admission_syntax, gen_name);
                } else {
                    ADMISSION_SYNTAX_set0_admissionAuthority(
                        admission_syntax, NULL);
                }
                ADMISSION_SYNTAX_free(admission_syntax);
            }
            
            // Create OSSL_TIME_PERIOD structure
            OSSL_TIME_PERIOD *time_period = OSSL_TIME_PERIOD_new();
            if (time_period) {
                // Check type using ossl_check function
                OSSL_TIME_PERIOD *checked = ossl_check_OSSL_TIME_PERIOD_type(
                    time_period);
                OSSL_TIME_PERIOD_free(time_period);
            }
            
            // Clean up aligned memory
            if (aligned_mem) {
                CRYPTO_secure_clear_free(aligned_mem, 
                    alloc_size, __FILE__, __LINE__);
            }
            
            // Clean up OSSL_ATTRIBUTE_MAPPING
            if (attr_mapping) {
                OSSL_ATTRIBUTE_MAPPING_free(attr_mapping);
            }
            
            assert_file_closed(&in_file);;
        }
        
        // Create output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            // Write some data to output file if we have input
            if (size > 0) {
                fwrite(data, 1, size > 1024 ? 1024 : size, out_file);
            }
            assert_file_closed(&out_file);;
        }
        
        // Clean up SSL context
        SSL_CTX_free(ctx);
    }
    
    return 0;
}