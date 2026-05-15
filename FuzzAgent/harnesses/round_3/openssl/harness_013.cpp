#include <stddef.h>
#include <stdint.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include <openssl/err.h>
#include <openssl/bn.h>
#include <openssl/asn1.h>
#include <openssl/evp.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_013 {
/*
 * Fuzzing harness for OpenSSL EC (Elliptic Curve) cryptography operations
 * Primary target: d2i_ECPKParameters with 1469 undiscovered branches
 * Targets: d2i_ECPKParameters, i2d_ECPKParameters, EC_KEY_new_by_curve_name,
 *          EC_KEY_generate_key, EC_KEY_check_key, EC_KEY_get0_group,
 *          EC_KEY_get0_public_key, EC_KEY_get0_private_key, EC_KEY_set_group,
 *          EC_KEY_set_public_key, EC_KEY_set_private_key, EC_KEY_free,
 *          EC_GROUP_get_curve_name, EC_GROUP_get_degree, EC_GROUP_get_order,
 *          EC_POINT_new, EC_POINT_set_affine_coordinates, EC_POINT_get_affine_coordinates,
 *          EC_POINT_mul, EC_POINT_add, EC_POINT_dbl, EC_POINT_invert,
 *          ECDSA_sign, ECDSA_verify, ECDSA_size
 * Helper APIs: BN_new, BN_free, BN_CTX_new, BN_CTX_free, OpenSSL_add_all_algorithms,
 *              ERR_clear_error, OBJ_nid2obj
 * Cleanup: EC_KEY_free, EC_GROUP_free, EC_POINT_free, BN_free, BN_CTX_free
 * Implements full EC cryptography lifecycle: Parse EC parameters → Create EC group →
 * Create EC keys → Generate key pairs → Perform point operations → Sign/verify →
 * Serialize/deserialize → Cleanup
 * Differentiated from all previous harnesses (000-012) - this is the first harness
 * targeting EC cryptography module which is critical for modern cryptography
 * including ECDSA signatures, ECDH key exchange, and elliptic curve operations.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) {
        return 0;  // Need minimum input for meaningful EC testing
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_clear_error();

    // Determine test scenario based on fuzzer input
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 8;

    // Common NIDs for elliptic curves
    int curve_nids[] = {
        NID_X9_62_prime256v1,      // P-256
        NID_secp384r1,             // P-384  
        NID_secp521r1,             // P-521
        NID_X9_62_prime192v1,      // P-192
        NID_secp224r1,             // P-224
        NID_X9_62_c2pnb163v1,      // Characteristic two field curve (if available)
        NID_sect233k1,             // Koblitz curve
        NID_sect233r1,             // Random curve
    };
    
    // Choose a curve based on fuzzer input
    uint8_t curve_idx = fdp.ConsumeIntegral<uint8_t>() % (sizeof(curve_nids)/sizeof(curve_nids[0]));
    int curve_nid = curve_nids[curve_idx];

    // Create BN context for EC operations
    BN_CTX* bn_ctx = BN_CTX_new();
    if (bn_ctx == NULL) {
        return 0;
    }

    // Main test switch
    switch (scenario) {
        case 0: {
            // Scenario 0: Test d2i_ECPKParameters with raw ASN.1 data
            size_t asn1_data_size = fdp.ConsumeIntegralInRange<size_t>(1, size/2);
            if (asn1_data_size > fdp.remaining_bytes()) {
                asn1_data_size = fdp.remaining_bytes();
            }
            std::vector<uint8_t> asn1_data = fdp.ConsumeBytes<uint8_t>(asn1_data_size);
            
            const unsigned char* p = asn1_data.data();
            EC_GROUP* group = d2i_ECPKParameters(NULL, &p, asn1_data.size());
            
            if (group != NULL) {
                // Try to get curve name and other properties
                int group_curve_nid = EC_GROUP_get_curve_name(group);
                int group_degree = EC_GROUP_get_degree(group);
                
                // Try to serialize back
                unsigned char* out_buf = NULL;
                int out_len = i2d_ECPKParameters(group, &out_buf);
                if (out_buf != NULL) {
                    OPENSSL_free(out_buf);
                }
                
                EC_GROUP_free(group);
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Create EC key with named curve and generate key pair
            EC_KEY* ec_key = EC_KEY_new_by_curve_name(curve_nid);
            if (ec_key == NULL) {
                break;
            }
            
            // Generate key pair
            if (EC_KEY_generate_key(ec_key)) {
                // Check key validity
                EC_KEY_check_key(ec_key);
                
                // Get group, public and private key components
                const EC_GROUP* group = EC_KEY_get0_group(ec_key);
                const EC_POINT* pub_key = EC_KEY_get0_public_key(ec_key);
                const BIGNUM* priv_key = EC_KEY_get0_private_key(ec_key);
                
                if (group != NULL && pub_key != NULL && priv_key != NULL) {
                    // Get group order
                    BIGNUM* order = BN_new();
                    if (order != NULL) {
                        EC_GROUP_get_order(group, order, bn_ctx);
                        BN_free(order);
                    }
                    
                    // Try to get affine coordinates of public key
                    BIGNUM* x = BN_new();
                    BIGNUM* y = BN_new();
                    if (x != NULL && y != NULL) {
                        EC_POINT_get_affine_coordinates(group, pub_key, x, y, bn_ctx);
                        BN_free(x);
                        BN_free(y);
                    }
                }
            }
            
            EC_KEY_free(ec_key);
            break;
        }
        
        case 2: {
            // Scenario 2: Test EC point operations
            EC_KEY* ec_key = EC_KEY_new_by_curve_name(curve_nid);
            if (ec_key == NULL || !EC_KEY_generate_key(ec_key)) {
                if (ec_key != NULL) EC_KEY_free(ec_key);
                break;
            }
            
            const EC_GROUP* group = EC_KEY_get0_group(ec_key);
            const EC_POINT* G = EC_GROUP_get0_generator(group);
            const EC_POINT* pub_key = EC_KEY_get0_public_key(ec_key);
            
            // Create new points for operations
            EC_POINT* point1 = EC_POINT_new(group);
            EC_POINT* point2 = EC_POINT_new(group);
            EC_POINT* point_result = EC_POINT_new(group);
            
            if (point1 != NULL && point2 != NULL && point_result != NULL) {
                // Copy generator to point1
                EC_POINT_copy(point1, G);
                
                // Test point doubling: point2 = 2*G
                EC_POINT_dbl(group, point2, G, bn_ctx);
                
                // Test point addition: point_result = G + 2*G = 3*G
                EC_POINT_add(group, point_result, G, point2, bn_ctx);
                
                // Test scalar multiplication with random scalar
                BIGNUM* scalar = BN_new();
                if (scalar != NULL) {
                    size_t scalar_bytes = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                    if (scalar_bytes > fdp.remaining_bytes()) {
                        scalar_bytes = fdp.remaining_bytes();
                    }
                    std::vector<uint8_t> scalar_data = fdp.ConsumeBytes<uint8_t>(scalar_bytes);
                    BN_bin2bn(scalar_data.data(), scalar_data.size(), scalar);
                    
                    // point1 = scalar * G
                    EC_POINT_mul(group, point1, scalar, NULL, NULL, bn_ctx);
                    
                    BN_free(scalar);
                }
                
                // Test point inversion
                EC_POINT_invert(group, point2, bn_ctx);
            }
            
            if (point1 != NULL) EC_POINT_free(point1);
            if (point2 != NULL) EC_POINT_free(point2);
            if (point_result != NULL) EC_POINT_free(point_result);
            EC_KEY_free(ec_key);
            break;
        }
        
        case 3: {
            // Scenario 3: Test ECDSA signing and verification
            EC_KEY* ec_key = EC_KEY_new_by_curve_name(curve_nid);
            if (ec_key == NULL || !EC_KEY_generate_key(ec_key)) {
                if (ec_key != NULL) EC_KEY_free(ec_key);
                break;
            }
            
            // Create message to sign
            size_t message_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            if (message_size > fdp.remaining_bytes()) {
                message_size = fdp.remaining_bytes();
            }
            std::vector<uint8_t> message = fdp.ConsumeBytes<uint8_t>(message_size);
            
            // Get ECDSA signature size
            int sig_size = ECDSA_size(ec_key);
            
            // Create buffer for signature
            std::vector<unsigned char> signature(sig_size);
            unsigned int sig_len = 0;
            
            // Sign the message
            if (ECDSA_sign(0, message.data(), message.size(), 
                          signature.data(), &sig_len, ec_key)) {
                // Verify the signature
                ECDSA_verify(0, message.data(), message.size(),
                            signature.data(), sig_len, ec_key);
                
                // Try to verify with modified message (should fail)
                if (message.size() > 0) {
                    std::vector<unsigned char> modified_message = message;
                    modified_message[0] ^= 0x01; // Flip one bit
                    ECDSA_verify(0, modified_message.data(), modified_message.size(),
                                signature.data(), sig_len, ec_key);
                }
                
                // Try to verify with modified signature (should fail)
                if (sig_len > 0) {
                    std::vector<unsigned char> modified_signature = signature;
                    modified_signature[0] ^= 0x01; // Flip one bit
                    ECDSA_verify(0, message.data(), message.size(),
                                modified_signature.data(), sig_len, ec_key);
                }
            }
            
            EC_KEY_free(ec_key);
            break;
        }
        
        case 4: {
            // Scenario 4: Test EC_KEY manipulation functions
            EC_KEY* ec_key = EC_KEY_new();
            if (ec_key == NULL) {
                break;
            }
            
            // Create a group by curve name
            EC_GROUP* group = EC_GROUP_new_by_curve_name(curve_nid);
            if (group != NULL) {
                // Set the group on the key
                EC_KEY_set_group(ec_key, group);
                
                // Generate a private key
                BIGNUM* priv_key = BN_new();
                if (priv_key != NULL) {
                    size_t priv_bytes = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                    if (priv_bytes > fdp.remaining_bytes()) {
                        priv_bytes = fdp.remaining_bytes();
                    }
                    std::vector<uint8_t> priv_data = fdp.ConsumeBytes<uint8_t>(priv_bytes);
                    BN_bin2bn(priv_data.data(), priv_data.size(), priv_key);
                    
                    // Set private key
                    EC_KEY_set_private_key(ec_key, priv_key);
                    
                    // Compute corresponding public key
                    EC_POINT* pub_point = EC_POINT_new(group);
                    if (pub_point != NULL) {
                        const BIGNUM* priv = EC_KEY_get0_private_key(ec_key);
                        if (priv != NULL) {
                            // pub_point = priv * G
                            EC_POINT_mul(group, pub_point, priv, NULL, NULL, bn_ctx);
                            EC_KEY_set_public_key(ec_key, pub_point);
                        }
                        EC_POINT_free(pub_point);
                    }
                    
                    BN_free(priv_key);
                }
                
                EC_GROUP_free(group);
            }
            
            EC_KEY_free(ec_key);
            break;
        }
        
        case 5: {
            // Scenario 5: Test with explicit parameters (not named curve)
            // Consume data for explicit parameters
            size_t param_data_size = fdp.ConsumeIntegralInRange<size_t>(1, size/3);
            if (param_data_size > fdp.remaining_bytes()) {
                param_data_size = fdp.remaining_bytes();
            }
            std::vector<uint8_t> param_data = fdp.ConsumeBytes<uint8_t>(param_data_size);
            
            const unsigned char* p = param_data.data();
            EC_GROUP* group = d2i_ECPKParameters(NULL, &p, param_data.size());
            
            if (group != NULL) {
                // Create key with this group
                EC_KEY* ec_key = EC_KEY_new();
                if (ec_key != NULL) {
                    EC_KEY_set_group(ec_key, group);
                    
                    // Try to generate key
                    EC_KEY_generate_key(ec_key);
                    
                    // Check key
                    EC_KEY_check_key(ec_key);
                    
                    EC_KEY_free(ec_key);
                }
                
                EC_GROUP_free(group);
            }
            break;
        }
        
        case 6: {
            // Scenario 6: Test EC_KEY duplication and reference counting
            EC_KEY* ec_key1 = EC_KEY_new_by_curve_name(curve_nid);
            if (ec_key1 == NULL || !EC_KEY_generate_key(ec_key1)) {
                if (ec_key1 != NULL) EC_KEY_free(ec_key1);
                break;
            }
            
            // Duplicate the key
            EC_KEY* ec_key2 = EC_KEY_dup(ec_key1);
            if (ec_key2 != NULL) {
                // Increase reference count
                EC_KEY_up_ref(ec_key2);
                
                // Check both keys
                EC_KEY_check_key(ec_key1);
                EC_KEY_check_key(ec_key2);
                
                EC_KEY_free(ec_key2);
                EC_KEY_free(ec_key2); // Second free for the increased ref
            }
            
            EC_KEY_free(ec_key1);
            break;
        }
        
        case 7: {
            // Scenario 7: Test edge cases with invalid data
            // Try to parse random data as EC parameters (should often fail)
            size_t random_data_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
            if (random_data_size > fdp.remaining_bytes()) {
                random_data_size = fdp.remaining_bytes();
            }
            std::vector<uint8_t> random_data = fdp.ConsumeBytes<uint8_t>(random_data_size);
            
            const unsigned char* p = random_data.data();
            EC_GROUP* group = d2i_ECPKParameters(NULL, &p, random_data.size());
            
            // Also try to create key with invalid curve NID
            int invalid_nid = fdp.ConsumeIntegral<int>();
            EC_KEY* bad_key = EC_KEY_new_by_curve_name(invalid_nid);
            if (bad_key != NULL) {
                EC_KEY_free(bad_key);
            }
            
            if (group != NULL) {
                EC_GROUP_free(group);
            }
            break;
        }
    }
    
    // Cleanup
    BN_CTX_free(bn_ctx);
    
    // Clear any accumulated errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_013
