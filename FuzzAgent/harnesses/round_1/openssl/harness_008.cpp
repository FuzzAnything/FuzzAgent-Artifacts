#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/ecdh.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/bn.h>
#include <openssl/obj_mac.h>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace harness_008 {
/*
 * Fuzzing harness for OpenSSL Elliptic Curve Cryptography (ECC) operations
 * Targets: EC key generation, EC group operations, point arithmetic, 
 *          ECDH key exchange, ECDSA signing/verification, serialization
 * Specifically targets EC_GROUP_get_ecpkparameters (22 branches, 352 accumulated undiscovered branches)
 * and other 0% covered EC functions identified in coverage analysis
 * 
 * Semantic diversity from existing harnesses:
 * - harness_000 (SSL/TLS APIs)
 * - harness_001 (BN module)
 * - harness_002 (EVP asymmetric encryption)
 * - harness_003 (PEM/X509 certificate handling - has some EC but in certificate context)
 * - harness_004 (PKCS12 operations)
 * - harness_005 (AES symmetric encryption)
 * - harness_006 (hash functions and other symmetric encryption)
 * - harness_007 (SSL_accept with memory BIOs)
 * 
 * This harness focuses specifically on ECC operations with various curves
 * (P-256, P-384, P-521) and comprehensive ECC functionality testing.
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

// Array of common EC curve NIDs to test
static const int ec_curve_nids[] = {
    NID_X9_62_prime256v1,    // P-256
    NID_secp384r1,           // P-384
    NID_secp521r1,           // P-521
    NID_secp224r1,           // P-224
    NID_X9_62_prime192v1,    // P-192 (prime192v1)
    NID_X25519,              // X25519 (Curve25519)
    NID_X448,                // X448
    NID_ED25519,             // Ed25519
    NID_ED448                // Ed448
};

static const size_t num_curves = sizeof(ec_curve_nids) / sizeof(ec_curve_nids[0]);
// Test EC key generation and basic operations
static void test_ec_key_generation(FuzzedDataProvider& fdp, int curve_nid) {
    // Create EC key
    EC_KEY* ec_key = EC_KEY_new_by_curve_name(curve_nid);
    if (ec_key == NULL) {
        return;
    }
    
    // Generate key pair
    if (EC_KEY_generate_key(ec_key) != 1) {
        EC_KEY_free(ec_key);
        return;
    }
    
    // Test getting group
    const EC_GROUP* group = EC_KEY_get0_group(ec_key);
    if (group == NULL) {
        EC_KEY_free(ec_key);
        return;
    }
    
    // Test getting public key
    const EC_POINT* pub_key = EC_KEY_get0_public_key(ec_key);
    if (pub_key != NULL) {
        // Test point operations
        EC_POINT* point_copy = EC_POINT_dup(pub_key, group);
        if (point_copy != NULL) {
            EC_POINT_free(point_copy);
        }
    }
    
    // Test getting private key
    const BIGNUM* priv_key = EC_KEY_get0_private_key(ec_key);
    if (priv_key != NULL) {
        // Can create a copy of the private key if needed
        BIGNUM* priv_copy = BN_dup(priv_key);
        if (priv_copy != NULL) {
            BN_free(priv_copy);
        }
    }
    
    // Test EC_GROUP operations - specifically target EC_GROUP_get_ecpkparameters
    ECPKPARAMETERS* ecpkparams = EC_GROUP_get_ecpkparameters(group, NULL);
    if (ecpkparams != NULL) {
        // Create new group from parameters
        EC_GROUP* new_group = EC_GROUP_new_from_ecpkparameters(ecpkparams);
        if (new_group != NULL) {
            EC_GROUP_free(new_group);
        }
        ECPKPARAMETERS_free(ecpkparams);
    }
    
    // Test EC_GROUP_get_ecparameters as well
    ECPARAMETERS* ecparams = EC_GROUP_get_ecparameters(group, NULL);
    if (ecparams != NULL) {
        ECPARAMETERS_free(ecparams);
    }
    
    EC_KEY_free(ec_key);
}

// Test ECDH key exchange
static void test_ecdh_key_exchange(FuzzedDataProvider& fdp, int curve_nid) {
    // Create two EC keys for key exchange
    EC_KEY* key1 = EC_KEY_new_by_curve_name(curve_nid);
    EC_KEY* key2 = EC_KEY_new_by_curve_name(curve_nid);
    
    if (key1 == NULL || key2 == NULL) {
        if (key1) EC_KEY_free(key1);
        if (key2) EC_KEY_free(key2);
        return;
    }
    
    // Generate key pairs
    if (EC_KEY_generate_key(key1) != 1 || EC_KEY_generate_key(key2) != 1) {
        EC_KEY_free(key1);
        EC_KEY_free(key2);
        return;
    }
    
    const EC_GROUP* group = EC_KEY_get0_group(key1);
    const EC_POINT* pub_key2 = EC_KEY_get0_public_key(key2);
    const BIGNUM* priv_key1 = EC_KEY_get0_private_key(key1);
    
    if (group == NULL || pub_key2 == NULL || priv_key1 == NULL) {
        EC_KEY_free(key1);
        EC_KEY_free(key2);
        return;
    }
    
    // Compute shared secret using ECDH
    size_t field_size = EC_GROUP_get_degree(group);
    size_t secret_len = (field_size + 7) / 8; // Convert bits to bytes
    
    std::vector<unsigned char> secret(secret_len);
    int secret_size = ECDH_compute_key(secret.data(), secret.size(), pub_key2, key1, NULL);
    
    // Also test with EVP_PKEY API for ECDH
    EVP_PKEY* pkey1 = EVP_PKEY_new();
    EVP_PKEY* pkey2 = EVP_PKEY_new();
    
    if (pkey1 != NULL && pkey2 != NULL) {
        EVP_PKEY_assign_EC_KEY(pkey1, EC_KEY_dup(key1));
        EVP_PKEY_assign_EC_KEY(pkey2, EC_KEY_dup(key2));
        
        // Create context for key exchange
        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(pkey1, NULL);
        if (ctx != NULL) {
            if (EVP_PKEY_derive_init(ctx) > 0) {
                if (EVP_PKEY_derive_set_peer(ctx, pkey2) > 0) {
                    size_t derived_len = 0;
                    // Get required buffer size
                    EVP_PKEY_derive(ctx, NULL, &derived_len);
                    
                    if (derived_len > 0 && derived_len <= 1024) {
                        std::vector<unsigned char> derived(derived_len);
                        EVP_PKEY_derive(ctx, derived.data(), &derived_len);
                    }
                }
            }
            EVP_PKEY_CTX_free(ctx);
        }
        
        EVP_PKEY_free(pkey1);
        EVP_PKEY_free(pkey2);
    }
    
    EC_KEY_free(key1);
    EC_KEY_free(key2);
}

// Test ECDSA signing and verification
static void test_ecdsa_sign_verify(FuzzedDataProvider& fdp, int curve_nid) {
    // Create EC key
    EC_KEY* ec_key = EC_KEY_new_by_curve_name(curve_nid);
    if (ec_key == NULL) {
        return;
    }
    
    // Generate key pair
    if (EC_KEY_generate_key(ec_key) != 1) {
        EC_KEY_free(ec_key);
        return;
    }
    
    // Create digest to sign (using fuzzer data)
    size_t digest_len = fdp.ConsumeIntegralInRange<size_t>(20, 64); // Common digest sizes
    std::vector<unsigned char> digest = fdp.ConsumeBytes<unsigned char>(digest_len);
    
    if (digest.empty()) {
        EC_KEY_free(ec_key);
        return;
    }
    
    // Sign the digest
    ECDSA_SIG* signature = ECDSA_do_sign(digest.data(), digest.size(), ec_key);
    if (signature != NULL) {
        // Verify the signature
        int verify_result = ECDSA_do_verify(digest.data(), digest.size(), signature, ec_key);
        
        // Also test with different data (should fail verification)
        if (fdp.remaining_bytes() > digest_len) {
            std::vector<unsigned char> wrong_digest = fdp.ConsumeBytes<unsigned char>(digest_len);
            if (!wrong_digest.empty()) {
                ECDSA_do_verify(wrong_digest.data(), wrong_digest.size(), signature, ec_key);
            }
        }
        
        ECDSA_SIG_free(signature);
    }
    
    // Test with EVP_PKEY API as well
    EVP_PKEY* pkey = EVP_PKEY_new();
    if (pkey != NULL) {
        EVP_PKEY_assign_EC_KEY(pkey, EC_KEY_dup(ec_key));
        
        // Create signing context
        EVP_PKEY_CTX* sign_ctx = EVP_PKEY_CTX_new(pkey, NULL);
        if (sign_ctx != NULL) {
            if (EVP_PKEY_sign_init(sign_ctx) > 0) {
                size_t sig_len = 0;
                // Get signature length
                EVP_PKEY_sign(sign_ctx, NULL, &sig_len, digest.data(), digest.size());
                
                if (sig_len > 0 && sig_len <= 1024) {
                    std::vector<unsigned char> evp_sig(sig_len);
                    EVP_PKEY_sign(sign_ctx, evp_sig.data(), &sig_len, digest.data(), digest.size());
                    
                    // Verify with EVP
                    EVP_PKEY_CTX* verify_ctx = EVP_PKEY_CTX_new(pkey, NULL);
                    if (verify_ctx != NULL) {
                        if (EVP_PKEY_verify_init(verify_ctx) > 0) {
                            EVP_PKEY_verify(verify_ctx, evp_sig.data(), sig_len, digest.data(), digest.size());
                        }
                        EVP_PKEY_CTX_free(verify_ctx);
                    }
                }
            }
            EVP_PKEY_CTX_free(sign_ctx);
        }
        
        EVP_PKEY_free(pkey);
    }
    
    EC_KEY_free(ec_key);
}

// Test EC point operations
static void test_ec_point_operations(FuzzedDataProvider& fdp, int curve_nid) {
    // Create EC group
    EC_GROUP* group = EC_GROUP_new_by_curve_name(curve_nid);
    if (group == NULL) {
        return;
    }
    
    // Create points
    EC_POINT* point1 = EC_POINT_new(group);
    EC_POINT* point2 = EC_POINT_new(group);
    EC_POINT* point_result = EC_POINT_new(group);
    
    if (point1 == NULL || point2 == NULL || point_result == NULL) {
        EC_POINT_free(point1);
        EC_POINT_free(point2);
        EC_POINT_free(point_result);
        EC_GROUP_free(group);
        return;
    }
    
    // Create random BIGNUMs for point multiplication
    BIGNUM* scalar1 = BN_new();
    BIGNUM* scalar2 = BN_new();
    
    if (scalar1 != NULL && scalar2 != NULL) {
        // Generate random scalars from fuzzer data
        size_t bn_size = fdp.ConsumeIntegralInRange<size_t>(16, 64);
        std::vector<unsigned char> bn_data = fdp.ConsumeBytes<unsigned char>(bn_size);
        
        if (!bn_data.empty()) {
            BN_bin2bn(bn_data.data(), bn_data.size() / 2, scalar1);
            BN_bin2bn(bn_data.data() + bn_data.size() / 2, bn_data.size() / 2, scalar2);
            
            // Set point1 to generator * scalar1
            if (EC_POINT_mul(group, point1, scalar1, NULL, NULL, NULL)) {
                // Set point2 to generator * scalar2
                if (EC_POINT_mul(group, point2, scalar2, NULL, NULL, NULL)) {
                    // Test point addition: point_result = point1 + point2
                    EC_POINT_add(group, point_result, point1, point2, NULL);
                    
                    // Test point doubling: point_result = 2 * point1
                    EC_POINT_dbl(group, point_result, point1, NULL);
                    
                    // Test point inversion: point_result = -point1
                    EC_POINT_invert(group, point1, NULL);
                    
                    // Test point copy
                    EC_POINT_copy(point_result, point1);
                    
                    // Test if point is at infinity
                    EC_POINT_is_at_infinity(group, point1);
                    
                    // Test if point is on curve
                    EC_POINT_is_on_curve(group, point1, NULL);
                    
                    // Test point equality
                    EC_POINT_cmp(group, point1, point2, NULL);
                }
            }
        }
        
        BN_free(scalar1);
        BN_free(scalar2);
    }
    
    // Test point serialization
    point_conversion_form_t forms[] = {
        POINT_CONVERSION_COMPRESSED,
        POINT_CONVERSION_UNCOMPRESSED,
        POINT_CONVERSION_HYBRID
    };
    
    for (size_t i = 0; i < sizeof(forms) / sizeof(forms[0]); i++) {
        size_t buf_len = EC_POINT_point2oct(group, point1, forms[i], NULL, 0, NULL);
        if (buf_len > 0 && buf_len <= 1024) {
            std::vector<unsigned char> buf(buf_len);
            EC_POINT_point2oct(group, point1, forms[i], buf.data(), buf_len, NULL);
            
            // Try to convert back
            EC_POINT* point_reconstructed = EC_POINT_new(group);
            if (point_reconstructed != NULL) {
                EC_POINT_oct2point(group, point_reconstructed, buf.data(), buf_len, NULL);
                EC_POINT_free(point_reconstructed);
            }
        }
    }
    
    EC_POINT_free(point1);
    EC_POINT_free(point2);
    EC_POINT_free(point_result);
    EC_GROUP_free(group);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Too small for meaningful EC testing
    }
    
    initialize_openssl();
    
    FuzzedDataProvider fdp(data, size);
    
    // Clear any previous errors
    ERR_clear_error();
    
    // Consume test type from fuzzer input
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume curve selection
    uint8_t curve_idx = fdp.ConsumeIntegral<uint8_t>() % num_curves;
    int curve_nid = ec_curve_nids[curve_idx];
    
    switch (test_type) {
        case 0:
            // Test EC key generation and basic operations
            test_ec_key_generation(fdp, curve_nid);
            break;
            
        case 1:
            // Test ECDH key exchange
            test_ecdh_key_exchange(fdp, curve_nid);
            break;
            
        case 2:
            // Test ECDSA signing and verification
            test_ecdsa_sign_verify(fdp, curve_nid);
            break;
            
        case 3:
            // Test EC point operations
            test_ec_point_operations(fdp, curve_nid);
            break;
    }
    
    // Clear any errors that occurred during testing
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_008
