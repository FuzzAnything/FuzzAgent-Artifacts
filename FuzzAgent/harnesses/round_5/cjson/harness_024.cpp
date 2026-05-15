#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cwctype>
#include "cjson/cJSON.h"

namespace harness_024 {
/*
 * Harness 024: cJSON_Compare object comparison edge cases targeting remaining 73/76 blocked branches
 * 
 * Focus: Specific edge cases in object comparison to complement harness_023 (complex nested structures):
 * 1) Objects with special characters in keys (quotes, backslashes, control chars)
 * 2) Objects with null values (test null value comparison vs null objects)
 * 3) Objects with duplicate keys (cJSON doesn't support but we can test edge behavior)
 * 4) Objects with escaped characters in keys and values
 * 5) Objects with Unicode keys (UTF-8 characters)
 * 6) Objects with mixed value types comparison
 * 
 * Target blocked branches in cJSON_Compare object comparison paths:
 * - Line 3166: b_element == NULL (key missing in second object)
 * - Line 3171: !cJSON_Compare(a_element, b_element, case_sensitive) (value comparison failure)
 * - Line 3182: a_element == NULL (key missing in first object)  
 * - Line 3187: !cJSON_Compare(b_element, a_element, case_sensitive) (reverse comparison failure)
 * - Line 3197: default case for invalid types
 *
 * Also targets get_object_item case-sensitive/insensitive string comparison:
 * - Line 1963: strcmp(name, current_element->string) != 0 (case-sensitive)
 * - Line 1970: case_insensitive_strcmp(...) != 0 (case-insensitive)
 *
 * Semantic diversity from harness_023: 
 * - harness_023: Complex nested structures, same keys different values, different keys
 * - This harness: Edge case key characters, null values, escaped chars, Unicode, mixed types
 */



// Helper to generate keys with special characters
static std::string generate_special_key(FuzzedDataProvider& fdp) {
    uint8_t key_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (key_type) {
        case 0: // Keys with quotes
            return "\"" + fdp.ConsumeRandomLengthString(8) + "\"";
        case 1: // Keys with backslashes
            return "key\\with\\backslashes\\" + fdp.ConsumeRandomLengthString(4);
        case 2: // Keys with control characters
            return std::string(1, '\t') + fdp.ConsumeRandomLengthString(6) + std::string(1, '\n');
        case 3: // Keys with special regex characters
            return ".*+?[]{}()|^$" + fdp.ConsumeRandomLengthString(4);
        case 4: // Keys with whitespace
            return " key with spaces " + fdp.ConsumeRandomLengthString(3);
        default: // Mixed special characters
            return "key\"with\\mix\ted\nchars" + fdp.ConsumeRandomLengthString(4);
    }
}

// Helper to generate Unicode keys (UTF-8)
static std::string generate_unicode_key(FuzzedDataProvider& fdp) {
    // Simple UTF-8 characters for testing
    std::vector<std::string> unicode_chars = {
        "α", "β", "γ", "δ", "ε", "ζ", "η", "θ", "λ", "μ",
        "あ", "い", "う", "え", "お",
        "가", "나", "다", "라", "마",
        "©", "®", "™", "€", "£", "¥",
        "🌍", "🚀", "🎉", "🔥", "⭐"
    };
    
    uint8_t char_idx = fdp.ConsumeIntegral<uint8_t>() % unicode_chars.size();
    return unicode_chars[char_idx] + fdp.ConsumeRandomLengthString(6);
}

// Helper to create object with null values
static cJSON* create_object_with_null_values(FuzzedDataProvider& fdp) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    int num_keys = fdp.ConsumeIntegralInRange<int>(1, 8);
    
    for (int i = 0; i < num_keys; i++) {
        std::string key = "key_" + std::to_string(i);
        
        // Always add null values
        cJSON* null_val = cJSON_CreateNull();
        if (null_val) {
            cJSON_AddItemToObject(obj, key.c_str(), null_val);
        }
    }
    
    // Add some non-null values for mixed comparison
    if (fdp.ConsumeBool()) {
        cJSON_AddItemToObject(obj, "non_null_string", cJSON_CreateString("test"));
    }
    if (fdp.ConsumeBool()) {
        cJSON_AddItemToObject(obj, "non_null_number", cJSON_CreateNumber(42.5));
    }
    
    return obj;
}

// Helper to create object with escaped characters in values
static cJSON* create_object_with_escaped_values(FuzzedDataProvider& fdp) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    // Strings with various escaped characters
    std::vector<std::string> escaped_strings = {
        "line1\\nline2",
        "tab\\tseparated",
        "backslash\\\\test",
        "quote\\\"inside",
        "control\\x01chars",
        "mixed\\n\\t\\r\\\"\\\\"
    };
    
    int num_items = fdp.ConsumeIntegralInRange<int>(1, 4);
    for (int i = 0; i < num_items; i++) {
        std::string key = "escaped_" + std::to_string(i);
        uint8_t str_idx = fdp.ConsumeIntegral<uint8_t>() % escaped_strings.size();
        
        cJSON* str_val = cJSON_CreateString(escaped_strings[str_idx].c_str());
        if (str_val) {
            cJSON_AddItemToObject(obj, key.c_str(), str_val);
        }
    }
    
    return obj;
}

