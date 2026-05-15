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
//<ID> 204
//<Prompt> []
/*<Combination>: [
*/
//<score> 35, nr_unique_branch: 19
//<Quality> {"density":5,"unique_branches":{"ASN1_item_ex_i2d":[[90,48,90,61,0,0,4,0],[146,5,146,34,0,0,4,0],[148,13,148,36,0,0,4,1]],"ASN1_item_i2d_bio":[[91,9,91,14,0,0,4,1],[91,18,91,27,0,0,4,1],[98,13,98,19,0,0,4,0]],"OSSL_PARAM_set_int64":[[784,9,784,43,0,0,4,0],[786,13,786,28,0,0,4,1],[790,17,790,29,0,0,4,1],[791,9,791,29,0,0,4,1],[799,9,799,29,0,0,4,0]],"i2c_ibuf":[[107,13,107,17,0,0,4,1],[110,20,110,23,0,0,4,0],[112,17,112,24,0,0,4,1],[114,24,114,32,0,0,4,1]],"asn1_template_ex_i2d":[[344,9,344,33,0,0,4,0],[348,13,348,19,0,0,4,0],[349,17,349,54,0,0,4,1]],"ASN1_item_i2d_fp":[[74,9,74,44,0,0,4,1]]},"library_calls":["PEM_read_X509_ACERT","X509_ACERT_get0_issuerName","d2i_PKCS12_fp","ossl_check_OSSL_ROLE_SPEC_CERT_ID_copyfunc_type","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_PROFESSION_INFO_sk_type","OPENSSL_sk_free","ossl_check_PROFESSION_INFO_sk_type","OSSL_PARAM_construct_int64","OSSL_PARAM_construct_end","OSSL_PARAM_set_int64","PEM_write_X509_ACERT","i2d_PKCS12_fp","X509_ACERT_free","PKCS12_free"],"critical_calls":["PEM_read_X509_ACERT","X509_ACERT_get0_issuerName","d2i_PKCS12_fp","ossl_check_OSSL_ROLE_SPEC_CERT_ID_copyfunc_type","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_PROFESSION_INFO_sk_type","OPENSSL_sk_free","ossl_check_PROFESSION_INFO_sk_type","OSSL_PARAM_construct_int64","OSSL_PARAM_construct_end","OSSL_PARAM_set_int64","PEM_write_X509_ACERT","i2d_PKCS12_fp","X509_ACERT_free","PKCS12_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output FILE*
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Variables to be used
    X509_ACERT *acert = nullptr;
    PKCS12 *p12 = nullptr;
    OSSL_PARAM *param = nullptr;
    
    // 1. Try to read X509_ACERT from input
    // Note: We'll try to read it as PEM format (common for certificates)
    fseek(in_file, 0, SEEK_SET);
    acert = PEM_read_X509_ACERT(in_file, nullptr, nullptr, nullptr);
    
    // Use X509_ACERT_get0_issuerName
    if (acert) {
        const X509_NAME *issuer = X509_ACERT_get0_issuerName(acert);
        // We could print or process the issuer name here
        // For fuzzing, we just verify the function can be called
    }

    // 2. Try to read PKCS12 from input
    fseek(in_file, 0, SEEK_SET);
    p12 = d2i_PKCS12_fp(in_file, nullptr);
    
    // Use PKCS12_free (will be called later if p12 exists)

    // 3. Use ossl_check_OSSL_ROLE_SPEC_CERT_ID_copyfunc_type
    // Create a dummy copy function and convert its type
    OPENSSL_sk_copyfunc generic_copy = [](const void *a) -> void* { 
        return const_cast<void*>(a); 
    };
    
    OPENSSL_sk_copyfunc converted = 
        ossl_check_OSSL_ROLE_SPEC_CERT_ID_copyfunc_type(
            reinterpret_cast<sk_OSSL_ROLE_SPEC_CERT_ID_copyfunc>(generic_copy)
        );

    // 4. Use ossl_check_PROFESSION_INFO_sk_type
    // Create a dummy stack and convert its type
    struct stack_st_PROFESSION_INFO *prof_stack = sk_PROFESSION_INFO_new_null();
    if (prof_stack) {
        OPENSSL_STACK *converted_stack = ossl_check_PROFESSION_INFO_sk_type(prof_stack);
        // Could use converted_stack for stack operations
        
        // Clean up
        sk_PROFESSION_INFO_free(prof_stack);
    }

    // 5. Use OSSL_PARAM_set_int64
    // Create and populate an OSSL_PARAM structure
    int64_t int64_value = 12345;
    OSSL_PARAM params[2];
    
    params[0] = OSSL_PARAM_construct_int64("test_int64", &int64_value);
    params[1] = OSSL_PARAM_construct_end();
    
    // Set a new value
    OSSL_PARAM_set_int64(&params[0], 67890);

    // Try to write something to output file based on what we processed
    if (acert) {
        // Write ACERT info to output
        PEM_write_X509_ACERT(out_file, acert);
    }
    
    if (p12) {
        // Write PKCS12 info to output (DER format)
        i2d_PKCS12_fp(out_file, p12);
    }

    // Clean up resources
    if (acert) {
        X509_ACERT_free(acert);
    }
    
    if (p12) {
        PKCS12_free(p12);  // Required API call
    }
    
    // OSSL_PARAM doesn't need explicit freeing when constructed this way
    // The array params is stack-allocated
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}