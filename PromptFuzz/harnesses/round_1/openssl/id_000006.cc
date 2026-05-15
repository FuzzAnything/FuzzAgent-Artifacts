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
//<ID> 149
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 53
//<Quality> {"density":4,"unique_branches":{"ASN1_INTEGER_get":[[576,9,576,15,0,0,4,0]],"skeymgmt_new":[[52,9,52,63,0,0,4,1],[54,9,54,46,0,0,4,1]],"asn1_get_int64":[[268,9,268,42,0,0,4,0],[288,13,288,27,0,0,4,1]],"deflt_query":[[771,5,771,26,0,0,4,0]],"EVP_SKEYMGMT_free":[[163,9,163,25,0,0,4,1],[167,9,167,16,0,0,4,0]],"skeymgmt_from_algorithm":[[68,9,68,44,0,0,4,1],[72,9,72,80,0,0,4,1],[78,12,78,33,0,0,4,0],[78,12,78,33,0,0,4,1],[79,17,79,33,0,0,4,1],[80,9,80,37,0,0,4,0],[80,9,80,37,0,0,4,1],[81,17,81,39,0,0,4,0],[84,9,84,39,0,0,4,0],[84,9,84,39,0,0,4,1],[85,17,85,41,0,0,4,0],[88,9,88,39,0,0,4,0],[88,9,88,39,0,0,4,1],[89,17,89,41,0,0,4,0],[92,9,92,41,0,0,4,1],[96,9,96,43,0,0,4,1],[100,9,100,52,0,0,4,0],[100,9,100,52,0,0,4,1],[101,17,101,45,0,0,4,0],[104,9,104,52,0,0,4,1],[112,9,112,31,0,0,4,1],[113,12,113,36,0,0,4,1],[114,12,114,36,0,0,4,1],[120,9,120,36,0,0,4,1]],"asn1_get_uint64":[[227,9,227,27,0,0,4,0]],"d2i_ASN1_UINTEGER":[[421,9,421,20,0,0,4,0],[422,13,422,47,0,0,4,1],[430,9,430,19,0,0,4,0],[430,9,430,19,0,0,4,1],[435,9,435,30,0,0,4,0],[435,9,435,30,0,0,4,1],[440,9,440,16,0,0,4,1],[445,9,445,26,0,0,4,1],[455,9,455,18,0,0,4,1],[458,9,458,12,0,0,4,0],[458,9,458,12,0,0,4,1],[459,13,459,22,0,0,4,0],[459,13,459,22,0,0,4,1],[459,26,459,36,0,0,4,0],[459,26,459,36,0,0,4,1],[468,9,468,18,0,0,4,1],[473,9,473,15,0,0,4,0],[475,9,475,20,0,0,4,0],[107,19,107,22,7,0,4,0],[107,19,107,22,7,0,4,1]]},"library_calls":["CRYPTO_set_mem_functions","PROXY_CERT_INFO_EXTENSION_new","d2i_ASN1_UINTEGER","RSA_new","RSA_up_ref","OSSL_FUNC_encoder_does_selection","EVP_PKEY_CTX_new_id","EVP_SKEYMGMT_fetch","EVP_SKEYMGMT_get0_gen_settable_params","EVP_SKEYMGMT_free","OBJ_nid2obj","EVP_PKEY_CTX_set0_dh_kdf_oid","EVP_PKEY_CTX_free","X509_policy_tree_level_count","ASN1_INTEGER_get","PROXY_CERT_INFO_EXTENSION_free","ASN1_INTEGER_free","RSA_free","RSA_free"],"critical_calls":["CRYPTO_set_mem_functions","PROXY_CERT_INFO_EXTENSION_new","d2i_ASN1_UINTEGER","RSA_new","RSA_up_ref","OSSL_FUNC_encoder_does_selection","EVP_PKEY_CTX_new_id","EVP_SKEYMGMT_fetch","EVP_SKEYMGMT_get0_gen_settable_params","EVP_SKEYMGMT_free","OBJ_nid2obj","EVP_PKEY_CTX_set0_dh_kdf_oid","EVP_PKEY_CTX_free","X509_policy_tree_level_count","ASN1_INTEGER_get","PROXY_CERT_INFO_EXTENSION_free","ASN1_INTEGER_free","RSA_free","RSA_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Step 1: Set custom memory functions
    CRYPTO_set_mem_functions(
        [](size_t sz, const char *file, int line) -> void* {
            return malloc(sz);
        },
        [](void *ptr, size_t sz, const char *file, int line) -> void* {
            return realloc(ptr, sz);
        },
        [](void *ptr, const char *file, int line) {
            free(ptr);
        }
    );

    // Step 2: Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 3: Create PROXY_CERT_INFO_EXTENSION
    PROXY_CERT_INFO_EXTENSION *proxy_info = PROXY_CERT_INFO_EXTENSION_new();
    
    // Step 4: Parse ASN1_INTEGER from input data
    const unsigned char *p = data;
    ASN1_INTEGER *asn1_int = d2i_ASN1_UINTEGER(NULL, &p, size);
    
    // Step 5: Create RSA and increment reference
    RSA *rsa = RSA_new();
    if (rsa) {
        RSA_up_ref(rsa);
    }
    
    // Step 6: Create OSSL_DISPATCH structure and get encoder function
    OSSL_DISPATCH dispatch[] = {
        {OSSL_FUNC_ENCODER_DOES_SELECTION, (void (*)(void))OSSL_FUNC_encoder_does_selection},
        {0, NULL}
    };
    OSSL_FUNC_encoder_does_selection_fn *does_selection = 
        OSSL_FUNC_encoder_does_selection(dispatch);
    
    // Step 7: Get settable parameters for EVP_SKEYMGMT
    // We need to create a temporary EVP_SKEYMGMT context
    EVP_PKEY_CTX *pkey_ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_DH, NULL);
    if (pkey_ctx) {
        // For demonstration, we'll use RSA as the key type
        const EVP_SKEYMGMT *skeymgmt = EVP_SKEYMGMT_fetch(NULL, "RSA", NULL);
        if (skeymgmt) {
            const OSSL_PARAM *settable_params = 
                EVP_SKEYMGMT_get0_gen_settable_params(skeymgmt);
            // Clean up
            EVP_SKEYMGMT_free((EVP_SKEYMGMT *)skeymgmt);
        }
        
        // Step 8: Set DH KDF OID using the parsed ASN1_INTEGER if available
        if (asn1_int) {
            // Create a simple OID for testing
            ASN1_OBJECT *oid = OBJ_nid2obj(NID_sha256);
            if (oid) {
                EVP_PKEY_CTX_set0_dh_kdf_oid(pkey_ctx, oid);
                // Note: set0 transfers ownership, don't free oid here
            }
        }
        
        EVP_PKEY_CTX_free(pkey_ctx);
    }
    
    // Step 9: Check policy tree level count
    // Create a minimal X509_POLICY_TREE structure for testing
    X509_POLICY_TREE *policy_tree = NULL;
    // We'll pass NULL since we don't have a real policy tree in this context
    int level_count = X509_policy_tree_level_count(policy_tree);
    
    // Step 10: Write some output to file (optional)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        if (asn1_int) {
            // Write the ASN1_INTEGER length to output
            int len = ASN1_INTEGER_get(asn1_int);
            fwrite(&len, sizeof(len), 1, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // Step 11: Clean up all allocated resources
    if (proxy_info) {
        PROXY_CERT_INFO_EXTENSION_free(proxy_info);
    }
    if (asn1_int) {
        ASN1_INTEGER_free(asn1_int);
    }
    if (rsa) {
        RSA_free(rsa);  // First free decrements ref count
        RSA_free(rsa);  // Second free actually frees if ref count reached 0
    }
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}