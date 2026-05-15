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
//<ID> 616
//<Prompt> []
/*<Combination>: [
*/
//<score> 96, nr_unique_branch: 3
//<Quality> {"density":24,"unique_branches":{"ossl_ecx_key_fromdata":[[87,12,89,45,0,0,4,0]],"SSL_set_bio":[[1663,25,1663,37,0,0,4,1]],"get_string_internal":[[1372,9,1372,22,0,0,4,0]]},"library_calls":["OPENSSL_version_patch","BIO_new_fp","BN_new","BN_bin2bn","EVP_PKEY_new_raw_public_key","X509_ALGOR_new","ASN1_STRING_new","ASN1_STRING_set","OBJ_nid2obj","X509_ALGOR_set0","X509_signature_print","X509_STORE_new","X509_STORE_get0_param","TLS_method","SSL_CTX_new","SSL_new","BIO_new_fp","SSL_set_bio","SSL_read_ex","BIO_sock_init","EVP_MAC_fetch","EVP_MAC_CTX_new","OSSL_PARAM_construct_utf8_string","OSSL_PARAM_construct_end","EVP_MAC_init","EVP_MAC_update","EVP_MAC_final","EVP_MAC_CTX_free","EVP_MAC_free","sk_CONF_VALUE_cmpfunc_thunk","SSL_free","SSL_CTX_free","X509_STORE_free","EVP_PKEY_free","X509_ALGOR_free","ASN1_STRING_free","BN_free"],"critical_calls":["OPENSSL_version_patch","BIO_new_fp","BN_new","BN_bin2bn","EVP_PKEY_new_raw_public_key","X509_ALGOR_new","ASN1_STRING_new","ASN1_STRING_set","OBJ_nid2obj","X509_ALGOR_set0","X509_signature_print","X509_STORE_new","X509_STORE_get0_param","TLS_method","SSL_CTX_new","SSL_new","BIO_new_fp","SSL_set_bio","SSL_read_ex","BIO_sock_init","EVP_MAC_fetch","EVP_MAC_CTX_new","OSSL_PARAM_construct_utf8_string","OSSL_PARAM_construct_end","EVP_MAC_init","EVP_MAC_update","EVP_MAC_final","EVP_MAC_CTX_free","EVP_MAC_free","sk_CONF_VALUE_cmpfunc_thunk","SSL_free","SSL_CTX_free","X509_STORE_free","EVP_PKEY_free","X509_ALGOR_free","ASN1_STRING_free","BN_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get OpenSSL patch version
    unsigned int patch = OPENSSL_version_patch();
    
    // 2. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Create FILE* for output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create BIO from output FILE
    BIO *bio_out = BIO_new_fp(out_file, BIO_CLOSE);
    
    // 5. Create BIGNUM from input data
    BIGNUM *bn = BN_new();
    if (size > 0) {
        // Use first part of data for BIGNUM
        size_t bn_len = size > 32 ? 32 : size;
        BN_bin2bn(data, bn_len, bn);
    }
    
    // 6. Create raw public key from input data
    EVP_PKEY *pkey = NULL;
    if (size > 64) {
        // Use portion of data for raw public key
        size_t key_len = size > 128 ? 64 : size - 64;
        pkey = EVP_PKEY_new_raw_public_key(EVP_PKEY_ED25519, NULL, 
                                          data + 32, key_len);
    }
    
    // 7. Create X509_ALGOR and ASN1_STRING
    X509_ALGOR *algor = X509_ALGOR_new();
    ASN1_STRING *asn1_str = ASN1_STRING_new();
    
    // 8. Set ASN1_STRING with input data
    if (size > 0) {
        ASN1_STRING_set(asn1_str, data, size > 100 ? 100 : size);
    }
    
    // 9. Get ASN1_OBJECT for SHA256
    ASN1_OBJECT *obj = OBJ_nid2obj(NID_sha256);
    
    // 10. Set X509_ALGOR
    X509_ALGOR_set0(algor, obj, V_ASN1_NULL, NULL);
    
    // 11. Print signature
    X509_signature_print(bio_out, algor, asn1_str);
    
    // 12. Create X509_STORE and get parameters
    X509_STORE *store = X509_STORE_new();
    X509_VERIFY_PARAM *param = X509_STORE_get0_param(store);
    
    // 13. Create SSL context and connection
    const SSL_METHOD *method = TLS_method();
    SSL_CTX *ssl_ctx = SSL_CTX_new(method);
    SSL *ssl = SSL_new(ssl_ctx);
    
    // 14. Create BIO for input
    BIO *bio_in = BIO_new_fp(in_file, BIO_NOCLOSE);
    
    // 15. Set BIO for SSL
    SSL_set_bio(ssl, bio_in, bio_out);
    
    // 16. Try to read from SSL (will likely fail but exercise the API)
    char buf[1024];
    size_t readbytes = 0;
    SSL_read_ex(ssl, buf, sizeof(buf), &readbytes);
    
    // 17. Initialize socket BIO
    BIO_sock_init();
    
    // 18. Initialize MAC operations
    EVP_MAC *mac = EVP_MAC_fetch(NULL, "HMAC", NULL);
    if (mac) {
        EVP_MAC_CTX *mac_ctx = EVP_MAC_CTX_new(mac);
        if (mac_ctx) {
            // Create OSSL_PARAM array
            char digest_name[] = "SHA256";
            OSSL_PARAM params[2];
            params[0] = OSSL_PARAM_construct_utf8_string("digest", digest_name, sizeof(digest_name));
            params[1] = OSSL_PARAM_construct_end();
            
            // Initialize MAC with key from input data
            if (size > 16) {
                EVP_MAC_init(mac_ctx, data, 16, params);
                EVP_MAC_update(mac_ctx, data + 16, size - 16 > 100 ? 100 : size - 16);
                
                unsigned char mac_out[64];
                size_t mac_len = 0;
                EVP_MAC_final(mac_ctx, mac_out, &mac_len, sizeof(mac_out));
            }
            EVP_MAC_CTX_free(mac_ctx);
        }
        EVP_MAC_free(mac);
    }
    
    // 19. Use thunk comparison function (dummy call)
    int (*cmp_func)(const void*, const void*) = [](const void *a, const void *b) { return 0; };
    sk_CONF_VALUE_cmpfunc_thunk(cmp_func, NULL, NULL);
    
    // 20. Clean up
    SSL_free(ssl);
    SSL_CTX_free(ssl_ctx);
    
    // Note: bio_in and bio_out are freed by SSL_free
    // Don't call BIO_free(bio_in) or BIO_free(bio_out) here
    
    X509_STORE_free(store);
    EVP_PKEY_free(pkey);
    X509_ALGOR_free(algor);
    ASN1_STRING_free(asn1_str);
    BN_free(bn);
    
    // Close files (BIO_CLOSE should handle out_file)
    assert_file_closed(&in_file);;
    
    // The following APIs couldn't be reasonably called without proper setup:
    // - X509_policy_node_get0_policy (no policy node available)
    // - CRYPTO_cts128_decrypt_block (no cipher context)
    // - OSSL_FUNC_cipher_pipeline_decrypt_init (no dispatch table)
    // - ossl_check_OSSL_CMP_CERTRESPONSE_type (no CMP response)
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}