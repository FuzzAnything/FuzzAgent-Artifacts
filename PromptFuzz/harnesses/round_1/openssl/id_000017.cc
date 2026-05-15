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
//<ID> 239
//<Prompt> []
/*<Combination>: [
*/
//<score> 80, nr_unique_branch: 9
//<Quality> {"density":16,"unique_branches":{"fd_read":[[117,9,117,20,0,0,4,0],[117,24,117,32,0,0,4,0],[122,13,122,21,0,0,4,0],[123,17,123,41,0,0,4,1],[125,22,125,30,0,0,4,0]],"BIO_fd_should_retry":[[233,9,233,17,0,0,4,0]],"fd_ctrl":[[161,5,161,22,0,0,4,1],[190,5,190,22,0,0,4,0]],"EVP_RAND_gettable_params":[[446,9,446,38,0,0,4,0]]},"library_calls":["OPENSSL_init_crypto","BIO_new_fd","BIO_get_init","d2i_X509_bio","ADMISSION_SYNTAX_new","ADMISSION_SYNTAX_get0_contentsOfAdmissions","ADMISSION_SYNTAX_free","X509_it","PKCS12_item_pack_safebag","BIO_new_fp","BIO_new_NDEF","PKCS12_SAFEBAG_it","BIO_get_init","BIO_write","BIO_ctrl","BIO_free","BIO_free","PKCS12_SAFEBAG_free","X509_free","EVP_RAND_fetch","EVP_RAND_gettable_params","EVP_RAND_free","BIO_free","ERR_clear_error"],"critical_calls":["OPENSSL_init_crypto","BIO_new_fd","BIO_get_init","d2i_X509_bio","ADMISSION_SYNTAX_new","ADMISSION_SYNTAX_get0_contentsOfAdmissions","ADMISSION_SYNTAX_free","X509_it","PKCS12_item_pack_safebag","BIO_new_fp","BIO_new_NDEF","PKCS12_SAFEBAG_it","BIO_get_init","BIO_write","BIO_ctrl","BIO_free","BIO_free","PKCS12_SAFEBAG_free","X509_free","EVP_RAND_fetch","EVP_RAND_gettable_params","EVP_RAND_free","BIO_free","ERR_clear_error"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations

    // Initialize OpenSSL if needed
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG, NULL);
    
    // 1. Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create output FILE*
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create BIO from input file descriptor
    int fd = fuzz_fileno(in_file);
    BIO *bio = BIO_new_fd(fd, BIO_NOCLOSE);
    if (!bio) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Check BIO initialization using BIO_get_init
    int bio_init = BIO_get_init(bio);
    // bio_init should be 1 if properly initialized
    
    // 4. Read an X509 certificate from the input BIO
    X509 *cert = d2i_X509_bio(bio, NULL);
    if (cert) {
        // 5. Create ADMISSION_SYNTAX structure (simulated for API usage)
        ADMISSION_SYNTAX *admission_syntax = ADMISSION_SYNTAX_new();
        if (admission_syntax) {
            // Use ADMISSION_SYNTAX_get0_contentsOfAdmissions
            const struct stack_st_ADMISSIONS *admissions = 
                ADMISSION_SYNTAX_get0_contentsOfAdmissions(admission_syntax);
            // API called, result may be NULL if no admissions
            
            ADMISSION_SYNTAX_free(admission_syntax);
        }
        
        // 6. Pack X509 certificate into PKCS12_SAFEBAG
        const ASN1_ITEM *it = X509_it();
        PKCS12_SAFEBAG *bag = PKCS12_item_pack_safebag(
            cert, 
            it, 
            NID_x509Certificate,  // bag type for certificate
            NID_pkcs7_data        // safe bag type
        );
        
        if (bag) {
            // 7. Create NDEF BIO for output
            BIO *out_bio = BIO_new_fp(out_file, BIO_NOCLOSE);
            if (out_bio) {
                // Convert bag to ASN1_VALUE for BIO_new_NDEF
                ASN1_VALUE *val = (ASN1_VALUE *)bag;
                BIO *ndef_bio = BIO_new_NDEF(out_bio, val, PKCS12_SAFEBAG_it());
                
                if (ndef_bio) {
                    // Check NDEF BIO initialization
                    int ndef_init = BIO_get_init(ndef_bio);
                    
                    // Write some data through the NDEF BIO
                    const char *test_data = "Test data for NDEF BIO";
                    BIO_write(ndef_bio, test_data, strlen(test_data));
                    
                    // Flush the BIO chain
                    BIO_flush(ndef_bio);
                    
                    BIO_free(ndef_bio);
                }
                BIO_free(out_bio);
            }
            
            PKCS12_SAFEBAG_free(bag);
        }
        
        X509_free(cert);
    }
    
    // 8. Get EVP_RAND parameters table
    EVP_RAND *rand = EVP_RAND_fetch(NULL, "CTR-DRBG", NULL);
    if (rand) {
        const OSSL_PARAM *gettable_params = EVP_RAND_gettable_params(rand);
        // API called, result may be NULL or parameter table
        
        EVP_RAND_free(rand);
    }
    
    // Cleanup
    BIO_free(bio);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up any remaining OpenSSL errors
    ERR_clear_error();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}