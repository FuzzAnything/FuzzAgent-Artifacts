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
//<ID> 203
//<Prompt> []
/*<Combination>: [
*/
//<score> 7.5, nr_unique_branch: 5
//<Quality> {"density":5,"unique_branches":{"dsa_cb":[[28,9,28,37,0,0,4,1],[33,16,33,45,0,0,4,0],[33,16,33,45,0,0,4,1]],"ossl_cipher_generic_block_update":[[345,9,345,23,0,0,4,1],[359,9,359,23,0,0,4,1]]},"library_calls":["d2i_X509_fp","PEM_read_X509","X509_get0_pubkey","EVP_PKEY_up_ref","DSA_new","ASN1_d2i_fp","EVP_PKEY_set1_DSA","SSL_CTX_new","TLS_method","SSL_new","SSL_set_alpn_protos","SSL_get0_alpn_selected","X509_STORE_new","X509_STORE_set_check_revocation","EVP_aria_256_cbc","OSSL_PARAM_construct_ulong","OSSL_PARAM_construct_end","OSSL_PARAM_get_ulong","ASYNC_WAIT_CTX_new","ASYNC_WAIT_CTX_set_callback","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","EVP_EncryptUpdate","EVP_EncryptFinal_ex","EVP_CIPHER_CTX_free","ASYNC_WAIT_CTX_free","SSL_free","SSL_CTX_free","X509_STORE_free","EVP_PKEY_free","X509_free","DSA_free"],"critical_calls":["d2i_X509_fp","PEM_read_X509","X509_get0_pubkey","EVP_PKEY_up_ref","DSA_new","ASN1_d2i_fp","EVP_PKEY_set1_DSA","SSL_CTX_new","TLS_method","SSL_new","SSL_set_alpn_protos","SSL_get0_alpn_selected","X509_STORE_new","X509_STORE_set_check_revocation","EVP_aria_256_cbc","OSSL_PARAM_construct_ulong","OSSL_PARAM_construct_end","OSSL_PARAM_get_ulong","ASYNC_WAIT_CTX_new","ASYNC_WAIT_CTX_set_callback","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","EVP_EncryptUpdate","EVP_EncryptFinal_ex","EVP_CIPHER_CTX_free","ASYNC_WAIT_CTX_free","SSL_free","SSL_CTX_free","X509_STORE_free","EVP_PKEY_free","X509_free","DSA_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Variables for SSL ALPN
    SSL_CTX *ctx = NULL;
    SSL *ssl = NULL;
    const unsigned char *alpn_data = NULL;
    unsigned int alpn_len = 0;
    
    // Variables for X509 and EVP_PKEY
    X509 *x509 = NULL;
    EVP_PKEY *pkey = NULL;
    DSA *dsa = NULL;
    
    // Variables for OSSL_PARAM
    OSSL_PARAM param_array[2];
    unsigned long ulong_val = 0;
    
    // Variables for ASYNC
    ASYNC_WAIT_CTX *async_ctx = NULL;
    
    // Variables for encryption
    const EVP_CIPHER *cipher = NULL;
    EVP_CIPHER_CTX *cipher_ctx = NULL;
    unsigned char iv[16] = {0};
    unsigned char inbuf[1024], outbuf[1024 + EVP_MAX_BLOCK_LENGTH];
    int outlen = 0, inlen = 0;
    
    // Variables for X509_STORE
    X509_STORE *store = NULL;
    
    // 1. Parse X509 certificate from input data
    fseek(in_file, 0, SEEK_SET);
    x509 = d2i_X509_fp(in_file, NULL);
    if (!x509) {
        // Try to parse as PEM if DER fails
        fseek(in_file, 0, SEEK_SET);
        x509 = PEM_read_X509(in_file, NULL, NULL, NULL);
    }
    
    // 2. Get public key from X509 certificate
    if (x509) {
        pkey = X509_get0_pubkey(x509);
        // We need to increment reference count since X509_get0_pubkey returns const
        if (pkey) EVP_PKEY_up_ref(pkey);
    }
    
    // 3. Create DSA key (for EVP_PKEY_set1_DSA)
    dsa = DSA_new();
    if (dsa) {
        // Try to read DSA parameters from remaining input
        fseek(in_file, 0, SEEK_SET);
        d2i_DSAparams_fp(in_file, &dsa);
    }
    
    // 4. Set DSA key into EVP_PKEY
    if (dsa && pkey) {
        EVP_PKEY_set1_DSA(pkey, dsa);
    }
    
    // 5. Create SSL context and SSL object
    ctx = SSL_CTX_new(TLS_method());
    if (ctx) {
        ssl = SSL_new(ctx);
        if (ssl) {
            // Set ALPN protocols (dummy data)
            const unsigned char alpn_protos[] = {8, 'h', 't', 't', 'p', '/', '1', '.', '1'};
            SSL_set_alpn_protos(ssl, alpn_protos, sizeof(alpn_protos));
            
            // Get selected ALPN protocol
            SSL_get0_alpn_selected(ssl, &alpn_data, &alpn_len);
        }
    }
    
    // 6. Create X509_STORE and set revocation check callback
    store = X509_STORE_new();
    if (store) {
        // Dummy revocation check callback
        auto dummy_check_revocation = [](X509_STORE_CTX *ctx) -> int {
            return 1; // Assume valid for fuzzing
        };
        X509_STORE_set_check_revocation(store, dummy_check_revocation);
    }
    
    // 7. Get ARIA-256-CBC cipher
    cipher = EVP_aria_256_cbc();
    
    // 8. Set up OSSL_PARAM and get unsigned long value
    if (size >= sizeof(unsigned long)) {
        unsigned long tmp;
        memcpy(&tmp, data, sizeof(unsigned long));
        param_array[0] = OSSL_PARAM_construct_ulong("test", &tmp);
        param_array[1] = OSSL_PARAM_construct_end();
        
        OSSL_PARAM_get_ulong(&param_array[0], &ulong_val);
    }
    
    // 9. Set up ASYNC_WAIT_CTX with callback
    async_ctx = ASYNC_WAIT_CTX_new();
    if (async_ctx) {
        // Dummy callback function
        auto dummy_callback = [](void *arg) -> int {
            return 1;
        };
        ASYNC_WAIT_CTX_set_callback(async_ctx, dummy_callback, NULL);
    }
    
    // 10. Use the cipher for encryption/decryption with remaining input data
    if (cipher) {
        cipher_ctx = EVP_CIPHER_CTX_new();
        if (cipher_ctx) {
            // Use first 16 bytes as IV if available
            size_t iv_len = (size > 16) ? 16 : size;
            if (iv_len > 0) {
                memcpy(iv, data, iv_len);
            }
            
            // Initialize encryption
            if (EVP_EncryptInit_ex(cipher_ctx, cipher, NULL, 
                                  (const unsigned char*)"01234567890123456789012345678901", // 32-byte key
                                  iv)) {
                // Encrypt remaining data
                size_t remaining = (size > iv_len) ? size - iv_len : 0;
                if (remaining > 0 && remaining <= sizeof(inbuf)) {
                    memcpy(inbuf, data + iv_len, remaining);
                    inlen = (int)remaining;
                    
                    if (EVP_EncryptUpdate(cipher_ctx, outbuf, &outlen, inbuf, inlen)) {
                        // Write encrypted data to output file
                        fwrite(outbuf, 1, outlen, out_file);
                        
                        // Finalize encryption
                        int finallen = 0;
                        if (EVP_EncryptFinal_ex(cipher_ctx, outbuf + outlen, &finallen)) {
                            fwrite(outbuf + outlen, 1, finallen, out_file);
                        }
                    }
                }
            }
        }
    }
    
    // 11. Clean up all allocated resources
    if (cipher_ctx) EVP_CIPHER_CTX_free(cipher_ctx);
    if (async_ctx) ASYNC_WAIT_CTX_free(async_ctx);
    if (ssl) SSL_free(ssl);
    if (ctx) SSL_CTX_free(ctx);
    if (store) X509_STORE_free(store);
    if (pkey) EVP_PKEY_free(pkey);
    if (x509) X509_free(x509);
    if (dsa) DSA_free(dsa);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}