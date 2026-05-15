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
//<ID> 130
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 7
//<Quality> {"density":10,"unique_branches":{"CMS_stream":[[25,9,25,20,0,0,4,1],[27,9,27,21,0,0,4,0],[29,9,29,21,0,0,4,0]],"OPENSSL_LH_new":[[71,18,71,29,0,0,4,0],[72,18,72,29,0,0,4,0]],"CMS_sign_ex":[[545,9,545,32,0,0,4,1]],"ossl_asn1_string_embed_free":[[366,9,366,44,0,0,4,1]]},"library_calls":["OPENSSL_init_crypto","PKCS12_init","EVP_MAC_fetch","EVP_MAC_CTX_new","EVP_MAC_free","EVP_ASYM_CIPHER_fetch","EVP_ASYM_CIPHER_settable_ctx_params","EVP_ASYM_CIPHER_free","PBE2PARAM_new","i2d_PBE2PARAM","PBE2PARAM_free","CMS_sign","CMS_stream","CMS_ContentInfo_free","X509_new","X509V3_set_ctx","OPENSSL_LH_set_thunks","OPENSSL_LH_new","ossl_check_CONF_VALUE_lh_hashfunc_type","ossl_check_CONF_VALUE_lh_compfunc_type","NCONF_new","NCONF_load","X509V3_EXT_add_conf","NCONF_free","OPENSSL_LH_free","ossl_check_CONF_VALUE_lh_type","X509_free","d2i_PKCS12_fp","PKCS12_free","OSSL_PARAM_construct_utf8_string","OSSL_PARAM_construct_end","EVP_MAC_init","EVP_MAC_update","EVP_MAC_final","EVP_MAC_CTX_free","PKCS12_free","ERR_clear_error"],"critical_calls":["OPENSSL_init_crypto","PKCS12_init","EVP_MAC_fetch","EVP_MAC_CTX_new","EVP_MAC_free","EVP_ASYM_CIPHER_fetch","EVP_ASYM_CIPHER_settable_ctx_params","EVP_ASYM_CIPHER_free","PBE2PARAM_new","i2d_PBE2PARAM","PBE2PARAM_free","CMS_sign","CMS_stream","CMS_ContentInfo_free","X509_new","X509V3_set_ctx","OPENSSL_LH_set_thunks","OPENSSL_LH_new","ossl_check_CONF_VALUE_lh_compfunc_type","ossl_check_CONF_VALUE_lh_hashfunc_type","NCONF_new","NCONF_load","X509V3_EXT_add_conf","NCONF_free","OPENSSL_LH_free","ossl_check_CONF_VALUE_lh_type","X509_free","d2i_PKCS12_fp","PKCS12_free","OSSL_PARAM_construct_utf8_string","OSSL_PARAM_construct_end","EVP_MAC_init","EVP_MAC_update","EVP_MAC_final","EVP_MAC_CTX_free","PKCS12_free","ERR_clear_error"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified OpenSSL APIs to process a byte stream:

