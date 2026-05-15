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
//<ID> 295
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"d2i_PKCS12_bio":[[253,9,253,20,0,0,4,1]]},"library_calls":["OPENSSL_init_crypto","OPENSSL_init_crypto","BIO_new_fp","BIO_new_fp","BIO_free","BIO_free","d2i_PKCS12_fp","BIO_ctrl","d2i_PKCS12_bio","i2d_PKCS12_bio","EVP_PKEY_CTX_new_id","EVP_PKEY_encrypt_init","EVP_PKEY_CTX_set_rsa_padding","EVP_PKEY_CTX_set0_rsa_oaep_label","EVP_aes_128_cbc","EVP_PKEY_new_CMAC_key","EVP_PKEY_CTX_new","EVP_PKEY_sign_init","EVP_PKEY_CTX_free","OSSL_CMP_PKIHEADER_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","OSSL_CMP_HDR_get0_recipNonce","ASN1_OCTET_STRING_free","EVP_RAND_fetch","EVP_RAND_fetch","EVP_RAND_names_do_all","PKCS12_free","EVP_PKEY_CTX_free","EVP_PKEY_free","OSSL_CMP_PKIHEADER_free","EVP_RAND_free","BIO_free","BIO_free"],"critical_calls":["OPENSSL_init_crypto","OPENSSL_init_crypto","BIO_new_fp","BIO_new_fp","d2i_PKCS12_fp","BIO_ctrl","d2i_PKCS12_bio","i2d_PKCS12_bio","EVP_PKEY_CTX_new_id","EVP_PKEY_encrypt_init","EVP_PKEY_CTX_set_rsa_padding","EVP_PKEY_CTX_set0_rsa_oaep_label","EVP_aes_128_cbc","EVP_PKEY_new_CMAC_key","EVP_PKEY_CTX_new","EVP_PKEY_sign_init","EVP_PKEY_CTX_free","OSSL_CMP_PKIHEADER_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","OSSL_CMP_HDR_get0_recipNonce","ASN1_OCTET_STRING_free","EVP_RAND_fetch","EVP_RAND_fetch","EVP_RAND_names_do_all","PKCS12_free","EVP_PKEY_CTX_free","EVP_PKEY_free","OSSL_CMP_PKIHEADER_free","EVP_RAND_free","BIO_free","BIO_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL error strings
    ERR_load_crypto_strings();
    OPENSSL_init_crypto(0, NULL);

    // Create input file from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create BIOs for file operations
    BIO *in_bio = BIO_new_fp(in_file, BIO_NOCLOSE);
    BIO *out_bio = BIO_new_fp(out_file, BIO_NOCLOSE);
    
    if (!in_bio || !out_bio) {
        if (in_bio) BIO_free(in_bio);
        if (out_bio) BIO_free(out_bio);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Parse PKCS12 structure from input
    PKCS12 *p12 = d2i_PKCS12_fp(in_file, NULL);
    if (!p12) {
        // Try to read from BIO instead
        BIO_seek(in_bio, 0);
        p12 = d2i_PKCS12_bio(in_bio, NULL);
    }

    // 2. Use i2d_PKCS12_bio to write PKCS12 to BIO
    if (p12) {
        i2d_PKCS12_bio(out_bio, p12);
    }

    // 3. Create RSA OAEP context and set label
    EVP_PKEY_CTX *pkey_ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (pkey_ctx) {
        // Initialize for encryption
        if (EVP_PKEY_encrypt_init(pkey_ctx) > 0) {
            // Set OAEP padding
            EVP_PKEY_CTX_set_rsa_padding(pkey_ctx, RSA_PKCS1_OAEP_PADDING);
            
            // Set OAEP label from input data (use first 32 bytes or available size)
            size_t label_len = size > 32 ? 32 : size;
            void *label = malloc(label_len);
            if (label) {
                memcpy(label, data, label_len);
                EVP_PKEY_CTX_set0_rsa_oaep_label(pkey_ctx, label, label_len);
                // Note: label is now owned by pkey_ctx and will be freed by it
            }
        }
    }

    // 4. Create CMAC key
    // Use a portion of input data as key material
    size_t key_len = size > 16 ? 16 : size;
    const unsigned char *key_data = data;
    
    // Use AES-128-CBC cipher for CMAC
    const EVP_CIPHER *cipher = EVP_aes_128_cbc();
    EVP_PKEY *cmac_key = EVP_PKEY_new_CMAC_key(NULL, key_data, key_len, cipher);
    
    if (cmac_key) {
        // Use the CMAC key for something (e.g., create a context)
        EVP_PKEY_CTX *cmac_ctx = EVP_PKEY_CTX_new(cmac_key, NULL);
        if (cmac_ctx) {
            // Initialize for signing
            EVP_PKEY_sign_init(cmac_ctx);
            EVP_PKEY_CTX_free(cmac_ctx);
        }
    }

    // 5. Create dummy CMP header and get recipient nonce
    OSSL_CMP_PKIHEADER *hdr = OSSL_CMP_PKIHEADER_new();
    if (hdr) {
        // Create a nonce from input data
        ASN1_OCTET_STRING *nonce = ASN1_OCTET_STRING_new();
        if (nonce && size > 0) {
            size_t nonce_len = size > 20 ? 20 : size; // Typical nonce size
            ASN1_OCTET_STRING_set(nonce, data, nonce_len);
            // In real usage, we would set this nonce in the header
            // For this fuzzer, we'll just get whatever nonce might be there
        }
        
        // Get recipient nonce (may be NULL)
        const ASN1_OCTET_STRING *recip_nonce = OSSL_CMP_HDR_get0_recipNonce(hdr);
        
        if (nonce) ASN1_OCTET_STRING_free(nonce);
    }

    // 6. Use EVP_RAND_names_do_all
    // Get default random method
    EVP_RAND *rand = EVP_RAND_fetch(NULL, "CTR-DRBG", NULL);
    if (!rand) {
        rand = EVP_RAND_fetch(NULL, "HMAC-DRBG", NULL);
    }
    
    if (rand) {
        // Define callback function
        auto name_callback = [](const char *name, void *data) {
            // Do nothing, just consume the name
            (void)name;
            (void)data;
        };
        
        // List all names
        EVP_RAND_names_do_all(rand, name_callback, NULL);
    }

    // Cleanup
    if (p12) PKCS12_free(p12);
    if (pkey_ctx) EVP_PKEY_CTX_free(pkey_ctx);
    if (cmac_key) EVP_PKEY_free(cmac_key);
    if (hdr) OSSL_CMP_PKIHEADER_free(hdr);
    if (rand) EVP_RAND_free(rand);
    
    BIO_free(in_bio);
    BIO_free(out_bio);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up OpenSSL
    ERR_free_strings();
    EVP_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}