#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/pkcs12.h>
#include <openssl/x509.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/objects.h>
#include <openssl/rand.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

namespace harness_006 {
/*
 * Fuzzing harness for OpenSSL PKCS12 certificate and private key container operations
 * Targets: PKCS12 container creation and manipulation with highest uncovered complexity
 * Coverage goal: Exercise 1089+ undiscovered branches in PKCS12_add_safe_ex
 * 
 * Target APIs:
 * - PKCS12_add_safe_ex (Target: 1089 undiscovered branches)
 * - PKCS12_add_safe (Target: Safe bag addition)
 * - PKCS12_create_ex2 (Target: Extended container creation)
 * - PKCS12_create_ex (Target: Extended container creation)
 * - PKCS12_create (Target: Main container creation)
 * 
 * Required helper APIs:
 * - PKCS12_free (Cleanup)
 * - PKCS12_parse (Parse/Validate)
 * - X509_new (Certificate creation)
 * - EVP_PKEY_new (Key creation)
 * - X509_free (Cleanup)
 * - EVP_PKEY_free (Cleanup)
 * 
 * Invocation sequence:
 * 1. Create X509 certificate using X509_new and populate with random data
 * 2. Create EVP_PKEY key using EVP_PKEY_new with random key material
 * 3. Call PKCS12_create to create a new PKCS12 container with certificate and private key
 * 4. Call PKCS12_add_safe to add additional safe bags to the container
 * 5. Call PKCS12_parse to validate the created container can be parsed
 * 6. Call PKCS12_free to clean up the container
 * 7. Call X509_free and EVP_PKEY_free to clean up certificates and keys
 * 
 * Differentiation from existing harnesses:
 * - harness_000.cpp: AES cryptographic operations
 * - harness_001.cpp: RSA cryptographic operations  
 * - harness_002.cpp: SSL/TLS protocol stack
 * - harness_003.cpp: CMP/CRMF certificate management
 * - harness_004.cpp: X.509 certificate parsing and CMS operations
 * - harness_005.cpp: EVP elliptic curve cryptography operations
 * - harness_006.cpp: PKCS12 certificate and private key container operations (NEW)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: enough for basic parameters and some data
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize OpenSSL libraries
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    
    // 1. Create X509 certificate object
    X509* cert = X509_new();
    if (cert == NULL) {
        return 0;
    }
    
    // Populate certificate with minimal data
    // Note: This creates an invalid certificate but allows testing PKCS12 APIs
    ASN1_INTEGER* serial = ASN1_INTEGER_new();
    if (serial != NULL) {
        // Set a random serial number from fuzzed input
        if (fdp.remaining_bytes() >= 8) {
            std::vector<uint8_t> serial_data = fdp.ConsumeBytes<uint8_t>(8);
            // Convert bytes to integer for serial
            int64_t serial_value = 0;
            for (int i = 0; i < 8 && i < serial_data.size(); i++) {
                serial_value = (serial_value << 8) | serial_data[i];
            }
            ASN1_INTEGER_set(serial, serial_value);
        } else {
            // Not enough data, use a small value
            ASN1_INTEGER_set(serial, 1);
        }
        X509_set_serialNumber(cert, serial);
        ASN1_INTEGER_free(serial);
    }
    
    // 2. Create EVP_PKEY key object
    EVP_PKEY* pkey = EVP_PKEY_new();
    if (pkey == NULL) {
        X509_free(cert);
        return 0;
    }
    
    // Create a simple RSA key with minimal components for testing
    // This creates an invalid key but allows PKCS12 APIs to be tested
    RSA* rsa = RSA_new();
    if (rsa != NULL) {
        BIGNUM* e = BN_new();
        if (e != NULL) {
            BN_set_word(e, 65537);  // Common RSA exponent
            
            // Create minimal n and d values (not valid RSA, but enough for API testing)
            BIGNUM* n = BN_new();
            BIGNUM* d = BN_new();
            
            if (n != NULL && d != NULL) {
                // Use fuzzed input for n value if available
                if (fdp.remaining_bytes() >= 4) {
                    uint32_t n_val = fdp.ConsumeIntegral<uint32_t>();
                    BN_set_word(n, n_val);
                } else {
                    BN_set_word(n, 3);  // Small prime
                }
                
                // Set d to same as n (invalid but allows API testing)
                BN_copy(d, n);
                
                RSA_set0_key(rsa, n, e, d);
                EVP_PKEY_assign_RSA(pkey, rsa);
            } else {
                if (n != NULL) BN_free(n);
                if (d != NULL) BN_free(d);
                BN_free(e);
                RSA_free(rsa);
                EVP_PKEY_free(pkey);
                X509_free(cert);
                return 0;
            }
        } else {
            RSA_free(rsa);
            EVP_PKEY_free(pkey);
            X509_free(cert);
            return 0;
        }
    }
    // 3. Create PKCS12 container
    // Consume parameters from fuzzed input
    std::string pass = fdp.ConsumeRandomLengthString(64);
    std::string name = fdp.ConsumeRandomLengthString(32);
    
    // Select encryption algorithms from common NIDs
    static const int key_nids[] = {
        NID_aes_256_cbc,
        NID_aes_192_cbc,
        NID_aes_128_cbc,
        NID_des_ede3_cbc,
        NID_pbe_WithSHA1And3_Key_TripleDES_CBC,
        NID_pbe_WithSHA1And40BitRC2_CBC,
    };
    
    static const int cert_nids[] = {
        NID_aes_256_cbc,
        NID_aes_192_cbc,
        NID_aes_128_cbc,
        NID_des_ede3_cbc,
    };
    
    int key_nid = key_nids[fdp.ConsumeIntegralInRange<size_t>(0, sizeof(key_nids)/sizeof(key_nids[0]) - 1)];
    int cert_nid = cert_nids[fdp.ConsumeIntegralInRange<size_t>(0, sizeof(cert_nids)/sizeof(cert_nids[0]) - 1)];
    
    int iter = fdp.ConsumeIntegralInRange<int>(1, 10000);
    int mac_iter = fdp.ConsumeIntegralInRange<int>(1, 10000);
    int keytype = fdp.ConsumeIntegralInRange<int>(0, 2);  // 0=KEY_EX, 1=KEY_SIG, 2=both
    
    // Create empty CA stack
    STACK_OF(X509)* ca = sk_X509_new_null();
    
    // Try different PKCS12 creation functions based on input
    uint8_t create_func = fdp.ConsumeIntegral<uint8_t>() % 3;
    PKCS12* p12 = NULL;
    
    switch (create_func) {
        case 0:
            // Basic PKCS12_create
            p12 = PKCS12_create(pass.c_str(), name.c_str(), pkey, cert, ca,
                               key_nid, cert_nid, iter, mac_iter, keytype);
            break;
            
        case 1:
            // PKCS12_create_ex with NULL context/propq
            p12 = PKCS12_create_ex(pass.c_str(), name.c_str(), pkey, cert, ca,
                                  key_nid, cert_nid, iter, mac_iter, keytype,
                                  NULL, NULL);
            break;
            
        case 2:
            // PKCS12_create_ex2 with NULL callback
            p12 = PKCS12_create_ex2(pass.c_str(), name.c_str(), pkey, cert, ca,
                                   key_nid, cert_nid, iter, mac_iter, keytype,
                                   NULL, NULL, NULL, NULL);
            break;
    }
    
    if (p12 == NULL) {
        sk_X509_free(ca);
        EVP_PKEY_free(pkey);
        X509_free(cert);
        return 0;
    }
    
    // 4. Add additional safe bags using PKCS12_add_safe or PKCS12_add_safe_ex
    if (fdp.remaining_bytes() > 16) {
        // Create a bag stack
        STACK_OF(PKCS12_SAFEBAG)* bags = sk_PKCS12_SAFEBAG_new_null();
        if (bags != NULL) {
            // Add a certificate bag
            PKCS12_SAFEBAG* cert_bag = PKCS12_add_cert(&bags, cert);
            if (cert_bag != NULL) {
                // Safes stack for PKCS12_add_safe functions
                STACK_OF(PKCS7)* safes = NULL;
                
                // Try to add safe
                int safe_nid = cert_nids[fdp.ConsumeIntegralInRange<size_t>(0, sizeof(cert_nids)/sizeof(cert_nids[0]) - 1)];
                int safe_iter = fdp.ConsumeIntegralInRange<int>(1, 1000);
                std::string safe_pass = fdp.ConsumeRandomLengthString(32);
                
                uint8_t add_safe_func = fdp.ConsumeIntegral<uint8_t>() % 2;
                int add_result = 0;
                if (add_safe_func == 0) {
                    // PKCS12_add_safe
                    add_result = PKCS12_add_safe(&safes, bags, safe_nid, safe_iter, safe_pass.c_str());
                } else {
                    // PKCS12_add_safe_ex with NULL context/propq
                    add_result = PKCS12_add_safe_ex(&safes, bags, safe_nid, safe_iter, safe_pass.c_str(),
                                      NULL, NULL);
                }
                
                // Clean up safes stack if it was allocated
                if (safes != NULL) {
                    sk_PKCS7_pop_free(safes, PKCS7_free);
                }
                
                // Clean up bag stack
                sk_PKCS12_SAFEBAG_pop_free(bags, PKCS12_SAFEBAG_free);
            } else {
                sk_PKCS12_SAFEBAG_free(bags);
            }
        }
    }
    
    // 5. Parse the created PKCS12 container to validate it
    EVP_PKEY* parsed_pkey = NULL;
    X509* parsed_cert = NULL;
    STACK_OF(X509)* parsed_ca = NULL;
    
    int parse_result = PKCS12_parse(p12, pass.c_str(), &parsed_pkey, &parsed_cert, &parsed_ca);
    // Note: parse_result may fail due to invalid certificate/key, but that's OK for fuzzing
    
    // Clean up parsed objects if parsing succeeded
    if (parse_result == 1) {
        if (parsed_pkey != NULL) EVP_PKEY_free(parsed_pkey);
        if (parsed_cert != NULL) X509_free(parsed_cert);
        if (parsed_ca != NULL) sk_X509_pop_free(parsed_ca, X509_free);
    }
    
    // 6. Cleanup
    PKCS12_free(p12);
    sk_X509_free(ca);
    EVP_PKEY_free(pkey);
    X509_free(cert);
    
    return 0;
}

} // namespace harness_006
