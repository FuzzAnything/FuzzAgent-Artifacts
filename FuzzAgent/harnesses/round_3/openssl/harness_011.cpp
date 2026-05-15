#include <stddef.h>
#include <stdint.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/asn1.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_011 {
/*
 * Fuzzing harness for OpenSSL X509 certificate name hashing operations
 * Primary target: X509_NAME_hash_ex with 708 undiscovered branches
 * Targets: X509_NAME_hash_ex, X509_issuer_name_hash, X509_subject_name_hash,
 *          X509_issuer_name_hash_old, X509_subject_name_hash_old
 * Helper APIs: X509_new, X509_free, X509_NAME_new, X509_NAME_add_entry_by_txt,
 *              X509_set_subject_name, X509_set_issuer_name, X509_get_subject_name,
 *              X509_get_issuer_name, X509_NAME_free
 * Cleanup: X509_free, X509_NAME_free
 * Implements X509 name hashing lifecycle: Create X509 certificates → 
 * Populate subject/issuer names → Compute hashes → Test edge cases → Cleanup
 * Differentiated from harness_001.cpp (X509 certificate verification) and 
 * harness_004.cpp (PKCS7/CMS) - this harness specifically targets 
 * X509 name hashing operations which are critical for certificate lookup
 * and comparison in certificate stores and revocation lists.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Need minimum input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_clear_error();

    // Determine test scenario based on fuzzer input
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 6;

    // Create X509 certificate for testing
    X509* cert = X509_new();
    if (cert == NULL) {
        return 0;
    }

    // Create X509_NAME structures for subject and issuer
    X509_NAME* subject_name = X509_NAME_new();
    X509_NAME* issuer_name = X509_NAME_new();
    
    if (subject_name == NULL || issuer_name == NULL) {
        if (subject_name) X509_NAME_free(subject_name);
        if (issuer_name) X509_NAME_free(issuer_name);
        X509_free(cert);
        return 0;
    }

    // Populate X509_NAME structures with fuzzer data
    // Determine number of name entries to add
    size_t num_subject_entries = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    size_t num_issuer_entries = fdp.ConsumeIntegralInRange<size_t>(0, 10);

    for (size_t i = 0; i < num_subject_entries && fdp.remaining_bytes() > 4; i++) {
        // Consume field type
        uint8_t field_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        const char* field_name = NULL;
        
        switch (field_type) {
            case 0: field_name = "CN"; break;  // Common Name
            case 1: field_name = "OU"; break;  // Organizational Unit
            case 2: field_name = "O"; break;   // Organization
            case 3: field_name = "L"; break;   // Locality
            case 4: field_name = "C"; break;   // Country
        }
        
        // Consume field value
        size_t value_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
        if (value_len > fdp.remaining_bytes()) {
            value_len = fdp.remaining_bytes();
        }
        std::string field_value = fdp.ConsumeBytesAsString(value_len);
        
        // Add entry to subject name
        X509_NAME_add_entry_by_txt(subject_name, field_name, MBSTRING_ASC,
                                   (const unsigned char*)field_value.c_str(), -1, -1, 0);
    }

    for (size_t i = 0; i < num_issuer_entries && fdp.remaining_bytes() > 4; i++) {
        // Consume field type
        uint8_t field_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        const char* field_name = NULL;
        
        switch (field_type) {
            case 0: field_name = "CN"; break;
            case 1: field_name = "OU"; break;
            case 2: field_name = "O"; break;
            case 3: field_name = "L"; break;
            case 4: field_name = "C"; break;
        }
        
        // Consume field value
        size_t value_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
        if (value_len > fdp.remaining_bytes()) {
            value_len = fdp.remaining_bytes();
        }
        std::string field_value = fdp.ConsumeBytesAsString(value_len);
        
        // Add entry to issuer name
        X509_NAME_add_entry_by_txt(issuer_name, field_name, MBSTRING_ASC,
                                   (const unsigned char*)field_value.c_str(), -1, -1, 0);
    }

    // Set names on certificate
    X509_set_subject_name(cert, subject_name);
    X509_set_issuer_name(cert, issuer_name);

    // Test different scenarios
    switch (scenario) {
        case 0: {
            // Test X509_NAME_hash_ex with subject name
            int ok = 0;
            unsigned long hash = X509_NAME_hash_ex(subject_name, NULL, NULL, &ok);
            // Hash may be 0 if ok is 0, which is valid for error cases
            break;
        }
        
        case 1: {
            // Test X509_NAME_hash_ex with issuer name
            int ok = 0;
            unsigned long hash = X509_NAME_hash_ex(issuer_name, NULL, NULL, &ok);
            break;
        }
        
        case 2: {
            // Test X509_issuer_name_hash
            unsigned long issuer_hash = X509_issuer_name_hash(cert);
            
            // Also test X509_subject_name_hash for comparison
            unsigned long subject_hash = X509_subject_name_hash(cert);
            break;
        }
        
        case 3: {
            // Test X509_NAME_hash_ex with NULL ok parameter
            unsigned long subject_hash = X509_NAME_hash_ex(subject_name, NULL, NULL, NULL);
            unsigned long issuer_hash = X509_NAME_hash_ex(issuer_name, NULL, NULL, NULL);
            break;
        }
        
        case 4: {
            // Test with empty X509_NAME (no entries)
            X509_NAME* empty_name = X509_NAME_new();
            if (empty_name != NULL) {
                int ok = 0;
                unsigned long empty_hash = X509_NAME_hash_ex(empty_name, NULL, NULL, &ok);
                X509_NAME_free(empty_name);
            }
            
            // Test X509_issuer_name_hash_old and X509_subject_name_hash_old if available
            #ifndef OPENSSL_NO_MD5
            unsigned long old_issuer_hash = X509_issuer_name_hash_old(cert);
            unsigned long old_subject_hash = X509_subject_name_hash_old(cert);
            #endif
            break;
        }
        
        case 5: {
            // Test with certificates that have duplicate name entries
            X509* cert_with_duplicates = X509_new();
            if (cert_with_duplicates != NULL) {
                X509_NAME* dup_name = X509_NAME_new();
                if (dup_name != NULL) {
                    // Add multiple entries with the same field name
                    for (int i = 0; i < 3 && fdp.remaining_bytes() > 4; i++) {
                        size_t value_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                        if (value_len > fdp.remaining_bytes()) {
                            value_len = fdp.remaining_bytes();
                        }
                        std::string value = fdp.ConsumeBytesAsString(value_len);
                        X509_NAME_add_entry_by_txt(dup_name, "CN", MBSTRING_ASC,
                                                  (const unsigned char*)value.c_str(), -1, -1, 0);
                    }
                    
                    X509_set_subject_name(cert_with_duplicates, dup_name);
                    X509_set_issuer_name(cert_with_duplicates, dup_name);
                    
                    // Test hash functions with certificate containing duplicate entries
                    unsigned long issuer_hash = X509_issuer_name_hash(cert_with_duplicates);
                    unsigned long subject_hash = X509_subject_name_hash(cert_with_duplicates);
                    
                    X509_NAME_free(dup_name);
                }
                X509_free(cert_with_duplicates);
            }
            break;
        }
    }

    // Additional tests with modified certificates
    if (fdp.remaining_bytes() > 16) {
        // Create another certificate with different names
        X509* cert2 = X509_new();
        if (cert2 != NULL) {
            X509_NAME* name2 = X509_NAME_new();
            if (name2 != NULL) {
                // Add a simple entry
                size_t cn_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                std::string cn_value = fdp.ConsumeBytesAsString(cn_len);
                X509_NAME_add_entry_by_txt(name2, "CN", MBSTRING_ASC,
                                          (const unsigned char*)cn_value.c_str(), -1, -1, 0);
                
                X509_set_subject_name(cert2, name2);
                X509_set_issuer_name(cert2, name2);
                
                // Compare hashes between two certificates
                unsigned long hash1 = X509_issuer_name_hash(cert);
                unsigned long hash2 = X509_issuer_name_hash(cert2);
                unsigned long hash3 = X509_subject_name_hash(cert);
                unsigned long hash4 = X509_subject_name_hash(cert2);
                
                X509_NAME_free(name2);
            }
            X509_free(cert2);
        }
    }

    // Clean up
    X509_NAME_free(subject_name);
    X509_NAME_free(issuer_name);
    X509_free(cert);

    return 0;
}

} // namespace harness_011
