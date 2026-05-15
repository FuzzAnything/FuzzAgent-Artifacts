#include <stddef.h>
#include <stdint.h>
#include <openssl/cms.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_007 {
/*
 * Fuzzing harness for OpenSSL CMS EncryptedData operations
 * Primary target: CMS_EncryptedData_decrypt (2122 undiscovered branches)
 * Targets: CMS_EncryptedData_decrypt, CMS_EncryptedData_encrypt_ex, CMS_EncryptedData_set1_key,
 *          CMS_digest_create_ex, CMS_ContentInfo_new_ex, CMS_ContentInfo_free, CMS_dataInit, CMS_get0_type
 * Helper APIs: BIO_new_mem_buf, BIO_new, BIO_free_all, EVP_cipher initialization, random key generation
 * Implements CMS EncryptedData lifecycle: Initialization → Encryption → Key Setting → Data Init → Decryption → Cleanup
 * Differentiated from harness_004.cpp (PKCS7/CMS signature verification) - this harness focuses on CMS EncryptedData operations
 * CMS EncryptedData operations are critical for secure message encryption/decryption (S/MIME)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Need minimum input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_clear_error();

    // Determine operation type based on fuzzer input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;

    // Create key material from fuzzer input
    size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);  // Key length 1-64 bytes
    if (key_len > fdp.remaining_bytes()) {
        key_len = fdp.remaining_bytes();
    }
    std::vector<uint8_t> key_data = fdp.ConsumeBytes<uint8_t>(key_len);

    // Create data to encrypt/decrypt
    size_t data_len = fdp.ConsumeIntegralInRange<size_t>(0, 4096);  // Data length 0-4096 bytes
    if (data_len > fdp.remaining_bytes()) {
        data_len = fdp.remaining_bytes();
    }
    std::vector<uint8_t> plain_data = fdp.ConsumeBytes<uint8_t>(data_len);

    // Create incorrect key for error path testing
    std::vector<uint8_t> wrong_key;
    if (fdp.remaining_bytes() > 16) {
        size_t wrong_key_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
        wrong_key = fdp.ConsumeBytes<uint8_t>(wrong_key_len);
    }

    // Determine cipher algorithm
    uint8_t cipher_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    const EVP_CIPHER* cipher = NULL;
    
    switch (cipher_choice) {
        case 0:
            cipher = EVP_aes_128_cbc();
            break;
        case 1:
            cipher = EVP_aes_256_cbc();
            break;
        case 2:
            cipher = EVP_aes_128_gcm();
            break;
        case 3:
            cipher = EVP_aes_256_gcm();
            break;
    }

    // Create BIO for data input
    BIO* data_bio = BIO_new_mem_buf(plain_data.data(), plain_data.size());
    if (data_bio == NULL) {
        return 0;
    }

    // Main CMS structures
    CMS_ContentInfo* cms = NULL;
    BIO* out_bio = NULL;
    int ret = 0;

    switch (operation) {
        case 0: {
            // Test 1: Encrypt data with random key, then decrypt with same key
            cms = CMS_EncryptedData_encrypt_ex(data_bio, cipher, key_data.data(), key_len, 0, NULL, NULL);
            if (cms == NULL) {
                break;
            }

            // Get CMS type
            const ASN1_OBJECT* cms_type = CMS_get0_type(cms);

            // Try to decrypt with correct key
            out_bio = BIO_new(BIO_s_mem());
            if (out_bio == NULL) {
                CMS_ContentInfo_free(cms);
                break;
            }

            // Set decryption key
            ret = CMS_EncryptedData_set1_key(cms, cipher, key_data.data(), key_len);
            if (ret > 0) {
                // Initialize data processing
                BIO* data_init_bio = CMS_dataInit(cms, NULL);
                if (data_init_bio != NULL) {
                    BIO_free_all(data_init_bio);
                }

                // Perform decryption
                ret = CMS_EncryptedData_decrypt(cms, key_data.data(), key_len, NULL, out_bio, 0);
            }
            break;
        }

        case 1: {
            // Test 2: Encrypt with different cipher algorithms (AES-128, AES-256)
            cms = CMS_EncryptedData_encrypt_ex(data_bio, cipher, key_data.data(), key_len, 0, NULL, NULL);
            if (cms == NULL) {
                break;
            }

            // Get CMS type
            const ASN1_OBJECT* cms_type = CMS_get0_type(cms);

            // Try to decrypt (should work with correct key)
            out_bio = BIO_new(BIO_s_mem());
            if (out_bio == NULL) {
                CMS_ContentInfo_free(cms);
                break;
            }

            ret = CMS_EncryptedData_decrypt(cms, key_data.data(), key_len, NULL, out_bio, 0);
            break;
        }

        case 2: {
            // Test 3: Decrypt with incorrect key (error path testing)
            // First create encrypted data
            cms = CMS_EncryptedData_encrypt_ex(data_bio, cipher, key_data.data(), key_len, 0, NULL, NULL);
            if (cms == NULL) {
                break;
            }

            out_bio = BIO_new(BIO_s_mem());
            if (out_bio == NULL) {
                CMS_ContentInfo_free(cms);
                break;
            }

            // Try to decrypt with wrong key (should fail)
            if (!wrong_key.empty()) {
                ret = CMS_EncryptedData_decrypt(cms, wrong_key.data(), wrong_key.size(), NULL, out_bio, 0);
            }
            break;
        }

        case 3: {
            // Test 4: Digest creation and verification
            // Determine digest algorithm
            uint8_t digest_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
            const EVP_MD* digest = NULL;
            
            switch (digest_choice) {
                case 0:
                    digest = EVP_sha256();
                    break;
                case 1:
                    digest = EVP_sha1();
                    break;
                case 2:
                    digest = EVP_sha512();
                    break;
            }

            if (digest != NULL) {
                cms = CMS_digest_create_ex(data_bio, digest, 0, NULL, NULL);
                if (cms != NULL) {
                    // Get CMS type for digest
                    const ASN1_OBJECT* cms_type = CMS_get0_type(cms);
                    
                    // Try digest verification
                    out_bio = BIO_new(BIO_s_mem());
                    if (out_bio != NULL) {
                        ret = CMS_digest_verify(cms, data_bio, out_bio, 0);
                        BIO_free_all(out_bio);
                        out_bio = NULL;
                    }
                }
            }
            break;
        }

        case 4: {
            // Test 5: Edge cases - empty data
            if (plain_data.empty()) {
                // Create BIO with empty data
                BIO* empty_bio = BIO_new_mem_buf(NULL, 0);
                if (empty_bio != NULL) {
                    cms = CMS_EncryptedData_encrypt_ex(empty_bio, cipher, key_data.data(), key_len, 0, NULL, NULL);
                    BIO_free_all(empty_bio);
                }
            } else {
                cms = CMS_EncryptedData_encrypt_ex(data_bio, cipher, key_data.data(), key_len, 0, NULL, NULL);
            }
            
            if (cms != NULL) {
                const ASN1_OBJECT* cms_type = CMS_get0_type(cms);
                CMS_ContentInfo_free(cms);
                cms = NULL;
            }
            break;
        }

        case 5: {
            // Test 6: Create new CMS content info and test basic operations
            cms = CMS_ContentInfo_new_ex(NULL, NULL);
            if (cms == NULL) {
                break;
            }

            // Get CMS type (should be NULL for newly created)
            const ASN1_OBJECT* cms_type = CMS_get0_type(cms);

            // Try to set key on non-encrypted content (should fail)
            if (!key_data.empty()) {
                ret = CMS_EncryptedData_set1_key(cms, cipher, key_data.data(), key_len);
            }
            break;
        }
    }

    // Cleanup
    if (out_bio != NULL) {
        BIO_free_all(out_bio);
    }
    if (cms != NULL) {
        CMS_ContentInfo_free(cms);
    }
    if (data_bio != NULL) {
        BIO_free_all(data_bio);
    }

    // Clear any remaining errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_007
