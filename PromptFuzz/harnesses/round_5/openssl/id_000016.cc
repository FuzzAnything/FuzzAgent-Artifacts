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
//<ID> 201
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 2
//<Quality> {"density":5,"unique_branches":{"SSL_set_alpn_protos":[[3946,9,3946,43,0,0,4,0]],"alpn_value_ok":[[3890,13,3890,29,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","X509_STORE_new","X509_STORE_set_check_revocation","EVP_aria_256_cbc","SSL_CTX_new","TLS_method","SSL_new","SSL_set_alpn_protos","SSL_get0_alpn_selected","OSSL_PARAM_construct_ulong","OSSL_PARAM_construct_end","OSSL_PARAM_get_ulong","DSA_new","EVP_PKEY_new","EVP_PKEY_set1_DSA","ASYNC_WAIT_CTX_new","ASYNC_WAIT_CTX_set_callback","d2i_X509","X509_get0_pubkey","X509_free","EVP_PKEY_free","EVP_PKEY_free","DSA_free","ASYNC_WAIT_CTX_free","SSL_free","SSL_CTX_free","X509_STORE_free"],"critical_calls":["OPENSSL_init_ssl","X509_STORE_new","X509_STORE_set_check_revocation","EVP_aria_256_cbc","SSL_CTX_new","TLS_method","SSL_new","SSL_set_alpn_protos","SSL_get0_alpn_selected","OSSL_PARAM_construct_ulong","OSSL_PARAM_construct_end","OSSL_PARAM_get_ulong","DSA_new","EVP_PKEY_new","EVP_PKEY_set1_DSA","ASYNC_WAIT_CTX_new","ASYNC_WAIT_CTX_set_callback","d2i_X509","X509_get0_pubkey","X509_free","EVP_PKEY_free","EVP_PKEY_free","DSA_free","ASYNC_WAIT_CTX_free","SSL_free","SSL_CTX_free","X509_STORE_free"],"visited":0}
/**/


// Define dummy callback functions
static int  id_000016_dummy_revocation_check(X509_STORE_CTX* ctx) {
    return 1;
}

static int  id_000016_dummy_callback(void* arg) {
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
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
    OPENSSL_init_ssl(0, NULL);
    
    // 1. Create and setup X509_STORE with custom revocation check
    X509_STORE* store = X509_STORE_new();
    if (store) {
        X509_STORE_set_check_revocation(store,  id_000016_dummy_revocation_check);
    }
    
    // 2. Get ARIA-256-CBC cipher
    const EVP_CIPHER* cipher = EVP_aria_256_cbc();
    (void)cipher; // Use to avoid unused variable warning
    
    // 3. Create SSL context and object for ALPN operations
    SSL_CTX* ctx = SSL_CTX_new(TLS_method());
    SSL* ssl = NULL;
    if (ctx) {
        ssl = SSL_new(ctx);
        if (ssl) {
            // Set ALPN protocols from input data (first 50 bytes)
            size_t alpn_len = size > 50 ? 50 : size;
            unsigned char* alpn_data = (unsigned char*)data;
            
            // Set ALPN protocols (simplified - just using raw data)
            SSL_set_alpn_protos(ssl, alpn_data, alpn_len);
            
            // Get ALPN selected (will be NULL as no handshake done)
            const unsigned char* alpn_selected = NULL;
            unsigned int alpn_selected_len = 0;
            SSL_get0_alpn_selected(ssl, &alpn_selected, &alpn_selected_len);
        }
    }
    
    // 4. Create and use OSSL_PARAM with ulong value
    OSSL_PARAM param[2];
    unsigned long ul_val = 0;
    
    // Use first 4 bytes of data for ulong value
    if (size >= 4) {
        memcpy(&ul_val, data, 4);
    }
    
    param[0] = OSSL_PARAM_construct_ulong("test_ulong", &ul_val);
    param[1] = OSSL_PARAM_construct_end();
    
    unsigned long retrieved_val = 0;
    OSSL_PARAM_get_ulong(&param[0], &retrieved_val);
    
    // 5. Create DSA key and set it in EVP_PKEY
    DSA* dsa = DSA_new();
    EVP_PKEY* pkey = EVP_PKEY_new();
    
    if (dsa && pkey) {
        // Generate DSA parameters (using simplified approach)
        // In real usage, you'd properly generate/load DSA parameters
        EVP_PKEY_set1_DSA(pkey, dsa);
    }
    
    // 6. Setup ASYNC_WAIT_CTX with callback
    ASYNC_WAIT_CTX* async_ctx = ASYNC_WAIT_CTX_new();
    if (async_ctx) {
        ASYNC_WAIT_CTX_set_callback(async_ctx,  id_000016_dummy_callback, NULL);
    }
    
    // 7. Create X509 certificate and get its public key
    X509* x509 = NULL;
    const unsigned char* p = data;
    long len = size;
    
    // Try to parse input as X509 certificate
    x509 = d2i_X509(NULL, &p, len);
    
    EVP_PKEY* pubkey = NULL;
    if (x509) {
        pubkey = X509_get0_pubkey(x509);
    }
    
    // 8. Process data through a file (as requested in requirements)
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // Read some data from the file to demonstrate usage
        unsigned char buffer[100];
        size_t read_bytes = fread(buffer, 1, sizeof(buffer), in_file);
        (void)read_bytes; // Use to avoid unused variable warning
        
        // Get file descriptor if needed
        int fd = fuzz_fileno(in_file);
        (void)fd; // Use to avoid unused variable warning
        
        assert_file_closed(&in_file);;
    }
    
    // 9. Create output file (as requested in requirements)
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some processed data
        if (size > 0) {
            fwrite(data, 1, size > 100 ? 100 : size, out_file);
        }
        
        // Get file descriptor if needed
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd; // Use to avoid unused variable warning
        
        assert_file_closed(&out_file);;
    }
    
    // Cleanup all allocated resources
    if (x509) X509_free(x509);
    if (pubkey) EVP_PKEY_free(pubkey);
    if (pkey) EVP_PKEY_free(pkey);
    if (dsa) DSA_free(dsa);
    if (async_ctx) ASYNC_WAIT_CTX_free(async_ctx);
    if (ssl) SSL_free(ssl);
    if (ctx) SSL_CTX_free(ctx);
    if (store) X509_STORE_free(store);
    
    // Remove output file if created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}