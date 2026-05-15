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

namespace harness_017 {

extern "C" {
}

// Helper function to create complex nested JSON structures with controlled differences
cJSON* create_complex_structure(FuzzedDataProvider& fdp, int depth, bool make_different) {
    if (depth <= 0 || fdp.remaining_bytes() < 16) {
        // Create leaf nodes
        uint8_t type = fdp.ConsumeIntegralInRange<uint8_t>(0, 5);
        switch (type) {
            case 0:
                return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            case 1:
                return cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
            case 2:
                return fdp.ConsumeBool() ? cJSON_CreateTrue() : cJSON_CreateFalse();
            case 3:
                return cJSON_CreateNull();
            case 4:
                return cJSON_CreateRaw(fdp.ConsumeRandomLengthString(32).c_str());
            case 5:
                return cJSON_CreateArray(); // Empty array as leaf
            default:
                return cJSON_CreateNull();
        }
    }
    
    uint8_t structure_type = fdp.ConsumeIntegralInRange<uint8_t>(0, 2);
    
    if (structure_type == 0) {
        // Create nested array
        cJSON* array = cJSON_CreateArray();
        if (!array) return NULL;
        
        int item_count = fdp.ConsumeIntegralInRange<int>(1, 4);
        for (int i = 0; i < item_count && fdp.remaining_bytes() > 0; i++) {
            cJSON* child = create_complex_structure(fdp, depth - 1, make_different && (i == item_count - 1));
            if (child) {
                cJSON_AddItemToArray(array, child);
            }
        }
        return array;
    } else if (structure_type == 1) {
        // Create nested object
        cJSON* object = cJSON_CreateObject();
        if (!object) return NULL;
        
        int key_count = fdp.ConsumeIntegralInRange<int>(1, 5);
        for (int i = 0; i < key_count && fdp.remaining_bytes() > 0; i++) {
            std::string key = fdp.ConsumeRandomLengthString(16);
            bool make_this_different = make_different && (i == key_count - 1);
            
            if (make_this_different && fdp.ConsumeBool()) {
                // Make this key/value pair different in the comparison
                // Skip adding this key or add different value
                continue;
            }
            
            cJSON* child = create_complex_structure(fdp, depth - 1, make_this_different);
            if (child) {
                cJSON_AddItemToObject(object, key.c_str(), child);
            }
        }
        return object;
    } else {
        // Create mixed array
        cJSON* array = cJSON_CreateArray();
        if (!array) return NULL;
        
        // Add different types to exercise type checking in comparisons
        array->child = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        if (array->child) {
            array->child->next = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
            if (array->child->next) {
                array->child->next->next = fdp.ConsumeBool() ? cJSON_CreateTrue() : cJSON_CreateFalse();
                if (array->child->next->next) {
                    array->child->next->next->next = cJSON_CreateArray();
                }
            }
        }
        return array;
    }
}

// Helper to create numeric edge cases
cJSON* create_number_edge_case(FuzzedDataProvider& fdp) {
    uint8_t case_type = fdp.ConsumeIntegralInRange<uint8_t>(0, 7);
    
    switch (case_type) {
        case 0:
            return cJSON_CreateNumber(std::numeric_limits<double>::quiet_NaN());
        case 1:
            return cJSON_CreateNumber(std::numeric_limits<double>::infinity());
        case 2:
            return cJSON_CreateNumber(-std::numeric_limits<double>::infinity());
        case 3:
            return cJSON_CreateNumber(std::numeric_limits<double>::max());
        case 4:
            return cJSON_CreateNumber(std::numeric_limits<double>::min());
        case 5:
            return cJSON_CreateNumber(std::numeric_limits<double>::epsilon());
        case 6:
            return cJSON_CreateNumber(0.0);
        case 7:
            return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        default:
            return cJSON_CreateNumber(0.0);
    }
}

// Helper to create string edge cases
cJSON* create_string_edge_case(FuzzedDataProvider& fdp) {
    uint8_t case_type = fdp.ConsumeIntegralInRange<uint8_t>(0, 6);
    
    switch (case_type) {
        case 0:
            return cJSON_CreateString("");  // Empty string - keep as edge case
        case 1: {
            std::string with_null = fdp.ConsumeRandomLengthString(16);
            if (!with_null.empty()) {
                with_null[0] = '\0';
            }
            return cJSON_CreateString(with_null.c_str());  // String with null byte
        }
        case 2: {
            std::string embedded_null = fdp.ConsumeRandomLengthString(20);
            if (embedded_null.size() > 5) {
                embedded_null[embedded_null.size() / 2] = '\0';
            }
            return cJSON_CreateString(embedded_null.c_str());  // String with embedded null
        }
        case 3: {
            std::string upper = fdp.ConsumeRandomLengthString(20);
            std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
            return cJSON_CreateString(upper.c_str());  // All uppercase
        }
        case 4: {
            std::string lower = fdp.ConsumeRandomLengthString(20);
            std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
            return cJSON_CreateString(lower.c_str());  // All lowercase
        }
        case 5: {
            std::string mixed = fdp.ConsumeRandomLengthString(20);
            if (!mixed.empty()) {
                mixed[0] = toupper(mixed[0]);
                for (size_t i = 1; i < mixed.size(); i++) {
                    mixed[i] = tolower(mixed[i]);
                }
            }
            return cJSON_CreateString(mixed.c_str());  // Mixed case
        }
        case 6: {
            std::string special = fdp.ConsumeRandomLengthString(32);
            // Add some special characters
            if (!special.empty() && fdp.ConsumeBool()) {
                special[special.size() / 2] = '\n';
            }
            return cJSON_CreateString(special.c_str());
        }
        default: {
            std::string default_str = fdp.ConsumeRandomLengthString(16);
            return cJSON_CreateString(default_str.c_str());
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 256) {
        return 0;  // Need sufficient input for comprehensive structural comparison testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters
    uint8_t test_category = fdp.ConsumeIntegralInRange<uint8_t>(0, 4);
    bool case_sensitive = fdp.ConsumeBool();
    
    // Test Category 0: Complex nested structure comparisons with subtle differences
    if (test_category == 0) {
        // Create two similar but different complex structures
        int depth = fdp.ConsumeIntegralInRange<int>(1, 5);
        
        cJSON* struct1 = create_complex_structure(fdp, depth, false);
        cJSON* struct2 = create_complex_structure(fdp, depth, true);  // Second one may have differences
        
        if (struct1 && struct2) {
            // Test comparison in both directions
            cJSON_bool result1 = cJSON_Compare(struct1, struct2, case_sensitive);
            cJSON_bool result2 = cJSON_Compare(struct2, struct1, case_sensitive);
            
            // Also compare each with itself (should be true)
            cJSON_bool result3 = cJSON_Compare(struct1, struct1, case_sensitive);
            cJSON_bool result4 = cJSON_Compare(struct2, struct2, case_sensitive);
        }
        
        if (struct1) cJSON_Delete(struct1);
        if (struct2) cJSON_Delete(struct2);
        
        return 0;
    }
    
    // Test Category 1: Array ordering and subset/superset comparisons
    if (test_category == 1) {
        // Create arrays with same elements in different orders
        cJSON* array1 = cJSON_CreateArray();
        cJSON* array2 = cJSON_CreateArray();
        
        if (array1 && array2) {
            int element_count = fdp.ConsumeIntegralInRange<int>(2, 8);
            std::vector<cJSON*> elements;
            
            // Create elements
            for (int i = 0; i < element_count && fdp.remaining_bytes() > 0; i++) {
                cJSON* elem = create_complex_structure(fdp, 2, false);
                if (elem) {
                    elements.push_back(elem);
                    cJSON_AddItemToArray(array1, elem);
                }
            }
            
            // Add elements to array2 in potentially different order
            if (!elements.empty()) {
                if (fdp.ConsumeBool()) {
                    // Same order
                    for (auto elem : elements) {
                        cJSON* copy = NULL;
                        if (elem->type == cJSON_Number) {
                            copy = cJSON_CreateNumber(elem->valuedouble);
                        } else if (elem->type == cJSON_String && elem->valuestring) {
                            copy = cJSON_CreateString(elem->valuestring);
                        } else if (elem->type == cJSON_True) {
                            copy = cJSON_CreateTrue();
                        } else if (elem->type == cJSON_False) {
                            copy = cJSON_CreateFalse();
                        } else if (elem->type == cJSON_NULL) {
                            copy = cJSON_CreateNull();
                        }
                        if (copy) {
                            cJSON_AddItemToArray(array2, copy);
                        }
                    }
                } else {
                    // Different order or subset
                    for (size_t i = 0; i < elements.size(); i++) {
                        if (fdp.ConsumeBool() || i == elements.size() - 1) {  // Skip some elements
                            cJSON* copy = NULL;
                            if (elements[i]->type == cJSON_Number) {
                                copy = cJSON_CreateNumber(elements[i]->valuedouble);
                            } else if (elements[i]->type == cJSON_String && elements[i]->valuestring) {
                                copy = cJSON_CreateString(elements[i]->valuestring);
                            }
                            if (copy) {
                                cJSON_AddItemToArray(array2, copy);
                            }
                        }
                    }
                }
            }
            
            // Compare arrays
            cJSON_bool result = cJSON_Compare(array1, array2, case_sensitive);
            
            // Test array length mismatch
            cJSON* longer_array = cJSON_CreateArray();
            if (longer_array) {
                for (int i = 0; i < 3; i++) {
                    cJSON_AddItemToArray(longer_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                }
                cJSON_bool length_mismatch_result = cJSON_Compare(array1, longer_array, case_sensitive);
                cJSON_Delete(longer_array);
            }
        }
        
        if (array1) cJSON_Delete(array1);
        if (array2) cJSON_Delete(array2);
        
        return 0;
    }
    
    // Test Category 2: Object key subset/superset and case sensitivity
    if (test_category == 2) {
        cJSON* obj1 = cJSON_CreateObject();
        cJSON* obj2 = cJSON_CreateObject();
        
        if (obj1 && obj2) {
            // Create base set of keys
            std::vector<std::string> keys;
            int key_count = fdp.ConsumeIntegralInRange<int>(3, 8);
            
            for (int i = 0; i < key_count && fdp.remaining_bytes() > 0; i++) {
                keys.push_back(fdp.ConsumeRandomLengthString(12));
            }
            
            // Add keys to obj1
            for (size_t i = 0; i < keys.size(); i++) {
                std::string value_key = "value_" + std::to_string(i);
                cJSON_AddNumberToObject(obj1, keys[i].c_str(), fdp.ConsumeFloatingPoint<double>());
            }
            
            // Add keys to obj2 with potential differences
            for (size_t i = 0; i < keys.size(); i++) {
                if (fdp.ConsumeBool() || i == keys.size() - 1) {  // Skip some keys for subset test
                    // Possibly change case for case sensitivity test
                    std::string modified_key = keys[i];
                    if (fdp.ConsumeBool() && !modified_key.empty()) {
                        if (islower(modified_key[0])) {
                            modified_key[0] = toupper(modified_key[0]);
                        } else {
                            modified_key[0] = tolower(modified_key[0]);
                        }
                    }
                    
                    double value = fdp.ConsumeFloatingPoint<double>();
                    // Possibly change value for same-key-different-value test
                    if (fdp.ConsumeBool()) {
                        value += 1.0;
                    }
                    
                    cJSON_AddNumberToObject(obj2, modified_key.c_str(), value);
                }
            }
            
            // Add extra key to obj2 for superset test
            if (fdp.ConsumeBool()) {
                std::string extra_key2 = "extra_key_" + fdp.ConsumeRandomLengthString(8);
                std::string extra_value2 = "extra_value_" + fdp.ConsumeRandomLengthString(8);
                cJSON_AddStringToObject(obj2, extra_key2.c_str(), extra_value2.c_str());
            }
            
            // Add extra key to obj1 for different-superset test
            if (fdp.ConsumeBool()) {
                std::string extra_key1 = "extra_key_" + fdp.ConsumeRandomLengthString(8);
                std::string extra_value1 = "extra_value_" + fdp.ConsumeRandomLengthString(8);
                cJSON_AddStringToObject(obj1, extra_key1.c_str(), extra_value1.c_str());
            }
            
            // Perform comparisons
            cJSON_bool result1 = cJSON_Compare(obj1, obj2, case_sensitive);
            cJSON_bool result2 = cJSON_Compare(obj2, obj1, case_sensitive);
            
            // Test with swapped case_sensitive flag
            cJSON_bool result3 = cJSON_Compare(obj1, obj2, !case_sensitive);
        }
        
        if (obj1) cJSON_Delete(obj1);
        if (obj2) cJSON_Delete(obj2);
        
        return 0;
    }
    
    // Test Category 3: Numeric and string edge cases
    if (test_category == 3) {
        // Test numeric edge cases
        cJSON* num1 = create_number_edge_case(fdp);
        cJSON* num2 = create_number_edge_case(fdp);
        
        if (num1 && num2) {
            cJSON_bool num_result = cJSON_Compare(num1, num2, case_sensitive);
            
            // Test number vs non-number type
            cJSON* str = create_string_edge_case(fdp);
            if (str) {
                cJSON_bool type_mismatch_result = cJSON_Compare(num1, str, case_sensitive);
                cJSON_Delete(str);
            }
        }
        
        // Test string edge cases
        cJSON* str1 = create_string_edge_case(fdp);
        cJSON* str2 = create_string_edge_case(fdp);
        
        if (str1 && str2) {
            cJSON_bool str_result = cJSON_Compare(str1, str2, case_sensitive);
            // Test case sensitivity with similar strings
            std::string base_str = fdp.ConsumeRandomLengthString(20);
            if (base_str.empty()) base_str = "default";
            
            std::string lower_str = base_str;
            std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
            
            std::string upper_str = base_str;
            std::transform(upper_str.begin(), upper_str.end(), upper_str.begin(), ::toupper);
            
            std::string mixed_str = base_str;
            if (!mixed_str.empty()) {
                mixed_str[0] = toupper(mixed_str[0]);
                for (size_t i = 1; i < mixed_str.size(); i++) {
                    mixed_str[i] = tolower(mixed_str[i]);
                }
            }
            
            cJSON* lower = cJSON_CreateString(lower_str.c_str());
            cJSON* upper = cJSON_CreateString(upper_str.c_str());
            cJSON* mixed = cJSON_CreateString(mixed_str.c_str());
            
            if (lower && upper && mixed) {
                cJSON_bool case_sensitive_result1 = cJSON_Compare(lower, upper, true);   // Should be false
                cJSON_bool case_sensitive_result2 = cJSON_Compare(lower, upper, false);  // Might be true depending on implementation
                cJSON_bool case_sensitive_result3 = cJSON_Compare(lower, mixed, true);   // Should be false
                cJSON_bool case_sensitive_result4 = cJSON_Compare(lower, mixed, false);  // Might be true
            }
            
            if (lower) cJSON_Delete(lower);
            if (upper) cJSON_Delete(upper);
            if (mixed) cJSON_Delete(mixed);
        }
        
        // Test raw JSON comparisons
        cJSON* raw1 = cJSON_CreateRaw(fdp.ConsumeRandomLengthString(32).c_str());
        cJSON* raw2 = cJSON_CreateRaw(fdp.ConsumeRandomLengthString(32).c_str());
        
        // Create identical raw JSON strings for equality test
        std::string same_raw_content = fdp.ConsumeRandomLengthString(24);
        cJSON* raw3 = cJSON_CreateRaw(same_raw_content.c_str());
        cJSON* raw4 = cJSON_CreateRaw(same_raw_content.c_str());
        
        if (raw1 && raw2) {
            cJSON_bool raw_result1 = cJSON_Compare(raw1, raw2, case_sensitive);
        }
        if (raw3 && raw4) {
            cJSON_bool raw_result2 = cJSON_Compare(raw3, raw4, case_sensitive);
        }
        // Cleanup
        if (num1) cJSON_Delete(num1);
        if (num2) cJSON_Delete(num2);
        if (str1) cJSON_Delete(str1);
        if (str2) cJSON_Delete(str2);
        if (raw1) cJSON_Delete(raw1);
        if (raw2) cJSON_Delete(raw2);
        if (raw3) cJSON_Delete(raw3);
        if (raw4) cJSON_Delete(raw4);
        
        return 0;
    }
    
    // Test Category 4: Special cases and error conditions
    if (test_category == 4) {
        // Test NULL comparisons
        cJSON_bool null_vs_null = cJSON_Compare(NULL, NULL, case_sensitive);
        
        cJSON* valid_obj = cJSON_CreateObject();
        if (valid_obj) {
            cJSON_AddNumberToObject(valid_obj, "test", 42.0);
            
            cJSON_bool null_vs_valid = cJSON_Compare(NULL, valid_obj, case_sensitive);
            cJSON_bool valid_vs_null = cJSON_Compare(valid_obj, NULL, case_sensitive);
            
            // Test identical object comparison (should be true)
            cJSON_bool identical_result = cJSON_Compare(valid_obj, valid_obj, case_sensitive);
            
            // Test invalid type objects (if we can create them)
            cJSON* invalid_obj = (cJSON*)cJSON_malloc(sizeof(cJSON));
            if (invalid_obj) {
                memset(invalid_obj, 0, sizeof(cJSON));
                invalid_obj->type = 0xFF;  // Invalid type
                
                cJSON_bool invalid_vs_valid = cJSON_Compare(invalid_obj, valid_obj, case_sensitive);
                cJSON_bool invalid_vs_invalid = cJSON_Compare(invalid_obj, invalid_obj, case_sensitive);
                
                cJSON_free(invalid_obj);
            }
            
            cJSON_Delete(valid_obj);
        }
        
        // Test maximum nesting depth (to potentially hit recursion limits)
        cJSON* deeply_nested = cJSON_CreateArray();
        cJSON* current = deeply_nested;
        int max_depth = fdp.ConsumeIntegralInRange<int>(10, 50);
        
        for (int i = 0; i < max_depth && current; i++) {
            cJSON* new_array = cJSON_CreateArray();
            if (new_array) {
                cJSON_AddItemToArray(current, new_array);
                current = new_array;
            } else {
                break;
            }
        }
        
        // Create similar but not identical deeply nested structure
        cJSON* deeply_nested2 = cJSON_CreateArray();
        cJSON* current2 = deeply_nested2;
        
        for (int i = 0; i < max_depth - 1 && current2; i++) {  // One less level
            cJSON* new_array = cJSON_CreateArray();
            if (new_array) {
                cJSON_AddItemToArray(current2, new_array);
                current2 = new_array;
            } else {
                break;
            }
        }
        
        if (deeply_nested && deeply_nested2) {
            cJSON_bool deep_comparison = cJSON_Compare(deeply_nested, deeply_nested2, case_sensitive);
        }
        
        if (deeply_nested) cJSON_Delete(deeply_nested);
        if (deeply_nested2) cJSON_Delete(deeply_nested2);
        
        return 0;
    }
    
    return 0;
}

} // namespace harness_017
