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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 96, nr_unique_branch: 77
//<Quality> {"density":6,"unique_branches":{"X509V3_add_value_uchar":[[92,9,92,22,0,0,4,0]],"X509_PURPOSE_get0":[[135,9,135,16,0,0,4,0]],"CRYPTO_128_wrap":[[62,9,62,22,0,0,4,1],[62,26,62,38,0,0,4,1],[62,42,62,70,0,0,4,1],[67,9,67,12,0,0,4,1],[72,17,72,22,0,0,4,0],[72,17,72,22,0,0,4,1],[74,21,74,30,0,0,4,0],[74,21,74,30,0,0,4,1],[78,17,78,25,0,0,4,1]],"asn1_template_ex_i2d":[[267,9,267,34,0,0,4,0],[274,13,274,26,0,0,4,1],[277,13,277,37,0,0,4,1],[289,13,289,25,0,0,4,1],[294,17,294,22,0,0,4,1],[302,21,302,52,0,0,4,1],[315,13,315,24,0,0,4,1],[318,13,318,37,0,0,4,1],[323,13,323,17,0,0,4,0],[323,13,323,17,0,0,4,1],[323,21,323,30,0,0,4,1],[328,13,328,37,0,0,4,1],[335,13,335,22,0,0,4,1]],"X509_VERIFY_PARAM_lookup":[[1034,9,1034,28,0,0,4,1]],"X509_STORE_CTX_cleanup":[[3035,9,3035,27,0,0,4,0],[3036,13,3036,32,0,0,4,0]],"ASN1_item_ex_i2d":[[101,13,101,26,0,0,4,0]],"CRYPTO_128_wrap_pad":[[209,9,209,19,0,0,4,1],[209,23,209,50,0,0,4,1],[213,9,213,13,0,0,4,1],[223,9,223,24,0,0,4,0],[223,9,223,24,0,0,4,1]],"X509_VERIFY_PARAM_inherit":[[414,23,414,40,24,0,4,0],[414,45,414,55,24,0,4,1],[414,59,414,77,24,0,4,0]],"SSL_set_psk_server_callback":[[6057,9,6057,19,0,0,4,1],[32,6,32,19,2,0,4,1],[33,23,33,61,2,0,4,0]],"X509_STORE_CTX_init":[[2878,9,2878,20,0,0,4,1],[2910,9,2910,22,0,0,4,0],[2915,9,2915,22,0,0,4,0],[2915,26,2915,53,0,0,4,1],[2920,9,2920,22,0,0,4,0],[2920,26,2920,51,0,0,4,1],[2925,9,2925,22,0,0,4,0],[2925,26,2925,50,0,0,4,1],[2930,9,2930,22,0,0,4,0],[2930,26,2930,47,0,0,4,1],[2935,9,2935,22,0,0,4,0],[2935,26,2935,57,0,0,4,1],[2940,9,2940,22,0,0,4,0],[2940,26,2940,48,0,0,4,1],[2945,9,2945,22,0,0,4,0],[2945,26,2945,50,0,0,4,1],[2950,9,2950,22,0,0,4,0],[2950,26,2950,49,0,0,4,1],[2955,9,2955,22,0,0,4,0],[2955,26,2955,53,0,0,4,1],[2960,9,2960,22,0,0,4,0],[2960,26,2960,53,0,0,4,1],[2965,9,2965,22,0,0,4,0],[2965,26,2965,52,0,0,4,1],[2971,9,2971,27,0,0,4,1],[2977,9,2977,22,0,0,4,1],[2979,14,2979,70,0,0,4,1],[2982,9,2982,52,0,0,4,1],[2989,9,2989,48,0,0,4,0],[2993,13,2993,23,0,0,4,1],[2997,9,2998,27,0,0,4,0]],"X509_STORE_CTX_set_default":[[3202,9,3202,22,0,0,4,1]],"asn1_set_seq_out":[[408,9,408,16,0,0,4,1],[423,9,423,17,0,0,4,0],[424,21,424,52,0,0,4,1]],"X509_PURPOSE_get_by_id":[[161,9,161,36,0,0,4,1],[163,9,163,24,0,0,4,0]],"X509_STORE_CTX_print_verify_cb":[[456,9,456,16,0,0,4,1]]},"library_calls":["X509V3_add_value_uchar","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","i2d_OSSL_ROLE_SPEC_CERT_ID_SYNTAX","CRYPTO_free","OPENSSL_sk_free","ossl_check_OSSL_ROLE_SPEC_CERT_ID_sk_type","X509_STORE_CTX_new","X509_STORE_new","X509_STORE_CTX_init","X509_STORE_CTX_print_verify_cb","X509_STORE_free","X509_STORE_CTX_free","AES_set_encrypt_key","CRYPTO_128_wrap_pad","SSL_CTX_new","TLS_server_method","SSL_new","SSL_set_psk_server_callback","SSL_free","SSL_CTX_free","BIO_snprintf","ERR_GET_REASON","ERR_get_error","ossl_check_ERR_STRING_DATA_lh_plain_type","OPENSSL_sk_pop_free","ossl_check_CONF_VALUE_sk_type","ossl_check_CONF_VALUE_freefunc_type"],"critical_calls":["X509V3_add_value_uchar","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","i2d_OSSL_ROLE_SPEC_CERT_ID_SYNTAX","CRYPTO_free","OPENSSL_sk_free","ossl_check_OSSL_ROLE_SPEC_CERT_ID_sk_type","X509_STORE_CTX_new","X509_STORE_new","X509_STORE_CTX_init","X509_STORE_CTX_print_verify_cb","X509_STORE_free","X509_STORE_CTX_free","AES_set_encrypt_key","CRYPTO_128_wrap_pad","SSL_CTX_new","TLS_server_method","SSL_new","SSL_set_psk_server_callback","SSL_free","SSL_CTX_free","BIO_snprintf","ERR_GET_REASON","ERR_get_error","ossl_check_ERR_STRING_DATA_lh_plain_type","OPENSSL_sk_pop_free","ossl_check_CONF_VALUE_freefunc_type","ossl_check_CONF_VALUE_sk_type"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if no data
    if (size == 0) {
        return 0;
    }

    // 1. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Create FILE* for output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 3. Use X509V3_add_value_uchar
    STACK_OF(CONF_VALUE) *extlist = NULL;
    // Use first min(100, size) bytes as name and value
    size_t name_len = size > 50 ? 50 : size / 2;
    size_t value_len = size - name_len;
    
    // Ensure we have valid strings
    unsigned char *name = (unsigned char *)malloc(name_len + 1);
    unsigned char *value = (unsigned char *)malloc(value_len + 1);
    
    if (name && value) {
        memcpy(name, data, name_len);
        name[name_len] = '\0';
        memcpy(value, data + name_len, value_len);
        value[value_len] = '\0';
        
        X509V3_add_value_uchar((const char *)name, value, &extlist);
    }
    
    // 4. Use i2d_OSSL_ROLE_SPEC_CERT_ID_SYNTAX
    // Create a minimal OSSL_ROLE_SPEC_CERT_ID_SYNTAX structure
    OSSL_ROLE_SPEC_CERT_ID_SYNTAX *role_spec = sk_OSSL_ROLE_SPEC_CERT_ID_new_null();
    unsigned char *der_out = NULL;
    if (role_spec) {
        int der_len = i2d_OSSL_ROLE_SPEC_CERT_ID_SYNTAX(role_spec, &der_out);
        // der_out is allocated by i2d function
        if (der_out) {
            OPENSSL_free(der_out);
        }
        sk_OSSL_ROLE_SPEC_CERT_ID_free(role_spec);
    }

    // 5. Use X509_STORE_CTX_print_verify_cb
    // Create a minimal X509_STORE_CTX for testing
    X509_STORE_CTX *store_ctx = X509_STORE_CTX_new();
    if (store_ctx) {
        // Initialize with dummy values
        X509 *cert = NULL;
        STACK_OF(X509) *chain = NULL;
        X509_STORE *store = X509_STORE_new();
        
        if (store) {
            X509_STORE_CTX_init(store_ctx, store, cert, chain);
            // Call the print verify callback
            int ok = 1; // Simulate verification success
            X509_STORE_CTX_print_verify_cb(ok, store_ctx);
            X509_STORE_free(store);
        }
        X509_STORE_CTX_free(store_ctx);
    }

    // 6. Use CRYPTO_128_wrap_pad
    // Prepare key, IV, and data for wrapping
    unsigned char key[16] = {0};
    unsigned char icv[8] = {0};
    unsigned char out[256] = {0};
    
    // Use part of input data as plaintext (max 128 bytes for this example)
    size_t wrap_len = size > 128 ? 128 : size;
    unsigned char *in = (unsigned char *)malloc(wrap_len);
    if (in) {
        memcpy(in, data, wrap_len);
        
        // Use AES_encrypt as the block function
        AES_KEY aes_key;
        AES_set_encrypt_key(key, 128, &aes_key);
        
        size_t result_len = CRYPTO_128_wrap_pad(&aes_key, icv, out, in, wrap_len, 
                                              (block128_f)AES_encrypt);
        
        free(in);
    }

    // 7. Use SSL_set_psk_server_callback
    // Create SSL context and set PSK callback
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_server_method());
    SSL *ssl = SSL_new(ssl_ctx);
    if (ssl) {
        // Define a simple PSK server callback
        SSL_psk_server_cb_func psk_callback = [](SSL *ssl, const char *identity,
                                                unsigned char *psk, unsigned int max_psk_len) -> unsigned int {
            // Simple test callback - return fixed PSK
            static const unsigned char test_psk[] = "test_psk";
            unsigned int len = sizeof(test_psk) - 1;
            if (len > max_psk_len) len = max_psk_len;
            memcpy(psk, test_psk, len);
            return len;
        };
        
        SSL_set_psk_server_callback(ssl, psk_callback);
        SSL_free(ssl);
    }
    if (ssl_ctx) {
        SSL_CTX_free(ssl_ctx);
    }

    // 8. Use BIO_snprintf
    char buf[256];
    // Format a string with size information
    BIO_snprintf(buf, sizeof(buf), "Input size: %zu bytes, Processed: %zu bytes", size, size);

    // 9. Use ossl_check_ERR_STRING_DATA_lh_plain_type
    // Create a dummy ERR_STRING_DATA for testing
    ERR_STRING_DATA err_data;
    err_data.error = ERR_GET_REASON(ERR_get_error());
    err_data.string = "Test error string";
    
    ERR_STRING_DATA *checked = ossl_check_ERR_STRING_DATA_lh_plain_type(&err_data);
    // checked is just the same pointer, used for type checking

    // 10. Cleanup
    if (extlist) {
        sk_CONF_VALUE_pop_free(extlist, X509V3_conf_free);
    }
    
    free(name);
    free(value);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}