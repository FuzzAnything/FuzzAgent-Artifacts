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
//<ID> 280
//<Prompt> []
/*<Combination>: [
*/
//<score> 56, nr_unique_branch: 36
//<Quality> {"density":8,"unique_branches":{"ASN1_bn_print":[[54,9,54,20,0,0,4,1],[56,11,56,30,0,0,4,1],[57,9,57,55,0,0,4,1],[59,9,59,24,0,0,4,1],[65,9,65,38,0,0,4,0],[65,9,65,38,0,0,4,1],[66,13,69,17,0,0,4,1],[76,9,76,20,0,0,4,1],[78,9,80,13,0,0,4,1],[79,13,79,28,0,0,4,1],[84,9,84,52,0,0,4,1]],"print_set_indent":[[1113,9,1113,19,0,0,4,1]],"SSL_shutdown":[[2949,9,2949,39,0,0,4,1],[2957,9,2957,19,0,0,4,1],[2960,9,2960,35,0,0,4,0],[96,6,96,19,2,0,4,0],[32,6,32,19,6,0,4,1],[33,23,33,61,6,0,4,0],[1242,29,1242,48,7,0,4,1]],"pkey_rsa_print":[[306,9,306,21,0,0,4,0],[316,17,316,21,0,0,4,0],[317,13,319,17,0,0,4,1],[318,26,318,40,0,0,4,0]],"print_reset_indent":[[1099,9,1099,21,0,0,4,1]],"ASN1_buf_print":[[25,17,25,27,0,0,4,0],[25,17,25,27,0,0,4,1],[26,13,26,44,0,0,4,0],[26,13,26,44,0,0,4,1],[27,17,27,22,0,0,4,0],[27,17,27,22,0,0,4,1],[27,26,27,49,0,0,4,1],[29,17,29,63,0,0,4,1],[36,13,38,17,0,0,4,1],[37,17,37,34,0,0,4,0],[37,17,37,34,0,0,4,1],[41,9,41,36,0,0,4,1]]},"library_calls":["d2i_X509_REQ_fp","d2i_X509_NAME_ENTRY","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","RSA_print_fp","BN_free","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","ASN1_OCTET_STRING_set","ASN1_OCTET_STRING_set","ASN1_OCTET_STRING_cmp","SSL_CTX_new","TLS_method","SSL_new","SSL_is_quic","SSL_client_hello_get0_ciphers","EVP_PKEY_CTX_new_id","EVP_PKEY_derive_init","EVP_PKEY_CTX_set_scrypt_N","EC_KEY_new_by_curve_name","EC_KEY_generate_key","EC_KEY_decoded_from_explicit_params","X509_REQ_free","X509_NAME_ENTRY_free","ASN1_OCTET_STRING_free","ASN1_OCTET_STRING_free","RSA_free","SSL_shutdown","SSL_free","SSL_CTX_free","EVP_PKEY_CTX_free","EC_KEY_free"],"critical_calls":["d2i_X509_REQ_fp","d2i_X509_NAME_ENTRY","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","RSA_print_fp","BN_free","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","ASN1_OCTET_STRING_set","ASN1_OCTET_STRING_cmp","SSL_CTX_new","TLS_method","SSL_new","SSL_is_quic","SSL_client_hello_get0_ciphers","EVP_PKEY_CTX_new_id","EVP_PKEY_derive_init","EVP_PKEY_CTX_set_scrypt_N","EC_KEY_new_by_curve_name","EC_KEY_generate_key","EC_KEY_decoded_from_explicit_params","X509_REQ_free","X509_NAME_ENTRY_free","ASN1_OCTET_STRING_free","ASN1_OCTET_STRING_free","RSA_free","SSL_shutdown","SSL_free","SSL_CTX_free","EVP_PKEY_CTX_free","EC_KEY_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

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

    // 1. Parse X509_REQ from input
    X509_REQ *req = d2i_X509_REQ_fp(in_file, NULL);
    
    // Reset file position for next operations
    rewind(in_file);
    
    // 2. Parse X509_NAME_ENTRY from input
    const unsigned char *p = data;
    X509_NAME_ENTRY *name_entry = d2i_X509_NAME_ENTRY(NULL, &p, (long)size);
    
    // 3. Create RSA structure (using generated key since we can't parse from arbitrary data)
    RSA *rsa = RSA_new();
    if (rsa) {
        // Use a simple RSA key for demonstration
        BIGNUM *e = BN_new();
        BN_set_word(e, 65537);
        RSA_generate_key_ex(rsa, 512, e, NULL);
        
        // Call RSA_print_fp API
        RSA_print_fp(out_file, rsa, 0);
        
        BN_free(e);
    }
    
    // 4. Create and compare ASN1_OCTET_STRINGs
    ASN1_OCTET_STRING *str1 = ASN1_OCTET_STRING_new();
    ASN1_OCTET_STRING *str2 = ASN1_OCTET_STRING_new();
    
    if (str1 && str2) {
        // Set some data for comparison
        ASN1_OCTET_STRING_set(str1, data, size > 100 ? 100 : (int)size);
        if (size > 50) {
            ASN1_OCTET_STRING_set(str2, data + 50, size > 150 ? 100 : (int)size - 50);
        } else {
            ASN1_OCTET_STRING_set(str2, data, (int)size);
        }
        
        // Call ASN1_OCTET_STRING_cmp API
        int cmp_result = ASN1_OCTET_STRING_cmp(str1, str2);
        (void)cmp_result; // Use result to avoid compiler warning
    }
    
    // 5. SSL operations
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = ctx ? SSL_new(ctx) : NULL;
    
    if (ssl) {
        // Call SSL_is_quic API
        int is_quic = SSL_is_quic(ssl);
        (void)is_quic;
        
        // Call SSL_client_hello_get0_ciphers API
        const unsigned char *ciphers = NULL;
        size_t cipher_len = SSL_client_hello_get0_ciphers(ssl, &ciphers);
        (void)cipher_len;
    }
    
    // 6. EVP_PKEY_CTX operations
    EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_SCRYPT, NULL);
    if (pctx) {
        EVP_PKEY_derive_init(pctx);
        // Call EVP_PKEY_CTX_set_scrypt_N API
        // Use first 8 bytes of input as N value if available, otherwise use default
        uint64_t n = 16384; // Default
        if (size >= sizeof(uint64_t)) {
            n = *(uint64_t*)data;
        }
        int set_result = EVP_PKEY_CTX_set_scrypt_N(pctx, n);
        (void)set_result;
    }
    
    // 7. EC_KEY operations
    EC_KEY *ec_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if (ec_key) {
        EC_KEY_generate_key(ec_key);
        // Call EC_KEY_decoded_from_explicit_params API
        int decoded_from_explicit = EC_KEY_decoded_from_explicit_params(ec_key);
        (void)decoded_from_explicit;
    }
    
    // Cleanup
    if (req) X509_REQ_free(req);
    if (name_entry) X509_NAME_ENTRY_free(name_entry);
    if (str1) ASN1_OCTET_STRING_free(str1);
    if (str2) ASN1_OCTET_STRING_free(str2);
    if (rsa) RSA_free(rsa);
    if (ssl) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
    }
    if (ctx) SSL_CTX_free(ctx);
    if (pctx) EVP_PKEY_CTX_free(pctx);
    if (ec_key) EC_KEY_free(ec_key);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}