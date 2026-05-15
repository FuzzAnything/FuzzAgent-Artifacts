#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include "cjson/cJSON.h"

namespace harness_010 {
/*
 * Harness_010: Systematic cJSON_Compare edge case testing for remaining blocked branches
 * 
 * Target: Specifically targets the 73 blocked branches (96% blocked) in cJSON_Compare
 * identified as the single biggest coverage gap despite 1650 calls.
 * 
 * This harness systematically covers the 6 specific edge case categories identified
 * in the coverage guidance for harness_010:
 * 1. Type mismatch comparisons (all type combinations)
 * 2. Deep nested object comparisons with missing keys  
 * 3. Array comparisons with different element ordering
 * 4. String comparisons with NULL valuestring
 * 5. Raw JSON comparisons
 * 6. Case-sensitive vs case-insensitive string comparisons
 * 
 * Semantic differentiation from previous harnesses (005-008):
 * - harness_005: General comparison testing with diverse structures
 * - harness_006: Focused on blocked branches with specific conditions  
 * - harness_007: Systematic organization by comparison category
 * - harness_008: Targeted edge cases with NULL valuestring and invalid types
 * - harness_010: Systematic coverage of ALL 6 guidance categories with methodical
 *                testing of each blocked branch location
 */



// Helper to create deep nested objects for missing key testing
static cJSON* create_deep_nested_object(FuzzedDataProvider& fdp, int depth, const std::vector<std::string>& mandatory_keys, const std::vector<std::string>& optional_keys) {
    if (depth <= 0) {
        // Leaf node - create a simple value
        uint8_t leaf_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (leaf_type) {
            case 0: return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            case 1: return cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
            case 2: return cJSON_CreateBool(fdp.ConsumeBool());
            case 3: return cJSON_CreateNull();
            default: return cJSON_CreateNull();
        }
    }
    
    cJSON* obj = cJSON_CreateObject();
    if (obj == NULL) return NULL;
    
    // Add all mandatory keys
    for (const auto& key : mandatory_keys) {
        cJSON* value = create_deep_nested_object(fdp, depth - 1, {}, {});
        if (value != NULL) {
            cJSON_AddItemToObject(obj, key.c_str(), value);
        }
    }
    
    // Add some optional keys (random subset)
    size_t num_optional = fdp.ConsumeIntegralInRange<size_t>(0, optional_keys.size());
    for (size_t i = 0; i < num_optional; i++) {
        size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, optional_keys.size() - 1);
        std::string key = optional_keys[idx];
        cJSON* value = create_deep_nested_object(fdp, depth - 1, {}, {});
        if (value != NULL) {
            cJSON_AddItemToObject(obj, key.c_str(), value);
        }
    }
    
    return obj;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for comprehensive edge case testing
    // Testing 6 categories with multiple scenarios each
    if (size < 512) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // ============================================================
    // CATEGORY 1: TYPE MISMATCH COMPARISONS (all type combinations)
    // ============================================================
    // Tests line 3078: if ((a == NULL) || (b == NULL) || ((a->type & 0xFF) != (b->type & 0xFF)))
    // Tests default switch cases for invalid types
    
    // Create all JSON types for type mismatch testing
    cJSON* null_item = cJSON_CreateNull();
    cJSON* true_item = cJSON_CreateTrue();
    cJSON* false_item = cJSON_CreateFalse();
    cJSON* number_item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
    cJSON* string_item = cJSON_CreateString(fdp.ConsumeRandomLengthString(64).c_str());
    cJSON* raw_item = cJSON_CreateRaw(fdp.ConsumeRandomLengthString(48).c_str());
    cJSON* array_item = cJSON_CreateArray();
    cJSON* object_item = cJSON_CreateObject();
    
