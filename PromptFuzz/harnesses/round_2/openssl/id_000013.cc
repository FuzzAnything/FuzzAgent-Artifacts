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
//<ID> 315
//<Prompt> []
/*<Combination>: [
*/
//<score> 224, nr_unique_branch: 74
//<Quality> {"density":8,"unique_branches":{"asn1_template_ex_i2d":[[370,9,370,17,0,0,4,0],[370,21,370,58,0,0,4,1]],"X509V3_add1_i2d":[[268,9,268,36,0,0,4,0],[272,9,272,20,0,0,4,1],[293,13,293,52,0,0,4,1],[293,56,293,85,0,0,4,1],[306,9,306,13,0,0,4,1],[312,9,312,20,0,0,4,1],[325,9,325,19,0,0,4,0],[326,12,326,56,0,0,4,1],[328,9,328,32,0,0,4,1]],"X509V3_parse_list":[[334,9,334,22,0,0,4,1],[335,17,335,25,0,0,4,0],[339,21,339,26,0,0,4,1],[358,9,358,23,0,0,4,0],[359,17,359,25,0,0,4,1],[376,9,376,27,0,0,4,0],[378,13,378,18,0,0,4,1],[382,13,382,51,0,0,4,1]],"d2i_PKCS12_fp":[[270,9,270,20,0,0,4,0],[272,13,272,26,0,0,4,1]],"DES_set_key_unchecked":[[742,17,742,31,0,0,4,0],[742,17,742,31,0,0,4,1],[743,13,743,23,0,0,4,0],[743,13,743,23,0,0,4,1]],"X509V3_EXT_i2d":[[195,9,195,55,0,0,4,1]],"X509v3_get_ext_by_NID":[[38,9,38,20,0,0,4,1]],"ASN1_item_d2i_ex":[[166,9,166,21,0,0,4,1]],"RC2_set_key":[[301,9,301,18,0,0,4,1],[325,12,325,15,0,0,4,0]],"PEM_ASN1_write_bio_internal":[[332,9,332,20,0,0,4,1],[347,9,347,20,0,0,4,1],[352,13,352,24,0,0,4,0],[353,9,353,19,0,0,4,0]],"ossl_pkcs12_get0_pkcs7ctx":[[61,9,61,20,0,0,4,0]],"v2i_BASIC_CONSTRAINTS":[[65,9,65,50,0,0,4,1],[69,17,69,38,0,0,4,0],[69,17,69,38,0,0,4,1],[71,13,71,41,0,0,4,0],[72,17,72,56,0,0,4,1]],"tdes_init":[[83,9,83,32,0,0,4,1],[90,9,90,19,0,0,4,0],[91,13,91,56,0,0,4,1],[101,9,101,20,0,0,4,0],[102,13,102,34,0,0,4,1],[106,13,106,50,0,0,4,1],[110,9,110,47,0,0,4,1]],"ossl_tdes_newctx":[[31,9,31,21,0,0,4,0],[205,9,205,32,1,0,4,1]],"v3_check_critical":[[207,9,207,31,0,0,4,1],[82,25,82,66,2,0,4,1],[58,30,58,74,3,0,4,0]],"EVP_CIPHER_CTX_reset":[[33,9,33,28,0,0,4,0]],"OSSL_ENCODER_get_params":[[590,9,590,24,0,0,4,0],[590,28,590,55,0,0,4,1]],"X509_EXTENSION_set_critical":[[253,20,253,26,0,0,4,0]],"asn1_ex_i2c":[[537,12,537,41,0,0,4,1],[538,13,538,26,0,0,4,0],[574,5,574,24,0,0,4,0],[576,13,576,25,0,0,4,1],[578,13,578,36,0,0,4,0],[582,17,582,23,0,0,4,0],[582,27,582,41,0,0,4,1],[584,17,584,24,0,0,4,1]],"i2d_EC_PUBKEY":[[814,9,814,18,0,0,4,0]],"X509v3_get_ext_by_OBJ":[[49,9,49,19,0,0,4,0]],"X509V3_add_value":[[84,9,84,22,0,0,4,0]],"PEM_ASN1_write":[[307,9,307,44,0,0,4,1]],"asn1_item_flags_i2d":[[63,13,63,21,0,0,4,0]],"ossl_tdes_get_ctx_params":[[159,9,159,58,0,0,4,1],[163,9,163,18,0,0,4,1]],"X509V3_get_value_bool":[[270,9,270,38,0,0,4,1],[272,9,272,34,0,0,4,0]],"asn1_i2d_ex_primitive":[[489,9,489,18,0,0,4,0]]},"library_calls":["PKCS12_BAGS_it","PKCS12_AUTHSAFES_it","d2i_PKCS12_fp","EVP_des_ede3_ecb","EVP_aria_128_gcm","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","BN_CTX_new","BN_bin2bn","BN_MONT_CTX_new","BN_MONT_CTX_set","BN_MONT_CTX_new","BN_MONT_CTX_copy","RC2_set_key","RC2_encrypt","OSSL_ENCODER_fetch","OSSL_PARAM_construct_end","OSSL_ENCODER_get_params","X509V3_EXT_conf_nid","X509_EXTENSION_get_data","X509V3_add1_i2d","PEM_write_EC_PUBKEY","OSSL_ENCODER_free","X509_EXTENSION_free","OPENSSL_sk_pop_free","ossl_check_X509_EXTENSION_sk_type","ossl_check_X509_EXTENSION_freefunc_type","PKCS12_free","EVP_CIPHER_CTX_free","BN_free","BN_MONT_CTX_free","BN_MONT_CTX_free","BN_CTX_free"],"critical_calls":["PKCS12_BAGS_it","PKCS12_AUTHSAFES_it","d2i_PKCS12_fp","EVP_des_ede3_ecb","EVP_aria_128_gcm","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","BN_CTX_new","BN_bin2bn","BN_MONT_CTX_new","BN_MONT_CTX_set","BN_MONT_CTX_new","BN_MONT_CTX_copy","RC2_set_key","RC2_encrypt","OSSL_ENCODER_fetch","OSSL_PARAM_construct_end","OSSL_ENCODER_get_params","X509V3_EXT_conf_nid","X509_EXTENSION_get_data","X509V3_add1_i2d","PEM_write_EC_PUBKEY","OSSL_ENCODER_free","X509_EXTENSION_free","OPENSSL_sk_pop_free","ossl_check_X509_EXTENSION_freefunc_type","ossl_check_X509_EXTENSION_sk_type","PKCS12_free","EVP_CIPHER_CTX_free","BN_free","BN_MONT_CTX_free","BN_MONT_CTX_free","BN_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 1: Create memory file for input
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Step 2: Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: Reference the ASN1_ITEM constants
    const ASN1_ITEM *bags_it = PKCS12_BAGS_it();
    const ASN1_ITEM *authsafes_it = PKCS12_AUTHSAFES_it();
    // Use them to avoid compiler warnings
    (void)bags_it;
    (void)authsafes_it;
    
    // Step 4: Read PKCS12 from input file
    PKCS12 *p12 = NULL;
    p12 = d2i_PKCS12_fp(in_file, &p12);
    
    // Step 5: Get ciphers for encryption
    const EVP_CIPHER *des3_ecb = EVP_des_ede3_ecb();
    const EVP_CIPHER *aria_gcm = EVP_aria_128_gcm();
    
    // Step 6: Create and initialize encryption context
    EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new();
    if (cipher_ctx && size >= 32) {
        // Use first 24 bytes as key, next 8 bytes as IV for DES3
        const unsigned char *key = data;
        const unsigned char *iv = data + 24;
        EVP_EncryptInit_ex(cipher_ctx, des3_ecb, NULL, key, iv);
    }
    
    // Step 7: Create BIGNUM from input data
    BN_CTX *bn_ctx = BN_CTX_new();
    BIGNUM *bn = NULL;
    if (size > 0) {
        size_t bn_len = size > 32 ? 32 : size;
        bn = BN_bin2bn(data, bn_len, NULL);
    }
    
    // Step 8: Montgomery context operations
    BN_MONT_CTX *mont = BN_MONT_CTX_new();
    BN_MONT_CTX *mont_copy = NULL;
    if (mont && bn) {
        BN_MONT_CTX_set(mont, bn, bn_ctx);
        mont_copy = BN_MONT_CTX_new();
        if (mont_copy) {
            BN_MONT_CTX_copy(mont_copy, mont);
        }
    }
    
    // Step 9: RC2 encryption
    RC2_KEY rc2_key;
    if (size >= 16) {
        RC2_set_key(&rc2_key, 16, data, 128);
        if (size >= 24) {
            unsigned long rc2_data[2] = {0, 0};
            // Use 8 bytes from input for RC2 data
            memcpy(rc2_data, data + 16, 8);
            RC2_encrypt(rc2_data, &rc2_key);
        }
    }
    
    // Step 10: Fetch encoder and get parameters
    OSSL_ENCODER *encoder = OSSL_ENCODER_fetch(NULL, "RSA", NULL);
    OSSL_PARAM params[2];
    params[0] = OSSL_PARAM_construct_end();
    if (encoder) {
        OSSL_ENCODER_get_params(encoder, params);
    }
    
    // Step 11: Create X.509 extension
    struct lhash_st_CONF_VALUE *conf = NULL;
    X509V3_CTX v3_ctx;
    memset(&v3_ctx, 0, sizeof(v3_ctx));
    
    X509_EXTENSION *ext = X509V3_EXT_conf_nid(conf, &v3_ctx, 
                                            NID_basic_constraints, "critical,CA:TRUE");
    if (ext) {
        const ASN1_OCTET_STRING *ext_data = X509_EXTENSION_get_data(ext);
        (void)ext_data; // Use to avoid warning
    }
    
    // Step 12: Add extension using X509V3_add1_i2d
    struct stack_st_X509_EXTENSION *exts = NULL;
    if (ext) {
        X509V3_add1_i2d(&exts, NID_subject_key_identifier, NULL, 0, 0);
    }
    
    // Step 13: Try to write EC_PUBKEY (even if NULL)
    EC_KEY *ec_key = NULL;
    PEM_write_EC_PUBKEY(out_file, ec_key);
    
    // Step 14: Cleanup
    if (encoder) OSSL_ENCODER_free(encoder);
    if (ext) X509_EXTENSION_free(ext);
    if (exts) sk_X509_EXTENSION_pop_free(exts, X509_EXTENSION_free);
    if (p12) PKCS12_free(p12);
    if (cipher_ctx) EVP_CIPHER_CTX_free(cipher_ctx);
    if (bn) BN_free(bn);
    if (mont) BN_MONT_CTX_free(mont);
    if (mont_copy) BN_MONT_CTX_free(mont_copy);
    if (bn_ctx) BN_CTX_free(bn_ctx);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}