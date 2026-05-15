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
//<ID> 167
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["OPENSSL_init_crypto","OCSP_REQUEST_new","OCSP_REQUEST_get_ext","OCSP_REQUEST_free","COMP_zlib","COMP_CTX_new","COMP_CTX_get_method","COMP_CTX_free","OSSL_ATTRIBUTE_VALUE_MAPPING_it","TLS_method","SSL_CTX_new","SSL_new","SSL_set1_compressed_cert","SSL_free","SSL_CTX_free","ASN1_UTCTIME_new","ASN1_UTCTIME_adj","ASN1_UTCTIME_free","ECPKPARAMETERS_new","ECPKPARAMETERS_free","EVP_CIPHER_CTX_new","EVP_aes_256_cbc","OSSL_PARAM_construct_end","EVP_DecryptInit_ex2","EVP_CIPHER_CTX_free","ASYNC_init_thread","ASYNC_get_current_job","ASYNC_get_wait_ctx","ASYNC_cleanup_thread","BIO_s_datagram","BIO_new","BIO_int_ctrl","BIO_free","EC_KEY_new_by_curve_name","EC_KEY_generate_key","EC_KEY_free"],"critical_calls":["OPENSSL_init_crypto","OCSP_REQUEST_new","OCSP_REQUEST_get_ext","OCSP_REQUEST_free","COMP_zlib","COMP_CTX_new","COMP_CTX_get_method","COMP_CTX_free","OSSL_ATTRIBUTE_VALUE_MAPPING_it","TLS_method","SSL_CTX_new","SSL_new","SSL_set1_compressed_cert","SSL_free","SSL_CTX_free","ASN1_UTCTIME_new","ASN1_UTCTIME_adj","ASN1_UTCTIME_free","ECPKPARAMETERS_new","ECPKPARAMETERS_free","EVP_CIPHER_CTX_new","EVP_aes_256_cbc","OSSL_PARAM_construct_end","EVP_DecryptInit_ex2","EVP_CIPHER_CTX_free","ASYNC_init_thread","ASYNC_get_current_job","ASYNC_get_wait_ctx","ASYNC_cleanup_thread","BIO_s_datagram","BIO_new","BIO_int_ctrl","BIO_free","EC_KEY_new_by_curve_name","EC_KEY_generate_key","EC_KEY_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 48) {
	return 0;
	}
	  // Need minimum data for key/IV etc.

    // 1. Initialize OpenSSL crypto
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, nullptr);

    // 2. Create OCSP request and get extension
    OCSP_REQUEST *ocsp_req = OCSP_REQUEST_new();
    if (ocsp_req) {
        const X509_EXTENSION *ext = OCSP_REQUEST_get_ext(ocsp_req, 0);  // Will likely be NULL
        (void)ext;  // Use variable to avoid warning
        OCSP_REQUEST_free(ocsp_req);
    }

    // 3. Compression context with zlib
    COMP_METHOD *zlib_method = COMP_zlib();
    COMP_CTX *comp_ctx = COMP_CTX_new(zlib_method);
    if (comp_ctx) {
        const COMP_METHOD *retrieved_method = COMP_CTX_get_method(comp_ctx);
        (void)retrieved_method;
        COMP_CTX_free(comp_ctx);
    }

    // 4. Get ASN.1 item for attribute mapping
    const ASN1_ITEM *attr_mapping_it = OSSL_ATTRIBUTE_VALUE_MAPPING_it();
    (void)attr_mapping_it;

    // 5. SSL/TLS context setup
    const SSL_METHOD *tls_method = TLS_method();
    SSL_CTX *ssl_ctx = SSL_CTX_new(tls_method);
    if (ssl_ctx) {
        SSL *ssl = SSL_new(ssl_ctx);
        if (ssl) {
            // Use first 4 bytes as algorithm ID, next 4 as orig_length
            uint32_t algorithm = 0;
            uint32_t orig_length = 0;
            if (size >= 8) {
                memcpy(&algorithm, data, 4);
                memcpy(&orig_length, data + 4, 4);
                algorithm %= 10;  // Keep in reasonable range
                orig_length %= 65536;  // Keep in reasonable range
            }
            
            // Use next part for compressed data (if available)
            size_t comp_length = size > 8 ? size - 8 : 0;
            unsigned char *comp_data = const_cast<unsigned char*>(size > 8 ? data + 8 : data);
            
            SSL_set1_compressed_cert(ssl, algorithm, comp_data, comp_length, orig_length);
            SSL_free(ssl);
        }
        SSL_CTX_free(ssl_ctx);
    }

    // 6. ASN1_UTCTIME operations
    ASN1_UTCTIME *utc_time = ASN1_UTCTIME_new();
    if (utc_time) {
        time_t current = time(nullptr);
        ASN1_UTCTIME_adj(utc_time, current, 0, 0);  // No offset
        ASN1_UTCTIME_free(utc_time);
    }

    // 7. ECPKPARAMETERS
    ECPKPARAMETERS *ecpk_params = ECPKPARAMETERS_new();
    if (ecpk_params) {
        ECPKPARAMETERS_free(ecpk_params);
    }

    // 8. AES-256-CBC decryption setup
    EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new();
    if (cipher_ctx) {
        const EVP_CIPHER *aes_256_cbc = EVP_aes_256_cbc();
        unsigned char key[32] = {0};
        unsigned char iv[16] = {0};
        
        // Use data for key and IV if available
        size_t key_iv_len = size > 8 ? 48 : size;
        if (key_iv_len >= 48) {
            memcpy(key, data, 32);
            memcpy(iv, data + 32, 16);
        } else if (size > 0) {
            // Use what we have
            memcpy(key, data, key_iv_len > 32 ? 32 : key_iv_len);
            if (key_iv_len > 32) {
                memcpy(iv, data + 32, key_iv_len - 32 > 16 ? 16 : key_iv_len - 32);
            }
        }
        
        OSSL_PARAM params[2];
        params[0] = OSSL_PARAM_construct_end();
        
        EVP_DecryptInit_ex2(cipher_ctx, aes_256_cbc, key, iv, params);
        EVP_CIPHER_CTX_free(cipher_ctx);
    }

    // 9. Async operations
    ASYNC_init_thread(1024 * 1024, 1024 * 1024);
    ASYNC_JOB *job = ASYNC_get_current_job();
    if (job) {
        ASYNC_WAIT_CTX *wait_ctx = ASYNC_get_wait_ctx(job);
        (void)wait_ctx;
    }
    ASYNC_cleanup_thread();

    // 10. Datagram BIO
    const BIO_METHOD *datagram_method = BIO_s_datagram();
    BIO *bio = BIO_new(datagram_method);
    if (bio) {
        // Use file descriptor from input data file
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            int fd = fuzz_fileno(in_file);
            BIO_int_ctrl(bio, BIO_C_SET_FD, 0, fd);
            assert_file_closed(&in_file);;
        }
        BIO_free(bio);
    }

    // 11. CMS certificates (simulated - we don't have actual CMS data)
    // Create a temporary CMS structure using available functions
    // Note: We'll create a minimal CMS structure even though we can't fully populate it
    FILE *tmp_out = fopen("output_file", "wb");
    if (tmp_out) {
        fwrite(data, 1, size > 1024 ? 1024 : size, tmp_out);
        assert_file_closed(&tmp_out);;
    }

    // 12. EC key generation
    EC_KEY *ec_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if (ec_key) {
        EC_KEY_generate_key(ec_key);
        EC_KEY_free(ec_key);
    }

    // Clean up temporary file
    remove("output_file");

    assert_file_closed(&tmp_out);
	assert_file_name_closed("output_file");
	return 0;
}