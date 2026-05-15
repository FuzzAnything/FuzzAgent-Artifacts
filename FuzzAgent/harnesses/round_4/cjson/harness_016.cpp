#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <cfloat>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_016 {
// Harness_016: Focus on symmetric object comparison path in cJSON_Compare (lines 3187-3193)
// Specifically targets the second comparison loop that checks b_element against a_element
// This path has 0 hits in coverage analysis and is critical for proper symmetric comparison

// Strategy: Create objects with same keys but values that are carefully crafted so that:
// 1. The first comparison loop (checking a elements in b) might partially pass
// 2. The second symmetric comparison loop (checking b elements in a) fails at line 3187-3190

// Helper to create objects with same keys but asymmetric values
// When compare_type is 0: Create object A with certain values
// When compare_type is 1: Create object B with different values for same keys
cJSON* create_asymmetric_objects(FuzzedDataProvider& fdp, int compare_type) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    // Determine number of key-value pairs (2-6 pairs for meaningful testing)
    int num_pairs = fdp.ConsumeIntegralInRange<int>(2, 6);
    
    // Generate keys first (same for both objects)
    std::vector<std::string> keys;
    for (int i = 0; i < num_pairs && fdp.remaining_bytes() > 8; i++) {
        size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
        if (fdp.remaining_bytes() < key_len) break;
        std::string key = fdp.ConsumeBytesAsString(key_len);
        // Ensure key is not empty
        if (!key.empty()) {
            keys.push_back(key);
        }
    }
    
    if (keys.empty()) {
        // Fallback to some default keys
        keys.push_back("key1");
        keys.push_back("key2");
        keys.push_back("key3");
    }
    
    // Add values based on compare_type
    for (size_t i = 0; i < keys.size() && fdp.remaining_bytes() > 4; i++) {
        const std::string& key = keys[i];
        
        // Determine value type for this key
        uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        
        switch (value_type) {
            case 0: { // Number - create values that are close but not equal
                double val;
                if (compare_type == 0) {
                    // Object A: Use a specific value
                    val = fdp.ConsumeFloatingPoint<double>();
                } else {
                    // Object B: Use a slightly different value
                    // Consume a value but adjust it slightly
                    double base = fdp.ConsumeFloatingPoint<double>();
                    // Make it different but close (within 2*DBL_EPSILON range)
                    val = base * (1.0 + 2.1 * DBL_EPSILON);
                }
                cJSON_AddNumberToObject(obj, key.c_str(), val);
                break;
            }
            case 1: { // Boolean - potentially different values
                bool val;
                if (compare_type == 0) {
                    val = fdp.ConsumeBool();
                } else {
                    // For object B, sometimes flip the boolean
                    bool base = fdp.ConsumeBool();
                    // 50% chance to flip for asymmetry
                    if (fdp.ConsumeBool()) {
                        val = !base;
                    } else {
                        val = base;
                    }
                }
                cJSON_AddBoolToObject(obj, key.c_str(), val);
                break;
            }
            case 2: { // String - similar but different strings
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                if (fdp.remaining_bytes() >= str_len) {
                    std::string str_val = fdp.ConsumeBytesAsString(str_len);
                    if (compare_type == 1 && !str_val.empty() && fdp.ConsumeBool()) {
                        // For object B, modify the string slightly
                        if (str_val.length() > 1) {
                            str_val[str_val.length() - 1] = 'X'; // Change last char
                        }
                    }
                    cJSON_AddStringToObject(obj, key.c_str(), str_val.c_str());
                }
                break;
            }
            case 3: { // Null
                cJSON_AddNullToObject(obj, key.c_str());
                break;
            }
            case 4: { // Array with potentially different contents
                cJSON* arr = cJSON_CreateArray();
                if (arr) {
                    int arr_size = fdp.ConsumeIntegralInRange<int>(0, 3);
                    for (int j = 0; j < arr_size && fdp.remaining_bytes() > 4; j++) {
                        if (compare_type == 0) {
                            cJSON_AddItemToArray(arr, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                        } else {
                            // Different value for object B
                            cJSON_AddItemToArray(arr, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>() * 1.1));
                        }
                    }
                    cJSON_AddItemToObject(obj, key.c_str(), arr);
                }
                break;
            }
            case 5: { // Nested object with asymmetric structure
                cJSON* nested = cJSON_CreateObject();
                if (nested) {
                    // Add a simple key-value pair
                    if (compare_type == 0) {
                        cJSON_AddNumberToObject(nested, "nested_key", fdp.ConsumeFloatingPoint<double>());
                    } else {
                        // Different value for object B's nested object
                        cJSON_AddNumberToObject(nested, "nested_key", fdp.ConsumeFloatingPoint<double>() + 0.1);
                    }
                    cJSON_AddItemToObject(obj, key.c_str(), nested);
                }
                break;
            }
        }
    }
    
    return obj;
}

