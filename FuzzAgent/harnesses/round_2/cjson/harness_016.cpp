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

namespace harness_016 {

extern "C" {
}

// Helper to create JSON values of specific types based on fuzzer input
cJSON* create_json_value(FuzzedDataProvider& fdp, int depth, bool create_variation) {
    if (depth <= 0 || fdp.remaining_bytes() < 16) {
        uint8_t type = fdp.ConsumeIntegral<uint8_t>() % 9;
        switch (type) {
            case 0: return cJSON_CreateFalse();
            case 1: return cJSON_CreateTrue();
            case 2: return cJSON_CreateNull();
            case 3: {
                double num = fdp.ConsumeFloatingPoint<double>();
                return cJSON_CreateNumber(num);
            }
            case 4: {
                std::string str = fdp.ConsumeRandomLengthString(64);
                return cJSON_CreateString(str.c_str());
            }
            case 5: {
                std::string raw = fdp.ConsumeRandomLengthString(64);
                return cJSON_CreateRaw(raw.c_str());
            }
            case 6: return cJSON_CreateArray();
            case 7: return cJSON_CreateObject();
            case 8: {
                // Create string with potential NULL valuestring (edge case)
                cJSON* str = cJSON_CreateString("");
                if (str && fdp.ConsumeBool()) {
                    free((void*)str->valuestring);
                    str->valuestring = NULL;
                }
                return str;
            }
            default: return NULL;
        }
    }
    
    uint8_t structure_type = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    if (structure_type == 0) {
        // Create array
        cJSON* arr = cJSON_CreateArray();
        if (!arr) return NULL;
        
        int elem_count = fdp.ConsumeIntegralInRange<int>(1, 8);
        for (int i = 0; i < elem_count && fdp.remaining_bytes() > 0; i++) {
            // For variation testing, sometimes skip elements
            if (create_variation && (fdp.ConsumeIntegral<uint8_t>() % 5 == 0)) {
                continue;
            }
            cJSON* elem = create_json_value(fdp, depth - 1, create_variation);
            if (elem) {
                cJSON_AddItemToArray(arr, elem);
            }
        }
        return arr;
    } else {
        // Create object
        cJSON* obj = cJSON_CreateObject();
        if (!obj) return NULL;
        
        int prop_count = fdp.ConsumeIntegralInRange<int>(1, 10);
        for (int i = 0; i < prop_count && fdp.remaining_bytes() > 0; i++) {
            std::string key = fdp.ConsumeRandomLengthString(20);
            
            // For variation testing, create key differences
            if (create_variation) {
                uint8_t var_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                if (var_type == 0) {
                    // Skip this key (object will be missing this)
                    continue;
                } else if (var_type == 1) {
                    // Change case for case-sensitive testing
                    if (!key.empty() && fdp.ConsumeBool()) {
                        key[0] = islower(key[0]) ? toupper(key[0]) : tolower(key[0]);
                    }
                } else if (var_type == 2) {
                    // Append suffix for different key
                    key += "_diff";
                }
            }
            
            cJSON* value = create_json_value(fdp, depth - 1, create_variation);
            if (value) {
                cJSON_AddItemToObject(obj, key.c_str(), value);
            }
        }
        return obj;
    }
}

// Helper to create numeric edge cases
cJSON* create_numeric_edge_case(FuzzedDataProvider& fdp) {
    uint8_t case_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    switch (case_type) {
        case 0: return cJSON_CreateNumber(std::numeric_limits<double>::quiet_NaN());
        case 1: return cJSON_CreateNumber(std::numeric_limits<double>::infinity());
        case 2: return cJSON_CreateNumber(-std::numeric_limits<double>::infinity());
        case 3: return cJSON_CreateNumber(std::numeric_limits<double>::max());
        case 4: return cJSON_CreateNumber(std::numeric_limits<double>::min());
        case 5: return cJSON_CreateNumber(std::numeric_limits<double>::epsilon());
        case 6: return cJSON_CreateNumber(std::numeric_limits<int>::max());
        case 7: return cJSON_CreateNumber(std::numeric_limits<int>::min());
        default: return cJSON_CreateNumber(0.0);
    }
}

// Test specific comparison scenarios systematically
void test_comparison_scenarios(FuzzedDataProvider& fdp) {
    // 1. NULL pointer tests (lines 3078-3081)
    cJSON_Compare(NULL, NULL, fdp.ConsumeBool());
    cJSON* valid_obj = cJSON_CreateObject();
    if (valid_obj) {
        cJSON_Compare(NULL, valid_obj, fdp.ConsumeBool());
        cJSON_Compare(valid_obj, NULL, fdp.ConsumeBool());
        cJSON_Delete(valid_obj);
    }
    
    // 2. Type mismatch tests (line 3078)
    cJSON* num = cJSON_CreateNumber(42.0);
    cJSON* str = cJSON_CreateString("test");
    if (num && str) {
        cJSON_Compare(num, str, fdp.ConsumeBool());
        cJSON_Compare(str, num, fdp.ConsumeBool());
    }
    if (num) cJSON_Delete(num);
    if (str) cJSON_Delete(str);
    
    // 3. Same pointer test (lines 3101-3104)
    cJSON* self = cJSON_CreateObject();
    if (self) {
        cJSON_Compare(self, self, fdp.ConsumeBool());
        cJSON_Delete(self);
    }
    
    // 4. Boolean/Null type comparisons (lines 3109-3112)
    cJSON* bool_true1 = cJSON_CreateTrue();
    cJSON* bool_true2 = cJSON_CreateTrue();
    cJSON* bool_false1 = cJSON_CreateFalse();
    cJSON* bool_false2 = cJSON_CreateFalse();
    cJSON* null1 = cJSON_CreateNull();
    cJSON* null2 = cJSON_CreateNull();
    
    if (bool_true1 && bool_true2) {
        cJSON_Compare(bool_true1, bool_true2, fdp.ConsumeBool());
    }
    if (bool_false1 && bool_false2) {
        cJSON_Compare(bool_false1, bool_false2, fdp.ConsumeBool());
    }
    if (bool_true1 && bool_false1) {
        cJSON_Compare(bool_true1, bool_false1, fdp.ConsumeBool());
    }
    if (null1 && null2) {
        cJSON_Compare(null1, null2, fdp.ConsumeBool());
    }
    
    if (bool_true1) cJSON_Delete(bool_true1);
    if (bool_true2) cJSON_Delete(bool_true2);
    if (bool_false1) cJSON_Delete(bool_false1);
    if (bool_false2) cJSON_Delete(bool_false2);
    if (null1) cJSON_Delete(null1);
    if (null2) cJSON_Delete(null2);
    
    // 5. Numeric comparisons with edge cases (lines 3114-3119)
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
        cJSON* num1 = create_numeric_edge_case(fdp);
        cJSON* num2 = create_numeric_edge_case(fdp);
        if (num1 && num2) {
            cJSON_Compare(num1, num2, fdp.ConsumeBool());
        }
        if (num1) cJSON_Delete(num1);
        if (num2) cJSON_Delete(num2);
    }
    
