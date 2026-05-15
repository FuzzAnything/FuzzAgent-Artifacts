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
//<ID> 455
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 2
//<Quality> {"density":11,"unique_branches":{"fd_ctrl":[[154,5,154,25,0,0,4,0],[193,5,193,12,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","ERR_clear_error","BIO_new_fd","d2i_X509_bio","BIO_ctrl","CRL_DIST_POINTS_new","ASIdentifiers_new","ASIdentifiers_new","CRYPTO_malloc","CRYPTO_free","X509v3_asid_subset","ASN1_SET_ANY_it","BIO_get_wpoll_descriptor","X509_get0_trust_objects","X509_get_pubkey","X509_PUBKEY_set","EVP_PKEY_type_names_do_all","X509_PUBKEY_free","EVP_PKEY_free","EVP_PKEY_new","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","BN_free","EVP_PKEY_assign","X509_PUBKEY_set","X509_PUBKEY_free","EVP_PKEY_type_names_do_all","EVP_PKEY_free","BIO_new_fp","PEM_write_bio_X509","BIO_free","ASIdentifiers_free","ASIdentifiers_free","CRL_DIST_POINTS_free","X509_free","BIO_free","ERR_clear_error"],"critical_calls":["OPENSSL_init_ssl","ERR_clear_error","BIO_new_fd","d2i_X509_bio","BIO_ctrl","CRL_DIST_POINTS_new","ASIdentifiers_new","ASIdentifiers_new","CRYPTO_malloc","CRYPTO_free","X509v3_asid_subset","ASN1_SET_ANY_it","BIO_get_wpoll_descriptor","EVP_PKEY_new","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","BN_free","EVP_PKEY_assign","X509_PUBKEY_set","X509_PUBKEY_free","EVP_PKEY_type_names_do_all","EVP_PKEY_free","BIO_new_fp","PEM_write_bio_X509","BIO_free","ASIdentifiers_free","ASIdentifiers_free","CRL_DIST_POINTS_free","X509_free","BIO_free","ERR_clear_error"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified OpenSSL APIs to process X.509 certificate data and perform ASN.1 operations:

*/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Initialize OpenSSL
    OPENSSL_init_ssl(0, NULL);
    ERR_clear_error();

    // 1. First, create BIOs to handle the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    int fd = fuzz_fileno(in_file);
    BIO *bio = BIO_new_fd(fd, BIO_NOCLOSE);
    if (!bio) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 2. Try to parse X509 certificate from the input
    X509 *x509 = d2i_X509_bio(bio, NULL);
    
    // Reset BIO position for other operations
    BIO_seek(bio, 0);
    
    // 3. Create CRL_DIST_POINTS structure
    CRL_DIST_POINTS *crl_dp = CRL_DIST_POINTS_new();
    
    // 4. Create ASIdentifiers structures for X509v3_asid_subset
    ASIdentifiers *asid1 = ASIdentifiers_new();
    ASIdentifiers *asid2 = ASIdentifiers_new();
    
    // Populate with some test data from input
    if (size >= 200) {
        // Use parts of input data for ASN.1 objects
        unsigned char *data_copy = (unsigned char *)OPENSSL_malloc(size);
        if (data_copy) {
            memcpy(data_copy, data, size);
            
            // Create simple ASN.1 structures from input data
            if (asid1 && asid2) {
                // For simplicity, we'll create minimal structures
                // In real usage, you'd properly parse ASIdentifiers from data
            }
            OPENSSL_free(data_copy);
        }
    }
    
    // 5. Call X509v3_asid_subset
    if (asid1 && asid2) {
        int subset_result = X509v3_asid_subset(asid1, asid2);
        // Result can be -1 (error), 0 (not subset), or 1 (subset)
        (void)subset_result; // Use result to avoid unused variable warning
    }
    
    // 6. Get ASN1_SET_ANY item
    const ASN1_ITEM *set_item = ASN1_SET_ANY_it();
    (void)set_item;
    
    // 7. Get poll descriptor from BIO
    BIO_POLL_DESCRIPTOR desc;
    int poll_result = BIO_get_wpoll_descriptor(bio, &desc);
    (void)poll_result;
    
    // 8. If we got an X509 certificate, use related APIs
    if (x509) {
        // Get trust objects from X509
        const struct stack_st_ASN1_OBJECT *trust_objs = X509_get0_trust_objects(x509);
        (void)trust_objs;
        
        // Try to get public key from certificate
        EVP_PKEY *pkey = X509_get_pubkey(x509);
        if (pkey) {
            // Create X509_PUBKEY and set the public key
            X509_PUBKEY *x509_pubkey = NULL;
            int set_result = X509_PUBKEY_set(&x509_pubkey, pkey);
            (void)set_result;
            
            // Iterate over all type names for the EVP_PKEY
            EVP_PKEY_type_names_do_all(pkey, 
                [](const char *name, void *data) {
                    // Callback function - just mark as used
                    (void)name;
                    (void)data;
                }, 
                NULL);
            
            // Clean up
            if (x509_pubkey) X509_PUBKEY_free(x509_pubkey);
            EVP_PKEY_free(pkey);
        }
    } else {
        // If no X509, create a dummy EVP_PKEY for testing
        EVP_PKEY *pkey = EVP_PKEY_new();
        if (pkey) {
            // Create a minimal RSA key
            RSA *rsa = RSA_new();
            if (rsa) {
                BIGNUM *bn = BN_new();
                if (bn) {
                    BN_set_word(bn, RSA_F4);
                    RSA_generate_key_ex(rsa, 512, bn, NULL);
                    BN_free(bn);
                }
                
                EVP_PKEY_assign_RSA(pkey, rsa);
                
                // Try to use X509_PUBKEY_set
                X509_PUBKEY *x509_pubkey = NULL;
                X509_PUBKEY_set(&x509_pubkey, pkey);
                
                if (x509_pubkey) X509_PUBKEY_free(x509_pubkey);
                
                // Don't free rsa - it's now owned by pkey
            }
            
            // Call EVP_PKEY_type_names_do_all
            EVP_PKEY_type_names_do_all(pkey, 
                [](const char *name, void *data) {
                    (void)name;
                    (void)data;
                }, 
                NULL);
            
            EVP_PKEY_free(pkey);
        }
    }
    
    // 9. Create output file for any potential output
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some processed data if needed
        if (x509) {
            // Write certificate in PEM format
            BIO *out_bio = BIO_new_fp(out_file, BIO_NOCLOSE);
            if (out_bio) {
                PEM_write_bio_X509(out_bio, x509);
                BIO_free(out_bio);
            }
        }
        assert_file_closed(&out_file);;
    }
    
    // 10. Clean up all allocated resources
    if (asid1) ASIdentifiers_free(asid1);
    if (asid2) ASIdentifiers_free(asid2);
    if (crl_dp) CRL_DIST_POINTS_free(crl_dp);
    if (x509) X509_free(x509);
    if (bio) BIO_free(bio);
    if (in_file) assert_file_closed(&in_file);;
    
    // Clear any remaining OpenSSL errors
    ERR_clear_error();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}