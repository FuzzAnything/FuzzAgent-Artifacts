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
//<ID> 413
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 2
//<Quality> {"density":4,"unique_branches":{"BN_set_word":[[421,15,421,16,0,0,4,1]],"asn1_string_set_int64":[[360,9,360,14,0,0,4,0]]},"library_calls":["CRYPTO_set_mem_functions","PROXY_CERT_INFO_EXTENSION_new","RSA_new","EVP_PKEY_CTX_new_id","EVP_PKEY_new","BN_new","BN_set_word","EVP_PKEY_set_bn_param","PKCS5_PBE_add","RSA_up_ref","OSSL_FUNC_encoder_does_selection","EVP_SKEYMGMT_get0_gen_settable_params","OBJ_nid2obj","EVP_PKEY_CTX_set0_dh_kdf_oid","ASN1_INTEGER_new","ASN1_INTEGER_set","ASN1_INTEGER_get","X509_policy_tree_level_count","ASIdOrRange_it","PROXY_CERT_INFO_EXTENSION_free","RSA_free","RSA_free","EVP_PKEY_CTX_free","EVP_PKEY_free","BN_free","ASN1_INTEGER_free","ASN1_OBJECT_free"],"critical_calls":["CRYPTO_set_mem_functions","PROXY_CERT_INFO_EXTENSION_new","RSA_new","EVP_PKEY_CTX_new_id","EVP_PKEY_new","BN_new","BN_set_word","EVP_PKEY_set_bn_param","PKCS5_PBE_add","RSA_up_ref","OSSL_FUNC_encoder_does_selection","EVP_SKEYMGMT_get0_gen_settable_params","OBJ_nid2obj","EVP_PKEY_CTX_set0_dh_kdf_oid","ASN1_INTEGER_new","ASN1_INTEGER_set","ASN1_INTEGER_get","X509_policy_tree_level_count","ASIdOrRange_it","PROXY_CERT_INFO_EXTENSION_free","RSA_free","RSA_free","EVP_PKEY_CTX_free","EVP_PKEY_free","BN_free","ASN1_INTEGER_free","ASN1_OBJECT_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Set custom memory functions first
    CRYPTO_set_mem_functions(
        (CRYPTO_malloc_fn)malloc,
        (CRYPTO_realloc_fn)realloc,
        (CRYPTO_free_fn)free
    );

    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize variables
    PROXY_CERT_INFO_EXTENSION *proxy_ext = NULL;
    RSA *rsa_key = NULL;
    EVP_PKEY_CTX *pkey_ctx = NULL;
    EVP_PKEY *pkey = NULL;
    ASN1_OBJECT *oid = NULL;
    ASN1_INTEGER *asn1_int = NULL;
    BIGNUM *bn = NULL;
    const OSSL_PARAM *params = NULL;
    OSSL_FUNC_encoder_does_selection_fn *encoder_fn = NULL;
    
    // Event: Create and manipulate cryptographic objects using input data
    
    // 1. Create PROXY_CERT_INFO_EXTENSION
    proxy_ext = PROXY_CERT_INFO_EXTENSION_new();
    
    // 2. Create RSA key
    rsa_key = RSA_new();
    
    // 3. Create EVP_PKEY context for DH
    pkey_ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_DH, NULL);
    
    // 4. Create EVP_PKEY and set BIGNUM parameter
    pkey = EVP_PKEY_new();
    bn = BN_new();
    
    // Read some data for BIGNUM
    if (size >= 4) {
        // Use first 4 bytes as integer value
        long value = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
        BN_set_word(bn, value);
        
        // Set BIGNUM parameter in EVP_PKEY
        EVP_PKEY_set_bn_param(pkey, "priv", bn);
    }
    
    // 5. Add PKCS5 PBE algorithms
    PKCS5_PBE_add();
    
    // 6. Increment RSA reference count
    if (rsa_key) {
        RSA_up_ref(rsa_key);
    }
    
    // 7. Get encoder selection function (simulate with dummy dispatch)
    {
        OSSL_DISPATCH dummy_dispatch = {0, NULL};
        encoder_fn = OSSL_FUNC_encoder_does_selection(&dummy_dispatch);
    }
    
    // 8. Get settable parameters from EVP_SKEYMGMT (simulate)
    {
        EVP_SKEYMGMT *skeymgmt = NULL;
        params = EVP_SKEYMGMT_get0_gen_settable_params(skeymgmt);
    }
    
    // 9. Convert NID to ASN1_OBJECT
    oid = OBJ_nid2obj(NID_dhKeyAgreement);
    
    // 10. Set DH KDF OID in context
    if (pkey_ctx && oid) {
        EVP_PKEY_CTX_set0_dh_kdf_oid(pkey_ctx, oid);
        oid = NULL; // Context takes ownership
    }
    
    // 11. Create and use ASN1_INTEGER from input data
    if (size >= 8) {
        asn1_int = ASN1_INTEGER_new();
        // Use bytes 4-7 for ASN1_INTEGER value
        long int_value = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];
        ASN1_INTEGER_set(asn1_int, int_value);
        
        // Get value back
        long retrieved = ASN1_INTEGER_get(asn1_int);
        (void)retrieved; // Use value to avoid unused variable warning
    }
    
    // 12. Simulate policy tree level count (requires X509_POLICY_TREE)
    {
        X509_POLICY_TREE *tree = NULL;
        int levels = X509_policy_tree_level_count(tree);
        (void)levels;
    }
    
    // 13. Get ASIdOrRange ASN1_ITEM
    const ASN1_ITEM *asid_item = ASIdOrRange_it();
    (void)asid_item;
    
    // Write some data to output file using the file descriptor
    if (size > 0) {
        int fd = fuzz_fileno(out_file);
        (void)fd; // File descriptor available for use
        fwrite(data, 1, size > 1024 ? 1024 : size, out_file);
    }
    
    // Read from input file using file descriptor
    {
        int in_fd = fuzz_fileno(in_file);
        (void)in_fd; // File descriptor available for use
        uint8_t buffer[128];
        fread(buffer, 1, sizeof(buffer), in_file);
    }
    
    // Cleanup - free all allocated resources
    if (proxy_ext) PROXY_CERT_INFO_EXTENSION_free(proxy_ext);
    if (rsa_key) {
        RSA_free(rsa_key);  // First free
        RSA_free(rsa_key);  // Second free due to up_ref
    }
    if (pkey_ctx) EVP_PKEY_CTX_free(pkey_ctx);
    if (pkey) EVP_PKEY_free(pkey);
    if (bn) BN_free(bn);
    if (asn1_int) ASN1_INTEGER_free(asn1_int);
    if (oid) ASN1_OBJECT_free(oid);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}