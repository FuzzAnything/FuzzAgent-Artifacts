#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/dh.h>
#include <openssl/dsa.h>
#include <openssl/err.h>
#include <openssl/bn.h>
#include <openssl/objects.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <sys/resource.h>
#include <signal.h>
#include <chrono>
#include <thread>

namespace harness_011 {
/*
 * Fuzzing harness for OpenSSL Diffie-Hellman (DH) key exchange and DSA digital signatures
 * Targets: DH key exchange lifecycle and DSA signature generation/verification
 * Coverage goal: Exercise DH module (0.42% coverage) and DSA module (0.93% coverage)
 * 
 * Target APIs:
 * DH APIs:
 * - DH_new(): Create DH context
 * - DH_generate_parameters_ex(): Generate DH parameters
 * - DH_generate_key(): Generate DH key pair
 * - DH_compute_key(): Compute shared secret
 * - DH_free(): Cleanup DH context
 * 
 * DSA APIs:
 * - DSA_new(): Create DSA context
 * - DSA_generate_parameters_ex(): Generate DSA parameters
 * - DSA_generate_key(): Generate DSA key pair
 * - DSA_sign(): Create DSA signature
 * - DSA_verify(): Verify DSA signature
 * - DSA_free(): Cleanup DSA context
 * 
 * Invocation sequence:
 * 1. DH Key Exchange Cycle:
 *    a. DH_new() to create DH context for party A
 *    b. DH_generate_parameters_ex() with random prime length
 *    c. DH_generate_key() to create key pair for party A
 *    d. Repeat a-c for party B (peer)
 *    e. DH_compute_key() on both sides to compute shared secret
 *    f. DH_free() cleanup both contexts
 * 
 * 2. DSA Signature Cycle:
 *    a. DSA_new() to create DSA context
 *    b. DSA_generate_parameters_ex() to generate DSA parameters
 *    c. DSA_generate_key() to create signing key pair
 *    d. DSA_sign() to sign random data
 *    e. DSA_verify() to verify the signature
 *    f. DSA_free() cleanup
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
 * - harness_010.cpp: Hash functions (SHA family)
 * - harness_011.cpp: DH key exchange and DSA digital signatures (NEW)
 */

// Timeout handler for long-running operations
static volatile sig_atomic_t timeout_flag = 0;

void timeout_handler(int sig) {
    (void)sig;
    timeout_flag = 1;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for algorithm selections and some data
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Set up timeout mechanism (2 second timeout for faster fuzzing)
    struct sigaction sa;
    struct sigaction old_sa;
    
    sa.sa_handler = timeout_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    timeout_flag = 0;
    sigaction(SIGALRM, &sa, &old_sa);
    alarm(2); // 2 second timeout for faster fuzzing
    
    // Initialize OpenSSL error strings
    ERR_load_crypto_strings();
    
    // Consume operation choice: 0 = DH key exchange, 1 = DSA signatures
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    if (operation == 0) {
        // ==============================
        // DH Key Exchange Cycle
        // ==============================
        
        // Check for timeout before starting
        if (timeout_flag) {
            alarm(0);
            sigaction(SIGALRM, &old_sa, NULL);
            return 0;
        }
        
        // Consume DH parameters - use smaller sizes for performance
        int prime_len = fdp.ConsumeIntegralInRange<int>(128, 256); // Further reduced for performance
        int generator = fdp.ConsumeIntegralInRange<int>(2, 3); // Reduced generator range
        
        // Create DH context for party A
        DH* dh_a = DH_new();
        if (dh_a == NULL) {
            alarm(0);
            sigaction(SIGALRM, &old_sa, NULL);
            return 0;
        }
        
        // Generate DH parameters for party A
        if (DH_generate_parameters_ex(dh_a, prime_len, generator, NULL) != 1) {
            DH_free(dh_a);
            alarm(0);
            sigaction(SIGALRM, &old_sa, NULL);
            return 0;
        }
        
        // Generate key pair for party A
        if (DH_generate_key(dh_a) != 1) {
            DH_free(dh_a);
            alarm(0);
            sigaction(SIGALRM, &old_sa, NULL);
            return 0;
        }
        
        // Create DH context for party B (peer)
        DH* dh_b = DH_new();
        if (dh_b == NULL) {
            DH_free(dh_a);
            alarm(0);
            sigaction(SIGALRM, &old_sa, NULL);
            return 0;
        }
        
        // Use same parameters for party B (typical DH exchange)
        const BIGNUM* p = NULL;
        const BIGNUM* q = NULL;
        const BIGNUM* g = NULL;
        DH_get0_pqg(dh_a, &p, &q, &g);
        
        BIGNUM* p_copy = BN_dup(p);
        BIGNUM* g_copy = BN_dup(g);
        if (p_copy == NULL || g_copy == NULL) {
            BN_free(p_copy);
            BN_free(g_copy);
            DH_free(dh_b);
            DH_free(dh_a);
            alarm(0);
            sigaction(SIGALRM, &old_sa, NULL);
            return 0;
        }
        
        // Set same parameters for party B
        if (DH_set0_pqg(dh_b, p_copy, NULL, g_copy) != 1) {
            BN_free(p_copy);
            BN_free(g_copy);
            DH_free(dh_b);
            DH_free(dh_a);
            alarm(0);
            sigaction(SIGALRM, &old_sa, NULL);
            return 0;
        }
        
        // Generate key pair for party B
        if (DH_generate_key(dh_b) != 1) {
            DH_free(dh_b);
            DH_free(dh_a);
            alarm(0);
            sigaction(SIGALRM, &old_sa, NULL);
            return 0;
        }
        
        // Get public keys for computation
        const BIGNUM* pub_key_a = NULL;
        const BIGNUM* pub_key_b = NULL;
        DH_get0_key(dh_a, &pub_key_a, NULL);
        DH_get0_key(dh_b, &pub_key_b, NULL);
        
        if (pub_key_a == NULL || pub_key_b == NULL) {
            DH_free(dh_b);
            DH_free(dh_a);
            alarm(0);
            sigaction(SIGALRM, &old_sa, NULL);
            return 0;
        }
        
        // Compute shared secret from A's perspective (using B's public key)
        int key_size_a = DH_size(dh_a);
        if (key_size_a > 0) {
            std::vector<unsigned char> shared_secret_a(key_size_a);
            // Don't check return value - could be -1 for various reasons, still valid to exercise
            DH_compute_key(shared_secret_a.data(), pub_key_b, dh_a);
        }
        
        // Compute shared secret from B's perspective (using A's public key)
        int key_size_b = DH_size(dh_b);
        if (key_size_b > 0) {
            std::vector<unsigned char> shared_secret_b(key_size_b);
            // Don't check return value - could be -1 for various reasons, still valid to exercise
            DH_compute_key(shared_secret_b.data(), pub_key_a, dh_b);
        }
        
        // Cleanup
        DH_free(dh_b);
        DH_free(dh_a);
        
    } else {
        // ==============================
        // DSA Signature Cycle
        // ==============================
        
        // Check for timeout before starting
        if (timeout_flag) {
            alarm(0);
            sigaction(SIGALRM, &old_sa, NULL);
            return 0;
        }
        
        // Consume DSA parameters - use smaller sizes for performance
        int bits = fdp.ConsumeIntegralInRange<int>(256, 512); // Reduced from 512-2048 for performance
        
        // Create DSA context
        DSA* dsa = DSA_new();
        if (dsa == NULL) {
            alarm(0);
            sigaction(SIGALRM, &old_sa, NULL);
            return 0;
        }
        
        // Generate DSA parameters with smaller seed for performance
        size_t seed_len = fdp.ConsumeIntegralInRange<size_t>(0, 32); // Further reduced for performance
        std::vector<unsigned char> seed = fdp.ConsumeBytes<unsigned char>(seed_len);
        
        int counter_ret = 0;
        unsigned long h_ret = 0;
        
        // Try to generate parameters (may fail with random seed, that's OK)
        DSA_generate_parameters_ex(dsa, bits, 
                                  seed.empty() ? NULL : seed.data(), 
                                  seed.size(),
                                  &counter_ret, &h_ret, NULL);
        
        // Generate DSA key pair (even if parameters didn't generate perfectly)
        // Some parameter validation happens inside DSA_generate_key
        DSA_generate_key(dsa);
        
        // Prepare data to sign (consume remaining input, limit size for performance)
        size_t max_data_size = fdp.ConsumeIntegralInRange<size_t>(0, 1024); // Limit data size
        std::vector<unsigned char> data_to_sign = fdp.ConsumeBytes<unsigned char>(max_data_size);
        
        if (!data_to_sign.empty()) {
            // Get DSA signature size - check for valid return value
            int dsa_size = DSA_size(dsa);
            if (dsa_size <= 0) {
                // Invalid DSA size, cannot allocate buffer - skip signing
                DSA_free(dsa);
                alarm(0);
                sigaction(SIGALRM, &old_sa, NULL);
                return 0;
            }
            
            // Use dynamic allocation to avoid stack issues with variable size
            unsigned char* sig_buf = new unsigned char[dsa_size];
            unsigned int sig_len = 0;
            
            // Try to sign the data
            // DSA_sign expects NID_sha1, NID_sha224, NID_sha256, etc.
            // We'll use NID_sha256 as a reasonable default
            int type = NID_sha256;
            
            DSA_sign(type, data_to_sign.data(), data_to_sign.size(),
                    sig_buf, &sig_len, dsa);
            
            // If signing succeeded, try to verify
            if (sig_len > 0 && sig_len <= static_cast<unsigned int>(dsa_size)) {
                DSA_verify(type, data_to_sign.data(), data_to_sign.size(),
                          sig_buf, sig_len, dsa);
            }
            
            // Cleanup allocated buffer
            delete[] sig_buf;
        }
        
        // Cleanup
        DSA_free(dsa);
    }
    
    // Cleanup timeout mechanism
    alarm(0);
    sigaction(SIGALRM, &old_sa, NULL);
    
    return 0;
}

} // namespace harness_011
