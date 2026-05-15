#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <random>
#include <cfloat>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_020 {
extern "C" {
}

// Helper to create JSON objects using both cJSON_Parse and programmatic creation
cJSON* create_json_via_parse(FuzzedDataProvider& fdp, bool& success) {
    // Create a JSON string from fuzzer input
    std::string json_str = fdp.ConsumeRandomLengthString(256);
    success = false;
    
    // Try to parse it
    cJSON* parsed = cJSON_Parse(json_str.c_str());
    if (parsed != NULL) {
        success = true;
    }
    return parsed;
}

// Helper to create the same JSON structure programmatically
cJSON* create_json_programmatically(FuzzedDataProvider& fdp, const std::string& json_str) {
    // This is a simplified version - in reality we'd need to parse the string
    // and recreate it programmatically. For fuzzing, we'll create similar structures.
    
    // For now, create a simple object with random data
    cJSON* obj = cJSON_CreateObject();
    if (obj == NULL) return NULL;
    
    // Add some properties based on fuzzer input
    int prop_count = fdp.ConsumeIntegralInRange<int>(1, 5);
    for (int i = 0; i < prop_count && fdp.remaining_bytes() > 0; i++) {
        std::string key = fdp.ConsumeRandomLengthString(16);
        uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 7;
        
        cJSON* value = NULL;
        switch (value_type) {
            case 0: value = cJSON_CreateFalse(); break;
            case 1: value = cJSON_CreateTrue(); break;
            case 2: value = cJSON_CreateNull(); break;
            case 3: value = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()); break;
            case 4: value = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str()); break;
            case 5: {
                value = cJSON_CreateArray();
                if (value != NULL) {
                    int arr_size = fdp.ConsumeIntegralInRange<int>(0, 3);
                    for (int j = 0; j < arr_size && fdp.remaining_bytes() > 0; j++) {
                        cJSON_AddItemToArray(value, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                    }
                }
                break;
            }
            case 6: {
                value = cJSON_CreateObject();
                if (value != NULL) {
                    std::string subkey = fdp.ConsumeRandomLengthString(8);
                    cJSON_AddNumberToObject(value, subkey.c_str(), fdp.ConsumeFloatingPoint<double>());
                }
                break;
            }
        }
        
        if (value != NULL) {
            cJSON_AddItemToObject(obj, key.c_str(), value);
        }
    }
    
    return obj;
}

