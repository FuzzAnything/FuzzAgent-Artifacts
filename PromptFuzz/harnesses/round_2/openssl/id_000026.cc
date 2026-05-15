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
//<ID> 519
//<Prompt> []
/*<Combination>: [
*/
//<score> 35, nr_unique_branch: 15
//<Quality> {"density":7,"unique_branches":{"asn1_template_noexp_d2i":[[668,21,668,28,0,0,4,1],[693,13,693,19,0,0,4,0]],"BN_bn2hex":[[21,9,21,22,0,0,4,1],[24,9,24,20,0,0,4,1],[27,9,27,15,0,0,4,1],[29,26,29,32,0,0,4,0],[29,26,29,32,0,0,4,1],[30,32,30,38,0,0,4,0],[30,32,30,38,0,0,4,1],[33,17,33,18,0,0,4,0],[33,17,33,18,0,0,4,1],[33,22,33,28,0,0,4,0]],"asn1_item_embed_d2i":[[266,13,266,39,0,0,4,0],[268,17,268,20,0,0,4,1]],"asn1_ex_c2i":[[935,5,935,30,0,0,4,0]]},"library_calls":["BIO_new_fp","UI_new","UI_add_input_string","BN_get_rfc3526_prime_4096","BN_bn2hex","CRYPTO_free","BN_free","d2i_X509_fp","X509_check_ip_asc","X509_get0_signature","X509_signature_print","X509_free","ossl_check_OPENSSL_CSTRING_lh_plain_type","EC_GROUP_new_by_curve_name","EC_GROUP_get_field_type","EC_GROUP_free","UI_free","BIO_free"],"critical_calls":["BIO_new_fp","UI_new","UI_add_input_string","BN_get_rfc3526_prime_4096","BN_bn2hex","CRYPTO_free","BN_free","d2i_X509_fp","X509_check_ip_asc","X509_get0_signature","X509_signature_print","X509_free","ossl_check_OPENSSL_CSTRING_lh_plain_type","EC_GROUP_new_by_curve_name","EC_GROUP_get_field_type","EC_GROUP_free","UI_free","BIO_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data

    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create FILE* for writing output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create BIO for output
    BIO *out_bio = BIO_new_fp(out_file, BIO_NOCLOSE);
    if (!out_bio) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Create UI and add input string
    UI *ui = UI_new();
    if (ui) {
        char result_buf[256] = {0};
        size_t str_len = (size > 50) ? 50 : size;
        char *input_str = new char[str_len + 1];
        memcpy(input_str, data, str_len);
        input_str[str_len] = '\0';
        
        UI_add_input_string(ui, "Enter value:", 0, result_buf, 1, 100);
        
        // Simulate UI processing (in real scenario would use UI_process)
        strncpy(result_buf, input_str, sizeof(result_buf) - 1);
        delete[] input_str;
    }

    // 2. Get RFC3526 prime 4096
    BIGNUM *prime = BN_get_rfc3526_prime_4096(NULL);
    if (prime) {
        // Use the prime in some way
        char *prime_str = BN_bn2hex(prime);
        if (prime_str) {
            // Write prime to output
            fwrite(prime_str, strlen(prime_str), 1, out_file);
            OPENSSL_free(prime_str);
        }
        BN_free(prime);
    }

    // 3. Parse and check X509 certificate
    // Rewind to read certificate from input file
    rewind(in_file);
    X509 *cert = d2i_X509_fp(in_file, NULL);
    if (cert) {
        // Check IP address - using sample IP
        const char *test_ip = "192.168.1.1";
        int ip_check_result = X509_check_ip_asc(cert, test_ip, 0);
        
        // 6. Print signature
        const X509_ALGOR *sig_alg = NULL;
        const ASN1_BIT_STRING *sig = NULL;
        X509_get0_signature(&sig, &sig_alg, cert);
        
        if (sig_alg && sig) {
            X509_signature_print(out_bio, sig_alg, sig);
        }
        
        X509_free(cert);
    }

    // 4. Use OPENSSL_CSTRING lhash type checking
    // Create a sample OPENSSL_CSTRING array
    const char *strings[] = {"test1", "test2", "test3"};
    OPENSSL_CSTRING *checked = ossl_check_OPENSSL_CSTRING_lh_plain_type(
        (OPENSSL_CSTRING *)strings);

    // 5. Create EC_GROUP and get field type
    EC_GROUP *group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    if (group) {
        int field_type = EC_GROUP_get_field_type(group);
        // Write field type to file as binary
        fwrite(&field_type, sizeof(field_type), 1, out_file);
        EC_GROUP_free(group);
    }

    // Clean up
    if (ui) UI_free(ui);
    BIO_free(out_bio);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}