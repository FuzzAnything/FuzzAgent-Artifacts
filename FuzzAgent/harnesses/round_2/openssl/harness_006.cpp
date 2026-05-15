#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/pem.h>
#include <openssl/ec.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

namespace harness_006 {
/*
 * Fuzzing harness for OpenSSL PEM file format operations for Elliptic Curve Cryptography
 * Targets: PEM_write_bio_ECPKParameters, PEM_read_bio_ECPKParameters, 
 *          PEM_write_bio_ECPrivateKey, PEM_read_bio_ECPrivateKey
 * Coverage goal: 1088+ undiscovered branches in PEM_write_bio_ECPKParameters (0% coverage)
 * Semantic differentiation: PEM file format operations for EC cryptography (vs. crypto basics,
 *                          SSL/TLS, OCSP, providers, QUIC, and envelope encryption in previous harnesses)
 * 
 * APIs targeted:
 * - PEM_write_bio_ECPKParameters - Write EC parameters to BIO in PEM format
 * - PEM_read_bio_ECPKParameters - Read EC parameters from BIO in PEM format
 * - PEM_write_bio_ECPrivateKey - Write EC private key to BIO in PEM format
 * - PEM_read_bio_ECPrivateKey - Read EC private key from BIO in PEM format
 * 
 * Required setup APIs:
 * - EC_GROUP_new_by_curve_name - Create EC group for specific curve
 * - EC_KEY_new_by_curve_name - Create EC key for specific curve
 * - EC_KEY_generate_key - Generate EC key pair
 * - BIO_new_mem_buf / BIO_new - Create memory BIO for PEM operations
 * 
 * Cleanup APIs:
 * - EC_KEY_free
 * - BIO_free
 * - EC_GROUP_free
 * 
 * PEM (Privacy Enhanced Mail) format is widely used for certificate and key storage.
 * This harness tests serialization/deserialization of EC cryptographic objects.
 */


int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < 32) {
        return 0; // Need minimum input for meaningful PEM testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize OpenSSL
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | 
                       OPENSSL_INIT_ADD_ALL_CIPHERS | 
                       OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
    ERR_clear_error();
    
    // Declare all variables at the beginning
    EC_GROUP *ec_group = NULL;
    EC_KEY *ec_key = NULL;
    EC_KEY *ec_key_read = NULL;
    EC_GROUP *ec_group_read = NULL;
    BIO *bio_mem = NULL;
    BIO *bio_read = NULL;
    int curve_nid = 0;
    int operation = 0;
    int pem_result = 0;
    unsigned char *bio_data = NULL;
    long bio_len = 0;
    
    // Determine which EC curve to use based on fuzzer input
    uint8_t curve_choice = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    switch (curve_choice) {
        case 0:
            curve_nid = NID_X9_62_prime256v1; // secp256r1
            break;
        case 1:
            curve_nid = NID_secp384r1; // secp384r1
            break;
        case 2:
            curve_nid = NID_secp521r1; // secp521r1
            break;
        case 3:
            curve_nid = NID_secp224r1; // secp224r1
            break;
        case 4:
            curve_nid = NID_X9_62_prime192v1; // secp192r1 (prime192v1 in X9.62)
            break;
        default:
            curve_nid = NID_X9_62_prime256v1;
            break;
    }
    
    // Create EC group for the selected curve
    ec_group = EC_GROUP_new_by_curve_name(curve_nid);
    if (ec_group == NULL) {
        goto cleanup;
    }
    
    // Create EC key for the selected curve
    ec_key = EC_KEY_new_by_curve_name(curve_nid);
    if (ec_key == NULL) {
        goto cleanup;
    }
    
    // Generate EC key pair
    if (!EC_KEY_generate_key(ec_key)) {
        goto cleanup;
    }
    
    // Set the key's group (already set by EC_KEY_new_by_curve_name, but ensure)
    EC_KEY_set_group(ec_key, ec_group);
    
    // Determine which PEM operation to test based on fuzzer input
    operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (operation) {
        case 0: {
            // Test PEM_write_bio_ECPKParameters and PEM_read_bio_ECPKParameters
            // Create memory BIO for writing
            bio_mem = BIO_new(BIO_s_mem());
            if (bio_mem == NULL) {
                goto cleanup;
            }
            
            // Write EC parameters to BIO in PEM format
            pem_result = PEM_write_bio_ECPKParameters(bio_mem, ec_group);
            if (pem_result == 0) {
                // Failed to write, but continue to test other paths
                BIO_free(bio_mem);
                bio_mem = NULL;
                break;
            }
            
            // Get the data written to BIO
            bio_len = BIO_get_mem_data(bio_mem, &bio_data);
            if (bio_len <= 0 || bio_data == NULL) {
                break;
            }
            
            // Create a new BIO for reading from the written data
            bio_read = BIO_new_mem_buf(bio_data, bio_len);
            if (bio_read == NULL) {
                break;
            }
            
            // Read EC parameters back from the BIO
            ec_group_read = PEM_read_bio_ECPKParameters(bio_read, NULL, NULL, NULL);
            
            if (ec_group_read != NULL) {
                // Successfully read, compare some properties
                int group_cmp = EC_GROUP_cmp(ec_group, ec_group_read, NULL);
                // group_cmp == 0 means groups are equal, != 0 means different
                (void)group_cmp; // Use result to avoid unused variable warning
            }
            
            break;
        }
        
        case 1: {
            // Test PEM_write_bio_ECPrivateKey and PEM_read_bio_ECPrivateKey without encryption
            bio_mem = BIO_new(BIO_s_mem());
            if (bio_mem == NULL) {
                goto cleanup;
            }
            
            // Write EC private key to BIO in PEM format (unencrypted)
            pem_result = PEM_write_bio_ECPrivateKey(bio_mem, ec_key, NULL, NULL, 0, NULL, NULL);
            if (pem_result == 0) {
                BIO_free(bio_mem);
                bio_mem = NULL;
                break;
            }
            
            // Get the data written to BIO
            bio_len = BIO_get_mem_data(bio_mem, &bio_data);
            if (bio_len <= 0 || bio_data == NULL) {
                break;
            }
            
            // Create a new BIO for reading from the written data
            bio_read = BIO_new_mem_buf(bio_data, bio_len);
            if (bio_read == NULL) {
                break;
            }
            
            // Read EC private key back from the BIO
            ec_key_read = PEM_read_bio_ECPrivateKey(bio_read, NULL, NULL, NULL);
            
            if (ec_key_read != NULL) {
                // Successfully read, verify the key
                int key_ok = EC_KEY_check_key(ec_key_read);
                (void)key_ok; // Use result to avoid unused variable warning
            }
            
            break;
        }
        
        case 2: {
            // Test PEM_write_bio_ECPrivateKey with password protection
            // Consume password from fuzzer input
            size_t password_len = fdp.ConsumeIntegralInRange<size_t>(0, 64);
            std::vector<uint8_t> password_vec = fdp.ConsumeBytes<uint8_t>(password_len);
            const char *password = NULL;
            if (!password_vec.empty()) {
                // Null-terminate the password
                password_vec.push_back(0);
                password = (const char *)password_vec.data();
            }
            
            bio_mem = BIO_new(BIO_s_mem());
            if (bio_mem == NULL) {
                goto cleanup;
            }
            
            // Write EC private key to BIO in PEM format with password encryption
            // Use default cipher (EVP_aes_256_cbc) if password provided
            const EVP_CIPHER *cipher = password != NULL ? EVP_aes_256_cbc() : NULL;
            
            pem_result = PEM_write_bio_ECPrivateKey(bio_mem, ec_key, cipher, 
                                                   (unsigned char *)password, 
                                                   password != NULL ? strlen(password) : 0, 
                                                   NULL, NULL);
            if (pem_result == 0) {
                BIO_free(bio_mem);
                bio_mem = NULL;
                break;
            }
            
            // Get the data written to BIO
            bio_len = BIO_get_mem_data(bio_mem, &bio_data);
            if (bio_len <= 0 || bio_data == NULL) {
                break;
            }
            
            // Create a new BIO for reading from the written data
            bio_read = BIO_new_mem_buf(bio_data, bio_len);
            if (bio_read == NULL) {
                break;
            }
            
            // Try to read EC private key back with and without password
            if (password != NULL) {
                // Try with correct password
                ec_key_read = PEM_read_bio_ECPrivateKey(bio_read, NULL, NULL, (void *)password);
                
                // Reset BIO to beginning
                BIO_free(bio_read);
                bio_read = BIO_new_mem_buf(bio_data, bio_len);
                
                if (bio_read != NULL) {
                    // Try with wrong password
                    EC_KEY *wrong_key = PEM_read_bio_ECPrivateKey(bio_read, NULL, NULL, (void *)"wrongpassword");
                    if (wrong_key != NULL) {
                        EC_KEY_free(wrong_key);
                    }
                }
            } else {
                // Try without password (should work since not encrypted)
                ec_key_read = PEM_read_bio_ECPrivateKey(bio_read, NULL, NULL, NULL);
            }
            
            break;
        }
        
        case 3: {
            // Test round-trip: parameters -> write -> read -> key -> write -> read
            bio_mem = BIO_new(BIO_s_mem());
            if (bio_mem == NULL) {
                goto cleanup;
            }
            
            // First, write EC parameters
            pem_result = PEM_write_bio_ECPKParameters(bio_mem, ec_group);
            if (pem_result == 0) {
                BIO_free(bio_mem);
                bio_mem = NULL;
                break;
            }
            
            // Get the data
            bio_len = BIO_get_mem_data(bio_mem, &bio_data);
            if (bio_len <= 0 || bio_data == NULL) {
                break;
            }
            
            // Read parameters back
            bio_read = BIO_new_mem_buf(bio_data, bio_len);
            if (bio_read == NULL) {
                break;
            }
            
            ec_group_read = PEM_read_bio_ECPKParameters(bio_read, NULL, NULL, NULL);
            
            // Now use the read group to create a new key
            if (ec_group_read != NULL) {
                BIO_free(bio_mem);
                bio_mem = BIO_new(BIO_s_mem());
                
                if (bio_mem != NULL) {
                    // Create a new EC key with the read group
                    EC_KEY *new_key = EC_KEY_new();
                    if (new_key != NULL) {
                        EC_KEY_set_group(new_key, ec_group_read);
                        if (EC_KEY_generate_key(new_key)) {
                            // Write the new key
                            PEM_write_bio_ECPrivateKey(bio_mem, new_key, NULL, NULL, 0, NULL, NULL);
                            
                            // Read it back
                            BIO_free(bio_read);
                            bio_len = BIO_get_mem_data(bio_mem, &bio_data);
                            if (bio_len > 0 && bio_data != NULL) {
                                bio_read = BIO_new_mem_buf(bio_data, bio_len);
                                if (bio_read != NULL) {
                                    EC_KEY *readback_key = PEM_read_bio_ECPrivateKey(bio_read, NULL, NULL, NULL);
                                    if (readback_key != NULL) {
                                        EC_KEY_free(readback_key);
                                    }
                                }
                            }
                        }
                        EC_KEY_free(new_key);
                    }
                }
            }
            
            break;
        }
    }
    
cleanup:
    // Clean up all allocated resources
    if (ec_key_read != NULL) {
        EC_KEY_free(ec_key_read);
    }
    
    if (ec_group_read != NULL) {
        EC_GROUP_free(ec_group_read);
    }
    
    if (bio_read != NULL) {
        BIO_free(bio_read);
    }
    
    if (bio_mem != NULL) {
        BIO_free(bio_mem);
    }
    
    if (ec_key != NULL) {
        EC_KEY_free(ec_key);
    }
    
    if (ec_group != NULL) {
        EC_GROUP_free(ec_group);
    }
    
    // Clear any remaining errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_006