// Helper to create JSON with subtle differences
void create_similar_objects_with_differences(FuzzedDataProvider& fdp, cJSON** obj1, cJSON** obj2) {
    *obj1 = cJSON_CreateObject();
    *obj2 = cJSON_CreateObject();
    
    if (*obj1 == NULL || *obj2 == NULL) {
        if (*obj1) cJSON_Delete(*obj1);
        if (*obj2) cJSON_Delete(*obj2);
        *obj1 = *obj2 = NULL;
        return;
    }
    
    // Create base object
    int base_props = fdp.ConsumeIntegralInRange<int>(2, 6);
    std::vector<std::pair<std::string, double>> base_properties;
    
    for (int i = 0; i < base_props && fdp.remaining_bytes() > 0; i++) {
        std::string key = fdp.ConsumeRandomLengthString(12);
        double value = fdp.ConsumeFloatingPoint<double>();
        base_properties.push_back({key, value});
        
        // Add to both objects
        cJSON_AddNumberToObject(*obj1, key.c_str(), value);
        cJSON_AddNumberToObject(*obj2, key.c_str(), value);
    }
    
    // Now introduce differences based on fuzzer input
    uint8_t diff_type = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    switch (diff_type) {
        case 0: // Different value for same key
            if (!base_properties.empty()) {
                int idx = fdp.ConsumeIntegralInRange<size_t>(0, base_properties.size() - 1);
                // Remove from obj2 and add with different value
                cJSON_DeleteItemFromObject(*obj2, base_properties[idx].first.c_str());
                cJSON_AddNumberToObject(*obj2, base_properties[idx].first.c_str(), 
                                       fdp.ConsumeFloatingPoint<double>());
            }
            break;
            
        case 1: // Missing key in obj2
            if (!base_properties.empty()) {
                int idx = fdp.ConsumeIntegralInRange<size_t>(0, base_properties.size() - 1);
                cJSON_DeleteItemFromObject(*obj2, base_properties[idx].first.c_str());
            }
            break;
            
        case 2: // Extra key in obj2
            {
                std::string extra_key = fdp.ConsumeRandomLengthString(12);
                cJSON_AddNumberToObject(*obj2, extra_key.c_str(), fdp.ConsumeFloatingPoint<double>());
            }
            break;
            
        case 3: // Different ordering (create with different order)
            // Recreate obj2 with shuffled properties
            cJSON_Delete(*obj2);
            *obj2 = cJSON_CreateObject();
            if (*obj2 != NULL) {
                // Add properties in different order
                std::shuffle(base_properties.begin(), base_properties.end(), 
                           std::default_random_engine(fdp.ConsumeIntegral<uint32_t>()));
                for (const auto& prop : base_properties) {
                    cJSON_AddNumberToObject(*obj2, prop.first.c_str(), prop.second);
                }
            }
            break;
            
        case 4: // Case difference in keys (for case_sensitive testing)
            if (!base_properties.empty()) {
                int idx = fdp.ConsumeIntegralInRange<size_t>(0, base_properties.size() - 1);
                std::string alt_key = base_properties[idx].first;
                // Change case of one character
                if (!alt_key.empty()) {
                    size_t pos = fdp.ConsumeIntegralInRange<size_t>(0, alt_key.length() - 1);
                    if (islower(alt_key[pos])) {
                        alt_key[pos] = toupper(alt_key[pos]);
                    } else if (isupper(alt_key[pos])) {
                        alt_key[pos] = tolower(alt_key[pos]);
                    }
                    
                    cJSON_DeleteItemFromObject(*obj2, base_properties[idx].first.c_str());
                    cJSON_AddNumberToObject(*obj2, alt_key.c_str(), base_properties[idx].second);
                }
            }
            break;
    }
}

// Helper to create nested structures with varying depths
cJSON* create_nested_structure(FuzzedDataProvider& fdp, int depth, bool is_object) {
    if (depth <= 0 || fdp.remaining_bytes() < 8) {
        // Create leaf node
        uint8_t leaf_type = fdp.ConsumeIntegral<uint8_t>() % 8;
        switch (leaf_type) {
            case 0: return cJSON_CreateFalse();
            case 1: return cJSON_CreateTrue();
            case 2: return cJSON_CreateNull();
            case 3: return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            case 4: return cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
            case 5: return cJSON_CreateRaw(fdp.ConsumeRandomLengthString(32).c_str());
            case 6: return cJSON_CreateArray(); // Empty array
            default: return cJSON_CreateObject(); // Empty object
        }
    }
    
    if (is_object) {
        cJSON* obj = cJSON_CreateObject();
        if (obj == NULL) return NULL;
        
        int prop_count = fdp.ConsumeIntegralInRange<int>(1, 4);
        for (int i = 0; i < prop_count && fdp.remaining_bytes() > 0; i++) {
            std::string key = fdp.ConsumeRandomLengthString(12);
            cJSON* child = create_nested_structure(fdp, depth - 1, !is_object);
            if (child != NULL) {
                cJSON_AddItemToObject(obj, key.c_str(), child);
            }
        }
        return obj;
    } else {
        cJSON* arr = cJSON_CreateArray();
        if (arr == NULL) return NULL;
        
        int elem_count = fdp.ConsumeIntegralInRange<int>(1, 4);
        for (int i = 0; i < elem_count && fdp.remaining_bytes() > 0; i++) {
            cJSON* child = create_nested_structure(fdp, depth - 1, !is_object);
            if (child != NULL) {
                cJSON_AddItemToArray(arr, child);
            }
        }
        return arr;
    }
}

