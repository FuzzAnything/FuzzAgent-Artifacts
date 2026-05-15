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
//<ID> 594
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 6
//<Quality> {"density":5,"unique_branches":{"ossl_pw_set_ui_method":[[82,9,82,21,0,0,4,1],[52,41,52,42,1,0,4,0],[52,41,52,42,1,0,4,0]],"OSSL_STORE_delete":[[511,9,511,26,0,0,4,0],[512,13,512,64,0,0,4,1],[513,16,513,59,0,0,4,1]]},"library_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","UI_create_method","UI_method_set_ex_data","NOTICEREF_it","TLS_method","SSL_CTX_new","SSL_new","SSL_use_PrivateKey_file","OSSL_CRMF_MSG_new","OSSL_CRMF_MSG_get0_regCtrl_oldCertID","EVP_des_ede","EVP_aes_128_cbc_hmac_sha256","ossl_check_void_sk_type","OPENSSL_sk_delete_ptr","X509_TRUST_get_count","ACCESS_DESCRIPTION_new","d2i_ACCESS_DESCRIPTION","i2d_ACCESS_DESCRIPTION","CRYPTO_free","OSSL_STORE_delete","ACCESS_DESCRIPTION_free","OSSL_CRMF_MSG_free","SSL_free","SSL_CTX_free","UI_destroy_method"],"critical_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","UI_create_method","UI_method_set_ex_data","NOTICEREF_it","TLS_method","SSL_CTX_new","SSL_new","SSL_use_PrivateKey_file","OSSL_CRMF_MSG_new","OSSL_CRMF_MSG_get0_regCtrl_oldCertID","EVP_des_ede","EVP_aes_128_cbc_hmac_sha256","ossl_check_void_sk_type","OPENSSL_sk_delete_ptr","X509_TRUST_get_count","ACCESS_DESCRIPTION_new","d2i_ACCESS_DESCRIPTION","i2d_ACCESS_DESCRIPTION","CRYPTO_free","OSSL_STORE_delete","ACCESS_DESCRIPTION_free","OSSL_CRMF_MSG_free","SSL_free","SSL_CTX_free","UI_destroy_method"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified APIs in a logical sequence:

*/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. Initialize OpenSSL
    OPENSSL_init_crypto(0, NULL);
    OPENSSL_init_ssl(0, NULL);
    
    // 2. Create UI method and set ex_data
    UI_METHOD *ui_method = UI_create_method("FuzzUI");
    UI_method_set_ex_data(ui_method, 0, NULL);
    
    // 3. Get NOTICEREF ASN1 item (reference only)
    const ASN1_ITEM *noticeref_item = NOTICEREF_it();
    (void)noticeref_item; // Used to avoid unused variable warning
    
    // 4. Create SSL context and SSL object
    const SSL_METHOD *method = TLS_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    SSL *ssl = SSL_new(ctx);
    
    // 5. Write input data to file for SSL_use_PrivateKey_file
    FILE *key_file = fmemopen((void *)data, size, "rb");
    if (key_file) {
        assert_file_closed(&key_file);;
        SSL_use_PrivateKey_file(ssl, "input_file", SSL_FILETYPE_PEM);
    }
    
    // 6. Create and manipulate CRMF message
    OSSL_CRMF_MSG *crmf_msg = OSSL_CRMF_MSG_new();
    if (crmf_msg) {
        OSSL_CRMF_CERTID *certid = OSSL_CRMF_MSG_get0_regCtrl_oldCertID(crmf_msg);
        (void)certid; // Used to avoid unused variable warning
    }
    
    // 7. Get cipher references
    const EVP_CIPHER *cipher1 = EVP_des_ede();
    const EVP_CIPHER *cipher2 = EVP_aes_128_cbc_hmac_sha256();
    (void)cipher1; // Used to avoid unused variable warning
    (void)cipher2; // Used to avoid unused variable warning
    
    // 8. Create and manipulate OPENSSL_STACK
    struct stack_st_void *void_stack = NULL;
    OPENSSL_STACK *checked_stack = ossl_check_void_sk_type(void_stack);
    if (checked_stack) {
        void *dummy_ptr = NULL;
        OPENSSL_sk_delete_ptr(checked_stack, dummy_ptr);
    }
    
    // 9. Get X509 trust count
    int trust_count = X509_TRUST_get_count();
    (void)trust_count; // Used to avoid unused variable warning
    
    // 10. Create, parse, and serialize ACCESS_DESCRIPTION from input data
    ACCESS_DESCRIPTION *ad = ACCESS_DESCRIPTION_new();
    if (ad && size > 0) {
        const unsigned char *p = data;
        ACCESS_DESCRIPTION *parsed_ad = d2i_ACCESS_DESCRIPTION(&ad, &p, (long)size);
        
        if (parsed_ad) {
            unsigned char *out_buf = NULL;
            int out_len = i2d_ACCESS_DESCRIPTION(parsed_ad, &out_buf);
            
            if (out_buf) {
                // Write serialized data to output file
                FILE *out_file = fopen("output_file", "wb");
                if (out_file) {
                    fwrite(out_buf, 1, out_len, out_file);
                    assert_file_closed(&out_file);;
                }
                
                // Free the buffer using CRYPTO_free
                CRYPTO_free(out_buf, __FILE__, __LINE__);
            }
        }
    }
    
    // 11. Attempt OSSL_STORE delete operation
    OSSL_STORE_delete("file://input_file", NULL, NULL, ui_method, NULL, NULL);
    
    // 12. Clean up all allocated resources
    if (ad) ACCESS_DESCRIPTION_free(ad);
    if (crmf_msg) OSSL_CRMF_MSG_free(crmf_msg);
    if (ssl) SSL_free(ssl);
    if (ctx) SSL_CTX_free(ctx);
    if (ui_method) UI_destroy_method(ui_method);
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&key_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}