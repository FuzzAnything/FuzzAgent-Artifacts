#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include "cjson/cJSON.h"

namespace harness_019 {
/*
 * Harness 019: Targeted cJSON_Compare fuzzing harness
 * 
 * Focus: Diverse JSON comparison scenarios targeting specific blocked branches in cJSON_Compare
 * Blocked branches to target:
 * 1. NULL comparisons (line 3059, 3081 - 0 hits)
 * 2. Type mismatch detection (line 3081)
 * 3. Default type case (line 3096 - 50 hits, needs invalid types)
 * 4. Identical object shortcut (line 3101 - 1.21k hits)
 * 5. Number comparison with precision (line 3115 - 2.76k hits)
 * 6. String/Raw comparison with NULL values (line 3126 - 155 hits)
 * 7. Array length mismatch detection (line 3154 - 124 hits)
 * 8. Object key missing scenarios (line 3169 - 393 hits, line 3185 - 95 hits)
 * 9. Recursive comparison failures (line 3144, 3174, 3190)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Minimum size for meaningful comparisons
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data for configuration
    uint8_t comparison_type = fdp.ConsumeIntegral<uint8_t>() % 10;
    bool case_sensitive = fdp.ConsumeBool();
    bool create_null_objects = fdp.ConsumeBool();
    bool create_invalid_types = fdp.ConsumeBool();
    
    // Consume numeric values for number comparisons
    double num1 = fdp.ConsumeFloatingPoint<double>();
    double num2 = fdp.ConsumeFloatingPoint<double>();
    int int1 = fdp.ConsumeIntegral<int>();
    int int2 = fdp.ConsumeIntegral<int>();
    
    // Consume string data
    std::string str1 = fdp.ConsumeRandomLengthString(256);
    std::string str2 = fdp.ConsumeRandomLengthString(256);
    std::string key1 = fdp.ConsumeRandomLengthString(64);
    std::string key2 = fdp.ConsumeRandomLengthString(64);
    std::string key3 = fdp.ConsumeRandomLengthString(64);
    
    // Remaining bytes for JSON parsing
    std::string json_input = fdp.ConsumeRemainingBytesAsString();
    
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *parsed1 = NULL;
    cJSON *parsed2 = NULL;
    
    // Test scenario based on comparison_type
    switch (comparison_type) {
        case 0: {
            // Scenario 1: NULL comparisons (targets line 3078)
            // Test various NULL combinations
            cJSON_Compare(NULL, NULL, case_sensitive);
            
            item1 = cJSON_CreateNull();
            cJSON_Compare(item1, NULL, case_sensitive);
            cJSON_Compare(NULL, item1, case_sensitive);
            
            item2 = cJSON_CreateTrue();
            cJSON_Compare(item1, item2, case_sensitive);  // NULL vs non-NULL
            
            if (item1) cJSON_Delete(item1);
            if (item2) cJSON_Delete(item2);
            break;
        }
        
        case 1: {
            // Scenario 2: Type mismatch detection (targets line 3078, 3081)
            item1 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            item2 = cJSON_CreateString(str1.c_str());
            cJSON_Compare(item1, item2, case_sensitive);  // Number vs String mismatch
            
            if (item1) cJSON_Delete(item1);
            if (item2) cJSON_Delete(item2);
            
            // More type mismatches
            item1 = cJSON_CreateArray();
            item2 = cJSON_CreateObject();
            cJSON_Compare(item1, item2, case_sensitive);  // Array vs Object mismatch
            
            if (item1) cJSON_Delete(item1);
            if (item2) cJSON_Delete(item2);
            
            // String vs Raw mismatch
            std::string ref_str = fdp.ConsumeRandomLengthString(32);
            std::string raw_json = "{\"" + fdp.ConsumeRandomLengthString(16) + "\":\"" + fdp.ConsumeRandomLengthString(16) + "\"}";
            item1 = cJSON_CreateStringReference(ref_str.c_str());
            item2 = cJSON_CreateRaw(raw_json.c_str());
            cJSON_Compare(item1, item2, case_sensitive);
            
            if (item1) cJSON_Delete(item1);
            if (item2) cJSON_Delete(item2);
            break;
        }
        
        case 2: {
            // Scenario 3: Default type case (line 3096) - needs invalid types
            // Create objects with potentially invalid type bits
            item1 = cJSON_CreateNumber(123.456);
            if (item1 && create_invalid_types) {
                // Try to create invalid type by manipulating bits
                // Note: This is testing edge case handling
                item1->type = 0xFF;  // Invalid type
            }
            
            item2 = cJSON_CreateNumber(123.456);
            if (item2 && create_invalid_types) {
                item2->type = 0x80;  // Another potentially invalid type
            }
            
            if (item1 && item2) {
                cJSON_Compare(item1, item2, case_sensitive);
            }
            
            if (item1) cJSON_Delete(item1);
            if (item2) cJSON_Delete(item2);
            break;
        }
        
        case 3: {
            // Scenario 4: Identical object shortcut (line 3101)
            item1 = cJSON_CreateObject();
            cJSON_Compare(item1, item1, case_sensitive);  // Same pointer comparison
            
            // Create identical objects (different pointers but same content)
            item2 = cJSON_CreateObject();
            cJSON_AddStringToObject(item1, key1.c_str(), str1.c_str());
            cJSON_AddStringToObject(item2, key1.c_str(), str1.c_str());
            
            cJSON_Compare(item1, item2, case_sensitive);
            
            if (item1) cJSON_Delete(item1);
            if (item2) cJSON_Delete(item2);
            break;
        }
        
        case 4: {
            // Scenario 5: Number comparison with precision (line 3115)
            item1 = cJSON_CreateNumber(num1);
            item2 = cJSON_CreateNumber(num2);
            cJSON_Compare(item1, item2, case_sensitive);
            
            // Test edge cases for floating point comparison using fuzzed data
            // Create potentially NaN/Inf values using fuzzed operations
            double nan_candidate = fdp.ConsumeFloatingPoint<double>();
            double inf_candidate = fdp.ConsumeFloatingPoint<double>();
            double neg_inf_candidate = fdp.ConsumeFloatingPoint<double>();
            
            // Try to create edge cases - these may or may not be NaN/Inf depending on values
            cJSON *edge_item1 = cJSON_CreateNumber(nan_candidate);
            cJSON *edge_item2 = cJSON_CreateNumber(inf_candidate);
            cJSON *edge_item3 = cJSON_CreateNumber(neg_inf_candidate);
            
            cJSON_Compare(edge_item1, item1, case_sensitive);
            cJSON_Compare(edge_item2, item2, case_sensitive);
            cJSON_Compare(edge_item3, edge_item1, case_sensitive);
            
            // Same number, different representation - use fuzzed values
            double same_num = fdp.ConsumeFloatingPoint<double>();
            cJSON *int_item = cJSON_CreateNumber((double)fdp.ConsumeIntegral<int>());
            cJSON *double_item = cJSON_CreateNumber(same_num);
            cJSON_Compare(int_item, double_item, case_sensitive);
            
            // Test with very small and very large numbers
            double tiny_num = fdp.ConsumeFloatingPointInRange<double>(1e-308, 1e-308 * 2);
            double huge_num = fdp.ConsumeFloatingPointInRange<double>(1e308, 1e308 * 2);
            cJSON *tiny_item = cJSON_CreateNumber(tiny_num);
            cJSON *huge_item = cJSON_CreateNumber(huge_num);
            cJSON_Compare(tiny_item, huge_item, case_sensitive);
            
            if (item1) cJSON_Delete(item1);
            if (item2) cJSON_Delete(item2);
            if (edge_item1) cJSON_Delete(edge_item1);
            if (edge_item2) cJSON_Delete(edge_item2);
            if (edge_item3) cJSON_Delete(edge_item3);
            if (int_item) cJSON_Delete(int_item);
            if (double_item) cJSON_Delete(double_item);
            if (tiny_item) cJSON_Delete(tiny_item);
            if (huge_item) cJSON_Delete(huge_item);
            break;
        }
        
        case 5: {
            // Scenario 6: String/Raw comparison with NULL values (line 3123, 3126)
            // Create string with NULL valuestring
            item1 = cJSON_CreateString(str1.c_str());
            item2 = cJSON_CreateString(str2.c_str());
            
            // Test normal string comparison
            cJSON_Compare(item1, item2, case_sensitive);
            
            // Test with empty strings
            cJSON *empty_str = cJSON_CreateString("");
            cJSON_Compare(item1, empty_str, case_sensitive);
            
            // Create Raw JSON objects using fuzzed data
            std::string raw_key1 = fdp.ConsumeRandomLengthString(16);
            std::string raw_val1 = fdp.ConsumeRandomLengthString(32);
            std::string raw_key2 = fdp.ConsumeRandomLengthString(16);
            std::string raw_val2 = fdp.ConsumeRandomLengthString(32);
            std::string raw_json1 = "{\"" + raw_key1 + "\":\"" + raw_val1 + "\"}";
            std::string raw_json2 = "{\"" + raw_key2 + "\":\"" + raw_val2 + "\"}";
            cJSON *raw1 = cJSON_CreateRaw(raw_json1.c_str());
            cJSON *raw2 = cJSON_CreateRaw(raw_json2.c_str());
            cJSON_Compare(raw1, raw2, case_sensitive);
            
            // Test case-sensitive vs case-insensitive with fuzzed strings
            std::string upper_str = fdp.ConsumeRandomLengthString(32);
            std::string lower_str = upper_str;
            // Convert to lowercase for case-insensitive test
            std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
            std::transform(upper_str.begin(), upper_str.end(), upper_str.begin(), ::toupper);
            
            cJSON *upper = cJSON_CreateString(upper_str.c_str());
            cJSON *lower = cJSON_CreateString(lower_str.c_str());
            cJSON_Compare(upper, lower, false);  // case-insensitive should match?
            cJSON_Compare(upper, lower, true);   // case-sensitive should not match
            
            if (item1) cJSON_Delete(item1);
            if (item2) cJSON_Delete(item2);
            if (empty_str) cJSON_Delete(empty_str);
            if (raw1) cJSON_Delete(raw1);
            if (raw2) cJSON_Delete(raw2);
            if (upper) cJSON_Delete(upper);
            if (lower) cJSON_Delete(lower);
            break;
        }
        case 6: {
            // Scenario 7: Array length mismatch detection (line 3151, 3154)
            item1 = cJSON_CreateArray();
            item2 = cJSON_CreateArray();
            
            // Add different number of elements to arrays
            for (int i = 0; i < 3; i++) {
                cJSON_AddItemToArray(item1, cJSON_CreateNumber(i));
            }
            for (int i = 0; i < 5; i++) {
                cJSON_AddItemToArray(item2, cJSON_CreateNumber(i * 2));
            }
            
            cJSON_Compare(item1, item2, case_sensitive);  // Different lengths
            
            // Create arrays with same length but different values
            cJSON *array1 = cJSON_CreateArray();
            cJSON *array2 = cJSON_CreateArray();
            for (int i = 0; i < 4; i++) {
                cJSON_AddItemToArray(array1, cJSON_CreateNumber(i));
                cJSON_AddItemToArray(array2, cJSON_CreateNumber(i + 10));
            }
            cJSON_Compare(array1, array2, case_sensitive);  // Same length, different values
            
            // Nested arrays
            cJSON *nested1 = cJSON_CreateArray();
            cJSON *nested2 = cJSON_CreateArray();
            cJSON *inner1 = cJSON_CreateArray();
            cJSON *inner2 = cJSON_CreateArray();
            
            cJSON_AddItemToArray(inner1, cJSON_CreateNumber(1));
            cJSON_AddItemToArray(inner2, cJSON_CreateNumber(2));
            cJSON_AddItemToArray(nested1, inner1);
            cJSON_AddItemToArray(nested2, inner2);
            
            cJSON_Compare(nested1, nested2, case_sensitive);
            
            if (item1) cJSON_Delete(item1);
            if (item2) cJSON_Delete(item2);
            if (array1) cJSON_Delete(array1);
            if (array2) cJSON_Delete(array2);
            if (nested1) cJSON_Delete(nested1);
            if (nested2) cJSON_Delete(nested2);
            break;
        }
        
        case 7: {
            // Scenario 8: Object key missing scenarios (line 3166, 3169, 3182, 3185)
            item1 = cJSON_CreateObject();
            item2 = cJSON_CreateObject();
            
            // Add different keys to objects
            cJSON_AddStringToObject(item1, key1.c_str(), str1.c_str());
            cJSON_AddNumberToObject(item1, key2.c_str(), fdp.ConsumeFloatingPoint<double>());
            
            cJSON_AddStringToObject(item2, key1.c_str(), str2.c_str());  // Same key, different value
            cJSON_AddNumberToObject(item2, key3.c_str(), fdp.ConsumeFloatingPoint<double>());  // Different key
            
            cJSON_Compare(item1, item2, case_sensitive);  // Missing key scenario
            
            // Test subset objects (a is subset of b)
            std::string common_key = fdp.ConsumeRandomLengthString(16);
            std::string common_val = fdp.ConsumeRandomLengthString(32);
            std::string extra_key = fdp.ConsumeRandomLengthString(16);
            
            cJSON *obj_a = cJSON_CreateObject();
            cJSON *obj_b = cJSON_CreateObject();
            
            cJSON_AddStringToObject(obj_a, common_key.c_str(), common_val.c_str());
            cJSON_AddNumberToObject(obj_a, "num", fdp.ConsumeIntegral<int>());
            
            cJSON_AddStringToObject(obj_b, common_key.c_str(), common_val.c_str());
            cJSON_AddNumberToObject(obj_b, "num", fdp.ConsumeIntegral<int>());
            cJSON_AddBoolToObject(obj_b, extra_key.c_str(), fdp.ConsumeBool());  // Extra key in b
            
            cJSON_Compare(obj_a, obj_b, case_sensitive);  // Should fail - b has extra key
            
            if (item1) cJSON_Delete(item1);
            if (item2) cJSON_Delete(item2);
            if (obj_a) cJSON_Delete(obj_a);
            if (obj_b) cJSON_Delete(obj_b);
            break;
        }
        case 8: {
            // Scenario 9: Recursive comparison failures (lines 3144, 3174, 3190)
            // Create deeply nested structures that differ at various depths
            cJSON *deep1 = cJSON_CreateObject();
            cJSON *deep2 = cJSON_CreateObject();
            
            // Build nested structure
            cJSON *current1 = deep1;
            cJSON *current2 = deep2;
            
            for (int depth = 0; depth < 4; depth++) {
                char key[32];
                snprintf(key, sizeof(key), "level%d", depth);
                
                cJSON *next1 = cJSON_CreateObject();
                cJSON *next2 = cJSON_CreateObject();
                
                cJSON_AddItemToObject(current1, key, next1);
                if (depth < 3) {
                    // Make structures differ at level 3
                    cJSON_AddItemToObject(current2, key, next2);
                } else {
                    // Add different type at deepest level to cause recursive failure
                    cJSON_AddNumberToObject(current2, key, 42);
                }
                
                current1 = next1;
                if (depth < 3) {
                    current2 = next2;
                }
            }
            
            cJSON_Compare(deep1, deep2, case_sensitive);  // Deep recursive failure
            
            // Test recursive array comparison failures
            cJSON *recursive_array1 = cJSON_CreateArray();
            cJSON *recursive_array2 = cJSON_CreateArray();
            
            for (int i = 0; i < 3; i++) {
                cJSON *inner_array1 = cJSON_CreateArray();
                cJSON *inner_array2 = cJSON_CreateArray();
                
                cJSON_AddItemToArray(inner_array1, cJSON_CreateNumber(i));
                cJSON_AddItemToArray(inner_array2, cJSON_CreateNumber(i * 2));  // Different values
                
                cJSON_AddItemToArray(recursive_array1, inner_array1);
                cJSON_AddItemToArray(recursive_array2, inner_array2);
            }
            
            cJSON_Compare(recursive_array1, recursive_array2, case_sensitive);
            
            if (deep1) cJSON_Delete(deep1);
            if (deep2) cJSON_Delete(deep2);
            if (recursive_array1) cJSON_Delete(recursive_array1);
            if (recursive_array2) cJSON_Delete(recursive_array2);
            break;
        }
        
        case 9: {
            // Scenario 10: Mixed/comprehensive comparison using parsed JSON
            if (!json_input.empty()) {
                parsed1 = cJSON_Parse(json_input.c_str());
                parsed2 = cJSON_Parse(json_input.c_str());
                
                if (parsed1 && parsed2) {
                    // Test identical parsed objects
                    cJSON_Compare(parsed1, parsed2, case_sensitive);
                    
                    // Modify one parsed object slightly
                    cJSON *modified = cJSON_Duplicate(parsed1, 1);
                    if (modified) {
                        cJSON_AddNumberToObject(modified, "fuzzed", fdp.ConsumeIntegral<int>());
                        cJSON_Compare(parsed1, modified, case_sensitive);
                        cJSON_Delete(modified);
                    }
                }
                
                // Also test parsing with options
                cJSON *parsed_with_opts = cJSON_ParseWithOpts(json_input.c_str(), NULL, 1);
                if (parsed_with_opts && parsed1) {
                    cJSON_Compare(parsed1, parsed_with_opts, case_sensitive);
                }
                if (parsed_with_opts) cJSON_Delete(parsed_with_opts);
            }
            
            if (parsed1) cJSON_Delete(parsed1);
            if (parsed2) cJSON_Delete(parsed2);
            break;
        }
    }
    
    return 0;
}

} // namespace harness_019