// Helper to create recursive comparison failure at specific depth
void create_recursive_failure_pair(FuzzedDataProvider& fdp, cJSON** obj1, cJSON** obj2, int target_depth) {
    if (target_depth <= 0) {
        // At target depth, create different values
        *obj1 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        *obj2 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        return;
    }
    
    // Create identical structures until target depth
    cJSON* container1 = cJSON_CreateObject();
    cJSON* container2 = cJSON_CreateObject();
    
    if (container1 == NULL || container2 == NULL) {
        if (container1) cJSON_Delete(container1);
        if (container2) cJSON_Delete(container2);
        *obj1 = *obj2 = NULL;
        return;
    }
    
    // Add same key
    std::string key = fdp.ConsumeRandomLengthString(8);
    
    // Create children with recursion
    cJSON* child1 = NULL;
    cJSON* child2 = NULL;
    create_recursive_failure_pair(fdp, &child1, &child2, target_depth - 1);
    
    if (child1 != NULL && child2 != NULL) {
        cJSON_AddItemToObject(container1, key.c_str(), child1);
        cJSON_AddItemToObject(container2, key.c_str(), child2);
    } else {
        // Fallback: create simple different values
        cJSON_Delete(container1);
        cJSON_Delete(container2);
        *obj1 = cJSON_CreateNumber(1.0);
        *obj2 = cJSON_CreateNumber(2.0);
        return;
    }
    
    *obj1 = container1;
    *obj2 = container2;
}

