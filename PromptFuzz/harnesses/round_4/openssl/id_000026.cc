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
//<ID> 348
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 13
//<Quality> {"density":7,"unique_branches":{"X509_VERIFY_PARAM_add1_host":[[676,13,676,76,0,0,4,1]],"buffer_from_string":[[59,9,59,52,0,0,4,1],[62,9,62,75,0,0,4,1],[65,9,65,39,0,0,4,1]],"add_string_to_buffer_stack":[[158,9,158,60,0,0,4,1],[162,9,162,26,0,0,4,0],[162,30,162,82,0,0,4,1],[165,9,165,46,0,0,4,1]],"validate_hostname_part":[[252,17,252,24,0,0,4,1],[280,9,280,20,0,0,4,1],[280,24,280,35,0,0,4,1]],"buffer_free":[[88,9,88,18,0,0,4,0],[88,9,88,18,0,0,4,1]]},"library_calls":["X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_add1_host","DSA_new","i2d_DSAPrivateKey_fp","EVP_CIPHER_CTX_new","EVP_CIPHER_CTX_set_cipher_data","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","BN_free","i2d_RSAPublicKey","SSL_CTX_new","TLS_method","SSL_new","SSL_get0_peer_CA_list","CRYPTO_free","SSL_free","SSL_CTX_free","EVP_CIPHER_CTX_free","DSA_free","RSA_free","X509_VERIFY_PARAM_free"],"critical_calls":["X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_add1_host","DSA_new","i2d_DSAPrivateKey_fp","EVP_CIPHER_CTX_new","EVP_CIPHER_CTX_set_cipher_data","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","BN_free","i2d_RSAPublicKey","SSL_CTX_new","TLS_method","SSL_new","SSL_get0_peer_CA_list","CRYPTO_free","SSL_free","SSL_CTX_free","EVP_CIPHER_CTX_free","DSA_free","RSA_free","X509_VERIFY_PARAM_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize variables
    X509_VERIFY_PARAM *param = NULL;
    DSA *dsa_key = NULL;
    RSA *rsa_key = NULL;
    EVP_CIPHER_CTX *cipher_ctx = NULL;
    SSL *ssl = NULL;
    SSL_CTX *ssl_ctx = NULL;
    unsigned char *rsa_output = NULL;
    
    // 1. Create and use X509_VERIFY_PARAM with host verification
    param = X509_VERIFY_PARAM_new();
    if (param && size > 10) {
        // Extract hostname from input data (first 10 bytes as example)
        char hostname[11];
        size_t hostname_len = size > 10 ? 10 : size;
        memcpy(hostname, data, hostname_len);
        hostname[hostname_len] = '\0';
        X509_VERIFY_PARAM_add1_host(param, hostname, hostname_len);
    }
    
    // 2. Read and write DSA private key using the input file
    if (size > 0) {
        // Create a minimal DSA key for testing
        dsa_key = DSA_new();
        if (dsa_key) {
            // Try to write DSA key to output file (even if empty/partial)
            i2d_DSAPrivateKey_fp(out_file, dsa_key);
        }
    }
    
    // 3. Create and use EVP_CIPHER_CTX with cipher data
    cipher_ctx = EVP_CIPHER_CTX_new();
    if (cipher_ctx && size > 0) {
        // Use part of input data as cipher data
        void *cipher_data = (void*)data;
        EVP_CIPHER_CTX_set_cipher_data(cipher_ctx, cipher_data);
    }
    
    // 4. Create RSA key and encode to DER format
    rsa_key = RSA_new();
    if (rsa_key) {
        // Generate a small RSA key for testing
        BIGNUM *bn = BN_new();
        if (bn) {
            BN_set_word(bn, RSA_F4);
            RSA_generate_key_ex(rsa_key, 512, bn, NULL);
            BN_free(bn);
        }
        
        // Encode RSA public key to DER
        int rsa_len = i2d_RSAPublicKey(rsa_key, &rsa_output);
        if (rsa_output) {
            // Write RSA output to file
            fwrite(rsa_output, 1, rsa_len, out_file);
        }
    }
    
    // 5. Create SSL context and get peer CA list
    ssl_ctx = SSL_CTX_new(TLS_method());
    if (ssl_ctx) {
        ssl = SSL_new(ssl_ctx);
        if (ssl) {
            const struct stack_st_X509_NAME *ca_list = SSL_get0_peer_CA_list(ssl);
            // ca_list may be NULL, but API is called
            (void)ca_list; // Use variable to avoid unused warning
        }
    }
    
    // Clean up allocated resources
    if (rsa_output) OPENSSL_free(rsa_output);
    if (ssl) SSL_free(ssl);
    if (ssl_ctx) SSL_CTX_free(ssl_ctx);
    if (cipher_ctx) EVP_CIPHER_CTX_free(cipher_ctx);
    if (dsa_key) DSA_free(dsa_key);
    if (rsa_key) RSA_free(rsa_key);
    if (param) X509_VERIFY_PARAM_free(param);
    
    // Close files
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}