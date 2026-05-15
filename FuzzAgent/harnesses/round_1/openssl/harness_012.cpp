#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/dh.h>
#include <openssl/bn.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace harness_012 {
/*
 * Fuzzing harness for OpenSSL Diffie-Hellman (DH) key exchange operations
 * Targets: DH parameter generation, key generation, key exchange computation,
 *          DH parameter validation, named DH parameters, and related DH functions
 * Specifically targets DH_generate_parameters with 539 undiscovered branches
 * and other 0% covered DH functions identified in coverage analysis
 * 
 * Primary targets:
 * - DH_generate_parameters_ex: Parameter generation with various prime lengths and generators
 * - DH_check / DH_check_params: Parameter validation and safety checks
 * - DH_generate_key: Key pair generation
 * - DH_compute_key / DH_compute_key_padded: Key exchange computation
 * - DH_new_by_nid / DH_get_nid: Named parameter support (RFC7919, RFC3526)
 * - DH_get_*_*: RFC5114 predefined parameters
 * - DHparams_print / DHparams_print_fp: Parameter serialization
 * 
 * Semantic diversity from existing harnesses:
 * - harness_000 (SSL/TLS APIs - includes some DH but only in SSL context)
 * - harness_001 (BN module - math operations, not DH-specific)
 * - harness_002 (EVP asymmetric encryption - EVP wrapper APIs, not low-level DH)
 * - harness_003 (PEM/X509 certificate handling)
 * - harness_004 (PKCS12 operations)
 * - harness_005 (AES symmetric encryption)
 * - harness_006 (hash functions and other symmetric encryption)
 * - harness_007 (SSL_accept with memory BIOs)
 * - harness_008 (ECC operations - has some DH but in ECC context)
 * - harness_009 (SRP protocol)
 * - harness_010 (CMS operations)
 * - harness_011 (CMP protocol)
 * 
 * This harness focuses specifically on comprehensive DH operations with various
 * parameter sizes (512, 1024, 2048, 3072 bits), generators (2, 3, 5), and
 * full lifecycle testing from parameter generation to key exchange.
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

// Test DH parameter generation with various configurations
static void test_dh_parameter_generation(FuzzedDataProvider& fdp) {
    // Consume parameters for DH generation
    int prime_len = fdp.ConsumeIntegralInRange<int>(512, 3072);
    int generator = fdp.ConsumeIntegralInRange<int>(2, 5);
    
    // Create new DH structure
    DH* dh = DH_new();
    if (dh == NULL) {
        return;
    }
    
    // Generate DH parameters
    if (DH_generate_parameters_ex(dh, prime_len, generator, NULL) != 1) {
        DH_free(dh);
        return;
    }
    
    // Test parameter checking
    int check_codes = 0;
    DH_check(dh, &check_codes);
    
    // Test getting parameters
    const BIGNUM* p = NULL;
    const BIGNUM* q = NULL;
    const BIGNUM* g = NULL;
    DH_get0_pqg(dh, &p, &q, &g);
    
    // Test security bits calculation
    DH_security_bits(dh);
    
    // Test size calculations
    DH_size(dh);
    DH_bits(dh);
    
    DH_free(dh);
}

// Test DH key generation and exchange
static void test_dh_key_exchange(FuzzedDataProvider& fdp) {
    // First, create DH parameters
    DH* dh_params = DH_new();
    if (dh_params == NULL) {
        return;
    }
    
    int prime_len = fdp.ConsumeIntegralInRange<int>(512, 2048);
    int generator = fdp.ConsumeIntegralInRange<int>(2, 5);
    
    if (DH_generate_parameters_ex(dh_params, prime_len, generator, NULL) != 1) {
        DH_free(dh_params);
        return;
    }
    
    // Create two DH instances for key exchange (Alice and Bob)
    DH* alice_dh = DH_new();
    DH* bob_dh = DH_new();
    
    if (alice_dh == NULL || bob_dh == NULL) {
        DH_free(dh_params);
        if (alice_dh) DH_free(alice_dh);
        if (bob_dh) DH_free(bob_dh);
        return;
    }
    
    // Copy parameters to both DH instances
    const BIGNUM* p = NULL;
    const BIGNUM* q = NULL;
    const BIGNUM* g = NULL;
    DH_get0_pqg(dh_params, &p, &q, &g);
    
    BIGNUM* p_copy = BN_dup(p);
    BIGNUM* g_copy = BN_dup(g);
    
    if (p_copy == NULL || g_copy == NULL) {
        DH_free(dh_params);
        DH_free(alice_dh);
        DH_free(bob_dh);
        if (p_copy) BN_free(p_copy);
        if (g_copy) BN_free(g_copy);
        return;
    }
    
    DH_set0_pqg(alice_dh, p_copy, NULL, g_copy);
    
    // Need new copies for Bob
    BIGNUM* p_copy2 = BN_dup(p);
    BIGNUM* g_copy2 = BN_dup(g);
    
    if (p_copy2 == NULL || g_copy2 == NULL) {
        DH_free(dh_params);
        DH_free(alice_dh);
        DH_free(bob_dh);
        if (p_copy2) BN_free(p_copy2);
        if (g_copy2) BN_free(g_copy2);
        return;
    }
    
    DH_set0_pqg(bob_dh, p_copy2, NULL, g_copy2);
    
    // Generate keys for both parties
    if (DH_generate_key(alice_dh) != 1 || DH_generate_key(bob_dh) != 1) {
        DH_free(dh_params);
        DH_free(alice_dh);
        DH_free(bob_dh);
        return;
    }
    
    // Get public keys for key exchange
    const BIGNUM* alice_pub = NULL;
    const BIGNUM* bob_pub = NULL;
    DH_get0_key(alice_dh, &alice_pub, NULL);
    DH_get0_key(bob_dh, &bob_pub, NULL);
    
    if (alice_pub != NULL && bob_pub != NULL) {
        // Allocate buffers for computed keys
        int key_size = DH_size(alice_dh);
        if (key_size > 0) {
            unsigned char* alice_secret = (unsigned char*)malloc(key_size);
            unsigned char* bob_secret = (unsigned char*)malloc(key_size);
            
            if (alice_secret != NULL && bob_secret != NULL) {
                // Alice computes shared secret with Bob's public key
                int alice_secret_len = DH_compute_key(alice_secret, bob_pub, alice_dh);
                
                // Bob computes shared secret with Alice's public key
                int bob_secret_len = DH_compute_key(bob_secret, alice_pub, bob_dh);
                
                // Also test padded version
                if (alice_secret_len > 0) {
                    unsigned char* padded_secret = (unsigned char*)malloc(key_size);
                    if (padded_secret != NULL) {
                        DH_compute_key_padded(padded_secret, bob_pub, alice_dh);
                        free(padded_secret);
                    }
                }
            }
            
            if (alice_secret) free(alice_secret);
            if (bob_secret) free(bob_secret);
        }
    }
    
    DH_free(dh_params);
    DH_free(alice_dh);
    DH_free(bob_dh);
}

// Test named DH parameters (RFC7919, RFC3526)
static void test_named_dh_parameters(FuzzedDataProvider& fdp) {
    // Test various named parameter NIDs
    int nid_choice = fdp.ConsumeIntegralInRange<int>(0, 10);
    int nid = 0;
    
    switch (nid_choice) {
        case 0: nid = NID_ffdhe2048; break;
        case 1: nid = NID_ffdhe3072; break;
        case 2: nid = NID_ffdhe4096; break;
        case 3: nid = NID_ffdhe6144; break;
        case 4: nid = NID_ffdhe8192; break;
        case 5: nid = NID_modp_1536; break;
        case 6: nid = NID_modp_2048; break;
        case 7: nid = NID_modp_3072; break;
        case 8: nid = NID_modp_4096; break;
        case 9: nid = NID_modp_8192; break;
        default: nid = NID_ffdhe2048; break;
    }
    
    DH* named_dh = DH_new_by_nid(nid);
    if (named_dh != NULL) {
        // Test getting NID back
        int retrieved_nid = DH_get_nid(named_dh);
        
        // Test key generation with named parameters
        if (DH_generate_key(named_dh) == 1) {
            // Test size calculations
            DH_size(named_dh);
            DH_bits(named_dh);
            DH_security_bits(named_dh);
        }
        
        DH_free(named_dh);
    }
}

// Test RFC5114 predefined parameters
static void test_rfc5114_parameters(FuzzedDataProvider& fdp) {
    int param_choice = fdp.ConsumeIntegralInRange<int>(0, 2);
    DH* rfc_dh = NULL;
    
    switch (param_choice) {
        case 0:
            rfc_dh = DH_get_1024_160();
            break;
        case 1:
            rfc_dh = DH_get_2048_224();
            break;
        case 2:
            rfc_dh = DH_get_2048_256();
            break;
    }
    
    if (rfc_dh != NULL) {
        // Test key generation with RFC5114 parameters
        if (DH_generate_key(rfc_dh) == 1) {
            const BIGNUM* pub_key = NULL;
            const BIGNUM* priv_key = NULL;
            DH_get0_key(rfc_dh, &pub_key, &priv_key);
            
            // Test parameter validation
            int check_codes = 0;
            DH_check(rfc_dh, &check_codes);
            
            if (pub_key != NULL) {
                int pub_check_codes = 0;
                DH_check_pub_key(rfc_dh, pub_key, &pub_check_codes);
            }
        }
        
        DH_free(rfc_dh);
    }
}

// Test DH parameter validation and checking
static void test_dh_validation(FuzzedDataProvider& fdp) {
    // Create DH with parameters
    DH* dh = DH_new();
    if (dh == NULL) {
        return;
    }
    
    int prime_len = fdp.ConsumeIntegralInRange<int>(512, 2048);
    int generator = fdp.ConsumeIntegralInRange<int>(2, 5);
    
    if (DH_generate_parameters_ex(dh, prime_len, generator, NULL) != 1) {
        DH_free(dh);
        return;
    }
    
    // Test various validation functions
    int check_codes = 0;
    DH_check(dh, &check_codes);
    
    int param_check_codes = 0;
    DH_check_params(dh, &param_check_codes);
    
    // Test with _ex variants
    DH_check_ex(dh);
    DH_check_params_ex(dh);
    
    // Test public key validation if we have a key
    if (DH_generate_key(dh) == 1) {
        const BIGNUM* pub_key = NULL;
        DH_get0_key(dh, &pub_key, NULL);
        
        if (pub_key != NULL) {
            int pub_check_codes = 0;
            DH_check_pub_key(dh, pub_key, &pub_check_codes);
            DH_check_pub_key_ex(dh, pub_key);
        }
    }
    
    DH_free(dh);
}

// Test DH parameter printing/serialization
static void test_dh_printing(FuzzedDataProvider& fdp) {
    DH* dh = DH_new();
    if (dh == NULL) {
        return;
    }
    
    int prime_len = fdp.ConsumeIntegralInRange<int>(512, 1024);
    int generator = fdp.ConsumeIntegralInRange<int>(2, 5);
    
    if (DH_generate_parameters_ex(dh, prime_len, generator, NULL) != 1) {
        DH_free(dh);
        return;
    }
    
    // Test BIO printing
    BIO* bio = BIO_new(BIO_s_mem());
    if (bio != NULL) {
        DHparams_print(bio, dh);
        BIO_free(bio);
    }
    
    DH_free(dh);
}

// Test DH method operations
static void test_dh_methods(FuzzedDataProvider& fdp) {
#ifndef OPENSSL_NO_DEPRECATED_3_0
    // Create a custom DH method
    DH_METHOD* dhm = DH_meth_new("TestDHMethod", 0);
    if (dhm != NULL) {
        // Test method operations
        const char* name = DH_meth_get0_name(dhm);
        int flags = DH_meth_get_flags(dhm);
        
        // Set some dummy callbacks (they won't be called, just testing the API)
        DH_meth_set_generate_key(dhm, [](DH* dh) -> int {
            // Dummy callback
            return 0;
        });
        
        DH_meth_set_compute_key(dhm, [](unsigned char* key, const BIGNUM* pub_key, DH* dh) -> int {
            // Dummy callback  
            return 0;
        });
        
        // Create DH with custom method
        DH* dh = DH_new_method(NULL);
        if (dh != NULL) {
            if (DH_set_method(dh, dhm) == 1) {
                // Method set successfully
            }
            DH_free(dh);
        }
        
        DH_meth_free(dhm);
    }
#endif
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;  // Too small for meaningful DH testing
    }
    
    initialize_openssl();
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test type to determine which DH tests to run
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Clear any previous errors
    ERR_clear_error();
    
    switch (test_type) {
        case 0:
            test_dh_parameter_generation(fdp);
            break;
        case 1:
            test_dh_key_exchange(fdp);
            break;
        case 2:
            test_named_dh_parameters(fdp);
            break;
        case 3:
            test_rfc5114_parameters(fdp);
            break;
        case 4:
            test_dh_validation(fdp);
            break;
        case 5:
            test_dh_printing(fdp);
            break;
        // Note: test_dh_methods is not included in switch as it's deprecated
        // and might cause compilation issues
    }
    
    // Clear any errors that occurred during testing
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_012
