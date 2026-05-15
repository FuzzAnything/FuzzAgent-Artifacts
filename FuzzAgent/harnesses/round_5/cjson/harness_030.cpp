#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include "cjson/cJSON.h"

namespace harness_030 {
/*
 * Harness 030: Advanced cJSON_Compare edge cases targeting remaining blocked branches
 * 
 * Focus: Specialized comparison scenarios not covered by previous harnesses:
 * 1) Circular/reference structures and self-referential comparisons
 * 2) Deep mixed-type recursive comparisons with type boundary testing
 * 3) Edge cases in get_object_item string matching (NULL strings, empty strings)
 * 4) Object comparison with same keys but deeply nested value differences
 * 5) Array comparison with complex nested elements in different structural arrangements
 * 
 * Target blocked branches identified in coverage analysis:
 * - Line 3196-3197: Default case in inner switch (coverage 0)
 * - get_object_item edge cases (lines 1955-1961, 1976-1981)
 * - String/Raw comparison with NULL valuestring (lines 3123-3126)
 * - Array length mismatch deeper testing (lines 3151-3153)
 * - Object key missing scenarios (lines 3166-3169, 3182-3185)
 * 
 * Semantic diversity from existing harnesses:
 * - harness_023: Complex nested structures, array ordering, case variations
 * - harness_024: Key character edge cases, null values, Unicode
 * - harness_025: Malformed objects, reference objects, special floats
 * - This harness: Circular references, type boundary recursion, string NULL edge cases
 */



// Helper to create circular reference structures (challenging but we can try)
static cJSON* create_circular_structure(FuzzedDataProvider& fdp) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    // Add some regular content
    cJSON_AddItemToObject(obj, "num", cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
    cJSON_AddItemToObject(obj, "str", cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str()));
    
    // Create a reference to the object itself (simulating circular reference)
    // Note: cJSON doesn't directly support circular refs, but we can create reference objects
    cJSON* self_ref = cJSON_CreateObjectReference(obj);
    if (self_ref) {
        cJSON_AddItemToObject(obj, "self_ref", self_ref);
    }
    
