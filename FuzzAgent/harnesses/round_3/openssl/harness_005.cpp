#include <stddef.h>
#include <stdint.h>
#include <openssl/cast.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_005 {
/*
 * Fuzzing harness for OpenSSL CAST cipher operations
 * Targets: CAST_cbc_encrypt (46 undiscovered branches), EVP_cast5_cbc(), 
 *          EVP_cast5_ecb(), EVP_cast5_cfb64(), EVP_cast5_ofb()
 * Helper APIs: EVP_CIPHER_CTX_new(), EVP_EncryptInit_ex(), EVP_EncryptUpdate(), 
 *              EVP_EncryptFinal_ex(), EVP_CIPHER_CTX_free(), CAST_set_key
 * Implements full CAST cipher testing sequence with proper resource management
 * Differentiated from harness_000.cpp which focused on hash/symmetric crypto but missed CAST-specific paths
 * This harness targets legacy CAST cipher implementation with significant undiscovered complexity
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) {
        return 0;  // Need minimum input for key, IV, and data
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_clear_error();

    // Consume operation type to decide what to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;

    // CAST constants
    const size_t CAST_KEY_LEN = CAST_KEY_LENGTH;  // 16 bytes
    const size_t CAST_BLOCK_SIZE = CAST_BLOCK;    // 8 bytes
    const size_t CAST_IV_SIZE = CAST_BLOCK;       // 8 bytes

    // Consume key material (16 bytes for CAST-128)
    if (fdp.remaining_bytes() < CAST_KEY_LEN + CAST_IV_SIZE + 16) {
        return 0;  // Not enough data for key, IV, and minimal data
    }

    std::vector<uint8_t> cast_key = fdp.ConsumeBytes<uint8_t>(CAST_KEY_LEN);
    std::vector<uint8_t> cast_iv = fdp.ConsumeBytes<uint8_t>(CAST_IV_SIZE);
    
    // Consume data to encrypt (multiple of block size for simplicity)
    size_t data_len = fdp.ConsumeIntegralInRange<size_t>(CAST_BLOCK_SIZE, 256);
    data_len = (data_len / CAST_BLOCK_SIZE) * CAST_BLOCK_SIZE; // Ensure block alignment
    
    if (fdp.remaining_bytes() < data_len) {
        data_len = fdp.remaining_bytes();
        data_len = (data_len / CAST_BLOCK_SIZE) * CAST_BLOCK_SIZE; // Re-align
        if (data_len < CAST_BLOCK_SIZE) {
            return 0;  // Not enough data for even one block
        }
    }
    
    std::vector<uint8_t> plaintext = fdp.ConsumeBytes<uint8_t>(data_len);
    std::vector<uint8_t> ciphertext(data_len);
    std::vector<uint8_t> decrypted(data_len);

    switch (operation) {
        case 0: {
            // Test low-level CAST_cbc_encrypt directly
            CAST_KEY key;
            
            // Set up CAST key
            CAST_set_key(&key, CAST_KEY_LEN, cast_key.data());
            
            // Test encryption
            CAST_cbc_encrypt(plaintext.data(), ciphertext.data(), 
                           plaintext.size(), &key, cast_iv.data(), CAST_ENCRYPT);
            
            // Test decryption (need to copy IV again as it gets modified)
            std::vector<uint8_t> iv_copy = cast_iv;
            CAST_cbc_encrypt(ciphertext.data(), decrypted.data(), 
                           ciphertext.size(), &key, iv_copy.data(), CAST_DECRYPT);
            break;
        }
        
        case 1: {
            // Test EVP CAST5 CBC mode
            EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
            if (ctx == NULL) {
                break;
            }
            
            const EVP_CIPHER* cipher = EVP_cast5_cbc();
            if (cipher == NULL) {
                EVP_CIPHER_CTX_free(ctx);
                break;
            }
            
            // Initialize encryption
            if (EVP_EncryptInit_ex(ctx, cipher, NULL, cast_key.data(), cast_iv.data())) {
                int out_len1 = 0, out_len2 = 0;
                
                // Process data in chunks to test update multiple times
                size_t processed = 0;
                while (processed < plaintext.size()) {
                    size_t chunk_size = std::min(plaintext.size() - processed, CAST_BLOCK_SIZE * 4);
                    EVP_EncryptUpdate(ctx, ciphertext.data() + processed, &out_len1,
                                     plaintext.data() + processed, chunk_size);
                    processed += chunk_size;
                }
                
                // Finalize encryption
                EVP_EncryptFinal_ex(ctx, ciphertext.data() + processed, &out_len2);
                
                // Clean up for next operation
                EVP_CIPHER_CTX_free(ctx);
            }
            break;
        }
        
        case 2: {
            // Test EVP CAST5 ECB mode
            EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
            if (ctx == NULL) {
                break;
            }
            
            const EVP_CIPHER* cipher = EVP_cast5_ecb();
            if (cipher == NULL) {
                EVP_CIPHER_CTX_free(ctx);
                break;
            }
            
            // ECB mode doesn't use IV
            if (EVP_EncryptInit_ex(ctx, cipher, NULL, cast_key.data(), NULL)) {
                int out_len1 = 0, out_len2 = 0;
                
                // Process data
                EVP_EncryptUpdate(ctx, ciphertext.data(), &out_len1,
                                 plaintext.data(), plaintext.size());
                
                // Finalize encryption
                EVP_EncryptFinal_ex(ctx, ciphertext.data() + out_len1, &out_len2);
                
                EVP_CIPHER_CTX_free(ctx);
            }
            break;
        }
        
        case 3: {
            // Test EVP CAST5 CFB64 mode
            EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
            if (ctx == NULL) {
                break;
            }
            
            const EVP_CIPHER* cipher = EVP_cast5_cfb64();
            if (cipher == NULL) {
                EVP_CIPHER_CTX_free(ctx);
                break;
            }
            
            if (EVP_EncryptInit_ex(ctx, cipher, NULL, cast_key.data(), cast_iv.data())) {
                int out_len1 = 0, out_len2 = 0;
                
                // Process data
                EVP_EncryptUpdate(ctx, ciphertext.data(), &out_len1,
                                 plaintext.data(), plaintext.size());
                
                // Finalize encryption
                EVP_EncryptFinal_ex(ctx, ciphertext.data() + out_len1, &out_len2);
                
                EVP_CIPHER_CTX_free(ctx);
            }
            break;
        }
        
        case 4: {
            // Test EVP CAST5 OFB mode
            EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
            if (ctx == NULL) {
                break;
            }
            
            const EVP_CIPHER* cipher = EVP_cast5_ofb();
            if (cipher == NULL) {
                EVP_CIPHER_CTX_free(ctx);
                break;
            }
            
            if (EVP_EncryptInit_ex(ctx, cipher, NULL, cast_key.data(), cast_iv.data())) {
                int out_len1 = 0, out_len2 = 0;
                
                // Process data
                EVP_EncryptUpdate(ctx, ciphertext.data(), &out_len1,
                                 plaintext.data(), plaintext.size());
                
                // Finalize encryption
                EVP_EncryptFinal_ex(ctx, ciphertext.data() + out_len1, &out_len2);
                
                EVP_CIPHER_CTX_free(ctx);
            }
            break;
        }
        
        case 5: {
            // Test CAST_ecb_encrypt directly
            CAST_KEY key;
            
            // Set up CAST key
            CAST_set_key(&key, CAST_KEY_LEN, cast_key.data());
            
            // Process each block individually in ECB mode
            for (size_t i = 0; i < plaintext.size(); i += CAST_BLOCK_SIZE) {
                CAST_ecb_encrypt(plaintext.data() + i, ciphertext.data() + i, 
                                &key, CAST_ENCRYPT);
                CAST_ecb_encrypt(ciphertext.data() + i, decrypted.data() + i, 
                                &key, CAST_DECRYPT);
            }
            break;
        }
    }

    return 0;
}

} // namespace harness_005
