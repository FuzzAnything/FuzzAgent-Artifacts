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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 88, nr_unique_branch: 84
//<Quality> {"density":8,"unique_branches":{"evp_kdf_from_algorithm":[[66,9,66,38,0,0,4,1],[71,9,71,75,0,0,4,1],[76,12,76,33,0,0,4,0],[76,12,76,33,0,0,4,1],[77,17,77,33,0,0,4,1],[78,9,78,34,0,0,4,0],[78,9,78,34,0,0,4,1],[79,17,79,36,0,0,4,1],[84,9,84,34,0,0,4,0],[84,9,84,34,0,0,4,1],[85,17,85,36,0,0,4,1],[89,9,89,35,0,0,4,0],[89,9,89,35,0,0,4,1],[90,17,90,37,0,0,4,1],[95,9,95,33,0,0,4,0],[95,9,95,33,0,0,4,1],[96,17,96,35,0,0,4,1],[100,9,100,34,0,0,4,0],[100,9,100,34,0,0,4,1],[101,17,101,36,0,0,4,1],[106,9,106,43,0,0,4,1],[111,9,111,47,0,0,4,0],[111,9,111,47,0,0,4,1],[112,17,112,49,0,0,4,1],[116,9,116,47,0,0,4,0],[116,9,116,47,0,0,4,1],[117,17,117,49,0,0,4,1],[121,9,121,38,0,0,4,1],[126,9,126,42,0,0,4,0],[126,9,126,42,0,0,4,1],[127,17,127,44,0,0,4,1],[131,9,131,42,0,0,4,0],[131,9,131,42,0,0,4,1],[132,17,132,44,0,0,4,1],[136,9,136,36,0,0,4,1],[141,9,141,39,0,0,4,1],[148,9,148,22,0,0,4,1],[148,26,148,39,0,0,4,1],[157,9,157,21,0,0,4,0],[157,25,157,52,0,0,4,1]],"evp_kdf_free":[[34,9,34,20,0,0,4,1],[38,9,38,16,0,0,4,0]],"EVP_KDF_CTX_new":[[28,9,28,20,0,0,4,1],[32,9,32,20,0,0,4,1],[33,12,33,77,0,0,4,1],[34,12,34,32,0,0,4,1]],"EVP_PKEY_derive_init_ex":[[362,12,362,15,0,0,4,0]],"hkdf_common_set_ctx_params":[[299,9,299,26,0,0,4,1],[312,9,312,24,0,0,4,1],[338,9,338,23,0,0,4,1],[346,9,346,24,0,0,4,1],[355,9,357,13,0,0,4,1]],"kdf_hkdf_new":[[101,9,101,32,0,0,4,1],[110,9,110,53,0,0,4,0]],"kdf_init":[[88,9,88,32,0,0,4,1],[89,12,89,27,0,0,4,1],[90,12,90,24,0,0,4,1],[91,12,91,39,0,0,4,1]],"kdf_hkdf_set_ctx_params":[[368,9,368,20,0,0,4,1],[368,24,368,64,0,0,4,1],[374,9,374,45,0,0,4,1]],"EVP_PKEY_derive_set_peer_ex":[[388,9,388,47,0,0,4,0]],"ossl_kdf_data_free":[[53,9,53,24,0,0,4,1],[57,9,57,16,0,0,4,0],[57,9,57,16,0,0,4,1]],"EVP_KDF_CTX_free":[[48,9,48,20,0,0,4,1]],"hkdf_set_ctx_params_decoder":[[51,9,51,18,0,0,4,1]],"ossl_kdf_data_up_ref":[[75,9,75,32,0,0,4,1]],"evp_kdf_new":[[50,9,50,53,0,0,4,1],[51,12,51,44,0,0,4,1]],"kdf_hkdf_reset_ex":[[136,27,136,34,0,0,4,0],[140,9,140,24,0,0,4,1],[156,9,156,24,0,0,4,1]],"EVP_KDF_CTX_set_params":[[307,9,307,42,0,0,4,0]],"deflt_query":[[751,5,751,21,0,0,4,0]],"ossl_param_get1_concat_octet_string":[[1632,9,1632,15,0,0,4,0]],"kdf_hkdf_free":[[121,9,121,20,0,0,4,0]],"ossl_kdf_data_new":[[33,9,33,32,0,0,4,1],[37,9,37,24,0,0,4,1],[40,9,40,45,0,0,4,1]],"kdf_newctx":[[50,9,50,32,0,0,4,1],[54,9,54,23,0,0,4,1],[60,9,60,20,0,0,4,1],[65,9,65,31,0,0,4,1]]},"library_calls":["OPENSSL_init_ssl","ERR_peek_last_error_all","SSL_CTX_new","TLS_method","SSL_new","SSL_set_default_read_buffer_len","SSL_new_listener_from","SSL_free","SSL_free","SSL_CTX_free","TS_TST_INFO_new","TS_TST_INFO_free","ossl_check_OCSP_SINGLERESP_compfunc_type","EVP_PKEY_CTX_new_id","EVP_PKEY_derive_init","EVP_PKEY_new","EVP_PKEY_derive_set_peer_ex","EVP_PKEY_free","EVP_PKEY_CTX_free"],"critical_calls":["OPENSSL_init_ssl","ERR_peek_last_error_all","SSL_CTX_new","TLS_method","SSL_new","SSL_set_default_read_buffer_len","SSL_new_listener_from","SSL_free","SSL_free","SSL_CTX_free","TS_TST_INFO_new","TS_TST_INFO_free","ossl_check_OCSP_SINGLERESP_compfunc_type","EVP_PKEY_CTX_new_id","EVP_PKEY_derive_init","EVP_PKEY_new","EVP_PKEY_derive_set_peer_ex","EVP_PKEY_free","EVP_PKEY_CTX_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL
    OPENSSL_init_ssl(0, NULL);
    
    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create FILE* for output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Use ERR_peek_last_error_all to check for errors
    const char *file = NULL;
    int line = 0;
    const char *func = NULL;
    const char *err_data = NULL;
    int flags = 0;
    unsigned long err = ERR_peek_last_error_all(&file, &line, &func, &err_data, &flags);
    
    // 2. Create SSL context and SSL object for SSL_new_listener_from
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = SSL_new(ctx);
    if (ssl) {
        // 6. Set default read buffer length
        SSL_set_default_read_buffer_len(ssl, 16384); // 16KB buffer
        
        // 4. Create listener SSL (flags from input data - use first 8 bytes if available)
        uint64_t listener_flags = 0;
        if (size >= sizeof(listener_flags)) {
            memcpy(&listener_flags, data, sizeof(listener_flags));
        }
        SSL *listener_ssl = SSL_new_listener_from(ssl, listener_flags);
        
        if (listener_ssl) {
            SSL_free(listener_ssl);
        }
        SSL_free(ssl);
    }
    SSL_CTX_free(ctx);
    
    // 3. Create and free TS_TST_INFO structure
    TS_TST_INFO *ts_info = TS_TST_INFO_new();
    if (ts_info) {
        // Try to parse some data as timestamp (this will likely fail but we call the API)
        TS_TST_INFO_free(ts_info);
    }
    
    // 5. Use OCSP_SINGLERESP comparison function
    // Create a dummy comparison function
    auto cmp_func = [](const OCSP_SINGLERESP *const *a, const OCSP_SINGLERESP *const *b) -> int {
        if (!a || !b || !*a || !*b) return 0;
        // Simple comparison based on pointer values for fuzzing
        return (*a < *b) ? -1 : ((*a > *b) ? 1 : 0);
    };
    
    // Convert to the proper function type
    sk_OCSP_SINGLERESP_compfunc compfunc = cmp_func;
    OPENSSL_sk_compfunc checked_compfunc = ossl_check_OCSP_SINGLERESP_compfunc_type(compfunc);
    
    // 2. Use EVP_PKEY_derive_set_peer_ex
    // Create PKEY context for key derivation
    EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_HKDF, NULL);
    if (pctx) {
        if (EVP_PKEY_derive_init(pctx) > 0) {
            // Create a temporary peer key (using RSA for example)
            EVP_PKEY *peer_key = EVP_PKEY_new();
            if (peer_key) {
                // Try to set peer (validation will likely fail but we call the API)
                EVP_PKEY_derive_set_peer_ex(pctx, peer_key, 0);
                EVP_PKEY_free(peer_key);
            }
        }
        EVP_PKEY_CTX_free(pctx);
    }
    
    // Try to read some data from input file to consume it
    unsigned char buffer[256];
    while (!feof(in_file) && !ferror(in_file)) {
        size_t read = fread(buffer, 1, sizeof(buffer), in_file);
        if (read > 0) {
            // Write to output file to demonstrate file usage
            fwrite(buffer, 1, read, out_file);
        }
    }
    
    // Get file descriptors if needed
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}