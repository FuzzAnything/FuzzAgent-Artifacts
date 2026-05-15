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
//<ID> 341
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["X509_STORE_new","X509_STORE_set_check_revocation","EVP_aria_256_cbc","d2i_X509_fp","TLS_method","SSL_CTX_new","SSL_new","SSL_get0_alpn_selected","OSSL_PARAM_construct_ulong","OSSL_PARAM_construct_end","OSSL_PARAM_set_ulong","OSSL_PARAM_get_ulong","EVP_PKEY_new","DSA_new","DSA_generate_parameters_ex","EVP_PKEY_set1_DSA","ASYNC_WAIT_CTX_new","ASYNC_WAIT_CTX_set_callback","X509_get0_pubkey","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","EVP_CIPHER_get_block_size","EVP_EncryptUpdate","EVP_EncryptFinal_ex","i2d_X509_fp","PKCS7_ISSUER_AND_SERIAL_new","i2d_PKCS7_ISSUER_AND_SERIAL","CRYPTO_free","PKCS7_ISSUER_AND_SERIAL_free","EVP_CIPHER_CTX_free","ASYNC_WAIT_CTX_free","EVP_PKEY_free","DSA_free","X509_free","SSL_free","SSL_CTX_free","X509_STORE_free"],"critical_calls":["X509_STORE_new","X509_STORE_set_check_revocation","EVP_aria_256_cbc","d2i_X509_fp","TLS_method","SSL_CTX_new","SSL_new","SSL_get0_alpn_selected","OSSL_PARAM_construct_ulong","OSSL_PARAM_construct_end","OSSL_PARAM_set_ulong","OSSL_PARAM_get_ulong","EVP_PKEY_new","DSA_new","DSA_generate_parameters_ex","EVP_PKEY_set1_DSA","ASYNC_WAIT_CTX_new","ASYNC_WAIT_CTX_set_callback","X509_get0_pubkey","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","EVP_CIPHER_get_block_size","EVP_EncryptUpdate","EVP_EncryptFinal_ex","i2d_X509_fp","PKCS7_ISSUER_AND_SERIAL_new","i2d_PKCS7_ISSUER_AND_SERIAL","CRYPTO_free","PKCS7_ISSUER_AND_SERIAL_free","EVP_CIPHER_CTX_free","ASYNC_WAIT_CTX_free","EVP_PKEY_free","DSA_free","X509_free","SSL_free","SSL_CTX_free","X509_STORE_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 256) {
        return 0;
    }

    // Create X509_STORE and set revocation check
    X509_STORE *store = X509_STORE_new();
    X509_STORE_set_check_revocation(store, NULL);

    // Get ARIA-256-CBC cipher
    const EVP_CIPHER *cipher = EVP_aria_256_cbc();

    // Read X509 certificate from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    X509 *x509 = NULL;
    d2i_X509_fp(in_file, &x509);
    assert_file_closed(&in_file);;

    // Create SSL context and SSL object
    const SSL_METHOD *method = TLS_method();
    SSL_CTX *ssl_ctx = SSL_CTX_new(method);
    SSL *ssl = SSL_new(ssl_ctx);

    // Get ALPN selected
    const unsigned char *alpn_data = NULL;
    unsigned int alpn_len = 0;
    SSL_get0_alpn_selected(ssl, &alpn_data, &alpn_len);

    // Create and manipulate OSSL_PARAM
    unsigned long param_value = 0;
    OSSL_PARAM params[2];
    params[0] = OSSL_PARAM_construct_ulong("test_key", &param_value);
    params[1] = OSSL_PARAM_construct_end();

    OSSL_PARAM_set_ulong(&params[0], 42);
    OSSL_PARAM_get_ulong(&params[0], &param_value);

    // Create DSA key and generate parameters
    EVP_PKEY *pkey = EVP_PKEY_new();
    DSA *dsa = DSA_new();
    int counter = 0;
    unsigned long h = 0;
    const unsigned char *seed = data;
    int seed_len = size > 20 ? 20 : size;
    DSA_generate_parameters_ex(dsa, 1024, seed, seed_len, &counter, &h, NULL);
    EVP_PKEY_set1_DSA(pkey, dsa);

    // Create ASYNC wait context
    ASYNC_WAIT_CTX *async_ctx = ASYNC_WAIT_CTX_new();
    ASYNC_WAIT_CTX_set_callback(async_ctx, NULL, NULL);

    // Get public key from X509
    if (x509) {
        EVP_PKEY *pubkey = X509_get0_pubkey(x509);
    }

    // Perform encryption using ARIA-256-CBC
    EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new();
    unsigned char key[32] = {0};
    unsigned char iv[16] = {0};
    if (size >= 48) {
        memcpy(key, data, 32);
        memcpy(iv, data + 32, 16);
    }

    EVP_EncryptInit_ex(cipher_ctx, cipher, NULL, key, iv);

    size_t encrypt_offset = 48;
    if (encrypt_offset >= size) {
        encrypt_offset = 0;
    }

    size_t encrypt_len = size - encrypt_offset;
    if (encrypt_len > 1024) {
        encrypt_len = 1024;
    }

    unsigned char *encrypted = (unsigned char *)malloc(encrypt_len + EVP_CIPHER_block_size(cipher));
    int out_len1 = 0, out_len2 = 0;

    if (encrypt_len > 0) {
        EVP_EncryptUpdate(cipher_ctx, encrypted, &out_len1, 
                         data + encrypt_offset, encrypt_len);
        EVP_EncryptFinal_ex(cipher_ctx, encrypted + out_len1, &out_len2);
    }

    // Write X509 certificate to output file
    FILE *out_file = fopen("output_file", "wb");
    if (x509 && out_file) {
        i2d_X509_fp(out_file, x509);
        assert_file_closed(&out_file);;
    }

    // Create and serialize PKCS7_ISSUER_AND_SERIAL
    PKCS7_ISSUER_AND_SERIAL *ias = PKCS7_ISSUER_AND_SERIAL_new();
    unsigned char *serialized = NULL;
    int serialized_len = i2d_PKCS7_ISSUER_AND_SERIAL(ias, &serialized);
    if (serialized) {
        OPENSSL_free(serialized);
    }

    // Cleanup
    PKCS7_ISSUER_AND_SERIAL_free(ias);
    EVP_CIPHER_CTX_free(cipher_ctx);
    free(encrypted);
    ASYNC_WAIT_CTX_free(async_ctx);
    EVP_PKEY_free(pkey);
    DSA_free(dsa);
    X509_free(x509);
    SSL_free(ssl);
    SSL_CTX_free(ssl_ctx);
    X509_STORE_free(store);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}