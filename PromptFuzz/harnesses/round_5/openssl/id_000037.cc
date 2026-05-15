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
//<ID> 339
//<Prompt> []
/*<Combination>: [
*/
//<score> 238, nr_unique_branch: 193
//<Quality> {"density":14,"unique_branches":{"ossl_prov_digest_load":[[101,9,101,38,0,0,4,1],[104,9,104,23,0,0,4,1],[106,9,106,52,0,0,4,1]],"evp_kdf_free":[[34,9,34,20,0,0,4,1],[38,9,38,16,0,0,4,0]],"evp_kdf_new":[[50,9,50,53,0,0,4,1],[51,12,51,44,0,0,4,1]],"ossl_prov_cipher_hw_aes_cbc":[[185,23,185,64,2,0,4,0]],"MD5_Update":[[214,9,214,14,0,0,4,0],[214,9,214,14,0,0,4,1],[222,9,222,17,0,0,4,0],[222,9,222,17,0,0,4,1],[23,26,23,52,2,0,4,1],[23,26,23,52,3,0,4,1],[22,24,22,50,4,0,4,1]],"MD5_Final":[[247,9,247,30,0,0,4,0],[247,9,247,30,0,0,4,1]],"evp_md_init_internal":[[208,9,208,28,0,0,4,1],[22,24,22,50,9,0,4,0],[23,26,23,52,10,0,4,1]],"pkcs12kdf_set_membuf":[[210,9,210,26,0,0,4,1],[213,16,213,31,0,0,4,0],[214,13,214,72,0,0,4,1]],"PKCS12_key_gen_uni_ex":[[89,9,89,15,0,0,4,1],[93,9,93,20,0,0,4,1],[97,9,97,20,0,0,4,1],[123,9,123,52,0,0,4,0]],"kdf_pkcs12_set_ctx_params":[[250,9,250,20,0,0,4,1],[250,24,250,66,0,0,4,1],[255,9,255,73,0,0,4,1],[258,9,258,21,0,0,4,0],[258,25,258,80,0,0,4,1],[261,9,261,23,0,0,4,0],[262,12,262,69,0,0,4,1],[265,9,265,24,0,0,4,0],[265,28,265,66,0,0,4,1],[268,9,268,23,0,0,4,0],[268,27,268,69,0,0,4,1],[276,9,276,53,0,0,4,0],[276,57,276,71,0,0,4,0],[276,75,276,90,0,0,4,0]],"pkcs12kdf_derive":[[64,9,64,20,0,0,4,1],[70,9,70,16,0,0,4,1],[70,20,70,27,0,0,4,1],[80,9,80,21,0,0,4,0],[86,9,86,18,0,0,4,1],[86,22,86,32,0,0,4,1],[86,36,86,45,0,0,4,1],[86,49,86,58,0,0,4,1],[88,17,88,22,0,0,4,0],[88,17,88,22,0,0,4,1],[91,17,91,25,0,0,4,0],[91,17,91,25,0,0,4,1],[93,17,93,25,0,0,4,0],[93,17,93,25,0,0,4,1],[96,13,96,51,0,0,4,1],[97,16,97,44,0,0,4,1],[98,16,98,47,0,0,4,1],[99,16,99,50,0,0,4,1],[101,28,101,43,0,0,4,0],[101,28,101,43,0,0,4,1],[102,17,102,55,0,0,4,1],[103,20,103,49,0,0,4,1],[104,20,104,54,0,0,4,1],[107,25,107,30,0,0,4,1],[108,13,108,19,0,0,4,0],[108,13,108,19,0,0,4,1],[114,21,114,26,0,0,4,0],[114,21,114,26,0,0,4,1],[116,21,116,29,0,0,4,0],[116,21,116,29,0,0,4,1],[121,25,121,30,0,0,4,0],[121,25,121,30,0,0,4,1]],"cipher_hw_aesni_initkey":[[27,44,27,74,0,0,4,0],[36,13,36,43,0,0,4,0]],"evp_kdf_from_algorithm":[[66,9,66,38,0,0,4,1],[71,9,71,75,0,0,4,1],[76,12,76,33,0,0,4,0],[76,12,76,33,0,0,4,1],[77,17,77,33,0,0,4,1],[78,9,78,34,0,0,4,0],[78,9,78,34,0,0,4,1],[79,17,79,36,0,0,4,1],[84,9,84,34,0,0,4,0],[84,9,84,34,0,0,4,1],[85,17,85,36,0,0,4,1],[89,9,89,35,0,0,4,0],[89,9,89,35,0,0,4,1],[90,17,90,37,0,0,4,1],[95,9,95,33,0,0,4,0],[95,9,95,33,0,0,4,1],[96,17,96,35,0,0,4,1],[100,9,100,34,0,0,4,0],[100,9,100,34,0,0,4,1],[101,17,101,36,0,0,4,1],[106,9,106,43,0,0,4,1],[111,9,111,47,0,0,4,0],[111,9,111,47,0,0,4,1],[112,17,112,49,0,0,4,1],[116,9,116,47,0,0,4,0],[116,9,116,47,0,0,4,1],[117,17,117,49,0,0,4,1],[121,9,121,38,0,0,4,1],[126,9,126,42,0,0,4,0],[126,9,126,42,0,0,4,1],[127,17,127,44,0,0,4,1],[131,9,131,42,0,0,4,0],[131,9,131,42,0,0,4,1],[132,17,132,44,0,0,4,1],[136,9,136,36,0,0,4,1],[141,9,141,39,0,0,4,1],[148,9,148,22,0,0,4,1],[148,26,148,39,0,0,4,1],[157,9,157,21,0,0,4,0],[157,25,157,52,0,0,4,1]],"kdf_pkcs12_free":[[165,9,165,20,0,0,4,0]],"enc_new":[[73,9,73,53,0,0,4,1],[77,9,77,28,0,0,4,1]],"OSSL_PARAM_BLD_push_octet_string":[[368,9,368,20,0,0,4,1],[368,24,368,35,0,0,4,1],[368,39,368,50,0,0,4,1],[375,9,375,19,0,0,4,1]],"param_bld_convert":[[431,20,431,55,0,0,4,0],[437,17,437,51,0,0,4,1]],"OSSL_PARAM_get_uint64":[[884,16,884,50,0,0,4,0],[889,17,889,29,0,0,4,1],[890,9,890,29,0,0,4,0],[892,17,892,25,0,0,4,0],[898,9,898,29,0,0,4,1],[22,24,22,50,3,0,4,1]],"set_propq":[[39,9,39,22,0,0,4,1]],"EVP_KDF_derive":[[142,9,142,20,0,0,4,1]],"MD5":[[30,9,30,19,0,0,4,1],[32,9,32,22,0,0,4,1]],"ossl_i2c_ASN1_BIT_STRING":[[55,9,55,18,0,0,4,1],[65,9,65,16,0,0,4,1]],"get_string_internal":[[1350,22,1350,52,0,0,4,1],[1350,56,1350,63,0,0,4,1],[1352,9,1352,25,0,0,4,0]],"pkcs12_set_ctx_params_decoder":[[43,9,43,18,0,0,4,0],[44,16,44,36,0,0,4,0],[44,16,44,36,0,0,4,1],[46,13,46,20,0,0,4,1],[48,13,48,21,0,0,4,0],[48,13,48,21,0,0,4,1],[59,13,59,21,0,0,4,0],[59,13,59,21,0,0,4,1],[61,17,61,24,0,0,4,1],[63,17,63,25,0,0,4,0],[63,17,63,25,0,0,4,1],[65,21,65,28,0,0,4,1],[67,21,67,30,0,0,4,0],[76,17,76,25,0,0,4,0],[76,17,76,25,0,0,4,1],[88,13,88,21,0,0,4,0],[88,13,88,21,0,0,4,1],[90,17,90,24,0,0,4,1],[92,17,92,25,0,0,4,0],[103,17,103,25,0,0,4,1],[115,13,115,21,0,0,4,0],[115,13,115,21,0,0,4,1],[22,24,22,50,1,0,4,0],[23,26,23,52,2,0,4,1],[23,26,23,52,6,0,4,1],[22,24,22,50,10,0,4,0],[23,26,23,52,11,0,4,1],[22,24,22,50,15,0,4,0],[23,26,23,52,16,0,4,1],[22,24,22,50,25,0,4,0],[23,26,23,52,26,0,4,1]],"EVP_KDF_CTX_free":[[48,9,48,20,0,0,4,1]],"aes_128_cbc_newctx":[[223,13,223,24,0,0,4,0],[205,9,205,32,1,0,4,1]],"kdf_pkcs12_derive":[[226,9,226,32,0,0,4,1],[226,36,226,75,0,0,4,1],[229,9,229,26,0,0,4,1],[234,9,234,26,0,0,4,1]],"enc_free":[[94,9,94,18,0,0,4,1],[98,9,98,18,0,0,4,1]],"EVP_KDF_CTX_new":[[28,9,28,20,0,0,4,1],[32,9,32,20,0,0,4,1],[33,12,33,77,0,0,4,1],[34,12,34,32,0,0,4,1]],"BIO_set_cipher":[[433,9,433,20,0,0,4,1],[436,9,436,55,0,0,4,1],[446,13,446,31,0,0,4,1],[453,9,453,58,0,0,4,1],[456,9,456,28,0,0,4,1],[460,14,460,30,0,0,4,1]],"EVP_MD_get_block_size":[[784,9,784,19,0,0,4,1]],"deflt_query":[[751,5,751,21,0,0,4,0]],"kdf_pkcs12_new":[[143,9,143,32,0,0,4,1],[147,9,147,20,0,0,4,1]]},"library_calls":["d2i_CRL_DIST_POINTS","DH_get_default_method","DH_meth_get_generate_key","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_UI_STRING_sk_type","OPENSSL_sk_free","ossl_check_UI_STRING_sk_type","PKCS7_ISSUER_AND_SERIAL_new","i2d_PKCS7_ISSUER_AND_SERIAL","CRYPTO_free","PKCS7_ISSUER_AND_SERIAL_free","PKCS12_key_gen_uni_ex","EVP_sha256","OSSL_FUNC_keymgmt_import","OSSL_PARAM_BLD_new","OSSL_PARAM_BLD_push_octet_string","OSSL_PARAM_BLD_to_param","OSSL_PARAM_free","OSSL_PARAM_BLD_free","BIO_new","BIO_f_cipher","EVP_aes_128_cbc","BIO_set_cipher","BIO_free","BIO_free","MD5","CRL_DIST_POINTS_free"],"critical_calls":["d2i_CRL_DIST_POINTS","DH_get_default_method","DH_meth_get_generate_key","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_UI_STRING_sk_type","OPENSSL_sk_free","ossl_check_UI_STRING_sk_type","PKCS7_ISSUER_AND_SERIAL_new","i2d_PKCS7_ISSUER_AND_SERIAL","CRYPTO_free","PKCS7_ISSUER_AND_SERIAL_free","PKCS12_key_gen_uni_ex","EVP_sha256","OSSL_FUNC_keymgmt_import","OSSL_PARAM_BLD_new","OSSL_PARAM_BLD_push_octet_string","OSSL_PARAM_BLD_to_param","OSSL_PARAM_free","OSSL_PARAM_BLD_free","BIO_new","BIO_f_cipher","EVP_aes_128_cbc","BIO_set_cipher","BIO_free","MD5","CRL_DIST_POINTS_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse CRL distribution points from input data
    const unsigned char *p = data;
    CRL_DIST_POINTS *crldp = d2i_CRL_DIST_POINTS(NULL, &p, size);
    if (!crldp) {
        // Input might not be valid CRL_DIST_POINTS data, skip further processing
        return 0;
    }
    
    // 2. Get DH method generate_key function pointer
    const DH_METHOD *dhm_default = DH_get_default_method();
    if (dhm_default) {
        DH_meth_get_generate_key(dhm_default);
    }
    
    // 3. Create UI string stack and convert type
    struct stack_st_UI_STRING *ui_stack = sk_UI_STRING_new_null();
    if (ui_stack) {
        OPENSSL_STACK *ossl_stack = ossl_check_UI_STRING_sk_type(ui_stack);
        sk_UI_STRING_free(ui_stack);
    }
    
    // 4. Create PKCS7_ISSUER_AND_SERIAL and serialize it
    PKCS7_ISSUER_AND_SERIAL *ias = PKCS7_ISSUER_AND_SERIAL_new();
    if (ias) {
        unsigned char *out = NULL;
        int len = i2d_PKCS7_ISSUER_AND_SERIAL(ias, &out);
        OPENSSL_free(out);
        PKCS7_ISSUER_AND_SERIAL_free(ias);
    }
    
    // 5. Generate PKCS12 key using input data as password/salt
    unsigned char out_key[64];
    if (size >= 16) {
        // Use first 8 bytes as password, next 8 bytes as salt
        unsigned char *pass = (unsigned char *)data;
        unsigned char *salt = (unsigned char *)data + 8;
        int passlen = size > 8 ? 8 : size;
        int saltlen = size > 16 ? 8 : (size > 8 ? size - 8 : 0);
        
        PKCS12_key_gen_uni_ex(pass, passlen, salt, saltlen, 
                             1, 2048, 64, out_key, 
                             EVP_sha256(), NULL, NULL);
    }
    
    // 6. Get keymgmt import function from dummy dispatch table
    OSSL_DISPATCH dummy_dispatch[] = {
        {0, NULL}
    };
    OSSL_FUNC_keymgmt_import_fn *import_fn = OSSL_FUNC_keymgmt_import(dummy_dispatch);
    
    // 7. Build OSSL parameters
    OSSL_PARAM_BLD *bld = OSSL_PARAM_BLD_new();
    if (bld) {
        // Add some sample parameters
        if (size > 0) {
            OSSL_PARAM_BLD_push_octet_string(bld, "data", data, 
                                           size > 1024 ? 1024 : size);
        }
        
        OSSL_PARAM *params = OSSL_PARAM_BLD_to_param(bld);
        OSSL_PARAM_free(params);
        OSSL_PARAM_BLD_free(bld);
    }
    
    // 8. Create BIO with cipher using input data as key/IV
    BIO *bio = BIO_new(BIO_f_cipher());
    if (bio && size >= 32) {
        // Use first 16 bytes as key, next 16 bytes as IV
        const unsigned char *key = data;
        const unsigned char *iv = data + 16;
        const EVP_CIPHER *cipher = EVP_aes_128_cbc();
        
        BIO_set_cipher(bio, cipher, key, iv, 1);
        BIO_free(bio);
    } else if (bio) {
        BIO_free(bio);
    }
    
    // 9. Compute MD5 hash of input data
    unsigned char md5_out[MD5_DIGEST_LENGTH];
    if (size > 0) {
        MD5(data, size, md5_out);
    }
    
    // Clean up
    CRL_DIST_POINTS_free(crldp);
    
    return 0;
}