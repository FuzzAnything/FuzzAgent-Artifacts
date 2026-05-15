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
//<ID> 593
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 4
//<Quality> {"density":7,"unique_branches":{"evp_decodeblock_int":[[687,5,687,11,0,0,4,0],[690,5,690,11,0,0,4,0],[694,5,694,11,0,0,4,1]],"EVP_DecodeUpdate":[[617,29,617,32,0,0,4,0]]},"library_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","TLS_method","SSL_CTX_new","SSL_new","SSL_use_PrivateKey_file","OSSL_STORE_delete","OSSL_CRMF_MSG_new","OSSL_CRMF_MSG_get0_regCtrl_oldCertID","OSSL_CRMF_MSG_free","EVP_des_ede","EVP_aes_128_cbc_hmac_sha256","NOTICEREF_it","X509_TRUST_get_count","ACCESS_DESCRIPTION_new","d2i_ACCESS_DESCRIPTION","i2d_ACCESS_DESCRIPTION","CRYPTO_free","ACCESS_DESCRIPTION_free","ACCESS_DESCRIPTION_free","ossl_check_void_sk_type","OPENSSL_sk_delete_ptr","UI_create_method","UI_method_set_ex_data","UI_destroy_method","SSL_free","SSL_CTX_free"],"critical_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","TLS_method","SSL_CTX_new","SSL_new","SSL_use_PrivateKey_file","OSSL_STORE_delete","OSSL_CRMF_MSG_new","OSSL_CRMF_MSG_get0_regCtrl_oldCertID","OSSL_CRMF_MSG_free","EVP_des_ede","EVP_aes_128_cbc_hmac_sha256","NOTICEREF_it","X509_TRUST_get_count","ACCESS_DESCRIPTION_new","d2i_ACCESS_DESCRIPTION","i2d_ACCESS_DESCRIPTION","CRYPTO_free","ACCESS_DESCRIPTION_free","ACCESS_DESCRIPTION_free","ossl_check_void_sk_type","OPENSSL_sk_delete_ptr","UI_create_method","UI_method_set_ex_data","UI_destroy_method","SSL_free","SSL_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
	return 0;
	}
	

    // Initialize OpenSSL
    OPENSSL_init_crypto(0, NULL);
    OPENSSL_init_ssl(0, NULL);

    // Create SSL context and SSL object
    const SSL_METHOD *meth = TLS_method();
    SSL_CTX *ctx = SSL_CTX_new(meth);
    SSL *ssl = SSL_new(ctx);

    // Write input data to a temporary file for SSL_use_PrivateKey_file
    FILE *key_file = fmemopen((void *)data, size, "rb");
    if (key_file) {
        // Create a temporary file name
        const char *key_filename = "input_file";
        FILE *tmp_key_file = fopen(key_filename, "wb");
        if (tmp_key_file) {
            fwrite(data, 1, size, tmp_key_file);
            assert_file_closed(&tmp_key_file);;
            
            // Use the private key file (even if it's not actually a valid key)
            SSL_use_PrivateKey_file(ssl, key_filename, SSL_FILETYPE_PEM);
            
            // Delete the temporary file using OSSL_STORE_delete
            OSSL_STORE_delete(key_filename, NULL, NULL, NULL, NULL, NULL);
        }
        assert_file_closed(&key_file);;
    }

    // Create and manipulate OSSL_CRMF_MSG
    OSSL_CRMF_MSG *crmf_msg = OSSL_CRMF_MSG_new();
    if (crmf_msg) {
        OSSL_CRMF_CERTID *certid = OSSL_CRMF_MSG_get0_regCtrl_oldCertID(crmf_msg);
        (void)certid; // May be NULL, that's fine
        OSSL_CRMF_MSG_free(crmf_msg);
    }

    // Get cipher references
    const EVP_CIPHER *cipher1 = EVP_des_ede();
    const EVP_CIPHER *cipher2 = EVP_aes_128_cbc_hmac_sha256();
    (void)cipher1;
    (void)cipher2;

    // Get NOTICEREF ASN1 item
    const ASN1_ITEM *notice_ref_item = NOTICEREF_it();
    (void)notice_ref_item;

    // Get X509 trust count
    int trust_count = X509_TRUST_get_count();
    (void)trust_count;

    // Create and manipulate ACCESS_DESCRIPTION
    ACCESS_DESCRIPTION *ad = ACCESS_DESCRIPTION_new();
    if (ad && size > 0) {
        const unsigned char *p = data;
        long len = size;
        
        // Try to decode ACCESS_DESCRIPTION from input
        ACCESS_DESCRIPTION *ad_decoded = d2i_ACCESS_DESCRIPTION(NULL, &p, len);
        
        if (ad_decoded) {
            // Encode it back
            unsigned char *out = NULL;
            int out_len = i2d_ACCESS_DESCRIPTION(ad_decoded, &out);
            if (out) {
                // Write encoded data to output file
                FILE *out_file = fopen("output_file", "wb");
                if (out_file) {
                    fwrite(out, 1, out_len, out_file);
                    assert_file_closed(&out_file);;
                }
                // Free the allocated buffer using CRYPTO_free
                CRYPTO_free(out, __FILE__, __LINE__);
            }
            ACCESS_DESCRIPTION_free(ad_decoded);
        }
        ACCESS_DESCRIPTION_free(ad);
    }

    // Stack manipulation (using void stack as placeholder)
    struct stack_st_void *void_stack = NULL;
    OPENSSL_STACK *ossl_stack = ossl_check_void_sk_type(void_stack);
    if (ossl_stack) {
        // Try to delete a pointer (won't actually delete since stack is empty)
        void *ptr = NULL;
        OPENSSL_sk_delete_ptr(ossl_stack, ptr);
    }

    // Set UI method ex_data (create minimal UI_METHOD first)
    UI_METHOD *ui_method = UI_create_method("FuzzerUI");
    if (ui_method) {
        UI_method_set_ex_data(ui_method, 0, NULL);
        UI_destroy_method(ui_method);
    }

    // Cleanup
    SSL_free(ssl);
    SSL_CTX_free(ctx);

    assert_file_closed(&key_file);
	return 0;
}