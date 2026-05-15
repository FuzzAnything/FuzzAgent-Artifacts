#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_021 {

extern "C" {
}

// Helper function to generate Unicode escape sequence string
std::string generate_unicode_escape_sequence(FuzzedDataProvider& fdp, int test_case_type) {
    std::string result;
    
    switch (test_case_type) {
        case 0: // Single \uXXXX sequence (ASCII range)
            result = "\\u";
            {
                uint16_t code = fdp.ConsumeIntegralInRange<uint16_t>(0x0020, 0x007F); // Printable ASCII
                char hex[5];
                snprintf(hex, sizeof(hex), "%04X", code);
                result += hex;
            }
            break;
            
        case 1: // Single \uXXXX sequence (2-byte UTF-8 range)
            result = "\\u";
            {
                uint16_t code = fdp.ConsumeIntegralInRange<uint16_t>(0x0080, 0x07FF);
                char hex[5];
                snprintf(hex, sizeof(hex), "%04X", code);
                result += hex;
            }
            break;
            
        case 2: // Single \uXXXX sequence (3-byte UTF-8 range, non-surrogate)
            result = "\\u";
            {
                uint16_t code = fdp.ConsumeIntegralInRange<uint16_t>(0x0800, 0xD7FF);
                char hex[5];
                snprintf(hex, sizeof(hex), "%04X", code);
                result += hex;
            }
            break;
            
        case 3: // Valid UTF-16 surrogate pair (4-byte UTF-8 range)
            result = "\\u";
            {
                // First surrogate: high surrogate range 0xD800-0xDBFF
                uint16_t first = fdp.ConsumeIntegralInRange<uint16_t>(0xD800, 0xDBFF);
                char hex1[5];
                snprintf(hex1, sizeof(hex1), "%04X", first);
                result += hex1;
                
                result += "\\u";
                
                // Second surrogate: low surrogate range 0xDC00-0xDFFF
                uint16_t second = fdp.ConsumeIntegralInRange<uint16_t>(0xDC00, 0xDFFF);
                char hex2[5];
                snprintf(hex2, sizeof(hex2), "%04X", second);
                result += hex2;
            }
            break;
            
        case 4: // Invalid: first code in low surrogate range (0xDC00-0xDFFF)
            result = "\\u";
            {
                uint16_t code = fdp.ConsumeIntegralInRange<uint16_t>(0xDC00, 0xDFFF);
                char hex[5];
                snprintf(hex, sizeof(hex), "%04X", code);
                result += hex;
            }
            break;
            
        case 5: // Invalid surrogate pair: missing second half
            result = "\\u";
            {
                uint16_t first = fdp.ConsumeIntegralInRange<uint16_t>(0xD800, 0xDBFF);
                char hex[5];
                snprintf(hex, sizeof(hex), "%04X", first);
                result += hex;
            }
            // No second sequence - this should cause parse failure
            break;
            
        case 6: // Invalid surrogate pair: second half outside low surrogate range
            result = "\\u";
            {
                uint16_t first = fdp.ConsumeIntegralInRange<uint16_t>(0xD800, 0xDBFF);
                char hex1[5];
                snprintf(hex1, sizeof(hex1), "%04X", first);
                result += hex1;
                
                result += "\\u";
                
                // Invalid second surrogate - outside 0xDC00-0xDFFF range
                uint16_t second = fdp.ConsumeIntegralInRange<uint16_t>(0x0000, 0xD7FF); // Non-surrogate
                if (second >= 0xDC00 && second <= 0xDFFF) {
                    second = 0xD7FF; // Avoid valid low surrogate
                }
                char hex2[5];
                snprintf(hex2, sizeof(hex2), "%04X", second);
                result += hex2;
            }
            break;
            
        case 7: // Unicode maximum codepoint (0x10FFFF)
            result = "\\uD83F\\uDFFF"; // Last valid Unicode codepoint
            break;
            
        case 8: // Mixed ASCII and Unicode
            {
                std::string ascii_part = fdp.ConsumeRandomLengthString(10);
                result = ascii_part + "\\u";
                uint16_t code = fdp.ConsumeIntegralInRange<uint16_t>(0x0041, 0x005A); // A-Z
                char hex[5];
                snprintf(hex, sizeof(hex), "%04X", code);
                result += hex + ascii_part;
            }
            break;
            
        case 9: // Multiple Unicode sequences
            {
                int count = fdp.ConsumeIntegralInRange<int>(2, 5);
                for (int i = 0; i < count; i++) {
                    result += "\\u";
                    uint16_t code = fdp.ConsumeIntegralInRange<uint16_t>(0x0041, 0x005A); // A-Z
                    char hex[5];
                    snprintf(hex, sizeof(hex), "%04X", code);
                    result += hex;
                }
            }
            break;
            
        case 10: // Boundary: input buffer exactly at limits (short sequences)
            {
                int seq_length = fdp.ConsumeIntegralInRange<int>(1, 5);
                for (int i = 0; i < seq_length; i++) {
                    result += "\\u";
                    if (fdp.remaining_bytes() < 2) break;
                    uint16_t code = fdp.ConsumeIntegral<uint16_t>();
                    char hex[5];
                    snprintf(hex, sizeof(hex), "%04X", code);
                    result += hex;
                }
            }
            break;
            
        case 11: // Emoji example: smiling face with open mouth
            result = "\\uD83D\\uDE00";
            break;
            
        default:
            result = "\\u0041"; // Default: 'A'
            break;
    }
    
    return result;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Need sufficient input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Determine test configuration from fuzzer input
    int test_case_count = fdp.ConsumeIntegralInRange<int>(1, 20);
    bool test_round_trip = fdp.ConsumeBool();
    bool test_printing = fdp.ConsumeBool();
    
    std::vector<std::string> test_cases;
    
    // Generate test cases using fuzzer input
    for (int i = 0; i < test_case_count && fdp.remaining_bytes() > 0; i++) {
        int test_type = fdp.ConsumeIntegralInRange<int>(0, 11);
        test_cases.push_back(generate_unicode_escape_sequence(fdp, test_type));
    }
    
    // Test each case
    for (const auto& unicode_seq : test_cases) {
        // Create JSON string with the Unicode sequence
        std::string json_str = "{\"text\": \"" + unicode_seq + "\"}";
        
        // Parse the JSON
        cJSON* parsed = cJSON_Parse(json_str.c_str());
        
        if (parsed != NULL) {
            // Test printing paths if enabled
            if (test_printing) {
                // Test cJSON_Print (formatted)
                char* printed = cJSON_Print(parsed);
                if (printed != NULL) {
                    // Optionally test cJSON_PrintUnformatted
                    char* unformatted = cJSON_PrintUnformatted(parsed);
                    if (unformatted != NULL) {
                        free(unformatted);
                    }
                    
                    // Test round-trip parsing if enabled
                    if (test_round_trip) {
                        cJSON* re_parsed = cJSON_Parse(printed);
                        if (re_parsed != NULL) {
                            cJSON_Delete(re_parsed);
                        }
                    }
                    
                    free(printed);
                }
            }
            
            cJSON_Delete(parsed);
        }
        
        // Also test with cJSON_ParseWithOpts for more coverage
        const char* end_ptr = nullptr;
        cJSON* parsed_with_opts = cJSON_ParseWithOpts(json_str.c_str(), &end_ptr, 1); // Require null-terminated
        if (parsed_with_opts != NULL) {
            cJSON_Delete(parsed_with_opts);
        }
    }
    
    // Additional test: Create JSON programmatically with Unicode strings
    if (fdp.remaining_bytes() > 0) {
        cJSON* root = cJSON_CreateObject();
        if (root != NULL) {
            // Add a string with Unicode escapes programmatically
            std::string key = "unicode_key";
            
            // Create a string value with potential Unicode content
            std::string value = "Text with ";
            value += generate_unicode_escape_sequence(fdp, fdp.ConsumeIntegralInRange<int>(0, 3));
            value += " sequence";
            
            cJSON_AddStringToObject(root, key.c_str(), value.c_str());
            
            // Print it to trigger print_string_ptr
            if (test_printing) {
                char* printed = cJSON_Print(root);
                if (printed != NULL) {
                    free(printed);
                }
            }
            
            cJSON_Delete(root);
        }
    }
    
    // Test boundary conditions with minimal input
    if (fdp.remaining_bytes() > 0) {
        // Test very short Unicode sequences that might be at buffer boundaries
        std::vector<std::string> boundary_cases = {
            "\\u",           // Too short
            "\\u0",          // Still too short  
            "\\u00",         // Getting there
            "\\u001",        // Almost
            "\\u001F",       // Valid
            "\\uD800",       // High surrogate alone
            "\\uD800\\u",    // High surrogate with incomplete second
            "\\uD800\\uD",   // High surrogate with very incomplete second
        };
        
        for (const auto& boundary_case : boundary_cases) {
            std::string json = "{\"b\": \"" + boundary_case + "\"}";
            cJSON* parsed = cJSON_Parse(json.c_str());
            if (parsed != NULL) {
                cJSON_Delete(parsed);
            }
        }
    }
    
    return 0;
}

} // namespace harness_021
