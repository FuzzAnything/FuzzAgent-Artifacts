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
//<ID> 383
//<Prompt> []
/*<Combination>: [
*/
//<score> 104, nr_unique_branch: 53
//<Quality> {"density":16,"unique_branches":{"asn1_template_ex_i2d":[[224,9,224,32,0,0,4,0]],"scan_quote":[[32,32,32,61,2,0,4,0]],"rand_get0_public":[[791,13,791,87,0,0,4,1]],"SHA256_Update_thunk":[[187,13,187,31,0,0,4,1],[187,35,187,57,0,0,4,0],[187,35,187,57,0,0,4,1],[22,24,22,50,5,0,4,0]],"asn1_get_const_enc_ptr":[[136,9,136,20,0,0,4,1],[136,24,136,62,0,0,4,0],[136,24,136,62,0,0,4,1]],"def_load_bio":[[437,35,437,44,0,0,4,0],[460,21,460,30,0,0,4,0]],"PKCS1_MGF1":[[360,9,360,18,0,0,4,1],[363,9,363,19,0,0,4,1],[366,17,366,29,0,0,4,0],[366,17,366,29,0,0,4,1],[373,13,373,46,0,0,4,1],[374,16,374,51,0,0,4,1],[375,16,375,44,0,0,4,1],[377,13,377,34,0,0,4,1],[382,17,382,49,0,0,4,1]],"ossl_asn1_enc_restore":[[191,9,191,20,0,0,4,1],[191,24,191,37,0,0,4,0]],"ASN1_item_ex_i2d":[[169,13,169,20,0,0,4,0],[169,24,169,65,0,0,4,1]],"clear_comments":[[53,29,53,53,7,0,4,0]],"ossl_rsa_padding_add_PKCS1_PSS_mgf1":[[207,9,207,39,0,0,4,1],[209,16,209,48,0,0,4,0],[230,9,230,36,0,0,4,0],[232,13,232,25,0,0,4,1],[238,9,238,17,0,0,4,0],[240,13,240,25,0,0,4,1],[242,13,242,59,0,0,4,1],[248,9,248,20,0,0,4,1],[250,9,250,44,0,0,4,1],[251,12,251,58,0,0,4,1],[252,12,252,47,0,0,4,1],[254,9,254,13,0,0,4,0],[254,17,254,51,0,0,4,1],[256,9,256,42,0,0,4,1],[260,9,260,55,0,0,4,1],[271,9,271,17,0,0,4,0],[272,21,272,29,0,0,4,0],[272,21,272,29,0,0,4,1],[275,9,275,15,0,0,4,0]],"str_copy":[[687,22,687,30,0,0,4,1],[689,22,689,30,0,0,4,1],[691,22,691,30,0,0,4,1],[693,22,693,30,0,0,4,1],[32,32,32,61,3,0,4,0],[33,32,33,61,7,0,4,0],[32,32,32,61,8,0,4,0],[32,32,32,61,8,0,4,1]]},"library_calls":["PEM_do_header","X509_CRL_new","NCONF_new","BIO_new_mem_buf","NCONF_load_bio","BIO_free","X509V3_EXT_CRL_add_nconf","NCONF_free","OSSL_FUNC_encoder_gettable_params","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_set_auth_level","X509_VERIFY_PARAM_get_auth_level","X509_VERIFY_PARAM_free","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","EVP_sha256","EVP_MD_CTX_new","EVP_DigestInit_ex","EVP_DigestUpdate","EVP_DigestFinal_ex","EVP_MD_CTX_free","RSA_size","CRYPTO_malloc","RSA_padding_add_PKCS1_PSS","CRYPTO_free","BN_free","RSA_free","PEM_write_X509_CRL","X509_CRL_free"],"critical_calls":["PEM_do_header","X509_CRL_new","NCONF_new","BIO_new_mem_buf","NCONF_load_bio","BIO_free","X509V3_EXT_CRL_add_nconf","NCONF_free","OSSL_FUNC_encoder_gettable_params","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_set_auth_level","X509_VERIFY_PARAM_get_auth_level","X509_VERIFY_PARAM_free","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","EVP_sha256","EVP_MD_CTX_new","EVP_DigestInit_ex","EVP_DigestUpdate","EVP_DigestFinal_ex","EVP_MD_CTX_free","RSA_size","CRYPTO_malloc","RSA_padding_add_PKCS1_PSS","CRYPTO_free","BN_free","RSA_free","PEM_write_X509_CRL","X509_CRL_free"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified OpenSSL APIs to achieve an event involving CRL extension processing with encrypted PEM data, encoder parameter retrieval, and RSA-PSS signature verification:

*/


// Password callback for PEM_do_header
static int  id_000041_password_callback(char *buf, int size, int rwflag, void *u) {
    const char *password = "test"; // Simple test password
    int len = strlen(password);
    if (len > size) len = size;
    memcpy(buf, password, len);
    return len;
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for processing

    // Create a FILE* from input data
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

    // 1. Process PEM header (using first 64 bytes for PEM data)
    EVP_CIPHER_INFO cipher_info;
    unsigned char pem_data[64];
    long pem_len = sizeof(pem_data);
    
    // Copy some data for PEM processing
    size_t copy_size = size < 64 ? size : 64;
    memcpy(pem_data, data, copy_size);
    
    int pem_result = PEM_do_header(&cipher_info, pem_data, &pem_len, 
                                    id_000041_password_callback, NULL);

    // 2. Create and configure X509_CRL for extension addition
    X509_CRL *crl = X509_CRL_new();
    if (!crl) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create CONF structure from input data
    CONF *conf = NCONF_new(NULL);
    if (conf) {
        // Create X509V3_CTX
        X509V3_CTX ctx;
        memset(&ctx, 0, sizeof(ctx));
        
        // Use part of input data as configuration
        const char *conf_data = reinterpret_cast<const char*>(data);
        size_t conf_len = size < 256 ? size : 256;
        
        BIO *conf_bio = BIO_new_mem_buf(conf_data, conf_len);
        if (conf_bio) {
            NCONF_load_bio(conf, conf_bio, NULL);
            BIO_free(conf_bio);
        }

        // Add extensions to CRL
        int add_result = X509V3_EXT_CRL_add_nconf(conf, &ctx, "default", crl);
        
        NCONF_free(conf);
    }

    // 3. Get encoder gettable parameters (simulate encoder usage)
    // Create a dummy dispatch table for encoder
    static OSSL_DISPATCH dummy_dispatch[] = {
        {0, NULL}
    };
    
    OSSL_FUNC_encoder_gettable_params_fn *gettable_params = 
        OSSL_FUNC_encoder_gettable_params(dummy_dispatch);
    
    // If we had a real encoder, we would use gettable_params here
    // For fuzzing, just ensure the function pointer exists
    (void)gettable_params;

    // 4. Get authentication level from verify param
    X509_VERIFY_PARAM *param = X509_VERIFY_PARAM_new();
    if (param) {
        // Set some parameters
        X509_VERIFY_PARAM_set_auth_level(param, 2);
        
        // Get auth level back
        int auth_level = X509_VERIFY_PARAM_get_auth_level(param);
        (void)auth_level; // Use result to avoid unused variable warning
        
        X509_VERIFY_PARAM_free(param);
    }

    // 5. RSA PSS padding operations
    // Create RSA key
    RSA *rsa = RSA_new();
    if (rsa) {
        // Generate a simple RSA key for testing
        BIGNUM *e = BN_new();
        BN_set_word(e, 65537);
        
        RSA_generate_key_ex(rsa, 512, e, NULL);
        
        // Prepare message hash (use part of input data)
        const EVP_MD *md = EVP_sha256();
        unsigned char mHash[EVP_MAX_MD_SIZE];
        unsigned int hashLen = 0;
        
        // Create hash from input data
        EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
        if (md_ctx) {
            EVP_DigestInit_ex(md_ctx, md, NULL);
            EVP_DigestUpdate(md_ctx, data, size < 100 ? size : 100);
            EVP_DigestFinal_ex(md_ctx, mHash, &hashLen);
            EVP_MD_CTX_free(md_ctx);
        }
        
        // Allocate buffer for padded message
        int rsa_size = RSA_size(rsa);
        unsigned char *EM = (unsigned char *)OPENSSL_malloc(rsa_size);
        
        if (EM && hashLen > 0) {
            // Add PSS padding
            int pad_result = RSA_padding_add_PKCS1_PSS(rsa, EM, mHash, md, -2);
            (void)pad_result; // Use result
            
            OPENSSL_free(EM);
        }
        
        BN_free(e);
        RSA_free(rsa);
    }

    // Write some output to file (simulating CRL export)
    if (crl) {
        PEM_write_X509_CRL(out_file, crl);
        X509_CRL_free(crl);
    }

    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}