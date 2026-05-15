#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

namespace harness_010 {
/*
 * Fuzzing harness for OpenSSL hash functions (SHA family)
 * Targets: SHA-1, SHA-2 family (SHA224, SHA256, SHA384, SHA512), and SHA-3 algorithms
 * Coverage goal: Exercise hash functions with 0% coverage for SHA family APIs
 * 
 * Target APIs:
 * Low-level APIs:
 * - SHA1_Init, SHA1_Update, SHA1_Final
 * - SHA224_Init, SHA224_Update, SHA224_Final
 * - SHA256_Init, SHA256_Update, SHA256_Final
 * - SHA384_Init, SHA384_Update, SHA384_Final
 * - SHA512_Init, SHA512_Update, SHA512_Final
 * 
 * EVP interface APIs:
 * - EVP_MD_CTX_new, EVP_MD_CTX_free
 * - EVP_DigestInit_ex, EVP_DigestUpdate, EVP_DigestFinal_ex
 * - EVP_sha1(), EVP_sha224(), EVP_sha256(), EVP_sha384(), EVP_sha512()
 * - EVP_sha3_224(), EVP_sha3_256(), EVP_sha3_384(), EVP_sha3_512()
 * 
 * Invocation sequence:
 * 1. Create context using EVP_MD_CTX_new()
 * 2. Initialize with specific hash algorithm using EVP_DigestInit_ex() with appropriate EVP_MD*
 * 3. Process data in chunks using EVP_DigestUpdate()
 * 4. Finalize hash using EVP_DigestFinal_ex()
 * 5. Clean up with EVP_MD_CTX_free()
 * 6. Also test low-level SHA*_Init/Update/Final APIs directly
 * 
 * Differentiation from existing harnesses:
 * - harness_000.cpp: AES cryptographic operations
 * - harness_001.cpp: RSA cryptographic operations
 * - harness_002.cpp: SSL/TLS protocol stack
 * - harness_003.cpp: CMP/CRMF certificate management
 * - harness_004.cpp: X.509 certificate parsing and CMS operations
 * - harness_005.cpp: PKCS12 parsing and encryption
 * - harness_006.cpp: DES and 3DES cryptographic operations
 * - harness_007.cpp: EC (Elliptic Curve) cryptographic operations
 * - harness_008.cpp: PEM format parsing of cryptographic keys
 * - harness_009.cpp: Time-Stamp Protocol (RFC 3161) operations
 * - harness_010.cpp: Hash functions (SHA family) (NEW)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough data for algorithm selection and some data to hash
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize OpenSSL error strings (optional but helpful)
    ERR_load_crypto_strings();
    OpenSSL_add_all_digests();
    
    // Consume algorithm selection (0-8 for different hash algorithms)
    uint8_t algo_choice = fdp.ConsumeIntegral<uint8_t>() % 9;
    
    // Consume chunk size strategy before consuming data
    size_t chunk_size = 0;
    if (fdp.remaining_bytes() > 0) {
        // Use 1 byte to determine chunking strategy (0=no chunking, 1-255=chunk size hint)
        uint8_t chunk_hint = fdp.ConsumeIntegral<uint8_t>();
        chunk_size = (chunk_hint == 0) ? 0 : (chunk_hint % 256);
    }
    
    // Consume data to hash (remaining bytes)
    std::vector<uint8_t> input_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (input_data.empty()) {
        // No data to process, return early
        return 0;
    }
    
    const uint8_t* data_ptr = input_data.data();
    size_t data_len = input_data.size();
    
    // Buffer for hash output (size for largest hash - SHA512)
    unsigned char hash_output[EVP_MAX_MD_SIZE];
    unsigned int hash_len = 0;
    
    // Test EVP interface (higher-level API)
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (ctx == NULL) {
        return 0;
    }

    
    const EVP_MD* md = NULL;
    
    // Select hash algorithm based on fuzzer input
    switch (algo_choice) {
        case 0:
            md = EVP_sha1();
            break;
        case 1:
            md = EVP_sha224();
            break;
        case 2:
            md = EVP_sha256();
            break;
        case 3:
            md = EVP_sha384();
            break;
        case 4:
            md = EVP_sha512();
            break;
        case 5:
            md = EVP_sha3_224();
            break;
        case 6:
            md = EVP_sha3_256();
            break;
        case 7:
            md = EVP_sha3_384();
            break;
        case 8:
            md = EVP_sha3_512();
            break;
    }
    
    if (md != NULL) {
        // Initialize digest context with selected algorithm
        if (EVP_DigestInit_ex(ctx, md, NULL) == 1) {
            // Process data in one or more chunks
            // Determine chunk strategy based on input data
            if (data_len > 64) {
                // Process in multiple chunks
                size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 256) % data_len;
                if (chunk_size == 0) chunk_size = 1;
                
                size_t processed = 0;
                while (processed < data_len) {
                    size_t remaining = data_len - processed;
                    size_t current_chunk = (chunk_size < remaining) ? chunk_size : remaining;
                    
                    EVP_DigestUpdate(ctx, data_ptr + processed, current_chunk);
                    processed += current_chunk;
                }
            } else {
                // Process all data at once
                EVP_DigestUpdate(ctx, data_ptr, data_len);
            }
            
            // Finalize hash
            EVP_DigestFinal_ex(ctx, hash_output, &hash_len);
        }
    }
    
    // Clean up EVP context
    EVP_MD_CTX_free(ctx);
    
    // Now test low-level SHA APIs directly
    // We'll use a different portion of algorithm choice to select which low-level API to test
    uint8_t low_level_algo = (algo_choice + 1) % 5;  // 0-4 for SHA1, SHA224, SHA256, SHA384, SHA512
    
    switch (low_level_algo) {
        case 0: {  // SHA-1
            SHA_CTX sha1_ctx;
            if (SHA1_Init(&sha1_ctx) == 1) {
                SHA1_Update(&sha1_ctx, data_ptr, data_len);
                SHA1_Final(hash_output, &sha1_ctx);
            }
            break;
        }
        case 1: {  // SHA-224
            SHA256_CTX sha224_ctx;
            if (SHA224_Init(&sha224_ctx) == 1) {
                SHA224_Update(&sha224_ctx, data_ptr, data_len);
                SHA224_Final(hash_output, &sha224_ctx);
            }
            break;
        }
        case 2: {  // SHA-256
            SHA256_CTX sha256_ctx;
            if (SHA256_Init(&sha256_ctx) == 1) {
                SHA256_Update(&sha256_ctx, data_ptr, data_len);
                SHA256_Final(hash_output, &sha256_ctx);
            }
            break;
        }
        case 3: {  // SHA-384
            SHA512_CTX sha384_ctx;
            if (SHA384_Init(&sha384_ctx) == 1) {
                SHA384_Update(&sha384_ctx, data_ptr, data_len);
                SHA384_Final(hash_output, &sha384_ctx);
            }
            break;
        }
        case 4: {  // SHA-512
            SHA512_CTX sha512_ctx;
            if (SHA512_Init(&sha512_ctx) == 1) {
                SHA512_Update(&sha512_ctx, data_ptr, data_len);
                SHA512_Final(hash_output, &sha512_ctx);
            }
            break;
        }
    }
    
    // Test one-shot hash functions (simpler API)
    if (data_len > 0) {
        unsigned char* result = NULL;
        
        // Use different one-shot function based on algorithm choice
        uint8_t oneshot_algo = algo_choice % 3;
        switch (oneshot_algo) {
            case 0:
                result = SHA1(data_ptr, data_len, hash_output);
                break;
            case 1:
                result = SHA256(data_ptr, data_len, hash_output);
                break;
            case 2:
                result = SHA512(data_ptr, data_len, hash_output);
                break;
        }
        // Result is in hash_output regardless of return value
    }
    
    // Test with zero-length input (edge case)
    if (data_len > 0) {
        SHA_CTX sha1_ctx_zero;
        if (SHA1_Init(&sha1_ctx_zero) == 1) {
            SHA1_Update(&sha1_ctx_zero, NULL, 0);  // Zero-length update
            SHA1_Final(hash_output, &sha1_ctx_zero);
        }
    }
    
    // Test with small chunk sizes (edge case for buffering)
    if (data_len >= 3) {
        SHA256_CTX sha256_ctx_chunk;
        if (SHA256_Init(&sha256_ctx_chunk) == 1) {
            // Update with 1-byte chunks
            for (size_t i = 0; i < data_len && i < 10; i++) {
                SHA256_Update(&sha256_ctx_chunk, data_ptr + i, 1);
            }
            SHA256_Final(hash_output, &sha256_ctx_chunk);
        }
    }
    
    // Clean up OpenSSL (optional - not strictly necessary but good practice)
    EVP_cleanup();
    ERR_free_strings();
    
    return 0;
}

} // namespace harness_010
