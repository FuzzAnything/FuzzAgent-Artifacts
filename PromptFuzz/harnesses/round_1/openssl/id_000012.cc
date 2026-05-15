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
//<ID> 181
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 12
//<Quality> {"density":17,"unique_branches":{"BN_mod_lshift_quick":[[306,13,306,26,0,0,4,0]],"CRYPTO_cbc128_decrypt":[[84,9,84,17,0,0,4,1],[88,9,88,18,0,0,4,0],[91,33,91,96,0,0,4,1],[102,20,102,29,0,0,4,0],[102,20,102,29,0,0,4,1],[107,29,107,52,0,0,4,0],[107,29,107,52,0,0,4,1],[115,13,115,23,0,0,4,0],[151,12,151,15,0,0,4,1]],"CRYPTO_cts128_decrypt_block":[[172,9,172,34,0,0,4,1],[177,9,177,12,0,0,4,0]]},"library_calls":["EVP_PKEY_free","BN_free","BIO_free","OPENSSL_version_patch","EVP_PKEY_new_raw_public_key","BN_bin2bn","BN_bin2bn","BN_new","BN_mod_lshift_quick","BN_free","BN_free","BN_free","CRYPTO_cts128_decrypt_block","BIO_new_fp","X509_ALGOR_new","ASN1_STRING_new","OBJ_nid2obj","X509_ALGOR_set0","ASN1_STRING_set","X509_signature_print","X509_ALGOR_free","ASN1_STRING_free","OSSL_FUNC_cipher_pipeline_decrypt_init","EVP_MAC_fetch","EVP_MAC_CTX_new","OSSL_PARAM_construct_utf8_string","OSSL_PARAM_construct_end","EVP_MAC_init","EVP_MAC_update","EVP_MAC_final","EVP_MAC_CTX_free","EVP_MAC_free"],"critical_calls":["BIO_free","BN_free","EVP_PKEY_free","OPENSSL_version_patch","EVP_PKEY_new_raw_public_key","BN_bin2bn","BN_bin2bn","BN_new","BN_free","BN_free","BN_free","CRYPTO_cts128_decrypt_block","BIO_new_fp","X509_ALGOR_new","ASN1_STRING_new","OBJ_nid2obj","X509_ALGOR_set0","ASN1_STRING_set","X509_signature_print","X509_ALGOR_free","ASN1_STRING_free","OSSL_FUNC_cipher_pipeline_decrypt_init","EVP_MAC_fetch","EVP_MAC_CTX_new","OSSL_PARAM_construct_utf8_string","OSSL_PARAM_construct_end","EVP_MAC_init","EVP_MAC_update","EVP_MAC_final","EVP_MAC_CTX_free","EVP_MAC_free"],"visited":1}
/**/