*/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL if not already initialized
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG, NULL);
    
    // Create a memory file from input data
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
    
    // 1. Initialize PKCS12 structure
    PKCS12 *p12 = PKCS12_init(NID_pkcs7_data);
    if (!p12) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Create MAC context
    EVP_MAC *mac = EVP_MAC_fetch(NULL, "HMAC", NULL);
    EVP_MAC_CTX *mac_ctx = NULL;
    if (mac) {
        mac_ctx = EVP_MAC_CTX_new(mac);
        EVP_MAC_free(mac);
    }
    
    // 3. Get asymmetric cipher settable parameters
    EVP_ASYM_CIPHER *asym_cipher = EVP_ASYM_CIPHER_fetch(NULL, "RSA", NULL);
    const OSSL_PARAM *settables = NULL;
    if (asym_cipher) {
        settables = EVP_ASYM_CIPHER_settable_ctx_params(asym_cipher);
        EVP_ASYM_CIPHER_free(asym_cipher);
    }
    
    // 4. Create and encode PBE2PARAM structure using input data
    PBE2PARAM *pbe2 = PBE2PARAM_new();
    if (pbe2) {
        // Try to read some data from input for PBE2 parameters
        if (size > 0) {
            // Use a portion of input data for encoding
            size_t encode_size = size > 100 ? 100 : size;
            unsigned char *encode_buf = (unsigned char *)malloc(encode_size);
            if (encode_buf) {
                fread(encode_buf, 1, encode_size, in_file);
                rewind(in_file);
                
                unsigned char *out_ptr = encode_buf;
                i2d_PBE2PARAM(pbe2, &out_ptr);
                free(encode_buf);
            }
        }
        PBE2PARAM_free(pbe2);
    }
    
    // 5. Create CMS structure and set up streaming
    CMS_ContentInfo *cms = CMS_sign(NULL, NULL, NULL, NULL, CMS_STREAM | CMS_DETACHED);
    if (cms) {
        unsigned char **boundary = NULL;
        CMS_stream(&boundary, cms);
        
        // Write some data to CMS
        if (size > 0) {
            unsigned char *cms_data = (unsigned char *)malloc(size);
            if (cms_data) {
                fread(cms_data, 1, size, in_file);
                rewind(in_file);
                // In a real scenario, we would feed this to CMS_EncryptedData_encrypt
                // or similar function
                free(cms_data);
            }
        }
        CMS_ContentInfo_free(cms);
    }
    
    // 6. Add X.509 extension configuration
    // Create a dummy certificate for testing
    X509 *cert = X509_new();
    if (cert) {
        // Set up X509V3 context
        X509V3_CTX ctx;
        X509V3_set_ctx(&ctx, cert, cert, NULL, NULL, 0);
        
        // Create a configuration hash table
        struct lhash_st_CONF_VALUE *conf = lh_CONF_VALUE_new(NULL, NULL);
        if (conf) {
            // Try to parse input as configuration
            if (size > 0) {
                // Write input to a temporary config file
                FILE *config_file = fopen("temp_config.cnf", "wb");
                if (config_file) {
                    fwrite(data, 1, size, config_file);
                    assert_file_closed(&config_file);;
                    
                    // Load configuration
                    CONF *cnf = NCONF_new(NULL);
                    if (cnf) {
                        if (NCONF_load(cnf, "temp_config.cnf", NULL)) {
                            // Convert to old-style CONF structure
                            // This is a simplification - actual conversion would be more complex
                            X509V3_EXT_add_conf(conf, &ctx, "default", cert);
                        }
                        NCONF_free(cnf);
                    }
                    remove("temp_config.cnf");
                }
            }
            lh_CONF_VALUE_free(conf);
        }
        X509_free(cert);
    }
    
    // Try to process the input file with PKCS12 if there's enough data
    if (size > 50) {
        rewind(in_file);
        PKCS12 *p12_in = d2i_PKCS12_fp(in_file, NULL);
        if (p12_in) {
            // Process the PKCS12 structure
            // In a real scenario, we would parse and extract data
            PKCS12_free(p12_in);
        }
    }
    
    // Write some output using the MAC context if available
    if (mac_ctx && size > 0) {
        unsigned char key[32] = {0};
        unsigned char in[64] = {0};
        unsigned char out[64];
        size_t outlen = sizeof(out);
        
        // Use a portion of input data
        size_t use_size = size > 64 ? 64 : size;
        fread(in, 1, use_size, in_file);
        
        // Initialize MAC with dummy key
        OSSL_PARAM params[2];
        params[0] = OSSL_PARAM_construct_utf8_string("digest", (char *)"SHA256", 0);
        params[1] = OSSL_PARAM_construct_end();
        
        EVP_MAC_init(mac_ctx, key, sizeof(key), params);
        EVP_MAC_update(mac_ctx, in, use_size);
        EVP_MAC_final(mac_ctx, out, &outlen, sizeof(out));
        
        // Write MAC output to file
        fwrite(out, 1, outlen, out_file);
    }
    
    // Clean up
    if (mac_ctx) {
        EVP_MAC_CTX_free(mac_ctx);
    }
    
    PKCS12_free(p12);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean error queue
    ERR_clear_error();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}