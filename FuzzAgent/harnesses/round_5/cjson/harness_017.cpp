#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <cstdio>
#include "cjson/cJSON.h"

namespace harness_017 {
/*
 * Harness_017: Deep comparison testing of complex JSON structures for cJSON_Compare
 * 
 * Target: cJSON_Compare function with 73/76 branches blocked
 * Focus: Deep comparison testing targeting specific blocked scenarios:
 *   1) Objects where one is a subset of the other
 *   2) Arrays with same length but different elements  
 *   3) Mixed type comparisons (string vs raw with same content)
 *   4) Deep nesting (3+ levels) with variations at leaf nodes
 *   5) Unicode string comparisons with case sensitivity
 * 
 * SEMANTIC DIFFERENTIATION FROM PREVIOUS HARNESSES (000-016):
 * - harness_014: Uses cJSON_Duplicate with systematic modifications
 * - harness_015: Direct internal structure manipulation
 * - harness_016: JSON string generation and parsing
 * - This harness_017: Combines programmatic creation AND parsing with focus on:
 *   a) Deep nested structures (3+ levels) with controlled variations
 *   b) Explicit subset/superset object relationships
 *   c) Unicode/case-sensitive string comparison testing
 *   d) Mixed type comparisons (string vs raw)
 *   e) Array element variations at same length
 * 
 * Novel approach benefits:
 * 1. Explicitly creates object subset relationships (missing key detection paths)
 * 2. Tests array length equality but element inequality paths
 * 3. Tests Unicode string handling with case sensitivity
 * 4. Tests deep recursion paths with variations at leaf nodes
 * 5. Tests mixed type comparisons that should return false
 * 6. Combines parsing and programmatic creation for maximum diversity
 * 
 * Implementation strategy:
 * 1. Create deep nested structures (3+ levels) programmatically
 * 2. Create JSON strings representing similar but different structures
 * 3. Parse JSON strings to create comparison objects
 * 4. Test specific blocked conditions:
 *    - Object subset checking (lines 3166, 3182 - missing key detection)
 *    - Array element comparison failures (lines 3141)
 *    - String vs Raw type mismatch (lines 3121-3132)
 *    - Unicode string comparisons with case sensitivity
 *    - Deep nested comparison failures
 */



// Helper to create deep nested object (3+ levels)
static cJSON* create_deep_nested_object(FuzzedDataProvider* fdp, int depth = 0) {
    if (depth > 4) {  // Max depth 4 for testing
        return cJSON_CreateNull();
    }
    
    cJSON* obj = cJSON_CreateObject();
    if (obj == NULL) {
        return NULL;
    }
    
    // Add various types at each level
    std::string key_prefix = "level" + std::to_string(depth) + "_";
    
    // Add a number
    cJSON_AddNumberToObject(obj, (key_prefix + "num").c_str(), 
                           fdp->ConsumeFloatingPoint<double>());
    
    // Add a string (potentially with Unicode)
    std::string str_val = fdp->ConsumeRandomLengthString(32);
    cJSON_AddStringToObject(obj, (key_prefix + "str").c_str(), str_val.c_str());
    
    // Add a boolean
    cJSON_AddBoolToObject(obj, (key_prefix + "bool").c_str(), fdp->ConsumeBool());
    
    // Add nested object at certain depths
    if (depth < 3) {
        cJSON* nested = create_deep_nested_object(fdp, depth + 1);
        if (nested != NULL) {
            cJSON_AddItemToObject(obj, (key_prefix + "nested").c_str(), nested);
        }
    }
    
    // Add array at certain depths
    if (depth < 2) {
        cJSON* array = cJSON_CreateArray();
        if (array != NULL) {
            size_t array_size = fdp->ConsumeIntegralInRange<size_t>(1, 4);
            for (size_t i = 0; i < array_size; i++) {
                uint8_t elem_type = fdp->ConsumeIntegral<uint8_t>() % 4;
                switch (elem_type) {
                    case 0:
                        cJSON_AddItemToArray(array, cJSON_CreateNumber(fdp->ConsumeFloatingPoint<double>()));
                        break;
                    case 1:
                        cJSON_AddItemToArray(array, cJSON_CreateString(fdp->ConsumeRandomLengthString(16).c_str()));
                        break;
                    case 2:
                        cJSON_AddItemToArray(array, cJSON_CreateBool(fdp->ConsumeBool()));
                        break;
                    case 3:
                        cJSON_AddItemToArray(array, cJSON_CreateNull());
                        break;
                }
            }
            cJSON_AddItemToObject(obj, (key_prefix + "arr").c_str(), array);
        }
    }
    
    return obj;
}

// Create object with specific key set (for subset testing)
static cJSON* create_object_with_key_set(FuzzedDataProvider* fdp, 
                                        const std::vector<std::string>& keys,
                                        bool include_all = true) {
    cJSON* obj = cJSON_CreateObject();
    if (obj == NULL) {
        return NULL;
    }
    
    for (size_t i = 0; i < keys.size(); i++) {
        // For subset testing, skip some keys
        if (!include_all && (fdp->ConsumeBool())) {
            continue;
        }
        
        std::string key = keys[i];
        uint8_t value_type = fdp->ConsumeIntegral<uint8_t>() % 5;
        
        switch (value_type) {
            case 0:
                cJSON_AddNumberToObject(obj, key.c_str(), fdp->ConsumeFloatingPoint<double>());
                break;
            case 1:
                cJSON_AddStringToObject(obj, key.c_str(), fdp->ConsumeRandomLengthString(24).c_str());
                break;
            case 2:
                cJSON_AddBoolToObject(obj, key.c_str(), fdp->ConsumeBool());
                break;
            case 3:
                cJSON_AddNullToObject(obj, key.c_str());
                break;
            case 4: {
                // Add a simple array
                cJSON* arr = cJSON_CreateArray();
                if (arr != NULL) {
                    size_t arr_size = fdp->ConsumeIntegralInRange<size_t>(1, 3);
                    for (size_t j = 0; j < arr_size; j++) {
                        cJSON_AddItemToArray(arr, cJSON_CreateNumber(fdp->ConsumeFloatingPoint<double>()));
                    }
                    cJSON_AddItemToObject(obj, key.c_str(), arr);
                }
                break;
            }
        }
    }
    
    return obj;
}

// Create array with same length but different elements
static cJSON* create_array_with_varied_elements(FuzzedDataProvider* fdp, size_t length, 
                                               bool make_different = false) {
    cJSON* array = cJSON_CreateArray();
    if (array == NULL) {
        return NULL;
    }
    
    for (size_t i = 0; i < length; i++) {
        if (make_different && i == length / 2) {
            // Make middle element different type
            cJSON_AddItemToArray(array, cJSON_CreateString("DIFFERENT"));
        } else {
            uint8_t elem_type = fdp->ConsumeIntegral<uint8_t>() % 4;
            switch (elem_type) {
                case 0:
                    cJSON_AddItemToArray(array, cJSON_CreateNumber(fdp->ConsumeFloatingPoint<double>()));
                    break;
                case 1:
                    cJSON_AddItemToArray(array, cJSON_CreateString(fdp->ConsumeRandomLengthString(16).c_str()));
                    break;
                case 2:
                    cJSON_AddItemToArray(array, cJSON_CreateBool(fdp->ConsumeBool()));
                    break;
                case 3:
                    cJSON_AddItemToArray(array, cJSON_CreateNull());
                    break;
            }
        }
    }
    
    return array;
}

// Create string vs raw comparison test
static void test_mixed_type_comparison(FuzzedDataProvider* fdp) {
    // Test 1: String vs Raw with same content (should return false due to different types)
    std::string content = fdp->ConsumeRandomLengthString(32);
    
    cJSON* str_obj = cJSON_CreateString(content.c_str());
    cJSON* raw_obj = cJSON_CreateRaw(content.c_str());
    
    if (str_obj != NULL && raw_obj != NULL) {
        // These should compare false because types are different (string vs raw)
        bool str_vs_raw = cJSON_Compare(str_obj, raw_obj, true);
        bool raw_vs_str = cJSON_Compare(raw_obj, str_obj, true);
        
        // Also test case-insensitive
        bool str_vs_raw_ci = cJSON_Compare(str_obj, raw_obj, false);
        bool raw_vs_str_ci = cJSON_Compare(raw_obj, str_obj, false);
    }
    
    if (str_obj != NULL) cJSON_Delete(str_obj);
    if (raw_obj != NULL) cJSON_Delete(raw_obj);
}

// Test Unicode string comparisons with case sensitivity
static void test_unicode_case_comparisons(FuzzedDataProvider* fdp) {
    // Create strings with potential Unicode characters
    std::string str1 = fdp->ConsumeRandomLengthString(24);
    std::string str2 = str1;  // Start with same content
    
    // Modify case for some characters (ASCII range only for simplicity)
    if (!str2.empty()) {
        size_t pos = fdp->ConsumeIntegralInRange<size_t>(0, str2.length() - 1);
        if (str2[pos] >= 'a' && str2[pos] <= 'z') {
            str2[pos] = str2[pos] - 32;  // Convert to uppercase
        } else if (str2[pos] >= 'A' && str2[pos] <= 'Z') {
            str2[pos] = str2[pos] + 32;  // Convert to lowercase
        }
    }
    
    cJSON* json1 = cJSON_CreateString(str1.c_str());
    cJSON* json2 = cJSON_CreateString(str2.c_str());
    
    if (json1 != NULL && json2 != NULL) {
        // Test case-sensitive comparison (should be false if case differs)
        bool case_sensitive_result = cJSON_Compare(json1, json2, true);
        
        // Test case-insensitive comparison (should be true for ASCII case differences)
        // Note: cJSON uses strcmp for case-sensitive, so case-insensitive won't match
        bool case_insensitive_result = cJSON_Compare(json1, json2, false);
    }
    
    if (json1 != NULL) cJSON_Delete(json1);
    if (json2 != NULL) cJSON_Delete(json2);
}

// Parse JSON string and compare with programmatically created structure
static void test_parsing_vs_programmatic(FuzzedDataProvider* fdp) {
    // Create a JSON string manually
    std::string json_str = "{";
    json_str += "\"number\":" + std::to_string(fdp->ConsumeFloatingPoint<double>()) + ",";
    json_str += "\"string\":\"" + fdp->ConsumeRandomLengthString(16) + "\",";
    json_str += "\"bool\":" + std::string(fdp->ConsumeBool() ? "true" : "false") + ",";
    json_str += "\"array\":[1,2,3]";
    json_str += "}";
    
    // Parse the JSON string
    cJSON* parsed = cJSON_Parse(json_str.c_str());
    
    // Create similar structure programmatically
    cJSON* programmatic = cJSON_CreateObject();
    if (programmatic != NULL && parsed != NULL) {
        // Note: We can't easily recreate the exact same values from consumed data
        // So we'll just test that we can compare them (likely false due to different values)
        bool comparison = cJSON_Compare(parsed, programmatic, true);
    }
    
    if (parsed != NULL) cJSON_Delete(parsed);
    if (programmatic != NULL) cJSON_Delete(programmatic);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for comprehensive testing
    if (size < 512) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test 1: Deep nested structures with variations
    cJSON* deep1 = create_deep_nested_object(&fdp, 0);
    cJSON* deep2 = create_deep_nested_object(&fdp, 0);
    
    if (deep1 != NULL && deep2 != NULL) {
        // Compare identical deep structures (might be true or false depending on randomness)
        bool deep_compare1 = cJSON_Compare(deep1, deep2, true);
        bool deep_compare2 = cJSON_Compare(deep1, deep2, false);
    }
    
    // Test 2: Object subset relationships
    std::vector<std::string> common_keys;
    for (int i = 0; i < 5; i++) {
        common_keys.push_back("key" + std::to_string(i));
    }
    
    // Create superset object (has all keys)
    cJSON* superset = create_object_with_key_set(&fdp, common_keys, true);
    
    // Create subset object (missing some keys)
    cJSON* subset = create_object_with_key_set(&fdp, common_keys, false);
    
    if (superset != NULL && subset != NULL) {
        // These should compare false (subset vs superset)
        bool subset_vs_superset = cJSON_Compare(subset, superset, true);
        bool superset_vs_subset = cJSON_Compare(superset, subset, true);
    }
    
    // Test 3: Arrays with same length but different elements
    size_t array_length = fdp.ConsumeIntegralInRange<size_t>(3, 8);
    cJSON* array1 = create_array_with_varied_elements(&fdp, array_length, false);
    cJSON* array2 = create_array_with_varied_elements(&fdp, array_length, true); // Different middle element
    
    if (array1 != NULL && array2 != NULL) {
        bool same_length_diff_elements = cJSON_Compare(array1, array2, true);
    }
    
    // Test 4: Mixed type comparisons (string vs raw)
    test_mixed_type_comparison(&fdp);
    
    // Test 5: Unicode and case sensitivity
    test_unicode_case_comparisons(&fdp);
    
    // Test 6: Parsing vs programmatic creation
    test_parsing_vs_programmatic(&fdp);
    
    // Test 7: Additional edge cases
    
    // Test with NULL objects
    if (deep1 != NULL) {
        bool null_vs_obj = cJSON_Compare(NULL, deep1, true);
        bool obj_vs_null = cJSON_Compare(deep1, NULL, true);
    }
    
    // Test identical object comparison (line 3101)
    if (deep1 != NULL) {
        bool identical = cJSON_Compare(deep1, deep1, true);
    }
    
    // Test simple type comparisons
    cJSON* true1 = cJSON_CreateTrue();
    cJSON* true2 = cJSON_CreateTrue();
    cJSON* false1 = cJSON_CreateFalse();
    cJSON* null1 = cJSON_CreateNull();
    
    if (true1 != NULL && true2 != NULL) {
        bool true_vs_true = cJSON_Compare(true1, true2, true);
    }
    
    if (true1 != NULL && false1 != NULL) {
        bool true_vs_false = cJSON_Compare(true1, false1, true);
    }
    
    if (null1 != NULL && true1 != NULL) {
        bool null_vs_true = cJSON_Compare(null1, true1, true);
    }
    
    // Cleanup
    if (deep1 != NULL) cJSON_Delete(deep1);
    if (deep2 != NULL) cJSON_Delete(deep2);
    if (superset != NULL) cJSON_Delete(superset);
    if (subset != NULL) cJSON_Delete(subset);
    if (array1 != NULL) cJSON_Delete(array1);
    if (array2 != NULL) cJSON_Delete(array2);
    if (true1 != NULL) cJSON_Delete(true1);
    if (true2 != NULL) cJSON_Delete(true2);
    if (false1 != NULL) cJSON_Delete(false1);
    if (null1 != NULL) cJSON_Delete(null1);
    
    return 0;
}

} // namespace harness_017