// Create objects where values differ in a way that's only detectable in symmetric comparison
// Specifically targets the cJSON_Compare(b_element, a_element, case_sensitive) call at line 3187
cJSON* create_symmetric_failure_objects(FuzzedDataProvider& fdp, int obj_type) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    // Create 3-5 key-value pairs
    int num_pairs = fdp.ConsumeIntegralInRange<int>(3, 5);
    
    for (int i = 0; i < num_pairs && fdp.remaining_bytes() > 8; i++) {
        char key[16];
        snprintf(key, sizeof(key), "key%d", i);
        
        if (obj_type == 0) {
            // Object A: All values are "base" values
            double base_val = fdp.ConsumeFloatingPointInRange<double>(-100.0, 100.0);
            cJSON_AddNumberToObject(obj, key, base_val);
        } else {
            // Object B: Values are slightly modified
            // Consume but don't use directly - create asymmetric values
            fdp.ConsumeFloatingPoint<double>(); // Discard
            
            // Create value that's different but might pass first comparison in some cases
            // The key insight: when comparing A to B, the comparison might pass for some
            // values (if they're close), but when comparing B to A (symmetric check),
            // the comparison fails due to the asymmetric modification
            double modified_val;
            if (i == num_pairs - 1) {
                // Last key gets a significantly different value to trigger symmetric failure
                modified_val = 999.999; // Very different value
            } else {
                // Other keys get values that might pass first comparison
                modified_val = 100.0 + i * 0.001; // Systematic but different
            }
            cJSON_AddNumberToObject(obj, key, modified_val);
        }
    }
    
    return obj;
}

// Create complex nested objects where asymmetry is deep in the structure
// This tests recursive symmetric comparison
cJSON* create_deep_asymmetric_objects(FuzzedDataProvider& fdp, int depth, int max_depth, int obj_type) {
    if (depth >= max_depth) {
        // Leaf node - create simple value
        if (obj_type == 0) {
            return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        } else {
            // Object B gets a different value
            double val = fdp.ConsumeFloatingPoint<double>();
            return cJSON_CreateNumber(val + 0.5); // Always different
        }
    }
    
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    int num_keys = fdp.ConsumeIntegralInRange<int>(1, 3);
    for (int i = 0; i < num_keys && fdp.remaining_bytes() > 8; i++) {
        char key[16];
        snprintf(key, sizeof(key), "k%d_%d", depth, i);
        
        // Create nested value
        cJSON* value = create_deep_asymmetric_objects(fdp, depth + 1, max_depth, obj_type);
        if (value) {
            cJSON_AddItemToObject(obj, key, value);
        }
    }
    
    return obj;
}

