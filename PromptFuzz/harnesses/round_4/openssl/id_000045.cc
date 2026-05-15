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
//<ID> 589
//<Prompt> []
/*<Combination>: [
*/
//<score> 115, nr_unique_branch: 34
//<Quality> {"density":23,"unique_branches":{"CMS_add0_CertificateChoices":[[518,9,518,23,0,0,4,1],[520,9,520,24,0,0,4,0],[522,9,522,24,0,0,4,1],[525,9,525,13,0,0,4,1],[527,9,527,54,0,0,4,1]],"ossl_cms_content_bio":[[132,9,132,20,0,0,4,1],[135,9,135,21,0,0,4,1],[140,9,140,21,0,0,4,1],[140,25,140,65,0,0,4,0]],"CMS_sign_ex":[[536,17,536,32,0,0,4,0],[539,13,539,35,0,0,4,1],[548,9,548,45,0,0,4,1],[549,12,549,45,0,0,4,1]],"ossl_asn1_item_embed_free":[[88,13,88,49,0,0,4,0],[477,12,477,15,11,0,4,0]],"ossl_asn1_do_lock":[[105,13,105,21,0,0,4,1]],"_CONF_get_section":[[31,12,31,30,0,0,4,1]],"CMS_add1_cert":[[562,9,562,27,0,0,4,1],[564,9,564,33,0,0,4,0]],"cms_get0_certificate_choices":[[493,5,493,26,0,0,4,0],[506,5,506,12,0,0,4,1]],"CMS_dataInit":[[149,9,149,14,0,0,4,1],[195,9,195,15,0,0,4,0]],"CMS_final":[[907,9,907,52,0,0,4,0]],"CMS_add0_cert":[[541,9,541,23,0,0,4,1],[543,17,543,59,0,0,4,1],[553,9,553,13,0,0,4,1]],"X509_up_ref":[[120,9,120,47,0,0,4,1],[293,12,293,18,2,0,4,1]],"def_load_bio":[[588,9,588,21,0,0,4,0]],"cms_sd_set_version":[[71,17,71,68,0,0,4,0],[73,13,73,46,0,0,4,1],[76,20,76,55,0,0,4,1],[79,20,79,55,0,0,4,1]]},"library_calls":["OPENSSL_init_crypto","ERR_clear_error","PKCS12_init","EVP_MAC_fetch","EVP_MAC_CTX_new","EVP_ASYM_CIPHER_fetch","EVP_ASYM_CIPHER_settable_ctx_params","PBE2PARAM_new","X509_new","X509V3_set_ctx","BN_CTX_new","BN_new","BN_set_word","NCONF_new","NCONF_load","i2d_PBE2PARAM","CRYPTO_free","BIO_new_mem_buf","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OPENSSL_sk_push","ossl_check_X509_sk_type","ossl_check_X509_type","CMS_sign","CMS_stream","CMS_ContentInfo_free","OPENSSL_sk_free","ossl_check_X509_sk_type","BIO_free","OSSL_PARAM_construct_utf8_string","OSSL_PARAM_construct_end","EVP_MAC_init","EVP_MAC_update","EVP_MAC_final","X509V3_EXT_add_conf","ossl_check_CONF_VALUE_lh_type","ossl_check_CONF_VALUE_lh_hashfunc_type","d2i_PKCS12_fp","PKCS12_free","PKCS12_free","EVP_MAC_free","EVP_MAC_CTX_free","EVP_ASYM_CIPHER_free","PBE2PARAM_free","X509_free","BN_CTX_free","BN_free","NCONF_free"],"critical_calls":["OPENSSL_init_crypto","ERR_clear_error","PKCS12_init","EVP_MAC_fetch","EVP_MAC_CTX_new","EVP_ASYM_CIPHER_fetch","EVP_ASYM_CIPHER_settable_ctx_params","PBE2PARAM_new","X509_new","X509V3_set_ctx","BN_CTX_new","BN_new","BN_set_word","NCONF_new","NCONF_load","i2d_PBE2PARAM","CRYPTO_free","BIO_new_mem_buf","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OPENSSL_sk_push","ossl_check_X509_type","ossl_check_X509_sk_type","CMS_sign","CMS_stream","CMS_ContentInfo_free","OPENSSL_sk_free","ossl_check_X509_sk_type","BIO_free","OSSL_PARAM_construct_utf8_string","OSSL_PARAM_construct_end","EVP_MAC_init","EVP_MAC_update","EVP_MAC_final","X509V3_EXT_add_conf","ossl_check_CONF_VALUE_lh_type","ossl_check_CONF_VALUE_lh_hashfunc_type","d2i_PKCS12_fp","PKCS12_free","PKCS12_free","EVP_MAC_free","EVP_MAC_CTX_free","EVP_ASYM_CIPHER_free","PBE2PARAM_free","X509_free","BN_CTX_free","BN_free","NCONF_free"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Initialize OpenSSL
    OPENSSL_init_crypto(0, NULL);
    
    // Clear any previous errors
    ERR_clear_error();
    
    // Create PKCS12 structure
    PKCS12 *p12 = PKCS12_init(NID_pkcs7_data);
    
    // Create EVP_MAC
    EVP_MAC *mac = EVP_MAC_fetch(NULL, "HMAC", NULL);
    
    // Create EVP_MAC context
    EVP_MAC_CTX *mac_ctx = EVP_MAC_CTX_new(mac);
    
    // Create EVP_ASYM_CIPHER
    EVP_ASYM_CIPHER *asym_cipher = EVP_ASYM_CIPHER_fetch(NULL, "RSA", NULL);
    
    // Get settable params for asymmetric cipher
    const OSSL_PARAM *params = EVP_ASYM_CIPHER_settable_ctx_params(asym_cipher);
    
    // Create PBE2PARAM
    PBE2PARAM *pbe2 = PBE2PARAM_new();
    
    // Create X509 certificate
    X509 *cert = X509_new();
    
    // Create X509V3 context
    X509V3_CTX ctx;
    X509V3_set_ctx(&ctx, cert, cert, NULL, NULL, 0);
    
    // Create BN_CTX
    BN_CTX *bn_ctx = BN_CTX_new();
    
    // Create BIGNUM
    BIGNUM *bn = BN_new();
    
    // Set word in BIGNUM
    BN_set_word(bn, 42);
    
    // Create CONF structure
    CONF *conf = NCONF_new(NULL);
    
    // Create a temporary file with input data for NCONF_load
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        char filename[] = "input_file";
        FILE *tmp_file = fopen(filename, "wb");
        if (tmp_file) {
            fwrite(data, 1, size, tmp_file);
            assert_file_closed(&tmp_file);;
            
            // Load configuration
            long eline = 0;
            NCONF_load(conf, filename, &eline);
            
            // Remove temporary file
            remove(filename);
        }
        assert_file_closed(&in_file);;
    }
    
    // Convert PBE2PARAM to DER (just get size)
    unsigned char *der = NULL;
    int der_len = i2d_PBE2PARAM(pbe2, &der);
    if (der_len > 0) {
        // We got the length, but buffer is allocated by i2d_PBE2PARAM
        OPENSSL_free(der);
    }
    
    // Create CMS ContentInfo for signing
    CMS_ContentInfo *cms = NULL;
    
    // We need a BIO for data - use memory BIO with input data
    BIO *data_bio = BIO_new_mem_buf(data, size);
    if (data_bio) {
        // Create a dummy certificate chain for CMS_sign
        // Note: In real usage, we would need proper certificates
        // This is just to call the API
        STACK_OF(X509) *certs = sk_X509_new_null();
        if (certs && cert) {
            sk_X509_push(certs, cert);
        }
        
        // Create CMS signature (may fail due to missing private key)
        cms = CMS_sign(NULL, NULL, certs, data_bio, 0);
        
        if (cms) {
            // Set up streaming
            unsigned char **boundary = NULL;
            CMS_stream(&boundary, cms);
            
            // Free CMS
            CMS_ContentInfo_free(cms);
        }
        
        if (certs) {
            sk_X509_free(certs);
        }
        BIO_free(data_bio);
    }
    
    // Use MAC operations if we have enough data
    if (mac_ctx && size > 32) {
        // Use first 16 bytes as key, rest as data
        size_t key_len = size > 16 ? 16 : size;
        size_t data_len = size - key_len;
        
        // Create OSSL_PARAM array
        OSSL_PARAM mac_params[3];
        char mdname[] = "SHA256";
        mac_params[0] = OSSL_PARAM_construct_utf8_string("digest", mdname, 0);
        mac_params[1] = OSSL_PARAM_construct_end();
        
        // Initialize MAC
        EVP_MAC_init(mac_ctx, data, key_len, mac_params);
        
        // Update with data
        if (data_len > 0) {
            EVP_MAC_update(mac_ctx, data + key_len, data_len);
        }
        
        // Finalize
        unsigned char mac_out[64];
        size_t out_len = 0;
        EVP_MAC_final(mac_ctx, mac_out, &out_len, sizeof(mac_out));
    }
    
    // Try to add extension from configuration (may fail if no valid config)
    X509V3_EXT_add_conf(NULL, &ctx, "default", cert);
    
    // Convert CONF lhash type
    struct lhash_st_CONF_VALUE *lh = (struct lhash_st_CONF_VALUE *)conf;
    OPENSSL_LHASH *ossl_lh = ossl_check_CONF_VALUE_lh_type(lh);
    
    // Get hash function type
    OPENSSL_LH_HASHFUNC hash_func = ossl_check_CONF_VALUE_lh_hashfunc_type(NULL);
    
    // Try to read PKCS12 from file
    FILE *p12_file = fmemopen((void *)data, size > 1024 ? 1024 : size, "rb");
    if (p12_file) {
        PKCS12 *p12_from_file = d2i_PKCS12_fp(p12_file, NULL);
        if (p12_from_file) {
            PKCS12_free(p12_from_file);
        }
        assert_file_closed(&p12_file);;
    }
    
    // Cleanup
    PKCS12_free(p12);
    EVP_MAC_free(mac);
    EVP_MAC_CTX_free(mac_ctx);
    EVP_ASYM_CIPHER_free(asym_cipher);
    PBE2PARAM_free(pbe2);
    X509_free(cert);
    BN_CTX_free(bn_ctx);
    BN_free(bn);
    NCONF_free(conf);
    
    // Note: ossl_lh is owned by conf, so don't free separately
    // OPENSSL_LH_free would be called by NCONF_free
    
    assert_file_closed(&p12_file);
	assert_file_closed(&in_file);
	return 0;
}