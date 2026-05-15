#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/err.h>
#include <openssl/objects.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_005 {
/*
 * Fuzzing harness for OpenSSL EVP elliptic curve cryptography operations
 * Targets: EC key generation, signing, and verification via EVP API
 * Coverage goal: Exercise 1257+ undiscovered branches in elliptic curve module (EC_POINT_hex2point)
 * 
 * Target APIs:
 * - EVP_PKEY_keygen (Target: EC key generation)
 * - EVP_PKEY_sign (Target: EC signing)  
 * - EVP_PKEY_verify (Target: EC verification)
 * 
 * Required helper APIs:
 * - EVP_PKEY_CTX_new_id (Context initialization)
 * - EVP_PKEY_CTX_free (Cleanup)
 * - EVP_PKEY_free (Cleanup)
 * - EC_KEY_new_by_curve_name (EC curve specification)
 * - EC_KEY_free (Cleanup)
 * - EVP_PKEY_keygen_init (Key generation initialization)
 * - EVP_PKEY_sign_init (Signing initialization)
 * - EVP_PKEY_verify_init (Verification initialization)
 * 
 * Invocation sequence:
 * 1. Specify EC curve using EC_KEY_new_by_curve_name
 * 2. Create context using EVP_PKEY_CTX_new_id with EVP_PKEY_EC
 * 3. Initialize key generation using EVP_PKEY_keygen_init
 * 4. Generate EC key pair using EVP_PKEY_keygen
 * 5. Initialize signing using EVP_PKEY_sign_init
 * 6. Sign random data using EVP_PKEY_sign
 * 7. Initialize verification using EVP_PKEY_verify_init
 * 8. Verify signature using EVP_PKEY_verify
 * 9. Cleanup all objects
 * 
 * Differentiation from existing harnesses:
 * - harness_000.cpp: AES cryptographic operations
 * - harness_001.cpp: RSA cryptographic operations  
 * - harness_002.cpp: SSL/TLS protocol stack
 * - harness_003.cpp: CMP/CRMF certificate management
 * - harness_004.cpp: X.509 certificate parsing and CMS operations
 * - harness_005.cpp: EVP elliptic curve cryptography operations (NEW)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for EC operations: enough for curve selection, key gen, and some data
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize OpenSSL libraries
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    
    // 1. Select EC curve from fuzzed input
    // Common EC curve NIDs for testing
    static const int ec_curves[] = {
        NID_X9_62_prime256v1,      // P-256
        NID_secp384r1,             // P-384  
        NID_secp521r1,             // P-521
        NID_X9_62_prime192v1,      // P-192
        NID_secp224r1,             // P-224
        NID_brainpoolP256r1,       // Brainpool P-256
        NID_brainpoolP384r1,       // Brainpool P-384
        NID_brainpoolP512r1,       // Brainpool P-512
    };
    
    int curve_choice = fdp.ConsumeIntegralInRange<int>(0, sizeof(ec_curves)/sizeof(ec_curves[0]) - 1);
    int curve_nid = ec_curves[curve_choice];
    
    // Create EC_KEY for curve specification (legacy API)
    EC_KEY* ec_key = EC_KEY_new_by_curve_name(curve_nid);
    if (ec_key == NULL) {
        return 0;
    }
    
    // 2. Create EVP_PKEY context for EC operations
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
    if (ctx == NULL) {
        EC_KEY_free(ec_key);
        return 0;
    }
    
    // 3. Initialize key generation
    if (EVP_PKEY_keygen_init(ctx) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EC_KEY_free(ec_key);
        return 0;
    }
    
    // Set EC curve parameter
    if (EVP_PKEY_CTX_set_ec_paramgen_curve_nid(ctx, curve_nid) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EC_KEY_free(ec_key);
        return 0;
    }
    
    // 4. Generate EC key pair
    EVP_PKEY* pkey = NULL;
    if (EVP_PKEY_keygen(ctx, &pkey) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EC_KEY_free(ec_key);
        return 0;
    }
    
    // Clean up key generation context
    EVP_PKEY_CTX_free(ctx);
    
    // 5. Prepare data for signing
    size_t data_to_sign_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::vector<uint8_t> data_to_sign = fdp.ConsumeBytes<uint8_t>(data_to_sign_size);
    
    if (data_to_sign.empty()) {
        EVP_PKEY_free(pkey);
        EC_KEY_free(ec_key);
        return 0;
    }
    
    // 6. Initialize signing context
    ctx = EVP_PKEY_CTX_new(pkey, NULL);
    if (ctx == NULL) {
        EVP_PKEY_free(pkey);
        EC_KEY_free(ec_key);
        return 0;
    }
    
    if (EVP_PKEY_sign_init(ctx) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pkey);
        EC_KEY_free(ec_key);
        return 0;
    }
    
    // Set digest type (SHA-256 is commonly used with EC)
    if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256()) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pkey);
        EC_KEY_free(ec_key);
        return 0;
    }
    
    // Determine signature buffer size
    size_t siglen = 0;
    if (EVP_PKEY_sign(ctx, NULL, &siglen, data_to_sign.data(), data_to_sign.size()) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pkey);
        EC_KEY_free(ec_key);
        return 0;
    }
    
    // Allocate signature buffer
    std::vector<uint8_t> signature(siglen);
    
    // Perform signing
    if (EVP_PKEY_sign(ctx, signature.data(), &siglen, data_to_sign.data(), data_to_sign.size()) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pkey);
        EC_KEY_free(ec_key);
        return 0;
    }
    
    signature.resize(siglen);  // Adjust to actual signature size
    
    // 7. Initialize verification context
    // Reuse the same context for verification
    if (EVP_PKEY_verify_init(ctx) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pkey);
        EC_KEY_free(ec_key);
        return 0;
    }
    
    // Set digest type for verification (must match signing)
    if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256()) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pkey);
        EC_KEY_free(ec_key);
        return 0;
    }
    
    // 8. Verify the signature
    // Note: Verification may fail if signature was corrupted or incompatible
    // We call it but don't check result as fuzzer input may produce invalid signatures
    EVP_PKEY_verify(ctx, signature.data(), signature.size(), 
                    data_to_sign.data(), data_to_sign.size());
    
    // 9. Cleanup
    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(pkey);
    EC_KEY_free(ec_key);
    
    return 0;
}

} // namespace harness_005
