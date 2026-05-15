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
//<ID> 755
//<Prompt> []
/*<Combination>: [
*/
//<score> 215, nr_unique_branch: 148
//<Quality> {"density":10,"unique_branches":{"evp_keymgmt_gen":[[457,24,457,52,0,0,4,0],[459,9,459,29,0,0,4,1],[467,9,467,20,0,0,4,0],[467,24,467,48,0,0,4,0]],"EC_GROUP_clear_free":[[144,9,144,15,0,0,4,1],[147,9,147,45,0,0,4,0]],"evp_skey_alloc":[[36,9,36,21,0,0,4,1],[39,9,39,55,0,0,4,1],[42,9,42,46,0,0,4,1],[46,9,46,27,0,0,4,1],[50,9,50,38,0,0,4,0]],"evp_keymgmt_util_gen":[[519,9,519,72,0,0,4,0]],"EVP_SKEYMGMT_get0_provider":[[177,12,177,30,0,0,4,0]],"EVP_PKEY_CTX_set_dh_pad":[[165,9,165,20,0,0,4,1],[165,24,165,50,0,0,4,0]],"evp_keymgmt_gen_cleanup":[[476,9,476,37,0,0,4,0]],"OSSL_HTTP_is_alive":[[1067,12,1067,24,0,0,4,1]],"evp_pkey_ctx_free_old_ops":[[231,13,231,43,0,0,4,0],[231,47,231,67,0,0,4,0],[482,5,482,53,5,0,4,0]],"BIO_ADDRINFO_family":[[389,9,389,20,0,0,4,0]],"BIO_get_host_ip":[[50,9,50,29,0,0,4,1],[53,9,53,77,0,0,4,0],[56,13,56,48,0,0,4,1],[58,20,58,76,0,0,4,0],[52,24,53,24,2,0,4,0]],"dh_gen_set_params":[[721,9,721,21,0,0,4,1],[721,25,721,63,0,0,4,1],[724,9,724,44,0,0,4,1],[727,9,727,28,0,0,4,1]],"BIO_lookup_ex":[[686,5,686,17,0,0,4,0],[688,5,688,18,0,0,4,1],[691,5,691,17,0,0,4,1],[694,5,694,19,0,0,4,1],[697,5,697,12,0,0,4,1],[703,9,703,26,0,0,4,1],[712,9,712,29,0,0,4,1],[727,13,727,25,0,0,4,0],[727,29,727,48,0,0,4,1],[732,13,732,45,0,0,4,1],[743,9,743,24,0,0,4,1],[750,9,750,24,0,0,4,1],[755,9,755,15,0,0,4,0],[758,9,758,16,0,0,4,1]],"dh_gen_cleanup":[[870,9,870,21,0,0,4,1]],"do_sigver_init":[[48,9,48,37,0,0,4,1],[51,9,51,26,0,0,4,0],[55,9,55,26,0,0,4,0]],"generic_free":[[28,9,28,24,0,0,4,0]],"EVP_SKEYMGMT_free":[[163,9,163,25,0,0,4,1],[167,9,167,16,0,0,4,0]],"ossl_ffc_params_set_seed":[[91,9,91,21,0,0,4,1]],"dh_gen_init_base":[[547,9,547,32,0,0,4,1],[550,9,550,97,0,0,4,1],[553,9,553,55,0,0,4,0],[564,26,564,52,0,0,4,1],[574,9,574,41,0,0,4,1]],"dh_gen_common_set_params":[[624,9,624,24,0,0,4,1],[634,9,634,30,0,0,4,1],[646,9,646,25,0,0,4,1],[649,9,649,27,0,0,4,1]],"skeymgmt_new":[[52,9,52,63,0,0,4,1],[54,9,54,46,0,0,4,1]],"DH_free":[[117,9,117,18,0,0,4,0]],"EVP_PKEY_generate":[[110,9,110,22,0,0,4,1],[113,9,113,20,0,0,4,1],[116,9,116,53,0,0,4,1],[119,9,119,23,0,0,4,0],[122,9,122,23,0,0,4,1],[127,9,127,39,0,0,4,1],[143,9,143,26,0,0,4,1],[162,11,162,14,0,0,4,0],[163,12,165,21,0,0,4,1],[171,9,171,12,0,0,4,1],[183,9,183,17,0,0,4,0],[184,13,184,35,0,0,4,0]],"skeymgmt_from_algorithm":[[68,9,68,44,0,0,4,1],[72,9,72,80,0,0,4,1],[78,12,78,33,0,0,4,0],[78,12,78,33,0,0,4,1],[79,17,79,33,0,0,4,1],[80,9,80,37,0,0,4,0],[80,9,80,37,0,0,4,1],[81,17,81,39,0,0,4,0],[84,9,84,39,0,0,4,0],[84,9,84,39,0,0,4,1],[85,17,85,41,0,0,4,0],[88,9,88,39,0,0,4,0],[88,9,88,39,0,0,4,1],[89,17,89,41,0,0,4,0],[92,9,92,41,0,0,4,1],[96,9,96,43,0,0,4,1],[100,9,100,52,0,0,4,0],[100,9,100,52,0,0,4,1],[101,17,101,45,0,0,4,0],[104,9,104,52,0,0,4,1],[112,9,112,31,0,0,4,1],[113,12,113,36,0,0,4,1],[114,12,114,36,0,0,4,1],[120,9,120,36,0,0,4,1]],"EC_POINT_clear_free":[[786,9,786,22,0,0,4,1],[789,9,789,45,0,0,4,0]],"deflt_query":[[771,5,771,26,0,0,4,0]],"EVP_DigestSignInit":[[302,9,302,21,0,0,4,1]],"evp_keymgmt_gen_init":[[400,9,400,34,0,0,4,1]],"int_ctx_new":[[86,17,86,32,0,0,4,1],[107,9,107,24,0,0,4,1],[155,9,155,24,0,0,4,0],[161,9,161,20,0,0,4,0]],"evp_skey_alloc_fetch":[[72,9,72,25,0,0,4,1]],"BIO_ADDR_rawaddress":[[176,9,176,36,0,0,4,0],[193,9,193,24,0,0,4,1],[196,9,196,18,0,0,4,0],[196,9,196,18,0,0,4,1],[199,9,199,18,0,0,4,0]],"dh_gen_set_params_decoder":[[593,9,593,18,0,0,4,1]],"dh_gen":[[752,9,752,32,0,0,4,1],[752,36,752,48,0,0,4,1],[760,9,760,37,0,0,4,1],[769,9,769,21,0,0,4,1],[777,9,777,49,0,0,4,1],[790,13,790,23,0,0,4,1],[795,13,795,37,0,0,4,1],[799,13,799,70,0,0,4,1],[801,13,801,31,0,0,4,1],[805,20,805,37,0,0,4,1],[808,13,808,33,0,0,4,1],[813,13,813,26,0,0,4,0],[816,13,816,75,0,0,4,1],[834,9,834,61,0,0,4,0],[835,13,835,27,0,0,4,0],[858,9,858,17,0,0,4,0],[52,41,52,42,3,0,4,0],[52,41,52,42,3,0,4,0]],"EVP_PKEY_keygen":[[211,9,211,45,0,0,4,1]],"generic_import":[[42,9,42,32,0,0,4,1],[45,9,45,59,0,0,4,0]],"EVP_SKEY_free":[[211,9,211,21,0,0,4,1],[216,9,216,14,0,0,4,1],[293,12,293,18,2,0,4,1]],"aes_import":[[24,9,24,20,0,0,4,0]],"ASN1_item_ex_i2d":[[137,13,137,20,0,0,4,1]],"ASN1_item_i2d_fp":[[74,9,74,44,0,0,4,1]],"BIO_ADDRINFO_address":[[448,9,448,20,0,0,4,0]],"BIO_ADDRINFO_free":[[455,9,455,20,0,0,4,1],[460,15,460,41,1,0,4,0]],"EVP_SKEY_import":[[95,9,95,21,0,0,4,1],[99,9,99,30,0,0,4,0]]},"library_calls":["X509_get_default_cert_area","EC_GROUP_new_by_curve_name","EC_GROUP_clear_free","EVP_PKEY_CTX_new_id","EVP_PKEY_keygen_init","EVP_PKEY_CTX_set_dh_pad","EVP_PKEY_keygen","EVP_PKEY_CTX_new","EVP_PKEY_encrypt_init","EVP_PKEY_encapsulate","EVP_PKEY_CTX_free","EVP_PKEY_free","EVP_PKEY_CTX_free","OSSL_HTTP_is_alive","OSSL_PARAM_construct_octet_string","OSSL_PARAM_construct_end","EVP_SKEY_import","EVP_SKEY_get0_raw_key","EVP_SKEY_free","BIO_get_host_ip","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_const_GENERAL_NAME_sk_type","OPENSSL_sk_free","ossl_check_GENERAL_NAME_sk_type","X509_ACERT_new","EVP_MD_CTX_new","EVP_DigestSignInit","EVP_sha256","X509_ACERT_sign_ctx","EVP_MD_CTX_free","i2d_X509_ACERT_fp","X509_ACERT_free"],"critical_calls":["X509_get_default_cert_area","EC_GROUP_new_by_curve_name","EC_GROUP_clear_free","EVP_PKEY_CTX_new_id","EVP_PKEY_keygen_init","EVP_PKEY_CTX_set_dh_pad","EVP_PKEY_keygen","EVP_PKEY_CTX_new","EVP_PKEY_encrypt_init","EVP_PKEY_encapsulate","EVP_PKEY_CTX_free","EVP_PKEY_free","EVP_PKEY_CTX_free","OSSL_HTTP_is_alive","OSSL_PARAM_construct_octet_string","OSSL_PARAM_construct_end","EVP_SKEY_import","EVP_SKEY_get0_raw_key","EVP_SKEY_free","BIO_get_host_ip","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_const_GENERAL_NAME_sk_type","OPENSSL_sk_free","ossl_check_GENERAL_NAME_sk_type","X509_ACERT_new","EVP_MD_CTX_new","EVP_DigestSignInit","EVP_sha256","X509_ACERT_sign_ctx","EVP_MD_CTX_free","i2d_X509_ACERT_fp","X509_ACERT_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create file pointer for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create file pointer for output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Suppress unused variable warning
    (void)out_fd; // Suppress unused variable warning
    
    // 1. Use X509_get_default_cert_area()
    const char *cert_area = X509_get_default_cert_area();
    if (cert_area) {
        // Could potentially use this path for certificate operations
    }
    
    // 2. Create and use EC_GROUP
    EC_GROUP *group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    if (group) {
        // Clear and free the EC_GROUP
        EC_GROUP_clear_free(group);
    }
    
    // 3. Create EVP_PKEY_CTX for DH operations
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_DH, NULL);
    if (ctx) {
        // Initialize for key generation
        if (EVP_PKEY_keygen_init(ctx) > 0) {
            // Set DH padding
            EVP_PKEY_CTX_set_dh_pad(ctx, 1);
            
            // Generate DH key
            EVP_PKEY *pkey = NULL;
            if (EVP_PKEY_keygen(ctx, &pkey) > 0) {
                // Create context for encapsulation
                EVP_PKEY_CTX *encap_ctx = EVP_PKEY_CTX_new(pkey, NULL);
                if (encap_ctx && EVP_PKEY_encrypt_init(encap_ctx) > 0) {
                    // Set up buffers for encapsulation
                    unsigned char wrappedkey[256];
                    unsigned char genkey[256];
                    size_t wrappedkeylen = sizeof(wrappedkey);
                    size_t genkeylen = sizeof(genkey);
                    
                    // Perform encapsulation
                    EVP_PKEY_encapsulate(encap_ctx, wrappedkey, &wrappedkeylen, 
                                        genkey, &genkeylen);
                    
                    // Clean up encapsulation context
                    EVP_PKEY_CTX_free(encap_ctx);
                }
                
                // Clean up pkey
                EVP_PKEY_free(pkey);
            }
        }
        
        // Clean up context
        EVP_PKEY_CTX_free(ctx);
    }
    
    // 4. Create dummy OSSL_HTTP_REQ_CTX and check if alive
    // Note: Creating a real HTTP context requires more setup, so we'll use NULL
    // which is allowed by the API for testing purposes
    int alive = OSSL_HTTP_is_alive(NULL);
    (void)alive; // Suppress unused variable warning
    
    // 5. Create and use EVP_SKEY
    // First create some dummy parameters for key import
    unsigned char raw_key_data[32];
    size_t data_to_use = size > sizeof(raw_key_data) ? sizeof(raw_key_data) : size;
    if (data_to_use > 0) {
        memcpy(raw_key_data, data, data_to_use);
    } else {
        // Use some default data if input is too small
        memset(raw_key_data, 0xAA, sizeof(raw_key_data));
    }
    
    OSSL_PARAM params[2];
    params[0] = OSSL_PARAM_construct_octet_string("raw-key", raw_key_data, data_to_use);
    params[1] = OSSL_PARAM_construct_end();
    
    // Import the symmetric key
    EVP_SKEY *skey = EVP_SKEY_import(NULL, "AES", NULL, EVP_PKEY_KEY_PARAMETERS, params);
    if (skey) {
        // Get raw key from EVP_SKEY
        const unsigned char *key_ptr = NULL;
        size_t key_len = 0;
        EVP_SKEY_get0_raw_key(skey, &key_ptr, &key_len);
        
        // Clean up
        EVP_SKEY_free(skey);
    }
    
    // 6. Use BIO_get_host_ip with a dummy hostname
    unsigned char ip[4];
    const char *test_hostname = "127.0.0.1";
    BIO_get_host_ip(test_hostname, ip);
    
    // 7. Create GENERAL_NAME stack and use ossl_check_const_GENERAL_NAME_sk_type
    struct stack_st_GENERAL_NAME *name_stack = sk_GENERAL_NAME_new_null();
    if (name_stack) {
        // Get const stack type
        const OPENSSL_STACK *const_stack = ossl_check_const_GENERAL_NAME_sk_type(name_stack);
        (void)const_stack; // Suppress unused variable warning
        
        // Clean up
        sk_GENERAL_NAME_free(name_stack);
    }
    
    // 8. Create X509_ACERT and sign it
    X509_ACERT *acert = X509_ACERT_new();
    if (acert) {
        // Create EVP_MD_CTX for signing
        EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
        if (md_ctx) {
            // Initialize signing context with SHA256
            if (EVP_DigestSignInit(md_ctx, NULL, EVP_sha256(), NULL, NULL) > 0) {
                // Sign the ACERT (this will fail but demonstrates API usage)
                X509_ACERT_sign_ctx(acert, md_ctx);
            }
            EVP_MD_CTX_free(md_ctx);
        }
        
        // Try to write ACERT to output file
        i2d_X509_ACERT_fp(out_file, acert);
        
        X509_ACERT_free(acert);
    }
    
    // Write some of the input data to output file
    if (size > 0) {
        fwrite(data, 1, size > 1024 ? 1024 : size, out_file);
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}