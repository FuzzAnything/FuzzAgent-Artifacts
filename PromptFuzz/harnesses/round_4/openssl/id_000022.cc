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
//<ID> 245
//<Prompt> []
/*<Combination>: [
*/
//<score> 234, nr_unique_branch: 80
//<Quality> {"density":13,"unique_branches":{"SSL_CONF_CTX_set_ssl_ctx":[[1194,9,1194,12,0,0,4,1]],"BN_mul_word":[[186,9,186,15,0,0,4,0],[187,13,187,19,0,0,4,1],[191,17,191,19,0,0,4,1]],"SSL_CONF_CTX_free":[[1116,9,1116,13,0,0,4,0]],"ssl_conf_cmd_skip_prefix":[[895,9,895,21,0,0,4,1],[895,25,895,38,0,0,4,1],[898,9,898,21,0,0,4,1],[906,16,906,51,0,0,4,0],[907,13,907,26,0,0,4,0]],"free_cert_filename":[[1107,17,1107,44,0,0,4,1]],"SSL_CONF_cmd":[[973,9,973,20,0,0,4,1],[978,9,978,46,0,0,4,0],[1006,9,1006,48,0,0,4,1]],"BN_add_word":[[119,27,119,37,0,0,4,1],[121,13,121,20,0,0,4,0],[123,9,123,10,0,0,4,0],[123,14,123,25,0,0,4,0],[124,13,124,46,0,0,4,1]],"EVP_RAND_CTX_new":[[345,9,345,21,0,0,4,0]],"BN_mod_inverse":[[531,9,531,14,0,0,4,0]],"int_bn_mod_inverse":[[206,33,206,46,0,0,4,0],[260,9,260,21,0,0,4,1],[357,16,357,30,0,0,4,0],[357,16,357,30,0,0,4,1],[367,17,367,49,0,0,4,0],[367,17,367,49,0,0,4,1],[368,21,368,28,0,0,4,1],[370,21,370,37,0,0,4,1],[372,24,372,60,0,0,4,0],[372,24,372,60,0,0,4,1],[374,21,374,38,0,0,4,1],[376,21,376,38,0,0,4,0],[376,21,376,38,0,0,4,1],[378,25,378,32,0,0,4,1],[380,25,380,41,0,0,4,1],[384,25,384,41,0,0,4,1],[386,25,386,41,0,0,4,1],[388,25,388,42,0,0,4,0],[388,25,388,42,0,0,4,1],[390,29,390,47,0,0,4,1],[397,29,397,47,0,0,4,1],[402,29,402,45,0,0,4,1],[407,21,407,45,0,0,4,1],[448,17,448,29,0,0,4,0],[448,17,448,29,0,0,4,1],[449,21,449,39,0,0,4,1],[452,21,452,37,0,0,4,0],[452,21,452,37,0,0,4,1],[453,25,453,44,0,0,4,1],[455,28,455,44,0,0,4,0],[455,28,455,44,0,0,4,1],[456,25,456,46,0,0,4,1],[458,28,458,39,0,0,4,0],[459,25,459,48,0,0,4,1],[461,25,461,51,0,0,4,1],[467,21,467,41,0,0,4,1],[486,9,486,17,0,0,4,1],[492,9,492,21,0,0,4,1],[507,9,507,22,0,0,4,0],[507,26,507,38,0,0,4,1]],"BN_MONT_CTX_set":[[266,9,266,24,0,0,4,0],[345,20,345,31,0,0,4,1],[349,18,349,61,0,0,4,0]],"SSL_CONF_cmd_argv":[[1017,9,1017,22,0,0,4,0],[1017,26,1017,37,0,0,4,1],[1019,9,1019,22,0,0,4,1],[1019,26,1019,36,0,0,4,0],[1021,9,1021,20,0,0,4,1],[1023,9,1023,22,0,0,4,1],[1023,26,1023,36,0,0,4,0],[1030,9,1030,15,0,0,4,1],[1038,9,1038,17,0,0,4,0]],"BN_sub":[[60,20,60,31,0,0,4,1]],"SSL_CONF_CTX_set_flags":[[1126,9,1126,46,0,0,4,1],[1130,9,1130,43,0,0,4,1]],"EC_GROUP_new_curve_GFp":[[61,9,61,47,0,0,4,0]],"ossl_ec_GFp_mont_group_set_curve":[[165,9,165,39,0,0,4,0],[182,9,182,13,0,0,4,0]],"ossl_ec_GFp_simple_group_set_curve":[[150,9,150,28,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","OPENSSL_init_crypto","BN_CTX_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_bin2bn","EC_GROUP_new_curve_GFp","EC_GROUP_free","EVP_RAND_CTX_new","EVP_RAND_nonce","EVP_RAND_CTX_free","EVP_aes_128_ocb","EVP_CIPHER_CTX_new","EVP_CIPHER_CTX_free","SSL_CONF_CTX_new","SSL_CONF_CTX_set_flags","SSL_CONF_CTX_set_ssl_ctx","SSL_CONF_cmd_argv","SSL_CONF_CTX_free","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OSSL_CRMF_MSG_new","OPENSSL_sk_push","ossl_check_OSSL_CRMF_MSG_sk_type","ossl_check_OSSL_CRMF_MSG_type","i2d_OSSL_CRMF_MSGS","CRYPTO_free","OPENSSL_sk_pop_free","ossl_check_OSSL_CRMF_MSG_sk_type","ossl_check_OSSL_CRMF_MSG_freefunc_type","ossl_check_OSSL_CRMF_ATTRIBUTETYPEANDVALUE_copyfunc_type","BN_free","BN_free","BN_free","BN_CTX_free"],"critical_calls":["OPENSSL_init_ssl","OPENSSL_init_crypto","BN_CTX_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_bin2bn","EC_GROUP_new_curve_GFp","EC_GROUP_free","EVP_RAND_CTX_new","EVP_RAND_nonce","EVP_RAND_CTX_free","EVP_aes_128_ocb","EVP_CIPHER_CTX_new","EVP_CIPHER_CTX_free","SSL_CONF_CTX_new","SSL_CONF_CTX_set_flags","SSL_CONF_CTX_set_ssl_ctx","SSL_CONF_cmd_argv","SSL_CONF_CTX_free","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OSSL_CRMF_MSG_new","OPENSSL_sk_push","ossl_check_OSSL_CRMF_MSG_type","ossl_check_OSSL_CRMF_MSG_sk_type","i2d_OSSL_CRMF_MSGS","CRYPTO_free","OPENSSL_sk_pop_free","ossl_check_OSSL_CRMF_MSG_freefunc_type","ossl_check_OSSL_CRMF_MSG_sk_type","ossl_check_OSSL_CRMF_ATTRIBUTETYPEANDVALUE_copyfunc_type","BN_free","BN_free","BN_free","BN_CTX_free"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified OpenSSL APIs to achieve certificate management and encryption operations:

*/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 32) {
	return 0;
	}
	  // Need minimum data for operations

    // Initialize OpenSSL
    OPENSSL_init_ssl(0, NULL);
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG, NULL);

    // 1. Create and use EC group with curve parameters from input
    BN_CTX *bn_ctx = BN_CTX_new();
    BIGNUM *p = BN_new();
    BIGNUM *a = BN_new();
    BIGNUM *b = BN_new();
    
    // Use first 32 bytes for curve parameters (simplified)
    if (size >= 96) {
        BN_bin2bn(data, 32, p);
        BN_bin2bn(data + 32, 32, a);
        BN_bin2bn(data + 64, 32, b);
        
        EC_GROUP *ec_group = EC_GROUP_new_curve_GFp(p, a, b, bn_ctx);
        if (ec_group) {
            EC_GROUP_free(ec_group);
        }
    }

    // 2. Generate nonce using EVP_RAND
    EVP_RAND_CTX *rand_ctx = EVP_RAND_CTX_new(NULL, NULL);
    if (rand_ctx) {
        unsigned char nonce[16];
        size_t nonce_len = size > 16 ? 16 : size;
        if (EVP_RAND_nonce(rand_ctx, nonce, nonce_len) > 0) {
            // Nonce generated successfully
        }
        EVP_RAND_CTX_free(rand_ctx);
    }

    // 3. Get AES-128-OCB cipher
    const EVP_CIPHER *cipher = EVP_aes_128_ocb();
    if (cipher) {
        EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new();
        if (cipher_ctx) {
            // Initialize cipher context (simplified - would need proper key/IV)
            unsigned char key[16] = {0};
            unsigned char iv[12] = {0};
            if (size >= 28) {
                memcpy(key, data, 16);
                memcpy(iv, data + 16, 12);
            }
            
            EVP_CIPHER_CTX_free(cipher_ctx);
        }
    }

    // 4. Process configuration commands
    SSL_CONF_CTX *cctx = SSL_CONF_CTX_new();
    if (cctx) {
        SSL_CONF_CTX_set_flags(cctx, SSL_CONF_FLAG_FILE);
        SSL_CONF_CTX_set_ssl_ctx(cctx, NULL);
        
        // Create argv from input data
        char **argv = (char **)malloc(3 * sizeof(char *));
        if (argv) {
            argv[0] = (char *)"fuzz";
            argv[1] = (char *)malloc(32);
            argv[2] = NULL;
            
            if (argv[1] && size >= 32) {
                memcpy(argv[1], data, 31);
                argv[1][31] = '\0';
                
                int argc = 2;
                char **pargv = argv;
                SSL_CONF_cmd_argv(cctx, &argc, &pargv);
                
                free(argv[1]);
            }
            free(argv);
        }
        SSL_CONF_CTX_free(cctx);
    }

    // 5. Create and serialize CRMF messages
    OSSL_CRMF_MSGS *msgs = sk_OSSL_CRMF_MSG_new_null();
    if (msgs) {
        // Add some dummy CRMF messages (simplified)
        OSSL_CRMF_MSG *msg = OSSL_CRMF_MSG_new();
        if (msg) {
            sk_OSSL_CRMF_MSG_push(msgs, msg);
        }
        
        // Serialize to DER
        unsigned char *der = NULL;
        int der_len = i2d_OSSL_CRMF_MSGS(msgs, &der);
        if (der) {
            OPENSSL_free(der);
        }
        
        sk_OSSL_CRMF_MSG_pop_free(msgs, OSSL_CRMF_MSG_free);
    }

    // 6. Use the copy function type checker
    sk_OSSL_CRMF_ATTRIBUTETYPEANDVALUE_copyfunc copy_func = 
        (sk_OSSL_CRMF_ATTRIBUTETYPEANDVALUE_copyfunc)sk_deep_copy;
    OPENSSL_sk_copyfunc checked_copy = 
        ossl_check_OSSL_CRMF_ATTRIBUTETYPEANDVALUE_copyfunc_type(copy_func);

    // Clean up
    BN_free(p);
    BN_free(a);
    BN_free(b);
    BN_CTX_free(bn_ctx);

    return 0;
}