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
//<ID> 142
//<Prompt> []
/*<Combination>: [
*/
//<score> 102, nr_unique_branch: 70
//<Quality> {"density":6,"unique_branches":{"write_string":[[205,13,205,36,0,0,4,1],[206,5,206,19,0,0,4,1],[207,5,207,18,0,0,4,1],[211,5,211,18,0,0,4,1],[212,5,212,20,0,0,4,0],[213,5,213,20,0,0,4,1],[214,5,214,21,0,0,4,1]],"echo_console":[[523,9,523,17,0,0,4,1]],"allocate_string_stack":[[86,9,86,28,0,0,4,0],[88,13,88,32,0,0,4,1]],"read_string":[[224,13,224,36,0,0,4,1],[225,5,225,21,0,0,4,1],[232,5,232,20,0,0,4,0],[238,5,238,20,0,0,4,1],[251,5,251,18,0,0,4,1],[252,5,252,18,0,0,4,1],[253,5,253,19,0,0,4,1]],"EVP_get_pw_prompt":[[36,9,36,33,0,0,4,0]],"UI_process":[[493,17,493,37,0,0,4,0],[494,13,494,46,0,0,4,0],[495,16,497,22,0,0,4,1],[519,17,519,37,0,0,4,0],[520,13,520,45,0,0,4,0],[524,13,524,20,0,0,4,1],[528,13,528,19,0,0,4,0],[532,13,532,20,0,0,4,1],[552,9,552,17,0,0,4,0]],"free_string":[[52,9,52,41,0,0,4,1]],"PEM_def_callback":[[42,9,42,17,0,0,4,1],[50,9,50,23,0,0,4,0],[60,15,60,21,0,0,4,1],[63,9,63,15,0,0,4,0]],"read_string_inner":[[293,9,293,14,0,0,4,0],[293,18,293,37,0,0,4,1],[338,9,338,18,0,0,4,0],[353,9,353,30,0,0,4,1],[355,9,355,17,0,0,4,0],[357,9,357,16,0,0,4,0],[357,20,357,25,0,0,4,0],[357,29,357,46,0,0,4,1],[360,9,360,16,0,0,4,0]],"popsig":[[630,17,630,30,0,0,4,0],[630,17,630,30,0,0,4,1],[632,13,632,25,0,0,4,0],[632,13,632,25,0,0,4,1],[636,13,636,25,0,0,4,0],[636,13,636,25,0,0,4,1]],"EVP_read_pw_string_min":[[59,9,59,25,0,0,4,1],[62,9,62,19,0,0,4,1],[64,9,66,16,0,0,4,1],[65,13,65,28,0,0,4,1],[67,13,67,19,0,0,4,1]],"pushsig":[[593,17,593,30,0,0,4,0],[593,17,593,30,0,0,4,1],[595,13,595,25,0,0,4,0],[595,13,595,25,0,0,4,1],[599,13,599,25,0,0,4,0],[599,13,599,25,0,0,4,1],[603,13,603,25,0,0,4,0],[603,13,603,25,0,0,4,1]],"general_allocate_string":[[128,9,128,18,0,0,4,0],[129,13,129,43,0,0,4,0],[135,17,135,25,0,0,4,1]],"noecho_console":[[491,9,491,17,0,0,4,1]],"d2i_PKCS8PrivateKey_bio":[[178,9,178,19,0,0,4,1],[180,9,180,19,0,0,4,1],[184,9,184,17,0,0,4,0]],"general_allocate_prompt":[[106,12,106,30,0,0,4,1],[108,16,108,60,0,0,4,0],[110,22,110,37,0,0,4,1]]},"library_calls":["OPENSSL_init_ssl","OPENSSL_init_crypto","CONF_parse_list","d2i_PKCS8PrivateKey_fp","d2i_PKCS8_PRIV_KEY_INFO","SSL_CTX_new","TLS_method","X509_TRUST_set_default","SSL_CTX_sess_get_get_cb","SSL_new","SSL_get0_domain","ossl_check_SRTP_PROTECTION_PROFILE_sk_type","ossl_check_ASIdOrRange_copyfunc_type","SSL_stream_reset","EVP_PKEY_free","PKCS8_PRIV_KEY_INFO_free","SSL_free","SSL_CTX_free"],"critical_calls":["OPENSSL_init_ssl","OPENSSL_init_crypto","CONF_parse_list","d2i_PKCS8PrivateKey_fp","d2i_PKCS8_PRIV_KEY_INFO","SSL_CTX_new","TLS_method","X509_TRUST_set_default","SSL_CTX_sess_get_get_cb","SSL_new","SSL_get0_domain","ossl_check_SRTP_PROTECTION_PROFILE_sk_type","ossl_check_ASIdOrRange_copyfunc_type","SSL_stream_reset","EVP_PKEY_free","PKCS8_PRIV_KEY_INFO_free","SSL_free","SSL_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL if needed
    OPENSSL_init_ssl(0, NULL);
    OPENSSL_init_crypto(0, NULL);
    
    // Variables to be freed at the end
    FILE *in_file = NULL;
    EVP_PKEY *pkey = NULL;
    PKCS8_PRIV_KEY_INFO *p8inf = NULL;
    SSL_CTX *ctx = NULL;
    SSL *ssl = NULL;
    OPENSSL_STACK *srtp_stack = NULL;
    
    // 1. Use CONF_parse_list to parse configuration from input data
    // We'll use a simple callback that does nothing
    auto list_cb = [](const char *elem, int len, void *arg) -> int {
        return 1;
    };
    
    // Create a null-terminated string from part of the input for CONF_parse_list
    size_t parse_len = size > 100 ? 100 : size;
    char *config_str = (char *)malloc(parse_len + 1);
    if (config_str) {
        memcpy(config_str, data, parse_len);
        config_str[parse_len] = '\0';
        CONF_parse_list(config_str, ',', 0, list_cb, NULL);
        free(config_str);
    }
    
    // 2. Read private key from input data using d2i_PKCS8PrivateKey_fp
    in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Try to read PKCS8 private key from the file
        pkey = d2i_PKCS8PrivateKey_fp(in_file, NULL, NULL, NULL);
        assert_file_closed(&in_file);;
        in_file = NULL;
    }
    
    // 3. Also try to parse PKCS8_PRIV_KEY_INFO directly from input
    const unsigned char *p = data;
    p8inf = d2i_PKCS8_PRIV_KEY_INFO(NULL, &p, (long)(size - (p - data)));
    
    // 4. Create SSL context and SSL object
    ctx = SSL_CTX_new(TLS_method());
    if (ctx) {
        // Set default trust setting
        X509_TRUST_set_default(NULL);
        
        // Get session get callback (may be NULL)
        SSL_CTX_sess_get_get_cb(ctx);
        
        // Create SSL object
        ssl = SSL_new(ctx);
        if (ssl) {
            // 5. Try to get domain from SSL (returns NULL for non-SNI)
            SSL_get0_domain(ssl);
            
            // 6. Create SRTP_PROTECTION_PROFILE stack and cast it
            struct stack_st_SRTP_PROTECTION_PROFILE *srtp_profiles = NULL;
            srtp_stack = ossl_check_SRTP_PROTECTION_PROFILE_sk_type(srtp_profiles);
            
            // 7. Setup copy function for ASIdOrRange (typecast only)
            sk_ASIdOrRange_copyfunc copyfunc = NULL;
            ossl_check_ASIdOrRange_copyfunc_type(copyfunc);
            
            // 8. Attempt SSL stream reset if we have SSL object
            // This would normally be used for QUIC connections
            SSL_STREAM_RESET_ARGS reset_args = {0};
            reset_args.quic_error_code = 0;
            SSL_stream_reset(ssl, &reset_args, sizeof(reset_args));
        }
    }
    
    // Cleanup
    if (pkey) EVP_PKEY_free(pkey);
    if (p8inf) PKCS8_PRIV_KEY_INFO_free(p8inf);
    if (ssl) SSL_free(ssl);
    if (ctx) SSL_CTX_free(ctx);
    
    assert_file_closed(&in_file);
	return 0;
}