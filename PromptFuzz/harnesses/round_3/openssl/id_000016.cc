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
//<ID> 323
//<Prompt> []
/*<Combination>: [
*/
//<score> 57, nr_unique_branch: 13
//<Quality> {"density":19,"unique_branches":{"x509_pubkey_ex_populate":[[101,13,101,34,0,0,4,0],[103,13,103,39,0,0,4,0]],"x509_pubkey_ex_d2i_ex":[[139,9,139,22,0,0,4,0],[139,26,139,73,0,0,4,1],[141,9,141,45,0,0,4,1],[147,9,151,13,0,0,4,0]],"SSL_set_accept_state":[[96,6,96,19,2,0,4,0],[17,6,17,19,3,0,4,1],[18,23,18,61,3,0,4,0],[1242,29,1242,48,4,0,4,1]],"file_ctrl":[[189,5,189,24,0,0,4,0],[190,13,190,49,0,0,4,1]],"asn1_item_embed_d2i":[[287,13,287,39,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","ERR_clear_error","SSL_CTX_new","TLS_method","SSL_CTX_dane_enable","BIO_new_fp","BIO_new_fp","BIO_free","BIO_free","SSL_CTX_free","ASN1_ITEM_lookup","ASN1_item_d2i_bio_ex","BIO_ctrl","ASN1_ITEM_lookup","ASN1_item_d2i_bio_ex","X509_PUBKEY_get","PEM_write_bio_PUBKEY","EVP_PKEY_free","X509_PUBKEY_free","X509_get_pubkey","PEM_write_bio_PUBKEY","EVP_PKEY_free","X509_free","sk_ESS_CERT_ID_cmpfunc_thunk","OPENSSL_sk_new","ossl_check_IPAddressOrRange_sk_type","OPENSSL_sk_free","SSL_new","SSL_set_accept_state","SSL_free","BIO_free","BIO_free","SSL_CTX_free","ERR_clear_error"],"critical_calls":["OPENSSL_init_ssl","ERR_clear_error","SSL_CTX_new","TLS_method","SSL_CTX_dane_enable","BIO_new_fp","BIO_new_fp","ASN1_ITEM_lookup","ASN1_item_d2i_bio_ex","BIO_ctrl","ASN1_ITEM_lookup","ASN1_item_d2i_bio_ex","X509_PUBKEY_get","PEM_write_bio_PUBKEY","EVP_PKEY_free","X509_PUBKEY_free","sk_ESS_CERT_ID_cmpfunc_thunk","OPENSSL_sk_new","ossl_check_IPAddressOrRange_sk_type","OPENSSL_sk_free","SSL_new","SSL_set_accept_state","SSL_free","BIO_free","BIO_free","SSL_CTX_free","ERR_clear_error"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified OpenSSL APIs to achieve parsing and processing of DANE-related certificate data:

*/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful processing
    
    // Initialize OpenSSL
    OPENSSL_init_ssl(0, NULL);
    ERR_clear_error();
    
    // Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create a FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create SSL context for DANE operations
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    if (!ctx) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // API 1: Enable DANE in SSL context
    int dane_result = SSL_CTX_dane_enable(ctx);
    
    // Create BIOs for I/O operations
    BIO *in_bio = BIO_new_fp(in_file, BIO_NOCLOSE);
    BIO *out_bio = BIO_new_fp(out_file, BIO_NOCLOSE);
    
    if (!in_bio || !out_bio) {
        if (in_bio) BIO_free(in_bio);
        if (out_bio) BIO_free(out_bio);
        SSL_CTX_free(ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // API 2: Parse ASN.1 structure from BIO (try parsing as X509 certificate)
    // Use X509_it() which should be available for certificate parsing
    X509 *cert = NULL;
    const ASN1_ITEM *cert_item = ASN1_ITEM_lookup("X509");
    if (cert_item) {
        cert = (X509 *)ASN1_item_d2i_bio_ex(cert_item, in_bio, NULL, NULL, NULL);
    }
    
    // If cert parsing failed, rewind and try as generic ASN.1 structure
    BIO_reset(in_bio);
    if (!cert) {
        // Try with a different ASN.1 item - using X509_PUBKEY structure
        const ASN1_ITEM *pubkey_item = ASN1_ITEM_lookup("X509_PUBKEY");
        if (pubkey_item) {
            X509_PUBKEY *pubkey = (X509_PUBKEY *)ASN1_item_d2i_bio_ex(
                pubkey_item, in_bio, NULL, NULL, NULL);
            
            if (pubkey) {
                // Extract EVP_PKEY from X509_PUBKEY
                EVP_PKEY *pkey = X509_PUBKEY_get(pubkey);
                if (pkey) {
                    // API 4: Write public key to BIO in PEM format
                    PEM_write_bio_PUBKEY(out_bio, pkey);
                    EVP_PKEY_free(pkey);
                }
                X509_PUBKEY_free(pubkey);
            }
        }
    } else {
        // We got a certificate, extract and write its public key
        EVP_PKEY *pkey = X509_get_pubkey(cert);
        if (pkey) {
            // API 4: Write public key to BIO in PEM format
            PEM_write_bio_PUBKEY(out_bio, pkey);
            EVP_PKEY_free(pkey);
        }
        X509_free(cert);
    }
    
    // API 5: Use the ESS_CERT_ID comparison function thunk
    // Create dummy comparison function
    int (*cmp_func)(const void *, const void *) = [](const void *a, const void *b) -> int {
        return 0; // Simple equality comparison
    };
    
    // Create dummy data for comparison
    unsigned char dummy1[16], dummy2[16];
    memset(dummy1, 0xAA, sizeof(dummy1));
    memset(dummy2, 0xBB, sizeof(dummy2));
    
    // Use the thunk function
    int cmp_result = sk_ESS_CERT_ID_cmpfunc_thunk(cmp_func, dummy1, dummy2);
    
    // API 1 (stack type checking): Create and use IPAddressOrRange stack
    // Note: We need to create the stack structure first
    struct stack_st_IPAddressOrRange *ip_stack = 
        (struct stack_st_IPAddressOrRange *)OPENSSL_sk_new(NULL);
    
    if (ip_stack) {
        // Use the type checking function
        OPENSSL_STACK *checked_stack = ossl_check_IPAddressOrRange_sk_type(ip_stack);
        
        // Add some dummy data if stack functions are available
        // (Simplified - in real usage we'd add actual IPAddressOrRange elements)
        
        // Clean up the stack
        OPENSSL_sk_free((OPENSSL_STACK *)ip_stack);
    }
    
    // Process remaining data with the SSL context
    // Create a simple SSL structure to use with the context
    SSL *ssl = SSL_new(ctx);
    if (ssl) {
        // Use some data to simulate SSL operations
        if (size > 50) {
            unsigned char buffer[50];
            size_t copy_size = size < sizeof(buffer) ? size : sizeof(buffer);
            memcpy(buffer, data, copy_size);
            
            // This is just to ensure the SSL structure is used
            SSL_set_accept_state(ssl);
        }
        SSL_free(ssl);
    }
    
    // Clean up all resources
    BIO_free(in_bio);
    BIO_free(out_bio);
    SSL_CTX_free(ctx);
    
    // Close file handles
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clear any OpenSSL errors that might have occurred
    ERR_clear_error();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}