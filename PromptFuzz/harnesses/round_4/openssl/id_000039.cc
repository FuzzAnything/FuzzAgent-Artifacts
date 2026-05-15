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
//<ID> 475
//<Prompt> []
/*<Combination>: [
*/
//<score> 132, nr_unique_branch: 54
//<Quality> {"density":11,"unique_branches":{"bn_mod_exp_mont_fixed_top":[[709,13,709,24,0,0,4,0],[709,28,709,48,0,0,4,0],[908,13,908,24,0,0,4,0],[908,28,908,35,0,0,4,0],[908,28,908,35,0,0,4,1],[943,26,943,33,0,0,4,1],[945,27,945,34,0,0,4,1],[951,38,951,45,0,0,4,0],[951,38,951,45,0,0,4,1],[967,21,967,27,0,0,4,0],[967,21,967,27,0,0,4,1],[971,21,971,26,0,0,4,0],[971,21,971,26,0,0,4,1],[975,29,975,35,0,0,4,0],[975,29,975,35,0,0,4,1],[980,16,980,22,0,0,4,0],[980,16,980,22,0,0,4,1],[986,16,986,22,0,0,4,0],[986,16,986,22,0,0,4,1],[1007,13,1007,20,0,0,4,0],[1008,20,1008,28,0,0,4,0],[1008,20,1008,28,0,0,4,1],[333,22,333,31,6,0,4,0]],"ossl_list_lru_entry_remove":[[129,13,129,32,0,0,4,1],[133,13,133,48,0,0,4,0]],"ossl_property_match_count":[[489,17,489,47,0,0,4,0],[490,32,490,54,0,0,4,0],[491,24,491,69,0,0,4,0],[493,22,493,24,0,0,4,0],[493,28,493,57,0,0,4,0]],"X509V3_EXT_print":[[92,9,92,17,0,0,4,0]],"SRP_Calc_client_key_ex":[[229,9,229,54,0,0,4,1],[231,9,231,45,0,0,4,1],[233,9,233,45,0,0,4,1],[235,9,235,39,0,0,4,1],[237,9,237,31,0,0,4,1],[240,9,240,18,0,0,4,0],[240,22,240,58,0,0,4,1]],"BN_mod_exp_mont":[[333,16,333,54,0,0,4,0]],"srp_Calc_xy":[[35,9,35,21,0,0,4,1],[38,9,38,15,0,0,4,1],[40,9,40,15,0,0,4,0],[40,19,40,37,0,0,4,0],[40,19,40,37,0,0,4,1],[42,9,42,54,0,0,4,1],[44,9,44,39,0,0,4,1],[45,12,45,49,0,0,4,1],[46,12,46,64,0,0,4,1]],"bn_mul_fixed_top":[[515,22,515,31,0,0,4,0]],"BN_mod_exp":[[145,16,145,56,0,0,4,0],[146,16,146,56,0,0,4,1]],"BN_mod_mul":[[219,9,219,15,0,0,4,1],[223,13,223,34,0,0,4,1]],"BN_mod_sub":[[117,9,117,25,0,0,4,1]]},"library_calls":["OSSL_LIB_CTX_new","EVP_set_default_properties","d2i_ASN1_BMPSTRING","EC_KEY_new_by_curve_name","EC_KEY_generate_key","i2d_EC_PUBKEY","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","X509_EXTENSION_create_by_NID","ASN1_OCTET_STRING_free","X509V3_EXT_print_fp","X509_EXTENSION_free","BN_bin2bn","BN_bin2bn","BN_new","BN_new","BN_new","BN_new","BN_set_word","BN_bin2bn","BN_bin2bn","BN_bin2bn","SRP_Calc_client_key_ex","BN_free","BN_free","BN_free","BN_free","BN_free","BN_free","BN_free","OPENSSL_sk_new_null","OPENSSL_sk_push","OPENSSL_sk_push","OPENSSL_sk_free","sk_OPENSSL_PSTRING_new","sk_OPENSSL_PSTRING_free","lh_CONF_VALUE_hash_thunk","ossl_check_CMS_RecipientEncryptedKey_freefunc_type","CRYPTO_free","EC_KEY_free","ASN1_BMPSTRING_free","OSSL_LIB_CTX_free"],"critical_calls":["OSSL_LIB_CTX_new","EVP_set_default_properties","d2i_ASN1_BMPSTRING","EC_KEY_new_by_curve_name","EC_KEY_generate_key","i2d_EC_PUBKEY","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","X509_EXTENSION_create_by_NID","ASN1_OCTET_STRING_free","X509V3_EXT_print_fp","X509_EXTENSION_free","BN_bin2bn","BN_bin2bn","BN_new","BN_new","BN_new","BN_new","BN_set_word","BN_bin2bn","BN_bin2bn","BN_bin2bn","SRP_Calc_client_key_ex","BN_free","BN_free","BN_free","BN_free","BN_free","BN_free","BN_free","OPENSSL_sk_new_null","OPENSSL_sk_push","OPENSSL_sk_push","OPENSSL_sk_free","sk_OPENSSL_PSTRING_new","sk_OPENSSL_PSTRING_free","lh_CONF_VALUE_hash_thunk","ossl_check_CMS_RecipientEncryptedKey_freefunc_type","CRYPTO_free","EC_KEY_free","ASN1_BMPSTRING_free","OSSL_LIB_CTX_free"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified APIs to achieve an event involving parsing ASN.1 BMP string, generating an EC key pair, computing SRP client key, and printing X.509 extension data:

*/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for operations

    // Initialize libctx and properties
    OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
    if (!libctx) {
	return 0;
	}
	
    
    // Set default properties
    EVP_set_default_properties(libctx, "provider=default");
    
    // Parse ASN.1 BMP string from input
    const unsigned char *p = data;
    ASN1_BMPSTRING *bmp_str = d2i_ASN1_BMPSTRING(NULL, &p, (long)(size / 2));
    
    // Create and generate EC key
    EC_KEY *ec_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if (ec_key) {
        EC_KEY_generate_key(ec_key);
        
        // Convert EC public key to DER format
        unsigned char *der_out = NULL;
        int der_len = i2d_EC_PUBKEY(ec_key, &der_out);
        
        if (der_len > 0 && der_out) {
            // Create FILE* for reading input data
            FILE *in_file = fmemopen((void *)data, size, "rb");
            FILE *out_file = fopen("output_file", "wb");
            
            if (in_file && out_file) {
                // Create X509_EXTENSION from BMP string
                X509_EXTENSION *ext = NULL;
                if (bmp_str) {
                    // Create extension using BMP string as value
                    ASN1_OCTET_STRING *octet_str = ASN1_OCTET_STRING_new();
                    if (octet_str) {
                        ASN1_OCTET_STRING_set(octet_str, 
                                             (unsigned char *)"test_value", 10);
                        ext = X509_EXTENSION_create_by_NID(NULL, NID_basic_constraints, 
                                                          0, octet_str);
                        ASN1_OCTET_STRING_free(octet_str);
                    }
                }
                
                // Print extension to file
                if (ext) {
                    X509V3_EXT_print_fp(out_file, ext, 0, 4);
                    X509_EXTENSION_free(ext);
                }
                
                // Create BIGNUMs for SRP calculation using input data
                BIGNUM *N = BN_bin2bn(data, (int)(size > 32 ? 32 : size), NULL);
                BIGNUM *B = BN_bin2bn(data + 32, (int)(size > 64 ? 32 : size - 32), NULL);
                BIGNUM *g = BN_new();
                BIGNUM *x = BN_new();
                BIGNUM *a = BN_new();
                BIGNUM *u = BN_new();
                
                if (g) BN_set_word(g, 2);
                if (x) BN_bin2bn(data + 64, (int)(size > 96 ? 32 : size - 64), x);
                if (a) BN_bin2bn(data, (int)(size > 32 ? 32 : size), a);
                if (u) BN_bin2bn(data + 32, (int)(size > 64 ? 32 : size - 32), u);
                
                // Calculate SRP client key
                if (N && B && g && x && a && u) {
                    BIGNUM *srp_key = SRP_Calc_client_key_ex(N, B, g, x, a, u, 
                                                           libctx, "provider=default");
                    if (srp_key) {
                        BN_free(srp_key);
                    }
                }
                
                // Clean up BIGNUMs
                if (N) BN_free(N);
                if (B) BN_free(B);
                if (g) BN_free(g);
                if (x) BN_free(x);
                if (a) BN_free(a);
                if (u) BN_free(u);
                
                // Create and use OPENSSL stack
                OPENSSL_STACK *stack = OPENSSL_sk_new_null();
                if (stack) {
                    OPENSSL_sk_push(stack, (void *)"test_data");
                    OPENSSL_sk_push(stack, (void *)data);
                    OPENSSL_sk_free(stack);
                }
                
                // Create and free OPENSSL_PSTRING stack
                struct stack_st_OPENSSL_PSTRING *pstring_stack = 
                    sk_OPENSSL_PSTRING_new(NULL);
                if (pstring_stack) {
                    sk_OPENSSL_PSTRING_free(pstring_stack);
                }
                
                // Use hash function thunk
                CONF_VALUE conf_val;
                memset(&conf_val, 0, sizeof(conf_val));
                conf_val.name = (char *)"test_name";
                conf_val.value = (char *)"test_value";
                unsigned long hash = lh_CONF_VALUE_hash_thunk(&conf_val, 
                                                             [](const void *d) -> unsigned long { 
                                                                 return 12345; 
                                                             });
                
                // Use free function type cast
                ossl_check_CMS_RecipientEncryptedKey_freefunc_type(
                    [](CMS_RecipientEncryptedKey *rek) { 
                        // Dummy free function
                    });
                
                assert_file_closed(&in_file);;
                assert_file_closed(&out_file);;
            }
            
            // Free DER output using CRYPTO_free
            CRYPTO_free(der_out, __FILE__, __LINE__);
        }
        
        // Free EC key
        EC_KEY_free(ec_key);
    }
    
    // Free BMP string
    ASN1_BMPSTRING_free(bmp_str);
    
    // Clean up libctx
    OSSL_LIB_CTX_free(libctx);
    
    return 0;
}