#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/bn.h>

namespace harness_004 {
/*
 * Fuzzing harness for OpenSSL targeting EVP envelope decryption operations
 * This harness tests: EVP envelope decryption with EVP_OpenInit/EVP_OpenFinal,
 * EVP_PKEY decryption operations, and related cipher context management.
 * Primary target: EVP_OpenFinal with 579 undiscovered branches
 * Secondary targets: EVP_OpenInit, EVP_PKEY_decrypt, EVP_DecryptUpdate, EVP_DecryptFinal_ex
 * This harness is semantically different from previous harnesses focusing on hash functions,
 * X509 certificates, SSL/TLS SRP authentication, and CMP operations.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize OpenSSL
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | 
                        OPENSSL_INIT_ADD_ALL_CIPHERS | 
                        OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
    ERR_clear_error();

    // Need minimum input size for meaningful envelope decryption
    // We need: RSA key size (for encrypted key) + IV + some ciphertext
    if (size < 512) {
        return 0;  // Need enough data for RSA key operations
    }

    FuzzedDataProvider fdp(data, size);

    // Declare all variables at the top to avoid goto jumping over initializations
    EVP_CIPHER_CTX* ctx = NULL;
    EVP_PKEY* priv_key = NULL;
    RSA* rsa = NULL;
    BIGNUM* e = NULL;
    BIGNUM* n = NULL;
    BIGNUM* d = NULL;
    
    // Buffer for decryption output
    unsigned char* out_buf = NULL;
    int out_len = 0;
    
    // Variables for data consumption
    size_t n_data_size = 0;
    size_t d_data_size = 0;
    size_t ek_size = 0;
    size_t iv_size = 0;
    size_t ciphertext_size = 0;
    std::vector<uint8_t> n_data;
    std::vector<uint8_t> d_data;
    std::vector<uint8_t> ek;
    std::vector<uint8_t> iv;
    std::vector<uint8_t> ciphertext;
    
    // Variables for API results
    int open_init_result = 0;
    int update_out_len = 0;
    int open_update_result = 0;
    int final_out_len = 0;
    int open_final_result = 0;
    
    // Variables for additional API testing
    EVP_PKEY_CTX* pkey_ctx = NULL;
    size_t decrypted_len = 0;
    unsigned char* decrypted_buf = NULL;
    
    // Cipher type pointer (declare at top, assign later)
    const EVP_CIPHER* cipher_type = NULL;
    // ==================== CREATE TEST RSA PRIVATE KEY ====================
    // Create a test RSA key for envelope decryption
    // For fuzzing, we create a minimal RSA key using proper APIs
    
    // Create RSA key
    rsa = RSA_new();
    if (rsa == NULL) {
        goto cleanup;
    }
    
    // Create BIGNUMs for RSA key components
    e = BN_new();
    n = BN_new();
    d = BN_new();
    
    if (e == NULL || n == NULL || d == NULL) {
        goto cleanup;
    }
    
    // Set small public exponent (65537 is common)
    if (!BN_set_word(e, 65537)) {
        goto cleanup;
    }
    
    // Generate small RSA key from fuzzer input
    // Consume data for modulus (n) and private exponent (d)
    n_data_size = fdp.ConsumeIntegralInRange<size_t>(64, 256);
    d_data_size = fdp.ConsumeIntegralInRange<size_t>(64, 256);
    
    n_data = fdp.ConsumeBytes<uint8_t>(n_data_size);
    d_data = fdp.ConsumeBytes<uint8_t>(d_data_size);
    
    // Set RSA key components using proper APIs
    if (n_data.size() > 0) {
        BN_bin2bn(n_data.data(), n_data.size(), n);
    }
    if (d_data.size() > 0) {
        BN_bin2bn(d_data.data(), d_data.size(), d);
    }
    
    // Use RSA_set0_key to set key components (transfers ownership)
    if (!RSA_set0_key(rsa, n, e, d)) {
        goto cleanup;
    }
    // RSA now owns n, e, d
    n = NULL;
    e = NULL;
    d = NULL;
    
    // Create EVP_PKEY from RSA key
    priv_key = EVP_PKEY_new();
    if (priv_key == NULL || !EVP_PKEY_assign_RSA(priv_key, rsa)) {
        goto cleanup;
    }
    rsa = NULL;  // EVP_PKEY now owns rsa
    
    // ==================== ENVELOPE DECRYPTION SETUP ====================
    // Create cipher context
    ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL) {
        goto cleanup;
    }
    
    // Choose cipher type (AES-256-CBC for envelope encryption)
    cipher_type = EVP_aes_256_cbc();
    // Consume data for encrypted symmetric key (ek) and IV
    // For RSA decryption, ek size should be RSA key size
    ek_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
    ek = fdp.ConsumeBytes<uint8_t>(ek_size);
    
    // Consume IV (should be EVP_MAX_IV_LENGTH for AES)
    iv_size = EVP_MAX_IV_LENGTH;
    iv = fdp.ConsumeBytes<uint8_t>(iv_size);
    if (iv.size() < iv_size) {
        // Not enough data for IV, use what we have
        iv.resize(iv_size, 0);
    }
    
    // Consume ciphertext data for streaming decryption
    ciphertext_size = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
    ciphertext = fdp.ConsumeBytes<uint8_t>(ciphertext_size);
    
    // Allocate output buffer for decryption
    out_buf = (unsigned char*)OPENSSL_malloc(ciphertext.size() + EVP_MAX_BLOCK_LENGTH);
    if (out_buf == NULL) {
        goto cleanup;
    }
    
    // ==================== EVP OPEN INITIALIZATION ====================
    // Call EVP_OpenInit (PRIMARY TARGET dependency)
    open_init_result = EVP_OpenInit(ctx, cipher_type, 
                                    ek.data(), ek.size(),
                                    iv.data(), priv_key);
    
    // Continue even if EVP_OpenInit fails to explore error paths
    
    // ==================== STREAMING DECRYPTION ====================
    // If we have ciphertext and EVP_OpenInit succeeded, try streaming decryption
    if (open_init_result > 0 && ciphertext.size() > 0) {
        // Call EVP_OpenUpdate (which internally calls EVP_DecryptUpdate)
        open_update_result = EVP_OpenUpdate(ctx, out_buf, &update_out_len,
                                           ciphertext.data(), ciphertext.size());
        
        // Track output length
        if (open_update_result > 0) {
            out_len = update_out_len;
        }
    }
    
    // ==================== FINALIZE DECRYPTION ====================
    // Call EVP_OpenFinal (PRIMARY TARGET)
    if (out_buf != NULL) {
        open_final_result = EVP_OpenFinal(ctx, out_buf + out_len, &final_out_len);
        
        if (open_final_result > 0) {
            out_len += final_out_len;
        }
    }
    
    // ==================== TEST RELATED APIs ====================
    // Also test EVP_PKEY_decrypt directly for additional coverage
    
    pkey_ctx = EVP_PKEY_CTX_new(priv_key, NULL);
    if (pkey_ctx != NULL) {
        // Initialize for decryption
        if (EVP_PKEY_decrypt_init(pkey_ctx) > 0) {
            // First call to determine buffer size
            EVP_PKEY_decrypt(pkey_ctx, NULL, &decrypted_len, 
                           ek.data(), ek.size());
            
            // Allocate buffer and try actual decryption if we have enough data
            if (decrypted_len > 0 && decrypted_len < 4096) {
                decrypted_buf = (unsigned char*)OPENSSL_malloc(decrypted_len);
                if (decrypted_buf != NULL) {
                    // Try decryption (may fail with our test key, but exercises the API)
                    EVP_PKEY_decrypt(pkey_ctx, decrypted_buf, &decrypted_len,
                                   ek.data(), ek.size());
                    OPENSSL_free(decrypted_buf);
                    decrypted_buf = NULL;
                }
            }
        }
        
        EVP_PKEY_CTX_free(pkey_ctx);
        pkey_ctx = NULL;
    }

cleanup:
    // ==================== CLEANUP ====================
    if (decrypted_buf != NULL) {
        OPENSSL_free(decrypted_buf);
    }
    
    if (pkey_ctx != NULL) {
        EVP_PKEY_CTX_free(pkey_ctx);
    }
    
    if (out_buf != NULL) {
        OPENSSL_free(out_buf);
    }
    
    if (ctx != NULL) {
        EVP_CIPHER_CTX_free(ctx);
    }
    
    if (priv_key != NULL) {
        EVP_PKEY_free(priv_key);
    }
    
    // Clean up BIGNUMs if they weren't transferred to RSA
    if (e != NULL) {
        BN_free(e);
    }
    if (n != NULL) {
        BN_free(n);
    }
    if (d != NULL) {
        BN_free(d);
    }
    
    // Clean up RSA if it wasn't transferred to EVP_PKEY
    if (rsa != NULL) {
        RSA_free(rsa);
    }
    
    return 0;
}

} // namespace harness_004