    // Create a simple array for testing
    if (array_item != NULL) {
        for (int i = 0; i < 3; i++) {
            cJSON_AddItemToArray(array_item, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
        }
    }
    
    // Create a simple object for testing
    if (object_item != NULL) {
        cJSON_AddStringToObject(object_item, "key1", fdp.ConsumeRandomLengthString(16).c_str());
        cJSON_AddNumberToObject(object_item, "key2", fdp.ConsumeFloatingPoint<double>());
    }
    
    // Test all type combinations - should all return false (type mismatch)
    std::vector<cJSON*> all_types = {null_item, true_item, false_item, number_item, 
                                     string_item, raw_item, array_item, object_item};
    
    for (size_t i = 0; i < all_types.size(); i++) {
        for (size_t j = i + 1; j < all_types.size(); j++) {
            if (all_types[i] != NULL && all_types[j] != NULL) {
                cJSON_bool case_sensitive = fdp.ConsumeBool();
                cJSON_bool result = cJSON_Compare(all_types[i], all_types[j], case_sensitive);
                // Should return false due to type mismatch
            }
        }
    }
    
    // ============================================================
    // CATEGORY 2: DEEP NESTED OBJECT COMPARISONS WITH MISSING KEYS
    // ============================================================
    // Tests lines 3166-3169 and 3182-3185: get_object_item returns NULL for missing keys
    
    // Create key sets for nested object testing
    std::vector<std::string> common_keys;
    std::vector<std::string> extra_keys_a;
    std::vector<std::string> extra_keys_b;
    
    size_t num_common = fdp.ConsumeIntegralInRange<size_t>(2, 4);
    size_t num_extra_a = fdp.ConsumeIntegralInRange<size_t>(1, 3);
    size_t num_extra_b = fdp.ConsumeIntegralInRange<size_t>(1, 3);
    
    for (size_t i = 0; i < num_common; i++) {
        common_keys.push_back(fdp.ConsumeRandomLengthString(8));
    }
    for (size_t i = 0; i < num_extra_a; i++) {
        extra_keys_a.push_back(fdp.ConsumeRandomLengthString(8));
    }
    for (size_t i = 0; i < num_extra_b; i++) {
        extra_keys_b.push_back(fdp.ConsumeRandomLengthString(8));
    }
    
    // Create object A with common_keys + extra_keys_a
    std::vector<std::string> keys_a = common_keys;
    keys_a.insert(keys_a.end(), extra_keys_a.begin(), extra_keys_a.end());
    
    // Create object B with common_keys + extra_keys_b  
    std::vector<std::string> keys_b = common_keys;
    keys_b.insert(keys_b.end(), extra_keys_b.begin(), extra_keys_b.end());
    
    // Create nested objects (depth 2-3)
    int depth = fdp.ConsumeIntegralInRange<int>(2, 3);
    cJSON* nested_obj_a = create_deep_nested_object(fdp, depth, keys_a, {});
    cJSON* nested_obj_b = create_deep_nested_object(fdp, depth, keys_b, {});
    
    // Also create objects where B has subset of A's keys
    std::vector<std::string> all_keys = common_keys;
    all_keys.insert(all_keys.end(), extra_keys_a.begin(), extra_keys_a.end());
    all_keys.insert(all_keys.end(), extra_keys_b.begin(), extra_keys_b.end());
    
    // Object with all keys
    cJSON* obj_all_keys = create_deep_nested_object(fdp, 1, all_keys, {});
    // Object with only common keys
    cJSON* obj_common_only = create_deep_nested_object(fdp, 1, common_keys, {});
    
    if (nested_obj_a != NULL && nested_obj_b != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool result1 = cJSON_Compare(nested_obj_a, nested_obj_b, case_sensitive);
        // Should return false (different key sets)
    }
    
    if (obj_all_keys != NULL && obj_common_only != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool result2 = cJSON_Compare(obj_all_keys, obj_common_only, case_sensitive);
        // Should return false (subset relationship)
        cJSON_bool result3 = cJSON_Compare(obj_common_only, obj_all_keys, case_sensitive);
        // Should also return false (superset relationship)
    }
    
    // ============================================================
    // CATEGORY 3: ARRAY COMPARISONS WITH DIFFERENT ELEMENT ORDERING
    // ============================================================
    // Tests array comparison logic lines 3139-3156
    
    // Create arrays with same elements in different order
    cJSON* array_ordered = cJSON_CreateArray();
    cJSON* array_reversed = cJSON_CreateArray();
    cJSON* array_shuffled = cJSON_CreateArray();
    
    if (array_ordered != NULL && array_reversed != NULL && array_shuffled != NULL) {
        std::vector<double> values;
        size_t array_size = fdp.ConsumeIntegralInRange<size_t>(3, 6);
        
        for (size_t i = 0; i < array_size; i++) {
            double val = fdp.ConsumeFloatingPoint<double>();
            values.push_back(val);
            
            // Add to ordered array
            cJSON_AddItemToArray(array_ordered, cJSON_CreateNumber(val));
        }
        
        // Add to reversed array
        for (auto it = values.rbegin(); it != values.rend(); ++it) {
            cJSON_AddItemToArray(array_reversed, cJSON_CreateNumber(*it));
        }
        
        // Shuffle values for shuffled array using Fisher-Yates algorithm
        std::vector<double> shuffled = values;
        // Simple shuffle: swap each element with a random later element
        for (size_t i = 0; i < shuffled.size(); i++) {
            size_t j = i + (fdp.ConsumeIntegral<uint32_t>() % (shuffled.size() - i));
            std::swap(shuffled[i], shuffled[j]);
        }
        for (double val : shuffled) {
            cJSON_AddItemToArray(array_shuffled, cJSON_CreateNumber(val));
        }
        
        // Test comparisons
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool ordered_vs_reversed = cJSON_Compare(array_ordered, array_reversed, case_sensitive);
        // Should return false (different order)
        
        cJSON_bool ordered_vs_shuffled = cJSON_Compare(array_ordered, array_shuffled, case_sensitive);
        // Should return false (different order)
        
        // Also test arrays with same values but one has extra element
        cJSON* array_extra = cJSON_CreateArray();
        if (array_extra != NULL) {
            for (double val : values) {
                cJSON_AddItemToArray(array_extra, cJSON_CreateNumber(val));
            }
            // Add one extra element
            cJSON_AddItemToArray(array_extra, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
            
            cJSON_bool ordered_vs_extra = cJSON_Compare(array_ordered, array_extra, case_sensitive);
            // Should return false (different lengths, tests line 3151)
        }
    }
    
    // ============================================================
    // CATEGORY 4: STRING COMPARISONS WITH NULL VALUESTRING
    // ============================================================
    // Tests lines 3123-3126: if ((a->valuestring == NULL) || (b->valuestring == NULL))
    
    // Create string with potentially NULL valuestring
    cJSON* string_normal = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
    
    // Create string with empty string (not NULL but tests edge case)
    cJSON* string_empty = cJSON_CreateString("");
    
    // Manually create cJSON string with NULL valuestring
    cJSON* string_null_valuestring = (cJSON*)malloc(sizeof(cJSON));
    if (string_null_valuestring != NULL) {
        memset(string_null_valuestring, 0, sizeof(cJSON));
        string_null_valuestring->type = cJSON_String;
        string_null_valuestring->valuestring = NULL;  // Explicitly NULL
    }
    
    if (string_normal != NULL && string_null_valuestring != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool result1 = cJSON_Compare(string_normal, string_null_valuestring, case_sensitive);
        // Should return false (NULL valuestring)
        
        cJSON_bool result2 = cJSON_Compare(string_null_valuestring, string_normal, case_sensitive);
        // Should also return false
    }
    
    if (string_null_valuestring != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool result3 = cJSON_Compare(string_null_valuestring, string_null_valuestring, case_sensitive);
        // Should return false (both NULL valuestring)
    }
    
    // ============================================================
    // CATEGORY 5: RAW JSON COMPARISONS
    // ============================================================
    // Tests same logic as strings (lines 3123-3132) but for cJSON_Raw type
    
    cJSON* raw_normal = cJSON_CreateRaw(fdp.ConsumeRandomLengthString(48).c_str());
    
    // Manually create cJSON raw with NULL valuestring
    cJSON* raw_null_valuestring = (cJSON*)malloc(sizeof(cJSON));
    if (raw_null_valuestring != NULL) {
        memset(raw_null_valuestring, 0, sizeof(cJSON));
        raw_null_valuestring->type = cJSON_Raw;
        raw_null_valuestring->valuestring = NULL;  // Explicitly NULL
    }
    
    // Create two raw items with same/different content
    std::string raw_content1 = fdp.ConsumeRandomLengthString(32);
    std::string raw_content2 = fdp.ConsumeRandomLengthString(32);
    cJSON* raw_same1 = cJSON_CreateRaw(raw_content1.c_str());
    cJSON* raw_same2 = cJSON_CreateRaw(raw_content1.c_str());  // Same content
    cJSON* raw_diff = cJSON_CreateRaw(raw_content2.c_str());   // Different content
    
    if (raw_normal != NULL && raw_null_valuestring != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool result1 = cJSON_Compare(raw_normal, raw_null_valuestring, case_sensitive);
        // Should return false
        
        cJSON_bool result2 = cJSON_Compare(raw_null_valuestring, raw_normal, case_sensitive);
        // Should also return false
    }
    
    if (raw_same1 != NULL && raw_same2 != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool result3 = cJSON_Compare(raw_same1, raw_same2, case_sensitive);
        // Should return true (identical raw content)
    }
    
    if (raw_same1 != NULL && raw_diff != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool result4 = cJSON_Compare(raw_same1, raw_diff, case_sensitive);
        // Should return false (different raw content)
    }
    
    // ============================================================
    // CATEGORY 6: CASE-SENSITIVE VS CASE-INSENSITIVE STRING COMPARISONS
    // ============================================================
    // Tests string comparison with case_sensitive parameter affecting get_object_item calls
    
    // Create strings with case variations
    std::string lower_str = "teststring";
    std::string upper_str = "TESTSTRING";
    std::string mixed_str = "TestString";
    
    cJSON* str_lower = cJSON_CreateString(lower_str.c_str());
    cJSON* str_upper = cJSON_CreateString(upper_str.c_str());
    cJSON* str_mixed = cJSON_CreateString(mixed_str.c_str());
    
    // Create objects with case-variant keys for case-sensitive testing
    cJSON* obj_case_variants = cJSON_CreateObject();
    cJSON* obj_same_case = cJSON_CreateObject();
    
    if (obj_case_variants != NULL && obj_same_case != NULL) {
        // Add same key with different cases
        cJSON_AddNumberToObject(obj_case_variants, "keyName", 1.0);
        cJSON_AddNumberToObject(obj_case_variants, "KEYNAME", 2.0);
        cJSON_AddNumberToObject(obj_case_variants, "Keyname", 3.0);
        
        // Add same key with same case
        cJSON_AddNumberToObject(obj_same_case, "keyname", 1.0);
        cJSON_AddNumberToObject(obj_same_case, "keyname", 1.0);  // Same key
        
        // Test case-sensitive vs case-insensitive comparisons
        cJSON_bool case_sensitive_true = true;
        cJSON_bool case_sensitive_false = false;
        
        // Compare object with itself (should be true regardless)
        cJSON_bool self_compare_true = cJSON_Compare(obj_case_variants, obj_case_variants, case_sensitive_true);
        cJSON_bool self_compare_false = cJSON_Compare(obj_case_variants, obj_case_variants, case_sensitive_false);
        // Both should return true
        
        // Test string comparisons with different case sensitivity
        if (str_lower != NULL && str_upper != NULL) {
            cJSON_bool case_sens_result = cJSON_Compare(str_lower, str_upper, case_sensitive_true);
            // Should return false (case-sensitive)
            
            cJSON_bool case_insens_result = cJSON_Compare(str_lower, str_upper, case_sensitive_false);
            // May return true or false depending on strcmp behavior with different cases
        }
    }
    
    // ============================================================
    // CLEANUP
    // ============================================================
    
    // Clean up all created items
    if (null_item != NULL) cJSON_Delete(null_item);
    if (true_item != NULL) cJSON_Delete(true_item);
    if (false_item != NULL) cJSON_Delete(false_item);
    if (number_item != NULL) cJSON_Delete(number_item);
    if (string_item != NULL) cJSON_Delete(string_item);
    if (raw_item != NULL) cJSON_Delete(raw_item);
    if (array_item != NULL) cJSON_Delete(array_item);
    if (object_item != NULL) cJSON_Delete(object_item);
    
    if (nested_obj_a != NULL) cJSON_Delete(nested_obj_a);
    if (nested_obj_b != NULL) cJSON_Delete(nested_obj_b);
    if (obj_all_keys != NULL) cJSON_Delete(obj_all_keys);
    if (obj_common_only != NULL) cJSON_Delete(obj_common_only);
    
    if (array_ordered != NULL) cJSON_Delete(array_ordered);
    if (array_reversed != NULL) cJSON_Delete(array_reversed);
    if (array_shuffled != NULL) cJSON_Delete(array_shuffled);
    
    if (string_normal != NULL) cJSON_Delete(string_normal);
    if (string_empty != NULL) cJSON_Delete(string_empty);
    if (string_null_valuestring != NULL) {
        free(string_null_valuestring);  // Manually allocated, not with cJSON_Delete
    }
    
    if (raw_normal != NULL) cJSON_Delete(raw_normal);
    if (raw_null_valuestring != NULL) {
        free(raw_null_valuestring);  // Manually allocated
    }
    if (raw_same1 != NULL) cJSON_Delete(raw_same1);
    if (raw_same2 != NULL) cJSON_Delete(raw_same2);
    if (raw_diff != NULL) cJSON_Delete(raw_diff);
    
    if (str_lower != NULL) cJSON_Delete(str_lower);
    if (str_upper != NULL) cJSON_Delete(str_upper);
    if (str_mixed != NULL) cJSON_Delete(str_mixed);
    
    if (obj_case_variants != NULL) cJSON_Delete(obj_case_variants);
    if (obj_same_case != NULL) cJSON_Delete(obj_same_case);
    
    return 0;
}

} // namespace harness_010
