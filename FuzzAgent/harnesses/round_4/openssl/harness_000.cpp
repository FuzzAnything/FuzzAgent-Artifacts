#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_000 {
/*
 * Fuzzing harness for OpenSSL AES cryptographic operations
 * Targets: AES encryption/decryption in various modes (ECB, CBC, CFB, OFB)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: enough for key (32 bytes), IV (16 bytes), and some data
    if (size < 48) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize OpenSSL error strings
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    
    // Consume AES key - can be 128, 192, or 256 bits
    size_t key_size_bytes;
    uint8_t key_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    switch (key_choice) {
        case 0: key_size_bytes = 16; break;  // 128-bit
        case 1: key_size_bytes = 24; break;  // 192-bit  
        case 2: key_size_bytes = 32; break;  // 256-bit
        default: key_size_bytes = 16; break;
    }
    
    std::vector<uint8_t> key = fdp.ConsumeBytes<uint8_t>(key_size_bytes);
    if (key.size() != key_size_bytes) return 0;
    
    // Consume IV for modes that need it (CBC, CFB, OFB)
    std::vector<uint8_t> iv = fdp.ConsumeBytes<uint8_t>(16);  // AES block size
    
    // Consume data to encrypt/decrypt
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    std::vector<uint8_t> plaintext = fdp.ConsumeBytes<uint8_t>(data_size);
    
    // Set up AES key structures
    AES_KEY encrypt_key, decrypt_key;
    
    // Test AES_set_encrypt_key and AES_set_decrypt_key
    if (AES_set_encrypt_key(key.data(), key_size_bytes * 8, &encrypt_key) < 0) {
        return 0;
    }
    if (AES_set_decrypt_key(key.data(), key_size_bytes * 8, &decrypt_key) < 0) {
        return 0;
    }
    
    // Choose AES mode based on input
    uint8_t mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Prepare output buffers
    std::vector<uint8_t> ciphertext(plaintext.size() + AES_BLOCK_SIZE);
    std::vector<uint8_t> decrypted(plaintext.size() + AES_BLOCK_SIZE);
    
    switch (mode) {
        case 0: {  // ECB mode
            // ECB encrypt/decrypt
            size_t blocks = (plaintext.size() + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE;
            for (size_t i = 0; i < blocks; i++) {
                size_t offset = i * AES_BLOCK_SIZE;
                size_t block_len = (offset + AES_BLOCK_SIZE <= plaintext.size()) ? 
                                  AES_BLOCK_SIZE : plaintext.size() - offset;
                
                uint8_t in_block[AES_BLOCK_SIZE] = {0};
                uint8_t out_block[AES_BLOCK_SIZE] = {0};
                memcpy(in_block, plaintext.data() + offset, block_len);
                
                AES_encrypt(in_block, out_block, &encrypt_key);
                memcpy(ciphertext.data() + offset, out_block, AES_BLOCK_SIZE);
                
                AES_decrypt(out_block, in_block, &decrypt_key);
                memcpy(decrypted.data() + offset, in_block, block_len);
            }
            break;
        }
        
        case 1: {  // CBC mode
            if (iv.size() < 16) break;
            
            unsigned char ivec[16];
            memcpy(ivec, iv.data(), 16);
            
            AES_cbc_encrypt(plaintext.data(), ciphertext.data(), 
                          plaintext.size(), &encrypt_key, ivec, AES_ENCRYPT);
            
            memcpy(ivec, iv.data(), 16);  // Reset IV for decryption
            AES_cbc_encrypt(ciphertext.data(), decrypted.data(),
                          plaintext.size(), &decrypt_key, ivec, AES_DECRYPT);
            break;
        }
        
        case 2: {  // CFB128 mode
            if (iv.size() < 16) break;
            
            unsigned char ivec[16];
            int num = 0;
            memcpy(ivec, iv.data(), 16);
            
            AES_cfb128_encrypt(plaintext.data(), ciphertext.data(),
                             plaintext.size(), &encrypt_key, ivec, &num, AES_ENCRYPT);
            
            memcpy(ivec, iv.data(), 16);
            num = 0;
            AES_cfb128_encrypt(ciphertext.data(), decrypted.data(),
                             plaintext.size(), &decrypt_key, ivec, &num, AES_DECRYPT);
            break;
        }
        
        case 3: {  // OFB mode
            if (iv.size() < 16) break;
            
            unsigned char ivec[16];
            int num = 0;
            memcpy(ivec, iv.data(), 16);
            
            AES_ofb128_encrypt(plaintext.data(), ciphertext.data(),
                             plaintext.size(), &encrypt_key, ivec, &num);
            
            // OFB is symmetric - encryption and decryption use same function
            memcpy(ivec, iv.data(), 16);
            num = 0;
            AES_ofb128_encrypt(ciphertext.data(), decrypted.data(),
                             plaintext.size(), &encrypt_key, ivec, &num);
            break;
        }
    }
    
    // Clean up
    ERR_free_strings();
    EVP_cleanup();
    
    return 0;
}

} // namespace harness_000
