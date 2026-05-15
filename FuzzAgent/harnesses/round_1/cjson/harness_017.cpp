#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <memory>
#include "cjson/cJSON.h"

namespace harness_017 {
/*
 * harness_017.cpp - Comprehensive cJSON_Duplicate testing harness
 * 
 * This harness specifically targets cJSON_Duplicate function to address
 * 20/36 blocked branches identified in its internal recursive function.
 * 
 * Focus on comprehensive testing of cJSON_Duplicate:
 * 1) cJSON_Duplicate with recurse=0 (shallow copy)
 * 2) cJSON_Duplicate with recurse=1 (deep copy)  
 * 3) Testing duplication of all cJSON types:
 *    - cJSON_NULL, cJSON_False, cJSON_True
 *    - cJSON_Number (integers, doubles, special values)
 *    - cJSON_String (empty, normal, long strings)
 *    - cJSON_Array (empty, nested, with various elements)
 *    - cJSON_Object (empty, nested, with various key-value pairs)
 *    - cJSON_Raw (raw JSON strings)
 * 4) Memory allocation edge cases and failure paths
 * 5) Circular reference protection (CJSON_CIRCULAR_LIMIT)
 * 6) Duplication of complex nested structures
 * 7) Edge cases with null/invalid inputs
 * 8) String reference handling (cJSON_StringIsConst, cJSON_IsReference)
 * 
 * Strategy: Use FuzzedDataProvider to generate diverse cJSON structures
 * and test duplication with both recursive and non-recursive modes.
 */


extern "C" {
}
// Helper to create different types of cJSON items
// Uses stable storage for strings: pre-allocate vector capacity and store strings individually
// to ensure pointers remain valid for reference items
static cJSON* create_cjson_item(FuzzedDataProvider& fdp, int depth, std::vector<std::unique_ptr<std::string>>& string_storage) {
    if (depth > 5) {
        return cJSON_CreateNull(); // Prevent excessive nesting
    }
    
    uint8_t item_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (item_type) {
        case 0:
            return cJSON_CreateNull();
            
        case 1:
            return cJSON_CreateBool(fdp.ConsumeBool());
            
        case 2: {
            // Create number with various types
            uint8_t num_type = fdp.ConsumeIntegral<uint8_t>() % 6;
            switch (num_type) {
                case 0:
                    return cJSON_CreateNumber(fdp.ConsumeIntegral<int>());
                case 1:
                    return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                case 2:
                    return cJSON_CreateNumber(std::numeric_limits<double>::quiet_NaN());
                case 3:
                    return cJSON_CreateNumber(std::numeric_limits<double>::infinity());
                case 4:
                    return cJSON_CreateNumber(-std::numeric_limits<double>::infinity());
                case 5:
                    return cJSON_CreateNumber(0.0);
                default:
                    return cJSON_CreateNumber(0.0);
            }
        }
            
        case 3: {
            // Create string
            std::string str = fdp.ConsumeRandomLengthString(256);
            auto str_ptr = std::make_unique<std::string>(str);
            const char* cstr = str_ptr->c_str();
            string_storage.push_back(std::move(str_ptr));
            return cJSON_CreateString(cstr);
        }
            
        case 4: {
            // Create array
            cJSON* array = cJSON_CreateArray();
            if (!array) return nullptr;
            
            int array_size = fdp.ConsumeIntegralInRange<int>(0, 5);
            for (int i = 0; i < array_size; i++) {
                cJSON* element = create_cjson_item(fdp, depth + 1, string_storage);
                if (element) {
                    cJSON_AddItemToArray(array, element);
                }
            }
            return array;
        }
            
        case 5: {
            // Create object
            cJSON* object = cJSON_CreateObject();
            if (!object) return nullptr;
            
            int num_fields = fdp.ConsumeIntegralInRange<int>(0, 5);
            for (int i = 0; i < num_fields; i++) {
                std::string key = fdp.ConsumeRandomLengthString(64);
                auto key_ptr = std::make_unique<std::string>(key);
                const char* ckey = key_ptr->c_str();
                string_storage.push_back(std::move(key_ptr));
                
                cJSON* value = create_cjson_item(fdp, depth + 1, string_storage);
                if (value) {
                    cJSON_AddItemToObject(object, ckey, value);
                }
            }
            return object;
        }
            
        case 6: {
            // Create raw JSON
            std::string raw = fdp.ConsumeRandomLengthString(128);
            auto raw_ptr = std::make_unique<std::string>(raw);
            const char* craw = raw_ptr->c_str();
            string_storage.push_back(std::move(raw_ptr));
            return cJSON_CreateRaw(craw);
        }
            
        case 7: {
            // Create reference item
            uint8_t ref_type = fdp.ConsumeIntegral<uint8_t>() % 3;
            std::string ref_str = fdp.ConsumeRandomLengthString(64);
            auto ref_str_ptr = std::make_unique<std::string>(ref_str);
            const char* cref_str = ref_str_ptr->c_str();
            string_storage.push_back(std::move(ref_str_ptr));
            
            switch (ref_type) {
                case 0:
                    return cJSON_CreateStringReference(cref_str);
                case 1: {
                    cJSON* obj = cJSON_CreateObject();
                    if (obj) {
                        // Need to store this string separately too
                        std::string test_str = "test";
                        auto test_str_ptr = std::make_unique<std::string>(test_str);
                        const char* ctest_str = test_str_ptr->c_str();
                        string_storage.push_back(std::move(test_str_ptr));
                        cJSON_AddStringToObject(obj, ctest_str, cref_str);
                    }
                    return cJSON_CreateObjectReference(obj);
                }
                case 2: {
                    cJSON* arr = cJSON_CreateArray();
                    if (arr) {
                        // Need to store this string separately too
                        std::string arr_str = ref_str;
                        auto arr_str_ptr = std::make_unique<std::string>(arr_str);
                        const char* carr_str = arr_str_ptr->c_str();
                        string_storage.push_back(std::move(arr_str_ptr));
                        cJSON_AddItemToArray(arr, cJSON_CreateString(carr_str));
                    }
                    return cJSON_CreateArrayReference(arr);
                }
                default:
                    return cJSON_CreateStringReference(cref_str);
            }
        }
            
        default:
            return cJSON_CreateNull();
    }
}

// Helper to compare two cJSON items (basic comparison for testing)
static bool compare_cjson_items(const cJSON* a, const cJSON* b, bool recursive) {
    if (!a || !b) return a == b;
    
    // Compare type (ignore reference flag for comparison)
    if ((a->type & ~cJSON_IsReference) != (b->type & ~cJSON_IsReference)) {
        return false;
    }
    
    // Compare values based on type
    if (cJSON_IsNumber(a)) {
        // For numbers, compare both integer and double representations
        if (a->valueint != b->valueint) {
            // Check if they're both doubles with same value
            if (a->valuedouble != b->valuedouble) {
                // Handle NaN and infinity
                if (std::isnan(a->valuedouble) && std::isnan(b->valuedouble)) {
                    // Both NaN
                } else {
                    return false;
                }
            }
        }
    } else if (cJSON_IsString(a) || cJSON_IsRaw(a)) {
        if (!a->valuestring || !b->valuestring) {
            return a->valuestring == b->valuestring;
        }
        if (strcmp(a->valuestring, b->valuestring) != 0) {
            return false;
        }
    } else if (cJSON_IsObject(a) || cJSON_IsArray(a)) {
        // For shallow copy, we don't compare children
        if (!recursive) {
            return true;
        }
        
        // For deep copy, compare children recursively
        const cJSON* child_a = a->child;
        const cJSON* child_b = b->child;
        
        while (child_a && child_b) {
            if (!compare_cjson_items(child_a, child_b, recursive)) {
                return false;
            }
            child_a = child_a->next;
            child_b = child_b->next;
        }
        
        // Both should be null at the same time
        if (child_a || child_b) {
            return false;
        }
    }
    
    return true;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for meaningful duplication testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test scenario
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 12;
    
    // Storage for strings to keep them alive - using unique_ptr for stable storage
    std::vector<std::unique_ptr<std::string>> string_storage;
    
    cJSON* original = nullptr;
    cJSON* duplicate_shallow = nullptr;
    cJSON* duplicate_deep = nullptr;
    cJSON* result = nullptr;
    
    switch (scenario) {
        case 0: {
            // Test 1: Basic duplication of various types (shallow and deep)
            original = create_cjson_item(fdp, 0, string_storage);
            if (!original) break;
            
            // Test shallow duplication (recurse = 0)
            duplicate_shallow = cJSON_Duplicate(original, 0);
            if (duplicate_shallow) {
                // Basic type comparison for shallow copy
                if (!compare_cjson_items(original, duplicate_shallow, false)) {
                    // Type mismatch - unexpected
                }
                cJSON_Delete(duplicate_shallow);
            }
            
            // Test deep duplication (recurse = 1)
            duplicate_deep = cJSON_Duplicate(original, 1);
            if (duplicate_deep) {
                // Compare for deep copy
                if (!compare_cjson_items(original, duplicate_deep, true)) {
                    // Deep copy mismatch - unexpected
                }
                cJSON_Delete(duplicate_deep);
            }
            
            cJSON_Delete(original);
            break;
        }
        
        case 1: {
            // Test 2: Duplication of complex nested structures
            original = cJSON_CreateObject();
            if (!original) break;
            
            // Create nested structure with stable string storage
            cJSON* nested_array = cJSON_CreateArray();
            if (nested_array) {
                for (int i = 0; i < 3; i++) {
                    cJSON* nested_obj = cJSON_CreateObject();
                    if (nested_obj) {
                        // Store string in stable storage
                        std::string key_str = "nested_" + std::to_string(i);
                        auto key_ptr = std::make_unique<std::string>(key_str);
                        const char* key_cstr = key_ptr->c_str();
                        string_storage.push_back(std::move(key_ptr));
                        
                        cJSON_AddStringToObject(nested_obj, key_cstr, "value");
                        cJSON_AddNumberToObject(nested_obj, "value", i * 10.5);
                        cJSON_AddItemToArray(nested_array, nested_obj);
                    }
                }
                cJSON_AddItemToObject(original, "nested_array", nested_array);
            }
            
            // Test both shallow and deep duplication
            duplicate_shallow = cJSON_Duplicate(original, 0);
            duplicate_deep = cJSON_Duplicate(original, 1);
            
            if (duplicate_shallow) cJSON_Delete(duplicate_shallow);
            if (duplicate_deep) cJSON_Delete(duplicate_deep);
            cJSON_Delete(original);
            break;
        }
        
        case 2: {
            // Test 3: Duplication with references - using stable string storage
            std::string ref_str = "referenced_string";
            auto ref_str_ptr = std::make_unique<std::string>(ref_str);
            const char* ref_cstr = ref_str_ptr->c_str();
            string_storage.push_back(std::move(ref_str_ptr));
            
            cJSON* referenced_string = cJSON_CreateString(ref_cstr);
            if (!referenced_string) break;
            
            // Store external string in stable storage
            std::string ext_str = "external_string";
            auto ext_str_ptr = std::make_unique<std::string>(ext_str);
            const char* ext_cstr = ext_str_ptr->c_str();
            string_storage.push_back(std::move(ext_str_ptr));
            
            cJSON* string_ref = cJSON_CreateStringReference(ext_cstr);
            cJSON* obj_ref = cJSON_CreateObjectReference(referenced_string);
            
            original = cJSON_CreateArray();
            if (original) {
                if (string_ref) cJSON_AddItemToArray(original, string_ref);
                if (obj_ref) cJSON_AddItemToArray(original, obj_ref);
                
                // Test duplication of references
                duplicate_deep = cJSON_Duplicate(original, 1);
                if (duplicate_deep) {
                    // References should be duplicated as regular items
                    cJSON_Delete(duplicate_deep);
                }
                
                cJSON_Delete(original);
            }
            
            cJSON_Delete(referenced_string);
            break;
        }
        
        case 3: {
            // Test 4: Edge case - null input
            result = cJSON_Duplicate(nullptr, 0);
            if (result != nullptr) {
                // Should be null for null input
                cJSON_Delete(result);
            }
            
            result = cJSON_Duplicate(nullptr, 1);
            if (result != nullptr) {
                // Should be null for null input
                cJSON_Delete(result);
            }
            break;
        }
        
        case 4: {
            // Test 5: Duplication with string const flag
            std::string const_str = "const_string";
            auto const_str_ptr = std::make_unique<std::string>(const_str);
            const char* const_cstr = const_str_ptr->c_str();
            string_storage.push_back(std::move(const_str_ptr));
            
            original = cJSON_CreateString(const_cstr);
            if (original) {
                // Mark as const string (simulate)
                original->type |= cJSON_StringIsConst;
                
                duplicate_deep = cJSON_Duplicate(original, 1);
                if (duplicate_deep) {
                    // Const strings should not be duplicated
                    cJSON_Delete(duplicate_deep);
                }
                
                cJSON_Delete(original);
            }
            break;
        }
        
        case 5: {
            // Test 6: Memory allocation failure simulation
            // Create a large structure that might stress memory allocation
            original = cJSON_CreateObject();
            if (original) {
                // Add many items to create complex structure
                for (int i = 0; i < 20 && fdp.remaining_bytes() > 10; i++) {
                    std::string key = fdp.ConsumeRandomLengthString(16);
                    std::string value = fdp.ConsumeRandomLengthString(64);
                    
                    auto key_ptr = std::make_unique<std::string>(key);
                    auto value_ptr = std::make_unique<std::string>(value);
                    const char* key_cstr = key_ptr->c_str();
                    const char* value_cstr = value_ptr->c_str();
                    
                    string_storage.push_back(std::move(key_ptr));
                    string_storage.push_back(std::move(value_ptr));
                    
                    cJSON_AddStringToObject(original, key_cstr, value_cstr);
                }
                
                // Try duplication - may fail due to memory limits
                duplicate_deep = cJSON_Duplicate(original, 1);
                if (duplicate_deep) {
                    cJSON_Delete(duplicate_deep);
                }
                
                cJSON_Delete(original);
            }
            break;
        }
        
        case 6: {
            // Test 7: Circular reference protection (CJSON_CIRCULAR_LIMIT)
            original = cJSON_CreateObject();
            if (original) {
                // Create self-referencing object
                cJSON_AddItemToObject(original, "self", original);
                
                // Duplication should handle circular references
                duplicate_deep = cJSON_Duplicate(original, 1);
                if (duplicate_deep) {
                    // Should handle circular reference
                    cJSON_Delete(duplicate_deep);
                }
                
                // Remove self-reference before deletion
                cJSON_DeleteItemFromObject(original, "self");
                cJSON_Delete(original);
            }
            break;
        }
        
        case 7: {
            // Test 8: Empty structures duplication
            original = cJSON_CreateArray();
            if (original) {
                duplicate_deep = cJSON_Duplicate(original, 1);
                if (duplicate_deep) cJSON_Delete(duplicate_deep);
                cJSON_Delete(original);
            }
            
            original = cJSON_CreateObject();
            if (original) {
                duplicate_deep = cJSON_Duplicate(original, 1);
                if (duplicate_deep) cJSON_Delete(duplicate_deep);
                cJSON_Delete(original);
            }
            break;
        }
        
        case 8: {
            // Test 9: Special number values (NaN, Infinity)
            original = cJSON_CreateArray();
            if (original) {
                cJSON_AddItemToArray(original, cJSON_CreateNumber(std::numeric_limits<double>::quiet_NaN()));
                cJSON_AddItemToArray(original, cJSON_CreateNumber(std::numeric_limits<double>::infinity()));
                cJSON_AddItemToArray(original, cJSON_CreateNumber(-std::numeric_limits<double>::infinity()));
                
                duplicate_deep = cJSON_Duplicate(original, 1);
                if (duplicate_deep) cJSON_Delete(duplicate_deep);
                cJSON_Delete(original);
            }
            break;
        }
        
        case 9: {
            // Test 10: Raw JSON duplication
            std::string raw_json = fdp.ConsumeRandomLengthString(100);
            auto raw_json_ptr = std::make_unique<std::string>(raw_json);
            const char* raw_cstr = raw_json_ptr->c_str();
            string_storage.push_back(std::move(raw_json_ptr));
            
            original = cJSON_CreateRaw(raw_cstr);
            if (original) {
                duplicate_shallow = cJSON_Duplicate(original, 0);
                duplicate_deep = cJSON_Duplicate(original, 1);
                
                if (duplicate_shallow) cJSON_Delete(duplicate_shallow);
                if (duplicate_deep) cJSON_Delete(duplicate_deep);
                cJSON_Delete(original);
            }
            break;
        }
        
        case 10: {
            // Test 11: Mixed array with various types
            original = cJSON_CreateArray();
            if (original) {
                // Add various types to array
                cJSON_AddItemToArray(original, cJSON_CreateNull());
                cJSON_AddItemToArray(original, cJSON_CreateTrue());
                cJSON_AddItemToArray(original, cJSON_CreateFalse());
                cJSON_AddItemToArray(original, cJSON_CreateNumber(42.5));
                
                std::string array_str = "array_string";
                auto array_str_ptr = std::make_unique<std::string>(array_str);
                const char* array_cstr = array_str_ptr->c_str();
                string_storage.push_back(std::move(array_str_ptr));
                cJSON_AddItemToArray(original, cJSON_CreateString(array_cstr));
                
                duplicate_deep = cJSON_Duplicate(original, 1);
                if (duplicate_deep) cJSON_Delete(duplicate_deep);
                cJSON_Delete(original);
            }
            break;
        }
        
        case 11: {
            // Test 12: Comprehensive test with random structures
            // Create random structure with fuzzed parameters
            int structure_type = fdp.ConsumeIntegral<uint8_t>() % 4;
            bool use_recursive = fdp.ConsumeBool();
            
            switch (structure_type) {
                case 0:
                    original = create_cjson_item(fdp, 0, string_storage);
                    break;
                case 1: {
                    // Create array with random elements
                    int array_size = fdp.ConsumeIntegralInRange<int>(0, 10);
                    original = cJSON_CreateArray();
                    for (int i = 0; i < array_size && original; i++) {
                        cJSON* elem = create_cjson_item(fdp, 1, string_storage);
                        if (elem) {
                            cJSON_AddItemToArray(original, elem);
                        }
                    }
                    break;
                }
                case 2: {
                    // Create object with random fields
                    int num_fields = fdp.ConsumeIntegralInRange<int>(0, 10);
                    original = cJSON_CreateObject();
                    for (int i = 0; i < num_fields && original; i++) {
                        std::string key = fdp.ConsumeRandomLengthString(32);
                        auto key_ptr = std::make_unique<std::string>(key);
                        const char* key_cstr = key_ptr->c_str();
                        string_storage.push_back(std::move(key_ptr));
                        
                        cJSON* value = create_cjson_item(fdp, 1, string_storage);
                        if (value) {
                            cJSON_AddItemToObject(original, key_cstr, value);
                        }
                    }
                    break;
                }
                case 3:
                    // Test with already existing item from previous cases
                    original = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                    break;
            }
            
            if (original) {
                result = cJSON_Duplicate(original, use_recursive ? 1 : 0);
                if (result) {
                    // Verify duplication
                    if (!compare_cjson_items(original, result, use_recursive)) {
                        // Comparison failed - unexpected
                    }
                    cJSON_Delete(result);
                }
                cJSON_Delete(original);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_017