// Helper to create object with mixed value types
static cJSON* create_object_with_mixed_types(FuzzedDataProvider& fdp) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    // Add different types to same object
    cJSON_AddItemToObject(obj, "string", cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str()));
    cJSON_AddItemToObject(obj, "number", cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
    cJSON_AddItemToObject(obj, "bool_true", cJSON_CreateTrue());
    cJSON_AddItemToObject(obj, "bool_false", cJSON_CreateFalse());
    cJSON_AddItemToObject(obj, "null", cJSON_CreateNull());
    
    // Add nested objects/arrays
    cJSON* nested_obj = cJSON_CreateObject();
    if (nested_obj) {
        cJSON_AddItemToObject(nested_obj, "nested_key", cJSON_CreateString("nested_value"));
        cJSON_AddItemToObject(obj, "object", nested_obj);
    }
    
    cJSON* array = cJSON_CreateArray();
    if (array) {
        cJSON_AddItemToArray(array, cJSON_CreateNumber(1.0));
        cJSON_AddItemToArray(array, cJSON_CreateString("array_item"));
        cJSON_AddItemToObject(obj, "array", array);
    }
    
    return obj;
}

// Helper to test duplicate keys (cJSON doesn't support but we can test edge behavior)
static void test_duplicate_key_behavior(FuzzedDataProvider& fdp) {
    // Create JSON string with duplicate keys
    std::string duplicate_json = "{\"key\": \"value1\", \"key\": \"value2\"}";
    cJSON* parsed = cJSON_Parse(duplicate_json.c_str());
    
    if (parsed) {
        // Test comparison with another object
        cJSON* compare_obj = cJSON_CreateObject();
        if (compare_obj) {
            cJSON_AddItemToObject(compare_obj, "key", cJSON_CreateString("value1"));
            cJSON_Compare(parsed, compare_obj, fdp.ConsumeBool());
            cJSON_Delete(compare_obj);
        }
        cJSON_Delete(parsed);
    }
}

