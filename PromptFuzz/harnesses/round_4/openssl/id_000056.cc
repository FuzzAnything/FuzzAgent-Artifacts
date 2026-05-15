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
//<ID> 760
//<Prompt> []
/*<Combination>: [
*/
//<score> 204, nr_unique_branch: 59
//<Quality> {"density":17,"unique_branches":{"ASN1_TIME_adj":[[323,23,323,33,0,0,4,0],[324,13,324,60,0,0,4,1]],"type_str":[[323,9,323,41,0,0,4,0],[323,45,323,62,0,0,4,1],[331,9,331,36,0,0,4,1]],"md_ctrl":[[144,5,144,24,0,0,4,1],[152,5,152,22,0,0,4,1],[159,5,159,26,0,0,4,1],[164,5,164,26,0,0,4,1],[170,5,170,22,0,0,4,0],[175,13,175,24,0,0,4,1],[175,28,175,40,0,0,4,0],[180,5,180,24,0,0,4,1],[181,5,181,32,0,0,4,1],[186,5,186,22,0,0,4,1],[192,5,192,22,0,0,4,1],[199,5,199,12,0,0,4,1]],"md_read":[[79,9,79,20,0,0,4,1],[85,9,85,22,0,0,4,1],[85,26,85,40,0,0,4,0]],"X509_time_adj_ex":[[2485,9,2485,14,0,0,4,1],[2490,9,2490,18,0,0,4,0],[2490,22,2490,64,0,0,4,1]],"posix_time_from_utc":[[99,9,99,41,0,0,4,1],[99,45,99,84,0,0,4,1],[101,9,101,19,0,0,4,1],[105,12,105,21,0,0,4,0],[107,27,107,36,0,0,4,0]],"OPENSSL_gmtime_adj":[[226,9,226,46,0,0,4,1],[235,9,235,23,0,0,4,0],[235,27,235,62,0,0,4,1],[237,9,237,23,0,0,4,1],[241,9,241,45,0,0,4,1]],"is_valid_date":[[38,9,38,16,0,0,4,1],[38,20,38,28,0,0,4,1],[38,32,38,43,0,0,4,1],[41,5,41,11,0,0,4,1],[42,5,42,11,0,0,4,1],[43,5,43,11,0,0,4,1],[44,5,44,11,0,0,4,1],[45,5,45,11,0,0,4,1],[46,5,46,12,0,0,4,1],[47,5,47,12,0,0,4,1],[50,5,50,11,0,0,4,0],[51,5,51,11,0,0,4,1],[52,5,52,11,0,0,4,1],[53,5,53,12,0,0,4,1],[56,5,56,11,0,0,4,1],[62,5,62,12,0,0,4,1]],"is_valid_time":[[74,12,74,22,0,0,4,0],[74,26,74,38,0,0,4,0],[74,42,74,54,0,0,4,0],[74,58,74,69,0,0,4,0],[74,73,74,86,0,0,4,0],[74,90,74,103,0,0,4,0]],"ASN1_STRING_set_by_NID":[[93,13,93,43,0,0,4,1]],"ASN1_mbstring_ncopy":[[56,9,56,18,0,0,4,0],[59,13,59,29,0,0,4,1],[135,14,135,43,0,0,4,0]]},"library_calls":["X509_NAME_new","X509_NAME_add_entry_by_txt","X509_NAME_get0_der","X509_NAME_free","IDEA_set_encrypt_key","IDEA_encrypt","NCONF_new","TS_RESP_CTX_new","BIO_new_fp","NCONF_load_bio","BIO_free","TS_CONF_set_accuracy","NCONF_free","TS_RESP_CTX_free","TS_VERIFY_CTX_new","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","X509_new","ASN1_INTEGER_set","X509_get_serialNumber","X509_gmtime_adj","X509_getm_notBefore","X509_gmtime_adj","X509_getm_notAfter","OPENSSL_sk_push","ossl_check_X509_sk_type","ossl_check_X509_type","TS_VERIFY_CTX_set0_certs","OPENSSL_sk_free","ossl_check_X509_sk_type","TS_VERIFY_CTX_free","EVP_CIPHER_CTX_new","EVP_CIPHER_CTX_clear_flags","EVP_CIPHER_CTX_free","SSL_alert_type_string_long","X509_STORE_CTX_new","X509_STORE_CTX_set_get_crl","X509_STORE_CTX_free","BIO_f_md","BIO_new","BIO_write","BIO_read","BIO_free"],"critical_calls":["X509_NAME_new","X509_NAME_add_entry_by_txt","X509_NAME_get0_der","X509_NAME_free","IDEA_set_encrypt_key","IDEA_encrypt","NCONF_new","TS_RESP_CTX_new","BIO_new_fp","NCONF_load_bio","BIO_free","TS_CONF_set_accuracy","NCONF_free","TS_RESP_CTX_free","TS_VERIFY_CTX_new","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","X509_new","ASN1_INTEGER_set","X509_get_serialNumber","X509_gmtime_adj","X509_getm_notBefore","X509_gmtime_adj","X509_getm_notAfter","OPENSSL_sk_push","ossl_check_X509_type","ossl_check_X509_sk_type","TS_VERIFY_CTX_set0_certs","TS_VERIFY_CTX_free","EVP_CIPHER_CTX_new","EVP_CIPHER_CTX_clear_flags","EVP_CIPHER_CTX_free","SSL_alert_type_string_long","X509_STORE_CTX_new","X509_STORE_CTX_set_get_crl","X509_STORE_CTX_free","BIO_f_md","BIO_new","BIO_write","BIO_read","BIO_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 256) {
	return 0;
	}
	 // Need minimum data for all operations

    // Split input data for different operations
    const uint8_t *der_data = data;
    size_t der_len = size / 8;
    
    const uint8_t *idea_data = data + der_len;
    size_t idea_len = size / 8;
    
    const uint8_t *conf_data = data + 2*der_len;
    size_t conf_len = size / 8;
    
    const uint8_t *cert_data = data + 3*der_len;
    size_t cert_len = size / 8;
    
    const uint8_t *alert_data = data + 5*der_len;
    size_t alert_len = size / 8;
    
    const uint8_t *bio_data = data + 7*der_len;
    size_t bio_len = size - 7*der_len;

    // 1. X509_NAME_get0_der usage
    X509_NAME *name = X509_NAME_new();
    if (name) {
        // Add minimal entry to make name valid
        X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char*)"US", -1, -1, 0);
        
        const unsigned char *pder;
        size_t pderlen;
        X509_NAME_get0_der(name, &pder, &pderlen);
        
        X509_NAME_free(name);
    }

    // 2. IDEA_encrypt usage
    IDEA_KEY_SCHEDULE ks;
    unsigned long in[2];
    
    if (idea_len >= 16) {
        // Use first 16 bytes for key schedule
        IDEA_set_encrypt_key(idea_data, &ks);
        // Use next 8 bytes for input
        if (idea_len >= 24) {
            memcpy(in, idea_data + 16, 8);
            IDEA_encrypt(in, &ks);
        }
    }

    // 3. TS_CONF_set_accuracy usage
    CONF *conf = NCONF_new(NULL);
    TS_RESP_CTX *ts_ctx = TS_RESP_CTX_new();
    
    if (conf && ts_ctx) {
        // Write conf data to temp file
        FILE *conf_file = fmemopen((void*)conf_data, conf_len, "rb");
        if (conf_file) {
            // Load configuration from memory stream
            BIO *conf_bio = BIO_new_fp(conf_file, BIO_CLOSE);
            if (conf_bio) {
                NCONF_load_bio(conf, conf_bio, NULL);
                BIO_free(conf_bio);
            }
            
            TS_CONF_set_accuracy(conf, "default", ts_ctx);
            assert_file_closed(&conf_file);;
        }
        
        NCONF_free(conf);
        TS_RESP_CTX_free(ts_ctx);
    }

    // 4. TS_VERIFY_CTX_set0_certs usage
    TS_VERIFY_CTX *verify_ctx = TS_VERIFY_CTX_new();
    if (verify_ctx) {
        struct stack_st_X509 *certs = sk_X509_new_null();
        if (certs) {
            // Create minimal certificate
            X509 *cert = X509_new();
            if (cert) {
                // Set some basic fields
                ASN1_INTEGER_set(X509_get_serialNumber(cert), 1);
                X509_gmtime_adj(X509_getm_notBefore(cert), 0);
                X509_gmtime_adj(X509_getm_notAfter(cert), 31536000L);
                
                sk_X509_push(certs, cert);
                TS_VERIFY_CTX_set0_certs(verify_ctx, certs);
                
                // certs is now owned by verify_ctx
            } else {
                sk_X509_free(certs);
            }
        }
        TS_VERIFY_CTX_free(verify_ctx);
    }

    // 5. EVP_CIPHER_CTX_clear_flags usage
    EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new();
    if (cipher_ctx) {
        EVP_CIPHER_CTX_clear_flags(cipher_ctx, EVP_CIPH_FLAG_DEFAULT_ASN1);
        EVP_CIPHER_CTX_free(cipher_ctx);
    }

    // 6. SSL_alert_type_string_long usage
    if (alert_len > 0) {
        int alert_value = alert_data[0] % 256;
        const char *alert_str = SSL_alert_type_string_long(alert_value);
        // Use alert_str to avoid optimization
        if (alert_str) {
            volatile char c = alert_str[0];
            (void)c;
        }
    }

    // 7. X509_STORE_CTX_set_get_crl usage
    X509_STORE_CTX *store_ctx = X509_STORE_CTX_new();
    if (store_ctx) {
        X509_STORE_CTX_set_get_crl(store_ctx, 
            [](X509_STORE_CTX *ctx, X509_CRL **crl, X509 *x) -> int {
                *crl = NULL;
                return 0;
            });
        X509_STORE_CTX_free(store_ctx);
    }

    // 8. BIO_f_md usage
    const BIO_METHOD *md_method = BIO_f_md();
    if (md_method) {
        BIO *bio = BIO_new(md_method);
        if (bio) {
            // Write data to bio
            BIO_write(bio, bio_data, bio_len);
            
            // Read it back
            char buf[256];
            BIO_read(bio, buf, sizeof(buf));
            
            BIO_free(bio);
        }
    }

    return 0;
}