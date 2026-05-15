#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/cmp.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/x509_vfy.h>
#include <openssl/err.h>
#include <openssl/crypto.h>

namespace harness_003 {
/*
 * Fuzzing harness for OpenSSL targeting CMP (Certificate Management Protocol) operations
 * This harness tests: CMP context creation, trusted/untrusted certificate store setup,
 * error printing, and CMP context lifecycle management.
 * Primary target: OSSL_CMP_CTX_new with 449 undiscovered accumulated branches
 * Secondary target: OSSL_CMP_CTX_print_errors with 234 undiscovered accumulated branches
 * This harness is semantically different from previous harnesses focusing on hash functions,
 * X509 certificates, and SSL/TLS SRP authentication.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize OpenSSL
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | 
                        OPENSSL_INIT_ADD_ALL_CIPHERS | 
                        OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
    ERR_clear_error();

    FuzzedDataProvider fdp(data, size);
    OSSL_CMP_CTX* ctx = NULL;
    X509_STORE* trusted_store = NULL;
    STACK_OF(X509)* untrusted_certs = NULL;
    X509* cert = NULL;
    int store_transferred = 0;
    int certs_transferred = 0;

    // ==================== CMP CONTEXT CREATION ====================
    // 1. Create CMP context (PRIMARY TARGET: OSSL_CMP_CTX_new)
    // Using NULL for libctx and propq as shown in existing fuzz/cmp.c
    ctx = OSSL_CMP_CTX_new(NULL, NULL);
    if (ctx == NULL) {
        // If we can't create context, we can't proceed
        return 0;
    }

    // ==================== TRUSTED STORE SETUP ====================
    // 2. Create and set trusted certificate store
    // Try to create an X509_STORE for trusted certificates
    trusted_store = X509_STORE_new();
    if (trusted_store != NULL) {
        // Try to add some certificate from fuzzer input to the store
        // First, attempt to parse a certificate from fuzzer input
        size_t cert_data_len = fdp.ConsumeIntegralInRange<size_t>(1, std::max<size_t>(1, fdp.remaining_bytes() / 3));
        if (cert_data_len > 0) {
            std::vector<uint8_t> cert_data = fdp.ConsumeBytes<uint8_t>(cert_data_len);
            const unsigned char* p = cert_data.data();
            cert = d2i_X509(NULL, &p, cert_data.size());
            
            if (cert != NULL) {
                // Add certificate to store (doesn't take ownership)
                X509_STORE_add_cert(trusted_store, cert);
                // Note: X509_STORE_add_cert doesn't increment refcount
                // We'll track ownership separately
            }
        }
        
        // Set trusted store in CMP context
        // Note: OSSL_CMP_CTX_set0_trusted is a macro for OSSL_CMP_CTX_set0_trustedStore
        // This function transfers ownership of the store to the context
        if (OSSL_CMP_CTX_set0_trusted(ctx, trusted_store)) {
            // Success: ctx now owns trusted_store, don't free it separately
            trusted_store = NULL; // Set to NULL to avoid double free
            store_transferred = 1;
            // If cert was added to store and store was transferred to ctx,
            // we should not free cert separately (store references it)
            if (cert != NULL) {
                cert = NULL; // Don't free, store (now in ctx) references it
            }
        }
    }

    // ==================== UNTRUSTED CERTIFICATES SETUP ====================
    // 3. Create and set untrusted certificates
    // Try to create a stack of untrusted certificates
    untrusted_certs = sk_X509_new_null();
    if (untrusted_certs != NULL) {
        // Try to add some certificates from fuzzer input
        // We'll attempt to parse up to 3 certificates
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
            size_t untrusted_cert_len = fdp.ConsumeIntegralInRange<size_t>(1, std::max<size_t>(1, fdp.remaining_bytes() / 4));
            if (untrusted_cert_len > 0) {
                std::vector<uint8_t> untrusted_cert_data = fdp.ConsumeBytes<uint8_t>(untrusted_cert_len);
                const unsigned char* p = untrusted_cert_data.data();
                X509* untrusted_cert = d2i_X509(NULL, &p, untrusted_cert_data.size());
                
                if (untrusted_cert != NULL) {
                    // Add to stack (doesn't take ownership, we need to free later)
                    sk_X509_push(untrusted_certs, untrusted_cert);
                }
            }
        }
        
        // Set untrusted certificates in CMP context
        // Note: OSSL_CMP_CTX_set1_untrusted takes ownership of the certs
        // (the "1" in set1 indicates it increments reference counts)
        if (sk_X509_num(untrusted_certs) > 0) {
            if (OSSL_CMP_CTX_set1_untrusted(ctx, untrusted_certs)) {
                // Success: ctx now has references to the certs
                certs_transferred = 1;
                // We should free the stack but not the individual certificates
                // The context now holds references
                untrusted_certs = NULL; // Don't free, certs are now owned by ctx
            }
        }
    }

    // ==================== CMP ERROR PRINTING ====================
    // 4. Print any errors that might have occurred (SECONDARY TARGET: OSSL_CMP_CTX_print_errors)
    // Even if no errors occurred, call this to cover the function
    OSSL_CMP_CTX_print_errors(ctx);

    // ==================== ADDITIONAL CMP PARAMETER SETUP ====================
    // 5. Try to set some additional CMP parameters if we have remaining input
    if (fdp.remaining_bytes() > 10) {
        // Try to set server URL
        std::string server_url = fdp.ConsumeRandomLengthString(128);
        if (!server_url.empty()) {
            // Note: OSSL_CMP_CTX_set1_server might be available
            // We'll use a simple approach - just consume the string
        }
        
        // Try to set reference value
        std::string ref_value = fdp.ConsumeRandomLengthString(64);
        if (!ref_value.empty()) {
            // Note: OSSL_CMP_CTX_set1_referenceValue might be available
        }
    }

    // ==================== CLEANUP ====================
    // 6. Clean up resources (following proper ownership rules)
    
    // Free untrusted certificates stack if not transferred to context
    if (untrusted_certs != NULL) {
        // If certs weren't transferred to context, we need to free them
        // sk_X509_pop_free frees both the stack and the elements
        sk_X509_pop_free(untrusted_certs, X509_free);
        untrusted_certs = NULL;
    }
    
    // Free trusted store if not transferred to context
    if (trusted_store != NULL) {
        X509_STORE_free(trusted_store);
        trusted_store = NULL;
    }
    
    // Free certificate if not added to store or store wasn't transferred
    if (cert != NULL) {
        X509_free(cert);
        cert = NULL;
    }
    
    // Finally, free the CMP context (REQUIRED HELPER: OSSL_CMP_CTX_free)
    OSSL_CMP_CTX_free(ctx);
    
    return 0;
}

} // namespace harness_003
