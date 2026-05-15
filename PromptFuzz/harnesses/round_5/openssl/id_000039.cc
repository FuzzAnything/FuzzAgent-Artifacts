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
//<ID> 343
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 5
//<Quality> {"density":9,"unique_branches":{"ossl_ffc_params_FIPS186_2_generate":[[1065,9,1067,33,0,0,4,0]],"DSA_generate_parameters_ex":[[62,13,63,31,0,0,4,0]],"ossl_dsa_generate_ffc_parameters":[[41,9,41,16,0,0,4,1]],"ossl_ffc_params_FIPS186_2_gen_verify":[[884,13,884,29,0,0,4,0],[1043,9,1043,20,0,0,4,1]]},"library_calls":["X509_STORE_new","X509_STORE_set_check_revocation","EVP_aria_256_cbc","d2i_X509_fp","TLS_method","SSL_CTX_new","SSL_new","SSL_get0_alpn_selected","OSSL_PARAM_construct_ulong","OSSL_PARAM_construct_end","OSSL_PARAM_set_ulong","OSSL_PARAM_get_ulong","EVP_PKEY_new","DSA_new","DSA_generate_parameters_ex","EVP_PKEY_set1_DSA","ASYNC_WAIT_CTX_new","ASYNC_WAIT_CTX_set_callback","X509_get0_pubkey","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","EVP_EncryptUpdate","EVP_EncryptFinal_ex","i2d_X509_fp","PKCS7_ISSUER_AND_SERIAL_new","i2d_PKCS7_ISSUER_AND_SERIAL","CRYPTO_free","EVP_CIPHER_CTX_free","EVP_PKEY_free","DSA_free","ASYNC_WAIT_CTX_free","SSL_free","SSL_CTX_free","X509_free","X509_STORE_free","PKCS7_ISSUER_AND_SERIAL_free"],"critical_calls":["X509_STORE_new","X509_STORE_set_check_revocation","EVP_aria_256_cbc","d2i_X509_fp","TLS_method","SSL_CTX_new","SSL_new","SSL_get0_alpn_selected","OSSL_PARAM_construct_ulong","OSSL_PARAM_construct_end","OSSL_PARAM_set_ulong","OSSL_PARAM_get_ulong","EVP_PKEY_new","DSA_new","DSA_generate_parameters_ex","EVP_PKEY_set1_DSA","ASYNC_WAIT_CTX_new","ASYNC_WAIT_CTX_set_callback","X509_get0_pubkey","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","EVP_EncryptUpdate","EVP_EncryptFinal_ex","i2d_X509_fp","PKCS7_ISSUER_AND_SERIAL_new","i2d_PKCS7_ISSUER_AND_SERIAL","CRYPTO_free","EVP_CIPHER_CTX_free","EVP_PKEY_free","DSA_free","ASYNC_WAIT_CTX_free","SSL_free","SSL_CTX_free","X509_free","X509_STORE_free","PKCS7_ISSUER_AND_SERIAL_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create X509_STORE and set revocation check
    X509_STORE *store = X509_STORE_new();
    if (store) {
        X509_STORE_set_check_revocation(store, nullptr);
    }

    // 2. Get ARIA-256-CBC cipher
    const EVP_CIPHER *cipher = EVP_aria_256_cbc();

    // 3. Read X509 certificate from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    X509 *cert = nullptr;
    if (in_file) {
        cert = d2i_X509_fp(in_file, nullptr);
        assert_file_closed(&in_file);;
    }

    // 4. Create SSL context and SSL object
    const SSL_METHOD *method = TLS_method();
    SSL_CTX *ssl_ctx = SSL_CTX_new(method);
    SSL *ssl = nullptr;
    if (ssl_ctx) {
        ssl = SSL_new(ssl_ctx);
        if (ssl) {
            const unsigned char *alpn_data = nullptr;
            unsigned int alpn_len = 0;
            SSL_get0_alpn_selected(ssl, &alpn_data, &alpn_len);
        }
    }

    // 5. Use OSSL_PARAM with ulong operations
    unsigned long ulong_val = 0;
    OSSL_PARAM params[2];
    params[0] = OSSL_PARAM_construct_ulong("test_key", &ulong_val);
    params[1] = OSSL_PARAM_construct_end();
    
    if (size > sizeof(unsigned long)) {
        unsigned long input_val = 0;
        memcpy(&input_val, data, sizeof(unsigned long));
        OSSL_PARAM_set_ulong(&params[0], input_val);
        unsigned long retrieved_val = 0;
        OSSL_PARAM_get_ulong(&params[0], &retrieved_val);
    }

    // 6. Create EVP_PKEY and DSA key
    EVP_PKEY *pkey = EVP_PKEY_new();
    DSA *dsa = DSA_new();
    if (dsa && size > 0) {
        int counter_ret = 0;
        unsigned long h_ret = 0;
        // Use first min(20, size) bytes as seed
        int seed_len = size > 20 ? 20 : size;
        DSA_generate_parameters_ex(dsa, 1024, data, seed_len, 
                                   &counter_ret, &h_ret, nullptr);
        if (pkey) {
            EVP_PKEY_set1_DSA(pkey, dsa);
        }
    }

    // 7. ASYNC operations
    ASYNC_WAIT_CTX *async_ctx = ASYNC_WAIT_CTX_new();
    if (async_ctx) {
        ASYNC_WAIT_CTX_set_callback(async_ctx, nullptr, nullptr);
    }

    // 8. Use certificate public key for encryption if cert exists
    EVP_CIPHER_CTX *enc_ctx = nullptr;
    if (cert) {
        EVP_PKEY *cert_pubkey = X509_get0_pubkey(cert);
        
        // Create encryption context and encrypt some data
        enc_ctx = EVP_CIPHER_CTX_new();
        if (enc_ctx && cipher && size > 48) { // Need at least key + IV + some data
            const unsigned char *key = data;
            const unsigned char *iv = data + 32; // ARIA-256 key is 32 bytes
            const unsigned char *plaintext = data + 48; // key + IV
            
            int plaintext_len = size - 48;
            if (plaintext_len > 0) {
                unsigned char *outbuf = (unsigned char *)malloc(plaintext_len + EVP_MAX_BLOCK_LENGTH);
                int outlen1 = 0, outlen2 = 0;
                
                EVP_EncryptInit_ex(enc_ctx, cipher, nullptr, key, iv);
                EVP_EncryptUpdate(enc_ctx, outbuf, &outlen1, plaintext, plaintext_len);
                EVP_EncryptFinal_ex(enc_ctx, outbuf + outlen1, &outlen2);
                
                free(outbuf);
            }
        }
    }

    // 9. Write certificate back to file if it exists
    if (cert) {
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            i2d_X509_fp(out_file, cert);
            assert_file_closed(&out_file);;
        }
    }

    // 10. Create and serialize PKCS7_ISSUER_AND_SERIAL
    PKCS7_ISSUER_AND_SERIAL *ias = PKCS7_ISSUER_AND_SERIAL_new();
    if (ias) {
        unsigned char *der = nullptr;
        int der_len = i2d_PKCS7_ISSUER_AND_SERIAL(ias, &der);
        if (der) {
            OPENSSL_free(der);
        }
    }

    // 11. Cleanup
    if (enc_ctx) {
        EVP_CIPHER_CTX_free(enc_ctx);
    }
    if (pkey) {
        EVP_PKEY_free(pkey);
    }
    if (dsa) {
        DSA_free(dsa);
    }
    if (async_ctx) {
        ASYNC_WAIT_CTX_free(async_ctx);
    }
    if (ssl) {
        SSL_free(ssl);
    }
    if (ssl_ctx) {
        SSL_CTX_free(ssl_ctx);
    }
    if (cert) {
        X509_free(cert);
    }
    if (store) {
        X509_STORE_free(store);
    }
    if (ias) {
        PKCS7_ISSUER_AND_SERIAL_free(ias);
    }

    assert_file_closed(&in_file);
	return 0;
}