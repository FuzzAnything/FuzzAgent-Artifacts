#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace harness_002 {
/*
 * Fuzzing harness for OpenSSL EVP asymmetric encryption operations
 * Targets: EVP_PKEY encryption/decryption, signing/verification, RSA key operations
 * Specifically targets EVP asymmetric encryption with 399 undiscovered branches
 */


// Initialize OpenSSL once
static bool openssl_initialized = false;

static void initialize_openssl() {
    if (!openssl_initialized) {
        OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | 
                           OPENSSL_INIT_ADD_ALL_CIPHERS |
                           OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
        ERR_clear_error();
        openssl_initialized = true;
    }
}

// Generate RSA key for testing
static EVP_PKEY* generate_rsa_key(FuzzedDataProvider& fdp) {
    EVP_PKEY* pkey = NULL;
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    
    if (ctx == NULL) {
        return NULL;
    }
    
    if (EVP_PKEY_keygen_init(ctx) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return NULL;
    }
    
    // Set key size based on fuzzer input
    int key_bits = fdp.ConsumeIntegralInRange<int>(512, 4096);
    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, key_bits) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return NULL;
    }
    
    // Generate key
    if (EVP_PKEY_keygen(ctx, &pkey) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return NULL;
    }
    
    EVP_PKEY_CTX_free(ctx);
    return pkey;
}

// Test EVP_PKEY encryption/decryption
static void test_evp_encrypt_decrypt(EVP_PKEY* pkey, FuzzedDataProvider& fdp) {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(pkey, NULL);
    if (ctx == NULL) {
        return;
    }
    
    // Initialize encryption
    if (EVP_PKEY_encrypt_init(ctx) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return;
    }
    
    // Set padding mode based on fuzzer input
    int padding = fdp.ConsumeIntegralInRange<int>(RSA_PKCS1_PADDING, RSA_PKCS1_OAEP_PADDING);
    EVP_PKEY_CTX_set_rsa_padding(ctx, padding);
    
    // Get data to encrypt
    size_t data_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::vector<uint8_t> plaintext = fdp.ConsumeBytes<uint8_t>(data_len);
    
    if (plaintext.empty()) {
        EVP_PKEY_CTX_free(ctx);
        return;
    }
    
    // Determine output buffer size
    size_t outlen;
    if (EVP_PKEY_encrypt(ctx, NULL, &outlen, plaintext.data(), plaintext.size()) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return;
    }
    
    // Allocate output buffer and encrypt
    std::vector<uint8_t> ciphertext(outlen);
    if (EVP_PKEY_encrypt(ctx, ciphertext.data(), &outlen, plaintext.data(), plaintext.size()) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return;
    }
    
    ciphertext.resize(outlen);
    
    // Now try decryption
    EVP_PKEY_CTX_free(ctx);
    ctx = EVP_PKEY_CTX_new(pkey, NULL);
    if (ctx == NULL) {
        return;
    }
    
    if (EVP_PKEY_decrypt_init(ctx) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return;
    }
    
    EVP_PKEY_CTX_set_rsa_padding(ctx, padding);
    
    // Determine decrypted buffer size
    size_t declen;
    if (EVP_PKEY_decrypt(ctx, NULL, &declen, ciphertext.data(), ciphertext.size()) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return;
    }
    
    // Allocate buffer and decrypt
    std::vector<uint8_t> decrypted(declen);
    if (EVP_PKEY_decrypt(ctx, decrypted.data(), &declen, ciphertext.data(), ciphertext.size()) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return;
    }
    
    decrypted.resize(declen);
    
    EVP_PKEY_CTX_free(ctx);
}