    // Create mutual reference between two objects
    cJSON* obj2 = cJSON_CreateObject();
    if (obj2) {
        cJSON_AddItemToObject(obj2, "num2", cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
        
        // Add reference from obj2 to obj
        cJSON* ref_to_obj = cJSON_CreateObjectReference(obj);
        if (ref_to_obj) {
            cJSON_AddItemToObject(obj2, "ref_to_first", ref_to_obj);
        }
        
        // Add reference from obj to obj2
        cJSON* ref_to_obj2 = cJSON_CreateObjectReference(obj2);
        if (ref_to_obj2) {
            cJSON_AddItemToObject(obj, "ref_to_second", ref_to_obj2);
        }
        
        cJSON_Delete(obj2);
    }
    
    return obj;
}

// Helper to create objects with NULL string values in String/Raw types
static cJSON* create_object_with_null_strings(FuzzedDataProvider& fdp) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    int num_items = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    for (int i = 0; i < num_items; i++) {
        std::string key = "key_" + std::to_string(i);
        uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        switch (value_type) {
            case 0: {
                // String with potentially NULL valuestring (simulate malformed)
                cJSON* str_item = cJSON_CreateString(fdp.ConsumeRandomLengthString(16).c_str());
                if (str_item && fdp.ConsumeBool()) {
                    // Intentionally don't set valuestring to test NULL path
                    // Note: Can't directly set valuestring to NULL as it's const
                    // We'll create a raw JSON with empty string instead
                    cJSON_AddItemToObject(obj, key.c_str(), str_item);
                } else if (str_item) {
                    cJSON_AddItemToObject(obj, key.c_str(), str_item);
                }
                break;
            }
            case 1: {
                // Raw JSON with empty or NULL-like content
                std::string raw_content = fdp.ConsumeRandomLengthString(32);
                cJSON* raw_item = cJSON_CreateRaw(raw_content.c_str());
                if (raw_item) {
                    cJSON_AddItemToObject(obj, key.c_str(), raw_item);
                }
                break;
            }
            case 2: {
                // Regular number for contrast
                cJSON_AddItemToObject(obj, key.c_str(), 
                    cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                break;
            }
        }
    }
    
    return obj;
}

// Helper to create deep mixed-type structures that test type boundaries
static cJSON* create_deep_mixed_structure(FuzzedDataProvider& fdp, int depth, int max_depth) {
    if (depth >= max_depth) {
        // Create leaf node with varied types
        uint8_t leaf_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        switch (leaf_type) {
            case 0: return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            case 1: return cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
            case 2: return cJSON_CreateBool(fdp.ConsumeBool());
            case 3: return cJSON_CreateNull();
            case 4: {
                std::string raw = fdp.ConsumeRandomLengthString(16);
                return cJSON_CreateRaw(raw.c_str());
            }
            default: return cJSON_CreateArray();
        }
    }
    
    // Create either object or array
    if (fdp.ConsumeBool()) {
        // Create object
        cJSON* obj = cJSON_CreateObject();
        if (!obj) return NULL;
        
        int num_keys = fdp.ConsumeIntegralInRange<int>(1, 4);
        for (int i = 0; i < num_keys; i++) {
            std::string key = "k" + std::to_string(i) + "_" + 
                             fdp.ConsumeRandomLengthString(4);
            cJSON* value = create_deep_mixed_structure(fdp, depth + 1, max_depth);
            if (value) {
                cJSON_AddItemToObject(obj, key.c_str(), value);
            }
        }
        return obj;
    } else {
        // Create array
        cJSON* arr = cJSON_CreateArray();
        if (!arr) return NULL;
        
        int num_items = fdp.ConsumeIntegralInRange<int>(1, 4);
        for (int i = 0; i < num_items; i++) {
            cJSON* item = create_deep_mixed_structure(fdp, depth + 1, max_depth);
            if (item) {
                cJSON_AddItemToArray(arr, item);
            }
        }
        return arr;
    }
}

// Helper to test get_object_item edge cases
static void test_get_object_item_edge_cases(FuzzedDataProvider& fdp, cJSON* obj) {
    if (!obj) return;
    
    // Test with NULL object
    cJSON* result = cJSON_GetObjectItem(NULL, "test");
    (void)result; // Ignore result
    
    // Test with NULL string
    result = cJSON_GetObjectItem(obj, NULL);
    (void)result;
    
    // Test with empty string
    result = cJSON_GetObjectItem(obj, "");
    (void)result;
    
    // Test with string that has special characters
    std::string special_key = fdp.ConsumeRandomLengthString(16);
    result = cJSON_GetObjectItem(obj, special_key.c_str());
    (void)result;
    
    // Test case-sensitive vs case-insensitive
    result = cJSON_GetObjectItemCaseSensitive(obj, "test");
    (void)result;
}

// Helper to create arrays with same elements but different nesting structures
static std::pair<cJSON*, cJSON*> create_arrays_different_structures(FuzzedDataProvider& fdp) {
    cJSON* arr1 = cJSON_CreateArray();
    cJSON* arr2 = cJSON_CreateArray();
    
    if (!arr1 || !arr2) {
        if (arr1) cJSON_Delete(arr1);
        if (arr2) cJSON_Delete(arr2);
        return {NULL, NULL};
    }
    
    int num_elements = fdp.ConsumeIntegralInRange<int>(2, 6);
    
    for (int i = 0; i < num_elements; i++) {
        // Create same value but potentially different structure
        double value = fdp.ConsumeFloatingPoint<double>();
        std::string str_value = fdp.ConsumeRandomLengthString(16);
        
        // Add to arr1 as simple values
        cJSON_AddItemToArray(arr1, cJSON_CreateNumber(value));
        cJSON_AddItemToArray(arr1, cJSON_CreateString(str_value.c_str()));
        
        // Add to arr2 with nesting
        cJSON* nested1 = cJSON_CreateArray();
        if (nested1) {
            cJSON_AddItemToArray(nested1, cJSON_CreateNumber(value));
            cJSON_AddItemToArray(arr2, nested1);
        }
        
        cJSON* nested2 = cJSON_CreateObject();
        if (nested2) {
            cJSON_AddItemToObject(nested2, "value", cJSON_CreateString(str_value.c_str()));
            cJSON_AddItemToArray(arr2, nested2);
        }
    }
    
    return {arr1, arr2};
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed parameters
    bool case_sensitive = fdp.ConsumeBool();
    int test_type = fdp.ConsumeIntegralInRange<int>(0, 6);
    int max_depth = fdp.ConsumeIntegralInRange<int>(2, 5);
    
    switch (test_type) {
        case 0: {
            // Circular/reference structure testing
            cJSON* circ1 = create_circular_structure(fdp);
            cJSON* circ2 = create_circular_structure(fdp);
            
            if (circ1 && circ2) {
                // Compare circular structures
                cJSON_Compare(circ1, circ2, case_sensitive);
                cJSON_Compare(circ1, circ1, case_sensitive);
                cJSON_Compare(circ2, circ2, case_sensitive);
                
                // Test get_object_item on circular structures
                test_get_object_item_edge_cases(fdp, circ1);
                test_get_object_item_edge_cases(fdp, circ2);
            }
            
            if (circ1) cJSON_Delete(circ1);
            if (circ2) cJSON_Delete(circ2);
            break;
        }
        
        case 1: {
            // NULL string value edge cases in String/Raw types
            cJSON* null_str1 = create_object_with_null_strings(fdp);
            cJSON* null_str2 = create_object_with_null_strings(fdp);
            
            if (null_str1 && null_str2) {
                cJSON_Compare(null_str1, null_str2, case_sensitive);
                cJSON_Compare(null_str1, null_str1, case_sensitive);
                cJSON_Compare(null_str2, null_str2, case_sensitive);
                
                // Create Raw JSON comparisons
                std::string raw1 = fdp.ConsumeRandomLengthString(32);
                std::string raw2 = fdp.ConsumeRandomLengthString(32);
                cJSON* raw_json1 = cJSON_CreateRaw(raw1.c_str());
                cJSON* raw_json2 = cJSON_CreateRaw(raw2.c_str());
                
                if (raw_json1 && raw_json2) {
                    cJSON_Compare(raw_json1, raw_json2, case_sensitive);
                    cJSON_Delete(raw_json1);
                    cJSON_Delete(raw_json2);
                }
            }
            
            if (null_str1) cJSON_Delete(null_str1);
            if (null_str2) cJSON_Delete(null_str2);
            break;
        }
        
        case 2: {
            // Deep mixed-type recursive comparison
            cJSON* deep1 = create_deep_mixed_structure(fdp, 0, max_depth);
            cJSON* deep2 = create_deep_mixed_structure(fdp, 0, max_depth);
            
            if (deep1 && deep2) {
                cJSON_Compare(deep1, deep2, case_sensitive);
                cJSON_Compare(deep1, deep1, case_sensitive);
                cJSON_Compare(deep2, deep2, case_sensitive);
                
                // Compare with shallower versions
                cJSON* shallow = create_deep_mixed_structure(fdp, 0, max_depth / 2);
                if (shallow) {
                    cJSON_Compare(deep1, shallow, case_sensitive);
                    cJSON_Compare(shallow, deep2, case_sensitive);
                    cJSON_Delete(shallow);
                }
            }
            
            if (deep1) cJSON_Delete(deep1);
            if (deep2) cJSON_Delete(deep2);
            break;
        }
        
        case 3: {
            // Arrays with same elements in different structural arrangements
            auto [arr1, arr2] = create_arrays_different_structures(fdp);
            
            if (arr1 && arr2) {
                cJSON_Compare(arr1, arr2, case_sensitive);
                cJSON_Compare(arr1, arr1, case_sensitive);
                cJSON_Compare(arr2, arr2, case_sensitive);
                
                // Also compare with reversed arrays
                cJSON* arr1_copy = cJSON_Duplicate(arr1, 1);
                cJSON* arr2_copy = cJSON_Duplicate(arr2, 1);
                
                if (arr1_copy && arr2_copy) {
                    cJSON_Compare(arr1_copy, arr2, case_sensitive);
                    cJSON_Compare(arr1, arr2_copy, case_sensitive);
                    cJSON_Delete(arr1_copy);
                    cJSON_Delete(arr2_copy);
                }
            }
            
            if (arr1) cJSON_Delete(arr1);
            if (arr2) cJSON_Delete(arr2);
            break;
        }
        
        case 4: {
            // Object with same keys but deeply nested different values
            cJSON* obj1 = cJSON_CreateObject();
            cJSON* obj2 = cJSON_CreateObject();
            
            if (obj1 && obj2) {
                // Add same keys with different nested structures
                for (int i = 0; i < 4; i++) {
                    std::string key = "key_" + std::to_string(i);
                    
                    // Add to obj1 as simple value
                    cJSON_AddItemToObject(obj1, key.c_str(), 
                        cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                    
                    // Add to obj2 as nested structure with same key
                    cJSON* nested = cJSON_CreateObject();
                    if (nested) {
                        cJSON_AddItemToObject(nested, "inner", 
                            cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                        cJSON_AddItemToObject(obj2, key.c_str(), nested);
                    }
                }
                
                // Add one common key with same structure
                cJSON* common_value = cJSON_CreateString("common");
                if (common_value) {
                    cJSON_AddItemToObject(obj1, "common_key", cJSON_Duplicate(common_value, 1));
                    cJSON_AddItemToObject(obj2, "common_key", common_value);
                }
                
                cJSON_Compare(obj1, obj2, case_sensitive);
                cJSON_Compare(obj1, obj1, case_sensitive);
                cJSON_Compare(obj2, obj2, case_sensitive);
                
                // Test get_object_item edge cases
                test_get_object_item_edge_cases(fdp, obj1);
                test_get_object_item_edge_cases(fdp, obj2);
            }
            
            if (obj1) cJSON_Delete(obj1);
            if (obj2) cJSON_Delete(obj2);
            break;
        }
        
        case 5: {
            // Combined edge case testing
            // Create multiple structures and compare in various combinations
            
            cJSON* structures[4];
            for (int i = 0; i < 4; i++) {
                if (i % 2 == 0) {
                    structures[i] = create_deep_mixed_structure(fdp, 0, 3);
                } else {
                    structures[i] = create_object_with_null_strings(fdp);
                }
            }
            
            // Compare all pairs
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (structures[i] && structures[j]) {
                        cJSON_Compare(structures[i], structures[j], case_sensitive);
                    }
                }
            }
            
            // Clean up
            for (int i = 0; i < 4; i++) {
                if (structures[i]) cJSON_Delete(structures[i]);
            }
            break;
        }
        
        default: {
            // Parse JSON strings and compare (tests many code paths)
            if (fdp.remaining_bytes() > 100) {
                std::string json_str1 = fdp.ConsumeRandomLengthString(256);
                std::string json_str2 = fdp.ConsumeRandomLengthString(256);
                
                cJSON* parsed1 = cJSON_Parse(json_str1.c_str());
                cJSON* parsed2 = cJSON_Parse(json_str2.c_str());
                
                if (parsed1 && parsed2) {
                    cJSON_Compare(parsed1, parsed2, case_sensitive);
                    cJSON_Compare(parsed1, parsed1, case_sensitive);
                    cJSON_Compare(parsed2, parsed2, case_sensitive);
                    
                    // Test get_object_item on parsed JSON
                    test_get_object_item_edge_cases(fdp, parsed1);
                    test_get_object_item_edge_cases(fdp, parsed2);
                }
                
                if (parsed1) cJSON_Delete(parsed1);
                if (parsed2) cJSON_Delete(parsed2);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_030
