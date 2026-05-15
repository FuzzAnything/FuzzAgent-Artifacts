#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

namespace harness_000 {
/*
 * Fuzzing harness for OpenSSL basic cryptographic operations
 * Targets: Hash functions, symmetric encryption, and RSA operations
 */


int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < 32) {
        return 0; // Need minimum input for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize OpenSSL
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | 
                       OPENSSL_INIT_ADD_ALL_CIPHERS | 
                       OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
    ERR_clear_error();
    
    // Determine which operation to test based on first byte
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (operation) {
        case 0: {
            // Test hash functions (SHA1, SHA256, SHA512, MD5)
            size_t hash_input_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            std::vector<uint8_t> hash_input = fdp.ConsumeBytes<uint8_t>(hash_input_len);
            
            if (hash_input.size() > 0) {
                unsigned char md5_digest[MD5_DIGEST_LENGTH];
                unsigned char sha1_digest[SHA_DIGEST_LENGTH];
                unsigned char sha256_digest[SHA256_DIGEST_LENGTH];
                unsigned char sha512_digest[SHA512_DIGEST_LENGTH];
                
                MD5(hash_input.data(), hash_input.size(), md5_digest);
                SHA1(hash_input.data(), hash_input.size(), sha1_digest);
                SHA256(hash_input.data(), hash_input.size(), sha256_digest);
                SHA512(hash_input.data(), hash_input.size(), sha512_digest);
            }
            break;
        }
        
        case 1: {
            // Test AES encryption/decryption
            size_t aes_key_len = 16; // 128-bit key
            size_t aes_iv_len = 16;
            
            if (fdp.remaining_bytes() < aes_key_len + aes_iv_len + 16) {
                break;
            }
            
            std::vector<uint8_t> aes_key = fdp.ConsumeBytes<uint8_t>(aes_key_len);
            std::vector<uint8_t> aes_iv = fdp.ConsumeBytes<uint8_t>(aes_iv_len);
            size_t aes_data_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            std::vector<uint8_t> aes_data = fdp.ConsumeBytes<uint8_t>(aes_data_len);
            
            if (aes_data.size() > 0) {
                AES_KEY enc_key, dec_key;
                unsigned char iv[AES_BLOCK_SIZE];
                unsigned char outbuf[aes_data.size() + AES_BLOCK_SIZE];
                int outlen = 0;
                
                memcpy(iv, aes_iv.data(), AES_BLOCK_SIZE);
                
                AES_set_encrypt_key(aes_key.data(), 128, &enc_key);
                AES_cbc_encrypt(aes_data.data(), outbuf, aes_data.size(), &enc_key, iv, AES_ENCRYPT);
                
                memcpy(iv, aes_iv.data(), AES_BLOCK_SIZE); // Reset IV for decryption
                AES_set_decrypt_key(aes_key.data(), 128, &dec_key);
                AES_cbc_encrypt(outbuf, outbuf, aes_data.size(), &dec_key, iv, AES_DECRYPT);
            }
            break;
        }
        
        case 2: {
            // Test DES encryption/decryption
            size_t des_key_len = 8; // DES key is 56 bits + parity
            size_t des_iv_len = 8;
            
            if (fdp.remaining_bytes() < des_key_len + des_iv_len + 8) {
                break;
            }
            
            std::vector<uint8_t> des_key = fdp.ConsumeBytes<uint8_t>(des_key_len);
            std::vector<uint8_t> des_iv = fdp.ConsumeBytes<uint8_t>(des_iv_len);
            size_t des_data_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
            std::vector<uint8_t> des_data = fdp.ConsumeBytes<uint8_t>(des_data_len);
            
            if (des_data.size() > 0) {
                DES_key_schedule schedule;
                DES_cblock key, iv;
                unsigned char outbuf[des_data.size() + 8];
                
                memcpy(key, des_key.data(), 8);
                memcpy(iv, des_iv.data(), 8);
                
                DES_set_key_unchecked(&key, &schedule);
                DES_ncbc_encrypt(des_data.data(), outbuf, des_data.size(), &schedule, &iv, DES_ENCRYPT);
                
                memcpy(iv, des_iv.data(), 8); // Reset IV for decryption
                DES_ncbc_encrypt(outbuf, outbuf, des_data.size(), &schedule, &iv, DES_DECRYPT);
            }
            break;
        }
        
        case 3: {
            // Test RSA key generation and operations
            if (fdp.remaining_bytes() < 64) {
                break;
            }
            
            int rsa_bits = fdp.ConsumeIntegralInRange<int>(512, 2048);
            BIGNUM *e = BN_new();
            BN_set_word(e, 65537);
            
            RSA *rsa = RSA_new();
            if (RSA_generate_key_ex(rsa, rsa_bits, e, NULL)) {
                // Test encryption/decryption with random data
                size_t rsa_data_len = RSA_size(rsa) - 42; // PKCS#1 padding
                if (rsa_data_len > 0 && fdp.remaining_bytes() >= rsa_data_len) {
                    std::vector<uint8_t> rsa_data = fdp.ConsumeBytes<uint8_t>(rsa_data_len);
                    unsigned char *encrypted = (unsigned char *)malloc(RSA_size(rsa));
                    unsigned char *decrypted = (unsigned char *)malloc(RSA_size(rsa));
                    
                    if (encrypted && decrypted && rsa_data.size() > 0) {
                        int enc_len = RSA_public_encrypt(rsa_data.size(), rsa_data.data(), 
                                                        encrypted, rsa, RSA_PKCS1_PADDING);
                        if (enc_len > 0) {
                            RSA_private_decrypt(enc_len, encrypted, decrypted, rsa, RSA_PKCS1_PADDING);
                        }
                    }
                    
                    free(encrypted);
                    free(decrypted);
                }
            }
            
            BN_free(e);
            RSA_free(rsa);
            break;
        }
        
        case 4: {
            // Test EVP (high-level) interfaces
            size_t evp_input_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
            std::vector<uint8_t> evp_input = fdp.ConsumeBytes<uint8_t>(evp_input_len);
            
            if (evp_input.size() > 0) {
                // Test EVP digest
                EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
                const EVP_MD *md = EVP_sha256();
                unsigned char md_value[EVP_MAX_MD_SIZE];
                unsigned int md_len;
                
                if (md_ctx) {
                    EVP_DigestInit_ex(md_ctx, md, NULL);
                    EVP_DigestUpdate(md_ctx, evp_input.data(), evp_input.size());
                    EVP_DigestFinal_ex(md_ctx, md_value, &md_len);
                    EVP_MD_CTX_free(md_ctx);
                }
                
                // Test EVP cipher
                size_t key_len = 32; // 256-bit key for AES-256
                size_t iv_len = 16;
                
                if (fdp.remaining_bytes() >= key_len + iv_len) {
                    std::vector<uint8_t> key = fdp.ConsumeBytes<uint8_t>(key_len);
                    std::vector<uint8_t> iv = fdp.ConsumeBytes<uint8_t>(iv_len);
                    
                    EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new();
                    if (cipher_ctx) {
                        const EVP_CIPHER *cipher = EVP_aes_256_cbc();
                        unsigned char outbuf[evp_input.size() + EVP_MAX_BLOCK_LENGTH];
                        int outlen1, outlen2;
                        
                        EVP_EncryptInit_ex(cipher_ctx, cipher, NULL, key.data(), iv.data());
                        EVP_EncryptUpdate(cipher_ctx, outbuf, &outlen1, evp_input.data(), evp_input.size());
                        EVP_EncryptFinal_ex(cipher_ctx, outbuf + outlen1, &outlen2);
                        
                        EVP_CIPHER_CTX_free(cipher_ctx);
                    }
                }
            }
            break;
        }
        
        case 5: {
            // Test random number generation and seeding
            size_t rand_seed_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            std::vector<uint8_t> rand_seed = fdp.ConsumeBytes<uint8_t>(rand_seed_len);
            
            if (rand_seed.size() > 0) {
                // Seed the PRNG
                RAND_seed(rand_seed.data(), rand_seed.size());
                
                // Generate some random bytes
                unsigned char rand_buf[256];
                RAND_bytes(rand_buf, sizeof(rand_buf));
                
                // Test RAND_status
                int status = RAND_status();
                (void)status; // Use result to avoid unused variable warning
            }
            break;
        }
    }
    
    // Clear any accumulated errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_000