// Test case-sensitive vs insensitive key matching with special characters
static void test_case_sensitive_special_keys(FuzzedDataProvider& fdp, bool case_sensitive) {
    cJSON* obj1 = cJSON_CreateObject();
    cJSON* obj2 = cJSON_CreateObject();
    
    if (!obj1 || !obj2) {
        if (obj1) cJSON_Delete(obj1);
        if (obj2) cJSON_Delete(obj2);
        return;
    }
    
    // Create keys with case variations and special characters
    std::string key1, key2;
    
    if (case_sensitive) {
        // Different case = different keys
        key1 = "KeyWithCase" + generate_special_key(fdp);
        key2 = "keywithcase" + generate_special_key(fdp); // lowercase version
    } else {
        // Same base key with different special characters
        key1 = "same_key" + generate_special_key(fdp);
        key2 = "same_key" + generate_special_key(fdp);
    }
    
    cJSON_AddItemToObject(obj1, key1.c_str(), cJSON_CreateNumber(1.0));
    cJSON_AddItemToObject(obj2, key2.c_str(), cJSON_CreateNumber(1.0));
    
    // Add some extra keys to make objects different
    if (fdp.ConsumeBool()) {
        cJSON_AddItemToObject(obj1, "extra1", cJSON_CreateString("extra"));
    }
    if (fdp.ConsumeBool()) {
        cJSON_AddItemToObject(obj2, "extra2", cJSON_CreateString("extra"));
    }
    
    cJSON_Compare(obj1, obj2, case_sensitive);
    
    cJSON_Delete(obj1);
    cJSON_Delete(obj2);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Minimum size for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool case_sensitive = fdp.ConsumeBool();
    
    cJSON *obj1 = NULL;
    cJSON *obj2 = NULL;
    
    switch (test_scenario) {
        case 0: {
            // Scenario 1: Objects with special characters in keys
            obj1 = cJSON_CreateObject();
            obj2 = cJSON_CreateObject();
            
            if (obj1 && obj2) {
                // Add keys with special characters
                for (int i = 0; i < 3; i++) {
                    std::string key1 = generate_special_key(fdp);
                    std::string key2 = generate_special_key(fdp);
                    
                    cJSON_AddItemToObject(obj1, key1.c_str(), 
                        cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                    cJSON_AddItemToObject(obj2, key2.c_str(),
                        cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                }
                
                // Add one matching key with special characters
                std::string common_key = generate_special_key(fdp);
                cJSON_AddItemToObject(obj1, common_key.c_str(), cJSON_CreateNumber(42.0));
                cJSON_AddItemToObject(obj2, common_key.c_str(), cJSON_CreateNumber(42.0));
                
                cJSON_Compare(obj1, obj2, case_sensitive);
            }
            break;
        }
        
        case 1: {
            // Scenario 2: Objects with null values
            obj1 = create_object_with_null_values(fdp);
            obj2 = create_object_with_null_values(fdp);
            
            if (obj1 && obj2) {
                cJSON_Compare(obj1, obj2, case_sensitive);
                
                // Also test null object vs non-null object
                cJSON* non_null_obj = create_object_with_mixed_types(fdp);
                if (non_null_obj) {
                    cJSON_Compare(obj1, non_null_obj, case_sensitive);
                    cJSON_Delete(non_null_obj);
                }
            }
            break;
        }
        
        case 2: {
            // Scenario 3: Objects with escaped characters
            obj1 = create_object_with_escaped_values(fdp);
            obj2 = create_object_with_escaped_values(fdp);
            
            if (obj1 && obj2) {
                cJSON_Compare(obj1, obj2, case_sensitive);
            }
            
            // Test escaped keys (cJSON may not support but we can try)
            cJSON* escaped_key_obj = cJSON_CreateObject();
            if (escaped_key_obj) {
                std::string escaped_key = "key\\\"with\\nescapes";
                cJSON_AddItemToObject(escaped_key_obj, escaped_key.c_str(), 
                    cJSON_CreateString("value"));
                cJSON_Delete(escaped_key_obj);
            }
            break;
        }
        
        case 3: {
            // Scenario 4: Objects with Unicode keys
            obj1 = cJSON_CreateObject();
            obj2 = cJSON_CreateObject();
            
            if (obj1 && obj2) {
                for (int i = 0; i < 3; i++) {
                    std::string key1 = generate_unicode_key(fdp);
                    std::string key2 = generate_unicode_key(fdp);
                    
                    cJSON_AddItemToObject(obj1, key1.c_str(),
                        cJSON_CreateString(fdp.ConsumeRandomLengthString(16).c_str()));
                    cJSON_AddItemToObject(obj2, key2.c_str(),
                        cJSON_CreateString(fdp.ConsumeRandomLengthString(16).c_str()));
                }
                
                cJSON_Compare(obj1, obj2, case_sensitive);
            }
            break;
        }
        
        case 4: {
            // Scenario 5: Objects with mixed value types
            obj1 = create_object_with_mixed_types(fdp);
            obj2 = create_object_with_mixed_types(fdp);
            
            if (obj1 && obj2) {
                cJSON_Compare(obj1, obj2, case_sensitive);
            }
            break;
        }
        
        case 5: {
            // Scenario 6: Case-sensitive vs insensitive with edge cases
            test_case_sensitive_special_keys(fdp, true);  // case-sensitive
            test_case_sensitive_special_keys(fdp, false); // case-insensitive
            break;
        }
        
        case 6: {
            // Scenario 7: Missing key scenarios (target lines 3166, 3182)
            obj1 = cJSON_CreateObject();
            obj2 = cJSON_CreateObject();
            
            if (obj1 && obj2) {
                // Add different keys to each object
                cJSON_AddItemToObject(obj1, "key_only_in_obj1", cJSON_CreateNumber(1.0));
                cJSON_AddItemToObject(obj2, "key_only_in_obj2", cJSON_CreateNumber(1.0));
                
                // Add one common key
                cJSON_AddItemToObject(obj1, "common_key", cJSON_CreateNumber(42.0));
                cJSON_AddItemToObject(obj2, "common_key", cJSON_CreateNumber(42.0));
                
                cJSON_Compare(obj1, obj2, case_sensitive);
            }
            break;
        }
        
        case 7: {
            // Scenario 8: Value comparison failures (target lines 3171, 3187)
            obj1 = cJSON_CreateObject();
            obj2 = cJSON_CreateObject();
            
            if (obj1 && obj2) {
                // Same keys, different values
                std::string common_key = "same_key_" + generate_special_key(fdp);
                cJSON_AddItemToObject(obj1, common_key.c_str(), 
                    cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                cJSON_AddItemToObject(obj2, common_key.c_str(),
                    cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                
                // Different types for same key
                std::string type_key = "type_key";
                cJSON_AddItemToObject(obj1, type_key.c_str(), cJSON_CreateNumber(1.0));
                cJSON_AddItemToObject(obj2, type_key.c_str(), cJSON_CreateString("1.0"));
                
                cJSON_Compare(obj1, obj2, case_sensitive);
            }
            break;
        }
    }
    
    // Cleanup
    if (obj1) cJSON_Delete(obj1);
    if (obj2) cJSON_Delete(obj2);
    
    // Test duplicate key behavior (separate from main switch)
    if (fdp.ConsumeBool()) {
        test_duplicate_key_behavior(fdp);
    }
    
    return 0;
}

} // namespace harness_024
