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
//<ID> 274
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 3
//<Quality> {"density":14,"unique_branches":{"d2i_PKCS7_fp":[[306,9,306,19,0,0,4,0],[306,23,306,34,0,0,4,1],[312,9,312,20,0,0,4,0]]},"library_calls":["OPENSSL_init_crypto","OSSL_LIB_CTX_new","RAND_get0_primary","OSSL_LIB_CTX_free","d2i_PKCS7_fp","EVP_PKEY_CTX_new_id","EVP_PKEY_keygen_init","EVP_PKEY_CTX_set_ec_paramgen_curve_nid","EVP_PKEY_keygen","X509_new","X509_set_version","X509_get_serialNumber","ASN1_INTEGER_set","X509_getm_notBefore","X509_gmtime_adj","X509_getm_notAfter","X509_gmtime_adj","X509_set_pubkey","PEM_read_X509","d2i_X509_fp","BIO_new","BIO_s_file","BIO_free","OPENSSL_sk_new_null","ossl_check_OSSL_ROLE_SPEC_CERT_ID_sk_type","OPENSSL_sk_set_thunks","OPENSSL_sk_free","POLICYINFO_new","ossl_check_POLICYINFO_type","POLICYINFO_free","DH_meth_new","DH_meth_get0_app_data","DH_meth_free","X509_CRL_new","X509_CRL_get_ext_by_critical","X509_CRL_free","BN_new","BN_bin2bn","BN_num_bits","BN_num_bits","BN_signed_bn2lebin","BN_free","CRYPTO_realloc","EVP_PKEY_get_group_name","EVP_PKEY_free","X509_free","X509_free","X509_free","EVP_PKEY_CTX_free","PKCS7_free","OSSL_LIB_CTX_free"],"critical_calls":["OPENSSL_init_crypto","OSSL_LIB_CTX_new","RAND_get0_primary","d2i_PKCS7_fp","EVP_PKEY_CTX_new_id","EVP_PKEY_keygen_init","EVP_PKEY_CTX_set_ec_paramgen_curve_nid","EVP_PKEY_keygen","X509_new","X509_set_version","X509_get_serialNumber","ASN1_INTEGER_set","X509_getm_notBefore","X509_gmtime_adj","X509_getm_notAfter","X509_gmtime_adj","X509_set_pubkey","PEM_read_X509","d2i_X509_fp","BIO_new","BIO_s_file","BIO_free","OPENSSL_sk_new_null","ossl_check_OSSL_ROLE_SPEC_CERT_ID_sk_type","OPENSSL_sk_set_thunks","OPENSSL_sk_free","POLICYINFO_new","ossl_check_POLICYINFO_type","POLICYINFO_free","DH_meth_new","DH_meth_get0_app_data","DH_meth_free","X509_CRL_new","X509_CRL_get_ext_by_critical","X509_CRL_free","BN_new","BN_bin2bn","BN_num_bits","BN_num_bits","BN_signed_bn2lebin","BN_free","CRYPTO_realloc","EVP_PKEY_get_group_name","EVP_PKEY_free","X509_free","X509_free","X509_free","EVP_PKEY_CTX_free","PKCS7_free","OSSL_LIB_CTX_free"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified OpenSSL APIs to achieve a comprehensive certificate and key processing event:

*/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Initialize OpenSSL
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    
    // 1. Create library context and get primary random
    OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
    EVP_RAND_CTX *rand_ctx = RAND_get0_primary(libctx);
    
    // 2. Create FILE* from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        OSSL_LIB_CTX_free(libctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Read PKCS7 structure from file
    PKCS7 *p7 = NULL;
    p7 = d2i_PKCS7_fp(in_file, &p7);
    
    // Rewind for other reads
    rewind(in_file);
    
    // 4. Create EC key pair
    EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
    if (pctx) {
        EVP_PKEY_keygen_init(pctx);
        EVP_PKEY_CTX_set_ec_paramgen_curve_nid(pctx, NID_X9_62_prime256v1);
        
        EVP_PKEY *pkey = NULL;
        EVP_PKEY_keygen(pctx, &pkey);
        
        // 5. Create and populate X509 certificate
        X509 *cert = X509_new();
        if (cert) {
            X509_set_version(cert, 2); // X509v3
            
            // Set serial number
            ASN1_INTEGER *serial = X509_get_serialNumber(cert);
            ASN1_INTEGER_set(serial, 12345);
            
            // Set validity period
            ASN1_TIME *notBefore = X509_getm_notBefore(cert);
            X509_gmtime_adj(notBefore, 0);
            
            ASN1_TIME *notAfter = X509_getm_notAfter(cert);
            X509_gmtime_adj(notAfter, 365 * 24 * 3600); // 1 year
            
            // Set public key
            if (pkey) {
                X509_set_pubkey(cert, pkey);
            }
        }
        
        // Read X509 in different formats
        rewind(in_file);
        X509 *cert2 = NULL;
        cert2 = PEM_read_X509(in_file, &cert2, NULL, NULL);
        
        rewind(in_file);
        X509 *cert3 = NULL;
        cert3 = d2i_X509_fp(in_file, &cert3);
        
        // Create BIO for file operations
        BIO *bio = BIO_new(BIO_s_file());
        if (bio) {
            BIO_free(bio);
        }
        
        // 6. Stack operations
        OPENSSL_STACK *stack = OPENSSL_sk_new_null();
        if (stack) {
            // Simulate type checking (though we don't have actual OSSL_ROLE_SPEC_CERT_ID)
            OPENSSL_STACK *typed_stack = ossl_check_OSSL_ROLE_SPEC_CERT_ID_sk_type(
                (struct stack_st_OSSL_ROLE_SPEC_CERT_ID *)stack);
            OPENSSL_sk_set_thunks(stack, sk_X509_NAME_ENTRY_freefunc_thunk);
            OPENSSL_sk_free(stack);
        }
        
        // 7. POLICYINFO operations
        POLICYINFO *policy = POLICYINFO_new();
        if (policy) {
            POLICYINFO *checked_policy = ossl_check_POLICYINFO_type(policy);
            POLICYINFO_free(policy);
        }
        
        // 8. DH operations
        DH_METHOD *dhmethod = DH_meth_new("test-dh", 0);
        if (dhmethod) {
            void *app_data = DH_meth_get0_app_data(dhmethod);
            DH_meth_free(dhmethod);
        }
        
        // 9. CRL operations
        X509_CRL *crl = X509_CRL_new();
        if (crl) {
            int ext_pos = X509_CRL_get_ext_by_critical(crl, 0, -1);
            X509_CRL_free(crl);
        }
        
        // 10. BIGNUM operations using input data
        BIGNUM *bn = BN_new();
        if (bn && size > 20) {
            // Use first 20 bytes for BIGNUM
            size_t bn_len = size > 20 ? 20 : size;
            BN_bin2bn(data, bn_len, bn);
            
            int bits = BN_num_bits(bn);
            
            // Convert to little-endian binary
            int req_len = BN_num_bytes(bn);
            unsigned char *le_buf = (unsigned char *)malloc(req_len);
            if (le_buf) {
                BN_signed_bn2lebin(bn, le_buf, req_len);
                free(le_buf);
            }
            
            BN_free(bn);
        }
        
        // 11. CRYPTO_realloc example
        void *alloc_buf = malloc(10);
        if (alloc_buf) {
            void *realloc_buf = CRYPTO_realloc(alloc_buf, 20, __FILE__, __LINE__);
            if (realloc_buf) {
                free(realloc_buf);
            } else {
                free(alloc_buf);
            }
        }
        
        // 12. Get EC group name
        if (pkey) {
            char group_name[256];
            size_t name_len = 0;
            EVP_PKEY_get_group_name(pkey, group_name, sizeof(group_name), &name_len);
            
            EVP_PKEY_free(pkey);
        }
        
        // Cleanup
        if (cert) X509_free(cert);
        if (cert2) X509_free(cert2);
        if (cert3) X509_free(cert3);
        
        EVP_PKEY_CTX_free(pctx);
    }
    
    // 13. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data to output file
        if (size > 0) {
            fwrite(data, 1, size > 100 ? 100 : size, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // 14. Get file descriptor example
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        (void)fd; // Use fd to avoid unused warning
        assert_file_closed(&in_file);;
    }
    
    // Final cleanup
    if (p7) PKCS7_free(p7);
    OSSL_LIB_CTX_free(libctx);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}