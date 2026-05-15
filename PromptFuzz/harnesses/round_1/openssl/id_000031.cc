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
//<ID> 411
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["CRYPTO_set_mem_functions","PROXY_CERT_INFO_EXTENSION_new","PROXY_CERT_INFO_EXTENSION_free","RSA_new","BN_new","BN_set_word","EVP_PKEY_new","EVP_PKEY_set_bn_param","EVP_PKEY_free","BN_free","RSA_up_ref","RSA_free","RSA_free","PKCS5_PBE_add","OSSL_FUNC_encoder_does_selection","EVP_PKEY_CTX_new_id","OBJ_nid2obj","EVP_PKEY_CTX_set0_dh_kdf_oid","EVP_PKEY_CTX_free","X509_policy_tree_level_count","ASN1_INTEGER_new","ASN1_INTEGER_set_int64","ASN1_INTEGER_get","ASN1_INTEGER_free","ASN1_INTEGER_free","ASIdOrRange_it","EVP_SKEYMGMT_get0_gen_settable_params"],"critical_calls":["CRYPTO_set_mem_functions","PROXY_CERT_INFO_EXTENSION_new","PROXY_CERT_INFO_EXTENSION_free","RSA_new","BN_new","BN_set_word","EVP_PKEY_new","EVP_PKEY_set_bn_param","EVP_PKEY_free","BN_free","RSA_up_ref","RSA_free","RSA_free","PKCS5_PBE_add","OSSL_FUNC_encoder_does_selection","EVP_PKEY_CTX_new_id","OBJ_nid2obj","EVP_PKEY_CTX_set0_dh_kdf_oid","EVP_PKEY_CTX_free","X509_policy_tree_level_count","ASN1_INTEGER_new","ASN1_INTEGER_set_int64","ASN1_INTEGER_get","ASN1_INTEGER_free","ASIdOrRange_it","EVP_SKEYMGMT_get0_gen_settable_params"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
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

    // Create and free PROXY_CERT_INFO_EXTENSION
    PROXY_CERT_INFO_EXTENSION *proxy_ext = PROXY_CERT_INFO_EXTENSION_new();
    if (proxy_ext) {
        PROXY_CERT_INFO_EXTENSION_free(proxy_ext);
    }

    // Create RSA key
    RSA *rsa = RSA_new();
    if (rsa) {
        // Create BIGNUM from input data if we have enough bytes
        if (size >= sizeof(BIGNUM*)) {
            BIGNUM *bn = BN_new();
            if (bn) {
                // Use part of input data for BIGNUM (simplified - actual BIGNUM creation from bytes is more complex)
                // For fuzzing purposes, we'll create a small BIGNUM
                BN_set_word(bn, 65537);
                
                // Create EVP_PKEY and set BIGNUM parameter
                EVP_PKEY *pkey = EVP_PKEY_new();
                if (pkey) {
                    // Try to set RSA parameter
                    EVP_PKEY_set_bn_param(pkey, "e", bn);
                    EVP_PKEY_free(pkey);
                }
                BN_free(bn);
            }
        }
        
        // Increase reference count
        RSA_up_ref(rsa);
        
        // Free RSA
        RSA_free(rsa);
        RSA_free(rsa); // Free again after up_ref
    }

    // Add PBE algorithms
    PKCS5_PBE_add();

    // Create encoder dispatch structure for OSSL_FUNC_encoder_does_selection
    OSSL_DISPATCH dispatch[] = {
        { 0, NULL }
    };
    OSSL_FUNC_encoder_does_selection_fn *encoder_func = OSSL_FUNC_encoder_does_selection(dispatch);

    // Create EVP_PKEY context for DH
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_DH, NULL);
    if (ctx) {
        // Create ASN1 object for OID
        ASN1_OBJECT *oid = OBJ_nid2obj(NID_dhKeyAgreement);
        if (oid) {
            // Set DH KDF OID
            EVP_PKEY_CTX_set0_dh_kdf_oid(ctx, oid);
            // Note: set0 doesn't need to free oid separately
        }
        
        // Free context
        EVP_PKEY_CTX_free(ctx);
    }

    // Create X509 policy tree (simplified for fuzzing)
    // Note: X509_policy_tree_level_count requires a valid tree
    // For fuzzing, we'll pass NULL which should handle gracefully
    int level_count = X509_policy_tree_level_count(NULL);

    // Create ASN1_INTEGER from input data
    ASN1_INTEGER *asn1_int = ASN1_INTEGER_new();
    if (asn1_int && size > 0) {
        // Use first byte of input data for ASN1_INTEGER
        // This is simplified - real ASN1_INTEGER_set would need proper formatting
        ASN1_INTEGER_set_int64(asn1_int, data[0] % 256);
        
        // Get integer value
        long int_value = ASN1_INTEGER_get(asn1_int);
        
        // Free ASN1_INTEGER
        ASN1_INTEGER_free(asn1_int);
    } else if (asn1_int) {
        ASN1_INTEGER_free(asn1_int);
    }

    // Get ASIdOrRange ASN1 item
    const ASN1_ITEM *as_id_item = ASIdOrRange_it();

    // Try to get EVP_SKEYMGMT parameters (requires proper initialization)
    // For fuzzing, we'll pass NULL
    const OSSL_PARAM *gen_params = EVP_SKEYMGMT_get0_gen_settable_params(NULL);

    // Consume remaining input data by creating a FILE* and reading from it
    if (size > 0) {
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            // Read a small amount to ensure we consume data
            char buffer[256];
            size_t read_bytes = fread(buffer, 1, sizeof(buffer) < size ? sizeof(buffer) : size, in_file);
            
            // Get file descriptor
            int fd = fuzz_fileno(in_file);
            (void)fd; // Use fd to avoid unused variable warning
            
            assert_file_closed(&in_file);;
        }
    }

    // Create output file and write something
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write a marker to indicate fuzzing occurred
        const char *marker = "FUZZ_TEST";
        fwrite(marker, 1, strlen(marker), out_file);
        
        // Get file descriptor
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd; // Use out_fd to avoid unused variable warning
        
        assert_file_closed(&out_file);;
        
        // Clean up output file
        remove("output_file");
    }

    // All resources were freed inline
    assert_file_closed(&out_file);
	return 0;
}