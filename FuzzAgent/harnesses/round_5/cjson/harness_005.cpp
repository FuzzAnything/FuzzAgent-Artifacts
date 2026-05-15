#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>
#include "cjson/cJSON.h"

namespace harness_005 {
/*
 * Harness for comprehensive cJSON_Compare testing - targets 62 blocked branches
 * in the cJSON_Compare function identified as the single biggest coverage gap.
 * 
 * This harness thoroughly tests cJSON_Compare with diverse JSON structures and
 * comparison scenarios to exercise all code paths in the comparison logic:
 * - Identical objects (should return true)
 * - Different objects (should return false)
 * - Nested structures (objects within arrays, arrays within objects)
 * - Case-sensitive vs case-insensitive string comparisons
 * - Number comparisons with edge cases (NaN, infinity, precision limits)
 * - All JSON type combinations
 * - Object subset/superset relationships
 * - Arrays with same elements in different order
 * 
 * Implementation follows the coverage guidance invocation sequence:
 * 1. Create pairs of JSON objects with various structures and types
 * 2. Test cJSON_Compare on identical objects (should return true)
 * 3. Test cJSON_Compare on different objects (should return false)
 * 4. Test nested structures
 * 5. Test case-sensitive vs case-insensitive string comparisons
 * 6. Test number comparisons with edge cases
 * 7. Test all JSON type combinations
 * 8. Always call cJSON_Delete to clean up allocated objects
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing of comparison operations
    // Need enough data for creating multiple JSON structures with various types
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Part 1: Create test JSON structures for comparison testing
    // -----------------------------------------------------------
    
    // First, create a variety of JSON items of different types
    
    // 1.1 Create primitive JSON items
    cJSON* null_item = cJSON_CreateNull();
    cJSON* true_item = cJSON_CreateTrue();
    cJSON* false_item = cJSON_CreateFalse();
    
    // Create boolean with random value
    cJSON_bool random_bool = fdp.ConsumeBool();
    cJSON* bool_item = cJSON_CreateBool(random_bool);
    
    // Create number with random value (including potential edge cases)
    double random_number = fdp.ConsumeFloatingPoint<double>();
    cJSON* number_item = cJSON_CreateNumber(random_number);
    
    // Create string with random content (for case-sensitive testing)
    std::string random_string = fdp.ConsumeRandomLengthString(256);
    cJSON* string_item = cJSON_CreateString(random_string.c_str());
    
    // Create raw JSON string
    std::string random_raw = fdp.ConsumeRandomLengthString(128);
    cJSON* raw_item = cJSON_CreateRaw(random_raw.c_str());
    
    // 1.2 Create arrays with different structures
    cJSON* empty_array = cJSON_CreateArray();
    cJSON* simple_array = cJSON_CreateArray();
    
    // Populate simple array with mixed types
    if (simple_array != NULL) {
        size_t array_size = fdp.ConsumeIntegralInRange<size_t>(1, 5);
        for (size_t i = 0; i < array_size; i++) {
            uint8_t elem_type = fdp.ConsumeIntegral<uint8_t>() % 6;
            switch (elem_type) {
                case 0:
                    cJSON_AddItemToArray(simple_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                    break;
                case 1:
                    cJSON_AddItemToArray(simple_array, cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str()));
                    break;
                case 2:
                    cJSON_AddItemToArray(simple_array, cJSON_CreateBool(fdp.ConsumeBool()));
                    break;
                case 3:
                    cJSON_AddItemToArray(simple_array, cJSON_CreateNull());
                    break;
                case 4:
                    cJSON_AddItemToArray(simple_array, cJSON_CreateRaw(fdp.ConsumeRandomLengthString(16).c_str()));
                    break;
                case 5:
                    cJSON_AddItemToArray(simple_array, cJSON_CreateObject());
                    break;
            }
        }
    }
    
    // Create another array that's identical to simple_array for equality testing
    cJSON* identical_array = NULL;
    if (simple_array != NULL) {
        // Duplicate the array structure
        identical_array = cJSON_Duplicate(simple_array, 1);
    }
    
    // 1.3 Create objects with different structures
    cJSON* empty_object = cJSON_CreateObject();
    cJSON* simple_object = cJSON_CreateObject();
    
    // Populate simple object with various key-value pairs
    if (simple_object != NULL) {
        size_t obj_size = fdp.ConsumeIntegralInRange<size_t>(2, 6);
        for (size_t i = 0; i < obj_size; i++) {
            std::string key = fdp.ConsumeRandomLengthString(16);
            uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 5;
            switch (value_type) {
                case 0:
                    cJSON_AddNumberToObject(simple_object, key.c_str(), fdp.ConsumeFloatingPoint<double>());
                    break;
                case 1:
                    cJSON_AddStringToObject(simple_object, key.c_str(), fdp.ConsumeRandomLengthString(64).c_str());
                    break;
                case 2:
                    cJSON_AddBoolToObject(simple_object, key.c_str(), fdp.ConsumeBool());
                    break;
                case 3:
                    cJSON_AddNullToObject(simple_object, key.c_str());
                    break;
                case 4:
                    cJSON_AddRawToObject(simple_object, key.c_str(), fdp.ConsumeRandomLengthString(32).c_str());
                    break;
            }
        }
    }
    
    // Create another object that's identical to simple_object for equality testing
    cJSON* identical_object = NULL;
    if (simple_object != NULL) {
        identical_object = cJSON_Duplicate(simple_object, 1);
    }
    
    // 1.4 Create nested structures for complex comparisons
    cJSON* nested_object = cJSON_CreateObject();
    if (nested_object != NULL) {
        // Add an array as a value
        cJSON* nested_array = cJSON_CreateArray();
        if (nested_array != NULL) {
            size_t nested_array_size = fdp.ConsumeIntegralInRange<size_t>(1, 3);
            for (size_t i = 0; i < nested_array_size; i++) {
                cJSON_AddItemToArray(nested_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
            }
            cJSON_AddItemToObject(nested_object, "nested_array", nested_array);
        }
        
        // Add an object as a value
        cJSON* inner_object = cJSON_CreateObject();
        if (inner_object != NULL) {
            cJSON_AddStringToObject(inner_object, "inner_key", fdp.ConsumeRandomLengthString(32).c_str());
            cJSON_AddItemToObject(nested_object, "inner_object", inner_object);
        }
    }
    
    // Create a modified version of nested_object for inequality testing
    cJSON* modified_nested_object = NULL;
    if (nested_object != NULL) {
        modified_nested_object = cJSON_Duplicate(nested_object, 1);
        if (modified_nested_object != NULL) {
            // Modify one value to make it different
            cJSON* array_item = cJSON_GetObjectItem(modified_nested_object, "nested_array");
            if (array_item != NULL && cJSON_GetArraySize(array_item) > 0) {
                cJSON* first_elem = cJSON_GetArrayItem(array_item, 0);
                if (first_elem != NULL) {
                    cJSON_SetNumberValue(first_elem, fdp.ConsumeFloatingPoint<double>());
                }
            }
        }
    }
    
    // Part 2: Comprehensive cJSON_Compare testing
    // --------------------------------------------
    
    // 2.1 Test identical objects (should return true)
    if (identical_object != NULL && simple_object != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool objects_equal = cJSON_Compare(simple_object, identical_object, case_sensitive);
        // objects_equal should be true for identical objects
    }
    
    // 2.2 Test different objects (should return false)
    if (simple_object != NULL && nested_object != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool objects_equal = cJSON_Compare(simple_object, nested_object, case_sensitive);
        // objects_equal should be false for different objects
    }
    
    // 2.3 Test empty vs non-empty arrays
    if (empty_array != NULL && simple_array != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool arrays_equal = cJSON_Compare(empty_array, simple_array, case_sensitive);
        // arrays_equal should be false
    }
    
    // 2.4 Test identical arrays (should return true)
    if (simple_array != NULL && identical_array != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool arrays_equal = cJSON_Compare(simple_array, identical_array, case_sensitive);
        // arrays_equal should be true
    }
    
    // 2.5 Test case-sensitive vs case-insensitive string comparisons
    if (string_item != NULL) {
        // Create a string with different casing for comparison
        std::string modified_string = random_string;
        if (!modified_string.empty()) {
            // Change first character case
            if (std::isalpha(modified_string[0])) {
                if (std::islower(modified_string[0])) {
                    modified_string[0] = std::toupper(modified_string[0]);
                } else {
                    modified_string[0] = std::tolower(modified_string[0]);
                }
            }
            cJSON* modified_string_item = cJSON_CreateString(modified_string.c_str());
            
            if (modified_string_item != NULL) {
                // Case-sensitive comparison (should return false if case differs)
                cJSON_bool case_sensitive_true = cJSON_Compare(string_item, modified_string_item, true);
                
                // Case-insensitive comparison (might return true if library supports it)
                cJSON_bool case_sensitive_false = cJSON_Compare(string_item, modified_string_item, false);
                
                cJSON_Delete(modified_string_item);
            }
        }
    }
    
    // 2.6 Test number comparisons with edge cases
    if (number_item != NULL) {
        // Create numbers with special values
        double special_values[] = {0.0, -0.0, 1.0, -1.0, 
                                  std::numeric_limits<double>::infinity(),
                                  -std::numeric_limits<double>::infinity(),
                                  std::numeric_limits<double>::quiet_NaN()};
        
        for (size_t i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
            double test_value = fdp.ConsumeFloatingPoint<double>();
            cJSON* test_number = cJSON_CreateNumber(test_value);
            
            if (test_number != NULL) {
                cJSON_bool case_sensitive = fdp.ConsumeBool();
                cJSON_bool numbers_equal = cJSON_Compare(number_item, test_number, case_sensitive);
                
                cJSON_Delete(test_number);
            }
        }
    }
    
    // 2.7 Test all JSON type combinations
    // Create pairs of different type items and compare them
    std::vector<cJSON*> type_items;
    if (null_item != NULL) type_items.push_back(null_item);
    if (true_item != NULL) type_items.push_back(true_item);
    if (false_item != NULL) type_items.push_back(false_item);
    if (bool_item != NULL) type_items.push_back(bool_item);
    if (number_item != NULL) type_items.push_back(number_item);
    if (string_item != NULL) type_items.push_back(string_item);
    if (raw_item != NULL) type_items.push_back(raw_item);
    if (empty_array != NULL) type_items.push_back(empty_array);
    if (empty_object != NULL) type_items.push_back(empty_object);
    
    // Compare each item with every other item (different types should return false)
    for (size_t i = 0; i < type_items.size() && i < 3; i++) {
        for (size_t j = i + 1; j < type_items.size() && j < 5; j++) {
            cJSON_bool case_sensitive = fdp.ConsumeBool();
            cJSON_bool types_equal = cJSON_Compare(type_items[i], type_items[j], case_sensitive);
            // Different types should compare as false
        }
    }
    
    // 2.8 Test nested structure comparisons
    if (nested_object != NULL && modified_nested_object != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool nested_equal = cJSON_Compare(nested_object, modified_nested_object, case_sensitive);
        // Should be false since we modified one value
    }
    
    // 2.9 Test object with same keys but different values
    if (simple_object != NULL) {
        // Create object with same keys but different values
        cJSON* same_keys_object = cJSON_Duplicate(simple_object, 1);
        if (same_keys_object != NULL) {
            // Change values for all items
            cJSON* child = same_keys_object->child;
            while (child != NULL && fdp.remaining_bytes() > 0) {
                if (cJSON_IsNumber(child)) {
                    cJSON_SetNumberValue(child, fdp.ConsumeFloatingPoint<double>());
                } else if (cJSON_IsString(child)) {
                    std::string new_str = fdp.ConsumeRandomLengthString(32);
                    cJSON_SetValuestring(child, new_str.c_str());
                }
                child = child->next;
            }
            
            cJSON_bool case_sensitive = fdp.ConsumeBool();
            cJSON_bool same_keys_equal = cJSON_Compare(simple_object, same_keys_object, case_sensitive);
            // Should be false since values are different
            
            cJSON_Delete(same_keys_object);
        }
    }
    
    // 2.10 Test arrays with same elements in different order
    if (simple_array != NULL && cJSON_GetArraySize(simple_array) >= 2) {
        // Create array with elements in reverse order
        cJSON* reversed_array = cJSON_CreateArray();
        if (reversed_array != NULL) {
            int array_size = cJSON_GetArraySize(simple_array);
            for (int i = array_size - 1; i >= 0; i--) {
                cJSON* item = cJSON_GetArrayItem(simple_array, i);
                if (item != NULL) {
                    cJSON* duplicated_item = cJSON_Duplicate(item, 1);
                    if (duplicated_item != NULL) {
                        cJSON_AddItemToArray(reversed_array, duplicated_item);
                    }
                }
            }
            
            cJSON_bool case_sensitive = fdp.ConsumeBool();
            cJSON_bool order_equal = cJSON_Compare(simple_array, reversed_array, case_sensitive);
            // Should be false if order matters in comparison
            
            cJSON_Delete(reversed_array);
        }
    }
    
    // Part 3: Cleanup all allocated JSON items
    // -----------------------------------------
    if (null_item != NULL) cJSON_Delete(null_item);
    if (true_item != NULL) cJSON_Delete(true_item);
    if (false_item != NULL) cJSON_Delete(false_item);
    if (bool_item != NULL) cJSON_Delete(bool_item);
    if (number_item != NULL) cJSON_Delete(number_item);
    if (string_item != NULL) cJSON_Delete(string_item);
    if (raw_item != NULL) cJSON_Delete(raw_item);
    if (empty_array != NULL) cJSON_Delete(empty_array);
    if (simple_array != NULL) cJSON_Delete(simple_array);
    if (identical_array != NULL) cJSON_Delete(identical_array);
    if (empty_object != NULL) cJSON_Delete(empty_object);
    if (simple_object != NULL) cJSON_Delete(simple_object);
    if (identical_object != NULL) cJSON_Delete(identical_object);
    if (nested_object != NULL) cJSON_Delete(nested_object);
    if (modified_nested_object != NULL) cJSON_Delete(modified_nested_object);
    
    return 0;
}

} // namespace harness_005
