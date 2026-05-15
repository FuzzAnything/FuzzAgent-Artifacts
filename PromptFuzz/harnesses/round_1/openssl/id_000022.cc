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
//<ID> 282
//<Prompt> []
/*<Combination>: [
*/
//<score> 70, nr_unique_branch: 45
//<Quality> {"density":7,"unique_branches":{"file_ctrl":[[358,5,358,22,0,0,4,0]],"ASN1_bn_print":[[54,9,54,20,0,0,4,0]],"BIO_write_ex":[[404,12,404,56,0,0,4,0]],"prefix_write":[[86,9,86,20,0,0,4,1],[93,10,93,29,0,0,4,0],[94,12,94,28,0,0,4,1],[106,12,106,20,0,0,4,0],[106,12,106,20,0,0,4,1],[114,13,114,27,0,0,4,0],[114,13,114,27,0,0,4,1],[117,17,117,36,0,0,4,1],[126,31,126,39,0,0,4,0],[126,31,126,39,0,0,4,1],[126,43,126,63,0,0,4,0],[126,43,126,63,0,0,4,1],[128,13,128,22,0,0,4,0],[128,13,128,22,0,0,4,1],[132,16,132,21,0,0,4,0],[132,16,132,21,0,0,4,1],[135,17,135,57,0,0,4,1],[144,13,144,22,0,0,4,0],[144,13,144,22,0,0,4,1]],"print_set_indent":[[1113,9,1113,19,0,0,4,0],[1116,26,1116,31,0,0,4,1],[1117,13,1117,46,0,0,4,0],[1120,17,1120,32,0,0,4,1],[1125,13,1125,46,0,0,4,1]],"prefix_create":[[56,9,56,20,0,0,4,1]],"print_reset_indent":[[1099,9,1099,21,0,0,4,0]],"pkey_rsa_print":[[306,9,306,21,0,0,4,1],[316,17,316,21,0,0,4,1],[324,13,324,67,0,0,4,1]],"prefix_ctrl":[[156,9,156,18,0,0,4,1],[156,22,156,53,0,0,4,1],[160,5,160,29,0,0,4,1],[170,5,170,29,0,0,4,0],[170,5,170,29,0,0,4,1],[171,13,171,21,0,0,4,0],[176,5,176,29,0,0,4,1],[179,5,179,12,0,0,4,0],[179,5,179,12,0,0,4,1],[181,17,181,20,0,0,4,0],[182,9,182,29,0,0,4,1],[183,9,183,28,0,0,4,1],[187,13,187,32,0,0,4,0]]},"library_calls":["d2i_X509_REQ_fp","X509_REQ_get_pubkey","EVP_PKEY_get1_RSA","EVP_PKEY_free","RSA_print_fp","RSA_new","RSA_print_fp","d2i_X509_NAME_ENTRY","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","ASN1_OCTET_STRING_set","ASN1_OCTET_STRING_cmp","SSL_CTX_new","TLS_method","SSL_new","SSL_is_quic","SSL_client_hello_get0_ciphers","EVP_PKEY_CTX_new_id","EVP_PKEY_derive_init","EVP_PKEY_CTX_set_scrypt_N","EC_KEY_new_by_curve_name","EC_KEY_generate_key","EC_KEY_decoded_from_explicit_params","ASN1_OCTET_STRING_free","ASN1_OCTET_STRING_free","X509_NAME_ENTRY_free","X509_REQ_free","RSA_free","SSL_free","SSL_CTX_free","EVP_PKEY_CTX_free","EC_KEY_free"],"critical_calls":["d2i_X509_REQ_fp","X509_REQ_get_pubkey","EVP_PKEY_get1_RSA","EVP_PKEY_free","RSA_new","RSA_print_fp","d2i_X509_NAME_ENTRY","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","ASN1_OCTET_STRING_set","ASN1_OCTET_STRING_cmp","SSL_CTX_new","TLS_method","SSL_new","SSL_is_quic","SSL_client_hello_get0_ciphers","EVP_PKEY_CTX_new_id","EVP_PKEY_derive_init","EVP_PKEY_CTX_set_scrypt_N","EC_KEY_new_by_curve_name","EC_KEY_generate_key","EC_KEY_decoded_from_explicit_params","ASN1_OCTET_STRING_free","ASN1_OCTET_STRING_free","X509_NAME_ENTRY_free","X509_REQ_free","RSA_free","SSL_free","SSL_CTX_free","EVP_PKEY_CTX_free","EC_KEY_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for operations

    // 1. Create FILE pointers for input/output
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 2. Read X509_REQ from input data
    X509_REQ *req = NULL;
    long file_pos = ftell(in_file);
    req = d2i_X509_REQ_fp(in_file, &req);
    
    RSA *rsa_key = NULL;
    if (req) {
        // Extract RSA public key from certificate request
        EVP_PKEY *pkey = X509_REQ_get_pubkey(req);
        if (pkey) {
            rsa_key = EVP_PKEY_get1_RSA(pkey);
            EVP_PKEY_free(pkey);
        }
    }
    
    // 3. Use RSA_print_fp to print RSA key information
    if (rsa_key) {
        RSA_print_fp(out_file, rsa_key, 4);
    } else {
        // Create a minimal RSA key if none was extracted
        rsa_key = RSA_new();
        RSA_print_fp(out_file, rsa_key, 4);
    }

    // 4. Parse X509_NAME_ENTRY from input data
    fseek(in_file, file_pos, SEEK_SET);  // Reset to beginning
    const unsigned char *p = data;
    X509_NAME_ENTRY *name_entry = d2i_X509_NAME_ENTRY(NULL, &p, size);
    
    // 5. Create and compare ASN1_OCTET_STRINGs
    ASN1_OCTET_STRING *str1 = ASN1_OCTET_STRING_new();
    ASN1_OCTET_STRING *str2 = ASN1_OCTET_STRING_new();
    
    if (str1 && str2 && size > 20) {
        // Use first 10 bytes for str1, next 10 for str2
        ASN1_OCTET_STRING_set(str1, data, size > 10 ? 10 : size);
        ASN1_OCTET_STRING_set(str2, data + (size > 10 ? 10 : 0), 
                             size > 20 ? 10 : (size > 10 ? size - 10 : 0));
        
        // Compare the two strings
        int cmp_result = ASN1_OCTET_STRING_cmp(str1, str2);
        (void)cmp_result;  // Result intentionally unused
    }

    // 6. SSL operations
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = NULL;
    if (ctx) {
        ssl = SSL_new(ctx);
        if (ssl) {
            // Check if SSL is QUIC
            int is_quic = SSL_is_quic(ssl);
            (void)is_quic;  // Result intentionally unused
            
            // Get client hello ciphers (simulated)
            const unsigned char *ciphers_out;
            size_t ciphers_len = SSL_client_hello_get0_ciphers(ssl, &ciphers_out);
            (void)ciphers_len;  // Result intentionally unused
        }
    }

    // 7. EVP_PKEY_CTX scrypt operation
    EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_SCRYPT, NULL);
    if (pctx) {
        if (EVP_PKEY_derive_init(pctx) > 0) {
            // Use a portion of input data as N parameter (modulo to safe range)
            uint64_t n_val = 1;
            if (size >= sizeof(uint64_t)) {
                memcpy(&n_val, data, sizeof(uint64_t));
                n_val = (n_val % 10000) + 1;  // Keep in reasonable range
            }
            EVP_PKEY_CTX_set_scrypt_N(pctx, n_val);
        }
    }

    // 8. EC_KEY operation
    EC_KEY *ec_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if (ec_key) {
        // Generate key pair to test the function
        EC_KEY_generate_key(ec_key);
        int decoded = EC_KEY_decoded_from_explicit_params(ec_key);
        (void)decoded;  // Result intentionally unused
    }

    // Cleanup
    if (str1) ASN1_OCTET_STRING_free(str1);
    if (str2) ASN1_OCTET_STRING_free(str2);
    if (name_entry) X509_NAME_ENTRY_free(name_entry);
    if (req) X509_REQ_free(req);
    if (rsa_key) RSA_free(rsa_key);
    if (ssl) SSL_free(ssl);
    if (ctx) SSL_CTX_free(ctx);
    if (pctx) EVP_PKEY_CTX_free(pctx);
    if (ec_key) EC_KEY_free(ec_key);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}