#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_015 {

// Helper to create objects with specific patterns for targeting the second comparison loop
// The goal is to trigger the false return path at line 3190 in cJSON_Compare

// Scenario 1: Objects with same keys but different values where first loop passes
// but second loop fails due to value mismatch when comparing b_element to a_element
cJSON* create_object_with_key_value_pairs(FuzzedDataProvider& fdp, bool create_for_a) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    // Create some key-value pairs
    int num_pairs = fdp.ConsumeIntegralInRange<int>(2, 6);
    for (int i = 0; i < num_pairs && fdp.remaining_bytes() > 16; i++) {
        // Create key
        size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
        if (fdp.remaining_bytes() < key_len) break;
        std::string key = fdp.ConsumeBytesAsString(key_len);
        
        // Create value based on whether this is for object A or B
        uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        
        switch (value_type) {
            case 0: {
                double val = fdp.ConsumeFloatingPoint<double>();
                cJSON_AddNumberToObject(obj, key.c_str(), val);
                break;
            }
            case 1: {
                bool val = fdp.ConsumeBool();
                cJSON_AddBoolToObject(obj, key.c_str(), val);
                break;
            }
            case 2: {
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                if (fdp.remaining_bytes() >= str_len) {
                    std::string str_val = fdp.ConsumeBytesAsString(str_len);
                    cJSON_AddStringToObject(obj, key.c_str(), str_val.c_str());
                }
                break;
            }
            case 3: {
                // Null value
                cJSON_AddNullToObject(obj, key.c_str());
                break;
            }
            case 4: {
                // Empty array
                cJSON* arr = cJSON_CreateArray();
                if (arr) {
                    cJSON_AddItemToObject(obj, key.c_str(), arr);
                }
                break;
            }
        }
    }
    
    return obj;
}

// Scenario 2: Objects where A has subset of B's keys but with different values
// First loop will find all A's keys in B, but second loop will find B's extra keys not in A
cJSON* create_subset_superset_objects(FuzzedDataProvider& fdp, bool is_superset, 
                                      const std::vector<std::string>& common_keys) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    // Add common keys
    for (const auto& key : common_keys) {
        if (fdp.remaining_bytes() < 8) break;
        
        uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (value_type) {
            case 0: {
                double val = fdp.ConsumeFloatingPoint<double>();
                cJSON_AddNumberToObject(obj, key.c_str(), val);
                break;
            }
            case 1: {
                bool val = fdp.ConsumeBool();
                cJSON_AddBoolToObject(obj, key.c_str(), val);
                break;
            }
            case 2: {
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                if (fdp.remaining_bytes() >= str_len) {
                    std::string str_val = fdp.ConsumeBytesAsString(str_len);
                    cJSON_AddStringToObject(obj, key.c_str(), str_val.c_str());
                }
                break;
            }
        }
    }
    
    // If creating superset, add extra keys
    if (is_superset) {
        int extra_keys = fdp.ConsumeIntegralInRange<int>(1, 4);
        for (int i = 0; i < extra_keys && fdp.remaining_bytes() > 16; i++) {
            size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
            if (fdp.remaining_bytes() < key_len) break;
            std::string key = fdp.ConsumeBytesAsString(key_len);
            
            // Make sure key is not in common_keys
            bool is_duplicate = false;
            for (const auto& common_key : common_keys) {
                if (key == common_key) {
                    is_duplicate = true;
                    break;
                }
            }
            
            if (!is_duplicate) {
                double val = fdp.ConsumeFloatingPoint<double>();
                cJSON_AddNumberToObject(obj, key.c_str(), val);
            }
        }
    }
    
    return obj;
}

