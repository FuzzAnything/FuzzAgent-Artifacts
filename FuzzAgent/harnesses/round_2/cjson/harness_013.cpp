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

namespace harness_013 {

extern "C" {
}

// Helper to create all 8 JSON types using fuzzer input
cJSON* create_json_type(FuzzedDataProvider& fdp, uint8_t type, bool use_valid_data = true) {
    switch (type) {
        case 0: // cJSON_False
            return cJSON_CreateFalse();
        case 1: // cJSON_True
            return cJSON_CreateTrue();
        case 2: // cJSON_NULL
            return cJSON_CreateNull();
        case 3: // cJSON_Number
            return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        case 4: // cJSON_String
            if (use_valid_data) {
                return cJSON_CreateString(fdp.ConsumeRandomLengthString(128).c_str());
            } else {
                // When use_valid_data is false, still create valid string but with empty or short content
                return cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
            }
        case 5: // cJSON_Array
            {
                cJSON* arr = cJSON_CreateArray();
                if (arr != NULL) {
                    int array_size = fdp.ConsumeIntegralInRange<int>(0, 5);
                    for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                        uint8_t elem_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                        cJSON* elem = create_json_type(fdp, elem_type, use_valid_data);
                        if (elem != NULL) {
                            cJSON_AddItemToArray(arr, elem);
                        }
                    }
                }
                return arr;
            }
        case 6: // cJSON_Object
            {
                cJSON* obj = cJSON_CreateObject();
                if (obj != NULL) {
                    int obj_size = fdp.ConsumeIntegralInRange<int>(0, 5);
                    for (int i = 0; i < obj_size && fdp.remaining_bytes() > 0; i++) {
                        std::string key = fdp.ConsumeRandomLengthString(16);
                        uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                        cJSON* value = create_json_type(fdp, value_type, use_valid_data);
                        if (value != NULL) {
                            cJSON_AddItemToObject(obj, key.c_str(), value);
                        }
                    }
                }
                return obj;
            }
        case 7: // cJSON_Raw
            if (use_valid_data) {
                return cJSON_CreateRaw(fdp.ConsumeRandomLengthString(128).c_str());
            } else {
                // When use_valid_data is false, still create valid raw JSON but with empty or short content
                return cJSON_CreateRaw(fdp.ConsumeRandomLengthString(32).c_str());
            }
        default:
            return NULL;
    }
}

// Helper to create deep nested structures
cJSON* create_deep_nested(FuzzedDataProvider& fdp, int depth, bool is_array) {
    if (depth <= 0) {
        // Create leaf node
        uint8_t leaf_type = fdp.ConsumeIntegral<uint8_t>() % 8;
        return create_json_type(fdp, leaf_type);
    }
    
    if (is_array) {
        cJSON* arr = cJSON_CreateArray();
        if (arr != NULL) {
            int elem_count = fdp.ConsumeIntegralInRange<int>(1, 3);
            for (int i = 0; i < elem_count && fdp.remaining_bytes() > 0; i++) {
                cJSON* elem = create_deep_nested(fdp, depth - 1, !is_array);
                if (elem != NULL) {
                    cJSON_AddItemToArray(arr, elem);
                }
            }
        }
        return arr;
    } else {
        cJSON* obj = cJSON_CreateObject();
        if (obj != NULL) {
            int prop_count = fdp.ConsumeIntegralInRange<int>(1, 3);
            for (int i = 0; i < prop_count && fdp.remaining_bytes() > 0; i++) {
                std::string key = "key" + std::to_string(i);
                cJSON* value = create_deep_nested(fdp, depth - 1, !is_array);
                if (value != NULL) {
                    cJSON_AddItemToObject(obj, key.c_str(), value);
                }
            }
        }
        return obj;
    }
}

