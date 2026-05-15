#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_007 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful testing of ParseWithLength and Compare
    // Need enough data for: operation type, JSON data, length parameters, comparison flags
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Determine the main testing scenario
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Consume JSON data from input
    size_t json_data_size = fdp.ConsumeIntegralInRange<size_t>(1, size - 16);
    std::string json_data = fdp.ConsumeBytesAsString(json_data_size);
    
    // For ParseWithLength, we need to test with various length parameters
    // Some scenarios: exact length, longer than data, shorter than data, zero length
    size_t parse_length = 0;
    
    std::string parse_buffer = json_data;  // Default buffer for parsing
    switch (scenario) {
        case 0:
            // Scenario 0: ParseWithLength with exact length (full JSON data)
            parse_length = json_data.size();
            break;
        case 1:
            // Scenario 1: ParseWithLength with length longer than data
            // Create a padded buffer to safely test longer lengths
            if (json_data.size() > 0) {
                size_t excess = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                // Create padded buffer with null bytes after JSON data
                parse_buffer = json_data + std::string(excess, '\0');
                parse_length = json_data.size() + excess;  // Safe: within padded buffer bounds
            } else {
                parse_length = 0;
            }
            break;
        case 2:
            // Scenario 2: ParseWithLength with length shorter than data (truncated JSON)
            if (json_data.size() > 1) {
                parse_length = fdp.ConsumeIntegralInRange<size_t>(1, json_data.size() - 1);
            } else {
                parse_length = json_data.size();
            }
            break;
        case 3:
            // Scenario 3: ParseWithLength with zero length
            parse_length = 0;
            break;
        case 4:
            // Scenario 4: ParseWithLength with random length
            // FIXED: Clamp to json_data.size() to prevent buffer overflow
            parse_length = fdp.ConsumeIntegralInRange<size_t>(0, json_data.size());
            break;
        case 5:
            // Scenario 5: Compare Parse vs ParseWithLength results
            parse_length = json_data.size();
            break;
    }
    // Test cJSON_ParseWithLength
    cJSON* parsed_with_length = NULL;
    if (parse_length > 0 || scenario == 3) {  // Include zero length test
        parsed_with_length = cJSON_ParseWithLength(parse_buffer.c_str(), parse_length);
    }
    
    // Test regular cJSON_Parse for comparison
    cJSON* parsed_regular = NULL;
    if (json_data.size() > 0) {
        parsed_regular = cJSON_Parse(json_data.c_str());
    }
    
    // Test cJSON_ParseWithOpts (another parsing variant)
    cJSON* parsed_with_opts = NULL;
    if (json_data.size() > 0 && fdp.remaining_bytes() > 0) {
        cJSON_bool require_null_terminated = fdp.ConsumeBool();
        parsed_with_opts = cJSON_ParseWithOpts(json_data.c_str(), NULL, require_null_terminated);
    }
    
    // Now test comparison operations between parsed results
    cJSON_bool case_sensitive = fdp.ConsumeBool();
    
    // Compare various combinations of parsed objects
    if (parsed_with_length != NULL) {
        // Compare with itself (should be equal)
        cJSON_bool self_equal = cJSON_Compare(parsed_with_length, parsed_with_length, case_sensitive);
        
        // Compare with regular parse if available
        if (parsed_regular != NULL) {
            cJSON_bool length_vs_regular = cJSON_Compare(parsed_with_length, parsed_regular, case_sensitive);
            
            // Also test with swapped parameters
            cJSON_bool regular_vs_length = cJSON_Compare(parsed_regular, parsed_with_length, case_sensitive);
            
            // Test with different case sensitivity
            cJSON_bool compare_different_case = cJSON_Compare(parsed_with_length, parsed_regular, !case_sensitive);
        }
        
        // Compare with opts parse if available
        if (parsed_with_opts != NULL) {
            cJSON_bool length_vs_opts = cJSON_Compare(parsed_with_length, parsed_with_opts, case_sensitive);
        }
    }
    
    // Test comparison between regular and opts parses
    if (parsed_regular != NULL && parsed_with_opts != NULL) {
        cJSON_bool regular_vs_opts = cJSON_Compare(parsed_regular, parsed_with_opts, case_sensitive);
    }
    
    // Test edge cases for comparison
    // 1. Compare NULL with valid object
    if (parsed_with_length != NULL) {
        cJSON_bool null_vs_valid = cJSON_Compare(NULL, parsed_with_length, case_sensitive);
        cJSON_bool valid_vs_null = cJSON_Compare(parsed_with_length, NULL, case_sensitive);
    }
    
    // 2. Compare two NULL objects
    cJSON_bool null_vs_null = cJSON_Compare(NULL, NULL, case_sensitive);
    
    // 3. Test with malformed/invalid JSON that returns NULL
    // Create a deliberately malformed JSON by truncating or modifying
    if (fdp.remaining_bytes() > 0) {
        size_t malformed_size = fdp.ConsumeIntegralInRange<size_t>(1, 32);
        if (fdp.remaining_bytes() >= malformed_size) {
            std::string malformed_json = fdp.ConsumeBytesAsString(malformed_size);
            cJSON* malformed_parsed = cJSON_ParseWithLength(malformed_json.c_str(), malformed_json.size());
            
            if (malformed_parsed != NULL) {
                // If somehow it parsed, compare with other parsed objects
                if (parsed_with_length != NULL) {
                    cJSON_bool malformed_vs_valid = cJSON_Compare(malformed_parsed, parsed_with_length, case_sensitive);
                }
                cJSON_Delete(malformed_parsed);
            }
        }
    }
    
    // 4. Test with empty string JSON
    if (fdp.remaining_bytes() > 0) {
        cJSON* empty_parsed = cJSON_ParseWithLength("", 0);
        if (empty_parsed != NULL) {
            cJSON_Delete(empty_parsed);
        }
        
        // Also test with length > 0 but empty string - this is safe with empty string
        // because the buffer is valid (empty string has null terminator)
        cJSON* empty_with_length = cJSON_ParseWithLength("", 0); // Use length 0 to be safe
        if (empty_with_length != NULL) {
            cJSON_Delete(empty_with_length);
        }
    }
    
    // 5. Test comparison with objects of different types
    // Create simple JSON items of different types and compare them
    if (fdp.remaining_bytes() > 0) {
        uint8_t item_type1 = fdp.ConsumeIntegral<uint8_t>() % 5;
        uint8_t item_type2 = fdp.ConsumeIntegral<uint8_t>() % 5;
        
        cJSON* item1 = NULL;
        cJSON* item2 = NULL;
        
        // Create first item
        switch (item_type1) {
            case 0:
                item1 = cJSON_CreateNull();
                break;
            case 1:
                item1 = cJSON_CreateBool(fdp.ConsumeBool());
                break;
            case 2:
                item1 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                break;
            case 3:
                if (fdp.remaining_bytes() > 0) {
                    size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                    if (fdp.remaining_bytes() >= str_len) {
                        std::string str_val = fdp.ConsumeBytesAsString(str_len);
                        item1 = cJSON_CreateString(str_val.c_str());
                    }
                }
                break;
            case 4:
                item1 = cJSON_CreateArray();
                break;
        }
        
        // Create second item
        switch (item_type2) {
            case 0:
                item2 = cJSON_CreateNull();
                break;
            case 1:
                item2 = cJSON_CreateBool(fdp.ConsumeBool());
                break;
            case 2:
                item2 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                break;
            case 3:
                if (fdp.remaining_bytes() > 0) {
                    size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                    if (fdp.remaining_bytes() >= str_len) {
                        std::string str_val = fdp.ConsumeBytesAsString(str_len);
                        item2 = cJSON_CreateString(str_val.c_str());
                    }
                }
                break;
            case 4:
                item2 = cJSON_CreateArray();
                break;
        }
        
        // Compare the two items (may be same type or different types)
        if (item1 != NULL && item2 != NULL) {
            cJSON_bool diff_type_compare = cJSON_Compare(item1, item2, case_sensitive);
            
            // Also test with opposite case sensitivity
            cJSON_bool diff_type_opposite = cJSON_Compare(item1, item2, !case_sensitive);
        }
        
        // Cleanup
        if (item1 != NULL) {
            cJSON_Delete(item1);
        }
        if (item2 != NULL) {
            cJSON_Delete(item2);
        }
    }
    
    // 6. Test ParseWithLengthOpts (the more general version)
    if (fdp.remaining_bytes() > 0 && json_data.size() > 0) {
        cJSON_bool require_null_term = fdp.ConsumeBool();
        const char* parse_end = NULL;
        // Use parse_buffer when parse_length exceeds json_data size
        const char* parse_buffer_ptr = (parse_length > json_data.size()) ? parse_buffer.c_str() : json_data.c_str();
        size_t safe_parse_length = (parse_length > json_data.size()) ? std::min(parse_length, parse_buffer.size()) : 
                                  (parse_length > 0 ? parse_length : json_data.size());
        
        cJSON* parsed_with_opts_full = cJSON_ParseWithLengthOpts(
            parse_buffer_ptr, 
            safe_parse_length,
            &parse_end,
            require_null_term
        );
        
        if (parsed_with_opts_full != NULL) {
            // Compare with other parsed versions
            if (parsed_with_length != NULL) {
                cJSON_bool opts_vs_length = cJSON_Compare(parsed_with_opts_full, parsed_with_length, case_sensitive);
            }
            cJSON_Delete(parsed_with_opts_full);
        }
    }
    
    // Cleanup all parsed objects
    if (parsed_with_length != NULL) {
        cJSON_Delete(parsed_with_length);
    }
    if (parsed_regular != NULL) {
        cJSON_Delete(parsed_regular);
    }
    if (parsed_with_opts != NULL) {
        cJSON_Delete(parsed_with_opts);
    }
    
    return 0;
}

} // namespace harness_007
