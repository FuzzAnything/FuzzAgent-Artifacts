#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/bn.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace harness_001 {
/*
 * Fuzzing harness for OpenSSL BN (Bignum) module operations
 * Targets: BN prime number generation, modular exponentiation, arithmetic operations
 * Specifically targets BN_generate_prime_ex (900 undiscovered branches) and related functions
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

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Too small for meaningful BN testing
    }
    
    initialize_openssl();
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    int bits = fdp.ConsumeIntegralInRange<int>(8, 1024);  // Reasonable bit sizes
    int safe = fdp.ConsumeBool() ? 1 : 0;  // 0 for regular prime, 1 for safe prime
    int nchecks = fdp.ConsumeIntegralInRange<int>(1, 100);  // Prime checks
    
    // Create BN context for operations
    BN_CTX* ctx = BN_CTX_new();
    if (ctx == NULL) {
        return 0;
    }
    
    BN_CTX_start(ctx);
    
    // Create some BIGNUMs for testing
    BIGNUM* a = BN_CTX_get(ctx);
    BIGNUM* b = BN_CTX_get(ctx);
    BIGNUM* c = BN_CTX_get(ctx);
    BIGNUM* d = BN_CTX_get(ctx);
    BIGNUM* e = BN_CTX_get(ctx);
    BIGNUM* m = BN_CTX_get(ctx);  // modulus
    BIGNUM* r = BN_CTX_get(ctx);  // result
    
    if (a == NULL || b == NULL || c == NULL || d == NULL || e == NULL || m == NULL || r == NULL) {
        BN_CTX_end(ctx);
        BN_CTX_free(ctx);
        return 0;
    }
    
    // Generate random BIGNUMs from fuzzer data
    size_t bn_data_size = fdp.ConsumeIntegralInRange<size_t>(16, 256);
    std::vector<uint8_t> bn_data = fdp.ConsumeBytes<uint8_t>(bn_data_size);
    
    // Initialize some BIGNUMs with random data
    BN_bin2bn(bn_data.data(), bn_data.size() / 2, a);
    BN_bin2bn(bn_data.data() + bn_data.size() / 2, bn_data.size() / 2, b);
    
    // Generate a random modulus (non-zero)
    if (BN_is_zero(b)) {
        BN_add_word(b, 1);
    }
    
    // Set m to a reasonable modulus
    BN_copy(m, b);
    if (BN_is_zero(m)) {
        BN_add_word(m, 1);
    }
    
    switch (test_type) {
        case 0: {
            // Test BN_generate_prime_ex - primary target with 900 undiscovered branches
            BIGNUM* prime = BN_new();
            if (prime != NULL) {
                // Consume optional 'add' and 'rem' parameters
                BIGNUM* add_param = NULL;
                BIGNUM* rem_param = NULL;
                
                // Sometimes use add/rem parameters
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 32) {
                    std::vector<uint8_t> add_data = fdp.ConsumeBytes<uint8_t>(16);
                    add_param = BN_bin2bn(add_data.data(), add_data.size(), NULL);
                }
                
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 32) {
                    std::vector<uint8_t> rem_data = fdp.ConsumeBytes<uint8_t>(16);
                    rem_param = BN_bin2bn(rem_data.data(), rem_data.size(), NULL);
                }
                
                // Generate prime with various parameters
                BN_GENCB* cb = NULL;
                if (fdp.ConsumeBool()) {
                    cb = BN_GENCB_new();
                }
                
                int ret = BN_generate_prime_ex(prime, bits, safe, add_param, rem_param, cb);
                
                if (add_param != NULL) BN_free(add_param);
                if (rem_param != NULL) BN_free(rem_param);
                if (cb != NULL) BN_GENCB_free(cb);
                
                if (ret == 1) {
                    // Verify it's actually prime
                    BN_is_prime_ex(prime, nchecks, ctx, NULL);
                }
                
                BN_free(prime);
            }
            break;
        }
        
        case 1: {
            // Test modular exponentiation operations
            // Generate random exponent
            size_t exp_size = fdp.ConsumeIntegralInRange<size_t>(1, 64);
            std::vector<uint8_t> exp_data = fdp.ConsumeBytes<uint8_t>(exp_size);
            BIGNUM* exp = BN_bin2bn(exp_data.data(), exp_data.size(), NULL);
            
            if (exp != NULL && !BN_is_zero(m)) {
                // Test different modular exponentiation functions
                uint8_t exp_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                
                switch (exp_type) {
                    case 0:
                        BN_mod_exp(r, a, exp, m, ctx);
                        break;
                    case 1:
                        BN_mod_exp_mont(r, a, exp, m, ctx, NULL);
                        break;
                    case 2:
                        BN_mod_exp_mont_consttime(r, a, exp, m, ctx, NULL);
                        break;
                    case 3:
                        BN_mod_exp_simple(r, a, exp, m, ctx);
                        break;
                }
                
                BN_free(exp);
            }
            break;
        }
        
        case 2: {
            // Test basic arithmetic operations
            uint8_t arith_type = fdp.ConsumeIntegral<uint8_t>() % 6;
            
            switch (arith_type) {
                case 0:
                    BN_add(r, a, b);
                    break;
                case 1:
                    BN_sub(r, a, b);
                    break;
                case 2:
                    BN_mul(r, a, b, ctx);
                    break;
                case 3:
                    if (!BN_is_zero(b)) {
                        BN_div(c, d, a, b, ctx);
                    }
                    break;
                case 4:
                    BN_sqr(r, a, ctx);
                    break;
                case 5: {
                    // Test word operations
                    BN_ULONG w = fdp.ConsumeIntegral<BN_ULONG>();
                    BN_add_word(a, w);
                    BN_sub_word(a, w);
                    BN_mul_word(a, w);
                    if (w != 0) {
                        BN_div_word(a, w);
                    }
                    break;
                }
            }
            break;
        }
        
        case 3: {
            // Test prime checking functions
            BIGNUM* test_prime = BN_new();
            if (test_prime != NULL) {
                // Generate or use existing number for prime testing
                if (fdp.ConsumeBool()) {
                    // Use existing a as test number
                    BN_copy(test_prime, a);
                } else {
                    // Generate a new random number
                    size_t prime_test_size = fdp.ConsumeIntegralInRange<size_t>(16, 128);
                    std::vector<uint8_t> prime_test_data = fdp.ConsumeBytes<uint8_t>(prime_test_size);
                    BN_bin2bn(prime_test_data.data(), prime_test_data.size(), test_prime);
                }
                
                // Make sure it's positive for prime testing
                BN_set_negative(test_prime, 0);
                
                // Test different prime checking functions
                uint8_t prime_check_type = fdp.ConsumeIntegral<uint8_t>() % 3;
                
                switch (prime_check_type) {
                    case 0:
                        BN_is_prime_ex(test_prime, nchecks, ctx, NULL);
                        break;
                    case 1:
                        BN_is_prime_fasttest_ex(test_prime, nchecks, ctx, 1, NULL);
                        break;
                    case 2:
                        BN_check_prime(test_prime, ctx, NULL);
                        break;
                }
                
                BN_free(test_prime);
            }
            break;
        }
        
        case 4: {
            // Test random number generation
            uint8_t rand_type = fdp.ConsumeIntegral<uint8_t>() % 3;
            int top = fdp.ConsumeBool() ? BN_RAND_TOP_ONE : BN_RAND_TOP_ANY;
            int bottom = fdp.ConsumeBool() ? BN_RAND_BOTTOM_ODD : BN_RAND_BOTTOM_ANY;
            
            switch (rand_type) {
                case 0:
                    BN_rand(a, bits, top, bottom);
                    break;
                case 1:
                    BN_priv_rand(a, bits, top, bottom);
                    break;
                case 2:
                    BN_pseudo_rand(a, bits, top, bottom);
                    break;
            }
            
            // Test BN_rand_range
            if (!BN_is_zero(b)) {
                BN_rand_range(a, b);
            }
            break;
        }
        
        case 5: {
            // Test modular arithmetic
            if (!BN_is_zero(m)) {
                uint8_t mod_type = fdp.ConsumeIntegral<uint8_t>() % 5;
                
                switch (mod_type) {
                    case 0:
                        BN_mod_add(r, a, b, m, ctx);
                        break;
                    case 1:
                        BN_mod_sub(r, a, b, m, ctx);
                        break;
                    case 2:
                        BN_mod_mul(r, a, b, m, ctx);
                        break;
                    case 3:
                        BN_mod_sqr(r, a, m, ctx);
                        break;
                    case 4: {
                        // Modular inverse (needs coprime numbers)
                        BIGNUM* inv_result = BN_mod_inverse(r, a, m, ctx);
                        // Result may be NULL if inverse doesn't exist
                        break;
                    }
                }
            }
            break;
        }
        
        case 6: {
            // Test comparison and bit operations
            int cmp_result = BN_cmp(a, b);
            
            // Test bit operations
            int n = fdp.ConsumeIntegralInRange<int>(0, BN_num_bits(a));
            BN_is_bit_set(a, n);
            BN_set_bit(a, n);
            BN_clear_bit(a, n);
            BN_mask_bits(a, n);
            
            // Test shifts
            BN_lshift(c, a, n);
            BN_rshift(c, a, n);
            
            // Test gcd and modular inverse
            BN_gcd(r, a, b, ctx);
            
            break;
        }
        
        case 7: {
            // Test Montgomery operations
            BN_MONT_CTX* mont = BN_MONT_CTX_new();
            if (mont != NULL && !BN_is_zero(m)) {
                // Set up Montgomery context
                BN_MONT_CTX_set(mont, m, ctx);
                
                // Convert to Montgomery form
                BN_to_montgomery(a, a, mont, ctx);
                BN_to_montgomery(b, b, mont, ctx);
                
                // Perform Montgomery multiplication
                BN_mod_mul_montgomery(r, a, b, mont, ctx);
                
                // Convert back from Montgomery form
                BN_from_montgomery(r, r, mont, ctx);
                
                BN_MONT_CTX_free(mont);
            }
            break;
        }
    }
    
    // Clear any accumulated errors
    ERR_clear_error();
    
    // Clean up
    BN_CTX_end(ctx);
    BN_CTX_free(ctx);
    
    return 0;
}

} // namespace harness_001
