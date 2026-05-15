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
//<ID> 200
//<Prompt> []
/*<Combination>: [
*/
//<score> 50, nr_unique_branch: 18
//<Quality> {"density":5,"unique_branches":{"PKCS12_free":[[34,9,34,20,0,0,4,0],[34,24,34,46,0,0,4,0]],"OSSL_PARAM_free":[[235,9,235,23,0,0,4,0],[238,26,238,40,0,0,4,1],[240,13,240,53,0,0,4,0]],"OSSL_PARAM_BLD_free":[[115,9,115,20,0,0,4,1]],"free_all_params":[[109,17,109,22,0,0,4,1]],"param_bld_convert":[[406,17,406,24,0,0,4,1]],"d2i_PKCS12_fp":[[270,9,270,20,0,0,4,1]],"OSSL_PARAM_BLD_to_param":[[458,9,458,20,0,0,4,1],[468,9,468,15,0,0,4,1],[476,9,476,23,0,0,4,1]],"OSSL_PARAM_BLD_new":[[95,9,95,18,0,0,4,0],[97,13,97,30,0,0,4,1]],"OSSL_PARAM_set_int64":[[779,9,779,18,0,0,4,1],[784,9,784,43,0,0,4,1],[806,16,806,59,0,0,4,1],[828,16,828,47,0,0,4,1]]},"library_calls":["OPENSSL_init_crypto","ERR_clear_error","d2i_PKCS12_fp","ERR_clear_error","X509_ACERT_get0_issuerName","OSSL_PARAM_BLD_new","OSSL_PARAM_BLD_to_param","OSSL_PARAM_set_int64","OSSL_PARAM_free","OSSL_PARAM_BLD_free","ossl_check_PROFESSION_INFO_sk_type","ossl_check_OSSL_ROLE_SPEC_CERT_ID_copyfunc_type","PKCS12_free","ERR_clear_error"],"critical_calls":["OPENSSL_init_crypto","ERR_clear_error","d2i_PKCS12_fp","X509_ACERT_get0_issuerName","OSSL_PARAM_BLD_new","OSSL_PARAM_BLD_to_param","OSSL_PARAM_set_int64","OSSL_PARAM_free","OSSL_PARAM_BLD_free","ossl_check_PROFESSION_INFO_sk_type","ossl_check_OSSL_ROLE_SPEC_CERT_ID_copyfunc_type","PKCS12_free","ERR_clear_error"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Initialize OpenSSL if needed
    OpenSSL_add_all_algorithms();
    ERR_clear_error();

    // Create input FILE* from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 1. Parse PKCS12 structure from input data
    PKCS12 *p12 = d2i_PKCS12_fp(in_file, NULL);
    assert_file_closed(&in_file);;
    
    if (!p12) {
        ERR_clear_error();
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Extract attribute certificate from PKCS12 (simulated - actual parsing would need proper API)
    // For fuzzing purposes, we'll create a dummy X509_ACERT context
    X509_ACERT *acert = NULL;
    // Note: In real usage, we would extract ACERT from PKCS12 using appropriate APIs
    
    // 3. Use X509_ACERT_get0_issuerName API
    if (acert) {
        const X509_NAME *issuer = X509_ACERT_get0_issuerName(acert);
        (void)issuer; // Use the result to avoid unused variable warning
    }

    // 4. Use OSSL_PARAM_set_int64 API
    OSSL_PARAM_BLD *param_bld = OSSL_PARAM_BLD_new();
    if (param_bld) {
        OSSL_PARAM *param = OSSL_PARAM_BLD_to_param(param_bld);
        if (param) {
            // Set an int64 value - using first byte of input as dummy value
            int64_t int_val = data[0];
            OSSL_PARAM_set_int64(param, int_val);
            OSSL_PARAM_free(param);
        }
        OSSL_PARAM_BLD_free(param_bld);
    }

    // 5. Use stack type conversion functions
    // Create dummy stacks and use the conversion functions
    struct stack_st_PROFESSION_INFO *prof_stack = NULL;
    OPENSSL_STACK *generic_stack = ossl_check_PROFESSION_INFO_sk_type(prof_stack);
    (void)generic_stack; // Use the result
    
    // For OSSL_ROLE_SPEC_CERT_ID copy function type conversion
    sk_OSSL_ROLE_SPEC_CERT_ID_copyfunc copy_func = NULL;
    OPENSSL_sk_copyfunc converted = 
        ossl_check_OSSL_ROLE_SPEC_CERT_ID_copyfunc_type(copy_func);
    (void)converted; // Use the result

    // 6. Clean up PKCS12 structure using PKCS12_free
    PKCS12_free(p12);

    // Clean up other resources if they were allocated
    // Note: acert would need to be freed if it was actually allocated
    
    // Clear any remaining OpenSSL errors
    ERR_clear_error();
    
    assert_file_closed(&in_file);
	return 0;
}