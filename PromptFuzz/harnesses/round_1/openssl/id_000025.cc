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
//<ID> 289
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 10
//<Quality> {"density":12,"unique_branches":{"BIO_fd_non_fatal_error":[[251,5,251,21,0,0,4,0],[256,5,256,18,0,0,4,0],[260,5,260,15,0,0,4,0],[270,5,270,16,0,0,4,0],[274,5,274,21,0,0,4,0],[278,5,278,18,0,0,4,0],[281,5,281,12,0,0,4,1]],"BN_nist_mod_224":[[506,30,506,74,0,0,4,0],[617,16,617,25,0,0,4,0],[627,18,627,23,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","OPENSSL_init_ssl","ASN1_TIME_new","ASN1_TIME_set_string_X509","ASN1_TIME_free","BN_CTX_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_nist_mod_224","BN_free","BN_free","BN_free","BN_CTX_free","SSL_CTX_new","TLS_method","SSL_new","SSL_get_rpoll_descriptor","BIO_fd_non_fatal_error","SSL_free","SSL_CTX_free","X509_new","X509_check_ip","X509_free","IPAddressChoice_it"],"critical_calls":["OPENSSL_init_ssl","OPENSSL_init_ssl","ASN1_TIME_new","ASN1_TIME_set_string_X509","ASN1_TIME_free","BN_CTX_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_nist_mod_224","BN_free","BN_free","BN_free","BN_CTX_free","SSL_CTX_new","TLS_method","SSL_new","SSL_get_rpoll_descriptor","BIO_fd_non_fatal_error","SSL_free","SSL_CTX_free","X509_new","X509_check_ip","X509_free","IPAddressChoice_it"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 32) {
	return 0;
	}
	  // Need minimum data

    // Initialize SSL library for SSL_get_rpoll_descriptor
    SSL_library_init();
    SSL_load_error_strings();

    // Part 1: Use ASN1_TIME_set_string_X509 with a string from input
    ASN1_TIME *time = ASN1_TIME_new();
    if (time) {
        // Extract null-terminated string from first part of input
        size_t str_len = size > 100 ? 100 : size;
        char *time_str = (char*)malloc(str_len + 1);
        if (time_str) {
            memcpy(time_str, data, str_len);
            time_str[str_len] = 0;
            ASN1_TIME_set_string_X509(time, time_str);
            free(time_str);
        }
        ASN1_TIME_free(time);
    }

    // Part 2: Use BN_nist_mod_224 with BIGNUMs created from input
    BN_CTX *ctx = BN_CTX_new();
    if (ctx) {
        BIGNUM *r = BN_new();
        BIGNUM *a = BN_new();
        BIGNUM *p = BN_new();
        
        if (r && a && p) {
            // Create BIGNUMs from input data
            size_t bn_data_len = size > 64 ? 64 : size;
            BN_bin2bn(data, bn_data_len, a);
            
            // Use second half of data for modulus (if available)
            size_t offset = size > 128 ? 64 : 0;
            size_t p_len = (size - offset) > 32 ? 32 : (size - offset);
            if (p_len > 0) {
                BN_bin2bn(data + offset, p_len, p);
                BN_nist_mod_224(r, a, p, ctx);
            }
        }
        
        BN_free(r);
        BN_free(a);
        BN_free(p);
        BN_CTX_free(ctx);
    }

    // Part 3: Create SSL connection and use SSL_get_rpoll_descriptor
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    if (ssl_ctx) {
        SSL *ssl = SSL_new(ssl_ctx);
        if (ssl) {
            BIO_POLL_DESCRIPTOR desc;
            SSL_get_rpoll_descriptor(ssl, &desc);
            
            // Use BIO_fd_non_fatal_error with possible error codes from input
            if (size > 0) {
                int error_code = data[size-1];
                BIO_fd_non_fatal_error(error_code);
            }
            
            SSL_free(ssl);
        }
        SSL_CTX_free(ssl_ctx);
    }

    // Part 4: Create a mock X509 certificate and use X509_check_ip
    X509 *x509 = X509_new();
    if (x509) {
        // Extract IP address from input (using first 4 bytes for IPv4)
        unsigned char ip[4] = {0};
        size_t ip_len = size > 4 ? 4 : size;
        memcpy(ip, data, ip_len);
        
        X509_check_ip(x509, ip, ip_len, 0);
        X509_free(x509);
    }

    // Part 5: Use IPAddressChoice_it()
    const ASN1_ITEM *item = IPAddressChoice_it();
    (void)item; // Mark as used

    // Cleanup
    ERR_free_strings();
    
    return 0;
}