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
//<ID> 447
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 55
//<Quality> {"density":6,"unique_branches":{"EVP_PBE_find_ex":[[241,9,241,29,0,0,4,0]],"SSL_CTX_callback_ctrl":[[3399,5,3399,35,0,0,4,0],[3405,5,3405,12,0,0,4,1]],"hexstr2buf_sep":[[222,60,222,62,0,0,4,0],[222,60,222,62,0,0,4,1],[225,13,225,22,0,0,4,1],[228,13,228,16,0,0,4,1],[234,13,234,20,0,0,4,1],[234,24,234,31,0,0,4,1],[239,13,239,22,0,0,4,0],[240,17,240,28,0,0,4,1],[248,9,248,23,0,0,4,0]],"PKCS12_pbe_crypt_ex":[[33,9,33,20,0,0,4,1],[39,9,40,57,0,0,4,0]],"EVP_PBE_CipherInit_ex":[[106,9,107,55,0,0,4,0],[110,13,110,28,0,0,4,1]],"OPENSSL_hexchar2int":[[170,13,170,14,0,0,4,1],[171,5,171,13,0,0,4,0],[171,5,171,13,0,0,4,1],[173,5,173,13,0,0,4,0],[173,5,173,13,0,0,4,1],[175,5,175,13,0,0,4,0],[175,5,175,13,0,0,4,1],[177,5,177,13,0,0,4,0],[177,5,177,13,0,0,4,1],[179,5,179,13,0,0,4,0],[179,5,179,13,0,0,4,1],[181,5,181,13,0,0,4,0],[181,5,181,13,0,0,4,1],[183,5,183,13,0,0,4,0],[183,5,183,13,0,0,4,1],[185,5,185,13,0,0,4,0],[185,5,185,13,0,0,4,1],[187,5,187,13,0,0,4,0],[187,5,187,13,0,0,4,1],[189,5,189,13,0,0,4,0],[189,5,189,13,0,0,4,1],[191,5,191,13,0,0,4,0],[191,5,191,13,0,0,4,1],[192,5,192,13,0,0,4,1],[194,5,194,13,0,0,4,0],[194,5,194,13,0,0,4,1],[195,5,195,13,0,0,4,1],[197,5,197,13,0,0,4,0],[197,5,197,13,0,0,4,1],[198,5,198,13,0,0,4,1],[200,5,200,13,0,0,4,0],[200,5,200,13,0,0,4,1],[201,5,201,13,0,0,4,1],[203,5,203,13,0,0,4,0],[203,5,203,13,0,0,4,1],[204,5,204,13,0,0,4,1],[206,5,206,13,0,0,4,0],[206,5,206,13,0,0,4,1],[207,5,207,13,0,0,4,1]]},"library_calls":["X509_REQ_new","X509_REQ_free","d2i_X509_fp","EVP_MD_CTX_new","X509_REQ_free","X509_free","X509_sign_ctx","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OPENSSL_sk_push","ossl_check_X509_sk_type","ossl_check_X509_type","sk_X509_cmpfunc_thunk","X509_ALGOR_new","PKCS12_pbe_crypt","CRYPTO_free","sk_OCSP_RESPONSE_new_null","OCSP_RESPONSE_new","sk_OCSP_RESPONSE_push","sk_OCSP_RESPONSE_delete_ptr","OCSP_RESPONSE_free","sk_OCSP_RESPONSE_free","OPENSSL_hexstr2buf_ex","OPENSSL_fork_child","SSL_CTX_new","TLS_method","SSL_CTX_set_msg_callback","d2i_X509_ACERT","X509_ACERT_get0_issuerUID","X509_ALGOR_free","EVP_MD_CTX_free","X509_free","X509_REQ_free","OPENSSL_sk_free","ossl_check_X509_sk_type","SSL_CTX_free","X509_ACERT_free"],"critical_calls":["X509_REQ_new","d2i_X509_fp","EVP_MD_CTX_new","X509_sign_ctx","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OPENSSL_sk_push","ossl_check_X509_type","ossl_check_X509_sk_type","sk_X509_cmpfunc_thunk","X509_ALGOR_new","PKCS12_pbe_crypt","CRYPTO_free","sk_OCSP_RESPONSE_new_null","OCSP_RESPONSE_new","sk_OCSP_RESPONSE_push","sk_OCSP_RESPONSE_delete_ptr","OCSP_RESPONSE_free","sk_OCSP_RESPONSE_free","OPENSSL_hexstr2buf_ex","OPENSSL_fork_child","SSL_CTX_new","TLS_method","SSL_CTX_set_msg_callback","d2i_X509_ACERT","X509_ACERT_get0_issuerUID","X509_ALGOR_free","EVP_MD_CTX_free","X509_free","X509_REQ_free","OPENSSL_sk_free","ossl_check_X509_sk_type","SSL_CTX_free","X509_ACERT_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create X509_REQ for signing
    X509_REQ *req = X509_REQ_new();
    if (!req) {
	return 0;
	}
	
    
    // 2. Create X509 certificate from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        X509_REQ_free(req);
        assert_file_closed(&in_file);
	return 0;
    }
    
    X509 *x509 = d2i_X509_fp(in_file, NULL);
    assert_file_closed(&in_file);;
    
    // 3. Setup signing context
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
    if (!md_ctx) {
        X509_REQ_free(req);
        if (x509) X509_free(x509);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Try to sign X509 certificate with context (even if x509 is NULL)
    if (x509) {
        X509_sign_ctx(x509, md_ctx);
    }
    
    // 5. Create stack of X509 certificates for comparison
    struct stack_st_X509 *sk = sk_X509_new_null();
    if (sk) {
        if (x509) {
            sk_X509_push(sk, x509);
        }
        // Add comparison function thunk
        sk_X509_cmpfunc_thunk([](const void *a, const void *b)->int {
            return 0;
        }, NULL, NULL);
    }
    
    // 6. Setup PKCS12 PBE encryption/decryption
    X509_ALGOR *algor = X509_ALGOR_new();
    if (algor && size > 0) {
        unsigned char *out_data = NULL;
        int out_len = 0;
        const char *pass = "fuzzpass";
        
        // Use first part of input as algorithm parameters
        if (size >= 16) {
            // Try to decrypt using PKCS12 PBE
            PKCS12_pbe_crypt(algor, pass, strlen(pass), 
                           data, size > 1024 ? 1024 : (int)size,
                           &out_data, &out_len, 0);
            if (out_data) OPENSSL_free(out_data);
        }
    }
    
    // 7. Create and manipulate OCSP response stack
    struct stack_st_OCSP_RESPONSE *ocsp_sk = sk_OCSP_RESPONSE_new_null();
    if (ocsp_sk) {
        OCSP_RESPONSE *ocsp_resp = OCSP_RESPONSE_new();
        if (ocsp_resp) {
            sk_OCSP_RESPONSE_push(ocsp_sk, ocsp_resp);
            // Delete pointer from stack
            sk_OCSP_RESPONSE_delete_ptr(ocsp_sk, ocsp_resp);
            OCSP_RESPONSE_free(ocsp_resp);
        }
        sk_OCSP_RESPONSE_free(ocsp_sk);
    }
    
    // 8. Hex string conversion from input data
    if (size > 0) {
        // Convert first 64 bytes to hex string format
        char hex_buf[129] = {0};
        for (size_t i = 0; i < size && i < 64; i++) {
            sprintf(hex_buf + i * 2, "%02x", data[i]);
        }
        
        unsigned char buf[64];
        size_t buflen = 0;
        OPENSSL_hexstr2buf_ex(buf, sizeof(buf), &buflen, hex_buf, ':');
    }
    
    // 9. Fork child process handling
    OPENSSL_fork_child();
    
    // 10. Setup SSL context with message callback
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    if (ssl_ctx) {
        SSL_CTX_set_msg_callback(ssl_ctx, [](int write_p, int version, 
                                            int content_type, const void *buf, 
                                            size_t len, SSL *ssl, void *arg) {
            // Empty callback for fuzzing
        });
    }
    
    // 11. Get issuer UID from X509_ACERT (if we can create one)
    X509_ACERT *acert = NULL;
    if (size > 0) {
        // Try to parse as X509_ACERT if we have enough data
        const unsigned char *p = data;
        acert = d2i_X509_ACERT(NULL, &p, size > 1024 ? 1024 : (int)size);
        if (acert) {
            const ASN1_BIT_STRING *issuer_uid = X509_ACERT_get0_issuerUID(acert);
            // Use the result (even if NULL)
        }
    }
    
    // 12. Cleanup all allocated resources
    if (algor) X509_ALGOR_free(algor);
    if (md_ctx) EVP_MD_CTX_free(md_ctx);
    if (x509) X509_free(x509);
    if (req) X509_REQ_free(req);
    if (sk) sk_X509_free(sk);
    if (ssl_ctx) SSL_CTX_free(ssl_ctx);
    if (acert) X509_ACERT_free(acert);
    
    assert_file_closed(&in_file);
	return 0;
}