// Define dummy block function for CTS decryption
static void  id_000012_dummy_block(const unsigned char in[16], unsigned char out[16], const void *key) {
    memcpy(out, in, 16);
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Track allocated resources for cleanup
    struct Cleanup {
        std::vector<EVP_PKEY*> pkeys;
        std::vector<BIGNUM*> bignums;
        std::vector<BIO*> bios;
        std::vector<FILE*> files;
        ~Cleanup() {
            for (auto pkey : pkeys) EVP_PKEY_free(pkey);
            for (auto bn : bignums) BN_free(bn);
            for (auto bio : bios) BIO_free(bio);
            for (auto file : files) assert_file_closed(&file);;
        }
    } cleanup;

    // 1. Get OpenSSL patch version
    unsigned int patch = OPENSSL_version_patch();
    (void)patch; // Use patch value to avoid unused variable warning

    // 2. Create raw public key from input data
    // Use first 32 bytes as key material if available
    size_t key_len = size > 32 ? 32 : size;
    EVP_PKEY *raw_pkey = EVP_PKEY_new_raw_public_key(
        EVP_PKEY_ED25519,  // Using ED25519 as example type
        nullptr,
        data,
        key_len
    );
    if (raw_pkey) cleanup.pkeys.push_back(raw_pkey);

    // 3. Perform BN modular left shift operation
    // Use next bytes for BIGNUM operations if available
    if (size >= key_len + 16) {
        const uint8_t* bn_data = data + key_len;
        size_t bn_data_len = size - key_len;
        
        // Create BIGNUMs for the operation
        BIGNUM *a = BN_bin2bn(bn_data, std::min(bn_data_len, (size_t)8), nullptr);
        BIGNUM *m = BN_bin2bn(bn_data + 8, std::min(bn_data_len - 8, (size_t)8), nullptr);
        BIGNUM *r = BN_new();
        
        if (a && m && r) {
            cleanup.bignums.push_back(a);
            cleanup.bignums.push_back(m);
            cleanup.bignums.push_back(r);
            
            // Perform modular left shift (shift by 2 bits as example)
            BN_mod_lshift_quick(r, a, 2, m);
        } else {
            if (a) BN_free(a);
            if (m) BN_free(m);
            if (r) BN_free(r);
        }
    }

    // 4. Perform CTS decryption
    if (size >= 48) { // Need at least 48 bytes for key(16) + IV(16) + minimum data(16)
        const uint8_t* cts_data = data;
        size_t cts_len = size;
        
        // Split data: first 16 bytes as key, next 16 as IV, rest as ciphertext
        unsigned char key[16];
        unsigned char ivec[16];
        memcpy(key, cts_data, 16);
        memcpy(ivec, cts_data + 16, 16);
        
        size_t cipher_len = cts_len - 32;
        if (cipher_len >= 16) { // CTS requires at least one block
            const unsigned char* ciphertext = cts_data + 32;
            unsigned char* plaintext = new unsigned char[cipher_len];
            
            CRYPTO_cts128_decrypt_block(
                ciphertext,
                plaintext,
                cipher_len,
                key,
                ivec,
                 id_000012_dummy_block
            );
            
            delete[] plaintext;
        }
    }

    // 5. Create FILE pointer for reading data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        cleanup.files.push_back(in_file);
        
        // Create output file for signature printing
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            cleanup.files.push_back(out_file);
            
            // Create BIO from FILE for X509_signature_print
            BIO *bp = BIO_new_fp(out_file, BIO_NOCLOSE);
            if (bp) {
                cleanup.bios.push_back(bp);
                
                // Create dummy signature for printing
                // Using minimal valid ASN.1 structures
                X509_ALGOR *alg = X509_ALGOR_new();
                ASN1_STRING *sig = ASN1_STRING_new();
                
                if (alg && sig) {
                    // Initialize with dummy values
                    ASN1_OBJECT *obj = OBJ_nid2obj(NID_sha256);
                    X509_ALGOR_set0(alg, obj, V_ASN1_NULL, NULL);
                    ASN1_STRING_set(sig, data, std::min(size, (size_t)128));
                    
                    // Print signature
                    X509_signature_print(bp, alg, sig);
                    
                    X509_ALGOR_free(alg);
                    ASN1_STRING_free(sig);
                }
            }
        }
    }

    // 6. Get cipher pipeline decrypt init function
    // Create minimal OSSL_DISPATCH structure
    OSSL_DISPATCH dispatch[] = {
        {0, nullptr}
    };
    OSSL_FUNC_cipher_pipeline_decrypt_init_fn *decrypt_init = 
        OSSL_FUNC_cipher_pipeline_decrypt_init(dispatch);
    (void)decrypt_init; // Use to avoid unused variable warning

    // 7. Create MAC context and finalize
    // Use EVP_MAC for HMAC-SHA256 as example
    EVP_MAC *mac = EVP_MAC_fetch(nullptr, "HMAC", nullptr);
    if (mac) {
        // Create MAC context
        EVP_MAC_CTX *mac_ctx = EVP_MAC_CTX_new(mac);
        if (mac_ctx) {
            // Set up HMAC with SHA256
            OSSL_PARAM params[2];
            const char *digest = "SHA256";
            params[0] = OSSL_PARAM_construct_utf8_string("digest", (char *)digest, 0);
            params[1] = OSSL_PARAM_construct_end();
            
            // Initialize with key from data
            const unsigned char *key_data = data;
            size_t key_size = std::min(size, (size_t)32);
            
            if (EVP_MAC_init(mac_ctx, key_data, key_size, params) > 0) {
                // Update with remaining data
                if (size > key_size) {
                    EVP_MAC_update(mac_ctx, data + key_size, size - key_size);
                }
                
                // Finalize MAC
                unsigned char mac_out[32];
                size_t mac_out_len;
                EVP_MAC_final(mac_ctx, mac_out, &mac_out_len, sizeof(mac_out));
            }
            
            EVP_MAC_CTX_free(mac_ctx);
        }
        EVP_MAC_free(mac);
    }

    // Cleanup happens automatically via Cleanup destructor
    assert_file_closed(&in_file);
	return 0;
}