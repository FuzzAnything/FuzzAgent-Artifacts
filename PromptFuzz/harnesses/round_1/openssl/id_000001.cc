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
//<ID> 73
//<Prompt> []
/*<Combination>: [
*/
//<score> 78, nr_unique_branch: 10
//<Quality> {"density":13,"unique_branches":{"OPENSSL_sk_value":[[489,32,489,44,0,0,4,0]],"OBJ_nid2obj":[[264,13,264,18,0,0,4,1],[264,22,264,33,0,0,4,1],[264,37,264,65,0,0,4,1],[270,9,270,30,0,0,4,1],[276,9,276,20,0,0,4,1]],"X509_ACERT_get0_signature":[[130,9,130,21,0,0,4,0],[132,9,132,21,0,0,4,0]],"OBJ_cmp":[[60,9,60,12,0,0,4,1]],"added_obj_hash":[[148,5,148,19,0,0,4,0]]},"library_calls":["POLICYQUALINFO_new","CRYPTO_malloc","OBJ_nid2obj","CRYPTO_free","POLICYQUALINFO_free","OSSL_FUNC_keymgmt_new","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OBJ_nid2obj","OBJ_nid2obj","OPENSSL_sk_push","ossl_check_ASN1_OBJECT_sk_type","ossl_check_ASN1_OBJECT_type","OPENSSL_sk_push","ossl_check_ASN1_OBJECT_sk_type","ossl_check_ASN1_OBJECT_type","sk_ASN1_OBJECT_cmpfunc_thunk","OPENSSL_sk_free","ossl_check_ASN1_OBJECT_sk_type","OSSL_FUNC_signature_digest_sign_final","X509_ACERT_new","X509_ACERT_get0_signature","X509_ACERT_free","sk_OPENSSL_PSTRING_new_null","CRYPTO_malloc","sk_OPENSSL_PSTRING_push","sk_OPENSSL_PSTRING_value","CRYPTO_free","sk_OPENSSL_PSTRING_value","sk_OPENSSL_PSTRING_free"],"critical_calls":["POLICYQUALINFO_new","CRYPTO_malloc","OBJ_nid2obj","CRYPTO_free","POLICYQUALINFO_free","OSSL_FUNC_keymgmt_new","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OBJ_nid2obj","OBJ_nid2obj","OPENSSL_sk_push","ossl_check_ASN1_OBJECT_type","ossl_check_ASN1_OBJECT_sk_type","OPENSSL_sk_push","ossl_check_ASN1_OBJECT_type","ossl_check_ASN1_OBJECT_sk_type","sk_ASN1_OBJECT_cmpfunc_thunk","OPENSSL_sk_free","ossl_check_ASN1_OBJECT_sk_type","OSSL_FUNC_signature_digest_sign_final","X509_ACERT_new","X509_ACERT_get0_signature","X509_ACERT_free","sk_OPENSSL_PSTRING_new_null","CRYPTO_malloc","sk_OPENSSL_PSTRING_push","sk_OPENSSL_PSTRING_value","CRYPTO_free","sk_OPENSSL_PSTRING_value","sk_OPENSSL_PSTRING_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE* to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE* for output (not strictly needed but satisfies requirement)
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create and free a POLICYQUALINFO structure
    POLICYQUALINFO *policy_qual = POLICYQUALINFO_new();
    if (policy_qual) {
        // Try to parse some data into the structure
        unsigned char *data_copy = (unsigned char *)OPENSSL_malloc(size);
        if (data_copy) {
            memcpy(data_copy, data, size);
            // Minimal attempt to use data - this is a fuzzer so we're lenient
            if (size > 4) {
                policy_qual->pqualid = OBJ_nid2obj(*(int*)data_copy);
            }
            OPENSSL_free(data_copy);
        }
        POLICYQUALINFO_free(policy_qual);
    }

    // Try to use OSSL_FUNC_keymgmt_new_fn (requires OSSL_DISPATCH)
    // Create a minimal dispatch table with NULL entries
    OSSL_DISPATCH dispatch_table[] = {
        {0, NULL}
    };
    OSSL_FUNC_keymgmt_new_fn *keymgmt_new = OSSL_FUNC_keymgmt_new(dispatch_table);
    // Note: keymgmt_new will likely be NULL but we've called the API

    // Create a stack of ASN1_OBJECT and use the comparison function
    STACK_OF(ASN1_OBJECT) *obj_stack = sk_ASN1_OBJECT_new_null();
    if (obj_stack) {
        // Create some dummy ASN1_OBJECTs for comparison
        ASN1_OBJECT *obj1 = OBJ_nid2obj(NID_commonName);
        ASN1_OBJECT *obj2 = OBJ_nid2obj(NID_countryName);
        
        if (obj1 && obj2) {
            sk_ASN1_OBJECT_push(obj_stack, obj1);
            sk_ASN1_OBJECT_push(obj_stack, obj2);
            
            // Use the comparison thunk function
            int cmp_result = sk_ASN1_OBJECT_cmpfunc_thunk(
                (int (*)(const void *, const void *))OBJ_cmp,
                obj1, 
                obj2
            );
            // cmp_result is unused in fuzzing context
        }
        sk_ASN1_OBJECT_free(obj_stack);
    }

    // Try to get OSSL_FUNC_signature_digest_sign_final_fn
    OSSL_FUNC_signature_digest_sign_final_fn *digest_sign_final = 
        OSSL_FUNC_signature_digest_sign_final(dispatch_table);
    // Note: digest_sign_final will likely be NULL but we've called the API

    // Try to read and process an X509_ACERT from the input
    X509_ACERT *acert = NULL;
    // First, read some bytes to use as potential certificate data
    fseek(in_file, 0, SEEK_SET);
    
    // We'll create a dummy X509_ACERT and use the get0_signature API
    // Note: In real usage, we'd parse actual certificate data
    X509_ACERT *dummy_acert = X509_ACERT_new();
    if (dummy_acert) {
        const ASN1_BIT_STRING *sig = NULL;
        const X509_ALGOR *algor = NULL;
        
        // Use the API to get signature info
        X509_ACERT_get0_signature(dummy_acert, &sig, &algor);
        // Results may be NULL for dummy certificate
        
        X509_ACERT_free(dummy_acert);
    }

    // Create and use a stack of OPENSSL_PSTRING
    STACK_OF(OPENSSL_PSTRING) *pstring_stack = sk_OPENSSL_PSTRING_new_null();
    if (pstring_stack) {
        // Add some strings from the input data
        // We'll use the first few bytes as a string if possible
        size_t str_len = size > 100 ? 100 : size;
        char *test_string = (char *)OPENSSL_malloc(str_len + 1);
        if (test_string) {
            memcpy(test_string, data, str_len);
            test_string[str_len] = '\0';
            
            OPENSSL_STRING *string_ptr = &test_string;
            sk_OPENSSL_PSTRING_push(pstring_stack, string_ptr);
            
            // Use the value API
            OPENSSL_STRING *retrieved = sk_OPENSSL_PSTRING_value(pstring_stack, 0);
            // retrieved could be used here
            
            OPENSSL_free(test_string);
        }
        
        // Try to access another index (should return NULL)
        OPENSSL_STRING *null_retrieved = sk_OPENSSL_PSTRING_value(pstring_stack, 5);
        
        sk_OPENSSL_PSTRING_free(pstring_stack);
    }

    // Write some data to output file to use file descriptor APIs
    if (size > 0 && out_file) {
        fwrite(data, 1, size > 1024 ? 1024 : size, out_file);
        fflush(out_file);
        
        // Demonstrate using file descriptor if needed
        int out_fd = fuzz_fileno(out_file);
        // out_fd could be used with other APIs that need file descriptors
    }

    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}