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
//<ID> 141
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 6
//<Quality> {"density":6,"unique_branches":{"SSL_get0_domain":[[96,6,96,19,1,0,4,0],[1242,29,1242,48,2,0,4,1]],"d2i_PKCS8PrivateKey_fp":[[257,9,257,51,0,0,4,1]],"d2i_PKCS8PrivateKey_bio":[[178,9,178,19,0,0,4,0]],"SSL_stream_reset":[[96,6,96,19,1,0,4,0],[1242,29,1242,48,2,0,4,1]]},"library_calls":["OPENSSL_init_ssl","ERR_clear_error","NCONF_new","CONF_parse_list","d2i_PKCS8PrivateKey_fp","d2i_PKCS8_PRIV_KEY_INFO","SSL_CTX_new","TLS_method","SSL_CTX_sess_get_get_cb","SSL_new","SSL_get0_domain","SSL_stream_reset","X509_TRUST_set_default","ossl_check_SRTP_PROTECTION_PROFILE_sk_type","ossl_check_ASIdOrRange_copyfunc_type","EVP_PKEY_free","PKCS8_PRIV_KEY_INFO_free","SSL_free","SSL_CTX_free","NCONF_free"],"critical_calls":["OPENSSL_init_ssl","ERR_clear_error","NCONF_new","CONF_parse_list","d2i_PKCS8PrivateKey_fp","d2i_PKCS8_PRIV_KEY_INFO","SSL_CTX_new","TLS_method","SSL_CTX_sess_get_get_cb","SSL_new","SSL_get0_domain","SSL_stream_reset","X509_TRUST_set_default","ossl_check_SRTP_PROTECTION_PROFILE_sk_type","ossl_check_ASIdOrRange_copyfunc_type","EVP_PKEY_free","PKCS8_PRIV_KEY_INFO_free","SSL_free","SSL_CTX_free","NCONF_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Initialize OpenSSL
    OPENSSL_init_ssl(0, NULL);
    ERR_clear_error();

    // 1. Use CONF_parse_list to parse configuration from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create a configuration context
    CONF *conf = NCONF_new(NULL);
    if (conf) {
        // Create callback for list parsing
        auto list_cb = [](const char *elem, int len, void *arg) -> int {
            return 1; // Always succeed for fuzzing
        };
        
        // Use a subset of input as string for parsing
        size_t str_len = size > 1024 ? 1024 : size;
        char *input_str = (char *)malloc(str_len + 1);
        if (input_str) {
            memcpy(input_str, data, str_len);
            input_str[str_len] = '\0';
            
            CONF_parse_list(input_str, ',', 0, list_cb, NULL);
            free(input_str);
        }
    }

    // 2. Use d2i_PKCS8PrivateKey_fp to read private key from FILE*
    EVP_PKEY *pkey = NULL;
    rewind(in_file);
    d2i_PKCS8PrivateKey_fp(in_file, &pkey, NULL, NULL);
    
    // 3. Use d2i_PKCS8_PRIV_KEY_INFO to decode from memory
    PKCS8_PRIV_KEY_INFO *p8inf = NULL;
    const unsigned char *p = data;
    long len = (long)size;
    d2i_PKCS8_PRIV_KEY_INFO(&p8inf, &p, len);

    // 4. Create SSL_CTX and SSL objects for SSL APIs
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = NULL;
    
    if (ctx) {
        // 5. Get session get callback using SSL_CTX_sess_get_get_cb
        SSL_SESSION *(*sess_get_cb)(SSL_CTX *) = 
            (SSL_SESSION *(*)(SSL_CTX *))SSL_CTX_sess_get_get_cb(ctx);
        
        // 6. Create SSL object and use SSL_get0_domain
        ssl = SSL_new(ctx);
        if (ssl) {
            SSL_get0_domain(ssl);
            
            // 7. Use SSL_stream_reset if QUIC is available
            SSL_STREAM_RESET_ARGS reset_args = {0};
            SSL_stream_reset(ssl, &reset_args, sizeof(reset_args));
        }
        
        // 8. Set default trust using X509_TRUST_set_default
        auto trust_cb = [](int trust, const X509 *x, int flags) -> int {
            return X509_TRUST_TRUSTED;
        };
        X509_TRUST_set_default((int (*)(int, const X509 *, int))trust_cb);
    }

    // 9. Use ossl_check_SRTP_PROTECTION_PROFILE_sk_type
    struct stack_st_SRTP_PROTECTION_PROFILE *srtp_stack = NULL;
    if (srtp_stack) {
        ossl_check_SRTP_PROTECTION_PROFILE_sk_type(srtp_stack);
    }

    // 10. Use ossl_check_ASIdOrRange_copyfunc_type
    auto copyfunc = [](const ASIdOrRange *a) -> ASIdOrRange * {
        return (ASIdOrRange *)a;
    };
    ossl_check_ASIdOrRange_copyfunc_type((sk_ASIdOrRange_copyfunc)copyfunc);

    // Cleanup
    if (pkey) EVP_PKEY_free(pkey);
    if (p8inf) PKCS8_PRIV_KEY_INFO_free(p8inf);
    if (ssl) SSL_free(ssl);
    if (ctx) SSL_CTX_free(ctx);
    if (conf) NCONF_free(conf);
    if (in_file) assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}