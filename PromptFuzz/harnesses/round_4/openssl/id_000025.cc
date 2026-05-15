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
//<ID> 347
//<Prompt> []
/*<Combination>: [
*/
//<score> 140, nr_unique_branch: 86
//<Quality> {"density":7,"unique_branches":{"ossl_ffc_params_copy":[[182,9,182,37,0,0,4,1],[183,12,183,40,0,0,4,1],[184,12,184,40,0,0,4,1],[185,12,185,40,0,0,4,1],[192,9,192,26,0,0,4,0],[194,13,194,30,0,0,4,1]],"ossl_ffc_params_FIPS186_2_generate":[[1065,9,1067,33,0,0,4,0]],"ffc_bn_cpy":[[168,9,168,20,0,0,4,0],[168,9,168,20,0,0,4,1],[170,14,170,51,0,0,4,1],[173,14,173,39,0,0,4,1]],"ossl_ffc_params_validate_unverifiable_g":[[33,9,33,39,0,0,4,1],[33,43,33,60,0,0,4,1],[43,9,43,50,0,0,4,1],[45,9,45,41,0,0,4,1]],"DSA_generate_key":[[36,9,36,38,0,0,4,1]],"ossl_ffc_params_FIPS186_4_validate":[[58,9,58,23,0,0,4,1],[58,27,58,44,0,0,4,0]],"generate_q_fips186_2":[[413,13,413,26,0,0,4,1],[420,34,420,40,0,0,4,1]],"ossl_dsa_generate_ffc_parameters":[[41,9,41,16,0,0,4,1]],"ossl_ffc_params_FIPS186_2_gen_verify":[[838,26,838,32,0,0,4,0],[883,9,883,24,0,0,4,0],[884,13,884,29,0,0,4,0],[884,13,884,29,0,0,4,1],[889,13,889,29,0,0,4,1],[912,9,912,16,0,0,4,1],[919,13,919,54,0,0,4,1],[926,13,926,53,0,0,4,0],[928,17,928,34,0,0,4,1],[935,9,935,26,0,0,4,0],[935,30,935,73,0,0,4,0],[998,9,998,16,0,0,4,1],[1008,16,1008,58,0,0,4,0],[1009,12,1011,17,0,0,4,1],[1018,9,1018,16,0,0,4,1],[1038,9,1038,49,0,0,4,0],[1043,9,1043,20,0,0,4,1]],"dsa_keygen":[[153,9,153,51,0,0,4,1],[156,9,156,30,0,0,4,0],[157,13,157,49,0,0,4,1],[164,9,165,38,0,0,4,0],[164,9,165,38,0,0,4,1],[173,9,175,36,0,0,4,1],[178,9,178,29,0,0,4,0],[179,13,179,41,0,0,4,1],[185,9,185,67,0,0,4,1],[214,9,214,32,0,0,4,1],[216,9,216,34,0,0,4,0],[216,9,216,34,0,0,4,1]],"ossl_ffc_generate_private_key":[[29,9,29,15,0,0,4,1],[31,9,31,15,0,0,4,1],[35,9,35,18,0,0,4,1],[35,22,35,31,0,0,4,1],[40,9,40,25,0,0,4,1],[40,29,40,68,0,0,4,1],[44,9,44,42,0,0,4,0],[48,13,48,59,0,0,4,1],[49,16,49,37,0,0,4,1],[52,13,52,32,0,0,4,0],[52,13,52,32,0,0,4,1]],"validate_hostname_part":[[247,9,247,16,0,0,4,0]],"ASN1_i2d_bio":[[41,9,41,15,0,0,4,1],[45,9,45,18,0,0,4,1],[53,13,53,19,0,0,4,0],[53,13,53,19,0,0,4,1],[55,13,55,19,0,0,4,0]],"ossl_ffc_params_set_seed":[[85,9,85,29,0,0,4,0],[86,13,86,33,0,0,4,1]],"ossl_dsa_generate_public_key":[[48,9,48,20,0,0,4,1],[53,9,53,69,0,0,4,1]],"ossl_ffc_params_FIPS186_2_validate":[[76,9,76,23,0,0,4,1],[76,27,76,44,0,0,4,1],[76,48,76,65,0,0,4,1]],"DSA_generate_parameters_ex":[[55,9,55,24,0,0,4,0],[56,12,57,16,0,0,4,1],[62,13,63,31,0,0,4,0]],"ossl_ffc_params_simple_validate":[[102,9,102,23,0,0,4,1],[105,9,105,20,0,0,4,0],[108,9,108,50,0,0,4,1],[115,9,115,55,0,0,4,0],[115,9,115,55,0,0,4,1],[123,9,123,43,0,0,4,0],[123,9,123,43,0,0,4,1],[124,12,124,58,0,0,4,1]],"file_write":[[167,13,167,16,0,0,4,1]]},"library_calls":["OPENSSL_init_ssl","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_add1_host","DSA_new","DSA_generate_parameters_ex","DSA_generate_key","i2d_DSAPrivateKey_fp","i2d_DSAPrivateKey_fp","EVP_CIPHER_CTX_new","EVP_CIPHER_CTX_set_cipher_data","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","i2d_RSAPublicKey","CRYPTO_free","BN_free","SSL_CTX_new","TLS_method","SSL_new","SSL_get0_peer_CA_list","DSA_free","RSA_free","EVP_CIPHER_CTX_free","X509_VERIFY_PARAM_free","SSL_free","SSL_CTX_free"],"critical_calls":["OPENSSL_init_ssl","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_add1_host","DSA_new","DSA_generate_parameters_ex","DSA_generate_key","i2d_DSAPrivateKey_fp","i2d_DSAPrivateKey_fp","EVP_CIPHER_CTX_new","EVP_CIPHER_CTX_set_cipher_data","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","i2d_RSAPublicKey","CRYPTO_free","BN_free","SSL_CTX_new","TLS_method","SSL_new","SSL_get0_peer_CA_list","DSA_free","RSA_free","EVP_CIPHER_CTX_free","X509_VERIFY_PARAM_free","SSL_free","SSL_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL if needed
    OPENSSL_init_ssl(0, NULL);
    
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
    
    // 1. Use X509_VERIFY_PARAM_add1_host
    X509_VERIFY_PARAM *param = X509_VERIFY_PARAM_new();
    if (param && size > 0) {
        // Use first part of data as hostname (ensure null-terminated)
        size_t host_len = size > 100 ? 100 : size;
        char *hostname = (char *)malloc(host_len + 1);
        if (hostname) {
            memcpy(hostname, data, host_len);
            hostname[host_len] = '\0';
            X509_VERIFY_PARAM_add1_host(param, hostname, host_len);
            free(hostname);
        }
    }
    
    // 2. Use i2d_DSAPrivateKey_fp with generated DSA key
    DSA *dsa = DSA_new();
    if (dsa) {
        // Generate DSA parameters (using fuzzed data for seed)
        DSA_generate_parameters_ex(dsa, 512, (const unsigned char *)data, 
                                  size > 20 ? 20 : size, NULL, NULL, NULL);
        
        // Generate DSA key
        DSA_generate_key(dsa);
        
        // Write DSA private key to file
        i2d_DSAPrivateKey_fp(out_file, dsa);
        
        // Also try to write to input file (for API usage)
        rewind(in_file);
        i2d_DSAPrivateKey_fp(in_file, dsa);
    }
    
    // 3. Use EVP_CIPHER_CTX_set_cipher_data
    EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new();
    if (cipher_ctx) {
        // Allocate cipher data from input
        void *cipher_data = malloc(size);
        if (cipher_data) {
            memcpy(cipher_data, data, size);
            EVP_CIPHER_CTX_set_cipher_data(cipher_ctx, cipher_data);
            free(cipher_data);
        }
    }
    
    // 4. Use i2d_RSAPublicKey with generated RSA key
    RSA *rsa = RSA_new();
    if (rsa) {
        // Generate RSA key (using fuzzed data as seed if needed)
        BIGNUM *e = BN_new();
        BN_set_word(e, 65537);
        RSA_generate_key_ex(rsa, 512, e, NULL);
        
        // Encode RSA public key
        unsigned char *rsa_out = NULL;
        int rsa_len = i2d_RSAPublicKey(rsa, &rsa_out);
        if (rsa_out) {
            // Write to output file
            fwrite(rsa_out, 1, rsa_len, out_file);
            OPENSSL_free(rsa_out);
        }
        
        BN_free(e);
    }
    
    // 5. Use SSL_get0_peer_CA_list with SSL context
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = NULL;
    if (ssl_ctx) {
        ssl = SSL_new(ssl_ctx);
        if (ssl) {
            const struct stack_st_X509_NAME *ca_list = SSL_get0_peer_CA_list(ssl);
            // ca_list may be NULL (no peer CA list), but API is called
        }
    }
    
    // Clean up
    if (dsa) DSA_free(dsa);
    if (rsa) RSA_free(rsa);
    if (cipher_ctx) EVP_CIPHER_CTX_free(cipher_ctx);
    if (param) X509_VERIFY_PARAM_free(param);
    if (ssl) SSL_free(ssl);
    if (ssl_ctx) SSL_CTX_free(ssl_ctx);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}