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
//<ID> 318
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 1
//<Quality> {"density":21,"unique_branches":{"ossl_provider_find":[[417,17,417,48,0,0,4,0]]},"library_calls":["EVP_CIPHER_CTX_new","BN_CTX_new","OSSL_LIB_CTX_new","EVP_CIPHER_CTX_set_app_data","d2i_NETSCAPE_CERT_SEQUENCE","PEM_write_NETSCAPE_CERT_SEQUENCE","NETSCAPE_CERT_SEQUENCE_free","OSSL_PROVIDER_load","RAND_set1_random_provider","OSSL_PROVIDER_unload","BN_new","BN_bin2bn","EC_GROUP_new_curve_GFp","EC_KEY_new","EC_KEY_set_group","EVP_PKEY_new","EVP_PKEY_assign","i2d_KeyParams","CRYPTO_free","EVP_PKEY_free","EC_KEY_free","EC_GROUP_free","BN_free","OSSL_CRMF_CERTID_new","OSSL_CRMF_CERTID_dup","OSSL_CRMF_CERTID_free","OSSL_CRMF_CERTID_free","OSSL_ATTRIBUTES_SYNTAX_it","X509_NAME_new","ASN1_STRING_new","ASN1_STRING_set","ASN1_STRING_get0_data","ASN1_STRING_length","X509_NAME_add_entry_by_txt","ASN1_STRING_free","OSSL_STORE_SEARCH_by_name","OSSL_STORE_SEARCH_get0_name","OSSL_STORE_SEARCH_free","X509_NAME_free","EVP_CIPHER_CTX_free","BN_CTX_free","OSSL_LIB_CTX_free"],"critical_calls":["EVP_CIPHER_CTX_new","BN_CTX_new","OSSL_LIB_CTX_new","EVP_CIPHER_CTX_set_app_data","d2i_NETSCAPE_CERT_SEQUENCE","PEM_write_NETSCAPE_CERT_SEQUENCE","NETSCAPE_CERT_SEQUENCE_free","OSSL_PROVIDER_load","RAND_set1_random_provider","OSSL_PROVIDER_unload","BN_new","BN_bin2bn","EC_GROUP_new_curve_GFp","EC_KEY_new","EC_KEY_set_group","EVP_PKEY_new","EVP_PKEY_assign","i2d_KeyParams","CRYPTO_free","EVP_PKEY_free","EC_GROUP_free","BN_free","OSSL_CRMF_CERTID_new","OSSL_CRMF_CERTID_dup","OSSL_CRMF_CERTID_free","OSSL_CRMF_CERTID_free","OSSL_ATTRIBUTES_SYNTAX_it","X509_NAME_new","ASN1_STRING_new","ASN1_STRING_set","ASN1_STRING_get0_data","ASN1_STRING_length","X509_NAME_add_entry_by_txt","ASN1_STRING_free","OSSL_STORE_SEARCH_by_name","OSSL_STORE_SEARCH_get0_name","OSSL_STORE_SEARCH_free","X509_NAME_free","EVP_CIPHER_CTX_free","BN_CTX_free","OSSL_LIB_CTX_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if no data
    if (size == 0) {
        return 0;
    }

    // 1. Create context objects
    EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new();
    BN_CTX *bn_ctx = BN_CTX_new();
    OSSL_LIB_CTX *lib_ctx = OSSL_LIB_CTX_new();
    
    // 2. Set application data for cipher context
    const char *app_data = "fuzz_app_data";
    EVP_CIPHER_CTX_set_app_data(cipher_ctx, (void*)app_data);

    // 3. Decode NETSCAPE_CERT_SEQUENCE from input data
    FILE *in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        NETSCAPE_CERT_SEQUENCE *seq = NULL;
        const unsigned char *p = data;
        long len = size;
        seq = d2i_NETSCAPE_CERT_SEQUENCE(NULL, &p, len);
        
        // Write to output file if successfully decoded
        if (seq) {
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                PEM_write_NETSCAPE_CERT_SEQUENCE(out_file, seq);
                assert_file_closed(&out_file);;
            }
            NETSCAPE_CERT_SEQUENCE_free(seq);
        }
        assert_file_closed(&in_file);;
    }

    // 4. Random provider setup
    if (lib_ctx) {
        // Get default provider for setting
        OSSL_PROVIDER *prov = OSSL_PROVIDER_load(NULL, "default");
        if (prov) {
            RAND_set1_random_provider(lib_ctx, prov);
            OSSL_PROVIDER_unload(prov);
        }
    }

    // 5. Create EC key and EVP_PKEY
    if (size >= 32) {  // Need at least 32 bytes for BIGNUM
        BIGNUM *bn = BN_new();
        if (bn) {
            // Use first 32 bytes for BIGNUM
            int bn_len = size > 32 ? 32 : size;
            BN_bin2bn(data, bn_len, bn);
            
            // Create EC group using the BIGNUM for parameters
            EC_GROUP *group = EC_GROUP_new_curve_GFp(bn, bn, bn, bn_ctx);
            
            if (group) {
                EC_KEY *ec_key = EC_KEY_new();
                if (ec_key && EC_KEY_set_group(ec_key, group)) {
                    EVP_PKEY *pkey = EVP_PKEY_new();
                    if (pkey) {
                        // Assign EC key to EVP_PKEY
                        EVP_PKEY_assign(pkey, EVP_PKEY_EC, ec_key);
                        
                        // Serialize key parameters
                        unsigned char *out = NULL;
                        int out_len = i2d_KeyParams(pkey, &out);
                        if (out) {
                            // Free the serialized data using CRYPTO_free
                            CRYPTO_free(out, __FILE__, __LINE__);
                        }
                        // Note: EVP_PKEY_free will also free the EC_KEY
                        EVP_PKEY_free(pkey);
                    } else {
                        EC_KEY_free(ec_key);
                    }
                }
                EC_GROUP_free(group);
            }
            BN_free(bn);
        }
    }

    // 6. Create and duplicate OSSL_CRMF_CERTID
    OSSL_CRMF_CERTID *certid = OSSL_CRMF_CERTID_new();
    if (certid) {
        OSSL_CRMF_CERTID *certid_dup = OSSL_CRMF_CERTID_dup(certid);
        OSSL_CRMF_CERTID_free(certid_dup);
        OSSL_CRMF_CERTID_free(certid);
    }

    // 7. Get ASN1_ITEM for OSSL_ATTRIBUTES_SYNTAX
    const ASN1_ITEM *attr_item = OSSL_ATTRIBUTES_SYNTAX_it();

    // 8. Create X509_NAME and add entry
    X509_NAME *name = X509_NAME_new();
    if (name && size > 0) {
        ASN1_STRING *asn1_str = ASN1_STRING_new();
        if (asn1_str) {
            // Use up to 64 bytes for ASN1_STRING data
            int str_len = size > 64 ? 64 : size;
            ASN1_STRING_set(asn1_str, data, str_len);
            
            // Add entry to X509_NAME
            const unsigned char *str_data = ASN1_STRING_get0_data(asn1_str);
            int actual_len = ASN1_STRING_length(asn1_str);
            X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_UTF8, 
                                       str_data, actual_len, -1, 0);
            
            ASN1_STRING_free(asn1_str);
        }
    }

    // 9. Create OSSL_STORE_SEARCH by name
    if (name) {
        OSSL_STORE_SEARCH *search = OSSL_STORE_SEARCH_by_name(name);
        if (search) {
            const X509_NAME *search_name = OSSL_STORE_SEARCH_get0_name(search);
            OSSL_STORE_SEARCH_free(search);
        }
        X509_NAME_free(name);
    }

    // 10. Clean up using the correct free function types
    if (cipher_ctx) EVP_CIPHER_CTX_free(cipher_ctx);
    if (bn_ctx) BN_CTX_free(bn_ctx);
    if (lib_ctx) OSSL_LIB_CTX_free(lib_ctx);

    assert_file_closed(&in_file);
	return 0;
}