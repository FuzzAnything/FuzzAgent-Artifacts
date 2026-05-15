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
//<ID> 539
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 2
//<Quality> {"density":7,"unique_branches":{"dgram_ctrl":[[585,5,585,24,0,0,4,0],[592,5,592,22,0,0,4,1]]},"library_calls":["OPENSSL_init_crypto","ASYNC_init_thread","TLS_method","SSL_CTX_new","ASYNC_cleanup_thread","SSL_new","SSL_CTX_free","ASYNC_cleanup_thread","COMP_zlib","COMP_CTX_new","COMP_CTX_get_method","SSL_set1_compressed_cert","COMP_CTX_free","ASN1_UTCTIME_new","ASN1_UTCTIME_adj","ASN1_UTCTIME_free","ECPKPARAMETERS_new","ECPKPARAMETERS_free","EVP_CIPHER_CTX_new","EVP_aes_256_cbc","OSSL_PARAM_construct_end","EVP_DecryptInit_ex2","EVP_CIPHER_CTX_free","ASYNC_get_current_job","ASYNC_get_wait_ctx","OCSP_REQUEST_new","OCSP_REQUEST_get_ext","OCSP_REQUEST_free","BIO_s_datagram","BIO_new","BIO_int_ctrl","BIO_free","OSSL_ATTRIBUTE_VALUE_MAPPING_it","SSL_free","SSL_CTX_free","ASYNC_cleanup_thread"],"critical_calls":["OPENSSL_init_crypto","ASYNC_init_thread","TLS_method","SSL_CTX_new","SSL_new","COMP_zlib","COMP_CTX_new","COMP_CTX_get_method","SSL_set1_compressed_cert","COMP_CTX_free","ASN1_UTCTIME_new","ASN1_UTCTIME_adj","ASN1_UTCTIME_free","ECPKPARAMETERS_new","ECPKPARAMETERS_free","EVP_CIPHER_CTX_new","EVP_aes_256_cbc","OSSL_PARAM_construct_end","EVP_DecryptInit_ex2","EVP_CIPHER_CTX_free","ASYNC_get_current_job","ASYNC_get_wait_ctx","OCSP_REQUEST_new","OCSP_REQUEST_get_ext","OCSP_REQUEST_free","BIO_s_datagram","BIO_new","BIO_int_ctrl","BIO_free","OSSL_ATTRIBUTE_VALUE_MAPPING_it","SSL_free","SSL_CTX_free","ASYNC_cleanup_thread"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL
    if (!OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG, NULL)) {
        return 0;
    }

    // Initialize async thread
    if (!ASYNC_init_thread(1024 * 1024, 1024 * 1024)) {
        return 0;
    }

    // Create SSL context
    const SSL_METHOD *method = TLS_method();
    SSL_CTX *ssl_ctx = SSL_CTX_new(method);
    if (!ssl_ctx) {
        ASYNC_cleanup_thread();
        return 0;
    }

    // Create SSL object
    SSL *ssl = SSL_new(ssl_ctx);
    if (!ssl) {
        SSL_CTX_free(ssl_ctx);
        ASYNC_cleanup_thread();
        return 0;
    }

    // Create compression context for zlib
    COMP_METHOD *comp_method = COMP_zlib();
    COMP_CTX *comp_ctx = COMP_CTX_new(comp_method);
    if (comp_ctx) {
        // Get compression method
        const COMP_METHOD *retrieved_method = COMP_CTX_get_method(comp_ctx);
        
        // Set compressed certificate if we have enough data
        if (size >= 128) {
            // Use first 64 bytes as compressed data, original length 128
            SSL_set1_compressed_cert(ssl, 1 /* algorithm ID */, 
                                     (unsigned char*)data, 64, 128);
        }
        
        COMP_CTX_free(comp_ctx);
    }

    // Create ASN1_UTCTIME and adjust it
    ASN1_UTCTIME *utctime = ASN1_UTCTIME_new();
    if (utctime) {
        time_t current_time = time(NULL);
        // Adjust time with offsets from input data
        int offset_day = (size > 0) ? (data[0] % 30) : 0;
        long offset_sec = (size > 1) ? (data[1] % 86400) : 0;
        ASN1_UTCTIME_adj(utctime, current_time, offset_day, offset_sec);
        ASN1_UTCTIME_free(utctime);
    }

    // Create ECPKPARAMETERS
    ECPKPARAMETERS *ecpkparams = ECPKPARAMETERS_new();
    if (ecpkparams) {
        ECPKPARAMETERS_free(ecpkparams);
    }

    // Create and setup EVP cipher context for decryption
    EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new();
    if (cipher_ctx) {
        const EVP_CIPHER *cipher = EVP_aes_256_cbc();
        OSSL_PARAM params[2];
        params[0] = OSSL_PARAM_construct_end();
        
        // Use parts of input data as key and IV if available
        unsigned char key[32] = {0};
        unsigned char iv[16] = {0};
        
        if (size >= 48) {
            memcpy(key, data, 32);
            memcpy(iv, data + 32, 16);
        } else if (size > 0) {
            // Partial data
            memcpy(key, data, size > 32 ? 32 : size);
            if (size > 32) {
                memcpy(iv, data + 32, size - 32 > 16 ? 16 : size - 32);
            }
        }
        
        EVP_DecryptInit_ex2(cipher_ctx, cipher, key, iv, params);
        EVP_CIPHER_CTX_free(cipher_ctx);
    }

    // Async operations
    ASYNC_JOB *current_job = ASYNC_get_current_job();
    if (current_job) {
        ASYNC_WAIT_CTX *wait_ctx = ASYNC_get_wait_ctx(current_job);
        // wait_ctx can be used here if needed
    }

    // Create OCSP_REQUEST and get extension
    FILE *in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        OCSP_REQUEST *ocsp_req = NULL;
        // Try to read OCSP request from input data
        // Note: In real usage, we'd use d2i_OCSP_REQUEST_fp,
        // but for fuzzing we'll create a dummy request
        ocsp_req = OCSP_REQUEST_new();
        if (ocsp_req) {
            // Try to get an extension (location 0)
            const X509_EXTENSION *ext = OCSP_REQUEST_get_ext(ocsp_req, 0);
            // ext will be NULL since we didn't add any extensions
            
            OCSP_REQUEST_free(ocsp_req);
        }
        assert_file_closed(&in_file);;
    }

    // Create datagram BIO
    const BIO_METHOD *bio_method = BIO_s_datagram();
    BIO *bio = BIO_new(bio_method);
    if (bio) {
        // Set integer control parameter using input data
        int ctrl_value = (size > 0) ? data[0] : 0;
        BIO_int_ctrl(bio, BIO_CTRL_RESET, 0, ctrl_value);
        BIO_free(bio);
    }

    // Get ASN1 item for attribute value mapping
    const ASN1_ITEM *attr_mapping_item = OSSL_ATTRIBUTE_VALUE_MAPPING_it();
    // This is a constant, just need to call it

    // Note: The following functions are type casting/thunk functions
    // and don't need to be explicitly called in normal usage:
    // - sk_OSSL_IETF_ATTR_SYNTAX_VALUE_freefunc_thunk
    // - ossl_check_ASN1_STRING_sk_type
    // - ossl_check_OSSL_CMP_CERTREPMESSAGE_freefunc_type
    // They are used internally by stack implementations

    // Cleanup
    SSL_free(ssl);
    SSL_CTX_free(ssl_ctx);
    ASYNC_cleanup_thread();

    assert_file_closed(&in_file);
	return 0;
}