// Helper to test all JSON types systematically
void test_all_json_types(FuzzedDataProvider& fdp, bool case_sensitive) {
    cJSON* items[8];
    
    // Create all 8 JSON types
    items[0] = cJSON_CreateFalse();
    items[1] = cJSON_CreateTrue();
    items[2] = cJSON_CreateNull();
    items[3] = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
    items[4] = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
    items[5] = cJSON_CreateRaw(fdp.ConsumeRandomLengthString(32).c_str());
    items[6] = cJSON_CreateArray();
    if (items[6] != NULL) {
        cJSON_AddItemToArray(items[6], cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
    }
    items[7] = cJSON_CreateObject();
    if (items[7] != NULL) {
        cJSON_AddNumberToObject(items[7], "key", fdp.ConsumeFloatingPoint<double>());
    }
    
    // Compare each type with itself (should return true for valid types)
    for (int i = 0; i < 8; i++) {
        if (items[i] != NULL) {
            cJSON_Compare(items[i], items[i], case_sensitive);
        }
    }
    
    // Compare each type with every other type (should return false for different types)
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (i != j && items[i] != NULL && items[j] != NULL) {
                cJSON_Compare(items[i], items[j], case_sensitive);
            }
        }
    }
    
    // Cleanup
    for (int i = 0; i < 8; i++) {
        if (items[i] != NULL) {
            cJSON_Delete(items[i]);
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 128) {
        return 0; // Need sufficient input for comprehensive testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test scenario selector
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 12;
    
    // Test Scenario 0: Identical objects via parse vs programmatic creation
    if (test_scenario == 0) {
        bool parse_success = false;
        cJSON* parsed = create_json_via_parse(fdp, parse_success);
        
        if (parse_success && parsed != NULL) {
            // Create similar structure programmatically
            std::string dummy_str = fdp.ConsumeRandomLengthString(32);
            cJSON* programmatic = create_json_programmatically(fdp, dummy_str);
            
            bool case_sensitive = fdp.ConsumeBool();
            
            // Compare parsed with itself
            cJSON_Compare(parsed, parsed, case_sensitive);
            
            // Compare programmatic with itself
            if (programmatic != NULL) {
                cJSON_Compare(programmatic, programmatic, case_sensitive);
            }
            
            // Cleanup
            cJSON_Delete(parsed);
            if (programmatic != NULL) {
                cJSON_Delete(programmatic);
            }
        } else if (parsed != NULL) {
            cJSON_Delete(parsed);
        }
        
        return 0;
    }
    
    // Test Scenario 1: Objects with subtle differences
    if (test_scenario == 1) {
        cJSON* obj1 = NULL;
        cJSON* obj2 = NULL;
        create_similar_objects_with_differences(fdp, &obj1, &obj2);
        
        if (obj1 != NULL && obj2 != NULL) {
            bool case_sensitive = fdp.ConsumeBool();
            
            // Test comparison
            cJSON_Compare(obj1, obj2, case_sensitive);
            cJSON_Compare(obj1, obj1, case_sensitive);
            cJSON_Compare(obj2, obj2, case_sensitive);
            
            // Also test with opposite case sensitivity
            cJSON_Compare(obj1, obj2, !case_sensitive);
        }
        
        if (obj1 != NULL) cJSON_Delete(obj1);
        if (obj2 != NULL) cJSON_Delete(obj2);
        
        return 0;
    }
    
    // Test Scenario 2: Case-sensitive string comparisons
    if (test_scenario == 2) {
        bool case_sensitive = fdp.ConsumeBool();
        
        // Create strings with case differences
        std::string str1 = fdp.ConsumeRandomLengthString(32);
        std::string str2 = str1;
        
        // Modify case of some characters
        if (!str2.empty()) {
            int changes = fdp.ConsumeIntegralInRange<int>(1, std::min(3, (int)str2.length()));
            for (int i = 0; i < changes; i++) {
                size_t pos = fdp.ConsumeIntegralInRange<size_t>(0, str2.length() - 1);
                if (islower(str2[pos])) {
                    str2[pos] = toupper(str2[pos]);
                } else if (isupper(str2[pos])) {
                    str2[pos] = tolower(str2[pos]);
                }
            }
        }
        
        cJSON* json_str1 = cJSON_CreateString(str1.c_str());
        cJSON* json_str2 = cJSON_CreateString(str2.c_str());
        
        if (json_str1 != NULL && json_str2 != NULL) {
            cJSON_Compare(json_str1, json_str2, case_sensitive);
            cJSON_Compare(json_str1, json_str2, !case_sensitive);
        }
        
        if (json_str1 != NULL) cJSON_Delete(json_str1);
        if (json_str2 != NULL) cJSON_Delete(json_str2);
        
        return 0;
    }
    
    // Test Scenario 3: Nested structures with varying depths
    if (test_scenario == 3) {
        int depth = fdp.ConsumeIntegralInRange<int>(1, 5);
        bool start_with_object = fdp.ConsumeBool();
        bool case_sensitive = fdp.ConsumeBool();
        
        // Create two identical nested structures
        cJSON* nested1 = create_nested_structure(fdp, depth, start_with_object);
        cJSON* nested2 = create_nested_structure(fdp, depth, start_with_object);
        
        if (nested1 != NULL && nested2 != NULL) {
            // Compare identical nested structures
            cJSON_Compare(nested1, nested2, case_sensitive);
            
            // Compare each with itself
            cJSON_Compare(nested1, nested1, case_sensitive);
            cJSON_Compare(nested2, nested2, case_sensitive);
        }
        
        // Create slightly different nested structure
        cJSON* nested3 = create_nested_structure(fdp, depth, !start_with_object);
        
        if (nested1 != NULL && nested3 != NULL) {
            // Compare different nested structures (different type at root)
            cJSON_Compare(nested1, nested3, case_sensitive);
        }
        
        if (nested1 != NULL) cJSON_Delete(nested1);
        if (nested2 != NULL) cJSON_Delete(nested2);
        if (nested3 != NULL) cJSON_Delete(nested3);
        
        return 0;
    }
    
    // Test Scenario 4: Edge cases - NULL inputs, type mismatches, empty vs non-empty
    if (test_scenario == 4) {
        bool case_sensitive = fdp.ConsumeBool();
        
        // NULL vs NULL
        cJSON_Compare(NULL, NULL, case_sensitive);
        
        // Create some valid objects
        cJSON* valid_objects[4];
        valid_objects[0] = cJSON_CreateObject(); // Empty object
        valid_objects[1] = cJSON_CreateArray();  // Empty array
        valid_objects[2] = cJSON_CreateString(""); // Empty string
        valid_objects[3] = cJSON_CreateNumber(0.0); // Zero number
        
        // NULL vs valid objects
        for (int i = 0; i < 4; i++) {
            if (valid_objects[i] != NULL) {
                cJSON_Compare(NULL, valid_objects[i], case_sensitive);
                cJSON_Compare(valid_objects[i], NULL, case_sensitive);
            }
        }
        
        // Empty vs non-empty comparisons
        cJSON* non_empty_obj = cJSON_CreateObject();
        if (non_empty_obj != NULL) {
            cJSON_AddNumberToObject(non_empty_obj, "key", fdp.ConsumeFloatingPoint<double>());
            
            if (valid_objects[0] != NULL) {
                cJSON_Compare(valid_objects[0], non_empty_obj, case_sensitive); // Empty vs non-empty object
            }
        }
        
        // Cleanup
        for (int i = 0; i < 4; i++) {
            if (valid_objects[i] != NULL) {
                cJSON_Delete(valid_objects[i]);
            }
        }
        if (non_empty_obj != NULL) cJSON_Delete(non_empty_obj);
        
        return 0;
    }
    
    // Test Scenario 5: Test all JSON types systematically
    if (test_scenario == 5) {
        bool case_sensitive = fdp.ConsumeBool();
        test_all_json_types(fdp, case_sensitive);
        return 0;
    }
    
    // Test Scenario 6: Recursive comparison failure paths
    if (test_scenario == 6) {
        int failure_depth = fdp.ConsumeIntegralInRange<int>(1, 4);
        bool case_sensitive = fdp.ConsumeBool();
        
        cJSON* obj1 = NULL;
        cJSON* obj2 = NULL;
        create_recursive_failure_pair(fdp, &obj1, &obj2, failure_depth);
        
        if (obj1 != NULL && obj2 != NULL) {
            cJSON_Compare(obj1, obj2, case_sensitive);
            
            // Also test each with itself
            cJSON_Compare(obj1, obj1, case_sensitive);
            cJSON_Compare(obj2, obj2, case_sensitive);
        }
        
        if (obj1 != NULL) cJSON_Delete(obj1);
        if (obj2 != NULL) cJSON_Delete(obj2);
        
        return 0;
    }
    
    // Test Scenario 7: Array length differences
    if (test_scenario == 7) {
        bool case_sensitive = fdp.ConsumeBool();
        
        // Create arrays of different lengths
        cJSON* arr1 = cJSON_CreateArray();
        cJSON* arr2 = cJSON_CreateArray();
        
        if (arr1 != NULL && arr2 != NULL) {
            // Add different number of elements
            int count1 = fdp.ConsumeIntegralInRange<int>(1, 5);
            int count2 = count1 + fdp.ConsumeIntegralInRange<int>(1, 3); // Ensure different
            
            for (int i = 0; i < count1 && fdp.remaining_bytes() > 0; i++) {
                cJSON_AddItemToArray(arr1, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
            }
            
            for (int i = 0; i < count2 && fdp.remaining_bytes() > 0; i++) {
                cJSON_AddItemToArray(arr2, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
            }
            
            cJSON_Compare(arr1, arr2, case_sensitive);
            
            // Also create arrays with same length but different values
            cJSON* arr3 = cJSON_CreateArray();
            if (arr3 != NULL) {
                for (int i = 0; i < count1 && fdp.remaining_bytes() > 0; i++) {
                    cJSON_AddItemToArray(arr3, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>() + 1.0));
                }
                cJSON_Compare(arr1, arr3, case_sensitive);
                cJSON_Delete(arr3);
            }
        }
        
        if (arr1 != NULL) cJSON_Delete(arr1);
        if (arr2 != NULL) cJSON_Delete(arr2);
        
        return 0;
    }
    
    // Test Scenario 8: Object subset comparison (A subset of B)
    if (test_scenario == 8) {
        bool case_sensitive = fdp.ConsumeBool();
        
        // Create object A
        cJSON* objA = cJSON_CreateObject();
        // Create object B with all of A's keys plus extra
        cJSON* objB = cJSON_CreateObject();
        
        if (objA != NULL && objB != NULL) {
            // Add some common properties
            int common_count = fdp.ConsumeIntegralInRange<int>(2, 4);
            for (int i = 0; i < common_count && fdp.remaining_bytes() > 0; i++) {
                std::string key = fdp.ConsumeRandomLengthString(8);
                double value = fdp.ConsumeFloatingPoint<double>();
                
                cJSON_AddNumberToObject(objA, key.c_str(), value);
                cJSON_AddNumberToObject(objB, key.c_str(), value);
            }
            
            // Add extra properties only to B
            int extra_count = fdp.ConsumeIntegralInRange<int>(1, 3);
            for (int i = 0; i < extra_count && fdp.remaining_bytes() > 0; i++) {
                std::string extra_key = fdp.ConsumeRandomLengthString(8);
                cJSON_AddNumberToObject(objB, extra_key.c_str(), fdp.ConsumeFloatingPoint<double>());
            }
            
            cJSON_Compare(objA, objB, case_sensitive);
            cJSON_Compare(objB, objA, case_sensitive);
        }
        
        if (objA != NULL) cJSON_Delete(objA);
        if (objB != NULL) cJSON_Delete(objB);
        
        return 0;
    }
    
    // Test Scenario 9: Floating point precision differences
    if (test_scenario == 9) {
        bool case_sensitive = fdp.ConsumeBool();
        
        // Test numbers that are close but not exactly equal
        double num1 = fdp.ConsumeFloatingPoint<double>();
        double num2 = num1 + DBL_EPSILON * fdp.ConsumeFloatingPoint<double>() * 10.0;
        
        cJSON* json_num1 = cJSON_CreateNumber(num1);
        cJSON* json_num2 = cJSON_CreateNumber(num2);
        
        if (json_num1 != NULL && json_num2 != NULL) {
            cJSON_Compare(json_num1, json_num2, case_sensitive);
            
            // Test with exactly equal numbers
            cJSON* json_num1_copy = cJSON_CreateNumber(num1);
            if (json_num1_copy != NULL) {
                cJSON_Compare(json_num1, json_num1_copy, case_sensitive);
                cJSON_Delete(json_num1_copy);
            }
        }
        
        if (json_num1 != NULL) cJSON_Delete(json_num1);
        if (json_num2 != NULL) cJSON_Delete(json_num2);
        
        return 0;
    }
    
    // Test Scenario 10: Raw JSON comparisons
    if (test_scenario == 10) {
        bool case_sensitive = fdp.ConsumeBool();
        
        // Create raw JSON strings
        std::string raw1 = fdp.ConsumeRandomLengthString(64);
        std::string raw2 = fdp.ConsumeRandomLengthString(64);
        
        cJSON* json_raw1 = cJSON_CreateRaw(raw1.c_str());
        cJSON* json_raw2 = cJSON_CreateRaw(raw2.c_str());
        cJSON* json_raw1_copy = cJSON_CreateRaw(raw1.c_str());
        
        if (json_raw1 != NULL && json_raw2 != NULL && json_raw1_copy != NULL) {
            // Different raw strings
            cJSON_Compare(json_raw1, json_raw2, case_sensitive);
            
            // Same raw strings
            cJSON_Compare(json_raw1, json_raw1_copy, case_sensitive);
            
            // Test NULL valuestring case (if we can create it)
            // Note: This would require accessing internal structure
        }
        
        if (json_raw1 != NULL) cJSON_Delete(json_raw1);
        if (json_raw2 != NULL) cJSON_Delete(json_raw2);
        if (json_raw1_copy != NULL) cJSON_Delete(json_raw1_copy);
        
        return 0;
    }
    
    // Test Scenario 11: Mixed complex scenarios
    if (test_scenario == 11) {
        bool case_sensitive = fdp.ConsumeBool();
        
        // Create a complex nested structure
        cJSON* complex = create_nested_structure(fdp, 3, true);
        
        if (complex != NULL) {
            // Compare with itself
            cJSON_Compare(complex, complex, case_sensitive);
            
            // Create a modified version
            cJSON* modified = create_nested_structure(fdp, 3, false); // Different type at root
            
            if (modified != NULL) {
                cJSON_Compare(complex, modified, case_sensitive);
                cJSON_Delete(modified);
            }
            
            // Test with subset of properties
            cJSON* subset = cJSON_CreateObject();
            if (subset != NULL) {
                // Try to copy some properties (simplified)
                cJSON* child = complex->child;
                int count = 0;
                while (child != NULL && count < 2 && fdp.remaining_bytes() > 0) {
                    if (child->string != NULL) {
                        cJSON* copy = cJSON_Duplicate(child, 1);
                        if (copy != NULL) {
                            cJSON_AddItemToObject(subset, child->string, copy);
                        }
                    }
                    child = child->next;
                    count++;
                }
                
                cJSON_Compare(complex, subset, case_sensitive);
                cJSON_Delete(subset);
            }
            
            cJSON_Delete(complex);
        }
        
        return 0;
    }
    
    return 0;
}

} // namespace harness_020
