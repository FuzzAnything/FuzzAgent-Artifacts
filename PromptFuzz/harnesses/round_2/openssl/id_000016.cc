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
//<ID> 328
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 2
//<Quality> {"density":6,"unique_branches":{"get_header_and_data":[[864,52,864,81,0,0,4,1]],"OPENSSL_sk_set_cmp_func":[[43,9,43,22,0,0,4,1]]},"library_calls":["BIO_new_fp","PEM_read_bio_EC_PUBKEY","UI_null","X509_CRL_new","X509_CRL_get0_extensions","OCSP_SINGLERESP_new","OCSP_SINGLERESP_get0_id","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_X509_LOOKUP_compfunc_type","OPENSSL_sk_set_cmp_func","ossl_check_X509_LOOKUP_sk_type","ossl_check_X509_LOOKUP_compfunc_type","BIO_new_fp","PEM_write_bio_EC_PUBKEY","BIO_free","EC_KEY_free","X509_CRL_free","OCSP_SINGLERESP_free","OPENSSL_sk_free","ossl_check_X509_LOOKUP_sk_type","BIO_free"],"critical_calls":["BIO_new_fp","PEM_read_bio_EC_PUBKEY","UI_null","X509_CRL_new","X509_CRL_get0_extensions","OCSP_SINGLERESP_new","OCSP_SINGLERESP_get0_id","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_X509_LOOKUP_compfunc_type","OPENSSL_sk_set_cmp_func","ossl_check_X509_LOOKUP_compfunc_type","ossl_check_X509_LOOKUP_sk_type","BIO_new_fp","PEM_write_bio_EC_PUBKEY","BIO_free","EC_KEY_free","X509_CRL_free","OCSP_SINGLERESP_free","OPENSSL_sk_free","ossl_check_X509_LOOKUP_sk_type","BIO_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create BIO from input file for PEM reading
    BIO *bio_in = BIO_new_fp(in_file, BIO_NOCLOSE);
    if (!bio_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Variables to be used
    EC_KEY *ec_key = NULL;
    X509_CRL *crl = NULL;
    OCSP_SINGLERESP *single_resp = NULL;
    const OCSP_CERTID *cert_id = NULL;
    STACK_OF(X509_LOOKUP) *lookup_stack = NULL;
    const UI_METHOD *ui_method = NULL;
    
    // Event: Process a CRL and its extensions, while reading an EC public key from PEM data
    // and handling OCSP response lookup with UI interaction
    
    // API 1: Read EC public key from PEM data (using input data)
    ec_key = PEM_read_bio_EC_PUBKEY(bio_in, NULL, NULL, NULL);
    
    // API 2: Get null UI method for potential password callbacks
    ui_method = UI_null();
    
    // Create a dummy CRL for testing (since we need a CRL object)
    crl = X509_CRL_new();
    if (crl) {
        // API 3: Get extensions from CRL
        const struct stack_st_X509_EXTENSION *exts = X509_CRL_get0_extensions(crl);
        
        // Create a dummy OCSP single response for testing
        single_resp = OCSP_SINGLERESP_new();
        if (single_resp) {
            // API 4: Get certificate ID from OCSP single response
            cert_id = OCSP_SINGLERESP_get0_id(single_resp);
        }
    }
    
    // Create a stack of X509_LOOKUP for testing
    lookup_stack = sk_X509_LOOKUP_new_null();
    if (lookup_stack) {
        // API 5: Convert comparison function type for X509_LOOKUP stack
        OPENSSL_sk_compfunc comp_func = 
            ossl_check_X509_LOOKUP_compfunc_type(
                (sk_X509_LOOKUP_compfunc)NULL);
        
        // Set the comparison function (dummy operation)
        sk_X509_LOOKUP_set_cmp_func(lookup_stack, 
            (sk_X509_LOOKUP_compfunc)comp_func);
    }
    
    // Write EC key to output file if successfully read
    if (ec_key && out_file) {
        BIO *bio_out = BIO_new_fp(out_file, BIO_NOCLOSE);
        if (bio_out) {
            PEM_write_bio_EC_PUBKEY(bio_out, ec_key);
            BIO_free(bio_out);
        }
    }
    
    // Cleanup all allocated resources
    if (ec_key) EC_KEY_free(ec_key);
    if (crl) X509_CRL_free(crl);
    if (single_resp) OCSP_SINGLERESP_free(single_resp);
    if (lookup_stack) sk_X509_LOOKUP_free(lookup_stack);
    
    BIO_free(bio_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}