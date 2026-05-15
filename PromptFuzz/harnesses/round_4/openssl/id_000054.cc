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
//<ID> 754
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"fd_ctrl":[[151,5,151,24,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","ERR_clear_error","BIO_new_fd","d2i_X509_bio","BIO_new_fp","BIO_free","X509_free","BIO_ctrl","CRL_DIST_POINTS_new","ASIdentifiers_new","CRYPTO_malloc","UI_create_method","UI_method_set_prompt_constructor","BIO_get_wpoll_descriptor","EVP_PKEY_new","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","EVP_PKEY_assign","X509_PUBKEY_set","EVP_PKEY_type_names_do_all","X509_PUBKEY_free","PEM_write_bio_X509","BN_free","RSA_free","EVP_PKEY_free","UI_destroy_method","CRYPTO_free","ASIdentifiers_free","CRL_DIST_POINTS_free","X509_free","BIO_free","BIO_free"],"critical_calls":["OPENSSL_init_ssl","ERR_clear_error","BIO_new_fd","d2i_X509_bio","BIO_new_fp","BIO_ctrl","CRL_DIST_POINTS_new","ASIdentifiers_new","CRYPTO_malloc","UI_create_method","UI_method_set_prompt_constructor","BIO_get_wpoll_descriptor","EVP_PKEY_new","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","EVP_PKEY_assign","X509_PUBKEY_set","EVP_PKEY_type_names_do_all","X509_PUBKEY_free","PEM_write_bio_X509","BN_free","RSA_free","EVP_PKEY_free","UI_destroy_method","CRYPTO_free","ASIdentifiers_free","CRL_DIST_POINTS_free","X509_free","BIO_free","BIO_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL library
    OPENSSL_init_ssl(0, NULL);
    
    // Clear any previous error stack
    ERR_clear_error();
    
    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
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
    
    // Create BIO from file descriptor for reading
    BIO *bio_fd = BIO_new_fd(in_fd, 0);  // Don't close fd
    if (!bio_fd) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Read X509 certificate from BIO
    X509 *x509 = NULL;
    x509 = d2i_X509_bio(bio_fd, &x509);
    
    // Create BIO from FILE* for writing
    BIO *bio_fp = BIO_new_fp(out_file, 0);  // Don't close FILE*
    if (!bio_fp) {
        BIO_free(bio_fd);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        X509_free(x509);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Test BIO control operation
    BIO_ctrl(bio_fd, BIO_CTRL_RESET, 0, NULL);
    
    // Create and use CRL_DIST_POINTS
    CRL_DIST_POINTS *crl_dist_points = CRL_DIST_POINTS_new();
    
    // Create and use ASIdentifiers
    ASIdentifiers *as_identifiers = ASIdentifiers_new();
    
    // Allocate memory using CRYPTO_malloc
    void *crypto_mem = CRYPTO_malloc(1024, __FILE__, __LINE__);
    
    // Create UI_METHOD
    UI_METHOD *ui_method = UI_create_method("TestUI");
    if (ui_method) {
        // Set prompt constructor (dummy function)
        UI_method_set_prompt_constructor(ui_method, 
            [](UI *ui, const char *prompt, const char *desc) -> char* {
                return NULL;
            });
    }
    
    // Try to get poll descriptor
    BIO_POLL_DESCRIPTOR desc;
    BIO_get_wpoll_descriptor(bio_fp, &desc);
    
    // Create RSA key
    EVP_PKEY *pkey = EVP_PKEY_new();
    RSA *rsa = RSA_new();
    BIGNUM *bn = BN_new();
    
    if (bn && rsa && pkey) {
        // Set BN value
        BN_set_word(bn, 65537);
        
        // Generate RSA key (using small bits for speed)
        RSA_generate_key_ex(rsa, 512, bn, NULL);
        
        // Assign RSA to EVP_PKEY
        EVP_PKEY_assign(pkey, EVP_PKEY_RSA, rsa);
        rsa = NULL;  // Ownership transferred
        
        // Create X509_PUBKEY and set the public key
        X509_PUBKEY *x509_pubkey = NULL;
        X509_PUBKEY_set(&x509_pubkey, pkey);
        
        // Iterate over EVP_PKEY type names
        EVP_PKEY_type_names_do_all(pkey, 
            [](const char *name, void *data) {
                // Callback function
            }, NULL);
        
        // Free X509_PUBKEY
        X509_PUBKEY_free(x509_pubkey);
    }
    
    // Write X509 certificate to BIO in PEM format if we have one
    if (x509) {
        PEM_write_bio_X509(bio_fp, x509);
    }
    
    // Cleanup - free all allocated resources
    if (bn) BN_free(bn);
    if (rsa) RSA_free(rsa);  // In case EVP_PKEY_assign didn't take ownership
    if (pkey) EVP_PKEY_free(pkey);
    
    if (ui_method) UI_destroy_method(ui_method);
    if (crypto_mem) CRYPTO_free(crypto_mem, __FILE__, __LINE__);
    
    if (as_identifiers) ASIdentifiers_free(as_identifiers);
    if (crl_dist_points) CRL_DIST_POINTS_free(crl_dist_points);
    
    if (x509) X509_free(x509);
    
    BIO_free(bio_fp);
    BIO_free(bio_fd);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}