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
//<ID> 202
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 4
//<Quality> {"density":6,"unique_branches":{"ossl_ffc_params_FIPS186_2_gen_verify":[[883,9,883,24,0,0,4,1]],"DSA_generate_parameters_ex":[[55,9,55,24,0,0,4,1],[71,9,71,28,0,0,4,1],[73,9,73,22,0,0,4,1]]},"library_calls":["X509_STORE_new","X509_STORE_set_check_revocation","EVP_aria_256_cbc","X509_STORE_free","d2i_X509_fp","SSL_CTX_new","TLS_method","SSL_new","SSL_get0_alpn_selected","OSSL_PARAM_construct_ulong","OSSL_PARAM_construct_end","OSSL_PARAM_set_ulong","OSSL_PARAM_get_ulong","EVP_PKEY_new","DSA_new","DSA_generate_parameters_ex","EVP_PKEY_set1_DSA","ASYNC_WAIT_CTX_new","ASYNC_WAIT_CTX_set_callback","X509_get0_pubkey","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","EVP_EncryptUpdate","EVP_EncryptFinal_ex","EVP_CIPHER_CTX_free","i2d_X509_fp","X509_free","X509_STORE_free","SSL_free","SSL_CTX_free","EVP_PKEY_free","DSA_free","ASYNC_WAIT_CTX_free"],"critical_calls":["X509_STORE_new","X509_STORE_set_check_revocation","EVP_aria_256_cbc","d2i_X509_fp","SSL_CTX_new","TLS_method","SSL_new","SSL_get0_alpn_selected","OSSL_PARAM_construct_ulong","OSSL_PARAM_construct_end","OSSL_PARAM_set_ulong","OSSL_PARAM_get_ulong","EVP_PKEY_new","DSA_new","DSA_generate_parameters_ex","EVP_PKEY_set1_DSA","ASYNC_WAIT_CTX_new","ASYNC_WAIT_CTX_set_callback","X509_get0_pubkey","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","EVP_EncryptUpdate","EVP_EncryptFinal_ex","EVP_CIPHER_CTX_free","i2d_X509_fp","X509_free","X509_STORE_free","SSL_free","SSL_CTX_free","EVP_PKEY_free","DSA_free","ASYNC_WAIT_CTX_free"],"visited":1}
/**/


// Custom revocation check callback
static int  id_000017_custom_revocation_check(X509_STORE_CTX *ctx) {
    // Always return success for fuzzing
    return 1;
}

// Async callback function
static int  id_000017_async_callback(void *arg) {
    // Simple callback that does nothing
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data

    // 1. Create X509_STORE and set revocation check
    X509_STORE *store = X509_STORE_new();
    if (store) {
        X509_STORE_set_check_revocation(store,  id_000017_custom_revocation_check);
    }

    // 2. Get ARIA-256-CBC cipher
    const EVP_CIPHER *cipher = EVP_aria_256_cbc();
    
    // 3. Parse X509 certificate from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        if (store) X509_STORE_free(store);
        assert_file_closed(&in_file);
	return 0;
    }
    
    X509 *cert = d2i_X509_fp(in_file, NULL);
    assert_file_closed(&in_file);;
    
    // 4. Get SSL ALPN data (simulate SSL context)
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = NULL;
    if (ssl_ctx) {
        ssl = SSL_new(ssl_ctx);
        if (ssl) {
            const unsigned char *alpn_data;
            unsigned int alpn_len;
            SSL_get0_alpn_selected(ssl, &alpn_data, &alpn_len);
        }
    }

    // 5. Use OSSL_PARAM with data
    OSSL_PARAM param[2];
    unsigned long ulong_val = 0;
    
    // Create a parameter from input data
    param[0] = OSSL_PARAM_construct_ulong("test", &ulong_val);
    param[1] = OSSL_PARAM_construct_end();
    
    // Set value from first 4 bytes of input
    if (size >= sizeof(unsigned long)) {
        unsigned long tmp;
        memcpy(&tmp, data, sizeof(unsigned long));
        OSSL_PARAM_set_ulong(&param[0], tmp);
        OSSL_PARAM_get_ulong(&param[0], &ulong_val);
    }

    // 6. Create DSA key and set to EVP_PKEY
    EVP_PKEY *pkey = EVP_PKEY_new();
    DSA *dsa = DSA_new();
    if (dsa && pkey) {
        // Generate DSA parameters (simplified for fuzzing)
        DSA_generate_parameters_ex(dsa, 512, NULL, 0, NULL, NULL, NULL);
        EVP_PKEY_set1_DSA(pkey, dsa);
    }

    // 7. Set async callback
    ASYNC_WAIT_CTX *wait_ctx = ASYNC_WAIT_CTX_new();
    if (wait_ctx) {
        ASYNC_WAIT_CTX_set_callback(wait_ctx,  id_000017_async_callback, NULL);
    }

    // 8. Get public key from certificate if available
    EVP_PKEY *cert_pubkey = NULL;
    if (cert) {
        cert_pubkey = X509_get0_pubkey(cert);
        // Note: X509_get0_pubkey returns const, but we can use it
    }

    // 9. Try to encrypt something using ARIA cipher (if we have enough data)
    if (cipher && size > 32) {
        EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new();
        if (cipher_ctx) {
            unsigned char key[32]; // ARIA-256 uses 32-byte key
            unsigned char iv[16];  // ARIA uses 16-byte IV
            unsigned char inbuf[64], outbuf[80];
            int outlen, tmplen;
            
            // Use first 32 bytes as key, next 16 as IV
            memcpy(key, data, 32);
            memcpy(iv, data + 32, 16);
            memcpy(inbuf, data + 48, size - 48 > 64 ? 64 : size - 48);
            
            EVP_EncryptInit_ex(cipher_ctx, cipher, NULL, key, iv);
            EVP_EncryptUpdate(cipher_ctx, outbuf, &outlen, inbuf, 64);
            EVP_EncryptFinal_ex(cipher_ctx, outbuf + outlen, &tmplen);
            outlen += tmplen;
            
            EVP_CIPHER_CTX_free(cipher_ctx);
        }
    }

    // 10. Write output to file (optional)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        if (cert) {
            i2d_X509_fp(out_file, cert);
        }
        assert_file_closed(&out_file);;
    }

    // Cleanup
    if (cert) X509_free(cert);
    if (store) X509_STORE_free(store);
    if (ssl) SSL_free(ssl);
    if (ssl_ctx) SSL_CTX_free(ssl_ctx);
    if (pkey) EVP_PKEY_free(pkey);
    if (dsa) DSA_free(dsa);
    if (wait_ctx) ASYNC_WAIT_CTX_free(wait_ctx);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}