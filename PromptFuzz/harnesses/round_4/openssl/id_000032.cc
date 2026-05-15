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
//<ID> 435
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 26
//<Quality> {"density":10,"unique_branches":{"ossl_ec_curve_nid2nist_int":[[173,17,173,31,0,0,4,0],[174,13,174,38,0,0,4,0],[174,13,174,38,0,0,4,1]],"BN_set_word":[[421,15,421,16,0,0,4,1]],"BN_mod_lshift1":[[245,9,245,26,0,0,4,1]],"BN_bn2dec":[[68,9,68,20,0,0,4,1],[68,24,68,39,0,0,4,1],[70,9,70,32,0,0,4,1],[75,9,75,22,0,0,4,0],[75,9,75,22,0,0,4,1],[79,13,79,30,0,0,4,1],[82,16,82,30,0,0,4,0],[82,16,82,30,0,0,4,1],[83,17,83,44,0,0,4,1],[86,17,86,36,0,0,4,1],[97,13,97,18,0,0,4,1],[100,16,100,29,0,0,4,1],[112,9,112,11,0,0,4,0]],"BN_div_word":[[68,9,68,11,0,0,4,1],[71,9,71,20,0,0,4,1],[77,9,77,28,0,0,4,1],[80,26,80,32,0,0,4,0],[80,26,80,32,0,0,4,1],[88,9,88,21,0,0,4,0],[88,25,88,48,0,0,4,0],[91,9,91,16,0,0,4,0]]},"library_calls":["EC_curve_nid2nist","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_const_UI_STRING_sk_type","sk_OSSL_CMP_ITAV_cmpfunc_thunk","BN_GENCB_new","BN_GENCB_set","OSSL_NAMED_DAY_it","BN_CTX_new","BN_new","BN_new","BN_new","BN_set_word","BN_set_word","BN_set_word","BN_mod_lshift1","BN_bn2dec","CRYPTO_free","BN_free","BN_free","BN_free","BN_CTX_free","BN_GENCB_free","OPENSSL_sk_free","ossl_check_UI_STRING_sk_type"],"critical_calls":["EC_curve_nid2nist","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_const_UI_STRING_sk_type","sk_OSSL_CMP_ITAV_cmpfunc_thunk","BN_GENCB_new","BN_GENCB_set","OSSL_NAMED_DAY_it","BN_CTX_new","BN_new","BN_new","BN_new","BN_set_word","BN_set_word","BN_mod_lshift1","BN_bn2dec","CRYPTO_free","BN_free","BN_free","BN_free","BN_CTX_free","BN_GENCB_free","OPENSSL_sk_free","ossl_check_UI_STRING_sk_type"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create FILE* for output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Variables for API usage
    BN_CTX *bn_ctx = NULL;
    BIGNUM *bn_a = NULL;
    BIGNUM *bn_r = NULL;
    BIGNUM *bn_m = NULL;
    BN_GENCB *gencb = NULL;
    struct stack_st_UI_STRING *ui_stack = NULL;
    
    // Step 1: Use EC_curve_nid2nist to convert NID to curve name
    const char *curve_name = EC_curve_nid2nist(NID_X9_62_prime256v1);
    
    // Step 2: Create UI_STRING stack and use ossl_check_const_UI_STRING_sk_type
    // Create a simple UI_STRING stack for demonstration
    ui_stack = sk_UI_STRING_new_null();
    if (ui_stack) {
        const OPENSSL_STACK *checked_stack = ossl_check_const_UI_STRING_sk_type(ui_stack);
        // Use the checked stack (demonstration)
        (void)checked_stack;
    }
    
    // Step 3: Prepare for sk_OSSL_CMP_ITAV_cmpfunc_thunk
    // Create comparison function wrapper
    auto cmp_func = [](const void *a, const void *b) -> int {
        return 0; // Simple comparison for demonstration
    };
    
    // Create dummy items for comparison
    void *dummy_a = NULL;
    void *dummy_b = NULL;
    int cmp_result = sk_OSSL_CMP_ITAV_cmpfunc_thunk(cmp_func, dummy_a, dummy_b);
    (void)cmp_result; // Use result to avoid unused variable warning
    
    // Step 4: Set up BN_GENCB callback
    gencb = BN_GENCB_new();
    if (gencb) {
        auto callback = [](int p, int n, BN_GENCB *cb) -> int {
            return 1; // Simple callback
        };
        BN_GENCB_set(gencb, callback, NULL);
    }
    
    // Step 5: Get ASN1_ITEM for OSSL_NAMED_DAY
    const ASN1_ITEM *named_day_item = OSSL_NAMED_DAY_it();
    (void)named_day_item; // Use item to avoid unused variable warning
    
    // Step 6: Use BN_mod_lshift1 with data from input
    bn_ctx = BN_CTX_new();
    bn_a = BN_new();
    bn_r = BN_new();
    bn_m = BN_new();
    
    if (bn_ctx && bn_a && bn_r && bn_m && size > 0) {
        // Read data from input file to create BIGNUMs
        fseek(in_file, 0, SEEK_SET);
        
        // Create a from input data
        if (size >= 4) {
            uint32_t val;
            fread(&val, sizeof(uint32_t), 1, in_file);
            BN_set_word(bn_a, val);
        }
        
        // Create m from input data (use different part if available)
        if (size >= 8) {
            fseek(in_file, 4, SEEK_SET);
            uint32_t val;
            fread(&val, sizeof(uint32_t), 1, in_file);
            BN_set_word(bn_m, val ? val : 1); // Avoid division by zero
        } else {
            BN_set_word(bn_m, 1); // Safe default
        }
        
        // Perform modular left shift by 1
        BN_mod_lshift1(bn_r, bn_a, bn_m, bn_ctx);
        
        // Write result to output file
        char *bn_str = BN_bn2dec(bn_r);
        if (bn_str) {
            fwrite(bn_str, strlen(bn_str), 1, out_file);
            OPENSSL_free(bn_str);
        }
    }
    
    // Clean up all resources
    if (bn_a) BN_free(bn_a);
    if (bn_r) BN_free(bn_r);
    if (bn_m) BN_free(bn_m);
    if (bn_ctx) BN_CTX_free(bn_ctx);
    if (gencb) BN_GENCB_free(gencb);
    if (ui_stack) sk_UI_STRING_free(ui_stack);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}