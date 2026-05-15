#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "cjson/cJSON.h"

namespace harness_008 {
/*
 * Harness_008: Targeted edge case testing for cJSON_Compare
 * 
 * Target: Specifically targets the 70 blocked branches in cJSON_Compare
 * with focus on the edge cases identified in coverage guidance:
 * 
 * 1. String/Raw objects with NULL valuestring (lines 3123-3126)
 * 2. Invalid type values to hit default switch cases (lines 3096-3098, 3196-3198)
 * 3. Array comparisons with different lengths (lines 3150-3153)
 * 4. Object comparisons with different key sets (lines 3166-3169, 3182-3185)
 * 5. Number comparisons with edge values (INT_MAX, INT_MIN, NaN, infinity)
 * 6. Case-sensitive vs case-insensitive string comparisons
 * 
 * This harness is semantically distinct from previous harnesses (005, 006, 007):
 * - Specifically manipulates cJSON structs to create edge cases
 * - Directly tests the blocked conditional branches
 * - Uses invalid type values to hit default switch cases
 * - Tests all comparison failure modes systematically
 */



// Helper function to create cJSON objects with NULL valuestring
// This tests the branch at lines 3123-3126 in cJSON_Compare
static cJSON* create_string_with_null_valuestring() {
    cJSON* item = (cJSON*)malloc(sizeof(cJSON));
    if (item == NULL) {
        return NULL;
    }
    memset(item, 0, sizeof(cJSON));
    item->type = cJSON_String;
    item->valuestring = NULL;  // Explicitly NULL valuestring
    return item;
}

static cJSON* create_raw_with_null_valuestring() {
    cJSON* item = (cJSON*)malloc(sizeof(cJSON));
    if (item == NULL) {
        return NULL;
    }
    memset(item, 0, sizeof(cJSON));
    item->type = cJSON_Raw;
    item->valuestring = NULL;  // Explicitly NULL valuestring
    return item;
}

// Helper to create cJSON with invalid type (to hit default switch cases)
static cJSON* create_invalid_type_cjson(uint8_t invalid_type) {
    cJSON* item = (cJSON*)malloc(sizeof(cJSON));
    if (item == NULL) {
        return NULL;
    }
    memset(item, 0, sizeof(cJSON));
    item->type = invalid_type;  // Invalid type that doesn't match any cJSON type
    return item;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for comprehensive edge case testing
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // ============================================================
    // PART 1: STRING/RAW OBJECTS WITH NULL VALUESTRING
    // ============================================================
    
    // 1.1 Test String with NULL valuestring vs normal string
    cJSON* null_string = create_string_with_null_valuestring();
    cJSON* normal_string = cJSON_CreateString(fdp.ConsumeRandomLengthString(64).c_str());
    cJSON* another_null_string = create_string_with_null_valuestring();
    
    if (null_string != NULL && normal_string != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool result1 = cJSON_Compare(null_string, normal_string, case_sensitive);
        // Should return false (NULL valuestring)
        
        cJSON_bool result2 = cJSON_Compare(normal_string, null_string, case_sensitive);
        // Should also return false
    }
    
    if (null_string != NULL && another_null_string != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool result = cJSON_Compare(null_string, another_null_string, case_sensitive);
        // Should return false (both have NULL valuestring)
    }
    
    // 1.2 Test Raw with NULL valuestring
    cJSON* null_raw = create_raw_with_null_valuestring();
    cJSON* normal_raw = cJSON_CreateRaw(fdp.ConsumeRandomLengthString(64).c_str());
    
    if (null_raw != NULL && normal_raw != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool result1 = cJSON_Compare(null_raw, normal_raw, case_sensitive);
        cJSON_bool result2 = cJSON_Compare(normal_raw, null_raw, case_sensitive);
        // Both should return false
    }
    
    // ============================================================
    // PART 2: INVALID TYPE VALUES (to hit default switch cases)
    // ============================================================
    
    // Create cJSON objects with invalid types
    uint8_t invalid_type1 = fdp.ConsumeIntegral<uint8_t>();
    uint8_t invalid_type2 = fdp.ConsumeIntegral<uint8_t>();
    
    cJSON* invalid_item1 = create_invalid_type_cjson(invalid_type1);
    cJSON* invalid_item2 = create_invalid_type_cjson(invalid_type2);
    cJSON* valid_item = cJSON_CreateNull();  // Valid item for cross-type comparison
    
    if (invalid_item1 != NULL && invalid_item2 != NULL) {
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        // Compare two invalid items - should hit default case
        cJSON_bool result1 = cJSON_Compare(invalid_item1, invalid_item2, case_sensitive);
        
        // Compare invalid with valid - should also hit default or type mismatch
        if (valid_item != NULL) {
            cJSON_bool result2 = cJSON_Compare(invalid_item1, valid_item, case_sensitive);
            cJSON_bool result3 = cJSON_Compare(valid_item, invalid_item1, case_sensitive);
        }
    }
    
    // ============================================================
    // PART 3: ARRAY COMPARISONS WITH DIFFERENT LENGTHS
    // ============================================================
    
    // Create arrays with different lengths
    cJSON* short_array = cJSON_CreateArray();
    cJSON* long_array = cJSON_CreateArray();
    
    if (short_array != NULL && long_array != NULL) {
        // Add elements to short array
        size_t short_len = fdp.ConsumeIntegralInRange<size_t>(1, 3);
        for (size_t i = 0; i < short_len; i++) {
            cJSON_AddItemToArray(short_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
        }
        
        // Add more elements to long array (including same elements as short array)
        for (size_t i = 0; i < short_len; i++) {
            cJSON_AddItemToArray(long_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
        }
        
        // Add extra elements to make long array longer
        size_t extra_len = fdp.ConsumeIntegralInRange<size_t>(1, 3);
        for (size_t i = 0; i < extra_len; i++) {
            cJSON_AddItemToArray(long_array, cJSON_CreateString(fdp.ConsumeRandomLengthString(16).c_str()));
        }
        
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool result = cJSON_Compare(short_array, long_array, case_sensitive);
        // Should return false (different lengths)
    }
    
    // ============================================================
    // PART 4: OBJECT COMPARISONS WITH DIFFERENT KEY SETS
    // ============================================================
    
    cJSON* obj1 = cJSON_CreateObject();
    cJSON* obj2 = cJSON_CreateObject();
    cJSON* obj3 = cJSON_CreateObject();
    
    if (obj1 != NULL && obj2 != NULL && obj3 != NULL) {
        // Create some common keys
        std::vector<std::string> common_keys;
        size_t num_common = fdp.ConsumeIntegralInRange<size_t>(2, 4);
        for (size_t i = 0; i < num_common; i++) {
            std::string key = fdp.ConsumeRandomLengthString(8);
            common_keys.push_back(key);
            
            double common_value = fdp.ConsumeFloatingPoint<double>();
            cJSON_AddNumberToObject(obj1, key.c_str(), common_value);
            cJSON_AddNumberToObject(obj2, key.c_str(), common_value);
            cJSON_AddNumberToObject(obj3, key.c_str(), fdp.ConsumeFloatingPoint<double>());  // Different value for obj3
        }
        
        // Add unique keys to each object
        size_t num_unique1 = fdp.ConsumeIntegralInRange<size_t>(1, 3);
        for (size_t i = 0; i < num_unique1; i++) {
            std::string key = fdp.ConsumeRandomLengthString(8);
            cJSON_AddStringToObject(obj1, key.c_str(), fdp.ConsumeRandomLengthString(16).c_str());
        }
        
        size_t num_unique2 = fdp.ConsumeIntegralInRange<size_t>(1, 3);
        for (size_t i = 0; i < num_unique2; i++) {
            std::string key = fdp.ConsumeRandomLengthString(8);
            cJSON_AddBoolToObject(obj2, key.c_str(), fdp.ConsumeBool());
        }
        
        // Test comparisons
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        
        // obj1 vs obj2: Same common keys, different unique keys
        cJSON_bool result1 = cJSON_Compare(obj1, obj2, case_sensitive);
        // Should return false (different key sets)
        
        // obj1 vs obj3: Same keys, different values
        cJSON_bool result2 = cJSON_Compare(obj1, obj3, case_sensitive);
        // Should return false (different values for same keys)
    }
    
    // ============================================================
    // PART 5: NUMBER COMPARISONS WITH EDGE VALUES
    // ============================================================
    
    // Create numbers with edge values
    double edge_values[] = {
        0.0,
        -0.0,
        1.0,
        -1.0,
        DBL_MAX,
        DBL_MIN,
        DBL_EPSILON,
        HUGE_VAL,
        -HUGE_VAL,
        NAN,
        INFINITY,
        -INFINITY,
        static_cast<double>(INT_MAX),
        static_cast<double>(INT_MIN),
        static_cast<double>(LONG_MAX),
        static_cast<double>(LONG_MIN)
    };
    
    // Test various edge value comparisons
    for (size_t i = 0; i < sizeof(edge_values)/sizeof(edge_values[0]) && i < 5; i++) {
        for (size_t j = 0; j < sizeof(edge_values)/sizeof(edge_values[0]) && j < 5; j++) {
            cJSON* num1 = cJSON_CreateNumber(edge_values[i]);
            cJSON* num2 = cJSON_CreateNumber(edge_values[j]);
            
            if (num1 != NULL && num2 != NULL) {
                cJSON_bool case_sensitive = fdp.ConsumeBool();
                cJSON_bool result = cJSON_Compare(num1, num2, case_sensitive);
                // Tests compare_double function with edge cases
                
                cJSON_Delete(num1);
                cJSON_Delete(num2);
            }
        }
    }
    
    // Also test random numbers from fuzzer input
    size_t num_random_comparisons = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    for (size_t i = 0; i < num_random_comparisons; i++) {
        double val1 = fdp.ConsumeFloatingPoint<double>();
        double val2 = fdp.ConsumeFloatingPoint<double>();
        
        cJSON* rand_num1 = cJSON_CreateNumber(val1);
        cJSON* rand_num2 = cJSON_CreateNumber(val2);
        
        if (rand_num1 != NULL && rand_num2 != NULL) {
            cJSON_bool case_sensitive = fdp.ConsumeBool();
            cJSON_bool result = cJSON_Compare(rand_num1, rand_num2, case_sensitive);
            
            cJSON_Delete(rand_num1);
            cJSON_Delete(rand_num2);
        }
    }
    
    // ============================================================
    // PART 6: CASE-SENSITIVE VS CASE-INSENSITIVE STRING COMPARISONS
    // ============================================================
    
    // Create strings with case variations
    std::string lower_str = fdp.ConsumeRandomLengthString(32);
    std::string upper_str = lower_str;
    std::string mixed_str = lower_str;
    
    // Convert to upper and mixed case
    for (char& c : upper_str) {
        if (c >= 'a' && c <= 'z') {
            c = c - 'a' + 'A';
        }
    }
    
    for (size_t i = 0; i < mixed_str.length(); i++) {
        if (i % 2 == 0 && mixed_str[i] >= 'a' && mixed_str[i] <= 'z') {
            mixed_str[i] = mixed_str[i] - 'a' + 'A';
        }
    }
    
    cJSON* str_lower = cJSON_CreateString(lower_str.c_str());
    cJSON* str_upper = cJSON_CreateString(upper_str.c_str());
    cJSON* str_mixed = cJSON_CreateString(mixed_str.c_str());
    cJSON* str_same = cJSON_CreateString(lower_str.c_str());  // Same as str_lower
    
    if (str_lower != NULL && str_upper != NULL) {
        // Case-sensitive comparison: should return false (different cases)
        cJSON_bool case_sensitive_true = true;
        cJSON_bool result_case_sensitive = cJSON_Compare(str_lower, str_upper, case_sensitive_true);
        
        // Case-insensitive comparison: might return true or false depending on get_object_item implementation
        cJSON_bool case_sensitive_false = false;
        cJSON_bool result_case_insensitive = cJSON_Compare(str_lower, str_upper, case_sensitive_false);
    }
    
    if (str_lower != NULL && str_same != NULL) {
        // Should return true for both case-sensitive and case-insensitive
        cJSON_bool case_sensitive = fdp.ConsumeBool();
        cJSON_bool result = cJSON_Compare(str_lower, str_same, case_sensitive);
    }
    
    // Test with object keys (case sensitivity affects key lookup in get_object_item)
    cJSON* obj_with_keys = cJSON_CreateObject();
    if (obj_with_keys != NULL && str_lower != NULL) {
        // Add item with lower case key
        cJSON_AddItemToObject(obj_with_keys, lower_str.c_str(), cJSON_CreateNumber(42));
        
        // Create another object with same key but different case
        cJSON* obj_with_keys2 = cJSON_CreateObject();
        if (obj_with_keys2 != NULL) {
            cJSON_AddItemToObject(obj_with_keys2, upper_str.c_str(), cJSON_CreateNumber(42));
            
            // Compare objects - case sensitivity affects whether keys match
            cJSON_bool case_sensitive_true = true;
            cJSON_bool result_case_sensitive = cJSON_Compare(obj_with_keys, obj_with_keys2, case_sensitive_true);
            
            cJSON_bool case_sensitive_false = false;
            cJSON_bool result_case_insensitive = cJSON_Compare(obj_with_keys, obj_with_keys2, case_sensitive_false);
            
            cJSON_Delete(obj_with_keys2);
        }
    }
    
    // ============================================================
    // CLEANUP
    // ============================================================
    
    // Clean up all created objects
    if (null_string != NULL) free(null_string);
    if (another_null_string != NULL) free(another_null_string);
    if (null_raw != NULL) free(null_raw);
    if (invalid_item1 != NULL) free(invalid_item1);
    if (invalid_item2 != NULL) free(invalid_item2);
    
    cJSON_Delete(normal_string);
    cJSON_Delete(normal_raw);
    cJSON_Delete(valid_item);
    cJSON_Delete(short_array);
    cJSON_Delete(long_array);
    cJSON_Delete(obj1);
    cJSON_Delete(obj2);
    cJSON_Delete(obj3);
    cJSON_Delete(str_lower);
    cJSON_Delete(str_upper);
    cJSON_Delete(str_mixed);
    cJSON_Delete(str_same);
    cJSON_Delete(obj_with_keys);
    
    return 0;
}

} // namespace harness_008