    // 6. String/Raw comparisons with NULL valuestring (lines 3123-3126)
    cJSON* str_null1 = cJSON_CreateString("");
    cJSON* str_null2 = cJSON_CreateString("");
    cJSON* str_valid = cJSON_CreateString("valid");
    
    if (str_null1 && str_null2 && str_valid) {
        // Create NULL valuestring edge case
        free((void*)str_null1->valuestring);
        str_null1->valuestring = NULL;
        
        free((void*)str_null2->valuestring);
        str_null2->valuestring = NULL;
        
        cJSON_Compare(str_null1, str_null2, fdp.ConsumeBool()); // Both NULL
        cJSON_Compare(str_null1, str_valid, fdp.ConsumeBool()); // One NULL
        cJSON_Compare(str_valid, str_null1, fdp.ConsumeBool()); // Other NULL
        
        // Test raw strings too
        cJSON* raw1 = cJSON_CreateRaw("raw1");
        cJSON* raw2 = cJSON_CreateRaw("raw2");
        if (raw1 && raw2) {
            cJSON_Compare(raw1, raw2, fdp.ConsumeBool());
            cJSON_Compare(raw1, raw1, fdp.ConsumeBool());
        }
        if (raw1) cJSON_Delete(raw1);
        if (raw2) cJSON_Delete(raw2);
    }
    
    if (str_null1) cJSON_Delete(str_null1);
    if (str_null2) cJSON_Delete(str_null2);
    if (str_valid) cJSON_Delete(str_valid);
    
    // 7. Array comparisons (lines 3134-3156)
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
        cJSON* arr1 = create_json_value(fdp, 2, false);
        cJSON* arr2 = create_json_value(fdp, 2, fdp.ConsumeBool()); // Possibly different
        
