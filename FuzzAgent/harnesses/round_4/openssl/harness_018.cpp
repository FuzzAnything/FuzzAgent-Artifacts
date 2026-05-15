#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/dsa.h>
#include <openssl/err.h>
#include <openssl/bn.h>
#include <openssl/objects.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_018 {
/*
 * Fuzzing harness for OpenSSL DSA cryptographic operations focusing on parameter duplication
 * Targets: DSA parameter duplication (DSAparams_dup) with 261 undiscovered branches and complete DSA lifecycle
 * Coverage goal: Improve DSA module coverage (currently 7.99%) by targeting DSAparams_dup and related operations
 * 
 * Target APIs:
 * - DSAparams_dup (Primary target: 261 undiscovered branches)
 * - DSA_new (Required Helper: Init)
 * - DSA_generate_parameters (Required Helper: Setup)
 * - DSA_generate_key (Target)
 * - DSA_sign (Target)
 * - DSA_verify (Target)
 * - DSA_free (Required Helper: Cleanup)
 * 
 * Invocation sequence:
 * 1. Call DSA_new to create a DSA structure
 * 2. Call DSA_generate_parameters_ex to generate DSA parameters
 * 3. Call DSA_generate_key to create public/private key pair
 * 4. Call DSAparams_dup to duplicate parameters (testing duplication logic)
 * 5. Call DSA_sign to sign test data
 * 6. Call DSA_verify to verify the signature
 * 7. Call DSA_free to clean up all DSA structures
 * 
 * Differentiation from existing harnesses:
 * - harness_011.cpp: Covers DSA operations but does NOT include DSAparams_dup (261 undiscovered branches)
 * - harness_018.cpp: Specifically targets DSA parameter duplication path including DSAparams_dup (NEW)
 * 
 * Coverage impact: DSA module currently has only 7.99% coverage. DSAparams_dup alone has 261 undiscovered branches,
 * indicating high complexity in parameter duplication logic that is currently untested.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for DSA parameters, seed, and signing data
    // DSA parameter generation requires bits, seed, and various control values
    if (size < 128) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize OpenSSL error strings
    ERR_load_crypto_strings();
    
    // 1. Create DSA structure
    DSA* dsa = DSA_new();
    if (dsa == NULL) {
        return 0;
    }
    
    // 2. Generate DSA parameters
    // Consume parameter bits from valid DSA range (512-3072 bits, multiples of 64)
    int bits = 512 + (fdp.ConsumeIntegralInRange<int>(0, 40) * 64); // 512-3072 in 64-bit steps
    if (bits > 3072) bits = 3072;
    
    // Consume seed for parameter generation (DSA requires 20-32 byte seed)
    size_t seed_len = fdp.ConsumeIntegralInRange<size_t>(20, 32);
    if (fdp.remaining_bytes() < seed_len + sizeof(int) + sizeof(unsigned long)) {
        DSA_free(dsa);
        return 0;
    }
    
    std::vector<unsigned char> seed = fdp.ConsumeBytes<unsigned char>(seed_len);
    int counter_ret = 0;
    unsigned long h_ret = 0;
    
    // Use DSA_generate_parameters_ex (newer API) instead of deprecated DSA_generate_parameters
    int param_gen_result = DSA_generate_parameters_ex(dsa, bits, 
                                                      seed.data(), seed_len,
                                                      &counter_ret, &h_ret, NULL);
    
    if (param_gen_result != 1) {
        // Parameter generation failed, cleanup and exit
        DSA_free(dsa);
        return 0;
    }
    
    // 3. Generate DSA key pair
    int key_gen_result = DSA_generate_key(dsa);
    if (key_gen_result != 1) {
        DSA_free(dsa);
        return 0;
    }
    
    // 4. Duplicate DSA parameters using DSAparams_dup (primary target)
    DSA* dsa_dup = DSAparams_dup(dsa);
    if (dsa_dup == NULL) {
        // Even if duplication fails, continue with original DSA structure
        // This tests error paths in DSAparams_dup
    } else {
        // If duplication succeeded, we have a second DSA structure with same parameters
        // Could generate key for duplicated structure too
        DSA_generate_key(dsa_dup);
    }
    
    // 5. Sign test data using DSA
    // Consume data to sign (remaining bytes after parameter setup)
    size_t data_to_sign_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    if (fdp.remaining_bytes() < data_to_sign_len) {
        if (dsa_dup != NULL) DSA_free(dsa_dup);
        DSA_free(dsa);
        return 0;
    }
    
    std::vector<unsigned char> data_to_sign = fdp.ConsumeBytes<unsigned char>(data_to_sign_len);
    
    // Allocate buffer for signature (DSA signature size is 2 * key size in bytes)
    unsigned int sig_len = DSA_size(dsa);
    std::vector<unsigned char> signature(sig_len);
    
    // Call DSA_sign with type = NID_sha1 (common for DSA)
    int sign_result = DSA_sign(0, data_to_sign.data(), data_to_sign.size(),
                               signature.data(), &sig_len, dsa);
    
    if (sign_result == 1 && sig_len <= signature.size()) {
        // 6. Verify the signature
        int verify_result = DSA_verify(0, data_to_sign.data(), data_to_sign.size(),
                                       signature.data(), sig_len, dsa);
        
        // Verification result can be 1 (valid), 0 (invalid), or -1 (error)
        // All are valid outcomes for fuzzing
    }
    
    // 7. Cleanup
    if (dsa_dup != NULL) {
        DSA_free(dsa_dup);
    }
    DSA_free(dsa);
    
    return 0;
}

} // namespace harness_018
