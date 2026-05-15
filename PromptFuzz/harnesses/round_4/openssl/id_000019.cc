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
//<score> 9, nr_unique_branch: 5
//<Quality> {"density":3,"unique_branches":{"X509v3_get_ext_count":[[26,9,26,18,0,0,4,0]],"SSL_CTX_is_quic":[[52,6,52,38,2,0,4,1],[52,42,52,81,2,0,4,1],[52,85,52,117,2,0,4,1],[52,121,52,146,2,0,4,1]]},"library_calls":["d2i_OCSP_REQUEST","OCSP_REQUEST_get_ext_count","SSL_CTX_new","TLS_method","SSL_CTX_is_quic","EVP_KEYMGMT_do_all_provided","OSSL_FUNC_store_eof","ESS_SIGNING_CERT_new","ESS_SIGNING_CERT_free","OCSP_REQUEST_free","SSL_CTX_free"],"critical_calls":["d2i_OCSP_REQUEST","OCSP_REQUEST_get_ext_count","SSL_CTX_new","TLS_method","SSL_CTX_is_quic","EVP_KEYMGMT_do_all_provided","OSSL_FUNC_store_eof","ESS_SIGNING_CERT_new","ESS_SIGNING_CERT_free","OCSP_REQUEST_free","SSL_CTX_free"],"visited":0}
/**/


// Dummy callback for EVP_KEYMGMT_do_all_provided
static void  id_000019_dummy_keymgmt_cb(EVP_KEYMGMT *keymgmt, void *arg) {
    // Do nothing, just satisfy the callback requirement
    (void)keymgmt;
    (void)arg;
}

// Dummy store eof function for dispatch table
static int  id_000019_dummy_store_eof(void *loaderctx) {
    (void)loaderctx;
    return 1; // Indicate EOF
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize variables for resource cleanup tracking
    OCSP_REQUEST *ocsp_req = nullptr;
    SSL_CTX *ssl_ctx = nullptr;
    ESS_SIGNING_CERT *ess_signing_cert = nullptr;
    OSSL_DISPATCH dummy_dispatch[2]; // For store eof function dispatch
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    
    // 1. Process OCSP request from input data
    if (size > 0) {
        // Create OCSP_REQUEST from input data
        const unsigned char *p = data;
        ocsp_req = d2i_OCSP_REQUEST(nullptr, &p, static_cast<long>(size));
        
        if (ocsp_req) {
            // Use OCSP_REQUEST_get_ext_count
            int ext_count = OCSP_REQUEST_get_ext_count(ocsp_req);
            (void)ext_count; // Use variable to avoid unused warning
        }
    }
    
    // 2. Create and check SSL_CTX for QUIC
    ssl_ctx = SSL_CTX_new(TLS_method());
    if (ssl_ctx) {
        // Use SSL_CTX_is_quic
        int is_quic = SSL_CTX_is_quic(ssl_ctx);
        (void)is_quic; // Use variable to avoid unused warning
    }
    
    // 3. Iterate over all key management methods
    EVP_KEYMGMT_do_all_provided(nullptr,  id_000019_dummy_keymgmt_cb, nullptr);
    
    // 4. Set up and use OSSL_FUNC_store_eof with dummy dispatch table
    // Create a minimal dispatch table with store eof function
    dummy_dispatch[0].function_id = OSSL_FUNC_STORE_EOF;
    dummy_dispatch[0].function = (void(*)(void)) id_000019_dummy_store_eof;
    dummy_dispatch[1].function_id = 0;
    dummy_dispatch[1].function = nullptr;
    
    // Get store eof function pointer
    OSSL_FUNC_store_eof_fn *store_eof_func = OSSL_FUNC_store_eof(dummy_dispatch);
    if (store_eof_func) {
        // Call the store eof function with null context
        int eof_result = store_eof_func(nullptr);
        (void)eof_result; // Use variable to avoid unused warning
    }
    
    // 5. Create and free ESS_SIGNING_CERT
    ess_signing_cert = ESS_SIGNING_CERT_new();
    
    // 6. Use input data with file operations as examples
    if (size > 0) {
        // Create input file from memory buffer
        in_file = fmemopen((void*)data, size, "rb");
        if (in_file) {
            // Example: get file descriptor
            int fd_in = fuzz_fileno(in_file);
            (void)fd_in; // Use variable to avoid unused warning
        }
        
        // Create output file
        out_file = fopen("output_file", "wb");
        if (out_file && in_file) {
            // Example: copy data from input to output
            uint8_t buffer[256];
            size_t bytes_read;
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
                fwrite(buffer, 1, bytes_read, out_file);
            }
            
            // Get output file descriptor
            int fd_out = fuzz_fileno(out_file);
            (void)fd_out; // Use variable to avoid unused warning
        }
    }
    
    // Clean up all allocated resources
    if (ess_signing_cert) {
        ESS_SIGNING_CERT_free(ess_signing_cert);
    }
    
    if (ocsp_req) {
        OCSP_REQUEST_free(ocsp_req);
    }
    
    if (ssl_ctx) {
        SSL_CTX_free(ssl_ctx);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
        // Remove output file since it's just for demonstration
        remove("output_file");
    }
    
    return 0;
}