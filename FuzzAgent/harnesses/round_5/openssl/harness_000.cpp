#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/evp.h>
#include <openssl/bn.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/rand.h>

namespace harness_000 {
/*
 * Fuzzing harness for OpenSSL targeting multiple cryptographic APIs
 * This harness tests: hash functions, symmetric encryption, bignum operations, and X509 parsing
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize OpenSSL
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | 
                        OPENSSL_INIT_ADD_ALL_CIPHERS | 
                        OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
    ERR_clear_error();

    if (size < 32) {
        return 0;  // Need minimum input size
    }

    FuzzedDataProvider fdp(data, size);

    // ==================== HASH FUNCTION TESTING ====================
    // Test various hash functions: SHA256, SHA1, MD5
    {
        // Consume data for hash testing
        size_t hash_input_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
        std::vector<uint8_t> hash_input = fdp.ConsumeBytes<uint8_t>(hash_input_len);
        
        // Choose hash algorithm
        int hash_choice = fdp.ConsumeIntegralInRange<int>(0, 3);
        const EVP_MD* md = NULL;
        
        switch (hash_choice) {
            case 0: md = EVP_sha256(); break;
            case 1: md = EVP_sha1(); break;
            case 2: md = EVP_md5(); break;
            case 3: md = EVP_sha512(); break;
        }
        
        if (md && hash_input.size() > 0) {
            EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
            if (mdctx) {
                unsigned char hash_value[EVP_MAX_MD_SIZE];
                unsigned int hash_len;
                
                if (EVP_DigestInit_ex(mdctx, md, NULL)) {
                    EVP_DigestUpdate(mdctx, hash_input.data(), hash_input.size());
                    EVP_DigestFinal_ex(mdctx, hash_value, &hash_len);
                }
                
                EVP_MD_CTX_free(mdctx);
            }
        }
    }

    // ==================== SYMMETRIC ENCRYPTION TESTING ====================
    // Test AES encryption/decryption
    {
        // Consume data for encryption
        size_t enc_input_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        std::vector<uint8_t> enc_input = fdp.ConsumeBytes<uint8_t>(enc_input_len);
        
        // Consume key and IV
        std::vector<uint8_t> key = fdp.ConsumeBytes<uint8_t>(16);  // 128-bit key
        std::vector<uint8_t> iv = fdp.ConsumeBytes<uint8_t>(16);   // 128-bit IV
        
        if (enc_input.size() > 0 && key.size() == 16 && iv.size() == 16) {
            EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
            if (ctx) {
                // Try encryption
                const EVP_CIPHER* cipher = EVP_aes_128_cbc();
                
                int outlen, final_outlen;
                std::vector<unsigned char> ciphertext(enc_input.size() + EVP_MAX_BLOCK_LENGTH);
                
                if (EVP_EncryptInit_ex(ctx, cipher, NULL, key.data(), iv.data())) {
                    EVP_EncryptUpdate(ctx, ciphertext.data(), &outlen, 
                                     enc_input.data(), enc_input.size());
                    EVP_EncryptFinal_ex(ctx, ciphertext.data() + outlen, &final_outlen);
                }
                
                // Try decryption
                std::vector<unsigned char> plaintext(ciphertext.size());
                EVP_CIPHER_CTX* dctx = EVP_CIPHER_CTX_new();
                if (dctx && EVP_DecryptInit_ex(dctx, cipher, NULL, key.data(), iv.data())) {
                    int declen, final_declen;
                    EVP_DecryptUpdate(dctx, plaintext.data(), &declen, 
                                     ciphertext.data(), outlen + final_outlen);
                    EVP_DecryptFinal_ex(dctx, plaintext.data() + declen, &final_declen);
                    EVP_CIPHER_CTX_free(dctx);
                }
                
                EVP_CIPHER_CTX_free(ctx);
            }
        }
    }

    // ==================== BIGNUM OPERATIONS ====================
    // Test bignum arithmetic
    {
        // Consume data for bignums
        size_t bn1_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        size_t bn2_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        std::vector<uint8_t> bn1_data = fdp.ConsumeBytes<uint8_t>(bn1_len);
        std::vector<uint8_t> bn2_data = fdp.ConsumeBytes<uint8_t>(bn2_len);
        
        if (bn1_data.size() > 0 && bn2_data.size() > 0) {
            BIGNUM* a = BN_bin2bn(bn1_data.data(), bn1_data.size(), NULL);
            BIGNUM* b = BN_bin2bn(bn2_data.data(), bn2_data.size(), NULL);
            
            if (a && b) {
                BN_CTX* ctx = BN_CTX_new();
                if (ctx) {
                    BIGNUM* result = BN_new();
                    
                    // Test various operations
                    BN_add(result, a, b);
                    BN_sub(result, a, b);
                    BN_mul(result, a, b, ctx);
                    
                    // Only divide if b is not zero
                    if (!BN_is_zero(b)) {
                        BIGNUM* rem = BN_new();
                        BN_div(result, rem, a, b, ctx);
                        BN_free(rem);
                    }
                    
                    BN_free(result);
                    BN_CTX_free(ctx);
                }
                
                BN_free(a);
                BN_free(b);
            }
        }
    }

    // ==================== X509 PARSING ====================
    // Test X509 certificate parsing
    {
        // Use remaining bytes for X509 data
        std::vector<uint8_t> x509_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        if (x509_data.size() > 0) {
            const unsigned char* p = x509_data.data();
            X509* cert = d2i_X509(NULL, &p, x509_data.size());
            
            if (cert) {
                // Extract and test various certificate fields
                X509_get_subject_name(cert);
                X509_get_issuer_name(cert);
                X509_get_serialNumber(cert);
                X509_get_pubkey(cert);
                
                X509_free(cert);
            }
        }
    }

    // Clear any accumulated errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_000
