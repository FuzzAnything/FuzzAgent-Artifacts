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
//<ID> 381
//<Prompt> []
/*<Combination>: [
*/
//<score> 70, nr_unique_branch: 9
//<Quality> {"density":14,"unique_branches":{"def_load_bio":[[382,36,382,51,0,0,4,0]],"SHA256_Final":[[247,9,247,30,0,0,4,0]],"str_copy":[[709,22,709,31,0,0,4,0],[714,17,714,18,0,0,4,0],[735,17,735,18,0,0,4,0],[736,21,736,27,0,0,4,0]],"ossl_rsa_padding_add_PKCS1_PSS_mgf1":[[207,9,207,39,0,0,4,0],[230,9,230,36,0,0,4,1],[234,16,234,39,0,0,4,0]]},"library_calls":["PEM_do_header","X509_CRL_new","NCONF_new","BIO_new_mem_buf","NCONF_load_bio","BIO_free","X509V3_EXT_CRL_add_nconf","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","BN_free","EVP_MD_CTX_new","EVP_DigestInit_ex","EVP_sha256","EVP_DigestUpdate","EVP_DigestFinal_ex","EVP_MD_CTX_free","RSA_padding_add_PKCS1_PSS","EVP_sha256","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_get_auth_level","OSSL_FUNC_encoder_gettable_params","X509_CRL_free","NCONF_free","RSA_free","X509_VERIFY_PARAM_free"],"critical_calls":["PEM_do_header","X509_CRL_new","NCONF_new","BIO_new_mem_buf","NCONF_load_bio","BIO_free","X509V3_EXT_CRL_add_nconf","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","BN_free","EVP_MD_CTX_new","EVP_DigestInit_ex","EVP_sha256","EVP_DigestUpdate","EVP_DigestFinal_ex","EVP_MD_CTX_free","RSA_padding_add_PKCS1_PSS","EVP_sha256","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_get_auth_level","OSSL_FUNC_encoder_gettable_params","X509_CRL_free","NCONF_free","RSA_free","X509_VERIFY_PARAM_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Minimum size for meaningful operations
    
    // 1. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Prepare for PEM_do_header
    EVP_CIPHER_INFO cipher_info;
    unsigned char *pem_data = (unsigned char *)malloc(size);
    long pem_len = size;
    
    if (!pem_data) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    memcpy(pem_data, data, size);
    
    // Dummy password callback
    auto dummy_callback = [](char *buf, int size, int rwflag, void *userdata) -> int {
        return 0; // No password
    };
    
    // Call PEM_do_header
    int pem_result = PEM_do_header(&cipher_info, pem_data, &pem_len, dummy_callback, nullptr);
    
    // 3. Prepare X509_CRL for X509V3_EXT_CRL_add_nconf
    X509_CRL *crl = X509_CRL_new();
    if (!crl) {
        free(pem_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Prepare CONF for X509V3_EXT_CRL_add_nconf
    CONF *conf = NCONF_new(nullptr);
    X509V3_CTX ctx;
    
    if (conf) {
        // Initialize X509V3_CTX
        memset(&ctx, 0, sizeof(ctx));
        
        // Load configuration from input data
        BIO *conf_bio = BIO_new_mem_buf(data, size);
        if (conf_bio) {
            NCONF_load_bio(conf, conf_bio, nullptr);
            BIO_free(conf_bio);
        }
        
        // Call X509V3_EXT_CRL_add_nconf
        X509V3_EXT_CRL_add_nconf(conf, &ctx, "default", crl);
    }
    
    // 5. Prepare for RSA_padding_add_PKCS1_PSS
    RSA *rsa = RSA_new();
    if (rsa) {
        // Generate a minimal RSA key for testing
        BIGNUM *e = BN_new();
        if (e) {
            BN_set_word(e, 65537);
            RSA_generate_key_ex(rsa, 512, e, nullptr);
            BN_free(e);
        }
        
        // Prepare data for PSS padding
        unsigned char EM[256];
        unsigned char mHash[EVP_MAX_MD_SIZE];
        unsigned int hash_len = 0;
        
        // Create a hash from input data
        EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
        if (md_ctx) {
            if (EVP_DigestInit_ex(md_ctx, EVP_sha256(), nullptr)) {
                EVP_DigestUpdate(md_ctx, data, size);
                EVP_DigestFinal_ex(md_ctx, mHash, &hash_len);
            }
            EVP_MD_CTX_free(md_ctx);
        }
        
        // Call RSA_padding_add_PKCS1_PSS
        if (hash_len > 0) {
            RSA_padding_add_PKCS1_PSS(rsa, EM, mHash, EVP_sha256(), -1);
        }
    }
    
    // 6. Prepare for X509_VERIFY_PARAM_get_auth_level
    X509_VERIFY_PARAM *param = X509_VERIFY_PARAM_new();
    if (param) {
        // Call X509_VERIFY_PARAM_get_auth_level
        int auth_level = X509_VERIFY_PARAM_get_auth_level(param);
        (void)auth_level; // Use result to avoid unused variable warning
    }
    
    // 7. Prepare for OSSL_FUNC_encoder_gettable_params
    // Create a minimal OSSL_DISPATCH structure
    OSSL_DISPATCH dispatch[] = {
        {0, nullptr}
    };
    
    // Call OSSL_FUNC_encoder_gettable_params
    OSSL_FUNC_encoder_gettable_params_fn *gettable_params = 
        OSSL_FUNC_encoder_gettable_params(dispatch);
    (void)gettable_params; // Use result to avoid unused variable warning
    
    // 8. Create output file for writing (optional)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some processed data if needed
        fwrite(pem_data, 1, pem_len, out_file);
        assert_file_closed(&out_file);;
    }
    
    // 9. Clean up all allocated resources
    free(pem_data);
    assert_file_closed(&in_file);;
    
    if (crl) X509_CRL_free(crl);
    if (conf) NCONF_free(conf);
    if (rsa) RSA_free(rsa);
    if (param) X509_VERIFY_PARAM_free(param);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}