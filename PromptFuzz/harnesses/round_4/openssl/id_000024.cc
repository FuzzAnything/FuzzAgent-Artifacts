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
//<ID> 344
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 55
//<Quality> {"density":9,"unique_branches":{"X509_STORE_CTX_init":[[2878,9,2878,20,0,0,4,1],[2910,9,2910,22,0,0,4,0],[2915,9,2915,22,0,0,4,0],[2915,26,2915,53,0,0,4,1],[2920,9,2920,22,0,0,4,0],[2920,26,2920,51,0,0,4,1],[2925,9,2925,22,0,0,4,0],[2925,26,2925,50,0,0,4,1],[2930,9,2930,22,0,0,4,0],[2930,26,2930,47,0,0,4,1],[2935,9,2935,22,0,0,4,0],[2935,26,2935,57,0,0,4,1],[2940,9,2940,22,0,0,4,0],[2940,26,2940,48,0,0,4,1],[2945,9,2945,22,0,0,4,0],[2945,26,2945,50,0,0,4,1],[2950,9,2950,22,0,0,4,0],[2950,26,2950,49,0,0,4,1],[2955,9,2955,22,0,0,4,0],[2955,26,2955,53,0,0,4,1],[2960,9,2960,22,0,0,4,0],[2960,26,2960,53,0,0,4,1],[2965,9,2965,22,0,0,4,0],[2965,26,2965,52,0,0,4,1],[2971,9,2971,27,0,0,4,1],[2977,9,2977,22,0,0,4,1],[2979,14,2979,70,0,0,4,1],[2982,9,2982,52,0,0,4,1],[2989,9,2989,48,0,0,4,0],[2993,13,2993,23,0,0,4,1],[2997,9,2998,27,0,0,4,0]],"X509_STORE_CTX_cleanup":[[3035,9,3035,27,0,0,4,0],[3036,13,3036,32,0,0,4,0]],"OPENSSL_sk_delete":[[332,9,332,19,0,0,4,0]],"OSSL_CRMF_MSG_set1_regInfo_utf8Pairs":[[62,13,62,24,0,0,4,1],[62,28,62,38,0,0,4,1],[64,13,64,67,0,0,4,1],[66,13,66,74,0,0,4,1],[68,13,68,56,0,0,4,1],[70,13,70,54,0,0,4,1]],"OSSL_CRMF_MSG_push0_regInfo":[[213,9,213,20,0,0,4,1],[213,24,213,34,0,0,4,1],[218,9,218,29,0,0,4,0],[220,9,220,29,0,0,4,1],[222,9,222,49,0,0,4,1]],"X509v3_delete_extension":[[102,9,102,18,0,0,4,1],[106,9,106,51,0,0,4,1]],"X509_PURPOSE_get_by_id":[[161,9,161,36,0,0,4,1],[163,9,163,24,0,0,4,0]],"X509_PURPOSE_get0":[[135,9,135,16,0,0,4,0]],"X509_STORE_CTX_set_default":[[3202,9,3202,22,0,0,4,1]],"X509_VERIFY_PARAM_inherit":[[414,23,414,40,24,0,4,0],[414,45,414,55,24,0,4,1],[414,59,414,77,24,0,4,0]],"X509_VERIFY_PARAM_lookup":[[1034,9,1034,28,0,0,4,1]]},"library_calls":["OSSL_CRMF_MSG_new","ASN1_UTF8STRING_new","ASN1_STRING_set","OSSL_CRMF_MSG_set1_regInfo_utf8Pairs","ASN1_UTF8STRING_free","X509_STORE_CTX_new","X509_STORE_new","X509_STORE_CTX_init","X509_STORE_CTX_get_num_untrusted","X509_STORE_free","RSA_PKCS1_OpenSSL","RSA_set_default_method","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OPENSSL_sk_push","ossl_check_OPENSSL_CSTRING_sk_type","ossl_check_OPENSSL_CSTRING_type","ossl_check_OPENSSL_CSTRING_copyfunc_type","OPENSSL_sk_num","ossl_check_const_OPENSSL_CSTRING_sk_type","OPENSSL_sk_value","ossl_check_const_OPENSSL_CSTRING_sk_type","OPENSSL_sk_free","ossl_check_OPENSSL_CSTRING_sk_type","TS_TST_INFO_new","X509_EXTENSION_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","ASN1_OCTET_STRING_free","X509_EXTENSION_free","TS_TST_INFO_delete_ext","TS_TST_INFO_free","OSSL_CRMF_MSG_free","X509_STORE_CTX_free"],"critical_calls":["OSSL_CRMF_MSG_new","ASN1_UTF8STRING_new","ASN1_STRING_set","OSSL_CRMF_MSG_set1_regInfo_utf8Pairs","ASN1_UTF8STRING_free","X509_STORE_CTX_new","X509_STORE_new","X509_STORE_CTX_init","X509_STORE_CTX_get_num_untrusted","X509_STORE_free","RSA_PKCS1_OpenSSL","RSA_set_default_method","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OPENSSL_sk_push","ossl_check_OPENSSL_CSTRING_type","ossl_check_OPENSSL_CSTRING_sk_type","ossl_check_OPENSSL_CSTRING_copyfunc_type","OPENSSL_sk_num","ossl_check_const_OPENSSL_CSTRING_sk_type","OPENSSL_sk_value","ossl_check_const_OPENSSL_CSTRING_sk_type","OPENSSL_sk_free","ossl_check_OPENSSL_CSTRING_sk_type","TS_TST_INFO_new","X509_EXTENSION_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","ASN1_OCTET_STRING_free","X509_EXTENSION_free","TS_TST_INFO_delete_ext","TS_TST_INFO_free","OSSL_CRMF_MSG_free","X509_STORE_CTX_free"],"visited":1}
/*Here's a C++ fuzz driver that achieves the event using the specified OpenSSL APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE pointer to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE pointer for output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Create and set up OSSL_CRMF_MSG
    OSSL_CRMF_MSG *crmf_msg = OSSL_CRMF_MSG_new();
    if (crmf_msg) {
        // Create ASN1_UTF8STRING from input data
        ASN1_UTF8STRING *utf8str = ASN1_UTF8STRING_new();
        if (utf8str && size > 0) {
            // Use first part of data for UTF8 string (limit to reasonable size)
            size_t str_len = size > 100 ? 100 : size;
            ASN1_STRING_set(utf8str, data, str_len);
            
            // Call API 1: Set registration info with UTF8 pairs
            OSSL_CRMF_MSG_set1_regInfo_utf8Pairs(crmf_msg, utf8str);
            
            ASN1_UTF8STRING_free(utf8str);
        }
    }

    // 2. Create X509_STORE_CTX and get number of untrusted certs
    X509_STORE_CTX *store_ctx = X509_STORE_CTX_new();
    if (store_ctx) {
        // Initialize with empty store
        X509_STORE *store = X509_STORE_new();
        if (store) {
            X509_STORE_CTX_init(store_ctx, store, NULL, NULL);
            
            // Call API 2: Get number of untrusted certificates
            int num_untrusted = X509_STORE_CTX_get_num_untrusted(store_ctx);
            (void)num_untrusted; // Use variable to avoid unused warning
            
            X509_STORE_free(store);
        }
    }

    // 3. Set default RSA method
    // Get the default RSA method
    const RSA_METHOD *default_meth = RSA_PKCS1_OpenSSL();
    
    // Call API 3: Set default RSA method
    RSA_set_default_method(default_meth);

    // 4. Use the OPENSSL_CSTRING copy function type
    // Create a stack of C strings
    STACK_OF(OPENSSL_CSTRING) *string_stack = sk_OPENSSL_CSTRING_new_null();
    if (string_stack) {
        // Add some test strings using input data
        if (size > 10) {
            char *str1 = (char *)malloc(11);
            if (str1) {
                memcpy(str1, data, 10);
                str1[10] = '\0';
                sk_OPENSSL_CSTRING_push(string_stack, str1);
            }
        }
        
        // Call API 4: Get the copy function type
        OPENSSL_sk_copyfunc copy_func = 
            ossl_check_OPENSSL_CSTRING_copyfunc_type(sk_OPENSSL_CSTRING_copyfunc(NULL));
        (void)copy_func; // Use variable to avoid unused warning
        
        // Clean up the stack
        for (int i = 0; i < sk_OPENSSL_CSTRING_num(string_stack); i++) {
            free((void *)sk_OPENSSL_CSTRING_value(string_stack, i));
        }
        sk_OPENSSL_CSTRING_free(string_stack);
    }

    // 5. Create TS_TST_INFO and delete an extension
    TS_TST_INFO *tst_info = TS_TST_INFO_new();
    if (tst_info) {
        // Try to add an extension first (simulate using input data)
        if (size > 20) {
            X509_EXTENSION *ext = X509_EXTENSION_new();
            if (ext) {
                // Create a simple extension using input data
                ASN1_OCTET_STRING *octet = ASN1_OCTET_STRING_new();
                if (octet) {
                    size_t ext_len = size > 50 ? 50 : size;
                    ASN1_OCTET_STRING_set(octet, data, ext_len);
                    // Note: This is simplified - actual extension creation would be more complex
                    ASN1_OCTET_STRING_free(octet);
                }
                X509_EXTENSION_free(ext);
            }
        }
        
        // Call API 5: Delete extension at location 0
        // Note: This will fail if no extensions exist, but we still call the API
        TS_TST_INFO_delete_ext(tst_info, 0);
        
        TS_TST_INFO_free(tst_info);
    }

    // 6. Consume remaining input data by writing to output file
    if (size > 0) {
        // Write some of the input data to output file
        size_t write_size = size > 1024 ? 1024 : size;
        fwrite(data, 1, write_size, out_file);
    }

    // Clean up resources
    if (crmf_msg) {
        OSSL_CRMF_MSG_free(crmf_msg);
    }
    
    if (store_ctx) {
        X509_STORE_CTX_free(store_ctx);
    }

    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}