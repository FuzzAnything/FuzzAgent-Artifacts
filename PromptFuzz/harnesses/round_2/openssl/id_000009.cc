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
//<ID> 99
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 96
//<Quality> {"density":10,"unique_branches":{"aria_gcm_newctx":[[20,9,20,32,0,0,4,1],[24,9,24,20,0,0,4,0]],"X509V3_parse_list":[[376,9,376,27,0,0,4,1],[387,13,387,18,0,0,4,1],[391,13,391,51,0,0,4,1]],"x509v3_add_len_value":[[49,9,49,22,0,0,4,1]],"ossl_gcm_set_ctx_params":[[257,9,257,20,0,0,4,1],[257,24,257,75,0,0,4,1],[260,9,260,22,0,0,4,1],[273,9,273,24,0,0,4,1],[290,9,290,22,0,0,4,1],[303,9,303,24,0,0,4,1],[314,9,314,24,0,0,4,1]],"gcm_init":[[57,9,57,32,0,0,4,1],[62,9,62,19,0,0,4,0],[63,13,63,23,0,0,4,1],[63,27,63,50,0,0,4,1],[73,9,73,20,0,0,4,0],[74,13,74,34,0,0,4,1],[78,13,78,52,0,0,4,1]],"ossl_cipher_gcm_set_ctx_params_decoder":[[216,9,216,18,0,0,4,1]],"ASN1_BIT_STRING_set_bit":[[148,9,148,14,0,0,4,1],[154,9,154,15,0,0,4,1],[157,9,157,18,0,0,4,1],[162,9,162,30,0,0,4,0],[163,13,163,19,0,0,4,1],[166,13,166,22,0,0,4,1],[168,13,168,34,0,0,4,0],[175,12,175,27,0,0,4,0],[175,31,175,60,0,0,4,1],[178,9,178,22,0,0,4,0],[184,13,184,29,0,0,4,1],[186,13,186,29,0,0,4,1],[188,13,188,29,0,0,4,1],[191,13,191,61,0,0,4,1]],"v3_check_critical":[[207,9,207,31,0,0,4,0],[58,30,58,74,3,0,4,1]],"ossl_aria_encrypt":[[427,9,427,19,0,0,4,1],[427,23,427,34,0,0,4,1],[427,38,427,49,0,0,4,1],[434,9,434,17,0,0,4,1],[450,12,450,25,0,0,4,0],[450,12,450,25,0,0,4,1]],"ossl_cipher_gcm_get_ctx_params_decoder":[[49,9,49,18,0,0,4,0],[50,16,50,36,0,0,4,0],[50,16,50,36,0,0,4,1],[52,13,52,20,0,0,4,1],[54,13,54,21,0,0,4,0],[54,13,54,21,0,0,4,1],[56,17,56,24,0,0,4,1],[58,17,58,25,0,0,4,0],[60,21,60,28,0,0,4,1],[62,21,62,29,0,0,4,1],[73,21,73,29,0,0,4,0],[84,21,84,30,0,0,4,1],[94,13,94,21,0,0,4,0],[94,13,94,21,0,0,4,1],[105,13,105,21,0,0,4,1],[171,13,171,21,0,0,4,1],[22,24,22,50,6,0,4,0],[23,26,23,52,7,0,4,1],[22,24,22,50,15,0,4,0],[23,26,23,52,16,0,4,1]],"gcm_get_funcs":[[446,9,446,40,0,0,4,0],[447,13,447,58,0,0,4,0]],"X509V3_add_value":[[84,9,84,22,0,0,4,1]],"ossl_gcm_get_ctx_params":[[159,9,159,20,0,0,4,1],[159,24,159,75,0,0,4,1],[162,9,162,24,0,0,4,0],[162,9,162,24,0,0,4,1],[162,28,162,71,0,0,4,1],[167,9,167,25,0,0,4,0],[167,9,167,25,0,0,4,1],[167,29,167,74,0,0,4,1],[172,9,172,25,0,0,4,1],[185,9,185,21,0,0,4,1],[197,9,197,24,0,0,4,1],[210,9,210,22,0,0,4,1],[215,9,215,22,0,0,4,1],[232,9,232,24,0,0,4,1],[238,9,238,22,0,0,4,1]],"asn1_bit_string_set_unused_bits":[[27,9,27,24,0,0,4,1]],"RC2_set_key":[[301,9,301,18,0,0,4,0]],"v2i_ASN1_BIT_STRING":[[75,9,75,45,0,0,4,1],[79,17,79,38,0,0,4,0],[79,17,79,38,0,0,4,1],[81,39,81,50,0,0,4,0],[82,17,82,52,0,0,4,0],[84,21,84,66,0,0,4,1],[92,13,92,25,0,0,4,1]],"ossl_aria_set_encrypt_key":[[481,9,481,24,0,0,4,1],[481,28,481,39,0,0,4,1],[484,9,484,20,0,0,4,1],[504,9,504,19,0,0,4,1],[584,9,584,19,0,0,4,1],[590,9,590,19,0,0,4,1]]},"library_calls":["PKCS12_BAGS_it","PKCS12_AUTHSAFES_it","d2i_PKCS12_fp","OSSL_ENCODER_fetch","OSSL_PARAM_construct_end","OSSL_ENCODER_get_params","OSSL_ENCODER_free","X509V3_EXT_conf_nid","X509_EXTENSION_get_data","X509_EXTENSION_free","PKCS12_free","EVP_aria_128_gcm","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","EVP_CIPHER_CTX_free","BN_CTX_new","BN_bin2bn","BN_MONT_CTX_new","BN_MONT_CTX_new","BN_MONT_CTX_set","BN_MONT_CTX_copy","BN_MONT_CTX_free","BN_MONT_CTX_free","BN_free","BN_CTX_free","RC2_set_key","RC2_encrypt"],"critical_calls":["PKCS12_BAGS_it","PKCS12_AUTHSAFES_it","d2i_PKCS12_fp","OSSL_ENCODER_fetch","OSSL_PARAM_construct_end","OSSL_ENCODER_get_params","OSSL_ENCODER_free","X509V3_EXT_conf_nid","X509_EXTENSION_get_data","X509_EXTENSION_free","PKCS12_free","EVP_aria_128_gcm","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","EVP_CIPHER_CTX_free","BN_CTX_new","BN_bin2bn","BN_MONT_CTX_new","BN_MONT_CTX_new","BN_MONT_CTX_set","BN_MONT_CTX_copy","BN_MONT_CTX_free","BN_MONT_CTX_free","BN_free","BN_CTX_free","RC2_set_key","RC2_encrypt"],"visited":3}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get ASN1_ITEM structures for PKCS12 components
    const ASN1_ITEM *pkcs12_bags_item = PKCS12_BAGS_it();
    const ASN1_ITEM *pkcs12_authsafes_item = PKCS12_AUTHSAFES_it();
    
    // 2. Create a FILE* from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Read and parse PKCS12 structure from input
    PKCS12 *p12 = d2i_PKCS12_fp(in_file, NULL);
    assert_file_closed(&in_file);;
    
    // 4. Process PKCS12 if successfully parsed
    if (p12) {
        // Get encoder for PKCS12 (hypothetical - using available encoder functions)
        OSSL_ENCODER *encoder = OSSL_ENCODER_fetch(NULL, "PKCS12", NULL);
        if (encoder) {
            OSSL_PARAM params[2];
            params[0] = OSSL_PARAM_construct_end();
            
            // 5. Get encoder parameters
            OSSL_ENCODER_get_params(encoder, params);
            
            OSSL_ENCODER_free(encoder);
        }
        
        // 6. Create X509 extension and get its data
        X509_EXTENSION *ext = X509V3_EXT_conf_nid(NULL, NULL, NID_netscape_cert_type, "client");
        if (ext) {
            const ASN1_OCTET_STRING *ext_data = X509_EXTENSION_get_data(ext);
            X509_EXTENSION_free(ext);
        }
        
        PKCS12_free(p12);
    }
    
    // 7. Use ARIA-128-GCM cipher
    const EVP_CIPHER *aria_cipher = EVP_aria_128_gcm();
    if (aria_cipher && size >= 16) {
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        if (ctx) {
            unsigned char key[16];
            unsigned char iv[12];
            memcpy(key, data, size > 16 ? 16 : size);
            if (size > 28) memcpy(iv, data + 16, 12);
            
            EVP_EncryptInit_ex(ctx, aria_cipher, NULL, key, iv);
            EVP_CIPHER_CTX_free(ctx);
        }
    }
    
    // 8. Use BN_MONT_CTX_copy
    BN_CTX *bn_ctx = BN_CTX_new();
    if (bn_ctx) {
        BIGNUM *a = BN_bin2bn(data, size > 32 ? 32 : size, NULL);
        if (a) {
            BN_MONT_CTX *mont = BN_MONT_CTX_new();
            BN_MONT_CTX *mont_copy = BN_MONT_CTX_new();
            
            if (mont && mont_copy) {
                BN_MONT_CTX_set(mont, a, bn_ctx);
                BN_MONT_CTX_copy(mont_copy, mont);
                
                BN_MONT_CTX_free(mont);
                BN_MONT_CTX_free(mont_copy);
            }
            BN_free(a);
        }
        BN_CTX_free(bn_ctx);
    }
    
    // 9. Use RC2 encryption
    if (size >= 8) {
        RC2_KEY rc2_key;
        unsigned long rc2_data[2] = {0, 0};
        
        // Set up key (use first 16 bytes of input if available)
        unsigned char rc2_key_data[16];
        size_t key_len = size > 16 ? 16 : size;
        memcpy(rc2_key_data, data, key_len);
        RC2_set_key(&rc2_key, key_len, rc2_key_data, 0);
        
        // Prepare data for encryption (use next 8 bytes if available)
        if (size >= 16) {
            memcpy(rc2_data, data + 8, 8);
        }
        
        // 10. Perform RC2 encryption
        RC2_encrypt(rc2_data, &rc2_key);
    }
    
    // 11. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some processed data to output
        if (size > 0) {
            fwrite(data, 1, size > 1024 ? 1024 : size, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // 12. Clean up temporary file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}