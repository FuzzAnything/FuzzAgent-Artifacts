#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/md2.h>
#include <openssl/md4.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/des.h>
#include <openssl/cast.h>
#include <openssl/rc2.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace harness_006 {
/*
 * Fuzzing harness for OpenSSL hash functions and symmetric encryption algorithms
 * Targets: MD2 (0%), MD4 (0%), MD5 (10.81%), SHA family, DES (0%), ARIA (0%), CAST (0%), RC2 (0%)
 * Specifically targets low-level hash functions and EVP digest/cipher interfaces
 * Ensures semantic diversity from existing harnesses:
 * - harness_000 (SSL/TLS APIs)
 * - harness_001 (BN module) 
 * - harness_002 (EVP asymmetric encryption)
 * - harness_003 (PEM/X509 certificate handling)
 * - harness_004 (PKCS12 operations)
 * - harness_005 (AES symmetric encryption)
 * This harness focuses on hash algorithms and other symmetric ciphers beyond AES
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

// Test MD2 hash function (low-level API)
static void test_md2(FuzzedDataProvider& fdp, const std::vector<uint8_t>& data) {
    if (data.empty()) return;
    
    MD2_CTX ctx;
    unsigned char md[MD2_DIGEST_LENGTH];
    
    // Test MD2_Init, MD2_Update, MD2_Final
    if (MD2_Init(&ctx)) {
        MD2_Update(&ctx, data.data(), data.size());
        MD2_Final(md, &ctx);
    }
    
    // Test one-shot MD2 function
    unsigned char md2_one[MD2_DIGEST_LENGTH];
    MD2(data.data(), data.size(), md2_one);
}

// Test MD4 hash function (low-level API)
static void test_md4(FuzzedDataProvider& fdp, const std::vector<uint8_t>& data) {
    if (data.empty()) return;
    
    MD4_CTX ctx;
    unsigned char md[MD4_DIGEST_LENGTH];
    
    // Test MD4_Init, MD4_Update, MD4_Final
    if (MD4_Init(&ctx)) {
        MD4_Update(&ctx, data.data(), data.size());
        MD4_Final(md, &ctx);
    }
    
    // Test one-shot MD4 function
    unsigned char md4_one[MD4_DIGEST_LENGTH];
    MD4(data.data(), data.size(), md4_one);
}

// Test MD5 hash function (low-level API)
static void test_md5(FuzzedDataProvider& fdp, const std::vector<uint8_t>& data) {
    if (data.empty()) return;
    
    MD5_CTX ctx;
    unsigned char md[MD5_DIGEST_LENGTH];
    
    // Test MD5_Init, MD5_Update, MD5_Final
    if (MD5_Init(&ctx)) {
        MD5_Update(&ctx, data.data(), data.size());
        MD5_Final(md, &ctx);
    }
    
    // Test one-shot MD5 function
    unsigned char md5_one[MD5_DIGEST_LENGTH];
    MD5(data.data(), data.size(), md5_one);
}

// Test SHA family hash functions
static void test_sha_family(FuzzedDataProvider& fdp, const std::vector<uint8_t>& data) {
    if (data.empty()) return;
    
    // Test various SHA algorithms
    unsigned char sha1_digest[SHA_DIGEST_LENGTH];
    SHA1(data.data(), data.size(), sha1_digest);
    
    unsigned char sha224_digest[SHA224_DIGEST_LENGTH];
    SHA224(data.data(), data.size(), sha224_digest);
    
    unsigned char sha256_digest[SHA256_DIGEST_LENGTH];
    SHA256(data.data(), data.size(), sha256_digest);
    
    unsigned char sha384_digest[SHA384_DIGEST_LENGTH];
    SHA384(data.data(), data.size(), sha384_digest);
    
    unsigned char sha512_digest[SHA512_DIGEST_LENGTH];
    SHA512(data.data(), data.size(), sha512_digest);
}
    
// Note: SHA512_224 and SHA512_256 functions may not be available in all OpenSSL versions
// They are included via EVP interface instead
// Test EVP digest interface for various hash algorithms
static void test_evp_digest(FuzzedDataProvider& fdp, const std::vector<uint8_t>& data) {
    if (data.empty()) return;
    
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (mdctx == NULL) return;
    
    // Select hash algorithm based on fuzzer input
    uint8_t hash_choice = fdp.ConsumeIntegral<uint8_t>() % 8;
    const EVP_MD* md = NULL;
    
    switch (hash_choice) {
        case 0: md = EVP_md2(); break;
        case 1: md = EVP_md4(); break;
        case 2: md = EVP_md5(); break;
        case 3: md = EVP_sha1(); break;
        case 4: md = EVP_sha224(); break;
        case 5: md = EVP_sha256(); break;
        case 6: md = EVP_sha384(); break;
        case 7: md = EVP_sha512(); break;
    }
    
    if (md != NULL) {
        unsigned char digest[EVP_MAX_MD_SIZE];
        unsigned int digest_len;
        
        // Initialize, update, finalize
        if (EVP_DigestInit_ex(mdctx, md, NULL)) {
            EVP_DigestUpdate(mdctx, data.data(), data.size());
            EVP_DigestFinal_ex(mdctx, digest, &digest_len);
        }
        
        // Test one-shot EVP_Digest function
        unsigned char digest2[EVP_MAX_MD_SIZE];
        unsigned int digest_len2;
        EVP_Digest(data.data(), data.size(), digest2, &digest_len2, md, NULL);
    }
    
    EVP_MD_CTX_free(mdctx);
}

// Test DES symmetric encryption (low-level API)
static void test_des(FuzzedDataProvider& fdp, const std::vector<uint8_t>& data) {
    if (data.size() < 16) return; // Need at least key + some data
    
    DES_cblock key;
    DES_cblock iv;
    DES_key_schedule schedule;
    
    // Use first 8 bytes as key
    memcpy(key, data.data(), 8);
    
    // Set key
    DES_set_key_unchecked(&key, &schedule);
    
    // Use next 8 bytes as IV if available
    if (data.size() >= 16) {
        memcpy(iv, data.data() + 8, 8);
    } else {
        memset(iv, 0, 8);
    }
    
    // Prepare input/output buffers
    size_t data_len = data.size() - 16;
    if (data_len > 0) {
        // Ensure data_len is multiple of 8 for DES
        data_len = (data_len / 8) * 8;
        if (data_len > 0) {
            std::vector<uint8_t> input(data_len);
            std::vector<uint8_t> output(data_len);
            
            memcpy(input.data(), data.data() + 16, data_len);
            
            // Test DES encryption/decryption
            DES_cblock iv_enc, iv_dec;
            memcpy(iv_enc, iv, 8);
            memcpy(iv_dec, iv, 8);
            
            // CBC mode encryption
            DES_ncbc_encrypt(input.data(), output.data(), data_len,
                           &schedule, &iv_enc, DES_ENCRYPT);
            
            // CBC mode decryption
            DES_ncbc_encrypt(output.data(), input.data(), data_len,
                           &schedule, &iv_dec, DES_DECRYPT);
        }
    }
}

// Test CAST symmetric encryption
static void test_cast(FuzzedDataProvider& fdp, const std::vector<uint8_t>& data) {
    if (data.size() < 16) return;
    
    CAST_KEY key;
    
    // Use first 16 bytes as key (CAST supports up to 16 byte keys)
    size_t key_len = fdp.ConsumeIntegralInRange<size_t>(5, 16);
    if (key_len > 16) key_len = 16;
    
    CAST_set_key(&key, key_len, data.data());
    
    // Prepare data for encryption
    size_t data_offset = 16;
    if (data.size() > data_offset + 8) {
        size_t data_len = data.size() - data_offset;
        data_len = (data_len / 8) * 8; // CAST block size is 8 bytes
        
        if (data_len >= 8) {
            std::vector<uint8_t> input(data_len);
            std::vector<uint8_t> output(data_len);
            
            memcpy(input.data(), data.data() + data_offset, data_len);
            
            // Test ECB mode
            CAST_ecb_encrypt(input.data(), output.data(), &key, CAST_ENCRYPT);
            CAST_ecb_encrypt(output.data(), input.data(), &key, CAST_DECRYPT);
        }
    }
}

// Test RC2 symmetric encryption
static void test_rc2(FuzzedDataProvider& fdp, const std::vector<uint8_t>& data) {
    if (data.size() < 16) return;
    
    RC2_KEY key;
    
    // Use first 16 bytes as key material
    int bits = fdp.ConsumeIntegralInRange<int>(40, 1024); // RC2 key bits
    RC2_set_key(&key, 16, data.data(), bits);
    
    // Prepare data for encryption
    size_t data_offset = 16;
    if (data.size() > data_offset + 8) {
        size_t data_len = data.size() - data_offset;
        data_len = (data_len / 8) * 8; // RC2 block size is 8 bytes
        
        if (data_len >= 8) {
            std::vector<uint8_t> input(data_len);
            std::vector<uint8_t> output(data_len);
            
            memcpy(input.data(), data.data() + data_offset, data_len);
            
            // Test ECB mode
            RC2_ecb_encrypt(input.data(), output.data(), &key, RC2_ENCRYPT);
            RC2_ecb_encrypt(output.data(), input.data(), &key, RC2_DECRYPT);
        }
    }
}

// Test EVP cipher interface for symmetric encryption
static void test_evp_cipher(FuzzedDataProvider& fdp, const std::vector<uint8_t>& data) {
    if (data.size() < 32) return; // Need key + IV + some data
    
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL) return;
    
    // Select cipher based on fuzzer input
    uint8_t cipher_choice = fdp.ConsumeIntegral<uint8_t>() % 6;
    const EVP_CIPHER* cipher = NULL;
    
    switch (cipher_choice) {
        case 0: cipher = EVP_des_cbc(); break;
        case 1: cipher = EVP_des_ede3_cbc(); break; // Triple DES
        case 2: cipher = EVP_cast5_cbc(); break;
        case 3: cipher = EVP_rc2_cbc(); break;
        case 4: cipher = EVP_aria_128_cbc(); break;
        case 5: cipher = EVP_aria_256_gcm(); break;
    }
    
    if (cipher == NULL) {
        EVP_CIPHER_CTX_free(ctx);
        return;
    }
    
    // Determine key and IV sizes
    int key_len = EVP_CIPHER_key_length(cipher);
    int iv_len = EVP_CIPHER_iv_length(cipher);
    
    if (data.size() < (size_t)(key_len + iv_len + 1)) {
        EVP_CIPHER_CTX_free(ctx);
        return;
    }
    
    // Extract key and IV from fuzzer data
    const uint8_t* key = data.data();
    const uint8_t* iv = data.data() + key_len;
    
    // Remaining data is plaintext
    size_t plaintext_len = data.size() - key_len - iv_len;
    
    // Allocate buffers
    std::vector<uint8_t> ciphertext(plaintext_len + EVP_CIPHER_block_size(cipher));
    std::vector<uint8_t> decrypted(plaintext_len + EVP_CIPHER_block_size(cipher));
    
    int ciphertext_len, decrypted_len;
    
    // Test encryption
    if (EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv)) {
        EVP_EncryptUpdate(ctx, ciphertext.data(), &ciphertext_len,
                         data.data() + key_len + iv_len, plaintext_len);
        int final_len;
        EVP_EncryptFinal_ex(ctx, ciphertext.data() + ciphertext_len, &final_len);
        ciphertext_len += final_len;
        
        // Test decryption
        if (EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv)) {
            EVP_DecryptUpdate(ctx, decrypted.data(), &decrypted_len,
                            ciphertext.data(), ciphertext_len);
            int final_len2;
            EVP_DecryptFinal_ex(ctx, decrypted.data() + decrypted_len, &final_len2);
            decrypted_len += final_len2;
        }
    }
    
    EVP_CIPHER_CTX_free(ctx);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Too small for meaningful testing
    }
    
    initialize_openssl();
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test type selection
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Consume data for testing (use all remaining bytes)
    std::vector<uint8_t> test_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Clear any previous errors
    ERR_clear_error();
    
    switch (test_type) {
        case 0:
            test_md2(fdp, test_data);
            break;
        case 1:
            test_md4(fdp, test_data);
            break;
        case 2:
            test_md5(fdp, test_data);
            break;
        case 3:
            test_sha_family(fdp, test_data);
            break;
        case 4:
            test_evp_digest(fdp, test_data);
            break;
        case 5:
            test_des(fdp, test_data);
            break;
        case 6:
            test_cast(fdp, test_data);
            break;
        case 7:
            test_rc2(fdp, test_data);
            break;
    }
    
    // Also test EVP cipher interface (uses its own data consumption)
    if (size >= 64) {
        FuzzedDataProvider fdp2(data, size);
        std::vector<uint8_t> cipher_data = fdp2.ConsumeRemainingBytes<uint8_t>();
        test_evp_cipher(fdp2, cipher_data);
    }
    
    return 0;
}

} // namespace harness_006
