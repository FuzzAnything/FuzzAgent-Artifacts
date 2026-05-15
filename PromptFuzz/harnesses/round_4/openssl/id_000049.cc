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
//<ID> 734
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 3
//<Quality> {"density":12,"unique_branches":{"mem_ctrl":[[332,5,332,26,0,0,4,0]],"ASN1_TIME_check":[[332,9,332,42,0,0,4,1],[334,14,334,39,0,0,4,1]]},"library_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","BIO_new_mem_buf","d2i_X509_REQ_bio","BIO_new","BIO_s_mem","i2d_X509_REQ_bio","X509_REQ_get0_pubkey","X509_REQ_get_version","d2i_GENERAL_NAME","i2d_GENERAL_NAME","CRYPTO_free","ASN1_TIME_new","ASN1_TIME_check","ASN1_TIME_free","EC_GROUP_new_by_curve_name","EC_GROUP_get_ecparameters","ECPARAMETERS_free","EC_GROUP_free","OSSL_INFO_SYNTAX_new","OSSL_INFO_SYNTAX_free","OSSL_USER_NOTICE_SYNTAX_new","OSSL_USER_NOTICE_SYNTAX_free","EVP_aes_256_cfb1","EVP_CIPHER_get0_provider","OSSL_ENCODER_fetch","OSSL_ENCODER_get0_properties","OSSL_ENCODER_free","EVP_RAND_fetch","EVP_RAND_settable_ctx_params","EVP_RAND_free","OSSL_CMP_SRV_CTX_new","OSSL_CMP_SRV_CTX_init_trans","OSSL_CMP_SRV_CTX_free","ASYNC_WAIT_CTX_new","ASYNC_WAIT_CTX_set_wait_fd","ASYNC_WAIT_CTX_get_fd","ASYNC_WAIT_CTX_free","BIO_ctrl","BIO_ctrl","EVP_sha256","EVP_MD_get0_provider","GENERAL_NAME_free","X509_REQ_free","BIO_free","BIO_free"],"critical_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","BIO_new_mem_buf","d2i_X509_REQ_bio","BIO_new","BIO_s_mem","i2d_X509_REQ_bio","X509_REQ_get0_pubkey","X509_REQ_get_version","d2i_GENERAL_NAME","i2d_GENERAL_NAME","CRYPTO_free","ASN1_TIME_new","ASN1_TIME_check","ASN1_TIME_free","EC_GROUP_new_by_curve_name","EC_GROUP_get_ecparameters","ECPARAMETERS_free","EC_GROUP_free","OSSL_INFO_SYNTAX_new","OSSL_INFO_SYNTAX_free","OSSL_USER_NOTICE_SYNTAX_new","OSSL_USER_NOTICE_SYNTAX_free","EVP_aes_256_cfb1","EVP_CIPHER_get0_provider","OSSL_ENCODER_fetch","OSSL_ENCODER_get0_properties","OSSL_ENCODER_free","EVP_RAND_fetch","EVP_RAND_settable_ctx_params","EVP_RAND_free","OSSL_CMP_SRV_CTX_new","OSSL_CMP_SRV_CTX_init_trans","OSSL_CMP_SRV_CTX_free","ASYNC_WAIT_CTX_new","ASYNC_WAIT_CTX_set_wait_fd","ASYNC_WAIT_CTX_get_fd","ASYNC_WAIT_CTX_free","BIO_ctrl","BIO_ctrl","EVP_sha256","EVP_MD_get0_provider","GENERAL_NAME_free","X509_REQ_free","BIO_free","BIO_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Initialize OpenSSL crypto and SSL libraries
    OPENSSL_init_crypto(0, NULL);
    OPENSSL_init_ssl(0, NULL);
    
    // Create memory BIO from input data
    BIO *mem_bio = BIO_new_mem_buf(data, size);
    if (!mem_bio) {
	return 0;
	}
	
    
    // Parse X509 certificate request from the input data
    X509_REQ *req = d2i_X509_REQ_bio(mem_bio, NULL);
    
    // Create another memory BIO for output operations
    BIO *out_bio = BIO_new(BIO_s_mem());
    
    if (req && out_bio) {
        // Convert X509_REQ back to DER format
        i2d_X509_REQ_bio(out_bio, req);
        
        // Check if there's any ASN1_TIME in the request
        if (X509_REQ_get0_pubkey(req)) {
            // Get the version (which involves ASN1_INTEGER operations)
            long version = X509_REQ_get_version(req);
        }
    }
    
    // Parse GENERAL_NAME from the remaining input data
    const unsigned char *p = data;
    GENERAL_NAME *gn = d2i_GENERAL_NAME(NULL, &p, size);
    
    if (gn && out_bio) {
        // Convert GENERAL_NAME back to DER format
        unsigned char *out_ptr = NULL;
        i2d_GENERAL_NAME(gn, &out_ptr);
        if (out_ptr) {
            CRYPTO_free(out_ptr, __FILE__, __LINE__);
        }
    }
    
    // Test ASN1_TIME operations
    ASN1_TIME *time = ASN1_TIME_new();
    if (time) {
        ASN1_TIME_check(time);
        ASN1_TIME_free(time);
    }
    
    // Test EC group operations
    EC_GROUP *ec_group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    if (ec_group) {
        ECPARAMETERS *ecparams = EC_GROUP_get_ecparameters(ec_group, NULL);
        if (ecparams) {
            ECPARAMETERS_free(ecparams);
        }
        EC_GROUP_free(ec_group);
    }
    
    // Test OSSL_INFO_SYNTAX operations
    OSSL_INFO_SYNTAX *info_syntax = OSSL_INFO_SYNTAX_new();
    if (info_syntax) {
        OSSL_INFO_SYNTAX_free(info_syntax);
    }
    
    // Test OSSL_USER_NOTICE_SYNTAX operations
    OSSL_USER_NOTICE_SYNTAX *user_notice = OSSL_USER_NOTICE_SYNTAX_new();
    if (user_notice) {
        OSSL_USER_NOTICE_SYNTAX_free(user_notice);
    }
    
    // Test EVP_CIPHER operations
    const EVP_CIPHER *cipher = EVP_aes_256_cfb1();
    if (cipher) {
        // Get provider information
        const OSSL_PROVIDER *provider = EVP_CIPHER_get0_provider(cipher);
    }
    
    // Test OSSL_ENCODER operations
    OSSL_ENCODER *encoder = OSSL_ENCODER_fetch(NULL, "DER", NULL);
    if (encoder) {
        const char *props = OSSL_ENCODER_get0_properties(encoder);
        OSSL_ENCODER_free(encoder);
    }
    
    // Test EVP_RAND operations
    EVP_RAND *rand = EVP_RAND_fetch(NULL, "CTR-DRBG", NULL);
    if (rand) {
        const OSSL_PARAM *rand_params = EVP_RAND_settable_ctx_params(rand);
        EVP_RAND_free(rand);
    }
    
    // Test OSSL_CMP_SRV_CTX operations
    OSSL_CMP_SRV_CTX *srv_ctx = OSSL_CMP_SRV_CTX_new(NULL, NULL);
    if (srv_ctx) {
        // Initialize with dummy callbacks
        OSSL_CMP_SRV_CTX_init_trans(srv_ctx, 
            [](OSSL_CMP_SRV_CTX *ctx, const OSSL_CMP_MSG *msg) -> int { return 0; },
            [](OSSL_CMP_SRV_CTX *ctx, const ASN1_OCTET_STRING *id) -> int { return 0; });
        OSSL_CMP_SRV_CTX_free(srv_ctx);
    }
    
    // Test ASYNC_WAIT_CTX operations
    ASYNC_WAIT_CTX *wait_ctx = ASYNC_WAIT_CTX_new();
    if (wait_ctx) {
        // Create a temporary file for file descriptor operations
        FILE *temp_file = fopen("temp_fuzz_file", "wb");
        if (temp_file) {
            // Write some data to the file
            fwrite(data, 1, size > 100 ? 100 : size, temp_file);
            fflush(temp_file);
            
            int fd = fuzz_fileno(temp_file);
            const char *key = "test_key";
            
            // Set wait file descriptor
            ASYNC_WAIT_CTX_set_wait_fd(wait_ctx, key, fd, NULL, NULL);
            
            // Get the file descriptor back
            int retrieved_fd;
            void *custom_data;
            ASYNC_WAIT_CTX_get_fd(wait_ctx, key, &retrieved_fd, &custom_data);
            
            assert_file_closed(&temp_file);;
            remove("temp_fuzz_file");
        }
        ASYNC_WAIT_CTX_free(wait_ctx);
    }
    
    // Use BIO_ctrl for various operations
    if (mem_bio) {
        // Get the amount of data pending in the BIO
        long pending = BIO_ctrl(mem_bio, BIO_CTRL_PENDING, 0, NULL);
        
        // Reset the BIO to beginning
        BIO_ctrl(mem_bio, BIO_CTRL_RESET, 0, NULL);
    }
    
    // Test EVP_MD provider operations
    const EVP_MD *md = EVP_sha256();
    if (md) {
        const OSSL_PROVIDER *md_provider = EVP_MD_get0_provider(md);
    }
    
    // Clean up all allocated resources
    if (gn) GENERAL_NAME_free(gn);
    if (req) X509_REQ_free(req);
    if (mem_bio) BIO_free(mem_bio);
    if (out_bio) BIO_free(out_bio);
    
    return 0;
}