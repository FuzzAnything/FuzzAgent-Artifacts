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
//<ID> 406
//<Prompt> []
/*<Combination>: [
*/
//<score> 91, nr_unique_branch: 23
//<Quality> {"density":13,"unique_branches":{"evp_encodeblock_int":[[231,21,231,77,0,0,4,0],[232,24,232,72,0,0,4,0],[263,5,263,11,0,0,4,0],[272,13,272,24,0,0,4,0],[273,17,273,73,0,0,4,1]],"EVP_EncodeUpdate":[[385,9,385,47,0,0,4,1],[390,9,390,22,0,0,4,1],[416,13,416,52,0,0,4,0],[417,34,417,76,0,0,4,0],[436,9,436,55,0,0,4,0],[441,9,441,24,0,0,4,1],[446,9,446,17,0,0,4,0]],"encode_base64_avx2":[[487,18,487,31,0,0,4,1],[489,26,489,37,0,0,4,0],[490,12,490,63,0,0,4,1],[496,12,496,29,0,0,4,1],[640,9,640,20,0,0,4,1],[663,9,663,21,0,0,4,0],[663,25,663,43,0,0,4,1]],"PEM_write_bio":[[672,13,672,19,0,0,4,0],[672,23,672,65,0,0,4,1]],"ASN1_mbstring_ncopy":[[116,26,116,43,0,0,4,0]],"UTF8_getc":[[76,13,76,28,0,0,4,0]]},"library_calls":["EVP_CIPHER_CTX_new","BN_CTX_new","OSSL_LIB_CTX_new","EVP_CIPHER_CTX_set_app_data","d2i_NETSCAPE_CERT_SEQUENCE","PEM_write_NETSCAPE_CERT_SEQUENCE","OSSL_PROVIDER_load","RAND_set1_random_provider","OSSL_PROVIDER_unload","BN_new","BN_bin2bn","BN_new","BN_new","BN_new","BN_set_word","BN_set_word","BN_set_word","EC_GROUP_new_curve_GFp","EC_KEY_new","EC_KEY_set_group","BN_free","BN_free","BN_free","EVP_PKEY_new","EVP_PKEY_assign","i2d_KeyParams","CRYPTO_free","OSSL_CRMF_CERTID_new","OSSL_CRMF_CERTID_dup","X509_NAME_new","ASN1_STRING_new","ASN1_STRING_set","ASN1_STRING_get0_data","ASN1_STRING_length","X509_NAME_add_entry_by_txt","OSSL_STORE_SEARCH_by_name","OSSL_STORE_SEARCH_get0_name","OPENSSL_sk_new","sk_ESS_CERT_ID_cmpfunc_thunk","ossl_check_IPAddressOrRange_sk_type","OPENSSL_sk_free","OSSL_STORE_SEARCH_free","ASN1_STRING_free","X509_NAME_free","OSSL_CRMF_CERTID_free","OSSL_CRMF_CERTID_free","EVP_PKEY_free","EC_KEY_free","EC_GROUP_free","BN_free","NETSCAPE_CERT_SEQUENCE_free","BN_CTX_free","EVP_CIPHER_CTX_free","OSSL_LIB_CTX_free"],"critical_calls":["EVP_CIPHER_CTX_new","BN_CTX_new","OSSL_LIB_CTX_new","EVP_CIPHER_CTX_set_app_data","d2i_NETSCAPE_CERT_SEQUENCE","PEM_write_NETSCAPE_CERT_SEQUENCE","OSSL_PROVIDER_load","RAND_set1_random_provider","OSSL_PROVIDER_unload","BN_new","BN_bin2bn","BN_new","BN_new","BN_new","BN_set_word","BN_set_word","BN_set_word","EC_GROUP_new_curve_GFp","EC_KEY_new","EC_KEY_set_group","BN_free","BN_free","BN_free","EVP_PKEY_new","EVP_PKEY_assign","i2d_KeyParams","CRYPTO_free","OSSL_CRMF_CERTID_new","OSSL_CRMF_CERTID_dup","X509_NAME_new","ASN1_STRING_new","ASN1_STRING_set","ASN1_STRING_get0_data","ASN1_STRING_length","X509_NAME_add_entry_by_txt","OSSL_STORE_SEARCH_by_name","OSSL_STORE_SEARCH_get0_name","OPENSSL_sk_new","sk_ESS_CERT_ID_cmpfunc_thunk","ossl_check_IPAddressOrRange_sk_type","OPENSSL_sk_free","OSSL_STORE_SEARCH_free","ASN1_STRING_free","X509_NAME_free","OSSL_CRMF_CERTID_free","OSSL_CRMF_CERTID_free","EVP_PKEY_free","EC_GROUP_free","BN_free","NETSCAPE_CERT_SEQUENCE_free","BN_CTX_free","EVP_CIPHER_CTX_free","OSSL_LIB_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Create OpenSSL context objects
    EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new();
    BN_CTX *bn_ctx = BN_CTX_new();
    OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
    
    // 2. Set application data in cipher context
    static int app_data = 42;
    if (cipher_ctx) {
        EVP_CIPHER_CTX_set_app_data(cipher_ctx, &app_data);
    }

    // 3. Parse NETSCAPE_CERT_SEQUENCE from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    NETSCAPE_CERT_SEQUENCE *seq = NULL;
    const unsigned char *p = data;
    if (size > 0) {
        seq = d2i_NETSCAPE_CERT_SEQUENCE(NULL, &p, (long)size);
    }

    // 4. Write NETSCAPE_CERT_SEQUENCE to output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file && seq) {
        PEM_write_NETSCAPE_CERT_SEQUENCE(out_file, seq);
        assert_file_closed(&out_file);;
    }

    // 5. Load provider and set random provider
    OSSL_PROVIDER *provider = NULL;
    if (libctx) {
        provider = OSSL_PROVIDER_load(libctx, "default");
        if (provider) {
            RAND_set1_random_provider(libctx, provider);
            OSSL_PROVIDER_unload(provider);
        }
    }

    // 6. Create BIGNUM from input data
    BIGNUM *bn = BN_new();
    if (bn && size > 0) {
        // Use first 32 bytes or available data for BIGNUM
        size_t bn_len = size > 32 ? 32 : size;
        BN_bin2bn(data, (int)bn_len, bn);
    }

    // 7. Create EC group and key
    EC_GROUP *group = NULL;
    EC_KEY *ec_key = NULL;
    if (bn && bn_ctx) {
        // Create a simple prime for testing (using small prime 23)
        BIGNUM *prime = BN_new();
        BIGNUM *a = BN_new();
        BIGNUM *b = BN_new();
        
        if (prime && a && b) {
            BN_set_word(prime, 23);
            BN_set_word(a, 3);
            BN_set_word(b, 4);
            
            group = EC_GROUP_new_curve_GFp(prime, a, b, bn_ctx);
            
            if (group) {
                ec_key = EC_KEY_new();
                if (ec_key) {
                    EC_KEY_set_group(ec_key, group);
                }
            }
            
            BN_free(prime);
            BN_free(a);
            BN_free(b);
        }
    }

    // 8. Create EVP_PKEY and assign EC key
    EVP_PKEY *pkey = NULL;
    if (ec_key) {
        pkey = EVP_PKEY_new();
        if (pkey) {
            EVP_PKEY_assign(pkey, EVP_PKEY_EC, ec_key);
            
            // Serialize key parameters
            unsigned char *key_buf = NULL;
            int key_len = i2d_KeyParams(pkey, &key_buf);
            if (key_buf) {
                CRYPTO_free(key_buf, __FILE__, __LINE__);
            }
        }
    }

    // 9. Create and duplicate CRMF Certificate ID
    OSSL_CRMF_CERTID *certid = OSSL_CRMF_CERTID_new();
    OSSL_CRMF_CERTID *certid_dup = NULL;
    if (certid) {
        certid_dup = OSSL_CRMF_CERTID_dup(certid);
    }

    // 10. Create X509_NAME and ASN1_STRING
    X509_NAME *name = X509_NAME_new();
    ASN1_STRING *asn1_str = ASN1_STRING_new();
    
    if (asn1_str && size > 0) {
        // Use up to 100 bytes from input for ASN1_STRING
        size_t str_len = size > 100 ? 100 : size;
        ASN1_STRING_set(asn1_str, data, (int)str_len);
        
        // Add entry to X509_NAME using ASN1_STRING data
        if (name) {
            const unsigned char *str_data = ASN1_STRING_get0_data(asn1_str);
            int len = ASN1_STRING_length(asn1_str);
            if (str_data && len > 0) {
                X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_UTF8, 
                                          str_data, len, -1, 0);
            }
        }
    }

    // 11. Create OSSL_STORE_SEARCH by name
    OSSL_STORE_SEARCH *search = NULL;
    if (name) {
        search = OSSL_STORE_SEARCH_by_name(name);
        if (search) {
            const X509_NAME *retrieved_name = OSSL_STORE_SEARCH_get0_name(search);
            (void)retrieved_name; // Use retrieved_name to avoid unused variable warning
        }
    }

    // 12. Create OPENSSL_STACK and use comparison thunk
    OPENSSL_STACK *stack = OPENSSL_sk_new(NULL);
    if (stack) {
        // Create dummy comparison using thunk function
        int dummy_a = 1, dummy_b = 2;
        int cmp_result = sk_ESS_CERT_ID_cmpfunc_thunk(
            [](const void *a, const void *b) -> int {
                return *(const int*)a - *(const int*)b;
            },
            &dummy_a, &dummy_b
        );
        (void)cmp_result; // Use cmp_result to avoid unused variable warning
        
        // Use IPAddressOrRange stack type checking
        struct stack_st_IPAddressOrRange *ip_stack = 
            (struct stack_st_IPAddressOrRange*)stack;
        OPENSSL_STACK *checked_stack = 
            ossl_check_IPAddressOrRange_sk_type(ip_stack);
        (void)checked_stack; // Use checked_stack to avoid unused variable warning
    }

    // 13. Cleanup all resources
    if (stack) {
        OPENSSL_sk_free(stack);
    }
    
    if (search) {
        OSSL_STORE_SEARCH_free(search);
    }
    
    if (asn1_str) {
        ASN1_STRING_free(asn1_str);
    }
    
    if (name) {
        X509_NAME_free(name);
    }
    
    if (certid_dup) {
        OSSL_CRMF_CERTID_free(certid_dup);
    }
    
    if (certid) {
        OSSL_CRMF_CERTID_free(certid);
    }
    
    if (pkey) {
        EVP_PKEY_free(pkey);
    } else if (ec_key) {
        EC_KEY_free(ec_key);
    }
    
    if (group) {
        EC_GROUP_free(group);
    }
    
    if (bn) {
        BN_free(bn);
    }
    
    if (seq) {
        NETSCAPE_CERT_SEQUENCE_free(seq);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (bn_ctx) {
        BN_CTX_free(bn_ctx);
    }
    
    if (cipher_ctx) {
        EVP_CIPHER_CTX_free(cipher_ctx);
    }
    
    if (libctx) {
        OSSL_LIB_CTX_free(libctx);
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}