// Helper to modify JSON structure slightly for partial equality testing
void modify_json_structure(cJSON* item, FuzzedDataProvider& fdp) {
    if (item == NULL) return;
    
    uint8_t modify_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (item->type & 0xFF) {
        case cJSON_Number:
            // Change number value slightly using proper API call with range checking
            {
                double new_valuedouble = item->valuedouble + fdp.ConsumeFloatingPoint<double>() * 0.1;
                
                // Range checking for double to int conversion safety
                // Check for NaN and infinity
                if (std::isnan(new_valuedouble) || std::isinf(new_valuedouble)) {
                    // Use a safe default value for invalid doubles
                    new_valuedouble = 0.0;
                }
                
                // Check if value is within reasonable bounds for int conversion
                // to avoid undefined behavior in cJSON internal conversion
                const double max_safe_int = static_cast<double>(std::numeric_limits<int>::max());
                const double min_safe_int = static_cast<double>(std::numeric_limits<int>::min());
                
                if (new_valuedouble > max_safe_int || new_valuedouble < min_safe_int) {
                    // Clamp to safe range
                    new_valuedouble = std::max(min_safe_int, std::min(max_safe_int, new_valuedouble));
                }
                
                // Use cJSON_SetNumberValue instead of direct assignment
                cJSON_SetNumberValue(item, new_valuedouble);
            }
            break;
        case cJSON_String:
        case cJSON_Raw:
            if (item->valuestring != NULL && fdp.remaining_bytes() > 0) {
                // Modify string slightly using proper API call
                std::string current = item->valuestring;
                if (!current.empty() && fdp.ConsumeBool()) {
                    size_t pos = fdp.ConsumeIntegralInRange<size_t>(0, current.length() - 1);
                    current[pos] = fdp.ConsumeIntegral<char>();
                    // Use cJSON_SetValuestring instead of direct assignment
                    cJSON_SetValuestring(item, current.c_str());
                }
            }
            break;
            
        case cJSON_Array:
            {
                cJSON* child = item->child;
                int child_idx = 0;
                while (child != NULL && fdp.remaining_bytes() > 0) {
                    if (fdp.ConsumeProbability<double>() < 0.3) {
                        modify_json_structure(child, fdp);
                    }
                    child = child->next;
                    child_idx++;
                }
            }
            break;
            
        case cJSON_Object:
            {
                cJSON* child = item->child;
                while (child != NULL && fdp.remaining_bytes() > 0) {
                    if (fdp.ConsumeProbability<double>() < 0.3) {
                        modify_json_structure(child, fdp);
                    }
                    child = child->next;
                }
            }
            break;
            
        default:
            // Other types (False, True, NULL) can't be modified
            break;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) {
        return 0;  // Need sufficient input for comprehensive testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume test scenario selector
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Test Scenario 0: All type combinations (8x8 = 64 comparisons)
    if (test_scenario == 0) {
        bool case_sensitive = fdp.ConsumeBool();
        
        // Test all type combinations
        for (int type1 = 0; type1 < 8; type1++) {
            for (int type2 = 0; type2 < 8; type2++) {
                cJSON* item1 = create_json_type(fdp, type1);
                cJSON* item2 = create_json_type(fdp, type2);
                
                if (item1 != NULL && item2 != NULL) {
                    cJSON_Compare(item1, item2, case_sensitive);
                    
                    // Also test with case_sensitive flag flipped
                    if (fdp.ConsumeBool()) {
                        cJSON_Compare(item1, item2, !case_sensitive);
                    }
                }
                
                if (item1 != NULL) cJSON_Delete(item1);
                if (item2 != NULL) cJSON_Delete(item2);
                
                if (fdp.remaining_bytes() < 100) break;
            }
            if (fdp.remaining_bytes() < 100) break;
        }
        return 0;
    }
    
    // Test Scenario 1: Identical vs modified structures
    if (test_scenario == 1) {
        bool case_sensitive = fdp.ConsumeBool();
        
        // Create base structure
        uint8_t base_type = fdp.ConsumeIntegral<uint8_t>() % 8;
        cJSON* base_item = create_json_type(fdp, base_type);
        
        if (base_item != NULL) {
            // Test identical comparison (should return true)
            cJSON_Compare(base_item, base_item, case_sensitive);
            
            // Create a copy and modify it slightly
            char* json_str = cJSON_Print(base_item);
            if (json_str != NULL) {
                cJSON* copy_item = cJSON_Parse(json_str);
                if (copy_item != NULL) {
                    // Modify the copy slightly
                    modify_json_structure(copy_item, fdp);
                    
                    // Compare original with modified copy
                    cJSON_Compare(base_item, copy_item, case_sensitive);
                    
                    cJSON_Delete(copy_item);
                }
                free(json_str);
            }
            
            cJSON_Delete(base_item);
        }
        return 0;
    }
    
    // Test Scenario 2: Nested arrays with different ordering
    if (test_scenario == 2) {
        bool case_sensitive = fdp.ConsumeBool();
        
        // Create two arrays with same elements but potentially different order
        cJSON* array1 = cJSON_CreateArray();
        cJSON* array2 = cJSON_CreateArray();
        
        if (array1 != NULL && array2 != NULL) {
            int array_size = fdp.ConsumeIntegralInRange<int>(1, 5);
            std::vector<cJSON*> elements;
            
            // Create elements
            for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                uint8_t elem_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                cJSON* elem = create_json_type(fdp, elem_type);
                if (elem != NULL) {
                    elements.push_back(elem);
                }
            }
            
            // Add elements to array1 in original order
            for (cJSON* elem : elements) {
                cJSON_AddItemToArray(array1, cJSON_Duplicate(elem, 1));
            }
            
            // Add elements to array2 in potentially different order
            if (fdp.ConsumeBool() && elements.size() > 1) {
                // Reverse order
                for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
                    cJSON_AddItemToArray(array2, cJSON_Duplicate(*it, 1));
                }
            } else {
                // Same order
                for (cJSON* elem : elements) {
                    cJSON_AddItemToArray(array2, cJSON_Duplicate(elem, 1));
                }
            }
            
            // Compare arrays
            cJSON_Compare(array1, array2, case_sensitive);
            
            // Cleanup
            for (cJSON* elem : elements) {
                cJSON_Delete(elem);
            }
        }
        
        if (array1 != NULL) cJSON_Delete(array1);
        if (array2 != NULL) cJSON_Delete(array2);
        return 0;
    }
    
    // Test Scenario 3: Objects with same/different keys
    if (test_scenario == 3) {
        bool case_sensitive = fdp.ConsumeBool();
        
        // Create two objects
        cJSON* obj1 = cJSON_CreateObject();
        cJSON* obj2 = cJSON_CreateObject();
        
        if (obj1 != NULL && obj2 != NULL) {
            int num_keys = fdp.ConsumeIntegralInRange<int>(1, 5);
            std::vector<std::string> keys;
            
            // Generate keys
            for (int i = 0; i < num_keys && fdp.remaining_bytes() > 0; i++) {
                keys.push_back(fdp.ConsumeRandomLengthString(10));
            }
            
            // Add same keys to both objects
            for (const std::string& key : keys) {
                uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                cJSON* value1 = create_json_type(fdp, value_type);
                cJSON* value2 = create_json_type(fdp, value_type);
                
                if (value1 != NULL) cJSON_AddItemToObject(obj1, key.c_str(), value1);
                if (value2 != NULL) cJSON_AddItemToObject(obj2, key.c_str(), value2);
            }
            
            // Compare objects with same keys
            cJSON_Compare(obj1, obj2, case_sensitive);
            
            // Now test with different keys
            if (fdp.ConsumeBool() && keys.size() > 1) {
                // Remove one key from obj2
                std::string key_to_remove = keys[fdp.ConsumeIntegralInRange<size_t>(0, keys.size() - 1)];
                cJSON_DeleteItemFromObject(obj2, key_to_remove.c_str());
                
                // Compare objects with different keys
                cJSON_Compare(obj1, obj2, case_sensitive);
            }
        }
        
        if (obj1 != NULL) cJSON_Delete(obj1);
        if (obj2 != NULL) cJSON_Delete(obj2);
        return 0;
    }
    
    // Test Scenario 4: Case-sensitive vs case-insensitive string comparisons
    if (test_scenario == 4) {
        // Create strings with case variations
        std::string base_str = fdp.ConsumeRandomLengthString(20);
        if (base_str.empty()) base_str = "test";
        
        std::string upper_str = base_str;
        std::transform(upper_str.begin(), upper_str.end(), upper_str.begin(), ::toupper);
        
        std::string lower_str = base_str;
        std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
        
        cJSON* str1 = cJSON_CreateString(base_str.c_str());
        cJSON* str2_upper = cJSON_CreateString(upper_str.c_str());
        cJSON* str2_lower = cJSON_CreateString(lower_str.c_str());
        
        if (str1 != NULL && str2_upper != NULL && str2_lower != NULL) {
            // Test case-sensitive (should be different for case-different strings)
            cJSON_Compare(str1, str2_upper, true);
            cJSON_Compare(str1, str2_lower, true);
            
            // Test case-insensitive (might be same depending on string)
            cJSON_Compare(str1, str2_upper, false);
            cJSON_Compare(str1, str2_lower, false);
            
            // Also test in object keys
            cJSON* obj1 = cJSON_CreateObject();
            cJSON* obj2 = cJSON_CreateObject();
            
            if (obj1 != NULL && obj2 != NULL) {
                cJSON_AddItemToObject(obj1, base_str.c_str(), cJSON_CreateNumber(1.0));
                cJSON_AddItemToObject(obj2, upper_str.c_str(), cJSON_CreateNumber(1.0));
                
                cJSON_Compare(obj1, obj2, true);   // Case-sensitive
                cJSON_Compare(obj1, obj2, false);  // Case-insensitive
                
                cJSON_Delete(obj1);
                cJSON_Delete(obj2);
            }
        }
        
        if (str1 != NULL) cJSON_Delete(str1);
        if (str2_upper != NULL) cJSON_Delete(str2_upper);
        if (str2_lower != NULL) cJSON_Delete(str2_lower);
        return 0;
    }
    
    // Test Scenario 5: Edge cases - NULL pointers, invalid JSON types
    if (test_scenario == 5) {
        bool case_sensitive = fdp.ConsumeBool();
        
        // Create some valid objects first
        cJSON* valid_objects[3];
        valid_objects[0] = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
        valid_objects[1] = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        valid_objects[2] = cJSON_CreateObject();
        if (valid_objects[2] != NULL) {
            std::string key = fdp.ConsumeRandomLengthString(16);
            cJSON_AddNumberToObject(valid_objects[2], key.c_str(), fdp.ConsumeFloatingPoint<double>());
        }
        
        // Test NULL vs NULL
        cJSON_Compare(NULL, NULL, case_sensitive);
        
        // Test NULL vs valid objects
        for (int i = 0; i < 3; i++) {
            if (valid_objects[i] != NULL) {
                cJSON_Compare(NULL, valid_objects[i], case_sensitive);
                cJSON_Compare(valid_objects[i], NULL, case_sensitive);
            }
        }
        
        // Test with malformed objects (using valid API calls but with unusual data)
        if (fdp.ConsumeBool()) {
            // Create a valid object but modify it in valid ways
            cJSON* unusual_obj = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            if (unusual_obj != NULL) {
                // Test with very large or small numbers
                double extreme_value = fdp.ConsumeFloatingPoint<double>();
                if (std::isnan(extreme_value) || std::isinf(extreme_value)) {
                    extreme_value = 0.0;
                }
                cJSON_SetNumberValue(unusual_obj, extreme_value);
                
                if (valid_objects[0] != NULL) {
                    cJSON_Compare(valid_objects[0], unusual_obj, case_sensitive);
                    cJSON_Compare(unusual_obj, valid_objects[0], case_sensitive);
                }
                
                cJSON_Delete(unusual_obj);
            }
        }
        
        // Cleanup
        for (int i = 0; i < 3; i++) {
            if (valid_objects[i] != NULL) cJSON_Delete(valid_objects[i]);
        }
        return 0;
    }
    
    // Test Scenario 6: Deeply nested structures
    if (test_scenario == 6) {
        bool case_sensitive = fdp.ConsumeBool();
        
        // Create deeply nested structure
        int depth = fdp.ConsumeIntegralInRange<int>(1, 5);
        cJSON* nested1 = create_deep_nested(fdp, depth, true);  // Start with array
        cJSON* nested2 = create_deep_nested(fdp, depth, false); // Start with object
        
        if (nested1 != NULL && nested2 != NULL) {
            // Compare different nested structures
            cJSON_Compare(nested1, nested2, case_sensitive);
            
            // Create duplicate of nested1 and compare
            char* json_str = cJSON_Print(nested1);
            if (json_str != NULL) {
                cJSON* nested1_copy = cJSON_Parse(json_str);
                if (nested1_copy != NULL) {
                    cJSON_Compare(nested1, nested1_copy, case_sensitive);
                    cJSON_Delete(nested1_copy);
                }
                free(json_str);
            }
        }
        
        if (nested1 != NULL) cJSON_Delete(nested1);
        if (nested2 != NULL) cJSON_Delete(nested2);
        return 0;
    }
    
    // Test Scenario 7: Large arrays and objects with many elements
    if (test_scenario == 7) {
        bool case_sensitive = fdp.ConsumeBool();
        
        // Create large array
        cJSON* large_array = cJSON_CreateArray();
        if (large_array != NULL) {
            int array_size = fdp.ConsumeIntegralInRange<int>(10, 20);
            for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                uint8_t elem_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                cJSON* elem = create_json_type(fdp, elem_type);
                if (elem != NULL) {
                    cJSON_AddItemToArray(large_array, elem);
                }
            }
            
            // Create duplicate and compare
            char* json_str = cJSON_Print(large_array);
            if (json_str != NULL) {
                cJSON* array_copy = cJSON_Parse(json_str);
                if (array_copy != NULL) {
                    cJSON_Compare(large_array, array_copy, case_sensitive);
                    
                    // Modify one element and compare again
                    if (large_array->child != NULL) {
                        cJSON* first_elem = large_array->child;
                        modify_json_structure(first_elem, fdp);
                        cJSON_Compare(large_array, array_copy, case_sensitive);
                    }
                    
                    cJSON_Delete(array_copy);
                }
                free(json_str);
            }
            
            cJSON_Delete(large_array);
        }
        
        // Create object with many key-value pairs
        cJSON* large_object = cJSON_CreateObject();
        if (large_object != NULL) {
            int obj_size = fdp.ConsumeIntegralInRange<int>(10, 20);
            for (int i = 0; i < obj_size && fdp.remaining_bytes() > 0; i++) {
                std::string key = "key_" + std::to_string(i);
                uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                cJSON* value = create_json_type(fdp, value_type);
                if (value != NULL) {
                    cJSON_AddItemToObject(large_object, key.c_str(), value);
                }
            }
            
            // Test comparison with itself
            cJSON_Compare(large_object, large_object, case_sensitive);
            
            cJSON_Delete(large_object);
        }
        
        return 0;
    }
    
    return 0;
}

} // namespace harness_013
