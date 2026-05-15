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
//<ID> 54
//<Prompt> []
/*<Combination>: [
*/
//<score> 121.5, nr_unique_branch: 136
//<Quality> {"density":9,"unique_branches":{"EVP_PKEY_get1_DH":[[962,9,962,20,0,0,4,1]],"evp_pkey_get0_DH_int":[[946,9,946,34,0,0,4,0],[946,38,946,64,0,0,4,0]],"custom_ext_find":[[88,17,88,38,0,0,4,1]],"custom_exts_free":[[448,37,448,58,0,0,4,0],[449,13,449,55,0,0,4,0]],"ssl_cipher_id_cmp":[[3412,9,3412,22,0,0,4,0],[3412,9,3412,22,0,0,4,1],[3414,9,3414,22,0,0,4,0],[3414,9,3414,22,0,0,4,1]],"ssl3_get_cipher_by_id":[[4720,9,4720,19,0,0,4,0],[4720,9,4720,19,0,0,4,1],[4723,9,4723,19,0,0,4,0],[4723,9,4723,19,0,0,4,1]],"ossl_tls_add_custom_ext_intern":[[484,9,484,23,0,0,4,1],[487,9,487,21,0,0,4,0],[496,9,496,61,0,0,4,1],[507,9,507,42,0,0,4,1],[512,9,512,26,0,0,4,1],[515,9,515,52,0,0,4,1],[519,9,519,20,0,0,4,1],[531,23,531,36,0,0,4,1]],"PEM_X509_INFO_read_bio_ex":[[68,9,68,19,0,0,4,0],[69,13,69,52,0,0,4,1],[76,9,76,39,0,0,4,1],[83,13,83,19,0,0,4,0],[83,13,83,19,0,0,4,1],[85,17,85,45,0,0,4,0],[85,17,85,45,0,0,4,1],[94,13,94,47,0,0,4,1],[95,16,95,54,0,0,4,1],[96,16,96,58,0,0,4,1],[112,20,112,58,0,0,4,1],[122,20,122,69,0,0,4,1],[153,13,153,24,0,0,4,1],[193,9,193,27,0,0,4,1],[193,31,193,48,0,0,4,1],[193,52,193,72,0,0,4,1],[193,76,193,98,0,0,4,1],[201,9,201,12,0,0,4,0],[201,9,201,12,0,0,4,1],[202,21,202,48,0,0,4,1],[206,13,206,22,0,0,4,0]],"ecp_nistz256_mult_precompute":[[817,9,817,26,0,0,4,1],[822,9,822,44,0,0,4,0]],"X509_INFO_free":[[29,9,29,18,0,0,4,1]],"SSL_bytes_to_cipher_list":[[7308,9,7308,19,0,0,4,1],[7308,23,7308,33,0,0,4,1],[7311,9,7311,43,0,0,4,1],[32,6,32,19,2,0,4,1],[33,23,33,61,2,0,4,0]],"X509_STORE_add_lookup":[[305,17,305,39,0,0,4,1],[313,9,313,19,0,0,4,1],[85,38,85,123,5,0,4,0]],"X509_PKEY_free":[[37,9,37,18,0,0,4,0]],"X509_LOOKUP_ctrl_ex":[[84,9,84,28,0,0,4,1],[86,9,86,37,0,0,4,0]],"ssl_get_cipher_by_char":[[2055,9,2055,18,0,0,4,0],[2055,9,2055,18,0,0,4,1],[2055,23,2055,27,0,0,4,1]],"X509_STORE_load_file_ex":[[55,9,55,21,0,0,4,1],[56,12,56,77,0,0,4,1],[57,12,59,17,0,0,4,0]],"X509_INFO_new":[[21,9,21,20,0,0,4,1]],"EC_GROUP_precompute_mult":[[1206,9,1206,30,0,0,4,1],[1210,9,1210,42,0,0,4,0]],"X509_LOOKUP_new":[[26,9,26,20,0,0,4,1],[30,9,30,33,0,0,4,1]],"X509_STORE_free":[[270,17,270,39,0,0,4,0]],"by_file_ctrl_ex":[[55,13,55,16,0,0,4,1],[56,5,56,26,0,0,4,0],[57,13,57,42,0,0,4,1],[72,17,72,42,0,0,4,0]],"SSL_extension_supported":[[625,5,625,60,0,0,4,1],[626,5,626,38,0,0,4,1],[627,5,627,38,0,0,4,1],[628,5,628,31,0,0,4,1],[630,5,630,36,0,0,4,1],[632,5,632,29,0,0,4,1],[633,5,633,33,0,0,4,1],[634,5,634,41,0,0,4,1],[635,5,635,33,0,0,4,1],[636,5,636,36,0,0,4,1],[637,5,637,42,0,0,4,1],[639,5,639,25,0,0,4,1],[642,5,642,36,0,0,4,1],[645,5,645,50,0,0,4,1],[648,5,648,30,0,0,4,1],[650,5,650,38,0,0,4,1],[651,5,651,40,0,0,4,1],[652,5,652,44,0,0,4,1],[653,5,653,35,0,0,4,1],[654,5,654,28,0,0,4,1],[655,5,655,32,0,0,4,1],[656,5,656,45,0,0,4,1],[657,5,657,25,0,0,4,1],[658,5,658,41,0,0,4,1],[659,5,659,42,0,0,4,1],[660,5,660,38,0,0,4,1],[661,5,661,38,0,0,4,1],[663,5,663,25,0,0,4,1],[664,5,664,38,0,0,4,1],[667,5,667,12,0,0,4,0]],"X509_LOOKUP_free":[[39,9,39,20,0,0,4,1],[41,9,41,30,0,0,4,0],[41,34,41,61,0,0,4,1]],"X509_load_cert_crl_file_ex":[[242,9,242,34,0,0,4,1],[249,9,249,19,0,0,4,0],[249,9,249,19,0,0,4,1],[255,9,255,20,0,0,4,0],[255,9,255,20,0,0,4,1],[259,17,259,37,0,0,4,1],[276,9,276,19,0,0,4,0]],"X509_LOOKUP_shutdown":[[73,9,73,28,0,0,4,1],[75,9,75,38,0,0,4,1]],"ossl_bytes_to_cipher_list":[[7327,9,7327,45,0,0,4,0],[7327,9,7327,45,0,0,4,1],[7328,13,7328,18,0,0,4,1],[7335,9,7335,49,0,0,4,0],[7335,9,7335,49,0,0,4,1],[7336,13,7336,18,0,0,4,1],[7346,9,7346,19,0,0,4,1],[7346,23,7346,36,0,0,4,1],[7354,12,7354,55,0,0,4,0],[7354,12,7354,55,0,0,4,1],[7356,13,7356,22,0,0,4,0],[7356,13,7356,22,0,0,4,1],[7357,18,7357,26,0,0,4,0],[7357,18,7357,26,0,0,4,1],[7357,30,7357,49,0,0,4,1],[7357,62,7357,71,0,0,4,0],[7357,62,7357,71,0,0,4,1],[7357,75,7357,94,0,0,4,1],[7366,9,7366,44,0,0,4,1],[7374,9,7374,20,0,0,4,0],[7378,9,7378,26,0,0,4,0]]},"library_calls":["SSL_CTX_new","TLS_method","EC_GROUP_new_by_curve_name","BN_CTX_new","EC_GROUP_precompute_mult","SSL_new","SSL_bytes_to_cipher_list","OPENSSL_sk_free","ossl_check_SSL_CIPHER_sk_type","OPENSSL_sk_free","ossl_check_SSL_CIPHER_sk_type","SSL_CTX_add_custom_ext","SSL_CTX_load_verify_file","CRYPTO_malloc","ossl_check_ERR_STRING_DATA_lh_type","CRYPTO_free","EVP_PKEY_new","DH_new","BN_new","BN_new","BN_set_word","BN_set_word","DH_set0_pqg","EVP_PKEY_get1_DH","DH_free","DH_free","OSSL_FUNC_keymgmt_free","EC_GROUP_free","BN_CTX_free","SSL_free","SSL_CTX_free","EVP_PKEY_free"],"critical_calls":["SSL_CTX_new","TLS_method","EC_GROUP_new_by_curve_name","BN_CTX_new","EC_GROUP_precompute_mult","SSL_new","SSL_bytes_to_cipher_list","OPENSSL_sk_free","ossl_check_SSL_CIPHER_sk_type","OPENSSL_sk_free","ossl_check_SSL_CIPHER_sk_type","SSL_CTX_add_custom_ext","SSL_CTX_load_verify_file","CRYPTO_malloc","ossl_check_ERR_STRING_DATA_lh_type","CRYPTO_free","EVP_PKEY_new","DH_new","BN_new","BN_new","BN_set_word","BN_set_word","DH_set0_pqg","EVP_PKEY_get1_DH","DH_free","DH_free","OSSL_FUNC_keymgmt_free","EC_GROUP_free","BN_CTX_free","SSL_free","SSL_CTX_free","EVP_PKEY_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Initialize SSL context
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    if (!ctx) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create EC_GROUP for EC operations
    EC_GROUP *ec_group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    BN_CTX *bn_ctx = BN_CTX_new();
    
    // 5. Use EC_GROUP_precompute_mult
    if (ec_group && bn_ctx) {
        EC_GROUP_precompute_mult(ec_group, bn_ctx);
    }
    
    // 6. Create SSL object for cipher list operations
    SSL *ssl = SSL_new(ctx);
    
    // 7. Use SSL_bytes_to_cipher_list
    if (ssl) {
        struct stack_st_SSL_CIPHER *sk = NULL;
        struct stack_st_SSL_CIPHER *scsvs = NULL;
        
        // Use first half of data for cipher conversion
        size_t cipher_len = size > 0 ? size / 2 : 0;
        SSL_bytes_to_cipher_list(ssl, data, cipher_len, 0, &sk, &scsvs);
        
        // Clean up cipher lists
        sk_SSL_CIPHER_free(sk);
        sk_SSL_CIPHER_free(scsvs);
    }
    
    // 8. Use SSL_CTX_add_custom_ext with dummy callbacks
    if (ctx) {
        SSL_CTX_add_custom_ext(ctx, 0x1234, 0, 
            [](SSL *, unsigned int, unsigned int, const unsigned char **, 
               size_t *, X509 *, size_t, int *, void *) { return 1; },
            [](SSL *, unsigned int, unsigned int, const unsigned char *, void *) {},
            nullptr,
            [](SSL *, unsigned int, unsigned int, const unsigned char *, 
               size_t, X509 *, size_t, int *, void *) { return 1; },
            nullptr);
    }
    
    // 9. Use SSL_CTX_load_verify_file with input data
    if (ctx) {
        // Write input data to a temporary CA file
        FILE *ca_file = fopen("temp_ca.pem", "wb");
        if (ca_file) {
            fwrite(data, 1, size > 1024 ? 1024 : size, ca_file);
            assert_file_closed(&ca_file);;
            SSL_CTX_load_verify_file(ctx, "temp_ca.pem");
            remove("temp_ca.pem");
        }
    }
    
    // 10. Create dummy lhash structure for ossl_check_ERR_STRING_DATA_lh_type
    struct lhash_st_ERR_STRING_DATA *dummy_lh = 
        (struct lhash_st_ERR_STRING_DATA *)OPENSSL_malloc(sizeof(struct lhash_st_ERR_STRING_DATA));
    if (dummy_lh) {
        OPENSSL_LHASH *lh = ossl_check_ERR_STRING_DATA_lh_type(dummy_lh);
        OPENSSL_free(dummy_lh);
    }
    
    // 11. Create dummy EVP_PKEY and get DH from it
    EVP_PKEY *pkey = EVP_PKEY_new();
    if (pkey) {
        // Create a minimal DH key
        DH *dh = DH_new();
        if (dh) {
            BIGNUM *p = BN_new();
            BIGNUM *g = BN_new();
            if (p && g) {
                BN_set_word(p, 23);
                BN_set_word(g, 5);
                DH_set0_pqg(dh, p, NULL, g);
                
                // Try to assign DH to PKEY (simulate the scenario)
                // Note: EVP_PKEY_assign_DH is not in provided list, so we skip
                // and just call EVP_PKEY_get1_DH which will return NULL
                struct dh_st *retrieved_dh = EVP_PKEY_get1_DH(pkey);
                if (retrieved_dh) {
                    DH_free(retrieved_dh);
                }
            }
            DH_free(dh);
        }
    }
    
    // 12. Create dummy OSSL_DISPATCH for OSSL_FUNC_keymgmt_free
    OSSL_DISPATCH dummy_dispatch[] = {
        {0, NULL}
    };
    OSSL_FUNC_keymgmt_free_fn *free_fn = OSSL_FUNC_keymgmt_free(dummy_dispatch);
    // Note: free_fn will be NULL, but we've called the function
    
    // 13. Clean up all allocated resources
    if (ec_group) EC_GROUP_free(ec_group);
    if (bn_ctx) BN_CTX_free(bn_ctx);
    if (ssl) SSL_free(ssl);
    if (ctx) SSL_CTX_free(ctx);
    if (pkey) EVP_PKEY_free(pkey);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}