        if (arr1 && arr2) {
            // Test same array
            cJSON_Compare(arr1, arr1, fdp.ConsumeBool());
            
            // Test potentially different arrays
            cJSON_Compare(arr1, arr2, fdp.ConsumeBool());
            
            // Create arrays of different lengths
            cJSON* short_arr = cJSON_CreateArray();
            cJSON* long_arr = cJSON_CreateArray();
            
            if (short_arr && long_arr) {
                for (int j = 0; j < 2; j++) {
                    cJSON_AddItemToArray(short_arr, cJSON_CreateNumber(j));
                }
                for (int j = 0; j < 5; j++) {
                    cJSON_AddItemToArray(long_arr, cJSON_CreateNumber(j));
                }
                
                cJSON_Compare(short_arr, long_arr, fdp.ConsumeBool());
                cJSON_Compare(long_arr, short_arr, fdp.ConsumeBool());
            }
            
            if (short_arr) cJSON_Delete(short_arr);
            if (long_arr) cJSON_Delete(long_arr);
        }
        
        if (arr1) cJSON_Delete(arr1);
        if (arr2) cJSON_Delete(arr2);
    }
    
    // 8. Object comparisons with case sensitivity (lines 3158-3194)
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
        bool case_sensitive = fdp.ConsumeBool();
        
        // Create base object
        cJSON* base_obj = cJSON_CreateObject();
        if (!base_obj) continue;
        
        // Add some properties
        for (int j = 0; j < 4 && fdp.remaining_bytes() > 0; j++) {
            std::string key = "key" + std::to_string(j);
            cJSON_AddItemToObject(base_obj, key.c_str(), cJSON_CreateNumber(j));
        }
        
        // Test identical object
        cJSON_Compare(base_obj, base_obj, case_sensitive);
        
        // Create identical copy
        char* printed = cJSON_Print(base_obj);
        cJSON* copy_obj = printed ? cJSON_Parse(printed) : NULL;
        if (printed) free(printed);
        
        if (copy_obj) {
            cJSON_Compare(base_obj, copy_obj, case_sensitive);
            cJSON_Delete(copy_obj);
        }
        
        // Create object with different values
        cJSON* diff_val_obj = cJSON_CreateObject();
        if (diff_val_obj) {
            for (int j = 0; j < 4 && fdp.remaining_bytes() > 0; j++) {
                std::string key = "key" + std::to_string(j);
                cJSON_AddItemToObject(diff_val_obj, key.c_str(), cJSON_CreateNumber(j * 2));
            }
            cJSON_Compare(base_obj, diff_val_obj, case_sensitive);
            cJSON_Delete(diff_val_obj);
        }
        
        // Create object with different keys (subset)
        cJSON* subset_obj = cJSON_CreateObject();
        if (subset_obj) {
            for (int j = 0; j < 2 && fdp.remaining_bytes() > 0; j++) {
                std::string key = "key" + std::to_string(j);
                cJSON_AddItemToObject(subset_obj, key.c_str(), cJSON_CreateNumber(j));
            }
            cJSON_Compare(base_obj, subset_obj, case_sensitive);
            cJSON_Compare(subset_obj, base_obj, case_sensitive);
            cJSON_Delete(subset_obj);
        }
        
        // Test case sensitivity for object keys
        if (!case_sensitive) {
            cJSON* case_obj = cJSON_CreateObject();
            if (case_obj) {
                // Use different case for same logical key
                cJSON_AddItemToObject(case_obj, "KEY0", cJSON_CreateNumber(0));
                cJSON_AddItemToObject(case_obj, "Key1", cJSON_CreateNumber(1));
                cJSON_AddItemToObject(case_obj, "kEy2", cJSON_CreateNumber(2));
                
                cJSON_Compare(base_obj, case_obj, case_sensitive);
                cJSON_Delete(case_obj);
            }
        }
        
        cJSON_Delete(base_obj);
    }
    
    // 9. Mixed nested structures
    for (int i = 0; i < 2 && fdp.remaining_bytes() > 0; i++) {
        cJSON* complex1 = create_json_value(fdp, 3, false);
        cJSON* complex2 = create_json_value(fdp, 3, fdp.ConsumeBool());
        
        if (complex1 && complex2) {
            bool case_sensitive = fdp.ConsumeBool();
            cJSON_Compare(complex1, complex2, case_sensitive);
            cJSON_Compare(complex1, complex1, case_sensitive);
            cJSON_Compare(complex2, complex2, case_sensitive);
        }
        
        if (complex1) cJSON_Delete(complex1);
        if (complex2) cJSON_Delete(complex2);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Run systematic comparison tests
    test_comparison_scenarios(fdp);
    
    // Additionally, generate random pairs for broader exploration
    while (fdp.remaining_bytes() > 64) {
        // Determine test type
        uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 10;
        bool case_sensitive = fdp.ConsumeBool();
        
        cJSON* item1 = NULL;
        cJSON* item2 = NULL;
        
        switch (test_type) {
            case 0: // Identical simple objects
                item1 = create_json_value(fdp, 1, false);
                item2 = item1 ? cJSON_Duplicate(item1, 1) : NULL;
                break;
                
            case 1: // Different types
                item1 = create_json_value(fdp, 1, false);
                // Force different type
                if (item1 && (item1->type & 0xFF) == cJSON_Number) {
                    item2 = cJSON_CreateString("different");
                } else {
                    item2 = cJSON_CreateNumber(42.0);
                }
                break;
                
            case 2: // Nested structures with variations
                item1 = create_json_value(fdp, 3, false);
                item2 = create_json_value(fdp, 3, true); // Create with variations
                break;
                
            case 3: // Empty vs non-empty
                item1 = cJSON_CreateObject();
                item2 = cJSON_CreateObject();
                if (item2) {
                    cJSON_AddItemToObject(item2, "key", cJSON_CreateString("value"));
                }
                break;
                
            case 4: // Arrays with same/different contents
                item1 = cJSON_CreateArray();
                item2 = cJSON_CreateArray();
                if (item1 && item2) {
                    int count = fdp.ConsumeIntegralInRange<int>(1, 5);
                    for (int i = 0; i < count; i++) {
                        cJSON* elem = cJSON_CreateNumber(i);
                        if (elem) {
                            cJSON_AddItemToArray(item1, elem);
                            // Sometimes add different value to second array
                            if (fdp.ConsumeBool()) {
                                cJSON_AddItemToArray(item2, cJSON_Duplicate(elem, 1));
                            } else {
                                cJSON_AddItemToArray(item2, cJSON_CreateNumber(i * 2));
                            }
                        }
                    }
                }
                break;
                
            case 5: // Strings with special characters
                {
                    std::string str1 = fdp.ConsumeRandomLengthString(50);
                    std::string str2 = str1; // Start identical
                    
                    // Maybe modify second string
                    if (fdp.ConsumeBool() && !str2.empty()) {
                        size_t pos = fdp.ConsumeIntegralInRange<size_t>(0, str2.length() - 1);
                        str2[pos] = str2[pos] ^ 1; // Flip one bit
                    }
                    
                    item1 = cJSON_CreateString(str1.c_str());
                    item2 = cJSON_CreateString(str2.c_str());
                }
                break;
                
            case 6: // Raw JSON comparisons
                {
                    std::string raw1 = fdp.ConsumeRandomLengthString(50);
                    std::string raw2 = fdp.ConsumeRandomLengthString(50);
                    item1 = cJSON_CreateRaw(raw1.c_str());
                    item2 = cJSON_CreateRaw(raw2.c_str());
                }
                break;
                
            case 7: // Reference items (if supported)
                item1 = create_json_value(fdp, 2, false);
                item2 = item1 ? cJSON_Duplicate(item1, 0) : NULL; // shallow copy
                break;
                
            case 8: // Invalid type objects (to trigger default case line 3197)
                // Create manually initialized object with invalid type
                item1 = (cJSON*)malloc(sizeof(cJSON));
                if (item1) {
                    memset(item1, 0, sizeof(cJSON));
                    item1->type = 0xFF; // Invalid type
                }
                item2 = (cJSON*)malloc(sizeof(cJSON));
                if (item2) {
                    memset(item2, 0, sizeof(cJSON));
                    item2->type = 0xFF; // Invalid type
                }
                break;
                
            case 9: // Mixed depth structures
                item1 = create_json_value(fdp, fdp.ConsumeIntegralInRange<int>(1, 4), false);
                item2 = create_json_value(fdp, fdp.ConsumeIntegralInRange<int>(1, 4), false);
                break;
        }
        
        if (item1 && item2) {
            cJSON_Compare(item1, item2, case_sensitive);
        }
        
        // Cleanup
        if (item1) {
            if (test_type == 8) { // Manually allocated
                free(item1);
            } else {
                cJSON_Delete(item1);
            }
        }
        if (item2) {
            if (test_type == 8) { // Manually allocated
                free(item2);
            } else if (test_type == 7) { // Shallow copy
                // Don't delete shallow copy fully if it shares data with item1
                free(item2);
            } else {
                cJSON_Delete(item2);
            }
        }
        
        // Break if we're running out of input
        if (fdp.remaining_bytes() < 128) {
            break;
        }
    }
    
    return 0;
}

} // namespace harness_016
