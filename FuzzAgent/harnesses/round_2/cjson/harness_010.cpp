#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_010 {

extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) {
        return 0;  // Need sufficient input for comprehensive comparison testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume test scenario selector
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 10;
    
    // Test Scenario 0: NULL parameter cases (targeting lines 3078-3081)
    if (test_scenario == 0) {
        // Create some valid JSON objects first with fuzzer input
        cJSON* valid_obj = cJSON_CreateObject();
        if (valid_obj != NULL) {
            // Consume key and value from fuzzer input
            std::string key = fdp.ConsumeRandomLengthString(16);
            double value = fdp.ConsumeFloatingPoint<double>();
            cJSON_AddNumberToObject(valid_obj, key.c_str(), value);
            
            // Test NULL vs NULL
            cJSON_bool result1 = cJSON_Compare(NULL, NULL, true);
            // Should return false according to line 3078
            
            // Test NULL vs valid object
            cJSON_bool result2 = cJSON_Compare(NULL, valid_obj, true);
            
            // Test valid object vs NULL
            cJSON_bool result3 = cJSON_Compare(valid_obj, NULL, true);
            
            // Test valid object vs valid object (baseline)
            cJSON_bool result4 = cJSON_Compare(valid_obj, valid_obj, true);
            // Should return true (identical objects)
            
            cJSON_Delete(valid_obj);
        }
        return 0;
    }
    
    // Test Scenario 1: Type mismatch comparisons (all type combinations)
    if (test_scenario == 1) {
        uint8_t type1 = fdp.ConsumeIntegral<uint8_t>() % 8;
        uint8_t type2 = fdp.ConsumeIntegral<uint8_t>() % 8;
        
        cJSON* item1 = NULL;
        cJSON* item2 = NULL;
        
        // Helper to create item of specific type
        auto create_item = [&](uint8_t type, bool second = false) -> cJSON* {
            switch (type) {
                case 0: // cJSON_False
                    return cJSON_CreateFalse();
                case 1: // cJSON_True
                    return cJSON_CreateTrue();
                case 2: // cJSON_NULL
                    return cJSON_CreateNull();
                case 3: // cJSON_Number
                    return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                case 4: // cJSON_String
                    return cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
                case 5: // cJSON_Array
                    {
                        cJSON* arr = cJSON_CreateArray();
                        int count = fdp.ConsumeIntegralInRange<int>(0, 3);
                        for (int i = 0; i < count && fdp.remaining_bytes() > 0; i++) {
                            cJSON_AddItemToArray(arr, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                        }
                        return arr;
                    }
                case 6: // cJSON_Object
                    {
                        cJSON* obj = cJSON_CreateObject();
                        int count = fdp.ConsumeIntegralInRange<int>(0, 3);
                        for (int i = 0; i < count && fdp.remaining_bytes() > 0; i++) {
                            std::string key = fdp.ConsumeRandomLengthString(8);
                            cJSON_AddNumberToObject(obj, key.c_str(), fdp.ConsumeFloatingPoint<double>());
                        }
                        return obj;
                    }
                case 7: // cJSON_Raw
                    {
                        std::string raw_str = fdp.ConsumeRandomLengthString(32);
                        return cJSON_CreateRaw(raw_str.c_str());
                    }
                default:
                    return NULL;
            }
        };
        
        item1 = create_item(type1);
        item2 = create_item(type2, true);
        
        if (item1 != NULL && item2 != NULL) {
            bool case_sensitive = fdp.ConsumeBool();
            cJSON_bool result = cJSON_Compare(item1, item2, case_sensitive);
            
            // If types are different, result should be false (line 3078)
            // If types are same, result depends on values
        }
        
        if (item1 != NULL) cJSON_Delete(item1);
        if (item2 != NULL) cJSON_Delete(item2);
        return 0;
    }
    
    // Test Scenario 2: Number comparisons with edge cases (NaN, Inf, large/small)
    if (test_scenario == 2) {
        // Generate special double values
        double special_values[] = {
            0.0, -0.0,
            std::numeric_limits<double>::infinity(),
            -std::numeric_limits<double>::infinity(),
            std::numeric_limits<double>::quiet_NaN(),
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::min(),
            std::numeric_limits<double>::epsilon(),
            std::numeric_limits<double>::denorm_min()
        };
        
        // Create number pairs for comparison
        int num_pairs = fdp.ConsumeIntegralInRange<int>(1, 5);
        for (int i = 0; i < num_pairs && fdp.remaining_bytes() > 0; i++) {
            double val1, val2;
            
            // Sometimes use special values, sometimes random
            if (fdp.ConsumeBool()) {
                val1 = special_values[fdp.ConsumeIntegralInRange<size_t>(0, sizeof(special_values)/sizeof(special_values[0]) - 1)];
            } else {
                val1 = fdp.ConsumeFloatingPoint<double>();
            }
            
            if (fdp.ConsumeBool()) {
                val2 = special_values[fdp.ConsumeIntegralInRange<size_t>(0, sizeof(special_values)/sizeof(special_values[0]) - 1)];
            } else {
                val2 = fdp.ConsumeFloatingPoint<double>();
            }
            
            cJSON* num1 = cJSON_CreateNumber(val1);
            cJSON* num2 = cJSON_CreateNumber(val2);
            
            if (num1 != NULL && num2 != NULL) {
                bool case_sensitive = fdp.ConsumeBool(); // Not used for numbers but still parameter
                cJSON_bool result = cJSON_Compare(num1, num2, case_sensitive);
                
                // Also test identical numbers
                cJSON* num1_copy = cJSON_CreateNumber(val1);
                cJSON_bool self_compare = cJSON_Compare(num1, num1_copy, case_sensitive);
                if (num1_copy != NULL) cJSON_Delete(num1_copy);
            }
            
            if (num1 != NULL) cJSON_Delete(num1);
            if (num2 != NULL) cJSON_Delete(num2);
        }
        return 0;
    }
    
    // Test Scenario 3: String comparisons with edge cases
    if (test_scenario == 3) {
        // Test various string cases
        std::vector<std::string> test_strings;
        
        // Add edge cases
        test_strings.push_back("");  // Empty string
        test_strings.push_back("a"); // Single character
        test_strings.push_back("Hello"); // Normal string
        test_strings.push_back("HELLO"); // Uppercase
        test_strings.push_back("hello"); // Lowercase
        test_strings.push_back("Hello World"); // With space
        test_strings.push_back("Hello\tWorld"); // With tab
        test_strings.push_back("Hello\nWorld"); // With newline
        
        // Add fuzzer-generated strings
        int fuzz_strings = fdp.ConsumeIntegralInRange<int>(0, 5);
        for (int i = 0; i < fuzz_strings && fdp.remaining_bytes() > 0; i++) {
            test_strings.push_back(fdp.ConsumeRandomLengthString(64));
        }
        
        // Test comparisons between all pairs
        for (size_t i = 0; i < test_strings.size(); i++) {
            for (size_t j = 0; j < test_strings.size(); j++) {
                cJSON* str1 = cJSON_CreateString(test_strings[i].c_str());
                cJSON* str2 = cJSON_CreateString(test_strings[j].c_str());
                
                if (str1 != NULL && str2 != NULL) {
                    // Test both case-sensitive and case-insensitive
                    cJSON_bool result_sensitive = cJSON_Compare(str1, str2, true);
                    cJSON_bool result_insensitive = cJSON_Compare(str1, str2, false);
                }
                
                if (str1 != NULL) cJSON_Delete(str1);
                if (str2 != NULL) cJSON_Delete(str2);
            }
        }
        
        // Test NULL string values (line 3123)
        cJSON* str_with_null = cJSON_CreateString(NULL);
        if (str_with_null != NULL) {
            cJSON* normal_str = cJSON_CreateString("test");
            if (normal_str != NULL) {
                cJSON_bool result1 = cJSON_Compare(str_with_null, normal_str, true);
                cJSON_bool result2 = cJSON_Compare(normal_str, str_with_null, true);
                cJSON_bool result3 = cJSON_Compare(str_with_null, str_with_null, true);
                cJSON_Delete(normal_str);
            }
            cJSON_Delete(str_with_null);
        }
        
        return 0;
    }
    
    // Test Scenario 4: Array comparisons
    if (test_scenario == 4) {
        // Create arrays with different characteristics
        
        // Array 1: Simple array
        cJSON* arr1 = cJSON_CreateArray();
        int arr1_size = fdp.ConsumeIntegralInRange<int>(0, 5);
        for (int i = 0; i < arr1_size && fdp.remaining_bytes() > 0; i++) {
            int elem_type = fdp.ConsumeIntegralInRange<int>(0, 3);
            switch (elem_type) {
                case 0:
                    cJSON_AddItemToArray(arr1, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                    break;
                case 1:
                    cJSON_AddItemToArray(arr1, cJSON_CreateString(fdp.ConsumeRandomLengthString(16).c_str()));
                    break;
                case 2:
                    cJSON_AddItemToArray(arr1, cJSON_CreateBool(fdp.ConsumeBool()));
                    break;
                case 3:
                    cJSON_AddItemToArray(arr1, cJSON_CreateNull());
                    break;
            }
        }
        
        // Array 2: Same length, different elements
        cJSON* arr2 = cJSON_CreateArray();
        for (int i = 0; i < arr1_size && fdp.remaining_bytes() > 0; i++) {
            // Create different elements
            cJSON_AddItemToArray(arr2, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
        }
        
        // Array 3: Different length
        cJSON* arr3 = cJSON_CreateArray();
        int arr3_size = fdp.ConsumeIntegralInRange<int>(0, 7);
        for (int i = 0; i < arr3_size && fdp.remaining_bytes() > 0; i++) {
            cJSON_AddItemToArray(arr3, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
        }
        
        // Array 4: Nested array with fuzzer input
        cJSON* arr4 = cJSON_CreateArray();
        cJSON* nested = cJSON_CreateArray();
        int nested_size = fdp.ConsumeIntegralInRange<int>(0, 3);
        for (int i = 0; i < nested_size && fdp.remaining_bytes() > 0; i++) {
            cJSON_AddItemToArray(nested, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
        }
        cJSON_AddItemToArray(arr4, nested);
        // Compare all pairs
        cJSON* arrays[] = {arr1, arr2, arr3, arr4};
        bool case_sensitive = fdp.ConsumeBool();
        
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (arrays[i] != NULL && arrays[j] != NULL) {
                    cJSON_bool result = cJSON_Compare(arrays[i], arrays[j], case_sensitive);
                }
            }
        }
        
        // Clean up
        for (int i = 0; i < 4; i++) {
            if (arrays[i] != NULL) cJSON_Delete(arrays[i]);
        }
        
        return 0;
    }
    
    // Test Scenario 5: Object comparisons
    if (test_scenario == 5) {
        // Create objects with different key/value patterns
        
        // Object 1: Simple object
        cJSON* obj1 = cJSON_CreateObject();
        int obj1_items = fdp.ConsumeIntegralInRange<int>(1, 4);
        for (int i = 0; i < obj1_items && fdp.remaining_bytes() > 0; i++) {
            std::string key = "key" + std::to_string(i);
            cJSON_AddNumberToObject(obj1, key.c_str(), fdp.ConsumeFloatingPoint<double>());
        }
        
        // Object 2: Same keys, different values
        cJSON* obj2 = cJSON_CreateObject();
        for (int i = 0; i < obj1_items && fdp.remaining_bytes() > 0; i++) {
            std::string key = "key" + std::to_string(i);
            cJSON_AddNumberToObject(obj2, key.c_str(), fdp.ConsumeFloatingPoint<double>());
        }
        
        // Object 3: Different keys
        cJSON* obj3 = cJSON_CreateObject();
        int obj3_items = fdp.ConsumeIntegralInRange<int>(1, 4);
        for (int i = 0; i < obj3_items && fdp.remaining_bytes() > 0; i++) {
            std::string key = "different_key" + std::to_string(i);
            cJSON_AddNumberToObject(obj3, key.c_str(), fdp.ConsumeFloatingPoint<double>());
        }
        
        // Object 4: Case variations in keys (use fuzzer input for values)
        cJSON* obj4 = cJSON_CreateObject();
        std::string base_key = fdp.ConsumeRandomLengthString(8);
        if (base_key.empty()) base_key = "key";
        
        // Create variations of the same key with different cases
        std::string key_upper = base_key;
        std::string key_lower = base_key;
        std::string key_mixed = base_key;
        
        // Convert to different cases (simple transformations)
        for (char& c : key_upper) c = toupper(c);
        for (char& c : key_lower) c = tolower(c);
        // Mixed case: first uppercase, rest lowercase
        if (!key_mixed.empty()) {
            key_mixed[0] = toupper(key_mixed[0]);
            for (size_t i = 1; i < key_mixed.size(); i++) {
                key_mixed[i] = tolower(key_mixed[i]);
            }
        }
        
        cJSON_AddNumberToObject(obj4, key_upper.c_str(), fdp.ConsumeFloatingPoint<double>());
        cJSON_AddNumberToObject(obj4, key_lower.c_str(), fdp.ConsumeFloatingPoint<double>());
        cJSON_AddNumberToObject(obj4, key_mixed.c_str(), fdp.ConsumeFloatingPoint<double>());
        
        // Object 5: Nested object with fuzzer input
        cJSON* obj5 = cJSON_CreateObject();
        cJSON* nested_obj = cJSON_CreateObject();
        std::string nested_key = fdp.ConsumeRandomLengthString(8);
        cJSON_AddNumberToObject(nested_obj, nested_key.c_str(), fdp.ConsumeFloatingPoint<double>());
        std::string parent_key = fdp.ConsumeRandomLengthString(8);
        cJSON_AddItemToObject(obj5, parent_key.c_str(), nested_obj);
        
        // Compare with both case-sensitive and case-insensitive
        cJSON* objects[] = {obj1, obj2, obj3, obj4, obj5};
        
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (objects[i] != NULL && objects[j] != NULL) {
                    // Test case-sensitive
                    cJSON_bool result_sensitive = cJSON_Compare(objects[i], objects[j], true);
                    // Test case-insensitive
                    cJSON_bool result_insensitive = cJSON_Compare(objects[i], objects[j], false);
                }
            }
        }
        
        // Clean up
        for (int i = 0; i < 5; i++) {
            if (objects[i] != NULL) cJSON_Delete(objects[i]);
        }
        
        return 0;
    }
    
    // Test Scenario 6: Raw type comparisons
    if (test_scenario == 6) {
        // Create raw JSON strings
        std::vector<std::string> raw_strings;
        
        // Add various raw JSON strings
        raw_strings.push_back("{}");
        raw_strings.push_back("[]");
        raw_strings.push_back("42");
        raw_strings.push_back("\"string\"");
        raw_strings.push_back("true");
        raw_strings.push_back("false");
        raw_strings.push_back("null");
        
        // Add fuzzer-generated raw strings
        int fuzz_raw = fdp.ConsumeIntegralInRange<int>(0, 3);
        for (int i = 0; i < fuzz_raw && fdp.remaining_bytes() > 0; i++) {
            raw_strings.push_back(fdp.ConsumeRandomLengthString(32));
        }
        
        // Compare all pairs
        for (size_t i = 0; i < raw_strings.size(); i++) {
            for (size_t j = 0; j < raw_strings.size(); j++) {
                cJSON* raw1 = cJSON_CreateRaw(raw_strings[i].c_str());
                cJSON* raw2 = cJSON_CreateRaw(raw_strings[j].c_str());
                
                if (raw1 != NULL && raw2 != NULL) {
                    bool case_sensitive = fdp.ConsumeBool(); // Not used for raw but still parameter
                    cJSON_bool result = cJSON_Compare(raw1, raw2, case_sensitive);
                }
                
                if (raw1 != NULL) cJSON_Delete(raw1);
                if (raw2 != NULL) cJSON_Delete(raw2);
            }
        }
        // Test NULL raw strings
        cJSON* raw_null = cJSON_CreateRaw(NULL);
        if (raw_null != NULL) {
            std::string raw_test_str = fdp.ConsumeRandomLengthString(32);
            cJSON* raw_normal = cJSON_CreateRaw(raw_test_str.c_str());
            if (raw_normal != NULL) {
                cJSON_bool result1 = cJSON_Compare(raw_null, raw_normal, true);
                cJSON_bool result2 = cJSON_Compare(raw_normal, raw_null, true);
                cJSON_Delete(raw_normal);
            }
            cJSON_Delete(raw_null);
        }
        return 0;
    }
    
    // Test Scenario 7: Invalid type edge cases
    if (test_scenario == 7) {
        // Try to create items with potentially invalid types
        // cJSON type is stored as bits 0-7 of the type field
        
        // Create a normal item then modify its type field
        double test_value = fdp.ConsumeFloatingPoint<double>();
        cJSON* item = cJSON_CreateNumber(test_value);
        if (item != NULL) {
            // Save original type
            int original_type = item->type;
            
            // Test various invalid type values
            int invalid_types[] = {0, 8, 9, 15, 16, 255};
            
            for (size_t i = 0; i < sizeof(invalid_types)/sizeof(invalid_types[0]); i++) {
                item->type = invalid_types[i];
                
                // Compare with itself
                cJSON_bool result = cJSON_Compare(item, item, true);
                // Should return false for invalid types (line 3097)
                
                // Compare with a valid item
                cJSON* valid_item = cJSON_CreateNumber(test_value);
                if (valid_item != NULL) {
                    cJSON_bool result2 = cJSON_Compare(item, valid_item, true);
                    cJSON_Delete(valid_item);
                }
            }
            
            // Restore original type
            item->type = original_type;
            cJSON_Delete(item);
        }
        
        return 0;
    }
    
    // Test Scenario 8: Complex nested structures
    if (test_scenario == 8) {
        // Create deeply nested structures
        cJSON* complex1 = cJSON_CreateObject();
        cJSON* complex2 = cJSON_CreateObject();
        
        if (complex1 != NULL && complex2 != NULL) {
            // Add nested arrays
            cJSON* array1 = cJSON_CreateArray();
            cJSON* array2 = cJSON_CreateArray();
            
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                cJSON_AddItemToArray(array1, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                cJSON_AddItemToArray(array2, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                
                // Add nested objects inside arrays
                if (i == 1) {
                    cJSON* nested_obj_in_array = cJSON_CreateObject();
                    cJSON_AddNumberToObject(nested_obj_in_array, "nested_value", fdp.ConsumeFloatingPoint<double>());
                    cJSON_AddItemToArray(array1, nested_obj_in_array);
                    
                    cJSON* nested_obj_in_array2 = cJSON_CreateObject();
                    cJSON_AddNumberToObject(nested_obj_in_array2, "nested_value", fdp.ConsumeFloatingPoint<double>());
                    cJSON_AddItemToArray(array2, nested_obj_in_array2);
                }
            }
            
            cJSON_AddItemToObject(complex1, "nested_array", array1);
            cJSON_AddItemToObject(complex2, "nested_array", array2);
            
            // Add objects with arrays
            cJSON* obj_with_array1 = cJSON_CreateObject();
            cJSON* obj_with_array2 = cJSON_CreateObject();
            
            cJSON* inner_array1 = cJSON_CreateArray();
            cJSON* inner_array2 = cJSON_CreateArray();
            
            for (int i = 0; i < 2 && fdp.remaining_bytes() > 0; i++) {
                cJSON_AddItemToArray(inner_array1, cJSON_CreateString(fdp.ConsumeRandomLengthString(8).c_str()));
                cJSON_AddItemToArray(inner_array2, cJSON_CreateString(fdp.ConsumeRandomLengthString(8).c_str()));
            }
            
            cJSON_AddItemToObject(obj_with_array1, "inner", inner_array1);
            cJSON_AddItemToObject(obj_with_array2, "inner", inner_array2);
            
            cJSON_AddItemToObject(complex1, "obj_with_array", obj_with_array1);
            cJSON_AddItemToObject(complex2, "obj_with_array", obj_with_array2);
            
            // Compare with both case modes
            cJSON_bool result_sensitive = cJSON_Compare(complex1, complex2, true);
            cJSON_bool result_insensitive = cJSON_Compare(complex1, complex2, false);
            
            // Compare each with itself (should be true)
            cJSON_bool self1 = cJSON_Compare(complex1, complex1, true);
            cJSON_bool self2 = cJSON_Compare(complex2, complex2, true);
        }
        
        if (complex1 != NULL) cJSON_Delete(complex1);
        if (complex2 != NULL) cJSON_Delete(complex2);
        return 0;
    }
    
    // Test Scenario 9: Mixed comprehensive test
    if (test_scenario == 9) {
        // Create a variety of JSON items and test all combinations
        std::vector<cJSON*> items;
        
        // Add items of all types using fuzzer input
        items.push_back(cJSON_CreateNull());
        items.push_back(cJSON_CreateTrue());
        items.push_back(cJSON_CreateFalse());
        
        // Create numbers from fuzzer input
        items.push_back(cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
        items.push_back(cJSON_CreateNumber(std::numeric_limits<double>::infinity()));
        items.push_back(cJSON_CreateNumber(std::numeric_limits<double>::quiet_NaN()));
        
        // Create strings from fuzzer input
        items.push_back(cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str()));
        items.push_back(cJSON_CreateString(""));  // Empty string is a valid edge case
        items.push_back(cJSON_CreateString(NULL)); // NULL string is an edge case
        
        // Create raw from fuzzer input
        std::string raw_data = fdp.ConsumeRandomLengthString(64);
        items.push_back(cJSON_CreateRaw(raw_data.c_str()));
        
        // Add array with fuzzer-generated elements
        cJSON* simple_array = cJSON_CreateArray();
        int array_size = fdp.ConsumeIntegralInRange<int>(0, 5);
        for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
            cJSON_AddItemToArray(simple_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
        }
        items.push_back(simple_array);
        
        // Add object with fuzzer-generated key-value pairs
        cJSON* simple_object = cJSON_CreateObject();
        int obj_size = fdp.ConsumeIntegralInRange<int>(0, 5);
        for (int i = 0; i < obj_size && fdp.remaining_bytes() > 0; i++) {
            std::string key = fdp.ConsumeRandomLengthString(8);
            std::string value = fdp.ConsumeRandomLengthString(16);
            cJSON_AddStringToObject(simple_object, key.c_str(), value.c_str());
        }
        items.push_back(simple_object);
        
        // Compare all pairs with both case sensitivity modes
        for (size_t i = 0; i < items.size(); i++) {
            for (size_t j = 0; j < items.size(); j++) {
                if (items[i] != NULL && items[j] != NULL) {
                    cJSON_bool result_sensitive = cJSON_Compare(items[i], items[j], true);
                    // Only test case-insensitive for objects (only relevant for object key comparison)
                    if (cJSON_IsObject(items[i]) && cJSON_IsObject(items[j])) {
                        cJSON_bool result_insensitive = cJSON_Compare(items[i], items[j], false);
                    }
                }
            }
        }
        
        // Clean up
        for (cJSON* item : items) {
            if (item != NULL) cJSON_Delete(item);
        }
        
        return 0;
    }
    
    return 0;
}

} // namespace harness_010
