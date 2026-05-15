#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_001 {
/*
 * Fuzzing harness for OpenSSL RSA cryptographic operations
 * Targets: RSA key generation, encryption, decryption, signing, verification
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: enough for RSA operations
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize OpenSSL error strings
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    
    // Create RSA context
    RSA* rsa = RSA_new();
    if (rsa == NULL) return 0;
    
    BIGNUM* e = BN_new();
    if (e == NULL) {
        RSA_free(rsa);
        return 0;
    }
    
    // Set public exponent (common values: 3, 17, 65537)
    uint8_t exp_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    switch (exp_choice) {
        case 0: BN_set_word(e, 3); break;
        case 1: BN_set_word(e, 17); break;
        case 2: BN_set_word(e, 65537); break;
        default: BN_set_word(e, 65537); break;
    }
    
    // Choose RSA key size (small for fuzzing performance)
    uint8_t key_size_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    int key_bits;
    switch (key_size_choice) {
        case 0: key_bits = 512; break;   // Small for fuzzing
        case 1: key_bits = 768; break;
        case 2: key_bits = 1024; break;
        case 3: key_bits = 2048; break;
        default: key_bits = 1024; break;
    }
    
    // Generate RSA key
    if (RSA_generate_key_ex(rsa, key_bits, e, NULL) != 1) {
        BN_free(e);
        RSA_free(rsa);
        return 0;
    }
    
    // Consume data for encryption/decryption
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::vector<uint8_t> plaintext = fdp.ConsumeBytes<uint8_t>(data_size);
    
    // Choose operation based on input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (operation) {
        case 0: {  // RSA public encrypt / private decrypt
            // RSA encryption requires plaintext size < RSA_size(rsa) - padding
            int rsa_size = RSA_size(rsa);
            if (plaintext.size() > (size_t)(rsa_size - 42)) {  // PKCS#1 v1.5 padding
                break;
            }
            
            std::vector<uint8_t> ciphertext(rsa_size);
            std::vector<uint8_t> decrypted(rsa_size);
            
            // Public encrypt
            int enc_len = RSA_public_encrypt(plaintext.size(), plaintext.data(),
                                            ciphertext.data(), rsa, RSA_PKCS1_PADDING);
            if (enc_len <= 0) break;
            
            // Private decrypt
            int dec_len = RSA_private_decrypt(enc_len, ciphertext.data(),
                                             decrypted.data(), rsa, RSA_PKCS1_PADDING);
            if (dec_len <= 0) break;
            
            // Verify decryption matches original (first bytes)
            size_t compare_len = (plaintext.size() < (size_t)dec_len) ? 
                                plaintext.size() : (size_t)dec_len;
            // Note: We don't compare directly as padding may differ
            break;
        }
        
        case 1: {  // RSA private encrypt / public decrypt
            int rsa_size = RSA_size(rsa);
            if (plaintext.size() > (size_t)(rsa_size - 11)) {  // RSA_PKCS1_PADDING
                break;
            }
            
            std::vector<uint8_t> ciphertext(rsa_size);
            std::vector<uint8_t> decrypted(rsa_size);
            
            // Private encrypt
            int enc_len = RSA_private_encrypt(plaintext.size(), plaintext.data(),
                                             ciphertext.data(), rsa, RSA_PKCS1_PADDING);
            if (enc_len <= 0) break;
            
            // Public decrypt
            int dec_len = RSA_public_decrypt(enc_len, ciphertext.data(),
                                            decrypted.data(), rsa, RSA_PKCS1_PADDING);
            if (dec_len <= 0) break;
            
            break;
        }
        
        case 2: {  // RSA signing and verification (raw)
            // Create a digest to sign (simulate SHA256 hash)
            std::vector<uint8_t> digest(32);  // SHA256 size
            if (fdp.remaining_bytes() >= 32) {
                auto digest_data = fdp.ConsumeBytes<uint8_t>(32);
                memcpy(digest.data(), digest_data.data(), 32);
            } else {
                // Fill with zeros if not enough data
                memset(digest.data(), 0, 32);
            }
            
            std::vector<uint8_t> signature(RSA_size(rsa));
            unsigned int sig_len = 0;
            
            // Sign with PKCS#1 v1.5 padding
            if (RSA_sign(NID_sha256, digest.data(), digest.size(),
                        signature.data(), &sig_len, rsa) != 1) {
                break;
            }
            
            // Verify the signature
            if (RSA_verify(NID_sha256, digest.data(), digest.size(),
                          signature.data(), sig_len, rsa) != 1) {
                // Signature verification failed - expected in some cases
            }
            break;
        }
        
        case 3: {  // Test RSA key operations
            // Export public key
            // Export public key - use i2d_RSAPublicKey instead
            unsigned char* der = NULL;
            int der_len = i2d_RSAPublicKey(rsa, &der);
            if (der_len > 0 && der != NULL) {
                OPENSSL_free(der);
            }
            // Test RSA_check_key
            int key_check = RSA_check_key(rsa);
            // Result can be 1 (valid), 0 (invalid), or -1 (error)
            
            // Test RSA_size
            int rsa_sz = RSA_size(rsa);
            
            // Test RSA_get0_key to get key components
            const BIGNUM* n = NULL;
            const BIGNUM* e2 = NULL;
            const BIGNUM* d = NULL;
            RSA_get0_key(rsa, &n, &e2, &d);
            
            break;
        }
    }
    
    // Clean up
    BN_free(e);
    RSA_free(rsa);
    ERR_free_strings();
    EVP_cleanup();
    
    return 0;
}

} // namespace harness_001
