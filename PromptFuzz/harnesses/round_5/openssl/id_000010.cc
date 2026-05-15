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
//<ID> 159
//<Prompt> []
/*<Combination>: [
*/
//<score> 180, nr_unique_branch: 93
//<Quality> {"density":10,"unique_branches":{"ossl_store_loader_store_remove_all_provided":[[401,9,401,22,0,0,4,0]],"ossl_lib_ctx_get_data":[[655,5,655,47,0,0,4,0]],"ossl_provider_disable_fallback_loading":[[343,9,343,53,0,0,4,0],[344,13,344,51,0,0,4,1]],"ossl_encoder_store_remove_all_provided":[[457,9,457,22,0,0,4,0]],"ossl_asn1_do_adb":[[275,13,275,36,0,0,4,0]],"EVP_CIPHER_can_pipeline":[[1551,11,1551,14,0,0,4,0],[1551,11,1551,14,0,0,4,1],[1551,18,1551,41,0,0,4,1],[1551,47,1551,51,0,0,4,0],[1551,47,1551,51,0,0,4,1],[1551,55,1551,78,0,0,4,1]],"ossl_store_loader_store_cache_flush":[[391,9,391,22,0,0,4,0]],"CMS_EncryptedData_set1_key":[[236,9,236,13,0,0,4,1],[236,17,236,24,0,0,4,1],[240,9,240,13,0,0,4,0],[241,13,241,74,0,0,4,1],[245,13,245,41,0,0,4,1],[250,13,250,34,0,0,4,1]],"ossl_decoder_cache_flush":[[815,9,815,22,0,0,4,1],[818,9,818,47,0,0,4,1]],"cms_ec_cb":[[89,9,89,39,0,0,4,0],[89,9,89,39,0,0,4,1]],"ossl_decoder_store_cache_flush":[[441,9,441,22,0,0,4,0]],"ossl_provider_find":[[416,13,416,22,0,0,4,0],[417,17,417,48,0,0,4,1],[432,13,432,71,0,0,4,1],[435,13,435,25,0,0,4,1]],"OSSL_trace_get_category_name":[[148,9,148,16,0,0,4,1],[148,20,148,45,0,0,4,1],[154,9,154,21,0,0,4,1],[155,12,155,24,0,0,4,1]],"ossl_provider_new":[[536,9,536,53,0,0,4,1],[540,9,540,30,0,0,4,0],[546,45,546,60,0,0,4,0],[547,17,547,43,0,0,4,1],[554,13,554,50,0,0,4,1],[556,42,556,64,0,0,4,1],[585,9,585,23,0,0,4,1],[610,9,610,36,0,0,4,1],[613,9,613,21,0,0,4,1],[616,9,616,60,0,0,4,1]],"provider_flush_store_cache":[[1334,9,1334,59,0,0,4,1],[1337,9,1337,46,0,0,4,1],[1342,9,1342,17,0,0,4,0]],"OSSL_PROVIDER_load_ex":[[61,9,61,55,0,0,4,0]],"ossl_cms_EncryptedContent_init":[[220,9,220,12,0,0,4,0],[221,13,221,55,0,0,4,1],[226,9,226,23,0,0,4,0]],"ossl_provider_set_module_path":[[802,9,802,28,0,0,4,0]],"provider_deactivate_free":[[238,9,238,29,0,0,4,1]],"ossl_decoder_store_remove_all_provided":[[451,9,451,22,0,0,4,0]],"OSSL_PROVIDER_unload":[[73,9,73,43,0,0,4,1]],"asn1_primitive_new":[[271,13,271,18,0,0,4,0],[272,17,272,31,0,0,4,0]],"ossl_provider_add_to_store":[[668,9,668,27,0,0,4,0],[671,9,671,59,0,0,4,1],[674,9,674,47,0,0,4,1],[679,9,679,18,0,0,4,0],[684,9,684,18,0,0,4,0],[685,13,685,63,0,0,4,1],[688,13,688,44,0,0,4,1],[692,13,692,30,0,0,4,0],[698,9,698,27,0,0,4,0],[699,13,699,45,0,0,4,1],[707,9,707,17,0,0,4,1]],"ossl_provider_activate":[[1405,9,1405,21,0,0,4,1],[1412,9,1412,16,0,0,4,1],[1415,9,1415,58,0,0,4,0],[1416,16,1416,26,0,0,4,0]],"evp_method_store_cache_flush":[[441,9,441,22,0,0,4,0]],"evp_method_store_remove_all_provided":[[451,9,451,22,0,0,4,0]],"create_provider_children":[[645,17,645,24,0,0,4,1]],"OSSL_ENCODER_CTX_new_for_pkey":[[351,9,351,21,0,0,4,0]],"ossl_method_store_cache_flush_all":[[940,21,940,35,0,0,4,0],[940,21,940,35,0,0,4,1],[943,13,943,42,0,0,4,1]],"OSSL_PROVIDER_try_load_ex":[[25,9,25,61,0,0,4,0],[26,13,26,78,0,0,4,1],[31,9,31,44,0,0,4,1],[37,9,37,14,0,0,4,0],[37,18,37,78,0,0,4,1],[42,9,42,23,0,0,4,1]],"ossl_method_store_remove_all_provided":[[691,21,691,35,0,0,4,0],[691,21,691,35,0,0,4,1],[694,13,694,42,0,0,4,1]],"ossl_encoder_store_cache_flush":[[447,9,447,22,0,0,4,0]],"provider_remove_store_methods":[[1374,9,1374,17,0,0,4,0],[1377,13,1377,57,0,0,4,1]],"ossl_asn1_primitive_free":[[159,17,159,19,0,0,4,0],[159,23,159,37,0,0,4,0]],"ASN1_item_unpack_ex":[[72,9,74,16,0,0,4,0],[72,9,74,16,0,0,4,1]]},"library_calls":["OSSL_LIB_CTX_new","OSSL_trace_get_category_name","OSSL_PROVIDER_load","OSSL_FUNC_core_thread_start","OSSL_FUNC_rand_gettable_params","EVP_aes_128_cfb128","EVP_CIPHER_can_pipeline","EVP_CIPHER_can_pipeline","OSSL_ENCODER_CTX_new_for_pkey","OSSL_ENCODER_CTX_set_selection","OSSL_ENCODER_CTX_free","CMS_ContentInfo_new","CMS_EncryptedData_set1_key","ASN1_STRING_new","ASN1_STRING_set","ASN1_OCTET_STRING_it","ASN1_item_unpack_ex","ASN1_item_free","ASN1_STRING_free","CMS_ContentInfo_free","OSSL_PROVIDER_unload","OSSL_LIB_CTX_free"],"critical_calls":["OSSL_LIB_CTX_new","OSSL_trace_get_category_name","OSSL_PROVIDER_load","OSSL_FUNC_core_thread_start","OSSL_FUNC_rand_gettable_params","EVP_aes_128_cfb128","EVP_CIPHER_can_pipeline","EVP_CIPHER_can_pipeline","OSSL_ENCODER_CTX_new_for_pkey","OSSL_ENCODER_CTX_set_selection","OSSL_ENCODER_CTX_free","CMS_ContentInfo_new","CMS_EncryptedData_set1_key","ASN1_STRING_new","ASN1_STRING_set","ASN1_OCTET_STRING_it","ASN1_item_unpack_ex","ASN1_item_free","ASN1_STRING_free","CMS_ContentInfo_free","OSSL_PROVIDER_unload","OSSL_LIB_CTX_free"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified OpenSSL APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Create file for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 2. Initialize OpenSSL context
    OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
    if (!libctx) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 3. Use OSSL_trace_get_category_name
    const char *trace_name = OSSL_trace_get_category_name(0);
    // We just call it, don't need to use the result for fuzzing

    // 4. Try to get dispatch functions (simulate provider context)
    OSSL_PROVIDER *prov = OSSL_PROVIDER_load(libctx, "default");
    if (prov) {
        // Get dispatch table (simplified - in reality we'd need to query provider)
        // We'll simulate this for the fuzzer
        OSSL_DISPATCH dummy_dispatch[] = {{0, NULL}};
        
        // Call OSSL_FUNC_core_thread_start (though we may not have real implementation)
        OSSL_FUNC_core_thread_start_fn *thread_start = 
            OSSL_FUNC_core_thread_start(dummy_dispatch);
        // We don't actually call it, just check if we got the function pointer
        
        // Call OSSL_FUNC_rand_gettable_params
        OSSL_FUNC_rand_gettable_params_fn *rand_gettable = 
            OSSL_FUNC_rand_gettable_params(dummy_dispatch);
        // We don't actually call it, just check if we got the function pointer
    }

    // 5. Create and use a cipher for pipeline checking
    const EVP_CIPHER *cipher = EVP_aes_128_cfb128();
    if (cipher) {
        // Check if cipher can pipeline
        int can_pipeline_encrypt = EVP_CIPHER_can_pipeline(cipher, 1);  // encrypt
        int can_pipeline_decrypt = EVP_CIPHER_can_pipeline(cipher, 0);  // decrypt
        // Results can be ignored for fuzzing
    }

    // 6. Create OSSL_ENCODER_CTX and set selection
    OSSL_ENCODER_CTX *encoder_ctx = OSSL_ENCODER_CTX_new_for_pkey(
        NULL,  // No pkey
        OSSL_KEYMGMT_SELECT_ALL,
        "DER",  // encoding
        NULL,   // structure
        0       // selection
    );
    
    if (encoder_ctx) {
        // Set selection on encoder context
        int selection_set = OSSL_ENCODER_CTX_set_selection(encoder_ctx, 
                                                          OSSL_KEYMGMT_SELECT_ALL);
        OSSL_ENCODER_CTX_free(encoder_ctx);
    }

    // 7. Create CMS ContentInfo and try to set key
    CMS_ContentInfo *cms = CMS_ContentInfo_new();
    if (cms && size >= 16) {
        // Use first 16 bytes as key if available
        size_t keylen = size > 16 ? 16 : size;
        const unsigned char *key = data;
        
        // Try to set key on encrypted data
        int key_set = CMS_EncryptedData_set1_key(cms, cipher, key, keylen);
        // Result can be ignored for fuzzing
    }

    // 8. Try to unpack ASN1 data from the input
    if (size > 0) {
        // Create an ASN1_STRING from the input data
        ASN1_STRING *asn1_str = ASN1_STRING_new();
        if (asn1_str) {
            // Copy input data into ASN1_STRING
            ASN1_STRING_set(asn1_str, data, size > 100 ? 100 : size);
            
            // Try to unpack using a simple ASN1 item (OCTET_STRING)
            const ASN1_ITEM *item = ASN1_OCTET_STRING_it();
            void *unpacked = ASN1_item_unpack_ex(asn1_str, item, libctx, NULL);
            
            if (unpacked) {
                // If unpacking succeeded, free the result
                ASN1_item_free(static_cast<ASN1_VALUE*>(unpacked), item);
            }
            
            ASN1_STRING_free(asn1_str);
        }
    }

    // 9. Try to read some data from the input file
    unsigned char buffer[256];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
    
    // 10. Try to write some data to output file
    if (bytes_read > 0) {
        fwrite(buffer, 1, bytes_read, out_file);
    }

    // Cleanup
    if (cms) {
        CMS_ContentInfo_free(cms);
    }
    
    if (prov) {
        OSSL_PROVIDER_unload(prov);
    }
    
    OSSL_LIB_CTX_free(libctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}