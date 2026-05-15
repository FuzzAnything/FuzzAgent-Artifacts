#include <stddef.h>
#include <stdint.h>
#include <openssl/bn.h>
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_009 {
/*
 * Fuzzing harness for OpenSSL Big Number (BN) modular operations
 * Primary target: BN_mod_lshift_quick (20 branches, 242 undiscovered accumulated branches)
 * Targets: BN_mod_lshift_quick, BN_mod_add, BN_mod_sub, BN_mod_mul, BN_mod_exp
 * Helper APIs: BN_new, BN_rand, BN_CTX_new, BN_CTX_free, BN_free
 * Invocation sequence: 
 *   1. Call BN_CTX_new to create BN context
 *   2. Call BN_new multiple times to create BN operands (a, b, m for modulus, r for result)
 *   3. Call BN_rand to populate operands with random values
 *   4. Call BN_mod_lshift_quick to test modular left shift (primary target)
 *   5. Call other BN modular operations (BN_mod_add, BN_mod_sub, BN_mod_mul, BN_mod_exp) in various combinations
 *   6. Call BN_free to clean up all BN numbers
 *   7. Call BN_CTX_free to clean up context
 * Semantic differentiation from previous harnesses - this targets fundamental cryptographic primitives (Big Number arithmetic)
 * This harness specifically focuses on modular arithmetic operations which are foundational for RSA, DSA, DH, and ECC.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Need minimum input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Create BN context for modular operations
    BN_CTX* ctx = BN_CTX_new();
    if (ctx == NULL) {
        return 0;
    }

    // Create BN operands: a, b, modulus (m), and result (r)
    BIGNUM* a = BN_new();
    BIGNUM* b = BN_new();
    BIGNUM* m = BN_new();  // modulus
    BIGNUM* r = BN_new();  // result
    BIGNUM* exp = BN_new(); // exponent for modular exponentiation

    if (a == NULL || b == NULL || m == NULL || r == NULL || exp == NULL) {
        if (a) BN_free(a);
        if (b) BN_free(b);
        if (m) BN_free(m);
        if (r) BN_free(r);
        if (exp) BN_free(exp);
        BN_CTX_free(ctx);
        return 0;
    }

    // Determine operation type based on fuzzer input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume parameters for BN operations
    // Shift amount for BN_mod_lshift_quick (0-256 bits)
    int shift_amount = fdp.ConsumeIntegralInRange<int>(0, 256);
    
    // Determine bit sizes for random number generation (1-512 bits)
    int bits_a = fdp.ConsumeIntegralInRange<int>(1, 512);
    int bits_b = fdp.ConsumeIntegralInRange<int>(1, 512);
    int bits_m = fdp.ConsumeIntegralInRange<int>(1, 512);
    int bits_exp = fdp.ConsumeIntegralInRange<int>(1, 128); // Smaller exponent for performance
    
    // Determine random number generation flags
    int top_a = fdp.ConsumeBool() ? BN_RAND_TOP_ANY : BN_RAND_TOP_TWO;
    int bottom_a = fdp.ConsumeBool() ? BN_RAND_BOTTOM_ANY : BN_RAND_BOTTOM_ODD;
    
    int top_b = fdp.ConsumeBool() ? BN_RAND_TOP_ANY : BN_RAND_TOP_TWO;
    int bottom_b = fdp.ConsumeBool() ? BN_RAND_BOTTOM_ANY : BN_RAND_BOTTOM_ODD;
    
    int top_m = fdp.ConsumeBool() ? BN_RAND_TOP_ANY : BN_RAND_TOP_TWO;
    int bottom_m = fdp.ConsumeBool() ? BN_RAND_BOTTOM_ANY : BN_RAND_BOTTOM_ODD;
    
    int top_exp = fdp.ConsumeBool() ? BN_RAND_TOP_ANY : BN_RAND_TOP_TWO;
    int bottom_exp = fdp.ConsumeBool() ? BN_RAND_BOTTOM_ANY : BN_RAND_BOTTOM_ODD;

    // Generate random BN values
    int success = 1;
    success &= BN_rand(a, bits_a, top_a, bottom_a);
    success &= BN_rand(b, bits_b, top_b, bottom_b);
    success &= BN_rand(m, bits_m, top_m, bottom_m);
    success &= BN_rand(exp, bits_exp, top_exp, bottom_exp);
    
    // Ensure modulus is not zero for modular operations
    if (BN_is_zero(m)) {
        BN_set_word(m, 65537);  // Use a common prime as fallback
    }

    if (!success) {
        BN_free(a);
        BN_free(b);
        BN_free(m);
        BN_free(r);
        BN_free(exp);
        BN_CTX_free(ctx);
        return 0;
    }

    // Perform selected operation
    switch (operation) {
        case 0: {
            // Primary target: BN_mod_lshift_quick
            // Modular left shift: r = (a << n) mod m
            int result = BN_mod_lshift_quick(r, a, shift_amount, m);
            // Also test BN_mod_lshift for comparison
            if (result) {
                BN_mod_lshift(r, a, shift_amount, m, ctx);
            }
            break;
        }
        case 1: {
            // Test multiple modular arithmetic operations
            // Modular addition: r = (a + b) mod m
            BN_mod_add(r, a, b, m, ctx);
            
            // Modular subtraction: r = (a - b) mod m  
            BN_mod_sub(r, a, b, m, ctx);
            
            // Modular multiplication: r = (a * b) mod m
            BN_mod_mul(r, a, b, m, ctx);
            break;
        }
        case 2: {
            // Modular exponentiation: r = (a^exp) mod m
            // This is computationally intensive but important for RSA/DH
            BN_mod_exp(r, a, exp, m, ctx);
            
            // Also test modular squaring
            BN_mod_sqr(r, a, m, ctx);
            break;
        }
        case 3: {
            // Test quick modular operations (without context)
            BN_mod_add_quick(r, a, b, m);
            BN_mod_sub_quick(r, a, b, m);
            
            // Test modular left shift by 1 (special case)
            BN_mod_lshift1(r, a, m, ctx);
            BN_mod_lshift1_quick(r, a, m);
            
            // Test modular left shift with context
            BN_mod_lshift(r, a, shift_amount % 64, m, ctx);
            break;
        }
    }

    // Cleanup
    BN_free(a);
    BN_free(b);
    BN_free(m);
    BN_free(r);
    BN_free(exp);
    BN_CTX_free(ctx);

    return 0;
}

} // namespace harness_009
