#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include "cjson/cJSON.h"

namespace harness_014 {
/*
 * Harness_014: cJSON_Compare testing with cJSON_Duplicate-based modifications
 * 
 * Target: cJSON_Compare function with 73/76 branches blocked
 * Focus: Complex object comparison scenarios to hit blocked branches in:
 *   - Subset checking (object key subset/superset relationships)
 *   - Array length mismatches
 *   - NULL string handling
 *   - Type validation
 * 
 * SEMANTIC DIFFERENTIATION FROM PREVIOUS HARNESSES (000-013):
 * - Previous harnesses: Create objects programmatically or parse JSON strings
 * - This harness_014: Uses cJSON_Duplicate to create identical copies, then
 *   systematically modifies specific aspects to test comparison logic
 * 
 * Novel approach benefits:
 * 1. Ensures objects are structurally identical before modification
 * 2. Allows precise targeting of specific comparison failure points
 * 3. Tests cJSON_Duplicate in conjunction with cJSON_Compare
 * 4. Can create subtle differences that test edge cases
 * 
 * Implementation strategy:
 * 1. Create complex JSON objects using programmatic creation
 * 2. Duplicate objects using cJSON_Duplicate
 * 3. Systematically modify duplicates to create specific failure cases
 * 4. Test cJSON_Compare on original vs modified objects
 * 5. Focus on blocked branch conditions:
 *    - Object subset relationships (keys present in one but not other)
 *    - Array length mismatches
 *    - NULL string pointers
 *    - Type validation failures
 */



// Helper to create a complex object with nested structures
static cJSON* create_complex_test_object(FuzzedDataProvider* fdp, int depth = 0) {
    if (depth > 3) {  // Limit recursion depth
        return cJSON_CreateNull();
    }
    
    cJSON* obj = cJSON_CreateObject();
    if (obj == NULL) {
        return NULL;
    }
    
    // Add different types of items based on fuzzer input
    size_t num_items = fdp->ConsumeIntegralInRange<size_t>(2, 6);
    
    for (size_t i = 0; i < num_items; i++) {
        std::string key = fdp->ConsumeRandomLengthString(8);
        uint8_t item_type = fdp->ConsumeIntegral<uint8_t>() % 7;
        
        switch (item_type) {
            case 0:  // Number
                cJSON_AddNumberToObject(obj, key.c_str(), fdp->ConsumeFloatingPoint<double>());
                break;
            case 1:  // String
                cJSON_AddStringToObject(obj, key.c_str(), fdp->ConsumeRandomLengthString(32).c_str());
                break;
            case 2:  // Boolean
                cJSON_AddBoolToObject(obj, key.c_str(), fdp->ConsumeBool());
                break;
            case 3:  // Null
                cJSON_AddNullToObject(obj, key.c_str());
                break;
            case 4:  // Array
                {
                    cJSON* array = cJSON_CreateArray();
                    if (array != NULL) {
                        size_t array_size = fdp->ConsumeIntegralInRange<size_t>(0, 4);
                        for (size_t j = 0; j < array_size; j++) {
                            uint8_t array_type = fdp->ConsumeIntegral<uint8_t>() % 4;
                            switch (array_type) {
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
                        cJSON_AddItemToObject(obj, key.c_str(), array);
                    }
                }
                break;
            case 5:  // Nested object
                {
                    cJSON* nested = create_complex_test_object(fdp, depth + 1);
                    if (nested != NULL) {
                        cJSON_AddItemToObject(obj, key.c_str(), nested);
                    }
                }
                break;
            case 6:  // Raw JSON
                cJSON_AddRawToObject(obj, key.c_str(), fdp->ConsumeRandomLengthString(24).c_str());
                break;
        }
    }
    
    return obj;
}

// Helper to create arrays with varying structures
static cJSON* create_test_array(FuzzedDataProvider* fdp) {
    cJSON* array = cJSON_CreateArray();
    if (array == NULL) {
        return NULL;
    }
    
    size_t array_size = fdp->ConsumeIntegralInRange<size_t>(2, 8);
    
    for (size_t i = 0; i < array_size; i++) {
        uint8_t elem_type = fdp->ConsumeIntegral<uint8_t>() % 5;
        switch (elem_type) {
            case 0:
                cJSON_AddItemToArray(array, cJSON_CreateNumber(fdp->ConsumeFloatingPoint<double>()));
                break;
            case 1:
                cJSON_AddItemToArray(array, cJSON_CreateString(fdp->ConsumeRandomLengthString(24).c_str()));
                break;
            case 2:
                cJSON_AddItemToArray(array, cJSON_CreateBool(fdp->ConsumeBool()));
                break;
            case 3:
                cJSON_AddItemToArray(array, cJSON_CreateNull());
                break;
            case 4:
                {
                    cJSON* obj = cJSON_CreateObject();
                    if (obj != NULL) {
                        std::string key = fdp->ConsumeRandomLengthString(6);
                        cJSON_AddNumberToObject(obj, key.c_str(), fdp->ConsumeFloatingPoint<double>());
                        cJSON_AddItemToArray(array, obj);
                    }
                }
                break;
        }
    }
    
    return array;
}

// Helper to modify a duplicated object to test specific comparison failure scenarios
static void modify_for_comparison_test(cJSON* duplicate, FuzzedDataProvider* fdp, uint8_t modification_type) {
    if (duplicate == NULL) {
        return;
    }
    
    switch (modification_type) {
        case 0:  // Modify a string value to test string comparison
            {
                cJSON* string_item = cJSON_GetObjectItemCaseSensitive(duplicate, "test_string");
                if (string_item == NULL || string_item->type != cJSON_String) {
                    // Add a new string item
                    cJSON_AddStringToObject(duplicate, "test_string", fdp->ConsumeRandomLengthString(16).c_str());
                } else {
                    // Modify existing string
                    free(string_item->valuestring);
                    string_item->valuestring = strdup(fdp->ConsumeRandomLengthString(16).c_str());
                }
            }
            break;
            
        case 1:  // Add an extra key to test subset checking
            {
                std::string extra_key = fdp->ConsumeRandomLengthString(8);
                cJSON_AddNumberToObject(duplicate, extra_key.c_str(), fdp->ConsumeFloatingPoint<double>());
            }
            break;
            
        case 2:  // Remove a key to test subset checking (from other perspective)
            {
                // Find first object key and remove it
                cJSON* child = duplicate->child;
                if (child != NULL && child->string != NULL) {
                    cJSON_DeleteItemFromObjectCaseSensitive(duplicate, child->string);
                }
            }
            break;
            
        case 3:  // Modify a number value
            {
                cJSON* num_item = cJSON_GetObjectItemCaseSensitive(duplicate, "test_number");
                if (num_item == NULL || num_item->type != cJSON_Number) {
                    // Add a new number item
                    cJSON_AddNumberToObject(duplicate, "test_number", fdp->ConsumeFloatingPoint<double>());
                } else {
                    // Modify existing number
                    // Modify existing number using safe API
                    cJSON_SetNumberValue(num_item, fdp->ConsumeFloatingPoint<double>());
                }
            }
            break;
            
        case 4:  // Change item type (e.g., string to number)
            {
                cJSON* item = duplicate->child;
                if (item != NULL) {
                    if (item->type == cJSON_String) {
                        // Change string to number
                        free(item->valuestring);
                        item->valuestring = NULL;
                        // Change string to number using safe API
                        cJSON_SetNumberValue(item, fdp->ConsumeFloatingPoint<double>());
                    } else if (item->type == cJSON_Number) {
                        // Change number to string
                        item->type = cJSON_String;
                        item->valuestring = strdup(fdp->ConsumeRandomLengthString(16).c_str());
                    }
                }
            }
            break;
            
        case 5:  // Test NULL string pointer scenario
            {
                cJSON* string_item = cJSON_GetObjectItemCaseSensitive(duplicate, "null_string_test");
                if (string_item == NULL || string_item->type != cJSON_String) {
                    std::string null_test_str = fdp->ConsumeRandomLengthString(16);
                    string_item = cJSON_AddStringToObject(duplicate, "null_string_test", null_test_str.c_str());
                }
                // Set valuestring to NULL to test NULL string comparison branch
                if (string_item != NULL && string_item->valuestring != NULL) {
                    free(string_item->valuestring);
                    string_item->valuestring = NULL;
                }
            }
            break;
        case 6:  // Modify array length
            {
                cJSON* array_item = cJSON_GetObjectItemCaseSensitive(duplicate, "test_array");
                if (array_item == NULL || array_item->type != cJSON_Array) {
                    // Create array if it doesn't exist
                    array_item = cJSON_CreateArray();
                    cJSON_AddItemToObject(duplicate, "test_array", array_item);
                }
                
                // Add or remove elements to change array length
                if (fdp->ConsumeBool()) {
                    // Add element
                    cJSON_AddItemToArray(array_item, cJSON_CreateNumber(fdp->ConsumeFloatingPoint<double>()));
                } else {
                    // Remove last element if array has elements
                    cJSON* last_child = NULL;
                    cJSON* child = array_item->child;
                    while (child != NULL && child->next != NULL) {
                        last_child = child;
                        child = child->next;
                    }
                    if (last_child != NULL) {
                        last_child->next = NULL;
                        if (child != NULL) {
                            cJSON_Delete(child);
                        }
                    } else if (array_item->child != NULL) {
                        // Only one element
                        cJSON_Delete(array_item->child);
                        array_item->child = NULL;
                    }
                }
            }
            break;
            
        case 7:  // Test case sensitivity with string modification
            {
                cJSON* string_item = cJSON_GetObjectItemCaseSensitive(duplicate, "case_test");
                if (string_item == NULL || string_item->type != cJSON_String) {
                    std::string case_test_str = fdp->ConsumeRandomLengthString(16);
                    string_item = cJSON_AddStringToObject(duplicate, "case_test", case_test_str.c_str());
                }
                // Modify case of first character
                if (string_item != NULL && string_item->valuestring != NULL && 
                    strlen(string_item->valuestring) > 0) {
                    char* str = strdup(string_item->valuestring);
                    if (str[0] >= 'a' && str[0] <= 'z') {
                        str[0] = str[0] - 'a' + 'A';  // To uppercase
                    } else if (str[0] >= 'A' && str[0] <= 'Z') {
                        str[0] = str[0] - 'A' + 'a';  // To lowercase
                    }
                    free(string_item->valuestring);
                    string_item->valuestring = str;
                }
            }
            break;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for complex object creation and modifications
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ============================================================
    // PART 1: Create original test objects
    // ============================================================
    
    // Create complex test object
    cJSON* original_object = create_complex_test_object(&fdp);
    if (original_object == NULL) {
        return 0;
    }
    
    // Create test array
    cJSON* original_array = create_test_array(&fdp);
    
    // Create individual test items
    cJSON* string_item = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
    cJSON* number_item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
    cJSON* bool_item = cJSON_CreateBool(fdp.ConsumeBool());
    cJSON* null_item = cJSON_CreateNull();
    cJSON* raw_item = cJSON_CreateRaw(fdp.ConsumeRandomLengthString(24).c_str());
    
    // ============================================================
    // PART 2: Duplicate and modify for comparison testing
    // ============================================================
    
    // Test 1: Compare object with its exact duplicate (should be true)
    cJSON* duplicate_object = cJSON_Duplicate(original_object, 1);
    if (duplicate_object != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool objects_equal = cJSON_Compare(original_object, duplicate_object, case_sensitive);
        // Should return true for identical objects
        cJSON_Delete(duplicate_object);
    }
    
    // Test 2: Systematic modification testing
    size_t num_modification_tests = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    for (size_t i = 0; i < num_modification_tests && fdp.remaining_bytes() > 0; i++) {
        cJSON* test_duplicate = cJSON_Duplicate(original_object, 1);
        if (test_duplicate != NULL) {
            uint8_t mod_type = fdp.ConsumeIntegral<uint8_t>() % 8;
            modify_for_comparison_test(test_duplicate, &fdp, mod_type);
            
            cJSON_bool case_sensitive = fdp.ConsumeBool();
            cJSON_bool compare_result = cJSON_Compare(original_object, test_duplicate, case_sensitive);
            // Result depends on modification type
            
            cJSON_Delete(test_duplicate);
        }
    }
    
    // Test 3: Array comparison testing
    if (original_array != NULL) {
        // Test identical arrays
        cJSON* duplicate_array = cJSON_Duplicate(original_array, 1);
        if (duplicate_array != NULL) {
            cJSON_bool case_sensitive = fdp.ConsumeBool();
            cJSON_bool arrays_equal = cJSON_Compare(original_array, duplicate_array, case_sensitive);
            // Should return true
            
            // Modify array length and compare again
            if (fdp.remaining_bytes() > 0) {
                // Add an extra element to change array length
                cJSON_AddItemToArray(duplicate_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                cJSON_bool arrays_diff_length = cJSON_Compare(original_array, duplicate_array, case_sensitive);
                // Should return false due to different lengths
            }
            
            cJSON_Delete(duplicate_array);
        }
    }
    
    // Test 4: Individual item comparisons
    // Test identical items
    if (string_item != NULL) {
        cJSON* duplicate_string = cJSON_Duplicate(string_item, 1);
        if (duplicate_string != NULL) {
            cJSON_bool case_sensitive = fdp.ConsumeBool();
            cJSON_bool strings_equal = cJSON_Compare(string_item, duplicate_string, case_sensitive);
            cJSON_Delete(duplicate_string);
        }
    }
    
    if (number_item != NULL) {
        cJSON* duplicate_number = cJSON_Duplicate(number_item, 1);
        if (duplicate_number != NULL) {
            cJSON_bool case_sensitive = fdp.ConsumeBool();
            cJSON_bool numbers_equal = cJSON_Compare(number_item, duplicate_number, case_sensitive);
            cJSON_Delete(duplicate_number);
        }
    }
    
    // Test 5: NULL and type validation testing
    // Test NULL parameters (should return false)
    if (original_object != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool null_vs_object = cJSON_Compare(NULL, original_object, case_sensitive);
        cJSON_bool object_vs_null = cJSON_Compare(original_object, NULL, case_sensitive);
        // Both should return false
    }
    
    // Test 6: Different type comparisons (should return false)
    if (string_item != NULL && number_item != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool different_types = cJSON_Compare(string_item, number_item, case_sensitive);
        // Should return false
    }
    
    // Test 7: Object subset relationships
    if (original_object != NULL && fdp.remaining_bytes() > 0) {
        // Create a subset object by duplicating and removing some keys
        cJSON* subset_object = cJSON_Duplicate(original_object, 1);
        if (subset_object != NULL) {
            // Remove some keys to create a subset
            size_t keys_to_remove = fdp.ConsumeIntegralInRange<size_t>(1, 3);
            for (size_t i = 0; i < keys_to_remove; i++) {
                cJSON* child = subset_object->child;
                if (child != NULL && child->string != NULL) {
                    cJSON_DeleteItemFromObjectCaseSensitive(subset_object, child->string);
                }
            }
            
            cJSON_bool case_sensitive = fdp.ConsumeBool();
            cJSON_bool subset_comparison = cJSON_Compare(subset_object, original_object, case_sensitive);
            // Should return false (subset is not equal to superset)
            cJSON_Delete(subset_object);
        }
    }
    
    // Test 8: Case-sensitive string comparison
    if (string_item != NULL && fdp.remaining_bytes() > 0) {
        cJSON* case_variant = cJSON_Duplicate(string_item, 1);
        if (case_variant != NULL && case_variant->valuestring != NULL) {
            // Create uppercase variant
            std::string upper_str = case_variant->valuestring;
            std::transform(upper_str.begin(), upper_str.end(), upper_str.begin(), ::toupper);
            
            free(case_variant->valuestring);
            case_variant->valuestring = strdup(upper_str.c_str());
            
            // Test with case_sensitive = true (should return false if case differs)
            cJSON_bool case_sensitive_true = cJSON_Compare(string_item, case_variant, true);
            // Test with case_sensitive = false (should return true if only case differs)
            cJSON_bool case_sensitive_false = cJSON_Compare(string_item, case_variant, false);
            
            cJSON_Delete(case_variant);
        }
    }
    
    // ============================================================
    // PART 3: Cleanup
    // ============================================================
    
    if (original_object != NULL) {
        cJSON_Delete(original_object);
    }
    if (original_array != NULL) {
        cJSON_Delete(original_array);
    }
    if (string_item != NULL) {
        cJSON_Delete(string_item);
    }
    if (number_item != NULL) {
        cJSON_Delete(number_item);
    }
    if (bool_item != NULL) {
        cJSON_Delete(bool_item);
    }
    if (null_item != NULL) {
        cJSON_Delete(null_item);
    }
    if (raw_item != NULL) {
        cJSON_Delete(raw_item);
    }
    
    return 0;
}

} // namespace harness_014