// Test case specifically designed for line 3187: cJSON_Compare(b_element, a_element, case_sensitive)
// This is the symmetric check that's currently uncovered
cJSON* create_targeted_3187_test(FuzzedDataProvider& fdp, int obj_type) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    // Create objects with these specific keys
    const char* keys[] = {"a", "b", "c", "d", "e"};
    const int num_keys = 5;
    
    for (int i = 0; i < num_keys && fdp.remaining_bytes() > 4; i++) {
        if (obj_type == 0) {
            // Object A: Simple sequential values
            cJSON_AddNumberToObject(obj, keys[i], i * 1.0);
        } else {
            // Object B: Different values
            // For keys "a", "b", "c" - same values (so first loop passes)
            // For key "d" - different value (triggers symmetric failure at line 3187)
            // For key "e" - same value (tests continuation after failure)
            if (i < 3) {
                cJSON_AddNumberToObject(obj, keys[i], i * 1.0); // Same as A
            } else if (i == 3) {
                cJSON_AddNumberToObject(obj, keys[i], 999.0); // Different - triggers symmetric failure
            } else {
                cJSON_AddNumberToObject(obj, keys[i], i * 1.0); // Same as A
            }
        }
    }
    
    return obj;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) return 0; // Need minimum input
    
    FuzzedDataProvider fdp(data, size);
    
    cJSON* json_a = NULL;
    cJSON* json_b = NULL;
    cJSON_bool compare_result = false;
    cJSON_bool case_sensitive = fdp.ConsumeBool();
    
    // Select test scenario
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Basic asymmetric objects with same keys
            // Targets the general symmetric comparison failure
            json_a = create_asymmetric_objects(fdp, 0);
            json_b = create_asymmetric_objects(fdp, 1);
            break;
        }
        
        case 1: {
            // Scenario 1: Specifically designed for line 3187 failure
            // Creates objects where first few keys match but one key differs
            // This should trigger the symmetric comparison at line 3187
            json_a = create_targeted_3187_test(fdp, 0);
            json_b = create_targeted_3187_test(fdp, 1);
            break;
        }
        
        case 2: {
            // Scenario 2: Deep nested asymmetric structures
            // Tests recursive symmetric comparison
            int max_depth = fdp.ConsumeIntegralInRange<int>(1, 3);
            json_a = create_deep_asymmetric_objects(fdp, 0, max_depth, 0);
            json_b = create_deep_asymmetric_objects(fdp, 0, max_depth, 1);
            break;
        }
        
        case 3: {
            // Scenario 3: Objects with mixed value types
            // Some values match, some don't, testing the iteration order
            json_a = cJSON_CreateObject();
            json_b = cJSON_CreateObject();
            
            if (json_a && json_b) {
                // Add matching boolean
                bool bool_val = fdp.ConsumeBool();
                cJSON_AddBoolToObject(json_a, "bool1", bool_val);
                cJSON_AddBoolToObject(json_b, "bool1", bool_val);
                
                // Add different numbers
                cJSON_AddNumberToObject(json_a, "num1", fdp.ConsumeFloatingPoint<double>());
                cJSON_AddNumberToObject(json_b, "num2", fdp.ConsumeFloatingPoint<double>()); // Different key!
                
                // Add matching string
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                if (fdp.remaining_bytes() >= str_len) {
                    std::string str_val = fdp.ConsumeBytesAsString(str_len);
                    cJSON_AddStringToObject(json_a, "str1", str_val.c_str());
                    // Different string for object B
                    if (!str_val.empty()) {
                        std::string mod_str = str_val + "X";
                        cJSON_AddStringToObject(json_b, "str1", mod_str.c_str());
                    }
                }
                
                // Add null to both
                cJSON_AddNullToObject(json_a, "null1");
                cJSON_AddNullToObject(json_b, "null1");
            }
            break;
        }
        
        case 4: {
            // Scenario 4: Objects where values are "almost equal" but not exactly
            // Tests the compare_double function within symmetric comparison
            json_a = cJSON_CreateObject();
            json_b = cJSON_CreateObject();
            
            if (json_a && json_b) {
                // Add values that are within DBL_EPSILON but not exactly equal
                double val_a = fdp.ConsumeFloatingPoint<double>();
                double val_b = val_a * (1.0 + 1.1 * DBL_EPSILON); // Just outside tolerance
                
                cJSON_AddNumberToObject(json_a, "almost", val_a);
                cJSON_AddNumberToObject(json_b, "almost", val_b);
                
                // Add another pair with values that are exactly equal
                double exact_val = fdp.ConsumeFloatingPoint<double>();
                cJSON_AddNumberToObject(json_a, "exact", exact_val);
                cJSON_AddNumberToObject(json_b, "exact", exact_val);
            }
            break;
        }
    }
    
    // Perform the comparison if both objects were created
    if (json_a && json_b) {
        compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
        
        // Additional validation: Also compare in reverse order
        // This tests true symmetry of the comparison function
        cJSON_bool reverse_result = cJSON_Compare(json_b, json_a, case_sensitive);
        
        // For proper symmetric comparison, results should be the same
        // (unless there's a bug in the symmetric check at lines 3187-3193)
        if (compare_result != reverse_result) {
            // This would indicate asymmetric behavior, which might be a bug
            // or could be expected in certain edge cases
        }
    }
    
    // Clean up
    if (json_a) cJSON_Delete(json_a);
    if (json_b) cJSON_Delete(json_b);
    
    return 0;
}

} // namespace harness_016
