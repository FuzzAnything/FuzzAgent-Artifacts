#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/pkcs12.h>
#include <openssl/x509.h>
#include <openssl/obj_mac.h>
#include <openssl/crypto.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_015 {
/*
 * Harness for testing PKCS12 attribute management operations
 * Target: PKCS12_add_friendlyname_asc (804 accumulated branches, 377 undiscovered branches)
 * Additional APIs: PKCS12_add_friendlyname_utf8, PKCS12_add_friendlyname_uni,
 *                  PKCS12_add_CSPName_asc, PKCS12_get_friendlyname,
 *                  PKCS12_add1_attr_by_NID, PKCS12_get_attr_gen
 * 
 * Setup APIs: PKCS12_init, PKCS12_SAFEBAG_create_cert, X509_new
 * Cleanup APIs: PKCS12_free, X509_free, PKCS12_SAFEBAG_free
 */

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed: enough for various string parameters and integer values
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Create a dummy certificate for testing
    X509* cert = X509_new();
    if (cert == nullptr) {
        return 0;
    }

    // Initialize PKCS12 structure
    PKCS12* p12 = PKCS12_init(NID_pkcs7_data);
    if (p12 == nullptr) {
        X509_free(cert);
        return 0;
    }

    // Create certificate bag
    PKCS12_SAFEBAG* bag = PKCS12_SAFEBAG_create_cert(cert);
    if (bag == nullptr) {
        PKCS12_free(p12);
        X509_free(cert);
        return 0;
    }

    // Consume various string parameters from fuzzer input
    std::string ascii_name = fdp.ConsumeRandomLengthString(256);
    std::string utf8_name = fdp.ConsumeRandomLengthString(256);
    std::string csp_name = fdp.ConsumeRandomLengthString(256);
    
    // For Unicode name, we need to consume bytes and treat as unsigned char*
    size_t uni_name_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::vector<unsigned char> uni_name_bytes = fdp.ConsumeBytes<unsigned char>(uni_name_len);
    
    // For custom attribute data
    size_t attr_data_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::vector<unsigned char> attr_data_bytes = fdp.ConsumeBytes<unsigned char>(attr_data_len);
    
    // Consume NID (Object Identifier) for custom attribute
    int attr_nid = fdp.ConsumeIntegral<int>();
    
    // Consume attribute type
    int attr_type = fdp.ConsumeIntegral<int>() % 5; // Limit to valid types
    
    // 1. Add ASCII friendly name
    PKCS12_add_friendlyname_asc(bag, ascii_name.c_str(), ascii_name.length());
    
    // 2. Add UTF8 friendly name
    PKCS12_add_friendlyname_utf8(bag, utf8_name.c_str(), utf8_name.length());
    
    // 3. Add Unicode friendly name (if we have bytes)
    if (!uni_name_bytes.empty()) {
        PKCS12_add_friendlyname_uni(bag, uni_name_bytes.data(), uni_name_bytes.size());
    }
    
    // 4. Add CSP name attribute
    PKCS12_add_CSPName_asc(bag, csp_name.c_str(), csp_name.length());
    
    // 5. Add custom attribute by NID (if we have data)
    if (!attr_data_bytes.empty()) {
        PKCS12_add1_attr_by_NID(bag, attr_nid, attr_type, 
                               attr_data_bytes.data(), attr_data_bytes.size());
    }
    
    // 6. Retrieve and verify added name
    char* retrieved_name = PKCS12_get_friendlyname(bag);
    if (retrieved_name != nullptr) {
        // Verify the retrieved name is valid
        size_t name_len = strlen(retrieved_name);
        // Free the retrieved name (allocated by OPENSSL_uni2utf8)
        OPENSSL_free(retrieved_name);
    }
    
    // 7. Retrieve custom attribute (if we added one)
    if (!attr_data_bytes.empty()) {
        // Get attributes from bag
        const STACK_OF(X509_ATTRIBUTE)* attrs = PKCS12_SAFEBAG_get0_attrs(bag);
        if (attrs != nullptr) {
            // Try to retrieve the attribute we just added
            const ASN1_TYPE* attr = PKCS12_get_attr_gen(attrs, attr_nid);
            // Attribute may or may not be found depending on NID validity
        }
    }
    
    // Additional testing: Try to add attribute with invalid parameters
    // This helps test error paths
    
    // Test with zero-length string
    PKCS12_add_friendlyname_asc(bag, "", 0);
    
    // Test with very long string (but within limits)
    std::string long_name = fdp.ConsumeRandomLengthString(1024);
    if (!long_name.empty()) {
        PKCS12_add_friendlyname_utf8(bag, long_name.c_str(), long_name.length());
    }
    
    // Test with NULL string pointer (should handle gracefully)
    // PKCS12_add_friendlyname_asc(bag, NULL, 0); // This would crash, so skip
    
    // Test with invalid NID
    int invalid_nid = fdp.ConsumeIntegral<int>();
    std::vector<unsigned char> test_data = fdp.ConsumeBytes<unsigned char>(10);
    if (!test_data.empty()) {
        PKCS12_add1_attr_by_NID(bag, invalid_nid, V_ASN1_OCTET_STRING,
                               test_data.data(), test_data.size());
    }
    
    // Test retrieving friendly name multiple times
    for (int i = 0; i < 3; i++) {
        char* name_again = PKCS12_get_friendlyname(bag);
        if (name_again != nullptr) {
            OPENSSL_free(name_again);
        }
    }
    
    // Cleanup in reverse order
    PKCS12_SAFEBAG_free(bag);
    X509_free(cert);
    PKCS12_free(p12);
    
    return 0;
}

} // namespace harness_015
