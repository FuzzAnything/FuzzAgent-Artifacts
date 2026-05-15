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
//<ID> 254
//<Prompt> []
/*<Combination>: [
*/
//<score> 154, nr_unique_branch: 49
//<Quality> {"density":11,"unique_branches":{"BIO_free_all":[[875,12,875,23,0,0,4,0],[881,13,881,20,0,0,4,0],[881,13,881,20,0,0,4,1]],"OSSL_ERR_STATE_save":[[29,9,29,19,0,0,4,0]],"SSL_set0_rbio":[[1610,9,1610,19,0,0,4,1],[96,6,96,19,2,0,4,0],[32,6,32,19,3,0,4,1],[33,23,33,61,3,0,4,0],[1242,29,1242,48,4,0,4,1]],"SSL_set0_wbio":[[1629,9,1629,19,0,0,4,1],[1635,9,1635,25,0,0,4,1],[1642,9,1642,25,0,0,4,1],[96,6,96,19,2,0,4,0],[32,6,32,19,3,0,4,1],[33,23,33,61,3,0,4,0],[1242,29,1242,48,4,0,4,1]],"SSL_set_connect_state":[[96,6,96,19,2,0,4,0],[17,6,17,19,3,0,4,1],[18,23,18,61,3,0,4,0],[1242,29,1242,48,4,0,4,1]],"ssl_set_new_record_layer":[[1372,9,1372,18,0,0,4,1],[1375,16,1375,26,0,0,4,0],[1375,30,1375,70,0,0,4,0],[1375,30,1375,70,0,0,4,1]],"SSL_set_bio":[[1656,9,1656,32,0,0,4,1],[1663,9,1663,21,0,0,4,0],[1663,25,1663,37,0,0,4,0],[1664,13,1664,30,0,0,4,1],[1671,9,1671,32,0,0,4,1],[1680,9,1680,32,0,0,4,1]],"SSL_get_wbio":[[1714,9,1714,19,0,0,4,1],[1717,9,1717,25,0,0,4,1],[96,6,96,19,2,0,4,0],[32,6,32,19,3,0,4,1],[33,23,33,61,3,0,4,0],[1242,29,1242,48,4,0,4,1]],"SSL_get_rbio":[[1699,9,1699,19,0,0,4,1],[96,6,96,19,2,0,4,0],[32,6,32,19,3,0,4,1],[33,23,33,61,3,0,4,0],[1242,29,1242,48,4,0,4,1]],"tls_set1_bio":[[1945,9,1945,20,0,0,4,0],[1945,24,1945,40,0,0,4,1]],"SSL_client_hello_get0_compression_methods":[[7034,9,7034,19,0,0,4,1],[7037,9,7037,32,0,0,4,0],[32,6,32,19,2,0,4,1],[33,23,33,61,2,0,4,0]],"OPENSSL_sk_sort":[[512,38,512,54,0,0,4,1]],"asn1_ex_i2c":[[604,5,604,30,0,0,4,0]]},"library_calls":["OSSL_ERR_STATE_save","d2i_X509_CRL","EVP_sha256","X509_CRL_digest","SSL_CTX_new","TLS_method","SSL_new","SSL_set_connect_state","BIO_new_mem_buf","SSL_set_bio","SSL_client_hello_get0_compression_methods","sk_OCSP_RESPONSE_new_null","d2i_OCSP_RESPONSE","sk_OCSP_RESPONSE_push","d2i_OCSP_RESPONSE","sk_OCSP_RESPONSE_push","sk_OCSP_RESPONSE_sort","d2i_X509","X509_issuer_and_serial_hash","X509_CRL_free","SSL_free","SSL_CTX_free","sk_OCSP_RESPONSE_num","sk_OCSP_RESPONSE_pop","OCSP_RESPONSE_free","sk_OCSP_RESPONSE_free","X509_free","ERR_clear_error"],"critical_calls":["OSSL_ERR_STATE_save","d2i_X509_CRL","EVP_sha256","X509_CRL_digest","SSL_CTX_new","TLS_method","SSL_new","SSL_set_connect_state","BIO_new_mem_buf","SSL_set_bio","SSL_client_hello_get0_compression_methods","sk_OCSP_RESPONSE_new_null","d2i_OCSP_RESPONSE","sk_OCSP_RESPONSE_push","d2i_OCSP_RESPONSE","sk_OCSP_RESPONSE_push","sk_OCSP_RESPONSE_sort","d2i_X509","X509_issuer_and_serial_hash","X509_CRL_free","SSL_free","SSL_CTX_free","sk_OCSP_RESPONSE_num","sk_OCSP_RESPONSE_pop","OCSP_RESPONSE_free","sk_OCSP_RESPONSE_free","X509_free","ERR_clear_error"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Setup error state
    ERR_STATE *err_state = NULL;
    // Save current error state
    OSSL_ERR_STATE_save(err_state);
    
    // 2. Process X509 CRL data
    X509_CRL *crl = NULL;
    const unsigned char *p = data;
    long remaining = static_cast<long>(size);
    
    // Try to parse CRL from input data
    crl = d2i_X509_CRL(NULL, &p, remaining);
    if (crl) {
        unsigned char md[EVP_MAX_MD_SIZE];
        unsigned int md_len = 0;
        
        // Get digest of CRL using SHA256
        const EVP_MD *md_type = EVP_sha256();
        if (X509_CRL_digest(crl, md_type, md, &md_len)) {
            // Digest computed successfully
        }
        
        // Update remaining data pointer
        remaining -= (p - data);
    }
    
    // 3. Process SSL/TLS client hello data
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = SSL_new(ssl_ctx);
    
    if (ssl && remaining > 0) {
        // Set up SSL for reading client hello
        SSL_set_connect_state(ssl);
        
        // Feed data to SSL object
        BIO *bio = BIO_new_mem_buf(p, static_cast<int>(remaining));
        SSL_set_bio(ssl, bio, bio);
        
        // Try to get compression methods from client hello
        const unsigned char *comp_methods = NULL;
        size_t comp_len = SSL_client_hello_get0_compression_methods(ssl, &comp_methods);
        
        // Update remaining data
        if (remaining > 0) {
            p += remaining;
            remaining = 0;
        }
    }
    
    // 4. Process OCSP response data
    struct stack_st_OCSP_RESPONSE *ocsp_stack = sk_OCSP_RESPONSE_new_null();
    
    if (ocsp_stack && size > 0) {
        // Try to parse OCSP response from beginning of data
        const unsigned char *ocsp_data = data;
        long ocsp_size = static_cast<long>(size);
        OCSP_RESPONSE *ocsp_resp = d2i_OCSP_RESPONSE(NULL, &ocsp_data, ocsp_size);
        
        if (ocsp_resp) {
            sk_OCSP_RESPONSE_push(ocsp_stack, ocsp_resp);
            
            // Add a second response (duplicate) for sorting
            const unsigned char *ocsp_data2 = data;
            OCSP_RESPONSE *ocsp_resp2 = d2i_OCSP_RESPONSE(NULL, &ocsp_data2, 
                ocsp_size > 100 ? 100 : ocsp_size); // Limit size for second parse
            if (ocsp_resp2) {
                sk_OCSP_RESPONSE_push(ocsp_stack, ocsp_resp2);
            }
            
            // Sort the OCSP responses
            sk_OCSP_RESPONSE_sort(ocsp_stack);
        }
    }
    
    // 5. Process X509 certificate data
    X509 *cert = NULL;
    if (size > 0) {
        const unsigned char *cert_data = data;
        long cert_size = static_cast<long>(size > 1000 ? 1000 : size); // Limit for cert parsing
        
        cert = d2i_X509(NULL, &cert_data, cert_size);
        if (cert) {
            // Compute hash of issuer and serial
            unsigned long hash = X509_issuer_and_serial_hash(cert);
            (void)hash; // Use hash to avoid unused variable warning
        }
    }
    
    // 6. Cleanup all allocated resources
    if (err_state) {
        // Note: OSSL_ERR_STATE_save doesn't allocate, just saves state
        // No cleanup needed for err_state
    }
    
    if (crl) {
        X509_CRL_free(crl);
    }
    
    if (ssl) {
        SSL_free(ssl);
    }
    
    if (ssl_ctx) {
        SSL_CTX_free(ssl_ctx);
    }
    
    if (ocsp_stack) {
        // Free all OCSP responses in the stack
        while (sk_OCSP_RESPONSE_num(ocsp_stack) > 0) {
            OCSP_RESPONSE *resp = sk_OCSP_RESPONSE_pop(ocsp_stack);
            if (resp) {
                OCSP_RESPONSE_free(resp);
            }
        }
        sk_OCSP_RESPONSE_free(ocsp_stack);
    }
    
    if (cert) {
        X509_free(cert);
    }
    
    // Clear any accumulated errors
    ERR_clear_error();
    
    return 0;
}