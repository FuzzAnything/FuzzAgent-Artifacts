#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace harness_015 {
/*
 * Fuzzing harness for OpenSSL HMAC (Hash-based Message Authentication Code) operations
 * Targets: HMAC creation, initialization, update, finalization, verification operations
 *          with various hash algorithms (MD5, SHA1, SHA256, SHA512, etc.)
 * Focus on: HMAC_CTX creation, HMAC_Init_ex, HMAC_Update, HMAC_Final, HMAC_CTX_copy,
 *           HMAC_CTX_new, HMAC_CTX_free, HMAC_CTX_reset, HMAC_size, and related HMAC functions
 * 
 * Semantic diversity from existing harnesses:
 * - harness_000 (SSL/TLS APIs) - has basic HMAC test but limited to SHA256 and one-shot HMAC()
 * - harness_001 (BN module)
 * - harness_002 (EVP asymmetric encryption)
 * - harness_003 (PEM/X509 certificate handling)
 * - harness_004 (PKCS12 operations)
 * - harness_005 (AES symmetric encryption)
 * - harness_006 (hash functions and other symmetric encryption) - focuses on hash functions, not HMAC
 * - harness_007 (SSL_accept with memory BIOs)
 * - harness_008 (ECC operations)
 * - harness_009 (SRP protocol)
 * - harness_010 (CMS operations)
 * - harness_011 (CMP protocol)
 * - harness_012 (DH operations)
 * - harness_013 (PEM key parameters)
 * - harness_014 (OCSP operations)
 * 
 * This harness focuses specifically on HMAC operations including:
 * - Full HMAC_CTX lifecycle (new, init, update, final, free, reset, copy)
 * - Multiple hash algorithms (MD5, SHA1, SHA224, SHA256, SHA384, SHA512)
 * - Incremental HMAC_Update for large data
 * - HMAC_CTX_copy functionality for state duplication
 * - Variable key sizes and data inputs
 * - One-shot HMAC() function usage
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

// Get hash algorithm based on selector
static const EVP_MD* get_hash_algorithm(uint8_t selector) {
    switch (selector % 7) {
        case 0: return EVP_md5();
        case 1: return EVP_sha1();
        case 2: return EVP_sha224();
        case 3: return EVP_sha256();
        case 4: return EVP_sha384();
        case 5: return EVP_sha512();
        default: return EVP_sha256(); // Default fallback
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Too small for meaningful HMAC testing
    }
    
    initialize_openssl();
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 5;
    uint8_t hash_selector = fdp.ConsumeIntegral<uint8_t>();
    uint16_t key_size = fdp.ConsumeIntegralInRange<uint16_t>(1, 1024);
    uint16_t data_size = fdp.ConsumeIntegralInRange<uint16_t>(1, 4096);
    
    // Consume variable-size data
    std::vector<uint8_t> key_data = fdp.ConsumeBytes<uint8_t>(key_size);
    std::vector<uint8_t> hmac_data = fdp.ConsumeBytes<uint8_t>(data_size);
    std::vector<uint8_t> extra_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Clear any previous errors
    ERR_clear_error();
    
    const EVP_MD* md = get_hash_algorithm(hash_selector);
    
    switch (test_type) {
        case 0: {
            // Test HMAC_CTX lifecycle with HMAC_Init_ex, HMAC_Update, HMAC_Final
            HMAC_CTX* ctx = HMAC_CTX_new();
            if (ctx == NULL) {
                break;
            }
            
            // Initialize with key and hash algorithm
            // Check if key_data is not empty before using it
            if (!key_data.empty() && HMAC_Init_ex(ctx, key_data.data(), key_data.size(), md, NULL)) {
                // Update with data (possibly in multiple chunks)
                size_t chunk_size = 64;
                size_t processed = 0;
                while (processed < hmac_data.size()) {
                    size_t to_process = std::min(chunk_size, hmac_data.size() - processed);
                    HMAC_Update(ctx, hmac_data.data() + processed, to_process);
                    processed += to_process;
                }
                
                // Finalize and get HMAC result
                unsigned char hmac_result[EVP_MAX_MD_SIZE];
                unsigned int hmac_len = 0;
                HMAC_Final(ctx, hmac_result, &hmac_len);
                
                // Test HMAC_size function
                size_t expected_size = HMAC_size(ctx);
                
                // Verify the HMAC using one-shot HMAC() function
                unsigned char verify_hmac[EVP_MAX_MD_SIZE];
                unsigned int verify_len = 0;
                if (!key_data.empty()) {
                    HMAC(md, key_data.data(), key_data.size(), 
                         hmac_data.data(), hmac_data.size(),
                         verify_hmac, &verify_len);
                }
                
                // Could compare results here, but fuzzing doesn't require validation
            }
            
            // Test HMAC_CTX_reset
            HMAC_CTX_reset(ctx);
            
            // Re-initialize with different parameters
            if (!hmac_data.empty() && hmac_data.size() >= 16) {
                HMAC_Init_ex(ctx, hmac_data.data(), 16, EVP_sha1(), NULL);
                if (!extra_data.empty()) {
                    HMAC_Update(ctx, extra_data.data(), extra_data.size());
                    unsigned char temp_result[EVP_MAX_MD_SIZE];
                    unsigned int temp_len = 0;
                    HMAC_Final(ctx, temp_result, &temp_len);
                }
            }
            
            HMAC_CTX_free(ctx);
            break;
        }
        
        case 1: {
            // Test HMAC_CTX_copy functionality
            HMAC_CTX* src_ctx = HMAC_CTX_new();
            HMAC_CTX* dst_ctx = HMAC_CTX_new();
            
            if (src_ctx != NULL && dst_ctx != NULL) {
                // Initialize source context
                if (!key_data.empty() && HMAC_Init_ex(src_ctx, key_data.data(), key_data.size(), md, NULL)) {
                    // Update source with some data
                    if (!hmac_data.empty()) {
                        HMAC_Update(src_ctx, hmac_data.data(), hmac_data.size());
                    }
                    
                    // Copy source to destination
                    if (HMAC_CTX_copy(dst_ctx, src_ctx)) {
                        // Continue updating both contexts
                        if (!extra_data.empty()) {
                            HMAC_Update(src_ctx, extra_data.data(), extra_data.size());
                            HMAC_Update(dst_ctx, extra_data.data(), extra_data.size());
                            
                            // Finalize both and compare (optional)
                            unsigned char src_result[EVP_MAX_MD_SIZE];
                            unsigned char dst_result[EVP_MAX_MD_SIZE];
                            unsigned int src_len = 0, dst_len = 0;
                            
                            HMAC_Final(src_ctx, src_result, &src_len);
                            HMAC_Final(dst_ctx, dst_result, &dst_len);
                        }
                    }
                }
            }
            
            if (src_ctx != NULL) HMAC_CTX_free(src_ctx);
            if (dst_ctx != NULL) HMAC_CTX_free(dst_ctx);
            break;
        }
        
        case 2: {
            // Test one-shot HMAC() function with various parameters
            for (int i = 0; i < 3; i++) {
                const EVP_MD* test_md = get_hash_algorithm(hash_selector + i);
                
                // Prepare key and data (use different portions of input)
                // Check for empty vectors to avoid division by zero
                if (key_data.empty() || hmac_data.empty()) {
                    continue;
                }
                
                size_t key_offset = i * 32 % key_data.size();
                size_t key_len = std::min((size_t)64, key_data.size() - key_offset);
                size_t data_offset = i * 64 % hmac_data.size();
                size_t data_len = std::min((size_t)128, hmac_data.size() - data_offset);
                
                if (key_len > 0 && data_len > 0) {
                    unsigned char result[EVP_MAX_MD_SIZE];
                    unsigned int result_len = 0;
                    
                    HMAC(test_md, 
                         key_data.data() + key_offset, key_len,
                         hmac_data.data() + data_offset, data_len,
                         result, &result_len);
                }
            }
            break;
        }
        
        case 3: {
            // Test HMAC with empty key or data (edge cases)
            HMAC_CTX* ctx = HMAC_CTX_new();
            if (ctx != NULL) {
                // Test with empty key
                HMAC_Init_ex(ctx, NULL, 0, md, NULL);
                if (!hmac_data.empty()) {
                    HMAC_Update(ctx, hmac_data.data(), hmac_data.size());
                }
                unsigned char result1[EVP_MAX_MD_SIZE];
                unsigned int len1 = 0;
                HMAC_Final(ctx, result1, &len1);
                
                // Reset and test with empty data
                HMAC_CTX_reset(ctx);
                if (!key_data.empty()) {
                    HMAC_Init_ex(ctx, key_data.data(), key_data.size(), md, NULL);
                    // Update with zero-length data
                    HMAC_Update(ctx, NULL, 0);
                    unsigned char result2[EVP_MAX_MD_SIZE];
                    unsigned int len2 = 0;
                    HMAC_Final(ctx, result2, &len2);
                }
                
                HMAC_CTX_free(ctx);
            }
            break;
        }
        
        case 4: {
            // Test HMAC with large data in small chunks
            HMAC_CTX* ctx = HMAC_CTX_new();
            // Check if key_data is not empty before using it
            if (ctx != NULL && !key_data.empty() && HMAC_Init_ex(ctx, key_data.data(), key_data.size(), md, NULL)) {
                // Process data in very small chunks (1-8 bytes)
                size_t pos = 0;
                size_t chunk_counter = 0;
                while (pos < hmac_data.size() && chunk_counter < 100) { // Limit iterations
                    // Access hmac_data[pos] only if pos is within bounds
                    if (pos >= hmac_data.size()) {
                        break;
                    }
                    size_t chunk_size = 1 + (hmac_data[pos] % 8); // 1-8 bytes
                    chunk_size = std::min(chunk_size, hmac_data.size() - pos);
                    
                    HMAC_Update(ctx, hmac_data.data() + pos, chunk_size);
                    pos += chunk_size;
                    chunk_counter++;
                }
                
                // Also process extra data if available
                if (!extra_data.empty()) {
                    HMAC_Update(ctx, extra_data.data(), extra_data.size());
                }
                
                unsigned char result[EVP_MAX_MD_SIZE];
                unsigned int result_len = 0;
                HMAC_Final(ctx, result, &result_len);
                
                HMAC_CTX_free(ctx);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_015
