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
//<ID> 476
//<Prompt> []
/*<Combination>: [
*/
//<score> 76, nr_unique_branch: 3
//<Quality> {"density":19,"unique_branches":{"X509V3_EXT_print":[[85,9,85,47,0,0,4,0]],"X509V3_EXT_get":[[77,9,77,73,0,0,4,0]],"ossl_property_list_to_string":[[716,13,716,32,0,0,4,0]]},"library_calls":["OSSL_LIB_CTX_new","EVP_set_default_properties","d2i_ASN1_BMPSTRING","EC_KEY_new_by_curve_name","EC_KEY_generate_key","i2d_EC_PUBKEY","CRYPTO_free","EC_KEY_free","ASN1_BMPSTRING_free","OSSL_LIB_CTX_free","ASN1_BMPSTRING_free","OSSL_LIB_CTX_free","X509_EXTENSION_new","X509V3_EXT_print_fp","X509_EXTENSION_free","BN_new","BN_new","BN_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","SRP_Calc_client_key_ex","BN_free","OPENSSL_sk_new_null","OPENSSL_sk_push","ossl_check_CMS_RecipientEncryptedKey_freefunc_type","sk_OPENSSL_PSTRING_new","sk_OPENSSL_PSTRING_free","OPENSSL_sk_free","lh_CONF_VALUE_hash_thunk","BN_free","BN_free","BN_free","BN_free","BN_free","BN_free","ASN1_BMPSTRING_free","OSSL_LIB_CTX_free"],"critical_calls":["OSSL_LIB_CTX_new","EVP_set_default_properties","d2i_ASN1_BMPSTRING","EC_KEY_new_by_curve_name","EC_KEY_generate_key","i2d_EC_PUBKEY","CRYPTO_free","EC_KEY_free","X509_EXTENSION_new","X509V3_EXT_print_fp","X509_EXTENSION_free","BN_new","BN_new","BN_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","SRP_Calc_client_key_ex","BN_free","OPENSSL_sk_new_null","OPENSSL_sk_push","ossl_check_CMS_RecipientEncryptedKey_freefunc_type","sk_OPENSSL_PSTRING_new","sk_OPENSSL_PSTRING_free","OPENSSL_sk_free","lh_CONF_VALUE_hash_thunk","BN_free","BN_free","BN_free","BN_free","BN_free","BN_free","ASN1_BMPSTRING_free","OSSL_LIB_CTX_free"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified OpenSSL APIs to achieve an event involving SRP client key calculation, EC key generation, and data parsing:

*/


extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for operations
    
    // Initialize OpenSSL library context
    OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
    if (!libctx) {
	return 0;
	}
	
    
    // 1. Set default properties
    EVP_set_default_properties(libctx, "default");
    
    // 2. Parse ASN1 BMPSTRING from input data
    const unsigned char *p = data;
    ASN1_BMPSTRING *bmp = d2i_ASN1_BMPSTRING(nullptr, &p, size);
    
    // Update pointer position and remaining size
    size_t consumed = p - data;
    size_t remaining = size - consumed;
    
    // 3. Create EC key and generate key pair
    EC_KEY *ec_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if (ec_key) {
        EC_KEY_generate_key(ec_key);
        
        // 4. Convert EC public key to DER format
        unsigned char *der_out = nullptr;
        int der_len = i2d_EC_PUBKEY(ec_key, &der_out);
        if (der_out) {
            CRYPTO_free(der_out, __FILE__, __LINE__);
        }
        
        EC_KEY_free(ec_key);
    }
    
    // 5. Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        if (bmp) ASN1_BMPSTRING_free(bmp);
        OSSL_LIB_CTX_free(libctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Create FILE* for writing output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        if (bmp) ASN1_BMPSTRING_free(bmp);
        OSSL_LIB_CTX_free(libctx);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Create and print a dummy X509 extension
    X509_EXTENSION *ext = X509_EXTENSION_new();
    if (ext) {
        // Use the file descriptor for printing
        int indent = 4;
        int flags = 0;
        X509V3_EXT_print_fp(out_file, ext, flags, indent);
        X509_EXTENSION_free(ext);
    }
    
    // 8. Calculate SRP client key (using random BIGNUMs since we don't have real SRP data)
    BIGNUM *N = BN_new();
    BIGNUM *B = BN_new();
    BIGNUM *g = BN_new();
    BIGNUM *x = BN_new();
    BIGNUM *a = BN_new();
    BIGNUM *u = BN_new();
    
    // Initialize BIGNUMs with some data
    if (N && B && g && x && a && u && remaining >= 48) {
        // Use remaining data for BIGNUM initialization
        BN_bin2bn(data + consumed, 8, N);
        BN_bin2bn(data + consumed + 8, 8, B);
        BN_bin2bn(data + consumed + 16, 8, g);
        BN_bin2bn(data + consumed + 24, 8, x);
        BN_bin2bn(data + consumed + 32, 8, a);
        BN_bin2bn(data + consumed + 40, 8, u);
        
        // Calculate SRP client key
        BIGNUM *client_key = SRP_Calc_client_key_ex(N, B, g, x, a, u, libctx, "default");
        if (client_key) {
            BN_free(client_key);
        }
    }
    
    // 9. Create and use OPENSSL stacks
    OPENSSL_STACK *stack = OPENSSL_sk_new_null();
    if (stack) {
        // Push some dummy data
        int dummy = 42;
        OPENSSL_sk_push(stack, &dummy);
        
        // Use the type-checking function (even though we don't have actual CMS data)
        OPENSSL_sk_freefunc freefunc = ossl_check_CMS_RecipientEncryptedKey_freefunc_type(
            (sk_CMS_RecipientEncryptedKey_freefunc)OPENSSL_sk_free);
        
        // Create and use OPENSSL_PSTRING stack
        struct stack_st_OPENSSL_PSTRING *pstring_stack = sk_OPENSSL_PSTRING_new(nullptr);
        if (pstring_stack) {
            sk_OPENSSL_PSTRING_free(pstring_stack);
        }
        
        OPENSSL_sk_free(stack);
    }
    
    // 10. Use CONF_VALUE hash function
    CONF_VALUE cv;
    cv.name = (char*)"test";
    cv.value = (char*)"value";
    cv.section = (char*)"section";
    unsigned long hash = lh_CONF_VALUE_hash_thunk(&cv, (OPENSSL_LH_HASHFUNC)OPENSSL_LH_strhash);
    
    // 11. Cleanup BIGNUMs
    if (N) BN_free(N);
    if (B) BN_free(B);
    if (g) BN_free(g);
    if (x) BN_free(x);
    if (a) BN_free(a);
    if (u) BN_free(u);
    
    // 12. Cleanup BMPSTRING
    if (bmp) ASN1_BMPSTRING_free(bmp);
    
    // 13. Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 14. Cleanup library context
    OSSL_LIB_CTX_free(libctx);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}