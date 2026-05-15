#include <stddef.h>
#include <stdint.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/aes.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_000 {
/*
 * First fuzzing harness for OpenSSL library
 * Targets fundamental cryptographic operations
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;  // Need minimum input
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_clear_error();

    // Consume operation type to decide what to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 5;

    switch (operation) {
        case 0: {
            // Test SHA-256 hash
            size_t hash_input_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            std::vector<uint8_t> hash_input = fdp.ConsumeBytes<uint8_t>(hash_input_len);
            
            unsigned char sha256_output[SHA256_DIGEST_LENGTH];
            SHA256(hash_input.data(), hash_input.size(), sha256_output);
            break;
        }

        case 1: {
            // Test MD5 hash
            size_t md5_input_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            std::vector<uint8_t> md5_input = fdp.ConsumeBytes<uint8_t>(md5_input_len);
            
            unsigned char md5_output[MD5_DIGEST_LENGTH];
            MD5(md5_input.data(), md5_input.size(), md5_output);
            break;
        }

        case 2: {
            // Test AES encryption
            // Need key and IV
            size_t aes_key_len = 16;  // 128-bit AES
            size_t aes_iv_len = 16;
            
            if (fdp.remaining_bytes() < aes_key_len + aes_iv_len + 16) {
                break;
            }
            
            std::vector<uint8_t> aes_key = fdp.ConsumeBytes<uint8_t>(aes_key_len);
            std::vector<uint8_t> aes_iv = fdp.ConsumeBytes<uint8_t>(aes_iv_len);
            size_t plaintext_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            std::vector<uint8_t> plaintext = fdp.ConsumeBytes<uint8_t>(plaintext_len);
            
            // Pad to multiple of block size
            size_t padded_len = ((plaintext_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
            std::vector<uint8_t> ciphertext(padded_len);
            std::vector<uint8_t> decrypted(padded_len);
            
            AES_KEY encrypt_key, decrypt_key;
            AES_set_encrypt_key(aes_key.data(), 128, &encrypt_key);
            AES_set_decrypt_key(aes_key.data(), 128, &decrypt_key);
            
            // Simple ECB mode for testing
            for (size_t i = 0; i < padded_len; i += AES_BLOCK_SIZE) {
                AES_ecb_encrypt(plaintext.data() + i, ciphertext.data() + i, &encrypt_key, AES_ENCRYPT);
                AES_ecb_encrypt(ciphertext.data() + i, decrypted.data() + i, &decrypt_key, AES_DECRYPT);
            }
            break;
        }

        case 3: {
            // Test HMAC
            size_t hmac_key_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
            size_t hmac_data_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            
            std::vector<uint8_t> hmac_key = fdp.ConsumeBytes<uint8_t>(hmac_key_len);
            std::vector<uint8_t> hmac_data = fdp.ConsumeBytes<uint8_t>(hmac_data_len);
            
            unsigned char hmac_output[EVP_MAX_MD_SIZE];
            unsigned int hmac_len;
            
            HMAC(EVP_sha256(), hmac_key.data(), hmac_key.size(),
                 hmac_data.data(), hmac_data.size(), hmac_output, &hmac_len);
            break;
        }

        case 4: {
            // Test random number generation with seed
            size_t rand_seed_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            std::vector<uint8_t> rand_seed = fdp.ConsumeBytes<uint8_t>(rand_seed_len);
            
            RAND_seed(rand_seed.data(), rand_seed.size());
            
            // Generate some random bytes
            unsigned char random_bytes[32];
            RAND_bytes(random_bytes, sizeof(random_bytes));
            break;
        }
    }

    // Clean up
    ERR_clear_error();
    return 0;
}

} // namespace harness_000
