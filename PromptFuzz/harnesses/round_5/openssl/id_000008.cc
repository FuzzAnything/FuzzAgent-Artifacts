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
//<ID> 148
//<Prompt> []
/*<Combination>: [
*/
//<score> 88, nr_unique_branch: 36
//<Quality> {"density":11,"unique_branches":{"BIO_fd_should_retry":[[233,9,233,17,0,0,4,0]],"BIO_fd_non_fatal_error":[[251,5,251,21,0,0,4,1],[256,5,256,18,0,0,4,1],[260,5,260,15,0,0,4,1],[270,5,270,16,0,0,4,1],[274,5,274,21,0,0,4,1],[278,5,278,18,0,0,4,1],[281,5,281,12,0,0,4,0]],"fd_free":[[101,9,101,18,0,0,4,1],[103,9,103,20,0,0,4,0],[103,9,103,20,0,0,4,1],[104,13,104,20,0,0,4,1]],"fd_read":[[117,9,117,20,0,0,4,0],[117,24,117,32,0,0,4,0],[122,13,122,21,0,0,4,0],[123,17,123,41,0,0,4,1],[125,22,125,30,0,0,4,0]],"BIO_new_fd":[[84,9,84,20,0,0,4,1]],"PKCS7_dataDecode":[[451,9,451,19,0,0,4,1],[460,9,460,26,0,0,4,0]],"fd_ctrl":[[151,5,151,24,0,0,4,1],[154,5,154,25,0,0,4,1],[157,5,157,25,0,0,4,1],[158,5,158,23,0,0,4,1],[161,5,161,22,0,0,4,0],[161,5,161,22,0,0,4,1],[167,5,167,22,0,0,4,1],[176,5,176,28,0,0,4,1],[179,5,179,28,0,0,4,1],[182,5,182,26,0,0,4,1],[183,5,183,27,0,0,4,1],[186,5,186,22,0,0,4,1],[187,5,187,24,0,0,4,1],[190,5,190,22,0,0,4,0],[190,5,190,22,0,0,4,1],[193,5,193,12,0,0,4,1]]},"library_calls":["ossl_check_OSSL_ROLE_SPEC_CERT_ID_sk_type","CRYPTO_realloc","BN_new","BN_bin2bn","BN_num_bits","BN_signed_bn2lebin","BN_num_bits","OSSL_LIB_CTX_new","RAND_get0_primary","PKCS7_new","EVP_PKEY_new","X509_new","BIO_new_mem_buf","BIO_new","BIO_s_mem","PKCS7_dataDecode","BIO_free","BIO_free","BIO_free","POLICYINFO_new","ossl_check_POLICYINFO_type","POLICYINFO_free","DH_meth_new","DH_meth_get0_app_data","DH_meth_free","X509_CRL_new","X509_CRL_get_ext_by_critical","X509_CRL_free","EVP_PKEY_CTX_new_id","EVP_PKEY_keygen_init","EVP_PKEY_CTX_set_ec_paramgen_curve_nid","EVP_PKEY_keygen","EVP_PKEY_get_group_name","EVP_PKEY_free","EVP_PKEY_CTX_free","BIO_new_fd","BIO_new_fd","BIO_read","BIO_write","BIO_free","BIO_free","BN_free","CRYPTO_free","PKCS7_free","EVP_PKEY_free","X509_free","OSSL_LIB_CTX_free"],"critical_calls":["ossl_check_OSSL_ROLE_SPEC_CERT_ID_sk_type","CRYPTO_realloc","BN_new","BN_bin2bn","BN_num_bits","BN_signed_bn2lebin","BN_num_bits","OSSL_LIB_CTX_new","RAND_get0_primary","PKCS7_new","EVP_PKEY_new","X509_new","BIO_new_mem_buf","BIO_new","BIO_s_mem","PKCS7_dataDecode","BIO_free","BIO_free","BIO_free","POLICYINFO_new","ossl_check_POLICYINFO_type","POLICYINFO_free","DH_meth_new","DH_meth_get0_app_data","DH_meth_free","X509_CRL_new","X509_CRL_get_ext_by_critical","X509_CRL_free","EVP_PKEY_CTX_new_id","EVP_PKEY_keygen_init","EVP_PKEY_CTX_set_ec_paramgen_curve_nid","EVP_PKEY_keygen","EVP_PKEY_get_group_name","EVP_PKEY_free","EVP_PKEY_CTX_free","BIO_new_fd","BIO_new_fd","BIO_read","BIO_write","BIO_free","BIO_free","BN_free","CRYPTO_free","PKCS7_free","EVP_PKEY_free","X509_free","OSSL_LIB_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for operations

    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create FILE* for writing output (if needed)
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Use ossl_check_OSSL_ROLE_SPEC_CERT_ID_sk_type
    // Create a dummy stack structure
    struct stack_st_OSSL_ROLE_SPEC_CERT_ID *stack = NULL;
    OPENSSL_STACK *checked_stack = ossl_check_OSSL_ROLE_SPEC_CERT_ID_sk_type(stack);
    // This is mostly a type cast, but we've used the API

    // 2. Use CRYPTO_realloc for memory allocation
    size_t alloc_size = size > 1024 ? 1024 : size;
    void *buffer = CRYPTO_realloc(NULL, alloc_size, __FILE__, __LINE__);
    if (!buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Read some data into buffer
    fread(buffer, 1, alloc_size, in_file);
    
    // 3. Create and use BIGNUM with BN_signed_bn2lebin
    BIGNUM *bn = BN_new();
    if (bn && alloc_size >= 32) {
        // Convert first 32 bytes to BIGNUM (simplified)
        BN_bin2bn((const unsigned char*)buffer, 32, bn);
        
        // Convert to little-endian signed binary
        unsigned char *le_buf = (unsigned char*)malloc(BN_num_bytes(bn) + 1);
        if (le_buf) {
            int ret = BN_signed_bn2lebin(bn, le_buf, BN_num_bytes(bn));
            free(le_buf);
        }
    }
    
    // 4. Use RAND_get0_primary
    OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
    if (libctx) {
        EVP_RAND_CTX *rand_ctx = RAND_get0_primary(libctx);
        // Just calling the API, result may be NULL if not initialized
    }
    
    // 5. Create dummy PKCS7 structure for PKCS7_dataDecode
    PKCS7 *p7 = PKCS7_new();
    EVP_PKEY *pkey = EVP_PKEY_new();
    X509 *cert = X509_new();
    
    if (p7 && pkey && cert) {
        // Create a simple memory BIO with some data
        BIO *in_bio = BIO_new_mem_buf(buffer, alloc_size);
        BIO *out_bio = BIO_new(BIO_s_mem());
        
        if (in_bio && out_bio) {
            // Try to decode - may fail but we're calling the API
            BIO *decoded = PKCS7_dataDecode(p7, pkey, in_bio, cert);
            if (decoded) {
                BIO_free(decoded);
            }
            
            BIO_free(in_bio);
            BIO_free(out_bio);
        }
    }
    
    // 6. Use ossl_check_POLICYINFO_type
    POLICYINFO *policy_info = POLICYINFO_new();
    if (policy_info) {
        POLICYINFO *checked_policy = ossl_check_POLICYINFO_type(policy_info);
        // Just type checking/casting
        POLICYINFO_free(policy_info);
    }
    
    // 7. Use DH_meth_get0_app_data
    DH_METHOD *dh_method = DH_meth_new("Test DH", 0);
    if (dh_method) {
        void *app_data = DH_meth_get0_app_data(dh_method);
        // app_data will be NULL unless set elsewhere
        DH_meth_free(dh_method);
    }
    
    // 8. Use X509_CRL_get_ext_by_critical
    X509_CRL *crl = X509_CRL_new();
    if (crl) {
        int ext_idx = X509_CRL_get_ext_by_critical(crl, 1, -1);
        // Will likely return -1 (not found) for empty CRL
        X509_CRL_free(crl);
    }
    
    // 9. Use EVP_PKEY_get_group_name
    // Create a simple EC key for testing
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
    if (ctx) {
        if (EVP_PKEY_keygen_init(ctx) > 0) {
            // Set parameters for P-256
            EVP_PKEY_CTX_set_ec_paramgen_curve_nid(ctx, NID_X9_62_prime256v1);
            
            EVP_PKEY *ec_key = NULL;
            if (EVP_PKEY_keygen(ctx, &ec_key) > 0 && ec_key) {
                char group_name[256];
                size_t gname_len = 0;
                int ret = EVP_PKEY_get_group_name(ec_key, group_name, 
                                                 sizeof(group_name), &gname_len);
                EVP_PKEY_free(ec_key);
            }
        }
        EVP_PKEY_CTX_free(ctx);
    }
    
    // 10. Use BIO for reading/writing with file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Create BIO from file descriptors
    BIO *bio_in = BIO_new_fd(in_fd, BIO_NOCLOSE);
    BIO *bio_out = BIO_new_fd(out_fd, BIO_NOCLOSE);
    
    if (bio_in && bio_out) {
        // Read and write some data through BIO
        unsigned char tmp[128];
        int read_len = BIO_read(bio_in, tmp, sizeof(tmp));
        if (read_len > 0) {
            BIO_write(bio_out, tmp, read_len);
        }
        
        BIO_free(bio_in);
        BIO_free(bio_out);
    }
    
    // Cleanup
    if (bn) BN_free(bn);
    if (buffer) CRYPTO_free(buffer, __FILE__, __LINE__);
    if (p7) PKCS7_free(p7);
    if (pkey) EVP_PKEY_free(pkey);
    if (cert) X509_free(cert);
    if (libctx) OSSL_LIB_CTX_free(libctx);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}