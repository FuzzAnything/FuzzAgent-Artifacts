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
//<ID> 215
//<Prompt> []
/*<Combination>: [
*/
//<score> 122.5, nr_unique_branch: 207
//<Quality> {"density":5,"unique_branches":{"CMS_dataInit":[[162,5,162,26,0,0,4,1],[175,5,175,29,0,0,4,0]],"get_error_values":[[482,9,482,20,0,0,4,0],[494,9,494,21,0,0,4,0],[496,13,496,26,0,0,4,1],[499,9,499,22,0,0,4,0],[501,9,501,21,0,0,4,1],[507,13,507,26,0,0,4,0],[507,13,507,26,0,0,4,1],[509,17,509,30,0,0,4,0]],"X509_OBJECT_new":[[639,9,639,20,0,0,4,1]],"ossl_asn1_do_lock":[[105,13,105,21,0,0,4,1]],"ossl_cms_EncryptedContent_init_bio":[[44,11,44,21,0,0,4,0],[47,9,47,18,0,0,4,1],[55,9,55,12,0,0,4,0],[60,13,60,28,0,0,4,0],[65,9,65,23,0,0,4,0],[68,13,68,33,0,0,4,0],[71,9,71,23,0,0,4,1],[78,9,78,67,0,0,4,1],[83,9,83,12,0,0,4,0],[85,13,85,36,0,0,4,1],[85,40,85,73,0,0,4,0],[203,9,203,18,0,0,4,0],[208,9,208,11,0,0,4,1]],"x509_verify_x509":[[352,9,352,26,0,0,4,1],[358,9,358,27,0,0,4,1],[368,9,368,78,0,0,4,1],[385,9,385,17,0,0,4,0],[385,21,385,44,0,0,4,1],[177,9,177,15,7,0,4,1],[80,6,80,20,8,0,4,1]],"OSSL_CMP_validate_cert_path":[[107,9,107,20,0,0,4,1],[107,24,107,36,0,0,4,1],[112,9,112,30,0,0,4,1],[117,9,117,71,0,0,4,1],[118,12,119,34,0,0,4,1],[126,9,126,15,0,0,4,0],[126,19,126,79,0,0,4,0]],"enc_new":[[73,9,73,53,0,0,4,1],[77,9,77,28,0,0,4,1]],"X509_STORE_CTX_get1_issuer":[[464,9,464,20,0,0,4,1],[467,9,467,17,0,0,4,0]],"X509_verify_cert":[[309,9,309,20,0,0,4,1],[313,12,313,30,0,0,4,1]],"cms_ec_cb":[[89,9,89,39,0,0,4,0],[89,9,89,39,0,0,4,1]],"EVP_CIPHER_get0_name":[[679,9,679,34,0,0,4,1]],"CMS_get0_content":[[276,5,276,26,0,0,4,1],[285,5,285,29,0,0,4,0]],"X509_PUBKEY_get0":[[447,9,447,20,0,0,4,1],[452,9,452,26,0,0,4,0]],"general_allocate_prompt":[[106,12,106,30,0,0,4,0]],"ossl_cms_EncryptedData_init_bio":[[269,9,269,42,0,0,4,0],[269,46,269,67,0,0,4,1]],"UI_get_result_length":[[451,9,451,14,0,0,4,1],[455,9,455,30,0,0,4,0]],"x509_object_free_internal":[[647,9,647,18,0,0,4,1],[649,13,649,20,0,0,4,1],[650,5,650,22,0,0,4,0],[652,5,652,22,0,0,4,1],[655,5,655,21,0,0,4,1]],"OPENSSL_sk_insert":[[276,9,276,25,0,0,4,1],[276,29,276,38,0,0,4,0]],"X509_add_cert":[[192,9,192,19,0,0,4,1],[196,9,196,21,0,0,4,1],[198,9,198,44,0,0,4,1],[210,9,210,43,0,0,4,1],[220,9,220,44,0,0,4,0],[220,48,220,74,0,0,4,1],[222,9,222,24,0,0,4,1],[97,113,97,116,7,0,4,1]],"build_chain":[[3665,9,3665,21,0,0,4,1],[3679,14,3679,36,0,0,4,0],[3681,13,3681,24,0,0,4,1],[3681,28,3681,80,0,0,4,0],[3689,9,3689,52,0,0,4,1],[3709,9,3709,77,0,0,4,1],[3718,9,3718,40,0,0,4,1],[3728,12,3728,23,0,0,4,0],[3747,13,3747,40,0,0,4,0],[3749,17,3749,46,0,0,4,1],[3770,18,3770,33,0,0,4,1],[3772,17,3772,23,0,0,4,1],[3778,17,3778,23,0,0,4,1],[3880,17,3880,46,0,0,4,0],[3880,17,3880,46,0,0,4,1],[3882,21,3882,50,0,0,4,1],[3885,21,3885,35,0,0,4,0],[3896,13,3896,42,0,0,4,0],[3898,17,3898,29,0,0,4,1],[3901,23,3901,52,0,0,4,1],[3901,56,3901,71,0,0,4,1],[3902,17,3902,31,0,0,4,0],[3909,21,3909,32,0,0,4,0],[3931,9,3931,18,0,0,4,1],[3939,9,3939,25,0,0,4,0],[3940,13,3940,42,0,0,4,0],[3942,13,3942,42,0,0,4,0],[3942,46,3942,71,0,0,4,0],[3947,5,3947,28,0,0,4,1],[3949,5,3949,29,0,0,4,1],[3952,5,3952,30,0,0,4,0],[3953,5,3953,12,0,0,4,1],[3955,9,3955,55,0,0,4,1],[3956,9,3956,43,0,0,4,1],[3957,9,3957,54,0,0,4,1],[3958,9,3958,41,0,0,4,1],[3960,9,3960,16,0,0,4,1],[3962,9,3962,23,0,0,4,0],[3970,13,3970,72,0,0,4,1],[3976,13,3976,37,0,0,4,1],[52,41,52,42,4,0,4,0],[52,41,52,42,4,0,4,0],[94,33,94,39,6,0,4,1],[95,33,95,39,7,0,4,1],[80,6,80,20,13,0,4,1],[101,36,101,42,52,0,4,1],[177,9,177,15,62,0,4,1],[177,10,177,14,63,0,4,1]],"ossl_x509_add_certs_new":[[248,17,248,22,0,0,4,1]],"ossl_x509_store_ctx_get_by_subject":[[446,9,446,22,0,0,4,1],[452,9,452,42,0,0,4,1],[454,9,454,31,0,0,4,0],[456,13,456,25,0,0,4,1],[463,9,463,20,0,0,4,0],[464,21,464,43,0,0,4,1],[477,13,477,24,0,0,4,0]],"ossl_x509_store_ht_get_by_name":[[350,9,350,30,0,0,4,0],[352,13,352,20,0,0,4,1],[360,9,360,18,0,0,4,0]],"ossl_cms_EncryptedContent_init":[[220,9,220,12,0,0,4,0],[221,13,221,55,0,0,4,1],[226,9,226,23,0,0,4,0]],"general_allocate_string":[[128,9,128,18,0,0,4,1]],"OSSL_CMP_print_to_bio":[[129,32,129,59,0,0,4,1],[129,72,129,99,0,0,4,1],[130,11,130,37,0,0,4,1],[131,11,131,36,0,0,4,0],[139,9,141,12,0,0,4,1]],"X509_get0_pubkey":[[383,9,383,18,0,0,4,1]],"ASN1_SCTX_new":[[29,9,29,20,0,0,4,1]],"CMS_EncryptedData_encrypt_ex":[[231,9,231,23,0,0,4,1],[236,9,236,20,0,0,4,1],[238,9,238,62,0,0,4,1],[241,9,241,32,0,0,4,0],[244,9,244,45,0,0,4,1],[245,12,245,43,0,0,4,1]],"aes_192_ctr_newctx":[[223,13,223,24,0,0,4,0],[205,9,205,32,1,0,4,1]],"ERR_lib_error_string":[[586,9,586,18,0,0,4,1],[593,13,593,24,0,0,4,1],[130,6,130,48,1,0,4,0]],"ossl_x509_add_cert_new":[[183,9,183,22,0,0,4,0],[183,26,183,62,0,0,4,1]],"CMS_EncryptedData_set1_key":[[236,9,236,13,0,0,4,1],[236,17,236,24,0,0,4,1],[240,9,240,13,0,0,4,0],[241,13,241,74,0,0,4,1],[245,13,245,41,0,0,4,1],[250,13,250,34,0,0,4,1]],"get0_best_issuer_sk":[[420,17,420,32,0,0,4,1]],"X509_add_certs":[[235,9,235,19,0,0,4,1]],"improve_location_name":[[118,9,118,25,0,0,4,1],[121,12,121,24,0,0,4,1],[121,28,121,41,0,0,4,1],[121,45,121,76,0,0,4,1]],"check_key_level":[[4057,9,4057,19,0,0,4,0]],"OSSL_CMP_CTX_print_errors":[[424,9,424,20,0,0,4,0],[424,24,424,61,0,0,4,1],[426,30,426,41,0,0,4,1]],"OSSL_CMP_print_errors_cb":[[158,12,158,78,0,0,4,0],[158,12,158,78,0,0,4,1],[172,13,172,23,0,0,4,1],[176,13,176,25,0,0,4,0],[176,29,176,58,0,0,4,0],[176,29,176,58,0,0,4,1],[181,13,181,27,0,0,4,0],[185,17,185,28,0,0,4,0],[222,35,222,71,1,0,4,1]],"X509_self_signed":[[106,9,106,48,0,0,4,0]],"verify_cb_cert":[[166,9,166,18,0,0,4,1],[170,25,170,34,0,0,4,1],[171,9,171,25,0,0,4,0]],"ossl_asn1_item_embed_free":[[88,13,88,49,0,0,4,0],[477,12,477,15,11,0,4,0]],"check_trust":[[1101,29,1101,36,0,0,4,1],[1115,9,1115,28,0,0,4,1],[1121,9,1121,29,0,0,4,0],[1122,12,1122,64,0,0,4,1],[96,31,96,37,2,0,4,1]],"X509_up_ref":[[120,9,120,47,0,0,4,1],[293,12,293,18,2,0,4,1]],"enc_free":[[94,9,94,18,0,0,4,1],[98,9,98,18,0,0,4,1]],"enc_ctrl":[[318,9,318,20,0,0,4,1],[322,5,322,24,0,0,4,1],[330,5,330,22,0,0,4,1],[340,5,340,27,0,0,4,1],[345,5,345,26,0,0,4,1],[350,5,350,24,0,0,4,1],[383,5,383,33,0,0,4,1],[386,5,386,32,0,0,4,1],[391,5,391,30,0,0,4,0],[396,5,396,22,0,0,4,1],[406,5,406,12,0,0,4,1]],"verify_chain":[[260,9,260,37,0,0,4,0]]},"library_calls":["d2i_PBKDF2PARAM","PBKDF2PARAM_free","DH_new","DH_get0_p","DH_free","EVP_aes_192_ctr","UI_new","UI_add_input_string","UI_get_result_length","UI_process","UI_free","CMS_EncryptedData_encrypt_ex","EVP_aes_192_ctr","CMS_get0_RecipientInfos","CMS_ContentInfo_free","ASN1_SCTX_new","ASN1_SCTX_get_flags","ASN1_SCTX_free","OSSL_CMP_CTX_new","X509_STORE_new","X509_new","OSSL_CMP_validate_cert_path","X509_free","OSSL_CMP_CTX_free","X509_STORE_free"],"critical_calls":["d2i_PBKDF2PARAM","PBKDF2PARAM_free","DH_new","DH_get0_p","DH_free","EVP_aes_192_ctr","UI_new","UI_add_input_string","UI_get_result_length","UI_process","UI_free","CMS_EncryptedData_encrypt_ex","EVP_aes_192_ctr","CMS_get0_RecipientInfos","CMS_ContentInfo_free","ASN1_SCTX_new","ASN1_SCTX_get_flags","ASN1_SCTX_free","OSSL_CMP_CTX_new","X509_STORE_new","X509_new","OSSL_CMP_validate_cert_path","X509_free","OSSL_CMP_CTX_free","X509_STORE_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse PBKDF2 parameters from input data
    const unsigned char *p = data;
    PBKDF2PARAM *pbkdf2 = d2i_PBKDF2PARAM(NULL, &p, (long)size);
    
    if (pbkdf2 != NULL) {
        // Clean up PBKDF2 parameters
        PBKDF2PARAM_free(pbkdf2);
    }
    
    // 2. Create a DH object and get its prime parameter
    DH *dh = DH_new();
    if (dh != NULL) {
        const BIGNUM *p_bn = DH_get0_p(dh);
        // Use the prime parameter if available
        (void)p_bn;
        DH_free(dh);
    }
    
    // 3. Get AES-192-CTR cipher for potential encryption
    const EVP_CIPHER *cipher = EVP_aes_192_ctr();
    (void)cipher;
    
    // 4. Create UI object for password input simulation
    UI *ui = UI_new();
    if (ui != NULL) {
        // Add a dummy password prompt
        UI_add_input_string(ui, "Password:", 0, NULL, 0, 100);
        int length = UI_get_result_length(ui, 0);
        (void)length; // Use the result length
        
        // Process UI (simulated - no actual user interaction)
        UI_process(ui);
        UI_free(ui);
    }
    
    // 5. Create CMS ContentInfo with encrypted data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file != NULL) {
        CMS_ContentInfo *cms = CMS_EncryptedData_encrypt_ex(
            NULL,  // Using NULL as BIO since we're creating structure
            EVP_aes_192_ctr(),
            (const unsigned char *)"key",  // Dummy key
            3,  // Key length
            0,  // Flags
            NULL,  // Default library context
            NULL   // Default property query
        );
        
        if (cms != NULL) {
            // Get recipient information from CMS
            struct stack_st_CMS_RecipientInfo *recip_infos = 
                CMS_get0_RecipientInfos(cms);
            (void)recip_infos; // Use the recipient infos
            
            CMS_ContentInfo_free(cms);
        }
        assert_file_closed(&in_file);;
    }
    
    // 6. Create ASN1 parsing context and get flags
    ASN1_SCTX *asn1_ctx = ASN1_SCTX_new(NULL);
    if (asn1_ctx != NULL) {
        unsigned long flags = ASN1_SCTX_get_flags(asn1_ctx);
        (void)flags; // Use the flags
        ASN1_SCTX_free(asn1_ctx);
    }
    
    // 7. Create CMP context and validate certificate path
    OSSL_CMP_CTX *cmp_ctx = OSSL_CMP_CTX_new(NULL, NULL);
    X509_STORE *store = X509_STORE_new();
    
    if (cmp_ctx != NULL && store != NULL) {
        // Create a dummy certificate for validation
        X509 *cert = X509_new();
        if (cert != NULL) {
            // Validate certificate path (will likely fail with dummy data)
            int result = OSSL_CMP_validate_cert_path(cmp_ctx, store, cert);
            (void)result; // Use the validation result
            
            X509_free(cert);
        }
    }
    
    // Clean up CMP context and store
    if (cmp_ctx != NULL) {
        OSSL_CMP_CTX_free(cmp_ctx);
    }
    if (store != NULL) {
        X509_STORE_free(store);
    }
    
    assert_file_closed(&in_file);
	return 0;
}