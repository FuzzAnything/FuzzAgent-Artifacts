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
//<ID> 477
//<Prompt> []
/*<Combination>: [
*/
//<score> 253, nr_unique_branch: 31
//<Quality> {"density":23,"unique_branches":{"X509V3_EXT_print":[[92,9,92,17,0,0,4,1],[95,9,95,20,0,0,4,0],[96,13,96,59,0,0,4,1],[131,9,131,19,0,0,4,0]],"do_ext_nconf":[[87,9,87,29,0,0,4,1],[91,9,91,55,0,0,4,1],[96,9,96,20,0,0,4,1],[113,16,113,27,0,0,4,0],[114,13,114,66,0,0,4,1],[130,9,130,19,0,0,4,0]],"v3_check_generic":[[61,5,61,63,1,0,4,1],[61,5,61,63,2,0,4,1],[58,30,58,74,4,0,4,1],[58,30,58,74,5,0,4,1]],"v3_check_critical":[[207,9,207,31,0,0,4,0],[58,30,58,74,3,0,4,1]],"do_ext_i2d":[[146,9,146,19,0,0,4,0],[149,13,149,24,0,0,4,1],[166,9,166,52,0,0,4,1],[175,9,175,13,0,0,4,1]],"s2i_ASN1_IA5STRING":[[47,9,47,20,0,0,4,1],[51,9,51,45,0,0,4,1],[55,9,55,68,0,0,4,1]],"ossl_property_merge":[[554,50,554,71,0,0,4,0],[555,13,555,35,0,0,4,0],[560,17,560,49,0,0,4,1]],"X509V3_EXT_conf_nid":[[563,9,563,41,0,0,4,1]],"X509V3_EXT_nconf_nid":[[71,9,71,46,0,0,4,1]],"i2s_ASN1_IA5STRING":[[34,9,34,20,0,0,4,1],[34,24,34,40,0,0,4,1],[36,9,36,56,0,0,4,1]]},"library_calls":["OSSL_LIB_CTX_new","EVP_set_default_properties","d2i_ASN1_BMPSTRING","OPENSSL_sk_new_null","OPENSSL_sk_push","sk_OPENSSL_PSTRING_new","BN_new","BN_new","BN_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","SRP_Calc_client_key_ex","EC_KEY_new_by_curve_name","EC_KEY_generate_key","i2d_EC_PUBKEY","X509V3_EXT_conf_nid","X509V3_EXT_print_fp","X509_EXTENSION_free","CRYPTO_free","EC_KEY_free","lh_CONF_VALUE_hash_thunk","ossl_check_CMS_RecipientEncryptedKey_freefunc_type","BN_free","BN_free","BN_free","BN_free","BN_free","BN_free","BN_free","ASN1_BMPSTRING_free","OPENSSL_sk_free","sk_OPENSSL_PSTRING_free","OSSL_LIB_CTX_free"],"critical_calls":["OSSL_LIB_CTX_new","EVP_set_default_properties","d2i_ASN1_BMPSTRING","OPENSSL_sk_new_null","OPENSSL_sk_push","sk_OPENSSL_PSTRING_new","BN_new","BN_new","BN_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","SRP_Calc_client_key_ex","EC_KEY_new_by_curve_name","EC_KEY_generate_key","i2d_EC_PUBKEY","X509V3_EXT_conf_nid","X509V3_EXT_print_fp","X509_EXTENSION_free","CRYPTO_free","EC_KEY_free","lh_CONF_VALUE_hash_thunk","ossl_check_CMS_RecipientEncryptedKey_freefunc_type","BN_free","BN_free","BN_free","BN_free","BN_free","BN_free","BN_free","ASN1_BMPSTRING_free","OPENSSL_sk_free","sk_OPENSSL_PSTRING_free","OSSL_LIB_CTX_free"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified openssl APIs to achieve a scenario involving SRP computation, EC key operations, ASN.1 parsing, and file operations:

*/


extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Initialize OSSL_LIB_CTX and set default properties
    OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
    EVP_set_default_properties(libctx, "provider=default");
    
    // 2. Parse ASN1_BMPSTRING from input data
    const unsigned char *p = data;
    ASN1_BMPSTRING *bmp = d2i_ASN1_BMPSTRING(NULL, &p, size);
    
    // 3. Create and use OPENSSL_STACK
    OPENSSL_STACK *stack = OPENSSL_sk_new_null();
    if (bmp) {
        OPENSSL_sk_push(stack, bmp);
    }
    
    // 4. Create sk_OPENSSL_PSTRING stack
    struct stack_st_OPENSSL_PSTRING *pstring_stack = sk_OPENSSL_PSTRING_new(NULL);
    
    // 5. Create BIGNUMs for SRP calculation (using data bytes)
    BIGNUM *N = BN_new();
    BIGNUM *B = BN_new();
    BIGNUM *g = BN_new();
    BIGNUM *x = BN_new();
    BIGNUM *a = BN_new();
    BIGNUM *u = BN_new();
    
    // Use portions of input data for BIGNUM values
    size_t offset = 0;
    if (size > offset + 32) BN_bin2bn(data + offset, 32, N);
    offset += 32;
    if (size > offset + 32) BN_bin2bn(data + offset, 32, B);
    offset += 32;
    if (size > offset + 32) BN_bin2bn(data + offset, 32, g);
    offset += 32;
    if (size > offset + 32) BN_bin2bn(data + offset, 32, x);
    offset += 32;
    if (size > offset + 16) BN_bin2bn(data + offset, 16, a);
    offset += 16;
    if (size > offset + 16) BN_bin2bn(data + offset, 16, u);
    
    // 6. Calculate SRP client key
    BIGNUM *srp_key = SRP_Calc_client_key_ex(N, B, g, x, a, u, libctx, "default");
    
    // 7. Create EC key and generate key pair
    EC_KEY *ec_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if (ec_key) {
        EC_KEY_generate_key(ec_key);
        
        // 8. Encode EC public key to DER
        unsigned char *ec_der = NULL;
        int ec_der_len = i2d_EC_PUBKEY(ec_key, &ec_der);
        
        // 9. Create FILE* for reading input data
        FILE *in_file = fmemopen((void *)data, size, "rb");
        
        // 10. Create FILE* for writing output
        FILE *out_file = fopen("output_file", "wb");
        
        if (in_file && out_file) {
            // 11. Create a simple X509_EXTENSION for testing
            X509_EXTENSION *ext = X509V3_EXT_conf_nid(NULL, NULL, NID_netscape_comment, 
                                                     "Test extension");
            if (ext) {
                // 12. Print extension to file
                X509V3_EXT_print_fp(out_file, ext, 0, 4);
                X509_EXTENSION_free(ext);
            }
            
            // 13. Use file descriptor operations if needed
            int in_fd = fuzz_fileno(in_file);
            int out_fd = fuzz_fileno(out_file);
            
            // 14. Write EC DER to output file
            if (ec_der && ec_der_len > 0) {
                fwrite(ec_der, 1, ec_der_len, out_file);
            }
            
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
        }
        
        // 15. Free EC DER buffer using CRYPTO_free
        if (ec_der) {
            CRYPTO_free(ec_der, __FILE__, __LINE__);
        }
        
        // 16. Free EC key
        EC_KEY_free(ec_key);
    }
    
    // 17. Use lh_CONF_VALUE_hash_thunk (hash function wrapper)
    if (p) {
        unsigned long hash = lh_CONF_VALUE_hash_thunk(p, [](const void *data) -> unsigned long {
            return (unsigned long)data;
        });
        (void)hash; // Suppress unused warning
    }
    
    // 18. Use ossl_check_CMS_RecipientEncryptedKey_freefunc_type
    // Create a dummy function pointer and cast it
    sk_CMS_RecipientEncryptedKey_freefunc dummy_free = [](CMS_RecipientEncryptedKey *rek) {};
    OPENSSL_sk_freefunc checked_free = ossl_check_CMS_RecipientEncryptedKey_freefunc_type(dummy_free);
    (void)checked_free;
    
    // 19. Cleanup
    if (srp_key) BN_free(srp_key);
    if (N) BN_free(N);
    if (B) BN_free(B);
    if (g) BN_free(g);
    if (x) BN_free(x);
    if (a) BN_free(a);
    if (u) BN_free(u);
    
    if (bmp) ASN1_BMPSTRING_free(bmp);
    if (stack) OPENSSL_sk_free(stack);
    if (pstring_stack) sk_OPENSSL_PSTRING_free(pstring_stack);
    if (libctx) OSSL_LIB_CTX_free(libctx);
    
    return 0;
}