// Scenario 3: Objects with same keys but nested structures that differ
// The outer objects match but inner structures cause comparison failure in second loop
cJSON* create_nested_object_with_variations(FuzzedDataProvider& fdp, int variant) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    // Add some common keys
    std::vector<std::string> keys = {"key1", "key2", "key3", "key4"};
    
    for (size_t i = 0; i < keys.size() && fdp.remaining_bytes() > 8; i++) {
        const std::string& key = keys[i];
        
        if (i == 1) {  // key2 - always add same type
            double val = fdp.ConsumeFloatingPoint<double>();
            cJSON_AddNumberToObject(obj, key.c_str(), val);
        } 
        else if (i == 2) {  // key3 - different based on variant
            if (variant == 0) {
                // For object A: array with numbers
                cJSON* arr = cJSON_CreateArray();
                if (arr) {
                    int arr_size = fdp.ConsumeIntegralInRange<int>(1, 3);
                    for (int j = 0; j < arr_size && fdp.remaining_bytes() > 4; j++) {
                        cJSON_AddItemToArray(arr, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                    }
                    cJSON_AddItemToObject(obj, key.c_str(), arr);
                }
            } else {
                // For object B: object with numbers
                cJSON* nested_obj = cJSON_CreateObject();
                if (nested_obj) {
                    int props = fdp.ConsumeIntegralInRange<int>(1, 3);
                    for (int j = 0; j < props && fdp.remaining_bytes() > 8; j++) {
                        char prop_name[16];
                        snprintf(prop_name, sizeof(prop_name), "prop%d", j);
                        cJSON_AddNumberToObject(nested_obj, prop_name, fdp.ConsumeFloatingPoint<double>());
                    }
                    cJSON_AddItemToObject(obj, key.c_str(), nested_obj);
                }
            }
        }
        else {
            // Other keys: simple values
            uint8_t val_type = fdp.ConsumeIntegral<uint8_t>() % 3;
            switch (val_type) {
                case 0: {
                    double val = fdp.ConsumeFloatingPoint<double>();
                    cJSON_AddNumberToObject(obj, key.c_str(), val);
                    break;
                }
                case 1: {
                    bool val = fdp.ConsumeBool();
                    cJSON_AddBoolToObject(obj, key.c_str(), val);
                    break;
                }
                case 2: {
                    size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                    if (fdp.remaining_bytes() >= str_len) {
                        std::string str_val = fdp.ConsumeBytesAsString(str_len);
                        cJSON_AddStringToObject(obj, key.c_str(), str_val.c_str());
                    }
                    break;
                }
            }
        }
    }
    
    return obj;
}

// Scenario 4: Case sensitivity edge cases
// Objects with keys that differ only in case, testing case_sensitive parameter
cJSON* create_case_sensitive_objects(FuzzedDataProvider& fdp, bool use_uppercase) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    std::vector<std::string> base_keys = {"key", "name", "value", "data"};
    
    for (const auto& base_key : base_keys) {
        if (fdp.remaining_bytes() < 8) break;
        
        std::string key = base_key;
        if (use_uppercase) {
            // Convert to uppercase
            for (char& c : key) {
                c = toupper(c);
            }
        }
        
        // Add value
        uint8_t val_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (val_type) {
            case 0: {
                double val = fdp.ConsumeFloatingPoint<double>();
                cJSON_AddNumberToObject(obj, key.c_str(), val);
                break;
            }
            case 1: {
                bool val = fdp.ConsumeBool();
                cJSON_AddBoolToObject(obj, key.c_str(), val);
                break;
            }
            case 2: {
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                if (fdp.remaining_bytes() >= str_len) {
                    std::string str_val = fdp.ConsumeBytesAsString(str_len);
                    cJSON_AddStringToObject(obj, key.c_str(), str_val.c_str());
                }
                break;
            }
        }
    }
    
    return obj;
}

