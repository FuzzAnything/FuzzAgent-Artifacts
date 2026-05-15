#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/srp.h>
#include <openssl/bn.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace harness_009 {
/*
 * Fuzzing harness for OpenSSL Secure Remote Password (SRP) protocol operations
 * Targets: SRP server/client context creation, SRP parameter generation, 
 *          SRP verification, SRP user database operations, and related SRP functions
 * Specifically targets SRP module identified as most critical uncovered area 
 * with 1000+ branches of undiscovered complexity.
 * 
 * Semantic diversity from existing harnesses:
 * - harness_000 (SSL/TLS APIs)
 * - harness_001 (BN module)
 * - harness_002 (EVP asymmetric encryption)
 * - harness_003 (PEM/X509 certificate handling)
 * - harness_004 (PKCS12 operations)
 * - harness_005 (AES symmetric encryption)
 * - harness_006 (hash functions and other symmetric encryption)
 * - harness_007 (SSL_accept with memory BIOs)
 * - harness_008 (ECC operations)
 * 
 * This harness focuses specifically on SRP protocol operations including
 * verifier creation, client/server key calculation, verification functions,
 * and SRP user database (VBASE) operations.
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

// Array of SRP parameter set IDs to test
static const char* srp_param_ids[] = {
    "1024",   // 1024-bit
    "1536",   // 1536-bit  
    "2048",   // 2048-bit
    "3072",   // 3072-bit
    "4096",   // 4096-bit
    "8192"    // 8192-bit
};

static const size_t num_param_ids = sizeof(srp_param_ids) / sizeof(srp_param_ids[0]);

// Test SRP verifier creation and basic SRP operations
static void test_srp_verifier_creation(FuzzedDataProvider& fdp, const char* param_id) {
    // Get default SRP parameters for the given ID
    SRP_gN* gn = SRP_get_default_gN(param_id);
    if (gn == NULL) {
        return;
    }
    
    // Consume username and password from fuzzer input
    std::string username = fdp.ConsumeRandomLengthString(64);
    std::string password = fdp.ConsumeRandomLengthString(64);
    
    // Create verifier (server side operation)
    BIGNUM* salt = NULL;
    BIGNUM* verifier = NULL;
    
    if (SRP_create_verifier_BN(username.c_str(), password.c_str(), 
                              &salt, &verifier, gn->N, gn->g)) {
        // Test SRP_Calc_x (client side: x = H(salt | H(username | ":" | password)))
        BIGNUM* x = SRP_Calc_x(salt, username.c_str(), password.c_str());
        
        // Generate random client secret 'a' and calculate A = g^a mod N
        std::vector<uint8_t> client_random = fdp.ConsumeBytes<uint8_t>(32);
        if (client_random.size() >= 16) {
            BIGNUM* a = BN_bin2bn(client_random.data(), client_random.size(), NULL);
            if (a != NULL && BN_is_zero(a) == 0) {
                BIGNUM* A = SRP_Calc_A(a, gn->N, gn->g);
                if (A != NULL) {
                    // Verify A mod N
                    SRP_Verify_A_mod_N(A, gn->N);
                    
                    // Generate random server secret 'b' and calculate B = (k*v + g^b) mod N
                    std::vector<uint8_t> server_random = fdp.ConsumeBytes<uint8_t>(32);
                    if (server_random.size() >= 16) {
                        BIGNUM* b = BN_bin2bn(server_random.data(), server_random.size(), NULL);
                        if (b != NULL && BN_is_zero(b) == 0) {
                            BIGNUM* B = SRP_Calc_B(b, gn->N, gn->g, verifier);
                            if (B != NULL) {
                                // Verify B mod N
                                SRP_Verify_B_mod_N(B, gn->N);
                                
                                // Calculate u = H(A | B)
                                BIGNUM* u = SRP_Calc_u(A, B, gn->N);
                                if (u != NULL) {
                                    // Calculate client session key
                                    // Fix: Don't free x early, use it directly or create new BIGNUM
                                    BIGNUM* x_to_use = (x != NULL) ? x : BN_new();
                                    if (x_to_use != NULL) {
                                        BIGNUM* Kclient = SRP_Calc_client_key(gn->N, B, gn->g, 
                                                                              x_to_use, 
                                                                              a, u);
                                        if (Kclient != NULL) {
                                            BN_free(Kclient);
                                        }
                                        // Only free x_to_use if it was newly created (not the original x)
                                        if (x == NULL && x_to_use != NULL) {
                                            BN_free(x_to_use);
                                        }
                                    }
                                    
                                    // Calculate server session key
                                    BIGNUM* Kserver = SRP_Calc_server_key(A, verifier, u, b, gn->N);
                                    if (Kserver != NULL) {
                                        BN_free(Kserver);
                                    }
                                    
                                    BN_free(u);
                                }
                                
                                BN_free(B);
                            }
                            BN_free(b);
                        }
                    }
                    
                    BN_free(A);
                }
                BN_free(a);
            }
        }
        
        // Clean up x if it was created
        if (x != NULL) {
            BN_free(x);
        }
        
        // Clean up salt and verifier
        BN_free(salt);
        BN_free(verifier);
    }
}

// Test SRP VBASE (user database) operations
static void test_srp_vbase_operations(FuzzedDataProvider& fdp) {
    // Create a new SRP VBASE
    SRP_VBASE* vb = SRP_VBASE_new(NULL);
    if (vb == NULL) {
        return;
    }
    
    // Test with multiple user entries
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 100; i++) {
        std::string username = fdp.ConsumeRandomLengthString(32);
        std::string password = fdp.ConsumeRandomLengthString(32);
        
        // Create user entry
        SRP_user_pwd* user = SRP_user_pwd_new();
        if (user != NULL) {
            // Get default 1024-bit parameters
            SRP_gN* gn = SRP_get_default_gN("1024");
            if (gn != NULL) {
                // Set parameters
                SRP_user_pwd_set_gN(user, gn->g, gn->N);
                
                // Set user ID
                SRP_user_pwd_set1_ids(user, username.c_str(), NULL);
                
                // Create salt and verifier
                BIGNUM* salt = NULL;
                BIGNUM* verifier = NULL;
                if (SRP_create_verifier_BN(username.c_str(), password.c_str(),
                                          &salt, &verifier, gn->N, gn->g)) {
                    // Set salt and verifier
                    SRP_user_pwd_set0_sv(user, salt, verifier);
                    
                    // Add user to VBASE
                    SRP_VBASE_add0_user(vb, user);
                } else {
                    SRP_user_pwd_free(user);
                    if (salt) BN_free(salt);
                    if (verifier) BN_free(verifier);
                }
            } else {
                SRP_user_pwd_free(user);
            }
        }
    }
    
    // Try to retrieve users from VBASE
    if (fdp.remaining_bytes() > 50) {
        std::string lookup_user = fdp.ConsumeRandomLengthString(32);
        SRP_user_pwd* found_user = SRP_VBASE_get1_by_user(vb, const_cast<char*>(lookup_user.c_str()));
        if (found_user != NULL) {
            SRP_user_pwd_free(found_user);
        }
    }
    
    // Clean up
    SRP_VBASE_free(vb);
}

// Test SRP_create_verifier with string parameters
static void test_srp_string_verifier(FuzzedDataProvider& fdp) {
    if (fdp.remaining_bytes() < 100) {
        return;
    }
    
    std::string username = fdp.ConsumeRandomLengthString(32);
    std::string password = fdp.ConsumeRandomLengthString(32);
    
    // Use 1024-bit parameters as strings
    const char* N_1024 = "EEAF0AB9ADB38DD69C33F80AFA8FC5E86072618775FF3C0B9EA2314C9C256576D674DF7496"
                        "EA81D3383B4813D692C6E0E0D5D8E250B98BE48E495C1D6089DAD15DC7D7B46154D6B6CE8E"
                        "F4AD69B15D4982559B297BCF1885C529F566660E57EC68EDBC3C05726CC02FD4CBF4976EAA"
                        "9AFD5138FE8376435B9FC61D2FC0EB06E3";
    
    const char* g_1024 = "2";
    
    char* salt = NULL;
    char* verifier = NULL;
    
    // Create verifier using string API
    char* result = SRP_create_verifier(username.c_str(), password.c_str(), 
                                      &salt, &verifier, N_1024, g_1024);
    if (result != NULL) {
        OPENSSL_free(result);
    }
    if (salt != NULL) {
        OPENSSL_free(salt);
    }
    if (verifier != NULL) {
        OPENSSL_free(verifier);
    }
}

// Test various SRP utility functions
static void test_srp_utility_functions(FuzzedDataProvider& fdp) {
    // Test SRP_check_known_gN_param with random values
    if (fdp.remaining_bytes() > 64) {
        std::vector<uint8_t> g_data = fdp.ConsumeBytes<uint8_t>(32);
        std::vector<uint8_t> N_data = fdp.ConsumeBytes<uint8_t>(64);
        
        if (g_data.size() > 0 && N_data.size() > 0) {
            BIGNUM* g = BN_bin2bn(g_data.data(), g_data.size(), NULL);
            BIGNUM* N = BN_bin2bn(N_data.data(), N_data.size(), NULL);
            
            if (g != NULL && N != NULL) {
                char* result = SRP_check_known_gN_param(g, N);
                if (result != NULL) {
                    OPENSSL_free(result);
                }
            }
            
            if (g != NULL) BN_free(g);
            if (N != NULL) BN_free(N);
        }
    }
    
    // Test getting different parameter sets
    for (size_t i = 0; i < num_param_ids && fdp.remaining_bytes() > 10; i++) {
        SRP_gN* gn = SRP_get_default_gN(srp_param_ids[i]);
        // Just reference to ensure it works
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 128) {
        return 0;  // Too small for meaningful SRP testing
    }
    
    initialize_openssl();
    
    FuzzedDataProvider fdp(data, size);
    
    // Clear any previous errors
    ERR_clear_error();
    
    // Determine which test to run based on first byte
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (test_type) {
        case 0: {
            // Test SRP verifier creation with different parameter sets
            size_t param_index = fdp.ConsumeIntegralInRange<size_t>(0, num_param_ids - 1);
            test_srp_verifier_creation(fdp, srp_param_ids[param_index]);
            break;
        }
        
        case 1: {
            // Test SRP VBASE (user database) operations
            test_srp_vbase_operations(fdp);
            break;
        }
        
        case 2: {
            // Test SRP string-based verifier creation
            test_srp_string_verifier(fdp);
            break;
        }
        
        case 3: {
            // Test SRP utility functions
            test_srp_utility_functions(fdp);
            break;
        }
    }
    
    // Clear any accumulated errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_009
