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
//<ID> 402
//<Prompt> []
/*<Combination>: [
*/
//<score> 105, nr_unique_branch: 48
//<Quality> {"density":7,"unique_branches":{"BIO_lookup_ex":[[686,5,686,17,0,0,4,0],[688,5,688,18,0,0,4,1],[691,5,691,17,0,0,4,1],[694,5,694,19,0,0,4,1],[697,5,697,12,0,0,4,1],[703,9,703,26,0,0,4,1],[712,9,712,29,0,0,4,1],[727,13,727,25,0,0,4,1],[732,13,732,45,0,0,4,1],[743,9,743,24,0,0,4,1],[750,9,750,24,0,0,4,1],[755,9,755,15,0,0,4,0],[755,9,755,15,0,0,4,1],[758,9,758,16,0,0,4,0],[758,9,758,16,0,0,4,1],[760,17,760,47,0,0,4,1],[768,30,768,37,0,0,4,1]],"d2i_X509_REQ_bio":[[362,9,362,20,0,0,4,0],[362,24,362,36,0,0,4,1]],"BIO_ADDR_rawport":[[207,9,207,36,0,0,4,0]],"MDC2":[[25,9,25,19,0,0,4,1],[27,9,27,23,0,0,4,1]],"mdc2_body":[[86,17,86,24,0,0,4,0],[86,17,86,24,0,0,4,1]],"MDC2_Update":[[50,9,50,15,0,0,4,1],[67,9,67,14,0,0,4,0],[67,9,67,14,0,0,4,1],[70,9,70,14,0,0,4,0],[70,9,70,14,0,0,4,1]],"BIO_get_port":[[79,9,79,20,0,0,4,1],[84,9,84,29,0,0,4,1],[87,9,87,77,0,0,4,0],[87,9,87,77,0,0,4,1],[88,13,88,48,0,0,4,1]],"BIO_ADDRINFO_family":[[389,9,389,20,0,0,4,0]],"MDC2_Final":[[123,9,123,16,0,0,4,0],[123,9,123,16,0,0,4,1],[123,20,123,28,0,0,4,1],[124,13,124,19,0,0,4,1]],"DES_set_odd_parity":[[63,17,63,31,0,0,4,0],[63,17,63,31,0,0,4,1]],"BIO_ADDRINFO_free":[[455,9,455,20,0,0,4,1],[460,15,460,41,1,0,4,0]],"SSL_get_conn_close_info":[[96,6,96,19,1,0,4,0],[1242,29,1242,48,2,0,4,1]],"BIO_ADDRINFO_address":[[448,9,448,20,0,0,4,0]],"ERR_count_to_mark":[[71,12,71,35,0,0,4,0],[73,15,73,22,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","ERR_clear_error","BIO_new_fp","d2i_X509_REQ_bio","ASN1_dup","X509_REQ_free","X509_REQ_free","MDC2","BIO_get_port","SSL_CTX_new","TLS_method","SSL_CTX_flush_sessions","SSL_new","SSL_get_conn_close_info","SSL_free","SSL_CTX_free","ossl_check_X509_POLICY_NODE_copyfunc_type","ERR_count_to_mark","BIO_free","ERR_clear_error"],"critical_calls":["OPENSSL_init_ssl","ERR_clear_error","BIO_new_fp","d2i_X509_REQ_bio","ASN1_dup","X509_REQ_free","X509_REQ_free","MDC2","BIO_get_port","SSL_CTX_new","TLS_method","SSL_CTX_flush_sessions","SSL_new","SSL_get_conn_close_info","SSL_free","SSL_CTX_free","ossl_check_X509_POLICY_NODE_copyfunc_type","ERR_count_to_mark","BIO_free","ERR_clear_error"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize OpenSSL
    OPENSSL_init_ssl(0, NULL);
    ERR_clear_error();

    // 1. Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Create BIO from FILE* for X509_REQ parsing
    BIO *bio = BIO_new_fp(in_file, BIO_NOCLOSE);
    if (!bio) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Parse X509_REQ from input data using d2i_X509_REQ_bio
    X509_REQ *req = NULL;
    X509_REQ *parsed_req = d2i_X509_REQ_bio(bio, &req);
    
    // 4. Use ASN1_dup to duplicate the parsed structure (if successful)
    if (parsed_req) {
        // Create i2d and d2i function pointers for X509_REQ
        i2d_of_void *i2d_func = (i2d_of_void*)i2d_X509_REQ;
        d2i_of_void *d2i_func = (d2i_of_void*)d2i_X509_REQ;
        
        // Duplicate the X509_REQ using ASN1_dup
        X509_REQ *dup_req = (X509_REQ*)ASN1_dup(i2d_func, d2i_func, (void*)parsed_req);
        
        if (dup_req) {
            X509_REQ_free(dup_req);
        }
        X509_REQ_free(parsed_req);
    }

    // 5. Compute MDC2 hash of input data
    unsigned char mdc2_hash[MDC2_DIGEST_LENGTH];
    unsigned char *hash_result = MDC2(data, size, mdc2_hash);
    // hash_result should point to mdc2_hash

    // 6. Extract a string from input for BIO_get_port (use first 100 bytes or less)
    size_t str_len = (size > 100) ? 100 : size;
    char *port_str = (char*)malloc(str_len + 1);
    if (port_str) {
        memcpy(port_str, data, str_len);
        port_str[str_len] = '\0';
        
        unsigned short port;
        int port_result = BIO_get_port(port_str, &port);
        // Result is ignored intentionally for fuzzing
        
        free(port_str);
    }

    // 7. Create SSL_CTX and use SSL_CTX_flush_sessions
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    if (ctx) {
        SSL_CTX_flush_sessions(ctx, 0);  // Flush all expired sessions
        
        // 8. Create SSL object and use SSL_get_conn_close_info
        SSL *ssl = SSL_new(ctx);
        if (ssl) {
            SSL_CONN_CLOSE_INFO close_info;
            size_t info_len = sizeof(close_info);
            int close_info_result = SSL_get_conn_close_info(ssl, &close_info, info_len);
            // Result is ignored intentionally for fuzzing
            
            SSL_free(ssl);
        }
        SSL_CTX_free(ctx);
    }

    // 9. Use ossl_check_X509_POLICY_NODE_copyfunc_type with a dummy function
    sk_X509_POLICY_NODE_copyfunc dummy_copy = NULL;
    OPENSSL_sk_copyfunc checked_copy = ossl_check_X509_POLICY_NODE_copyfunc_type(dummy_copy);
    // The result is not used, just ensuring the function is called

    // 10. Check error mark count
    int error_count = ERR_count_to_mark();
    // Result is ignored intentionally for fuzzing

    // Cleanup
    BIO_free(bio);
    // Note: in_file is already freed by BIO_free with BIO_NOCLOSE
    // But we need to close it since we used BIO_NOCLOSE
    assert_file_closed(&in_file);;

    // Clear any remaining errors
    ERR_clear_error();

    assert_file_closed(&in_file);
	return 0;
}