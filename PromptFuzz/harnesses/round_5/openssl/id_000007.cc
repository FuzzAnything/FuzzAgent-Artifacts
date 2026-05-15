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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 152
//<Quality> {"density":3,"unique_branches":{"X509_verify_cert_error_string":[[24,5,24,19,0,0,4,0],[24,5,24,19,0,0,4,1],[26,5,26,32,0,0,4,0],[26,5,26,32,0,0,4,1],[28,5,28,46,0,0,4,0],[28,5,28,46,0,0,4,1],[30,5,30,38,0,0,4,1],[32,5,32,53,0,0,4,0],[32,5,32,53,0,0,4,1],[34,5,34,52,0,0,4,1],[36,5,36,55,0,0,4,1],[38,5,38,43,0,0,4,1],[40,5,40,42,0,0,4,1],[42,5,42,39,0,0,4,1],[44,5,44,37,0,0,4,0],[44,5,44,37,0,0,4,1],[46,5,46,38,0,0,4,0],[46,5,46,38,0,0,4,1],[48,5,48,36,0,0,4,1],[50,5,50,51,0,0,4,0],[50,5,50,51,0,0,4,1],[52,5,52,50,0,0,4,0],[52,5,52,50,0,0,4,1],[54,5,54,51,0,0,4,1],[56,5,56,51,0,0,4,0],[56,5,56,51,0,0,4,1],[58,5,58,31,0,0,4,1],[60,5,60,48,0,0,4,1],[62,5,62,46,0,0,4,1],[64,5,64,54,0,0,4,1],[66,5,66,52,0,0,4,1],[68,5,68,40,0,0,4,1],[70,5,70,33,0,0,4,1],[72,5,72,41,0,0,4,1],[74,5,74,41,0,0,4,1],[76,5,76,36,0,0,4,1],[78,5,78,35,0,0,4,1],[80,5,80,34,0,0,4,1],[82,5,82,44,0,0,4,1],[84,5,84,39,0,0,4,1],[86,5,86,48,0,0,4,1],[88,5,88,41,0,0,4,1],[90,5,90,45,0,0,4,1],[92,5,92,49,0,0,4,1],[94,5,94,41,0,0,4,0],[94,5,94,41,0,0,4,1],[96,5,96,53,0,0,4,1],[98,5,98,35,0,0,4,1],[100,5,100,47,0,0,4,1],[102,5,102,50,0,0,4,1],[104,5,104,51,0,0,4,0],[104,5,104,51,0,0,4,1],[106,5,106,38,0,0,4,1],[108,5,108,45,0,0,4,1],[110,5,110,39,0,0,4,1],[112,5,112,40,0,0,4,1],[114,5,114,50,0,0,4,1],[116,5,116,38,0,0,4,0],[116,5,116,38,0,0,4,1],[118,5,118,40,0,0,4,1],[120,5,120,39,0,0,4,0],[120,5,120,39,0,0,4,1],[122,5,122,35,0,0,4,0],[122,5,122,35,0,0,4,1],[124,5,124,45,0,0,4,1],[126,5,126,48,0,0,4,0],[126,5,126,48,0,0,4,1],[128,5,128,50,0,0,4,1],[130,5,130,44,0,0,4,0],[130,5,130,44,0,0,4,1],[132,5,132,46,0,0,4,1],[134,5,134,30,0,0,4,1],[136,5,136,44,0,0,4,1],[138,5,138,46,0,0,4,1],[140,5,140,42,0,0,4,1],[142,5,142,56,0,0,4,1],[144,5,144,44,0,0,4,1],[146,5,146,57,0,0,4,1],[148,5,148,38,0,0,4,1],[150,5,150,35,0,0,4,1],[152,5,152,40,0,0,4,0],[152,5,152,40,0,0,4,1],[154,5,154,34,0,0,4,1],[156,5,156,37,0,0,4,0],[156,5,156,37,0,0,4,1],[158,5,158,37,0,0,4,1],[160,5,160,35,0,0,4,0],[160,5,160,35,0,0,4,1],[162,5,162,33,0,0,4,1],[164,5,164,33,0,0,4,1],[166,5,166,34,0,0,4,0],[166,5,166,34,0,0,4,1],[168,5,168,49,0,0,4,1],[170,5,170,39,0,0,4,1],[172,5,172,39,0,0,4,1],[174,5,174,38,0,0,4,1],[176,5,176,38,0,0,4,1],[178,5,178,43,0,0,4,1],[180,5,180,39,0,0,4,1],[182,5,182,37,0,0,4,0],[182,5,182,37,0,0,4,1],[184,5,184,37,0,0,4,1],[186,5,186,52,0,0,4,1],[188,5,188,49,0,0,4,1],[190,5,190,54,0,0,4,1],[192,5,192,31,0,0,4,0],[192,5,192,31,0,0,4,1],[194,5,194,47,0,0,4,0],[194,5,194,47,0,0,4,1],[196,5,196,53,0,0,4,1],[198,5,198,56,0,0,4,1],[200,5,200,38,0,0,4,1],[202,5,202,39,0,0,4,0],[202,5,202,39,0,0,4,1],[204,5,204,53,0,0,4,0],[204,5,204,53,0,0,4,1],[206,5,206,51,0,0,4,1],[208,5,208,43,0,0,4,1],[210,5,210,42,0,0,4,1],[212,5,212,51,0,0,4,1],[214,5,214,54,0,0,4,1],[216,5,216,52,0,0,4,0],[216,5,216,52,0,0,4,1],[218,5,218,46,0,0,4,1],[220,5,220,49,0,0,4,1],[222,5,222,43,0,0,4,1],[224,5,224,34,0,0,4,1],[226,5,226,51,0,0,4,1],[228,5,228,50,0,0,4,0],[228,5,228,50,0,0,4,1],[230,5,230,40,0,0,4,1],[238,5,238,12,0,0,4,0],[238,5,238,12,0,0,4,1]],"UI_create_method":[[606,9,606,65,0,0,4,1],[607,12,607,60,0,0,4,1],[608,12,609,33,0,0,4,1]],"asn1_item_embed_new":[[89,13,89,26,0,0,4,0],[90,17,90,71,0,0,4,1]],"UI_method_get_data_destructor":[[769,9,769,23,0,0,4,0]],"cms_get0_econtent_type":[[315,5,315,26,0,0,4,1],[318,5,318,29,0,0,4,1],[321,5,321,26,0,0,4,1],[324,5,324,29,0,0,4,1],[327,5,327,43,0,0,4,1],[330,5,330,34,0,0,4,1],[333,5,333,40,0,0,4,1],[336,5,336,12,0,0,4,0]],"CMS_get0_eContentType":[[346,9,346,15,0,0,4,1]],"UI_destroy_method":[[633,9,633,26,0,0,4,1]],"evp_pkey_ctx_set_uint64":[[954,9,954,20,0,0,4,1],[954,24,954,50,0,0,4,0]],"int_ctx_new":[[136,17,136,36,0,0,4,1]]},"library_calls":["d2i_OSSL_INFO_SYNTAX_POINTER","CMS_ContentInfo_new","CMS_get0_eContentType","EVP_PKEY_CTX_new_id","EVP_PKEY_CTX_set_scrypt_r","EVP_PKEY_CTX_free","EVP_des_ede","X509_verify_cert_error_string","UI_create_method","UI_method_get_data_destructor","UI_destroy_method","OSSL_INFO_SYNTAX_POINTER_free","CMS_ContentInfo_free"],"critical_calls":["d2i_OSSL_INFO_SYNTAX_POINTER","CMS_ContentInfo_new","CMS_get0_eContentType","EVP_PKEY_CTX_new_id","EVP_PKEY_CTX_set_scrypt_r","EVP_PKEY_CTX_free","EVP_des_ede","X509_verify_cert_error_string","UI_create_method","UI_method_get_data_destructor","UI_destroy_method","OSSL_INFO_SYNTAX_POINTER_free","CMS_ContentInfo_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse OSSL_INFO_SYNTAX_POINTER from input data
    const unsigned char *p = data;
    OSSL_INFO_SYNTAX_POINTER *info_ptr = d2i_OSSL_INFO_SYNTAX_POINTER(nullptr, &p, static_cast<long>(size));
    
    // 2. Create a CMS_ContentInfo structure (dummy for API usage)
    CMS_ContentInfo *cms = CMS_ContentInfo_new();
    if (cms) {
        // Get eContentType from CMS
        const ASN1_OBJECT *content_type = CMS_get0_eContentType(cms);
        (void)content_type; // Use to avoid unused variable warning
    }
    
    // 3. Set up EVP_PKEY_CTX for scrypt
    EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_SCRYPT, nullptr);
    if (pctx) {
        // Set scrypt r parameter using data from input
        uint64_t r = 0;
        if (size >= sizeof(r)) {
            memcpy(&r, data, sizeof(r));
        }
        EVP_PKEY_CTX_set_scrypt_r(pctx, r);
        EVP_PKEY_CTX_free(pctx);
    }
    
    // 4. Get DES-EDE cipher
    const EVP_CIPHER *des_ede_cipher = EVP_des_ede();
    (void)des_ede_cipher; // Use to avoid unused variable warning
    
    // 5. Get certificate verification error string
    long error_code = 0;
    if (size > 0) {
        error_code = static_cast<long>(data[0]);
    }
    const char *error_str = X509_verify_cert_error_string(error_code);
    (void)error_str; // Use to avoid unused variable warning
    
    // 6. Create UI_METHOD and get data destructor
    UI_METHOD *ui_method = UI_create_method("FuzzMethod");
    if (ui_method) {
        // Get data destructor (returns function pointer, cast to void to use)
        void (*destructor)(void*) = reinterpret_cast<void(*)(void*)>(
            const_cast<void*>(reinterpret_cast<const void*>(
                UI_method_get_data_destructor(ui_method))));
        (void)destructor; // Use to avoid unused variable warning
        
        // Clean up UI_METHOD
        UI_destroy_method(ui_method);
    }
    
    // 7. Create a FILE for reading input data (if needed for other operations)
    FILE *in_file = nullptr;
    if (size > 0) {
        in_file = fmemopen((void*)data, size, "rb");
        if (in_file) {
            // Get file descriptor for potential use
            int fd = fuzz_fileno(in_file);
            (void)fd; // Use to avoid unused variable warning
            
            // Example: Read a byte from the file
            int ch = fgetc(in_file);
            (void)ch; // Use to avoid unused variable warning
            
            assert_file_closed(&in_file);;
        }
    }
    
    // 8. Create a FILE for writing output
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write something to output file
        if (size > 0) {
            fwrite(data, 1, size < 100 ? size : 100, out_file);
        }
        
        // Get file descriptor for potential use
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd; // Use to avoid unused variable warning
        
        assert_file_closed(&out_file);;
    }
    
    // 9. Clean up allocated resources
    if (info_ptr) {
        OSSL_INFO_SYNTAX_POINTER_free(info_ptr);
    }
    
    if (cms) {
        CMS_ContentInfo_free(cms);
    }
    
    // 10. Remove output file if created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	return 0;
}