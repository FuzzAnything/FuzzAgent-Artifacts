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
//<ID> 763
//<Prompt> []
/*<Combination>: [
*/
//<score> 152, nr_unique_branch: 15
//<Quality> {"density":19,"unique_branches":{"EVP_CIPHER_CTX_set_flags":[[929,9,929,67,0,0,4,1]],"OPENSSL_sk_pop":[[443,9,443,19,0,0,4,1],[443,23,443,35,0,0,4,0],[443,23,443,35,0,0,4,1]],"conf_value_cmp":[[113,28,113,43,0,0,4,1],[117,12,117,29,0,0,4,1]],"NCONF_load_fp":[[266,9,266,53,0,0,4,1]],"x509_name_ex_d2i":[[163,9,163,28,0,0,4,0]],"def_load_bio":[[299,17,299,53,0,0,4,1],[505,21,505,33,0,0,4,0],[507,25,507,38,0,0,4,0],[508,29,508,64,0,0,4,1],[514,25,514,37,0,0,4,1],[577,12,577,33,0,0,4,0]],"str_copy":[[761,17,761,48,0,0,4,0]]},"library_calls":["d2i_X509_NAME","X509_NAME_get0_der","X509_NAME_free","IDEA_set_encrypt_key","IDEA_encrypt","NCONF_new","NCONF_load_fp","TS_RESP_CTX_new","TS_CONF_set_accuracy","TS_RESP_CTX_free","NCONF_free","TS_VERIFY_CTX_new","d2i_X509","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OPENSSL_sk_push","ossl_check_X509_sk_type","ossl_check_X509_type","TS_VERIFY_CTX_set0_certs","X509_free","TS_VERIFY_CTX_free","EVP_CIPHER_CTX_new","EVP_CIPHER_CTX_set_flags","EVP_CIPHER_CTX_clear_flags","EVP_CIPHER_CTX_free","SSL_alert_type_string_long","X509_STORE_CTX_new","X509_STORE_CTX_set_get_crl","X509_STORE_CTX_free","BIO_f_md","BIO_new","BIO_write","BIO_free"],"critical_calls":["d2i_X509_NAME","X509_NAME_get0_der","X509_NAME_free","IDEA_set_encrypt_key","IDEA_encrypt","NCONF_new","NCONF_load_fp","TS_RESP_CTX_new","TS_CONF_set_accuracy","TS_RESP_CTX_free","NCONF_free","TS_VERIFY_CTX_new","d2i_X509","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OPENSSL_sk_push","ossl_check_X509_type","ossl_check_X509_sk_type","TS_VERIFY_CTX_set0_certs","TS_VERIFY_CTX_free","EVP_CIPHER_CTX_new","EVP_CIPHER_CTX_set_flags","EVP_CIPHER_CTX_clear_flags","EVP_CIPHER_CTX_free","SSL_alert_type_string_long","X509_STORE_CTX_new","X509_STORE_CTX_set_get_crl","X509_STORE_CTX_free","BIO_f_md","BIO_new","BIO_write","BIO_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0; // Not enough data for meaningful fuzzing
    }

    // 1. X509_NAME_get0_der - Parse X509_NAME from input data
    const unsigned char *der_ptr = data;
    X509_NAME *name = d2i_X509_NAME(NULL, &der_ptr, size);
    if (name) {
        const unsigned char *der;
        size_t der_len;
        X509_NAME_get0_der(name, &der, &der_len);
        X509_NAME_free(name);
    }

    // 2. IDEA_encrypt - Use IDEA encryption on part of input data
    if (size >= 16 + 8) { // 16 bytes key + 8 bytes data
        IDEA_KEY_SCHEDULE ks;
        unsigned long in[2];
        
        // Use first 16 bytes as key
        IDEA_set_encrypt_key(data, &ks);
        
        // Use next 8 bytes as input (64-bit block)
        memcpy(in, data + 16, 8);
        IDEA_encrypt(in, &ks);
    }

    // 3. TS_CONF_set_accuracy - Configure timestamp accuracy
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        CONF *conf = NCONF_new(NULL);
        if (conf) {
            NCONF_load_fp(conf, in_file, NULL);
            TS_RESP_CTX *ts_ctx = TS_RESP_CTX_new();
            if (ts_ctx) {
                // Use default section name
                TS_CONF_set_accuracy(conf, "default", ts_ctx);
                TS_RESP_CTX_free(ts_ctx);
            }
            NCONF_free(conf);
        }
        assert_file_closed(&in_file);;
    }

    // 4. TS_VERIFY_CTX_set0_certs - Set certificates for timestamp verification
    TS_VERIFY_CTX *verify_ctx = TS_VERIFY_CTX_new();
    if (verify_ctx) {
        // Try to parse X509 certificates from input data
        const unsigned char *cert_ptr = data;
        X509 *cert = d2i_X509(NULL, &cert_ptr, size);
        if (cert) {
            STACK_OF(X509) *certs = sk_X509_new_null();
            if (certs) {
                sk_X509_push(certs, cert);
                TS_VERIFY_CTX_set0_certs(verify_ctx, certs);
                // certs is now owned by verify_ctx, don't free separately
            } else {
                X509_free(cert);
            }
        }
        TS_VERIFY_CTX_free(verify_ctx);
    }

    // 5. EVP_CIPHER_CTX_clear_flags - Clear cipher context flags
    EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new();
    if (cipher_ctx) {
        // Set some flags first
        EVP_CIPHER_CTX_set_flags(cipher_ctx, EVP_CIPH_NO_PADDING);
        // Clear the flags
        EVP_CIPHER_CTX_clear_flags(cipher_ctx, EVP_CIPH_NO_PADDING);
        EVP_CIPHER_CTX_free(cipher_ctx);
    }

    // 6. SSL_alert_type_string_long - Convert alert value to string
    if (size > 0) {
        int alert_value = data[0] % 256; // Use first byte as alert value
        const char *alert_str = SSL_alert_type_string_long(alert_value);
        // alert_str can be used or ignored
    }

    // 7. X509_STORE_CTX_set_get_crl - Set CRL retrieval function
    X509_STORE_CTX *store_ctx = X509_STORE_CTX_new();
    if (store_ctx) {
        // Create a dummy get_crl function
        X509_STORE_CTX_get_crl_fn get_crl = [](X509_STORE_CTX *ctx, X509_CRL **crl, X509 *x) {
            return 0; // Return 0 for simplicity
        };
        X509_STORE_CTX_set_get_crl(store_ctx, get_crl);
        X509_STORE_CTX_free(store_ctx);
    }

    // 8. BIO_f_md - Create message digest BIO
    const BIO_METHOD *md_method = BIO_f_md();
    if (md_method) {
        BIO *bio = BIO_new(md_method);
        if (bio) {
            // Write some data to the BIO
            if (size > 0) {
                BIO_write(bio, data, size > 100 ? 100 : size);
            }
            BIO_free(bio);
        }
    }

    assert_file_closed(&in_file);
	return 0;
}