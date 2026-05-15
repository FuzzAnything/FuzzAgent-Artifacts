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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 65, nr_unique_branch: 10
//<Quality> {"density":13,"unique_branches":{"EVP_PKEY_derive_set_peer_ex":[[383,9,383,35,0,0,4,0]],"ossl_err_string_int":[[523,9,523,17,0,0,4,1],[528,9,528,19,0,0,4,1],[548,9,548,19,0,0,4,1],[555,9,555,31,0,0,4,1],[222,35,222,71,1,0,4,1]],"OPENSSL_sk_pop_free":[[465,17,465,39,0,0,4,1]],"ERR_lib_error_string":[[586,9,586,18,0,0,4,1],[593,13,593,24,0,0,4,1],[130,6,130,48,1,0,4,0]]},"library_calls":["OPENSSL_init_ssl","OPENSSL_init_ssl","OPENSSL_init_crypto","SSL_CTX_new","TLS_method","SSL_new","SSL_set_default_read_buffer_len","SSL_new_listener_from","SSL_free","EVP_PKEY_CTX_new_id","EVP_PKEY_new","EVP_PKEY_derive_set_peer_ex","EVP_PKEY_free","EVP_PKEY_CTX_free","d2i_TS_TST_INFO_fp","TS_TST_INFO_free","ossl_check_OCSP_SINGLERESP_compfunc_type","OPENSSL_sk_set_cmp_thunks","OPENSSL_sk_new","ossl_check_OCSP_SINGLERESP_compfunc_type","OCSP_SINGLERESP_new","OPENSSL_sk_push","ossl_check_OCSP_SINGLERESP_sk_type","ossl_check_OCSP_SINGLERESP_type","OPENSSL_sk_pop_free","ossl_check_OCSP_SINGLERESP_sk_type","ossl_check_OCSP_SINGLERESP_freefunc_type","OPENSSL_sk_free","ossl_check_OCSP_SINGLERESP_sk_type","ERR_peek_last_error_all","ERR_error_string_n","SSL_free","SSL_CTX_free","ERR_clear_error"],"critical_calls":["OPENSSL_init_ssl","OPENSSL_init_ssl","OPENSSL_init_crypto","SSL_CTX_new","TLS_method","SSL_new","SSL_set_default_read_buffer_len","SSL_new_listener_from","SSL_free","EVP_PKEY_CTX_new_id","EVP_PKEY_new","EVP_PKEY_derive_set_peer_ex","EVP_PKEY_free","EVP_PKEY_CTX_free","d2i_TS_TST_INFO_fp","TS_TST_INFO_free","ossl_check_OCSP_SINGLERESP_compfunc_type","OPENSSL_sk_set_cmp_thunks","OPENSSL_sk_new","ossl_check_OCSP_SINGLERESP_compfunc_type","OCSP_SINGLERESP_new","OPENSSL_sk_push","ossl_check_OCSP_SINGLERESP_type","ossl_check_OCSP_SINGLERESP_sk_type","OPENSSL_sk_pop_free","ossl_check_OCSP_SINGLERESP_freefunc_type","ossl_check_OCSP_SINGLERESP_sk_type","ERR_peek_last_error_all","ERR_error_string_n","SSL_free","SSL_CTX_free","ERR_clear_error"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0;
    }

    // Initialize SSL library
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

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

    // 1. Create SSL context and SSL object
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = SSL_new(ssl_ctx);
    
    // 2. Use SSL_set_default_read_buffer_len
    SSL_set_default_read_buffer_len(ssl, 4096);

    // 3. Create listener SSL from existing SSL
    uint64_t flags = 0;
    SSL *listener_ssl = SSL_new_listener_from(ssl, flags);
    
    if (listener_ssl) {
        // Write some test data to output file
        fwrite("Listener created\n", 1, 17, out_file);
        SSL_free(listener_ssl);
    }

    // 4. Create EVP_PKEY context for key derivation
    EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_DH, NULL);
    if (pctx) {
        EVP_PKEY *peer_key = EVP_PKEY_new();
        
        // Try to use EVP_PKEY_derive_set_peer_ex
        if (peer_key) {
            // Note: In real usage, peer_key should be properly initialized
            int validate_peer = 0;
            EVP_PKEY_derive_set_peer_ex(pctx, peer_key, validate_peer);
            EVP_PKEY_free(peer_key);
        }
        EVP_PKEY_CTX_free(pctx);
    }

    // 5. Create and free TS_TST_INFO
    TS_TST_INFO *tst_info = NULL;
    // Try to read TS_TST_INFO from input file
    fseek(in_file, 0, SEEK_SET);
    tst_info = d2i_TS_TST_INFO_fp(in_file, NULL);
    if (tst_info) {
        TS_TST_INFO_free(tst_info);
        fwrite("TS_TST_INFO processed\n", 1, 22, out_file);
    }

    // 6. Use ossl_check_OCSP_SINGLERESP_compfunc_type
    sk_OCSP_SINGLERESP_compfunc comp_func = NULL;
    OPENSSL_sk_compfunc checked_compfunc = 
        ossl_check_OCSP_SINGLERESP_compfunc_type(comp_func);
    
    // Create a dummy OCSP response stack to use the compfunc
    STACK_OF(OCSP_SINGLERESP) *ocsp_stack = sk_OCSP_SINGLERESP_new(comp_func);
    if (ocsp_stack) {
        // Create a minimal OCSP_SINGLERESP
        OCSP_SINGLERESP *single_resp = OCSP_SINGLERESP_new();
        if (single_resp) {
            sk_OCSP_SINGLERESP_push(ocsp_stack, single_resp);
            sk_OCSP_SINGLERESP_pop_free(ocsp_stack, OCSP_SINGLERESP_free);
        } else {
            sk_OCSP_SINGLERESP_free(ocsp_stack);
        }
    }

    // 7. Use ERR_peek_last_error_all
    const char *file = NULL;
    int line = 0;
    const char *func = NULL;
    const char *data_str = NULL;
    int flags_err = 0;
    
    unsigned long error = ERR_peek_last_error_all(&file, &line, &func, &data_str, &flags_err);
    
    if (error) {
        char error_buf[256];
        ERR_error_string_n(error, error_buf, sizeof(error_buf));
        fwrite(error_buf, 1, strlen(error_buf), out_file);
        fwrite("\n", 1, 1, out_file);
    }

    // Read remaining data from input file
    uint8_t buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, out_file);
    }

    // Cleanup
    if (ssl) {
        SSL_free(ssl);
    }
    if (ssl_ctx) {
        SSL_CTX_free(ssl_ctx);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clear error queue
    ERR_clear_error();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}