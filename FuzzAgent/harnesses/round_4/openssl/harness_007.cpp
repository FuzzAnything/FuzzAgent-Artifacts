#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/des.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

namespace harness_007 {
/*
 * Fuzzing harness for OpenSSL DES symmetric encryption operations
 * Targets: DES encryption/decryption with various modes (ECB, CBC, CFB, OFB, PCBC, XCBC)
 * Coverage goal: Exercise DES module with 0% coverage across 20+ DES-related files
 * 
 * Target APIs:
 * - DES_set_key (Required Helper: Key initialization)
 * - DES_ecb_encrypt (Target: ECB mode encryption)
 * - DES_cbc_encrypt (Target: CBC mode encryption)  
 * - DES_ncbc_encrypt (Target: Modified CBC mode)
 * - DES_cfb64_encrypt (Target: CFB mode encryption)
 * - DES_ofb64_encrypt (Target: OFB mode encryption)
 * - DES_pcbc_encrypt (Target: PCBC mode encryption)
 * - DES_xcbc_encrypt (Target: XCBC mode encryption)
 * - DES_ede3_cbc_encrypt (Target: Triple DES CBC mode)
 * - DES_string_to_key (Required Helper: Key derivation)
 * 
 * Required helper APIs:
 * - DES_set_odd_parity (Key parity adjustment)
 * - DES_is_weak_key (Weak key detection)
 * - DES_random_key (Random key generation)
 * 
 * Invocation sequence:
 * 1. Generate random keys or use DES_string_to_key for key derivation
 * 2. Call DES_set_key to initialize key schedules
 * 3. Test various encryption modes (ECB, CBC, CFB, OFB, PCBC, XCBC)
 * 4. Test both encryption and decryption operations
 * 5. Test Triple DES variants (DES_ede3_cbc_encrypt)
 * 6. Include cleanup of any allocated resources
 * 
 * Differentiation from existing harnesses:
 * - harness_000.cpp: AES cryptographic operations
 * - harness_001.cpp: RSA cryptographic operations  
 * - harness_002.cpp: SSL/TLS protocol stack
 * - harness_003.cpp: CMP/CRMF certificate management
 * - harness_004.cpp: X.509 certificate parsing and CMS operations
 * - harness_005.cpp: EVP elliptic curve cryptography operations
 * - harness_006.cpp: PKCS12 certificate and private key container operations
 * - harness_007.cpp: DES symmetric encryption operations (NEW)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: enough for keys, IV, and some data
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize OpenSSL libraries
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    
    // 1. Key generation and setup
    DES_cblock key1, key2, key3;
    DES_cblock iv, iv2, iv3;
    DES_key_schedule ks1, ks2, ks3;
    
    // Consume keys from fuzzed input
    std::vector<uint8_t> key1_data = fdp.ConsumeBytes<uint8_t>(8);
    std::vector<uint8_t> key2_data = fdp.ConsumeBytes<uint8_t>(8);
    std::vector<uint8_t> key3_data = fdp.ConsumeBytes<uint8_t>(8);
    
    // Copy key data to DES_cblock
    if (key1_data.size() == 8) memcpy(key1, key1_data.data(), 8);
    if (key2_data.size() == 8) memcpy(key2, key2_data.data(), 8);
    if (key3_data.size() == 8) memcpy(key3, key3_data.data(), 8);
    
    // Consume IVs from fuzzed input
    std::vector<uint8_t> iv_data = fdp.ConsumeBytes<uint8_t>(8);
    std::vector<uint8_t> iv2_data = fdp.ConsumeBytes<uint8_t>(8);
    std::vector<uint8_t> iv3_data = fdp.ConsumeBytes<uint8_t>(8);
    
    if (iv_data.size() == 8) memcpy(iv, iv_data.data(), 8);
    if (iv2_data.size() == 8) memcpy(iv2, iv2_data.data(), 8);
    if (iv3_data.size() == 8) memcpy(iv3, iv3_data.data(), 8);
    
    // Test key parity functions
    DES_set_odd_parity(&key1);
    DES_set_odd_parity(&key2);
    DES_set_odd_parity(&key3);
    
    // Check for weak keys
    int is_weak_key1 = DES_is_weak_key(&key1);
    int is_weak_key2 = DES_is_weak_key(&key2);
    int is_weak_key3 = DES_is_weak_key(&key3);
    
    // Set up key schedules
    int key_setup_result1 = DES_set_key(&key1, &ks1);
    int key_setup_result2 = DES_set_key(&key2, &ks2);
    int key_setup_result3 = DES_set_key(&key3, &ks3);
    
    // If key setup fails, generate random keys
    if (key_setup_result1 != 0) {
        DES_random_key(&key1);
        DES_set_key(&key1, &ks1);
    }
    if (key_setup_result2 != 0) {
        DES_random_key(&key2);
        DES_set_key(&key2, &ks2);
    }
    if (key_setup_result3 != 0) {
        DES_random_key(&key3);
        DES_set_key(&key3, &ks3);
    }
    
    // 2. Test DES_string_to_key
    std::string password = fdp.ConsumeRandomLengthString(32);
    DES_cblock derived_key;
    DES_string_to_key(password.c_str(), &derived_key);
    DES_set_odd_parity(&derived_key);
    
    DES_key_schedule derived_ks;
    DES_set_key(&derived_key, &derived_ks);
    
    // 3. Prepare data for encryption
    // Consume remaining data for plaintext
    size_t data_length = fdp.ConsumeIntegralInRange<size_t>(8, 256);
    if (data_length % 8 != 0) {
        data_length = (data_length / 8 + 1) * 8; // Make multiple of 8 for DES
    }
    
    std::vector<uint8_t> plaintext = fdp.ConsumeBytes<uint8_t>(data_length);
    if (plaintext.size() < 8) {
        // Not enough data, create minimal buffer
        plaintext.resize(8);
        for (size_t i = 0; i < 8; i++) {
            plaintext[i] = fdp.ConsumeIntegral<uint8_t>();
        }
    }
    
    // Ensure plaintext is multiple of 8 bytes
    if (plaintext.size() % 8 != 0) {
        size_t new_size = (plaintext.size() / 8 + 1) * 8;
        plaintext.resize(new_size, 0);
    }
    
    // Allocate output buffers
    std::vector<uint8_t> ciphertext(plaintext.size(), 0);
    std::vector<uint8_t> decrypted(plaintext.size(), 0);
    
    // 4. Test various encryption modes
    
    // Test ECB mode (encryption and decryption)
    if (plaintext.size() >= 8) {
        DES_ecb_encrypt((const_DES_cblock*)plaintext.data(), (DES_cblock*)ciphertext.data(), &ks1, DES_ENCRYPT);
        DES_ecb_encrypt((const_DES_cblock*)ciphertext.data(), (DES_cblock*)decrypted.data(), &ks1, DES_DECRYPT);
    }
    
    // Test CBC mode
    DES_cbc_encrypt(plaintext.data(), ciphertext.data(), plaintext.size(), &ks1, &iv, DES_ENCRYPT);
    DES_cbc_encrypt(ciphertext.data(), decrypted.data(), ciphertext.size(), &ks1, &iv, DES_DECRYPT);
    
    // Test NCBC mode (updates IV)
    DES_cblock iv_copy1;
    memcpy(iv_copy1, iv, 8);
    DES_ncbc_encrypt(plaintext.data(), ciphertext.data(), plaintext.size(), &ks1, &iv_copy1, DES_ENCRYPT);
    
    DES_cblock iv_copy2;
    memcpy(iv_copy2, iv_copy1, 8);
    DES_ncbc_encrypt(ciphertext.data(), decrypted.data(), ciphertext.size(), &ks1, &iv_copy2, DES_DECRYPT);
    
    // Test PCBC mode
    DES_pcbc_encrypt(plaintext.data(), ciphertext.data(), plaintext.size(), &ks1, &iv, DES_ENCRYPT);
    DES_pcbc_encrypt(ciphertext.data(), decrypted.data(), ciphertext.size(), &ks1, &iv, DES_DECRYPT);
    
    // Test CFB64 mode
    int num1 = 0;
    DES_cfb64_encrypt(plaintext.data(), ciphertext.data(), plaintext.size(), &ks1, &iv, &num1, DES_ENCRYPT);
    
    int num2 = 0;
    DES_cfb64_encrypt(ciphertext.data(), decrypted.data(), ciphertext.size(), &ks1, &iv, &num2, DES_DECRYPT);
    
    // Test OFB64 mode
    int num3 = 0;
    DES_ofb64_encrypt(plaintext.data(), ciphertext.data(), plaintext.size(), &ks1, &iv, &num3);
    
    int num4 = 0;
    DES_ofb64_encrypt(ciphertext.data(), decrypted.data(), ciphertext.size(), &ks1, &iv, &num4);
    
    // Test XCBC mode (requires additional keying material)
    DES_cblock inw, outw;
    std::vector<uint8_t> inw_data = fdp.ConsumeBytes<uint8_t>(8);
    std::vector<uint8_t> outw_data = fdp.ConsumeBytes<uint8_t>(8);
    
    if (inw_data.size() == 8) memcpy(inw, inw_data.data(), 8);
    if (outw_data.size() == 8) memcpy(outw, outw_data.data(), 8);
    
    DES_xcbc_encrypt(plaintext.data(), ciphertext.data(), plaintext.size(), &ks1, &iv, &inw, &outw, DES_ENCRYPT);
    DES_xcbc_encrypt(ciphertext.data(), decrypted.data(), ciphertext.size(), &ks1, &iv, &inw, &outw, DES_DECRYPT);
    
    // 5. Test Triple DES (3DES) variants
    
    // Test 3DES CBC mode
    DES_ede3_cbc_encrypt(plaintext.data(), ciphertext.data(), plaintext.size(), 
                        &ks1, &ks2, &ks3, &iv, DES_ENCRYPT);
    
    DES_ede3_cbc_encrypt(ciphertext.data(), decrypted.data(), ciphertext.size(), 
                        &ks1, &ks2, &ks3, &iv, DES_DECRYPT);
    
    // Test 3DES CFB64 mode
    int num5 = 0;
    DES_ede3_cfb64_encrypt(plaintext.data(), ciphertext.data(), plaintext.size(), 
                          &ks1, &ks2, &ks3, &iv, &num5, DES_ENCRYPT);
    
    int num6 = 0;
    DES_ede3_cfb64_encrypt(ciphertext.data(), decrypted.data(), ciphertext.size(), 
                          &ks1, &ks2, &ks3, &iv, &num6, DES_DECRYPT);
    
    // Test 3DES OFB64 mode
    int num7 = 0;
    DES_ede3_ofb64_encrypt(plaintext.data(), ciphertext.data(), plaintext.size(), 
                          &ks1, &ks2, &ks3, &iv, &num7);
    
    int num8 = 0;
    DES_ede3_ofb64_encrypt(ciphertext.data(), decrypted.data(), ciphertext.size(), 
                          &ks1, &ks2, &ks3, &iv, &num8);
    
    // 6. Test CFB mode with different bit sizes
    DES_cfb_encrypt(plaintext.data(), ciphertext.data(), 1, plaintext.size(), &ks1, &iv, DES_ENCRYPT);
    DES_cfb_encrypt(plaintext.data(), ciphertext.data(), 8, plaintext.size(), &ks1, &iv, DES_ENCRYPT);
    
    // 7. Test OFB mode with different bit sizes
    DES_ofb_encrypt(plaintext.data(), ciphertext.data(), 1, plaintext.size(), &ks1, &iv);
    DES_ofb_encrypt(plaintext.data(), ciphertext.data(), 8, plaintext.size(), &ks1, &iv);
    
    // 8. Test 3DES CFB mode with different bit sizes
    DES_ede3_cfb_encrypt(plaintext.data(), ciphertext.data(), 1, plaintext.size(), 
                        &ks1, &ks2, &ks3, &iv, DES_ENCRYPT);
    DES_ede3_cfb_encrypt(plaintext.data(), ciphertext.data(), 8, plaintext.size(), 
                        &ks1, &ks2, &ks3, &iv, DES_ENCRYPT);
    
    // 9. Test DES_string_to_2keys
    std::string password2 = fdp.ConsumeRandomLengthString(32);
    DES_cblock derived_key1, derived_key2;
    DES_string_to_2keys(password2.c_str(), &derived_key1, &derived_key2);
    
    DES_set_odd_parity(&derived_key1);
    DES_set_odd_parity(&derived_key2);
    
    DES_key_schedule derived_ks1, derived_ks2;
    DES_set_key(&derived_key1, &derived_ks1);
    DES_set_key(&derived_key2, &derived_ks2);
    
    // 10. Test with derived keys
    if (plaintext.size() >= 8) {
        DES_ecb_encrypt((const_DES_cblock*)plaintext.data(), (DES_cblock*)ciphertext.data(), &derived_ks1, DES_ENCRYPT);
    }
    
    // 11. Cleanup (OpenSSL DES functions don't require explicit cleanup for key schedules)
    // Clear sensitive data
    memset(key1, 0, 8);
    memset(key2, 0, 8);
    memset(key3, 0, 8);
    memset(iv, 0, 8);
    memset(derived_key, 0, 8);
    memset(derived_key1, 0, 8);
    memset(derived_key2, 0, 8);
    
    return 0;
}

} // namespace harness_007
