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
//<ID> 182
//<Prompt> []
/*<Combination>: [
*/
//<score> 12.666667, nr_unique_branch: 5
//<Quality> {"density":19,"unique_branches":{"BN_add_word":[[119,27,119,37,0,0,4,1],[121,13,121,20,0,0,4,0],[123,9,123,10,0,0,4,0],[123,14,123,25,0,0,4,0],[124,13,124,46,0,0,4,1]]},"library_calls":["EVP_CIPHER_CTX_new","BN_CTX_new","OSSL_LIB_CTX_new","EVP_CIPHER_CTX_set_app_data","EVP_aes_256_cbc","EVP_CIPHER_get_mode","OSSL_PROVIDER_load","RAND_set1_random_provider","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_bin2bn","EC_GROUP_new_curve_GFp","EC_KEY_new","EC_KEY_set_group","EVP_PKEY_new","EVP_PKEY_assign","i2d_KeyParams","CRYPTO_free","OSSL_CRMF_CERTID_new","OSSL_CRMF_CERTID_dup","OSSL_CRMF_CERTID_free","OSSL_CRMF_CERTID_free","OSSL_ATTRIBUTES_SYNTAX_it","X509_NAME_new","ASN1_STRING_new","ASN1_STRING_set","X509_NAME_add_entry_by_txt","ASN1_STRING_get0_data","ASN1_STRING_length","ASN1_STRING_free","OSSL_STORE_SEARCH_by_name","OSSL_STORE_SEARCH_get0_name","OSSL_STORE_SEARCH_free","X509_NAME_free","CRYPTO_free","CRYPTO_free","ossl_check_IPAddressFamily_freefunc_type","EVP_CIPHER_CTX_free","BN_CTX_free","BN_free","BN_free","BN_free","EC_GROUP_free","EVP_PKEY_free","OSSL_PROVIDER_unload","OSSL_LIB_CTX_free"],"critical_calls":["EVP_CIPHER_CTX_new","BN_CTX_new","OSSL_LIB_CTX_new","EVP_CIPHER_CTX_set_app_data","EVP_aes_256_cbc","EVP_CIPHER_get_mode","OSSL_PROVIDER_load","RAND_set1_random_provider","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_bin2bn","EC_GROUP_new_curve_GFp","EC_KEY_new","EC_KEY_set_group","EVP_PKEY_new","EVP_PKEY_assign","i2d_KeyParams","CRYPTO_free","OSSL_CRMF_CERTID_new","OSSL_CRMF_CERTID_dup","OSSL_CRMF_CERTID_free","OSSL_CRMF_CERTID_free","OSSL_ATTRIBUTES_SYNTAX_it","X509_NAME_new","ASN1_STRING_new","ASN1_STRING_set","X509_NAME_add_entry_by_txt","ASN1_STRING_length","ASN1_STRING_get0_data","ASN1_STRING_free","OSSL_STORE_SEARCH_by_name","OSSL_STORE_SEARCH_get0_name","OSSL_STORE_SEARCH_free","X509_NAME_free","CRYPTO_free","CRYPTO_free","ossl_check_IPAddressFamily_freefunc_type","EVP_CIPHER_CTX_free","BN_CTX_free","BN_free","BN_free","BN_free","EC_GROUP_free","EVP_PKEY_free","OSSL_PROVIDER_unload","OSSL_LIB_CTX_free"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for operations

    // 1. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 3. Initialize context structures
    EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new();
    BN_CTX *bn_ctx = BN_CTX_new();
    OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
    OSSL_PROVIDER *provider = NULL;
    
    // 4. Use EVP_CIPHER_CTX_set_app_data
    uint8_t app_data[32];
    size_t read_size = fread(app_data, 1, sizeof(app_data), in_file);
    EVP_CIPHER_CTX_set_app_data(cipher_ctx, app_data);

    // 5. Use EVP_CIPHER_get_mode
    const EVP_CIPHER *cipher = EVP_aes_256_cbc();
    int cipher_mode = EVP_CIPHER_get_mode(cipher);

    // 6. Use RAND_set1_random_provider
    // Try to load default provider
    provider = OSSL_PROVIDER_load(libctx, "default");
    if (provider) {
        RAND_set1_random_provider(libctx, provider);
    }

    // 7. Use EC_GROUP_new_curve_GFp with data from input
    rewind(in_file);
    BIGNUM *p = BN_new();
    BIGNUM *a = BN_new();
    BIGNUM *b = BN_new();
    
    // Read BIGNUM data from input
    uint8_t bn_data[128];
    fread(bn_data, 1, sizeof(bn_data), in_file);
    
    BN_bin2bn(bn_data, 64, p);
    BN_bin2bn(bn_data + 64, 32, a);
    BN_bin2bn(bn_data + 96, 32, b);
    
    EC_GROUP *ec_group = EC_GROUP_new_curve_GFp(p, a, b, bn_ctx);

    // 8. Use i2d_KeyParams - create EC key and serialize parameters
    EVP_PKEY *pkey = NULL;
    if (ec_group) {
        EC_KEY *ec_key = EC_KEY_new();
        EC_KEY_set_group(ec_key, ec_group);
        pkey = EVP_PKEY_new();
        EVP_PKEY_assign_EC_KEY(pkey, ec_key);
        
        unsigned char *key_params = NULL;
        int key_params_len = i2d_KeyParams(pkey, &key_params);
        if (key_params_len > 0 && key_params) {
            fwrite(key_params, 1, key_params_len, out_file);
            OPENSSL_free(key_params);
        }
    }

    // 9. Use OSSL_CRMF_CERTID_dup - create and duplicate CERTID
    OSSL_CRMF_CERTID *certid = OSSL_CRMF_CERTID_new();
    if (certid) {
        // Set some data from input
        uint8_t cert_id_data[20];
        fread(cert_id_data, 1, sizeof(cert_id_data), in_file);
        
        OSSL_CRMF_CERTID *certid_dup = OSSL_CRMF_CERTID_dup(certid);
        OSSL_CRMF_CERTID_free(certid_dup);
        OSSL_CRMF_CERTID_free(certid);
    }

    // 10. Use OSSL_ATTRIBUTES_SYNTAX_it
    const ASN1_ITEM *attr_syntax_it = OSSL_ATTRIBUTES_SYNTAX_it();

    // 11. Use OSSL_STORE_SEARCH_get0_name - create name search
    X509_NAME *name = X509_NAME_new();
    if (name) {
        // Add some entries from input data
        uint8_t name_data[64];
        fread(name_data, 1, sizeof(name_data), in_file);
        
        ASN1_STRING *common_name = ASN1_STRING_new();
        ASN1_STRING_set(common_name, name_data, 32);
        X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, 
                                   ASN1_STRING_get0_data(common_name), 
                                   ASN1_STRING_length(common_name), -1, 0);
        ASN1_STRING_free(common_name);
        
        OSSL_STORE_SEARCH *search = OSSL_STORE_SEARCH_by_name(name);
        const X509_NAME *retrieved_name = OSSL_STORE_SEARCH_get0_name(search);
        OSSL_STORE_SEARCH_free(search);
        X509_NAME_free(name);
    }

    // 12. Use ossl_check_IPAddressFamily_freefunc_type
    // Create a dummy function pointer
    void (*freefunc)(void*) = [](void* ptr) { OPENSSL_free(ptr); };
    OPENSSL_sk_freefunc checked_freefunc = 
        ossl_check_IPAddressFamily_freefunc_type(
            reinterpret_cast<sk_IPAddressFamily_freefunc>(freefunc));

    // 13. Clean up
    EVP_CIPHER_CTX_free(cipher_ctx);
    BN_CTX_free(bn_ctx);
    BN_free(p);
    BN_free(a);
    BN_free(b);
    
    if (ec_group) EC_GROUP_free(ec_group);
    if (pkey) EVP_PKEY_free(pkey);
    
    if (provider) OSSL_PROVIDER_unload(provider);
    OSSL_LIB_CTX_free(libctx);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}