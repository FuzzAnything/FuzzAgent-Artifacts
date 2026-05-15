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
//<ID> 238
//<Prompt> []
/*<Combination>: [
*/
//<score> 6.5, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","BIO_new_mem_buf","d2i_X509_REQ_bio","BIO_new","BIO_s_mem","i2d_X509_REQ_bio","BIO_free","X509_REQ_free","BIO_ctrl","d2i_GENERAL_NAME","i2d_GENERAL_NAME","CRYPTO_free","GENERAL_NAME_free","EC_GROUP_new_by_curve_name","EC_GROUP_get_ecparameters","ECPARAMETERS_free","EC_GROUP_free","OSSL_INFO_SYNTAX_new","OSSL_INFO_SYNTAX_free","OSSL_USER_NOTICE_SYNTAX_new","OSSL_USER_NOTICE_SYNTAX_free","OSSL_ENCODER_fetch","OSSL_ENCODER_get0_properties","OSSL_ENCODER_free","EVP_RAND_fetch","EVP_RAND_settable_ctx_params","EVP_RAND_free","OSSL_CMP_SRV_CTX_new","OSSL_CMP_SRV_CTX_init_trans","OSSL_CMP_SRV_CTX_free","ASYNC_WAIT_CTX_new","ASYNC_WAIT_CTX_set_wait_fd","ASYNC_WAIT_CTX_get_fd","ASYNC_WAIT_CTX_free","BIO_free"],"critical_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","BIO_new_mem_buf","d2i_X509_REQ_bio","BIO_new","BIO_s_mem","i2d_X509_REQ_bio","BIO_free","X509_REQ_free","BIO_ctrl","d2i_GENERAL_NAME","i2d_GENERAL_NAME","CRYPTO_free","GENERAL_NAME_free","EC_GROUP_new_by_curve_name","EC_GROUP_get_ecparameters","ECPARAMETERS_free","EC_GROUP_free","OSSL_INFO_SYNTAX_new","OSSL_INFO_SYNTAX_free","OSSL_USER_NOTICE_SYNTAX_new","OSSL_USER_NOTICE_SYNTAX_free","OSSL_ENCODER_fetch","OSSL_ENCODER_get0_properties","OSSL_ENCODER_free","EVP_RAND_fetch","EVP_RAND_settable_ctx_params","EVP_RAND_free","OSSL_CMP_SRV_CTX_new","OSSL_CMP_SRV_CTX_init_trans","OSSL_CMP_SRV_CTX_free","ASYNC_WAIT_CTX_new","ASYNC_WAIT_CTX_set_wait_fd","ASYNC_WAIT_CTX_get_fd","ASYNC_WAIT_CTX_free","BIO_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL library
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS, NULL);
    
    // Create a memory BIO from input data
    BIO *mem_bio = BIO_new_mem_buf(data, size);
    if (!mem_bio) {
        return 0;
    }
    
    // 1. Try to parse X509_REQ from input data
    X509_REQ *req = d2i_X509_REQ_bio(mem_bio, NULL);
    if (req) {
        // Write it back to a BIO using i2d_X509_REQ_bio
        BIO *out_bio = BIO_new(BIO_s_mem());
        if (out_bio) {
            i2d_X509_REQ_bio(out_bio, req);
            BIO_free(out_bio);
        }
        X509_REQ_free(req);
    }
    
    // Reset BIO position for next operations
    (void)BIO_reset(mem_bio);
    
    // 2. Try to parse GENERAL_NAME from input data
    const unsigned char *p = data;
    GENERAL_NAME *gen_name = d2i_GENERAL_NAME(NULL, &p, size);
    if (gen_name) {
        // Use i2d_GENERAL_NAME
        unsigned char *out = NULL;
        int len = i2d_GENERAL_NAME(gen_name, &out);
        if (out) {
            OPENSSL_free(out);
        }
        GENERAL_NAME_free(gen_name);
    }
    
    // 3. Create and use EC_GROUP with ECPARAMETERS
    EC_GROUP *group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    if (group) {
        ECPARAMETERS *params = EC_GROUP_get_ecparameters(group, NULL);
        if (params) {
            // Use the parameters - in real usage we'd do something with them
            ECPARAMETERS_free(params);
        }
        EC_GROUP_free(group);
    }
    
    // 4. Create and free OSSL_INFO_SYNTAX
    OSSL_INFO_SYNTAX *info_syntax = OSSL_INFO_SYNTAX_new();
    if (info_syntax) {
        OSSL_INFO_SYNTAX_free(info_syntax);
    }
    
    // 5. Create and free OSSL_USER_NOTICE_SYNTAX
    OSSL_USER_NOTICE_SYNTAX *user_notice = OSSL_USER_NOTICE_SYNTAX_new();
    if (user_notice) {
        OSSL_USER_NOTICE_SYNTAX_free(user_notice);
    }
    
    // 6. Get OSSL_ENCODER properties
    OSSL_ENCODER *encoder = OSSL_ENCODER_fetch(NULL, "DER", NULL);
    if (encoder) {
        const char *props = OSSL_ENCODER_get0_properties(encoder);
        (void)props; // Use props to avoid unused variable warning
        OSSL_ENCODER_free(encoder);
    }
    
    // 7. Get EVP_RAND settable context parameters
    EVP_RAND *rand = EVP_RAND_fetch(NULL, "CTR-DRBG", NULL);
    if (rand) {
        const OSSL_PARAM *params = EVP_RAND_settable_ctx_params(rand);
        (void)params; // Use params to avoid unused variable warning
        EVP_RAND_free(rand);
    }
    
    // 8. Initialize OSSL_CMP_SRV_CTX transaction
    OSSL_CMP_SRV_CTX *srv_ctx = OSSL_CMP_SRV_CTX_new(NULL, NULL);
    if (srv_ctx) {
        // Create dummy callbacks
        auto delay_cb = [](OSSL_CMP_SRV_CTX *, const OSSL_CMP_MSG *) -> int { return 1; };
        auto clean_cb = [](OSSL_CMP_SRV_CTX *, const ASN1_OCTET_STRING *) -> int { return 1; };
        
        OSSL_CMP_SRV_CTX_init_trans(srv_ctx, delay_cb, clean_cb);
        OSSL_CMP_SRV_CTX_free(srv_ctx);
    }
    
    // 9. Use ASYNC_WAIT_CTX with file descriptor
    ASYNC_WAIT_CTX *wait_ctx = ASYNC_WAIT_CTX_new();
    if (wait_ctx) {
        int fd;
        void *custom_data;
        
        // Create a temporary file from input data
        FILE *tmp_file = fmemopen((void *)data, size, "rb");
        if (tmp_file) {
            int file_fd = fuzz_fileno(tmp_file);
            const char *key = "test_key";
            
            // Add the fd to the wait context first
            if (ASYNC_WAIT_CTX_set_wait_fd(wait_ctx, key, file_fd, NULL, NULL)) {
                // Now get it back
                ASYNC_WAIT_CTX_get_fd(wait_ctx, key, &fd, &custom_data);
            }
            assert_file_closed(&tmp_file);;
        }
        ASYNC_WAIT_CTX_free(wait_ctx);
    }
    
    // Clean up
    BIO_free(mem_bio);
    
    // Also try to create output file as mentioned in instructions
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some of the input data to output file
        if (size > 0) {
            fwrite(data, 1, size > 100 ? 100 : size, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    assert_file_closed(&out_file);
	return 0;
}