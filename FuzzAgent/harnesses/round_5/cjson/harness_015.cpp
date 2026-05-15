#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <cmath>
#include <cfloat>
#include <algorithm>
#include "cjson/cJSON.h"

namespace harness_015 {
/*
 * Harness_015: Direct internal structure manipulation for comprehensive cJSON_Compare failure testing
 * 
 * Target: cJSON_Compare function with 73/76 branches blocked
 * 
 * SEMANTIC DIFFERENTIATION FROM PREVIOUS HARNESSES (000-014):
 * - Previous harnesses: Use normal API functions (cJSON_Create*, cJSON_Add*, cJSON_Duplicate, cJSON_Parse)
 * - This harness_015: Directly manipulates cJSON internal structures to test edge cases
 *   that normal API usage prevents, specifically targeting all blocked comparison failure paths
 * 
 * Novel approach benefits:
 * 1. Can create invalid type values to test default switch case (line 3096)
 * 2. Can directly set valuestring = NULL to test line 3123 NULL check
 * 3. Can manipulate array child/next pointers to test length mismatch (line 3151)
 * 4. Can test both directions of object key checking thoroughly (lines 3166, 3182)
 * 5. Can directly test case_sensitive parameter with controlled string comparisons
 * 6. Can test recursive comparison failure paths in nested structures
 * 
 * Implementation strategy:
 * 1. Create cJSON items using malloc and direct field manipulation
 * 2. Systematically test each blocked condition in cJSON_Compare
 * 3. Test both case_sensitive=true and false modes
 * 4. Test all type validation failure paths
 * 5. Test array length mismatch in both directions
 * 6. Test object key presence/absence in both directions
 * 7. Test nested structure comparison failures
 */


// Helper to create a cJSON item with direct field manipulation
static cJSON* create_direct_cjson_item(uint8_t type, const char* string_val, double number_val, cJSON* child) {
    cJSON* item = (cJSON*)malloc(sizeof(cJSON));
    if (item == NULL) {
        return NULL;
    }
    memset(item, 0, sizeof(cJSON));
    item->type = type;
    
    if (string_val != NULL) {
        item->valuestring = strdup(string_val);
    }
    item->valuedouble = number_val;
    item->child = child;
    
    return item;
}

// Helper to manually link cJSON items as array elements
static void link_as_array_elements(cJSON* parent, std::vector<cJSON*>& elements) {
    if (elements.empty()) {
        return;
    }
    
    parent->child = elements[0];
    for (size_t i = 0; i < elements.size(); i++) {
        if (i < elements.size() - 1) {
            elements[i]->next = elements[i + 1];
        }
        if (i > 0) {
            elements[i]->prev = elements[i - 1];
        }
    }
}

// Helper to create object with specific key-value pairs (direct manipulation)
static cJSON* create_object_with_keys(FuzzedDataProvider* fdp, const std::vector<std::string>& keys, 
                                     bool set_strings = true) {
    cJSON* obj = create_direct_cjson_item(cJSON_Object, NULL, 0.0, NULL);
    if (obj == NULL || keys.empty()) {
        return obj;
    }
    
    std::vector<cJSON*> key_items;
    for (size_t i = 0; i < keys.size(); i++) {
        cJSON* item = create_direct_cjson_item(cJSON_Number, NULL, fdp->ConsumeFloatingPoint<double>(), NULL);
        if (item != NULL) {
            item->string = strdup(keys[i].c_str());
            key_items.push_back(item);
        }
    }
    
    link_as_array_elements(obj, key_items);
    return obj;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for comprehensive testing
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ============================================================
    // TEST 1: NULL POINTER COMPARISONS (line 3078)
    // ============================================================
    
    // Test NULL vs NULL (should return false)
    cJSON_bool null_vs_null = cJSON_Compare(NULL, NULL, fdp.ConsumeBool());
    
    // Test NULL vs valid item
    cJSON* valid_item = cJSON_CreateNull();
    if (valid_item != NULL) {
        cJSON_bool null_vs_valid = cJSON_Compare(NULL, valid_item, fdp.ConsumeBool());
        cJSON_bool valid_vs_null = cJSON_Compare(valid_item, NULL, fdp.ConsumeBool());
    }
    
    // ============================================================
    // TEST 2: TYPE MISMATCH SCENARIOS (line 3078 type check, line 3096 default case)
    // ============================================================
    
    // Create items with different valid types
    std::vector<cJSON*> different_type_items;
    different_type_items.push_back(cJSON_CreateNull());
    different_type_items.push_back(cJSON_CreateTrue());
    different_type_items.push_back(cJSON_CreateFalse());
    different_type_items.push_back(cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
    different_type_items.push_back(cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str()));
    different_type_items.push_back(cJSON_CreateArray());
    different_type_items.push_back(cJSON_CreateObject());
    
    // Test all pairwise combinations of different types
    for (size_t i = 0; i < different_type_items.size() && i < 4; i++) {
        for (size_t j = i + 1; j < different_type_items.size() && j < 6; j++) {
            if (different_type_items[i] != NULL && different_type_items[j] != NULL) {
                cJSON_bool case_sensitive = fdp.ConsumeBool();
                cJSON_bool result = cJSON_Compare(different_type_items[i], different_type_items[j], case_sensitive);
                // Should return false due to type mismatch
            }
        }
    }
    
    // Test invalid type values (to hit default switch case at line 3096)
    // Create items with type values outside valid cJSON type range
    uint8_t invalid_type1 = fdp.ConsumeIntegralInRange<uint8_t>(cJSON_Invalid, 255);
    uint8_t invalid_type2 = fdp.ConsumeIntegralInRange<uint8_t>(cJSON_Invalid, 255);
    
    cJSON* invalid_item1 = create_direct_cjson_item(invalid_type1, NULL, 0.0, NULL);
    cJSON* invalid_item2 = create_direct_cjson_item(invalid_type2, NULL, 0.0, NULL);
    cJSON* valid_for_invalid_test = cJSON_CreateNull();
    
    if (invalid_item1 != NULL) {
        // Compare invalid type with valid type
        if (valid_for_invalid_test != NULL) {
            cJSON_bool invalid_vs_valid = cJSON_Compare(invalid_item1, valid_for_invalid_test, fdp.ConsumeBool());
            // Should return false (type mismatch check at line 3078)
        }
        
        // Compare two invalid types
        if (invalid_item2 != NULL) {
            cJSON_bool invalid_vs_invalid = cJSON_Compare(invalid_item1, invalid_item2, fdp.ConsumeBool());
            // Should return false (type mismatch check at line 3078 or default case at line 3096)
        }
    }
    
    // ============================================================
    // TEST 3: ARRAY LENGTH MISMATCHES (line 3151: a_element != b_element)
    // ============================================================
    
    // Create arrays of different lengths using direct manipulation
    size_t longer_len = fdp.ConsumeIntegralInRange<size_t>(3, 6);
    size_t shorter_len = longer_len - 1;
    
    // Create longer array
    std::vector<cJSON*> longer_array_elements;
    for (size_t i = 0; i < longer_len; i++) {
        cJSON* elem = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        longer_array_elements.push_back(elem);
    }
    
    cJSON* longer_array = create_direct_cjson_item(cJSON_Array, NULL, 0.0, NULL);
    if (longer_array != NULL && !longer_array_elements.empty()) {
        link_as_array_elements(longer_array, longer_array_elements);
    }
    
    // Create shorter array (subset of elements)
    std::vector<cJSON*> shorter_array_elements;
    for (size_t i = 0; i < shorter_len && i < longer_array_elements.size(); i++) {
        // Duplicate the element for fair comparison
        if (longer_array_elements[i] != NULL) {
            cJSON* dup_elem = cJSON_CreateNumber(longer_array_elements[i]->valuedouble);
            shorter_array_elements.push_back(dup_elem);
        }
    }
    
    cJSON* shorter_array = create_direct_cjson_item(cJSON_Array, NULL, 0.0, NULL);
    if (shorter_array != NULL && !shorter_array_elements.empty()) {
        link_as_array_elements(shorter_array, shorter_array_elements);
    }
    
    // Test array length mismatch
    if (longer_array != NULL && shorter_array != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool arrays_equal = cJSON_Compare(longer_array, shorter_array, case_sensitive);
        // Should return false (different lengths)
        
        cJSON_bool arrays_equal_reverse = cJSON_Compare(shorter_array, longer_array, case_sensitive);
        // Should also return false
    }
    
    // ============================================================
    // TEST 4: OBJECT KEY PRESENCE/ABSENCE VARIATIONS (lines 3166, 3182)
    // ============================================================
    
    // Create objects with overlapping but not identical keys
    std::vector<std::string> common_keys;
    size_t num_common_keys = fdp.ConsumeIntegralInRange<size_t>(2, 4);
    for (size_t i = 0; i < num_common_keys; i++) {
        common_keys.push_back(fdp.ConsumeRandomLengthString(6));
    }
    
    std::vector<std::string> extra_keys_a;
    std::vector<std::string> extra_keys_b;
    
    size_t num_extra_a = fdp.ConsumeIntegralInRange<size_t>(1, 3);
    for (size_t i = 0; i < num_extra_a; i++) {
        extra_keys_a.push_back(fdp.ConsumeRandomLengthString(6));
    }
    
    size_t num_extra_b = fdp.ConsumeIntegralInRange<size_t>(1, 3);
    for (size_t i = 0; i < num_extra_b; i++) {
        extra_keys_b.push_back(fdp.ConsumeRandomLengthString(6));
    }
    
    // Create object A: common_keys + extra_keys_a
    std::vector<std::string> all_keys_a = common_keys;
    all_keys_a.insert(all_keys_a.end(), extra_keys_a.begin(), extra_keys_a.end());
    cJSON* object_a = create_object_with_keys(&fdp, all_keys_a);
    
    // Create object B: common_keys + extra_keys_b  
    std::vector<std::string> all_keys_b = common_keys;
    all_keys_b.insert(all_keys_b.end(), extra_keys_b.begin(), extra_keys_b.end());
    cJSON* object_b = create_object_with_keys(&fdp, all_keys_b);
    
    // Test object comparison with different key sets
    if (object_a != NULL && object_b != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool objects_equal = cJSON_Compare(object_a, object_b, case_sensitive);
        // Should return false (different key sets)
        
        // Test both case_sensitive modes
        cJSON_bool objects_equal_case_true = cJSON_Compare(object_a, object_b, true);
        cJSON_bool objects_equal_case_false = cJSON_Compare(object_a, object_b, false);
    }
    
    // Test subset relationship: object with all keys vs object with subset
    std::vector<std::string> all_keys = common_keys;
    all_keys.insert(all_keys.end(), extra_keys_a.begin(), extra_keys_a.end());
    all_keys.insert(all_keys.end(), extra_keys_b.begin(), extra_keys_b.end());
    
    std::vector<std::string> subset_keys = common_keys;
    subset_keys.insert(subset_keys.end(), extra_keys_a.begin(), extra_keys_a.end());
    
    cJSON* superset_object = create_object_with_keys(&fdp, all_keys);
    cJSON* subset_object = create_object_with_keys(&fdp, subset_keys);
    
    if (superset_object != NULL && subset_object != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool subset_test = cJSON_Compare(superset_object, subset_object, case_sensitive);
        // Should return false (subset is not equal to superset)
        
        cJSON_bool reverse_subset_test = cJSON_Compare(subset_object, superset_object, case_sensitive);
        // Should also return false
    }
    
    // ============================================================
    // TEST 5: NESTED STRUCTURE DIFFERENCES
    // ============================================================
    
    // Create nested objects with subtle differences
    cJSON* nested_object_outer1 = cJSON_CreateObject();
    cJSON* nested_object_outer2 = cJSON_CreateObject();
    
    if (nested_object_outer1 != NULL && nested_object_outer2 != NULL) {
        // Add identical outer properties
        std::string outer_key1 = fdp.ConsumeRandomLengthString(8);
        std::string outer_key2 = fdp.ConsumeRandomLengthString(8);
        
        cJSON_AddNumberToObject(nested_object_outer1, outer_key1.c_str(), fdp.ConsumeFloatingPoint<double>());
        cJSON_AddNumberToObject(nested_object_outer2, outer_key1.c_str(), fdp.ConsumeFloatingPoint<double>());
        
        // Create inner objects with differences
        cJSON* inner_object1 = cJSON_CreateObject();
        cJSON* inner_object2 = cJSON_CreateObject();
        
        if (inner_object1 != NULL && inner_object2 != NULL) {
            // Add same key with different values
            std::string inner_key = fdp.ConsumeRandomLengthString(6);
            cJSON_AddStringToObject(inner_object1, inner_key.c_str(), fdp.ConsumeRandomLengthString(16).c_str());
            cJSON_AddStringToObject(inner_object2, inner_key.c_str(), fdp.ConsumeRandomLengthString(16).c_str());
            
            cJSON_AddItemToObject(nested_object_outer1, outer_key2.c_str(), inner_object1);
            cJSON_AddItemToObject(nested_object_outer2, outer_key2.c_str(), inner_object2);
            
            // Test nested comparison
            cJSON_bool case_sensitive = fdp.ConsumeBool();
            cJSON_bool nested_equal = cJSON_Compare(nested_object_outer1, nested_object_outer2, case_sensitive);
            // Should return false (different inner values)
        }
    }
    
    // ============================================================
    // TEST 6: CASE-SENSITIVE STRING COMPARISON VARIATIONS
    // ============================================================
    
    // Create strings that differ only in case - use fuzzer input for base string
    std::string base_str = fdp.ConsumeRandomLengthString(32);
    if (base_str.empty()) {
        base_str = "a";  // Ensure we have at least one character
    }
    
    // Create case variations from the base string
    std::string lower_str = base_str;
    std::string upper_str = base_str;
    std::string mixed_str = base_str;
    
    // Convert to different cases (simplified - just modify first character)
    if (!lower_str.empty()) {
        // Ensure lower case version
        for (char& c : lower_str) {
            if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
        }
    }
    if (!upper_str.empty()) {
        // Ensure upper case version  
        for (char& c : upper_str) {
            if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
        }
    }
    if (!mixed_str.empty()) {
        // Create mixed case (first char uppercase, rest lowercase)
        if (mixed_str[0] >= 'a' && mixed_str[0] <= 'z') {
            mixed_str[0] = mixed_str[0] - 'a' + 'A';
        }
        for (size_t i = 1; i < mixed_str.size(); i++) {
            if (mixed_str[i] >= 'A' && mixed_str[i] <= 'Z') {
                mixed_str[i] = mixed_str[i] - 'A' + 'a';
            }
        }
    }
    
    cJSON* string_lower = cJSON_CreateString(lower_str.c_str());
    cJSON* string_upper = cJSON_CreateString(upper_str.c_str());
    cJSON* string_mixed = cJSON_CreateString(mixed_str.c_str());
    cJSON* string_identical = cJSON_CreateString(lower_str.c_str());
    
    // Test case-sensitive comparisons
    if (string_lower != NULL && string_upper != NULL) {
        // case_sensitive = true: should be different
        cJSON_bool case_true_result = cJSON_Compare(string_lower, string_upper, true);
        // Should return false
        
        // case_sensitive = false: should be equal (case-insensitive)
        cJSON_bool case_false_result = cJSON_Compare(string_lower, string_upper, false);
        // Might return true if case_insensitive_strcmp is used
    }
    
    if (string_lower != NULL && string_identical != NULL) {
        // Identical strings should be equal regardless of case_sensitive
        cJSON_bool case_true_identical = cJSON_Compare(string_lower, string_identical, true);
        // Should return true
        cJSON_bool case_false_identical = cJSON_Compare(string_lower, string_identical, false);
        // Should return true
    }
    
    // Test NULL valuestring (line 3123)
    cJSON* string_with_null_valuestring = create_direct_cjson_item(cJSON_String, NULL, 0.0, NULL);
    if (string_with_null_valuestring != NULL && string_lower != NULL) {
        cJSON_bool null_string_test = cJSON_Compare(string_with_null_valuestring, string_lower, fdp.ConsumeBool());
        // Should return false (NULL valuestring check)
    }
    
    // Test raw JSON comparison (same logic as strings)
    std::string raw_data = fdp.ConsumeRandomLengthString(64);
    cJSON* raw_item1 = cJSON_CreateRaw(raw_data.c_str());
    cJSON* raw_item2 = cJSON_CreateRaw(raw_data.c_str());
    std::string raw_data_different = fdp.ConsumeRandomLengthString(64);
    cJSON* raw_item3 = cJSON_CreateRaw(raw_data_different.c_str());
    
    if (raw_item1 != NULL && raw_item2 != NULL && raw_item3 != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool raw_equal = cJSON_Compare(raw_item1, raw_item2, case_sensitive);
        // Should return true (identical raw data)
        
        cJSON_bool raw_different = cJSON_Compare(raw_item1, raw_item3, case_sensitive);
        // Should return false (different raw data)
    }
    
    // ============================================================
    // TEST 7: NUMBER COMPARISON EDGE CASES (line 3115 compare_double)
    // ============================================================
    
    // Create numbers with values near DBL_EPSILON threshold
    double base_value = fdp.ConsumeFloatingPoint<double>();
    double slightly_different = base_value * (1.0 + DBL_EPSILON * 2.0);
    double very_different = base_value * 2.0;
    
    cJSON* number_base = cJSON_CreateNumber(base_value);
    cJSON* number_slight_diff = cJSON_CreateNumber(slightly_different);
    cJSON* number_very_diff = cJSON_CreateNumber(very_different);
    
    if (number_base != NULL && number_slight_diff != NULL && number_very_diff != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();  // Ignored for numbers but still needed
        
        cJSON_bool numbers_near_equal = cJSON_Compare(number_base, number_slight_diff, case_sensitive);
        // Depends on compare_double implementation - might be true or false
        
        cJSON_bool numbers_very_diff = cJSON_Compare(number_base, number_very_diff, case_sensitive);
        // Should return false
    }
    
    // ============================================================
    // CLEANUP
    // ============================================================
    
    // Clean up all created items
    if (valid_item != NULL) cJSON_Delete(valid_item);
    
    for (cJSON* item : different_type_items) {
        if (item != NULL) cJSON_Delete(item);
    }
    
    if (invalid_item1 != NULL) {
        free(invalid_item1);
    }
    if (invalid_item2 != NULL) {
        free(invalid_item2);
    }
    if (valid_for_invalid_test != NULL) {
        cJSON_Delete(valid_for_invalid_test);
    }
    
    if (longer_array != NULL) {
        // Only delete first element - cJSON_Delete will follow next chain
        if (!longer_array_elements.empty() && longer_array_elements[0] != NULL) {
            cJSON_Delete(longer_array_elements[0]);
        }
        free(longer_array);
    }
    
    if (shorter_array != NULL) {
        // Only delete first element - cJSON_Delete will follow next chain
        if (!shorter_array_elements.empty() && shorter_array_elements[0] != NULL) {
            cJSON_Delete(shorter_array_elements[0]);
        }
        free(shorter_array);
    }
    
    if (object_a != NULL) {
        // Need to manually free object and its children
        cJSON* child = object_a->child;
        while (child != NULL) {
            cJSON* next = child->next;
            if (child->string != NULL) free(child->string);
            free(child);
            child = next;
        }
        free(object_a);
    }
    
    if (object_b != NULL) {
        cJSON* child = object_b->child;
        while (child != NULL) {
            cJSON* next = child->next;
            if (child->string != NULL) free(child->string);
            free(child);
            child = next;
        }
        free(object_b);
    }
    
    if (superset_object != NULL) {
        cJSON* child = superset_object->child;
        while (child != NULL) {
            cJSON* next = child->next;
            if (child->string != NULL) free(child->string);
            free(child);
            child = next;
        }
        free(superset_object);
    }
    
    if (subset_object != NULL) {
        cJSON* child = subset_object->child;
        while (child != NULL) {
            cJSON* next = child->next;
            if (child->string != NULL) free(child->string);
            free(child);
            child = next;
        }
        free(subset_object);
    }
    
    if (nested_object_outer1 != NULL) cJSON_Delete(nested_object_outer1);
    if (nested_object_outer2 != NULL) cJSON_Delete(nested_object_outer2);
    
    if (string_lower != NULL) cJSON_Delete(string_lower);
    if (string_upper != NULL) cJSON_Delete(string_upper);
    if (string_mixed != NULL) cJSON_Delete(string_mixed);
    if (string_identical != NULL) cJSON_Delete(string_identical);
    if (string_with_null_valuestring != NULL) free(string_with_null_valuestring);
    
    if (raw_item1 != NULL) cJSON_Delete(raw_item1);
    if (raw_item2 != NULL) cJSON_Delete(raw_item2);
    if (raw_item3 != NULL) cJSON_Delete(raw_item3);
    
    if (number_base != NULL) cJSON_Delete(number_base);
    if (number_slight_diff != NULL) cJSON_Delete(number_slight_diff);
    if (number_very_diff != NULL) cJSON_Delete(number_very_diff);
    
    return 0;
}

} // namespace harness_015
