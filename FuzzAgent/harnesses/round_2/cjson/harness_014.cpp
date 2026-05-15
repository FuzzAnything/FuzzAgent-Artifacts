#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_014 {

extern "C" {
}

// Helper to create JSON objects with specific characteristics for comparison testing
cJSON* create_comparison_object(FuzzedDataProvider& fdp, int depth, bool create_asymmetric) {
    if (depth <= 0 || fdp.remaining_bytes() < 16) {
        // Create leaf node
        uint8_t type = fdp.ConsumeIntegral<uint8_t>() % 8;
        switch (type) {
            case 0: return cJSON_CreateFalse();
            case 1: return cJSON_CreateTrue();
            case 2: return cJSON_CreateNull();
            case 3: return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            case 4: return cJSON_CreateString(fdp.ConsumeRandomLengthString(64).c_str());
            case 5: return cJSON_CreateRaw(fdp.ConsumeRandomLengthString(64).c_str());
            case 6: return cJSON_CreateArray();
            case 7: return cJSON_CreateObject();
            default: return NULL;
        }
    }
    
    uint8_t structure_type = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    if (structure_type == 0) {
        // Create array
        cJSON* arr = cJSON_CreateArray();
        if (arr == NULL) return NULL;
        
        int elem_count = fdp.ConsumeIntegralInRange<int>(1, 5);
        for (int i = 0; i < elem_count && fdp.remaining_bytes() > 0; i++) {
            // For asymmetric testing, sometimes skip adding elements
            if (create_asymmetric && (fdp.ConsumeIntegral<uint8_t>() % 4 == 0)) {
                continue;
            }
            cJSON* elem = create_comparison_object(fdp, depth - 1, create_asymmetric);
            if (elem != NULL) {
                cJSON_AddItemToArray(arr, elem);
            }
        }
        return arr;
    } else {
        // Create object
        cJSON* obj = cJSON_CreateObject();
        if (obj == NULL) return NULL;
        
        int prop_count = fdp.ConsumeIntegralInRange<int>(1, 8);
        for (int i = 0; i < prop_count && fdp.remaining_bytes() > 0; i++) {
            std::string key = fdp.ConsumeRandomLengthString(16);
            
            // For asymmetric testing, sometimes use different keys or skip properties
            if (create_asymmetric) {
                uint8_t asym_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                if (asym_type == 0) {
                    // Skip this property (asymmetric object will miss this key)
                    continue;
                } else if (asym_type == 1) {
                    // Use slightly different key (case variation for case-sensitive testing)
                    key = key + "_alt";
                }
            }
            
            cJSON* value = create_comparison_object(fdp, depth - 1, create_asymmetric);
            if (value != NULL) {
                cJSON_AddItemToObject(obj, key.c_str(), value);
            }
        }
        return obj;
    }
}

// Helper to create objects with NULL valuestring (edge case for string/raw types)
cJSON* create_string_with_null_valuestring(FuzzedDataProvider& fdp) {
    cJSON* item = (cJSON*)cJSON_malloc(sizeof(cJSON));
    if (item == NULL) return NULL;
    
    memset(item, 0, sizeof(cJSON));
    item->type = fdp.ConsumeBool() ? cJSON_String : cJSON_Raw;
    item->valuestring = NULL;  // NULL valuestring - critical edge case for lines 3123-3126
    item->string = NULL;
    item->next = item->prev = item->child = NULL;
    item->valueint = 0;
    item->valuedouble = 0.0;
    
    return item;
}

// Helper to create JSON objects with invalid types (to trigger default case at line 3197)
cJSON* create_invalid_type_object(FuzzedDataProvider& fdp) {
    cJSON* item = (cJSON*)cJSON_malloc(sizeof(cJSON));
    if (item == NULL) return NULL;
    
    memset(item, 0, sizeof(cJSON));
    // Create invalid type - use values not in the valid type set
    // Valid types are: 1, 2, 4, 8, 16, 32, 64, 128 (powers of 2 up to 128)
    // Use values like 0, 3, 5, 6, 7, 9, etc. or values with high bits
    uint8_t invalid_type = fdp.ConsumeIntegral<uint8_t>();
    // Ensure it's not a valid type
    while (invalid_type == 1 || invalid_type == 2 || invalid_type == 4 || invalid_type == 8 || 
           invalid_type == 16 || invalid_type == 32 || invalid_type == 64 || invalid_type == 128) {
        invalid_type = fdp.ConsumeIntegral<uint8_t>();
    }
    
    item->type = invalid_type;
    item->valuestring = NULL;
    item->string = NULL;
    item->next = item->prev = item->child = NULL;
    item->valueint = 0;
    item->valuedouble = 0.0;
    
    return item;
}

