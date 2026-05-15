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
//<ID> 517
//<Prompt> []
/*<Combination>: [
*/
//<score> 120, nr_unique_branch: 38
//<Quality> {"density":10,"unique_branches":{"i2a_ASN1_OBJECT":[[194,24,194,41,0,0,4,1],[197,9,197,35,0,0,4,1],[206,9,206,15,0,0,4,1],[213,9,213,17,0,0,4,1]],"evp_pkey_asn1_find":[[72,19,72,53,0,0,4,1]],"DSA_SIG_free":[[43,9,43,20,0,0,4,1]],"d2i_DSA_SIG":[[54,9,54,16,0,0,4,1],[56,9,56,21,0,0,4,1],[60,13,60,24,0,0,4,1],[63,9,63,23,0,0,4,0],[65,9,65,23,0,0,4,0],[67,9,67,23,0,0,4,1],[67,27,67,41,0,0,4,1],[68,12,68,75,0,0,4,0],[69,13,69,25,0,0,4,0]],"ossl_decode_der_dsa_sig":[[241,9,241,43,0,0,4,1],[242,12,242,37,0,0,4,1],[243,12,243,30,0,0,4,0]],"X509_signature_print":[[302,9,302,29,0,0,4,0],[305,13,305,62,0,0,4,0],[305,13,305,62,0,0,4,1],[307,17,307,22,0,0,4,0],[307,26,307,42,0,0,4,0]],"dsa_sig_print":[[380,9,380,20,0,0,4,1],[388,9,388,24,0,0,4,1],[406,9,406,32,0,0,4,1]],"ossl_obj_find_sigid_algs":[[81,13,81,17,0,0,4,0],[81,21,81,55,0,0,4,1],[90,13,90,17,0,0,4,0]],"rsa_sig_print":[[404,9,404,59,0,0,4,1],[412,16,412,39,0,0,4,1],[415,9,415,12,0,0,4,0]],"OBJ_obj2txt":[[409,9,409,17,0,0,4,0],[409,21,409,56,0,0,4,0],[411,13,411,22,0,0,4,1],[413,13,413,22,0,0,4,0],[414,17,414,28,0,0,4,0]],"asn1_template_noexp_d2i":[[668,21,668,28,0,0,4,0]]},"library_calls":["UI_new","UI_add_input_string","UI_free","BN_get_rfc3526_prime_4096","BN_free","d2i_X509","X509_check_ip_asc","X509_free","ossl_check_OPENSSL_CSTRING_lh_plain_type","EC_GROUP_new_by_curve_name","EC_GROUP_get_field_type","EC_GROUP_free","BIO_new_fp","ASN1_STRING_new","X509_ALGOR_new","OBJ_nid2obj","ASN1_STRING_set","X509_signature_print","ASN1_STRING_free","X509_ALGOR_free","BIO_free"],"critical_calls":["UI_new","UI_add_input_string","UI_free","BN_get_rfc3526_prime_4096","BN_free","d2i_X509","X509_check_ip_asc","X509_free","ossl_check_OPENSSL_CSTRING_lh_plain_type","EC_GROUP_new_by_curve_name","EC_GROUP_get_field_type","EC_GROUP_free","BIO_new_fp","ASN1_STRING_new","X509_ALGOR_new","OBJ_nid2obj","ASN1_STRING_set","X509_signature_print","ASN1_STRING_free","X509_ALGOR_free","BIO_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. UI_add_input_string
    UI *ui = UI_new();
    if (ui) {
        // Use the first 100 bytes of input for the result buffer if available
        char result_buf[101] = {0}; // +1 for null terminator
        size_t ui_input_size = (size < 100) ? size : 100;
        if (ui_input_size > 0) {
            // Copy input data to result buffer for UI input simulation
            memcpy(result_buf, data, ui_input_size);
            result_buf[ui_input_size] = '\0';
        }
        // Add input string to UI - using minimal flags and sizes
        UI_add_input_string(ui, "Fuzz prompt:", UI_INPUT_FLAG_DEFAULT_PWD,
                            result_buf, 0, sizeof(result_buf)-1);
        UI_free(ui);
    }

    // 2. BN_get_rfc3526_prime_4096
    BIGNUM *prime = BN_get_rfc3526_prime_4096(nullptr);
    if (prime) {
        BN_free(prime);
    }

    // 3. X509_check_ip_asc - Need an X509 certificate
    // Try to parse input as X509 certificate
    const unsigned char *p = data;
    X509 *x509 = d2i_X509(nullptr, &p, size);
    if (x509) {
        // Check against a sample IP address
        X509_check_ip_asc(x509, "192.168.1.1", 0);
        X509_free(x509);
    }

    // 4. ossl_check_OPENSSL_CSTRING_lh_plain_type
    // Create a simple string array for demonstration
    const char *test_strings[] = {"test1", "test2", nullptr};
    OPENSSL_CSTRING *checked_ptr = ossl_check_OPENSSL_CSTRING_lh_plain_type(
        const_cast<OPENSSL_CSTRING*>(test_strings));
    (void)checked_ptr; // Mark as used

    // 5. EC_GROUP_get_field_type
    EC_GROUP *group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    if (group) {
        EC_GROUP_get_field_type(group);
        EC_GROUP_free(group);
    }

    // 6. X509_signature_print
    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Create FILE* for writing output
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            BIO *bp = BIO_new_fp(out_file, BIO_NOCLOSE);
            if (bp) {
                // Create sample ASN1_STRING and X509_ALGOR from input
                ASN1_STRING *asn1_str = ASN1_STRING_new();
                X509_ALGOR *alg = X509_ALGOR_new();
                
                if (asn1_str && alg && size > 0) {
                    // Use first byte as algorithm identifier (simplified)
                    alg->algorithm = OBJ_nid2obj((int)data[0] % 100);
                    // Use input data for ASN1_STRING
                    ASN1_STRING_set(asn1_str, data, (size > 1000) ? 1000 : size);
                    
                    X509_signature_print(bp, alg, asn1_str);
                }
                
                if (asn1_str) ASN1_STRING_free(asn1_str);
                if (alg) X509_ALGOR_free(alg);
                BIO_free(bp);
            }
            assert_file_closed(&out_file);;
            remove("output_file"); // Clean up output file
        }
        assert_file_closed(&in_file);;
    }

    assert_file_closed(&in_file);
	return 0;
}