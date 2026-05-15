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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 17
//<Quality> {"density":5,"unique_branches":{"dgram_adjust_rcv_timeout":[[319,9,319,47,0,0,4,1]],"dgram_reset_rcv_timeout":[[401,9,401,47,0,0,4,1]],"BIO_dgram_should_retry":[[2804,9,2804,17,0,0,4,1],[2804,21,2804,30,0,0,4,0]],"dgram_read":[[430,9,430,20,0,0,4,0],[430,24,430,32,0,0,4,0],[434,13,434,27,0,0,4,1],[439,13,439,29,0,0,4,0],[439,33,439,41,0,0,4,1],[443,13,443,20,0,0,4,0],[444,17,444,44,0,0,4,1]],"BIO_dgram_non_fatal_error":[[2835,5,2835,21,0,0,4,1],[2840,5,2840,15,0,0,4,1],[2850,5,2850,16,0,0,4,1],[2854,5,2854,21,0,0,4,1],[2858,5,2858,18,0,0,4,1],[2862,5,2862,12,0,0,4,0]]},"library_calls":["OPENSSL_init_crypto","OCSP_REQUEST_new","OCSP_REQUEST_get_ext","COMP_CTX_new","COMP_zlib","COMP_CTX_get_method","OSSL_ATTRIBUTE_VALUE_MAPPING_it","SSL_CTX_new","TLS_method","SSL_new","SSL_set1_compressed_cert","ASN1_UTCTIME_new","ASN1_UTCTIME_adj","ECPKPARAMETERS_new","EVP_CIPHER_CTX_new","EVP_aes_256_cbc","OSSL_PARAM_construct_end","EVP_DecryptInit_ex2","ASYNC_init_thread","ASYNC_get_current_job","ASYNC_get_wait_ctx","ASYNC_cleanup_thread","BIO_s_datagram","BIO_new","BIO_int_ctrl","BIO_read","OCSP_REQUEST_free","COMP_CTX_free","SSL_free","SSL_CTX_free","ASN1_UTCTIME_free","ECPKPARAMETERS_free","EVP_CIPHER_CTX_free","BIO_free"],"critical_calls":["OPENSSL_init_crypto","OCSP_REQUEST_new","OCSP_REQUEST_get_ext","COMP_CTX_new","COMP_zlib","COMP_CTX_get_method","OSSL_ATTRIBUTE_VALUE_MAPPING_it","SSL_CTX_new","TLS_method","SSL_new","SSL_set1_compressed_cert","ASN1_UTCTIME_new","ASN1_UTCTIME_adj","ECPKPARAMETERS_new","EVP_CIPHER_CTX_new","EVP_aes_256_cbc","OSSL_PARAM_construct_end","EVP_DecryptInit_ex2","ASYNC_init_thread","ASYNC_get_current_job","ASYNC_get_wait_ctx","ASYNC_cleanup_thread","BIO_s_datagram","BIO_new","BIO_int_ctrl","BIO_read","OCSP_REQUEST_free","COMP_CTX_free","SSL_free","SSL_CTX_free","ASN1_UTCTIME_free","ECPKPARAMETERS_free","EVP_CIPHER_CTX_free","BIO_free"],"visited":2}
/*Looking at the provided APIs, I'll create a fuzz driver that simulates a scenario where we're processing compressed certificate data from an OCSP request, decrypting it, and handling various SSL/encryption operations. The event will involve parsing an OCSP request, handling compression, certificate processing, and decryption.

Here's the complete fuzz driver:

*/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for operations

    // Initialize OpenSSL if needed
    OPENSSL_init_crypto(0, NULL);

    // 1. Create and use OCSP_REQUEST_get_ext
    OCSP_REQUEST *ocsp_req = OCSP_REQUEST_new();
    if (ocsp_req) {
        // Try to get extension (will likely fail but we call the API)
        const X509_EXTENSION *ext = OCSP_REQUEST_get_ext(ocsp_req, 0);
        (void)ext; // Use variable to avoid unused warning
    }

    // 2. Handle compression context
    COMP_CTX *comp_ctx = COMP_CTX_new(COMP_zlib());
    if (comp_ctx) {
        const COMP_METHOD *method = COMP_CTX_get_method(comp_ctx);
        (void)method; // Use variable
    }

    // 3. Get ASN1 item for attribute value mapping
    const ASN1_ITEM *item = OSSL_ATTRIBUTE_VALUE_MAPPING_it();
    (void)item;

    // 4. SSL compressed certificate handling
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = SSL_new(ssl_ctx);
    if (ssl && size > 50) {
        size_t comp_len = size < 100 ? size : 100;
        size_t orig_len = comp_len * 2; // Estimate original length
        unsigned char *comp_data = (unsigned char*)malloc(comp_len);
        if (comp_data) {
            memcpy(comp_data, data, comp_len < size ? comp_len : size);
            // Try to set compressed certificate
            SSL_set1_compressed_cert(ssl, NID_zlib_compression, 
                                    comp_data, comp_len, orig_len);
            free(comp_data);
        }
    }

    // 5. Create and adjust ASN1_UTCTIME
    ASN1_UTCTIME *utc_time = ASN1_UTCTIME_new();
    if (utc_time) {
        time_t current_time = time(NULL);
        // Use part of input data for offset calculations
        int offset_day = data[0] % 30; // Limit to reasonable range
        long offset_sec = data[1] % 86400; // Limit to seconds in day
        
        ASN1_UTCTIME *adjusted = ASN1_UTCTIME_adj(utc_time, current_time, 
                                                 offset_day, offset_sec);
        (void)adjusted;
    }

    // 6. Create ECPKPARAMETERS
    ECPKPARAMETERS *ecpk_params = ECPKPARAMETERS_new();
    (void)ecpk_params;

    // 7. Setup decryption context
    EVP_CIPHER_CTX *decrypt_ctx = EVP_CIPHER_CTX_new();
    if (decrypt_ctx && size > 48) {
        // Use AES-256-CBC cipher
        const EVP_CIPHER *cipher = EVP_aes_256_cbc();
        // Extract key and IV from input data
        unsigned char key[32]; // 256-bit key
        unsigned char iv[16];  // 128-bit IV
        
        // Safely copy from input data
        size_t copy_size_key = size > 32 ? 32 : size;
        memcpy(key, data, copy_size_key);
        
        size_t copy_size_iv = (size > 48 && (size - 32) > 16) ? 16 : 
                             (size > 32 ? size - 32 : 0);
        if (copy_size_iv > 0) {
            memcpy(iv, data + 32, copy_size_iv);
        } else {
            memset(iv, 0, 16); // Default IV if not enough data
        }
        
        OSSL_PARAM params[2];
        params[0] = OSSL_PARAM_construct_end();
        
        // Initialize decryption
        EVP_DecryptInit_ex2(decrypt_ctx, cipher, key, iv, params);
    }

    // 8. Async job handling
    ASYNC_WAIT_CTX *wait_ctx = NULL;
    if (ASYNC_init_thread(1, 1)) { // Initialize async
        ASYNC_JOB *job = ASYNC_get_current_job();
        if (job) {
            wait_ctx = ASYNC_get_wait_ctx(job);
        }
        ASYNC_cleanup_thread();
    }

    // 9. Create datagram BIO
    const BIO_METHOD *datagram_method = BIO_s_datagram();
    BIO *bio = NULL;
    if (datagram_method) {
        bio = BIO_new(datagram_method);
        if (bio) {
            // Create a temporary file for BIO operations
            FILE *in_file = fmemopen((void*)data, size, "rb");
            if (in_file) {
                int fd = fuzz_fileno(in_file);
                BIO_set_fd(bio, fd, BIO_NOCLOSE);
                
                // Try to read some data through BIO
                char buffer[100];
                BIO_read(bio, buffer, sizeof(buffer));
                
                assert_file_closed(&in_file);;
            }
        }
    }

    // 10. Additional processing with input data
    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        (void)fd; // Use file descriptor
        
        // Read and process some data
        uint8_t read_buffer[50];
        fread(read_buffer, 1, sizeof(read_buffer), in_file);
        
        assert_file_closed(&in_file);;
    }

    // Cleanup all resources
    if (ocsp_req) OCSP_REQUEST_free(ocsp_req);
    if (comp_ctx) COMP_CTX_free(comp_ctx);
    if (ssl) SSL_free(ssl);
    if (ssl_ctx) SSL_CTX_free(ssl_ctx);
    if (utc_time) ASN1_UTCTIME_free(utc_time);
    if (ecpk_params) ECPKPARAMETERS_free(ecpk_params);
    if (decrypt_ctx) EVP_CIPHER_CTX_free(decrypt_ctx);
    if (bio) BIO_free(bio);

    assert_file_closed(&in_file);
	return 0;
}