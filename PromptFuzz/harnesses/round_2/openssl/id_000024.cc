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
//<ID> 515
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 27
//<Quality> {"density":9,"unique_branches":{"do_x509_check":[[883,9,883,32,0,0,4,1],[887,16,887,37,0,0,4,1],[902,9,902,20,0,0,4,1],[906,9,906,13,0,0,4,1],[986,9,986,26,0,0,4,0]],"get_ipv4_dot":[[1241,9,1241,21,0,0,4,1]],"ossl_a2i_ipadd":[[1195,13,1195,41,0,0,4,1]],"X509V3_get_d2i":[[206,9,206,11,0,0,4,0],[207,13,207,16,0,0,4,1],[209,13,209,17,0,0,4,1]],"ipv4_from_asc":[[1250,9,1250,41,0,0,4,1],[1250,45,1250,63,0,0,4,1],[1251,12,1251,44,0,0,4,1],[1251,48,1251,66,0,0,4,1],[1252,12,1252,44,0,0,4,1],[1252,48,1252,66,0,0,4,1],[1253,12,1253,44,0,0,4,1],[1253,48,1253,59,0,0,4,1]],"X509_check_ip_asc":[[1076,9,1076,22,0,0,4,1],[1079,9,1079,19,0,0,4,1]],"get_ipv4_component":[[1213,13,1213,33,0,0,4,1],[1217,13,1217,22,0,0,4,1],[1222,13,1222,27,0,0,4,0],[1222,13,1222,27,0,0,4,1],[1222,31,1222,46,0,0,4,0],[1222,31,1222,46,0,0,4,1],[1226,13,1226,21,0,0,4,1]]},"library_calls":["OPENSSL_init_crypto","UI_new","UI_add_input_string","UI_free","BN_get_rfc3526_prime_4096","BN_bin2bn","BN_cmp","BN_free","BN_free","X509_new","X509_check_ip_asc","X509_free","ossl_check_OPENSSL_CSTRING_lh_plain_type","EC_GROUP_new_by_curve_name","EC_GROUP_get_field_type","EC_GROUP_free","BIO_new_fp","X509_ALGOR_new","ASN1_STRING_new","CRYPTO_malloc","ASN1_STRING_set","CRYPTO_free","BIO_new_fp","X509_signature_print","BIO_free","X509_ALGOR_free","ASN1_STRING_free","BIO_free"],"critical_calls":["OPENSSL_init_crypto","UI_new","UI_add_input_string","UI_free","BN_get_rfc3526_prime_4096","BN_bin2bn","BN_cmp","BN_free","BN_free","X509_new","X509_check_ip_asc","X509_free","ossl_check_OPENSSL_CSTRING_lh_plain_type","EC_GROUP_new_by_curve_name","EC_GROUP_get_field_type","EC_GROUP_free","BIO_new_fp","X509_ALGOR_new","ASN1_STRING_new","CRYPTO_malloc","ASN1_STRING_set","CRYPTO_free","BIO_new_fp","X509_signature_print","BIO_free","X509_ALGOR_free","ASN1_STRING_free","BIO_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL if needed
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG, nullptr);
    
    // 1. Use UI_add_input_string
    UI *ui = UI_new();
    if (ui) {
        char result_buf[256] = {0};
        size_t prompt_len = size > 100 ? 100 : size;
        char prompt[101] = {0};
        memcpy(prompt, data, prompt_len);
        
        // Use part of input data for prompt and result buffer initialization
        int flags = 0;
        UI_add_input_string(ui, prompt, flags, result_buf, 1, 255);
        
        // Copy some input data to result buffer
        size_t copy_size = size > 255 ? 255 : size;
        memcpy(result_buf, data, copy_size);
        UI_free(ui);
    }
    
    // 2. Use BN_get_rfc3526_prime_4096
    BIGNUM *prime = BN_get_rfc3526_prime_4096(nullptr);
    if (prime) {
        // Use input data to create a BIGNUM and compare
        BIGNUM *input_bn = BN_bin2bn(data, size > 32 ? 32 : size, nullptr);
        if (input_bn) {
            // Compare with prime
            BN_cmp(input_bn, prime);
            BN_free(input_bn);
        }
        BN_free(prime);
    }
    
    // 3. Use X509_check_ip_asc with a dummy X509 certificate
    X509 *x509 = X509_new();
    if (x509) {
        // Create a simple IP string from input data
        char ip_str[64] = {0};
        if (size >= 4) {
            snprintf(ip_str, sizeof(ip_str), "%u.%u.%u.%u", 
                     data[0] & 0xFF, data[1] & 0xFF, 
                     data[2] & 0xFF, data[3] & 0xFF);
        } else {
            strcpy(ip_str, "127.0.0.1");
        }
        
        unsigned int flags = 0;
        X509_check_ip_asc(x509, ip_str, flags);
        X509_free(x509);
    }
    
    // 4. Use ossl_check_OPENSSL_CSTRING_lh_plain_type
    OPENSSL_CSTRING test_string = "test";
    OPENSSL_CSTRING *checked_ptr = ossl_check_OPENSSL_CSTRING_lh_plain_type(&test_string);
    (void)checked_ptr; // Suppress unused warning
    
    // 5. Use EC_GROUP_get_field_type
    EC_GROUP *group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    if (group) {
        EC_GROUP_get_field_type(group);
        EC_GROUP_free(group);
    }
    
    // 6. Use X509_signature_print with data from input
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        BIO *bio = BIO_new_fp(in_file, BIO_NOCLOSE);
        if (bio) {
            X509_ALGOR *alg = X509_ALGOR_new();
            ASN1_STRING *sig_str = ASN1_STRING_new();
            
            if (alg && sig_str) {
                // Try to read some data for the signature
                if (size > 0) {
                    unsigned char *sig_data = (unsigned char *)OPENSSL_malloc(size);
                    if (sig_data) {
                        memcpy(sig_data, data, size);
                        ASN1_STRING_set(sig_str, sig_data, size > 128 ? 128 : size);
                        OPENSSL_free(sig_data);
                    }
                }
                
                BIO *out_bio = BIO_new_fp(stdout, BIO_NOCLOSE);
                if (out_bio) {
                    X509_signature_print(out_bio, alg, sig_str);
                    BIO_free(out_bio);
                }
            }
            
            if (alg) X509_ALGOR_free(alg);
            if (sig_str) ASN1_STRING_free(sig_str);
            BIO_free(bio);
        }
        assert_file_closed(&in_file);;
    }
    
    // Clean up any remaining resources
    assert_file_closed(&in_file);
	return 0;
}