// Scenario 5: Objects with null string values and references
cJSON* create_objects_with_null_strings(FuzzedDataProvider& fdp, int variant) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    // Add regular key-value pairs
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 8; i++) {
        char key[16];
        snprintf(key, sizeof(key), "key%d", i);
        double val = fdp.ConsumeFloatingPoint<double>();
        cJSON_AddNumberToObject(obj, key, val);
    }
    
    // Add string-related entries based on variant
    if (variant == 0) {
        // Object A: Add NULL string reference
        cJSON_AddItemToObject(obj, "null_str", cJSON_CreateStringReference(NULL));
        // Add regular string
        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
        if (fdp.remaining_bytes() >= str_len) {
            std::string str_val = fdp.ConsumeBytesAsString(str_len);
            cJSON_AddStringToObject(obj, "regular_str", str_val.c_str());
        }
    } else {
        // Object B: Add empty string
        cJSON_AddStringToObject(obj, "null_str", "");
        // Add potentially different regular string
        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
        if (fdp.remaining_bytes() >= str_len) {
            std::string str_val = fdp.ConsumeBytesAsString(str_len);
            cJSON_AddStringToObject(obj, "regular_str", str_val.c_str());
        }
    }
    
    return obj;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for complex object comparisons
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine test scenario specifically targeting second comparison loop
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    cJSON_bool case_sensitive = fdp.ConsumeBool();
    
    cJSON* json_a = NULL;
    cJSON* json_b = NULL;
    cJSON_bool compare_result = false;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Objects with same keys but different values
            // First loop passes (finds all keys), second loop fails (values differ when comparing b to a)
            json_a = create_object_with_key_value_pairs(fdp, true);
            json_b = create_object_with_key_value_pairs(fdp, false);
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
                // Likely false due to different values for same keys
            }
            break;
        }
        
        case 1: {
            // Scenario 1: A is subset of B (B has extra keys)
            // First loop passes (finds all A's keys in B), second loop fails (B has keys not in A)
            std::vector<std::string> common_keys;
            int num_common = fdp.ConsumeIntegralInRange<int>(2, 4);
            for (int i = 0; i < num_common && fdp.remaining_bytes() > 8; i++) {
                size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 8);
                if (fdp.remaining_bytes() >= key_len) {
                    std::string key = fdp.ConsumeBytesAsString(key_len);
                    common_keys.push_back(key);
                }
            }
            
            json_a = create_subset_superset_objects(fdp, false, common_keys);  // subset
            json_b = create_subset_superset_objects(fdp, true, common_keys);   // superset
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
                // Should be false - A is subset of B
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Nested objects with structural differences
            // Outer objects have same keys but inner structures differ
            // First loop might pass for some keys, second loop fails on nested comparison
            json_a = create_nested_object_with_variations(fdp, 0);
            json_b = create_nested_object_with_variations(fdp, 1);
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
                // Likely false due to different nested structures
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Case sensitivity testing
            // Objects with same keys but different casing
            json_a = create_case_sensitive_objects(fdp, false);  // lowercase keys
            json_b = create_case_sensitive_objects(fdp, true);   // uppercase keys
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
                // Result depends on case_sensitive parameter
            }
            break;
        }
        
        case 4: {
            // Scenario 4: Objects with null strings vs empty strings
            // Tests edge cases in string comparison within objects
            json_a = create_objects_with_null_strings(fdp, 0);
            json_b = create_objects_with_null_strings(fdp, 1);
            
            if (json_a && json_b) {
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
                // Likely false due to NULL vs empty string comparison
            }
            break;
        }
        
        case 5: {
            // Scenario 5: Complex objects where first few keys match but later ones differ
            // Tests the iteration order in the second comparison loop
            json_a = cJSON_CreateObject();
            json_b = cJSON_CreateObject();
            
            if (json_a && json_b) {
                // Add matching key-value pairs
                for (int i = 0; i < 3 && fdp.remaining_bytes() > 8; i++) {
                    char key[16];
                    snprintf(key, sizeof(key), "match%d", i);
                    double val = fdp.ConsumeFloatingPoint<double>();
                    cJSON_AddNumberToObject(json_a, key, val);
                    cJSON_AddNumberToObject(json_b, key, val);
                }
                
                // Add different key to A
                if (fdp.remaining_bytes() > 8) {
                    cJSON_AddNumberToObject(json_a, "diff_a", fdp.ConsumeFloatingPoint<double>());
                }
                
                // Add different key to B  
                if (fdp.remaining_bytes() > 8) {
                    cJSON_AddNumberToObject(json_b, "diff_b", fdp.ConsumeFloatingPoint<double>());
                }
                
                // Add one more matching pair
                if (fdp.remaining_bytes() > 8) {
                    double val = fdp.ConsumeFloatingPoint<double>();
                    cJSON_AddNumberToObject(json_a, "final", val);
                    cJSON_AddNumberToObject(json_b, "final", val);
                }
                
                compare_result = cJSON_Compare(json_a, json_b, case_sensitive);
                // Should be false - objects have different keys
            }
            break;
        }
    }
    
    // Clean up
    if (json_a) cJSON_Delete(json_a);
    if (json_b) cJSON_Delete(json_b);
    
    return 0;
}

} // namespace harness_015