// Test EVP_PKEY signing/verification
static void test_evp_sign_verify(EVP_PKEY* pkey, FuzzedDataProvider& fdp) {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(pkey, NULL);
    if (ctx == NULL) {
        return;
    }
    
    // Initialize signing
    if (EVP_PKEY_sign_init(ctx) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return;
    }
    
    // Set digest type
    const EVP_MD* md = EVP_sha256();
    if (EVP_PKEY_CTX_set_signature_md(ctx, md) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return;
    }
    
    // Set padding
    EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);
    
    // Get data to sign
    size_t data_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::vector<uint8_t> data = fdp.ConsumeBytes<uint8_t>(data_len);
    
    if (data.empty()) {
        EVP_PKEY_CTX_free(ctx);
        return;
    }
    
    // Determine signature size
    size_t siglen;
    if (EVP_PKEY_sign(ctx, NULL, &siglen, data.data(), data.size()) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return;
    }
    
    // Allocate signature buffer and sign
    std::vector<uint8_t> signature(siglen);
    if (EVP_PKEY_sign(ctx, signature.data(), &siglen, data.data(), data.size()) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return;
    }
    
    signature.resize(siglen);
    
    // Now verify
    EVP_PKEY_CTX_free(ctx);
    ctx = EVP_PKEY_CTX_new(pkey, NULL);
    if (ctx == NULL) {
        return;
    }
    
    if (EVP_PKEY_verify_init(ctx) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return;
    }
    
    if (EVP_PKEY_CTX_set_signature_md(ctx, md) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return;
    }
    
    EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);
    
    // Verify the signature
    EVP_PKEY_verify(ctx, signature.data(), signature.size(), data.data(), data.size());
    
    EVP_PKEY_CTX_free(ctx);
}
// Test RSA-specific operations
static void test_rsa_operations(EVP_PKEY* pkey, FuzzedDataProvider& fdp) {
    const RSA* rsa = EVP_PKEY_get0_RSA(pkey);
    if (rsa == NULL) {
        return;
    }
    
    // Test RSA public encrypt/decrypt
    size_t data_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
    std::vector<uint8_t> plaintext = fdp.ConsumeBytes<uint8_t>(data_len);
    
    if (plaintext.empty()) {
        return;
    }
    
    int rsa_size = RSA_size(rsa);
    std::vector<uint8_t> ciphertext(rsa_size);
    std::vector<uint8_t> decrypted(rsa_size);
    
    // Try different padding modes
    int padding_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
    int padding;
    switch (padding_mode % 4) {
        case 0: padding = RSA_PKCS1_PADDING; break;
        case 1: padding = RSA_X931_PADDING; break;
        case 2: padding = RSA_NO_PADDING; break;
        case 3: padding = RSA_PKCS1_OAEP_PADDING; break;
        default: padding = RSA_PKCS1_PADDING;
    }
    
    // Public encrypt - need non-const RSA for these operations
    RSA* rsa_nonconst = const_cast<RSA*>(rsa);
    int encrypt_len = RSA_public_encrypt(plaintext.size(), plaintext.data(), 
                                         ciphertext.data(), rsa_nonconst, padding);
    
    if (encrypt_len > 0) {
        // Private decrypt
        RSA_private_decrypt(encrypt_len, ciphertext.data(), 
                           decrypted.data(), rsa_nonconst, padding);
    }
    
    // Test RSA sign/verify
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len = sizeof(hash);
    
    // Generate a hash
    if (fdp.remaining_bytes() > 32) {
        std::vector<uint8_t> hash_data = fdp.ConsumeBytes<uint8_t>(32);
        memcpy(hash, hash_data.data(), std::min(hash_data.size(), sizeof(hash)));
        hash_len = std::min(hash_data.size(), sizeof(hash));
        
        std::vector<unsigned char> sig(RSA_size(rsa));
        unsigned int sig_len;
        
        // Sign
        if (RSA_sign(NID_sha256, hash, hash_len, sig.data(), &sig_len, rsa_nonconst) == 1) {
            // Verify
            RSA_verify(NID_sha256, hash, hash_len, sig.data(), sig_len, rsa_nonconst);
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) {
        return 0;  // Too small for meaningful testing
    }
    
    initialize_openssl();
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Generate RSA key for testing
    EVP_PKEY* pkey = generate_rsa_key(fdp);
    if (pkey == NULL) {
        return 0;
    }
    
    // Clear any previous errors
    ERR_clear_error();
    
    switch (test_type) {
        case 0:
            // Test EVP_PKEY encryption/decryption
            test_evp_encrypt_decrypt(pkey, fdp);
            break;
            
        case 1:
            // Test EVP_PKEY signing/verification
            test_evp_sign_verify(pkey, fdp);
            break;
            
        case 2:
            // Test RSA-specific operations
            test_rsa_operations(pkey, fdp);
            break;
            
        case 3:
            // Test multiple operations
            test_evp_encrypt_decrypt(pkey, fdp);
            if (fdp.remaining_bytes() > 32) {
                test_evp_sign_verify(pkey, fdp);
            }
            if (fdp.remaining_bytes() > 32) {
                test_rsa_operations(pkey, fdp);
            }
            break;
    }
    
    // Cleanup
    EVP_PKEY_free(pkey);
    
    // Clear errors at the end
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_002