// Helper to test specific comparison scenarios
void test_comparison_scenario(FuzzedDataProvider& fdp, cJSON* a, cJSON* b, const char* scenario_name) {
    if (a == NULL || b == NULL) return;
    
    // Test both case-sensitive and case-insensitive
    bool case_sensitive = fdp.ConsumeBool();
    cJSON_bool result = cJSON_Compare(a, b, case_sensitive);
    
    // Also test with swapped parameters
    cJSON_bool swapped_result = cJSON_Compare(b, a, case_sensitive);
    
    // For identical objects, results should match
    if (a == b) {
        // Should return true
    }
}
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 128) {
        return 0;  // Need sufficient input for comprehensive comparison testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume test scenario selector
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 13;
    
    // Test Scenario 0: Identical objects (targeting line 3101-3104)
    if (test_scenario == 0) {
        cJSON* obj = NULL;
        uint8_t obj_type = fdp.ConsumeIntegral<uint8_t>() % 8;
        
        switch (obj_type) {
            case 0: obj = cJSON_CreateFalse(); break;
            case 1: obj = cJSON_CreateTrue(); break;
            case 2: obj = cJSON_CreateNull(); break;
            case 3: obj = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()); break;
            case 4: obj = cJSON_CreateString(fdp.ConsumeRandomLengthString(64).c_str()); break;
            case 5: obj = cJSON_CreateRaw(fdp.ConsumeRandomLengthString(64).c_str()); break;
            case 6: obj = create_comparison_object(fdp, 3, false); break; // Array
            case 7: obj = create_comparison_object(fdp, 3, false); break; // Object
        }
        
        if (obj != NULL) {
            bool case_sensitive = fdp.ConsumeBool();
            // Compare object with itself (should return true)
            cJSON_Compare(obj, obj, case_sensitive);
            cJSON_Delete(obj);
        }
        
        return 0;
    }
    
    // Test Scenario 1: NULL valuestring handling (targeting lines 3123-3126)
    if (test_scenario == 1) {
        cJSON* null_string1 = create_string_with_null_valuestring(fdp);
        cJSON* null_string2 = create_string_with_null_valuestring(fdp);
        cJSON* valid_string = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
        
        bool case_sensitive = fdp.ConsumeBool();
        
        // NULL valuestring vs NULL valuestring
        if (null_string1 != NULL && null_string2 != NULL) {
            cJSON_Compare(null_string1, null_string2, case_sensitive);
        }
        
        // NULL valuestring vs valid string
        if (null_string1 != NULL && valid_string != NULL) {
            cJSON_Compare(null_string1, valid_string, case_sensitive);
            cJSON_Compare(valid_string, null_string1, case_sensitive);
        }
        
        // Cleanup
        if (null_string1 != NULL) cJSON_free(null_string1);
        if (null_string2 != NULL) cJSON_free(null_string2);
        if (valid_string != NULL) cJSON_Delete(valid_string);
        
        return 0;
    }
    
    // Test Scenario 2: Arrays of different lengths (targeting lines 3151-3153)
    if (test_scenario == 2) {
        cJSON* arr1 = cJSON_CreateArray();
        cJSON* arr2 = cJSON_CreateArray();
        
        if (arr1 != NULL && arr2 != NULL) {
            // Add different number of elements to each array
            int count1 = fdp.ConsumeIntegralInRange<int>(1, 10);
            int count2 = count1 + fdp.ConsumeIntegralInRange<int>(1, 5); // Ensure different lengths
            
            for (int i = 0; i < count1 && fdp.remaining_bytes() > 0; i++) {
                cJSON_AddItemToArray(arr1, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
            }
            
            for (int i = 0; i < count2 && fdp.remaining_bytes() > 0; i++) {
                cJSON_AddItemToArray(arr2, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
            }
            
            bool case_sensitive = fdp.ConsumeBool();
            cJSON_Compare(arr1, arr2, case_sensitive);
        }
        
        if (arr1 != NULL) cJSON_Delete(arr1);
        if (arr2 != NULL) cJSON_Delete(arr2);
        
        return 0;
    }
    
    // Test Scenario 3: Objects with same keys but different values
    if (test_scenario == 3) {
        cJSON* obj1 = cJSON_CreateObject();
        cJSON* obj2 = cJSON_CreateObject();
        
        if (obj1 != NULL && obj2 != NULL) {
            int prop_count = fdp.ConsumeIntegralInRange<int>(2, 8);
            
            for (int i = 0; i < prop_count && fdp.remaining_bytes() > 0; i++) {
                std::string key = fdp.ConsumeRandomLengthString(16);
                
                // Add same key with different values to each object
                cJSON_AddNumberToObject(obj1, key.c_str(), fdp.ConsumeFloatingPoint<double>());
                cJSON_AddNumberToObject(obj2, key.c_str(), fdp.ConsumeFloatingPoint<double>()); // Different value
            }
            
            bool case_sensitive = fdp.ConsumeBool();
            cJSON_Compare(obj1, obj2, case_sensitive);
        }
        
        if (obj1 != NULL) cJSON_Delete(obj1);
        if (obj2 != NULL) cJSON_Delete(obj2);
        
        return 0;
    }
    
    // Test Scenario 4: Objects with different keys (targeting lines 3166-3169, 3182-3185)
    if (test_scenario == 4) {
        cJSON* obj1 = cJSON_CreateObject();
        cJSON* obj2 = cJSON_CreateObject();
        
        if (obj1 != NULL && obj2 != NULL) {
            // Add different keys to each object
            int prop_count = fdp.ConsumeIntegralInRange<int>(2, 6);
            
            for (int i = 0; i < prop_count && fdp.remaining_bytes() > 0; i++) {
                std::string key1 = fdp.ConsumeRandomLengthString(16);
                std::string key2 = key1 + "_different"; // Ensure different keys
                
                cJSON_AddNumberToObject(obj1, key1.c_str(), fdp.ConsumeFloatingPoint<double>());
                cJSON_AddNumberToObject(obj2, key2.c_str(), fdp.ConsumeFloatingPoint<double>());
            }
            
            bool case_sensitive = fdp.ConsumeBool();
            cJSON_Compare(obj1, obj2, case_sensitive);
        }
        
        if (obj1 != NULL) cJSON_Delete(obj1);
        if (obj2 != NULL) cJSON_Delete(obj2);
        
        return 0;
    }
    
    // Test Scenario 5: Case-sensitive vs case-insensitive string comparisons
    if (test_scenario == 5) {
        std::string base_str = fdp.ConsumeRandomLengthString(32);
        std::string same_str = base_str;
        std::string different_case_str = base_str;
        
        // Modify case of some characters
        if (!different_case_str.empty()) {
            size_t pos = fdp.ConsumeIntegralInRange<size_t>(0, different_case_str.length() - 1);
            if (different_case_str[pos] >= 'a' && different_case_str[pos] <= 'z') {
                different_case_str[pos] = different_case_str[pos] - 'a' + 'A';
            } else if (different_case_str[pos] >= 'A' && different_case_str[pos] <= 'Z') {
                different_case_str[pos] = different_case_str[pos] - 'A' + 'a';
            }
        }
        
        cJSON* str1 = cJSON_CreateString(base_str.c_str());
        cJSON* str2 = cJSON_CreateString(same_str.c_str());
        cJSON* str3 = cJSON_CreateString(different_case_str.c_str());
        
        if (str1 != NULL && str2 != NULL && str3 != NULL) {
            // Case-sensitive: same strings should match, different case should not
            cJSON_Compare(str1, str2, true);   // case-sensitive, should match
            cJSON_Compare(str1, str3, true);   // case-sensitive, should NOT match
            
            // Case-insensitive: both should match (if strcmp is used, they won't match)
            // Note: cJSON_Compare uses strcmp for strings, not case-insensitive compare
            cJSON_Compare(str1, str2, false);  // case-insensitive flag doesn't affect string comparison
            cJSON_Compare(str1, str3, false);  // case-insensitive flag doesn't affect string comparison
        }
        
        if (str1 != NULL) cJSON_Delete(str1);
        if (str2 != NULL) cJSON_Delete(str2);
        if (str3 != NULL) cJSON_Delete(str3);
        
        return 0;
    }
    
    // Test Scenario 6: Deeply nested structures (5+ levels)
    if (test_scenario == 6) {
        cJSON* deep1 = create_comparison_object(fdp, 6, false); // 6 levels deep
        cJSON* deep2 = create_comparison_object(fdp, 6, false);
        
        if (deep1 != NULL && deep2 != NULL) {
            bool case_sensitive = fdp.ConsumeBool();
            cJSON_Compare(deep1, deep2, case_sensitive);
            
            // Also compare with itself
            cJSON_Compare(deep1, deep1, case_sensitive);
        }
        
        if (deep1 != NULL) cJSON_Delete(deep1);
        if (deep2 != NULL) cJSON_Delete(deep2);
        
        return 0;
    }
    
    // Test Scenario 7: Mixed type comparisons
    if (test_scenario == 7) {
        // Create two items of different types
        uint8_t type1 = fdp.ConsumeIntegral<uint8_t>() % 8;
        uint8_t type2 = (type1 + 1 + (fdp.ConsumeIntegral<uint8_t>() % 7)) % 8;
        
        cJSON* item1 = NULL;
        cJSON* item2 = NULL;
        
        auto create_item = [&](uint8_t type) -> cJSON* {
            switch (type) {
                case 0: return cJSON_CreateFalse();
                case 1: return cJSON_CreateTrue();
                case 2: return cJSON_CreateNull();
                case 3: return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                case 4: return cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
                case 5: return cJSON_CreateRaw(fdp.ConsumeRandomLengthString(32).c_str());
                case 6: return cJSON_CreateArray();
                case 7: return cJSON_CreateObject();
                default: return NULL;
            }
        };
        
        item1 = create_item(type1);
        item2 = create_item(type2);
        
        if (item1 != NULL && item2 != NULL) {
            bool case_sensitive = fdp.ConsumeBool();
            cJSON_Compare(item1, item2, case_sensitive);
        }
        
        if (item1 != NULL) cJSON_Delete(item1);
        if (item2 != NULL) cJSON_Delete(item2);
        
        return 0;
    }
    
    // Test Scenario 8: Asymmetric object structures (different nesting patterns)
    if (test_scenario == 8) {
        cJSON* sym_obj = create_comparison_object(fdp, 4, false); // Symmetric
        cJSON* asym_obj = create_comparison_object(fdp, 4, true);  // Asymmetric
        
        if (sym_obj != NULL && asym_obj != NULL) {
            bool case_sensitive = fdp.ConsumeBool();
            cJSON_Compare(sym_obj, asym_obj, case_sensitive);
        }
        
        if (sym_obj != NULL) cJSON_Delete(sym_obj);
        if (asym_obj != NULL) cJSON_Delete(asym_obj);
        
        return 0;
    }
    
    // Test Scenario 9: Number comparisons with floating point edge cases
    if (test_scenario == 9) {
        // Test various double comparisons
        double val1 = fdp.ConsumeFloatingPoint<double>();
        double val2 = fdp.ConsumeFloatingPoint<double>();
        
        // Create numbers that are very close (within epsilon)
        cJSON* num1 = cJSON_CreateNumber(val1);
        cJSON* num2 = cJSON_CreateNumber(val2);
        cJSON* num3 = cJSON_CreateNumber(val1); // Same as num1
        
        if (num1 != NULL && num2 != NULL && num3 != NULL) {
            bool case_sensitive = fdp.ConsumeBool();
            cJSON_Compare(num1, num2, case_sensitive); // Different values
            cJSON_Compare(num1, num3, case_sensitive); // Same values
        }
        
        if (num1 != NULL) cJSON_Delete(num1);
        if (num2 != NULL) cJSON_Delete(num2);
        if (num3 != NULL) cJSON_Delete(num3);
        
        return 0;
    }
    
    // Test Scenario 10: NULL pointer inputs (edge cases)
    if (test_scenario == 10) {
        cJSON* valid_obj = create_comparison_object(fdp, 2, false);
        
        if (valid_obj != NULL) {
            bool case_sensitive = fdp.ConsumeBool();
            
            // NULL vs NULL
            cJSON_Compare(NULL, NULL, case_sensitive);
            
            // NULL vs valid object
            cJSON_Compare(NULL, valid_obj, case_sensitive);
            cJSON_Compare(valid_obj, NULL, case_sensitive);
            
            cJSON_Delete(valid_obj);
        }
        
        return 0;
    }
    
    // Test Scenario 11: Complex combination - parse JSON and compare
    if (test_scenario == 11) {
        // Parse JSON from input
        std::string json_str1 = fdp.ConsumeRandomLengthString(256);
        std::string json_str2 = fdp.ConsumeRandomLengthString(256);
        
        cJSON* parsed1 = cJSON_Parse(json_str1.c_str());
        cJSON* parsed2 = cJSON_Parse(json_str2.c_str());
        
        // Also create programmatic objects
        cJSON* created1 = create_comparison_object(fdp, 3, false);
        cJSON* created2 = create_comparison_object(fdp, 3, true); // Asymmetric
        
        bool case_sensitive = fdp.ConsumeBool();
        
        // Compare various combinations
        if (parsed1 != NULL && parsed2 != NULL) {
            cJSON_Compare(parsed1, parsed2, case_sensitive);
            cJSON_Compare(parsed1, parsed1, case_sensitive); // Self-comparison
        }
        
        if (created1 != NULL && created2 != NULL) {
            cJSON_Compare(created1, created2, case_sensitive);
        }
        
        // Mixed: parsed vs created
        if (parsed1 != NULL && created1 != NULL) {
            cJSON_Compare(parsed1, created1, case_sensitive);
        }
        
        // Cleanup
        if (parsed1 != NULL) cJSON_Delete(parsed1);
        if (parsed2 != NULL) cJSON_Delete(parsed2);
        if (created1 != NULL) cJSON_Delete(created1);
        if (created2 != NULL) cJSON_Delete(created2);
        
        return 0;
    }
    
    // Test Scenario 12: Invalid type comparisons (to trigger default case at line 3197)
    if (test_scenario == 12) {
        cJSON* invalid_obj1 = create_invalid_type_object(fdp);
        cJSON* invalid_obj2 = create_invalid_type_object(fdp);
        
        // Create some valid objects for comparison
        cJSON* valid_obj1 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        cJSON* valid_obj2 = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
        
        bool case_sensitive = fdp.ConsumeBool();
        
        // Test invalid type comparisons
        // Invalid vs invalid (same type check - will fail type mismatch check at line 3078)
        if (invalid_obj1 != NULL) {
            cJSON_Compare(invalid_obj1, invalid_obj1, case_sensitive);
        }
        
        // Invalid vs invalid (different types)
        if (invalid_obj1 != NULL && invalid_obj2 != NULL) {
            cJSON_Compare(invalid_obj1, invalid_obj2, case_sensitive);
        }
        
        // Invalid vs valid (should fail type check at line 3078 or reach default case at line 3197)
        if (invalid_obj1 != NULL && valid_obj1 != NULL) {
            cJSON_Compare(invalid_obj1, valid_obj1, case_sensitive);
        }
        
        // Valid vs invalid  
        if (valid_obj2 != NULL && invalid_obj2 != NULL) {
            cJSON_Compare(valid_obj2, invalid_obj2, case_sensitive);
        }
        
        // Cleanup
        if (invalid_obj1 != NULL) cJSON_free(invalid_obj1);
        if (invalid_obj2 != NULL) cJSON_free(invalid_obj2);
        if (valid_obj1 != NULL) cJSON_Delete(valid_obj1);
        if (valid_obj2 != NULL) cJSON_Delete(valid_obj2);
        
        return 0;
    }
    
    return 0;
}

} // namespace harness_014
