#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_010 {
/*
 * Harness 010: Advanced JSON Processing with Minification, Comparison, and Duplication
 * 
 * Target: Uncovered cJSON APIs with semantic diversity from existing harnesses
 * 
 * This harness focuses on:
 * 1. cJSON_Minify function for JSON minification (line 266: 0 hits in coverage)
 * 2. Comparison functions: cJSON_Compare with case sensitivity variations
 * 3. Duplication: cJSON_Duplicate with recursion options
 * 4. Advanced parsing: cJSON_ParseWithLengthOpts, cJSON_ParseWithOpts with various options
 * 5. Combined realistic sequences for semantic diversity
 * 
 * Key coverage targets:
 * - cJSON_Minify edge cases: empty JSON, whitespace-only, malformed input
 * - cJSON_Compare with case_sensitive variations on objects and arrays
 * - cJSON_Duplicate with recursion depth testing (0 and 1)
 * - Combined workflows: parse → minify → compare → duplicate → print
 */

static void* custom_malloc(size_t size) {
    return malloc(size);
}

static void custom_free(void* ptr) {
    free(ptr);
}
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful testing
    // Need enough data for JSON input, configuration, and operation selection
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size configuration data first
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;
    bool case_sensitive = fdp.ConsumeBool();
    bool duplicate_recursive = fdp.ConsumeBool();
    bool use_custom_hooks = fdp.ConsumeBool();
    bool require_null_terminated = fdp.ConsumeBool();
    uint8_t minify_test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume JSON input for parsing and minification
    std::string json_input1 = fdp.ConsumeRandomLengthString(2048);
    std::string json_input2 = fdp.ConsumeRandomLengthString(2048);
    std::string whitespace_string = fdp.ConsumeRandomLengthString(512);
    
    // Consume numeric arrays for comparison testing
    std::vector<int> int_array1, int_array2;
    std::vector<double> double_array1, double_array2;
    uint8_t array_size = fdp.ConsumeIntegralInRange<uint8_t>(1, 10);
    
    for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
        if (fdp.remaining_bytes() >= sizeof(int)) {
            int_array1.push_back(fdp.ConsumeIntegral<int>());
            int_array2.push_back(fdp.ConsumeIntegral<int>());
        }
        if (fdp.remaining_bytes() >= sizeof(double)) {
            double_array1.push_back(fdp.ConsumeFloatingPoint<double>());
            double_array2.push_back(fdp.ConsumeFloatingPoint<double>());
        }
    }
    
    // Initialize custom memory hooks if requested
    if (use_custom_hooks) {
        cJSON_Hooks hooks;
        hooks.malloc_fn = custom_malloc;
        hooks.free_fn = custom_free;
        cJSON_InitHooks(&hooks);
    }
    
    // Use operation to determine API flow with semantic diversity
    switch (operation) {
        case 0: {
            // Test cJSON_Minify with various input scenarios
            char* minify_buffer = NULL;
            size_t buffer_size = 0;
            
            switch (minify_test_type) {
                case 0: {
                    // Test with regular JSON input
                    buffer_size = json_input1.length() + 1;
                    minify_buffer = (char*)malloc(buffer_size);
                    if (minify_buffer != NULL) {
                        memcpy(minify_buffer, json_input1.c_str(), buffer_size);
                        cJSON_Minify(minify_buffer);
                        free(minify_buffer);
                    }
                    break;
                }
                    
                case 1: {
                    // Test with whitespace-heavy JSON (use second JSON input with simulated whitespace)
                    // Create a JSON string by wrapping json_input2 with whitespace
                    std::string whitespaced_json = "  {  \"data\": " + json_input2 + "  }  ";
                    buffer_size = whitespaced_json.length() + 1;
                    minify_buffer = (char*)malloc(buffer_size);
                    if (minify_buffer != NULL) {
                        memcpy(minify_buffer, whitespaced_json.c_str(), buffer_size);
                        cJSON_Minify(minify_buffer);
                        free(minify_buffer);
                    }
                    break;
                }
                    
                case 2: {
                    // Test with empty string
                    buffer_size = 1;
                    minify_buffer = (char*)malloc(buffer_size);
                    if (minify_buffer != NULL) {
                        minify_buffer[0] = '\0';
                        cJSON_Minify(minify_buffer);
                        free(minify_buffer);
                    }
                    break;
                }
                    
                case 3: {
                    // Test with malformed JSON (fuzzer-generated whitespace)
                    buffer_size = whitespace_string.length() + 1;
                    minify_buffer = (char*)malloc(buffer_size);
                    if (minify_buffer != NULL) {
                        memcpy(minify_buffer, whitespace_string.c_str(), buffer_size);
                        cJSON_Minify(minify_buffer);
                        free(minify_buffer);
                    }
                    break;
                }
            }
            break;
        }
            
        case 1: {
            // Test cJSON_Compare with case sensitivity variations
            cJSON* item1 = cJSON_Parse(json_input1.c_str());
            cJSON* item2 = cJSON_Parse(json_input2.c_str());
            
            if (item1 != NULL && item2 != NULL) {
                // Test comparison with case sensitivity flag
                cJSON_bool compare_result = cJSON_Compare(item1, item2, case_sensitive);
                (void)compare_result; // Suppress unused variable warning
                
                // Test self-comparison (should be equal)
                cJSON_bool self_compare = cJSON_Compare(item1, item1, case_sensitive);
                (void)self_compare;
                
                // Test with NULL items
                cJSON_bool null_compare1 = cJSON_Compare(NULL, item2, case_sensitive);
                cJSON_bool null_compare2 = cJSON_Compare(item1, NULL, case_sensitive);
                cJSON_bool null_compare3 = cJSON_Compare(NULL, NULL, case_sensitive);
                (void)null_compare1; (void)null_compare2; (void)null_compare3;
            }
            
            if (item1 != NULL) cJSON_Delete(item1);
            if (item2 != NULL) cJSON_Delete(item2);
            break;
        }
            
        case 2: {
            // Test cJSON_Duplicate with recursion options
            cJSON* original = cJSON_Parse(json_input1.c_str());
            if (original != NULL) {
                // Test duplicate with recursion
                cJSON* duplicate_recursive_item = cJSON_Duplicate(original, duplicate_recursive ? 1 : 0);
                if (duplicate_recursive_item != NULL) {
                    // Compare original with duplicate
                    cJSON_bool compare_dup = cJSON_Compare(original, duplicate_recursive_item, case_sensitive);
                    (void)compare_dup;
                    cJSON_Delete(duplicate_recursive_item);
                }
                
                // Test duplicate without recursion
                cJSON* duplicate_nonrecursive = cJSON_Duplicate(original, 0);
                if (duplicate_nonrecursive != NULL) {
                    cJSON_Delete(duplicate_nonrecursive);
                }
                
                // Test duplicate with invalid recursion value
                cJSON* duplicate_invalid = cJSON_Duplicate(original, 2); // Should act like 1
                if (duplicate_invalid != NULL) {
                    cJSON_Delete(duplicate_invalid);
                }
                
                // Test duplicate of NULL
                cJSON* null_duplicate = cJSON_Duplicate(NULL, duplicate_recursive ? 1 : 0);
                (void)null_duplicate; // Should be NULL
                
                cJSON_Delete(original);
            }
            break;
        }
            
        case 3: {
            cJSON* parsed1 = cJSON_ParseWithOpts(json_input1.c_str(), NULL, require_null_terminated);
            cJSON* parsed2 = cJSON_ParseWithLengthOpts(json_input2.c_str(), json_input2.length(), NULL, require_null_terminated);
            
            if (parsed1 != NULL) {
                // Print and minify workflow
                char* printed1 = cJSON_Print(parsed1);
                if (printed1 != NULL) {
                    // Create a mutable copy for minification
                    size_t printed_len = strlen(printed1);
                    char* minify_target = (char*)malloc(printed_len + 1);
                    if (minify_target != NULL) {
                        memcpy(minify_target, printed1, printed_len + 1);
                        cJSON_Minify(minify_target);
                        free(minify_target);
                    }
                    free(printed1);
                }
                
                // Duplicate and compare workflow
                cJSON* duplicate = cJSON_Duplicate(parsed1, duplicate_recursive ? 1 : 0);
                if (duplicate != NULL) {
                    if (parsed2 != NULL) {
                        cJSON_bool compare_result = cJSON_Compare(parsed1, parsed2, case_sensitive);
                        cJSON_bool dup_compare_result = cJSON_Compare(parsed1, duplicate, case_sensitive);
                        (void)compare_result; (void)dup_compare_result;
                    }
                    cJSON_Delete(duplicate);
                }
                
                cJSON_Delete(parsed1);
            }
            
            if (parsed2 != NULL) {
                cJSON_Delete(parsed2);
            }
            break;
        }
            
        case 4: {
            // Test array comparison scenarios with cJSON_Compare
            cJSON* array1 = NULL;
            cJSON* array2 = NULL;
            
            if (!int_array1.empty()) {
                array1 = cJSON_CreateIntArray(int_array1.data(), int_array1.size());
            }
            if (!int_array2.empty()) {
                array2 = cJSON_CreateIntArray(int_array2.data(), int_array2.size());
            }
            
            if (array1 != NULL && array2 != NULL) {
                cJSON_bool array_compare = cJSON_Compare(array1, array2, case_sensitive);
                (void)array_compare;
            }
            
            // Test with double arrays
            cJSON* double_array_json1 = NULL;
            cJSON* double_array_json2 = NULL;
            
            if (!double_array1.empty()) {
                double_array_json1 = cJSON_CreateDoubleArray(double_array1.data(), double_array1.size());
            }
            if (!double_array2.empty()) {
                double_array_json2 = cJSON_CreateDoubleArray(double_array2.data(), double_array2.size());
            }
            
            if (double_array_json1 != NULL && double_array_json2 != NULL) {
                cJSON_bool double_array_compare = cJSON_Compare(double_array_json1, double_array_json2, case_sensitive);
                (void)double_array_compare;
            }
            
            // Test mixed type comparison (should be false)
            if (array1 != NULL && double_array_json1 != NULL) {
                cJSON_bool mixed_compare = cJSON_Compare(array1, double_array_json1, case_sensitive);
                (void)mixed_compare;
            }
            
            if (array1 != NULL) cJSON_Delete(array1);
            if (array2 != NULL) cJSON_Delete(array2);
            if (double_array_json1 != NULL) cJSON_Delete(double_array_json1);
            if (double_array_json2 != NULL) cJSON_Delete(double_array_json2);
            break;
        }
            
        case 5: {
            // Test advanced parsing options with return_parse_end
            const char* return_parse_end1 = NULL;
            const char* return_parse_end2 = NULL;
            
            // Test cJSON_ParseWithOpts with return_parse_end
            cJSON* parsed_opts1 = cJSON_ParseWithOpts(json_input1.c_str(), &return_parse_end1, require_null_terminated);
            if (parsed_opts1 != NULL) {
                // Test duplicate with recursion
                cJSON* duplicate = cJSON_Duplicate(parsed_opts1, duplicate_recursive ? 1 : 0);
                if (duplicate != NULL) {
                    // Minify the duplicate's printed output
                    char* printed_dup = cJSON_Print(duplicate);
                    if (printed_dup != NULL) {
                        char* minify_buffer = strdup(printed_dup);
                        if (minify_buffer != NULL) {
                            cJSON_Minify(minify_buffer);
                            free(minify_buffer);
                        }
                        free(printed_dup);
                    }
                    cJSON_Delete(duplicate);
                }
            }
            
            // Test cJSON_ParseWithLengthOpts with return_parse_end
            size_t parse_length = json_input2.length();
            if (parse_length > 0) {
                cJSON* parsed_length_opts = cJSON_ParseWithLengthOpts(json_input2.c_str(), parse_length, &return_parse_end2, require_null_terminated);
                if (parsed_length_opts != NULL) {
                    // Compare with first parse if both succeeded
                    if (parsed_opts1 != NULL) {
                        cJSON_bool compare_parsed = cJSON_Compare(parsed_opts1, parsed_length_opts, case_sensitive);
                        (void)compare_parsed;
                    }
                    cJSON_Delete(parsed_length_opts);
                }
            }
            
            // Now delete parsed_opts1 after comparison
            if (parsed_opts1 != NULL) {
                cJSON_Delete(parsed_opts1);
            }
            
            // Test edge cases: NULL input with opts
            cJSON* null_parse_opts = cJSON_ParseWithOpts(NULL, NULL, require_null_terminated);
            (void)null_parse_opts; // Should be NULL
            
            // Test with zero-length input
            if (json_input1.length() > 0) {
                cJSON* zero_length_parse = cJSON_ParseWithLengthOpts(json_input1.c_str(), 0, NULL, require_null_terminated);
                (void)zero_length_parse; // Should be NULL
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_010
