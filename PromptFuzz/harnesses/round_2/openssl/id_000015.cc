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
//<ID> 327
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 37
//<Quality> {"density":11,"unique_branches":{"BIO_new_fd":[[84,9,84,20,0,0,4,1]],"fd_read":[[117,9,117,20,0,0,4,0],[117,24,117,32,0,0,4,0],[122,13,122,21,0,0,4,0],[123,17,123,41,0,0,4,1],[125,22,125,30,0,0,4,0]],"fd_ctrl":[[151,5,151,24,0,0,4,0],[151,5,151,24,0,0,4,1],[154,5,154,25,0,0,4,1],[157,5,157,25,0,0,4,1],[158,5,158,23,0,0,4,1],[161,5,161,22,0,0,4,0],[161,5,161,22,0,0,4,1],[167,5,167,22,0,0,4,1],[176,5,176,28,0,0,4,1],[179,5,179,28,0,0,4,1],[182,5,182,26,0,0,4,1],[183,5,183,27,0,0,4,1],[186,5,186,22,0,0,4,1],[187,5,187,24,0,0,4,1],[190,5,190,22,0,0,4,1],[193,5,193,12,0,0,4,1]],"fd_free":[[101,9,101,18,0,0,4,1],[103,9,103,20,0,0,4,0],[103,9,103,20,0,0,4,1],[104,13,104,20,0,0,4,1]],"BIO_fd_should_retry":[[233,9,233,17,0,0,4,0]],"fd_gets":[[217,12,217,21,0,0,4,0],[217,25,217,48,0,0,4,1],[224,9,224,23,0,0,4,1]],"BIO_fd_non_fatal_error":[[251,5,251,21,0,0,4,1],[256,5,256,18,0,0,4,1],[260,5,260,15,0,0,4,1],[270,5,270,16,0,0,4,1],[274,5,274,21,0,0,4,1],[278,5,278,18,0,0,4,1],[281,5,281,12,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","BIO_new_fd","BIO_new_fd","PEM_read_bio_EC_PUBKEY","BIO_ctrl","UI_null","OCSP_SINGLERESP_new","OCSP_SINGLERESP_get0_id","X509_CRL_new","X509_CRL_get0_extensions","ossl_check_X509_LOOKUP_compfunc_type","PEM_write_bio_EC_PUBKEY","EC_KEY_get0_group","EC_GROUP_get_curve_name","OBJ_obj2txt","OBJ_nid2obj","BIO_printf","EC_KEY_free","OCSP_SINGLERESP_free","X509_CRL_free","BIO_free","BIO_free"],"critical_calls":["OPENSSL_init_ssl","BIO_new_fd","BIO_new_fd","PEM_read_bio_EC_PUBKEY","BIO_ctrl","UI_null","OCSP_SINGLERESP_new","OCSP_SINGLERESP_get0_id","X509_CRL_new","X509_CRL_get0_extensions","ossl_check_X509_LOOKUP_compfunc_type","PEM_write_bio_EC_PUBKEY","EC_KEY_get0_group","EC_GROUP_get_curve_name","OBJ_obj2txt","OBJ_nid2obj","BIO_printf","EC_KEY_free","OCSP_SINGLERESP_free","X509_CRL_free","BIO_free","BIO_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize OpenSSL if needed
    OPENSSL_init_ssl(0, NULL);

    // Create temporary files for reading/writing
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Create BIO objects for I/O operations
    BIO *in_bio = BIO_new_fd(in_fd, BIO_NOCLOSE);
    BIO *out_bio = BIO_new_fd(out_fd, BIO_NOCLOSE);
    
    // Event: Read and verify an EC public key from PEM data,
    // check CRL extensions, and process OCSP response
    
    // 1. Read EC public key from PEM data
    EC_KEY *ec_key = NULL;
    pem_password_cb *cb = NULL; // No password callback
    void *u = NULL;
    
    // Read EC public key from the input data
    ec_key = PEM_read_bio_EC_PUBKEY(in_bio, &ec_key, cb, u);
    
    // Reset BIO position for additional processing
    BIO_reset(in_bio);
    
    // 2. Get a null UI method (for password prompts, etc.)
    const UI_METHOD *ui_null_method = UI_null();
    (void)ui_null_method; // Use to avoid unused variable warning
    
    // 3. Create a dummy OCSP_SINGLERESP structure to demonstrate API usage
    // In real usage, we'd parse this from input data
    OCSP_SINGLERESP *ocsp_resp = OCSP_SINGLERESP_new();
    if (ocsp_resp) {
        // Get the certificate ID from OCSP response
        const OCSP_CERTID *cert_id = OCSP_SINGLERESP_get0_id(ocsp_resp);
        (void)cert_id; // Use to avoid unused variable warning
    }
    
    // 4. Create a dummy X509_CRL to demonstrate API usage
    // In real usage, we'd parse this from input data
    X509_CRL *crl = X509_CRL_new();
    if (crl) {
        // Get extensions from CRL
        const struct stack_st_X509_EXTENSION *extensions = X509_CRL_get0_extensions(crl);
        (void)extensions; // Use to avoid unused variable warning
    }
    
    // 5. Demonstrate the X509_LOOKUP comparison function type conversion
    // Create a dummy comparison function
    auto dummy_cmp = [](const X509_LOOKUP *const *a, const X509_LOOKUP *const *b) -> int {
        return 0;
    };
    
    // Convert the comparison function type
    OPENSSL_sk_compfunc converted_cmp = ossl_check_X509_LOOKUP_compfunc_type(
        (sk_X509_LOOKUP_compfunc)dummy_cmp
    );
    (void)converted_cmp; // Use to avoid unused variable warning
    
    // Write some output data if we successfully read the EC key
    if (ec_key) {
        // Write the EC key back out in PEM format
        PEM_write_bio_EC_PUBKEY(out_bio, ec_key);
        
        // Also write some information about the key
        const EC_GROUP *group = EC_KEY_get0_group(ec_key);
        if (group) {
            int curve_nid = EC_GROUP_get_curve_name(group);
            char curve_name[256];
            if (curve_nid != NID_undef) {
                OBJ_obj2txt(curve_name, sizeof(curve_name), 
                           OBJ_nid2obj(curve_nid), 0);
                BIO_printf(out_bio, "\nCurve: %s\n", curve_name);
            }
        }
    }
    
    // Clean up all allocated resources
    if (ec_key) {
        EC_KEY_free(ec_key);
    }
    
    if (ocsp_resp) {
        OCSP_SINGLERESP_free(ocsp_resp);
    }
    
    if (crl) {
        X509_CRL_free(crl);
    }
    
    if (in_bio) {
        BIO_free(in_bio);
    }
    
    if (out_bio) {
        BIO_free(out_bio);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove the temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}