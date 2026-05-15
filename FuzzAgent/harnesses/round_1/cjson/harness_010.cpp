#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include "cjson/cJSON.h"

namespace harness_010 {
/*
 * harness_010.cpp - cJSON_Compare comprehensive structural equality testing harness
 * 
 * This harness specifically targets cJSON_Compare function which has 42/76 branches 
 * uncovered (44.74% coverage). Focus on structural equality testing to exercise 
 * success paths for:
 * 1) boolean/null comparisons
 * 2) number/string equality  
 * 3) array iteration
 * 4) object symmetry checks
 * 
 * Based on analysis of cJSON_Compare source code, we need to test:
 * - Type validation switch (cJSON_False, cJSON_True, cJSON_NULL, cJSON_Number, 
 *   cJSON_String, cJSON_Raw, cJSON_Array, cJSON_Object)
 * - compare_double() function for number comparisons
 * - strcmp() for string comparisons with case sensitivity
 * - Array iteration with equal/different lengths
 * - Object symmetry checks (O(n^2) algorithm)
 * - Edge cases: null pointers, identical objects (a == b), invalid types
 */


extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for meaningful comparison scenarios
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test scenario selection
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Consume case sensitivity flag
    cJSON_bool case_sensitive = fdp.ConsumeBool() ? 1 : 0;
    
    cJSON* obj1 = nullptr;
    cJSON* obj2 = nullptr;
    cJSON_bool compare_result = 0;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Boolean and NULL type comparisons
            // Test cJSON_False, cJSON_True, cJSON_NULL cases
            uint8_t type1 = fdp.ConsumeIntegral<uint8_t>() % 3;
            uint8_t type2 = fdp.ConsumeIntegral<uint8_t>() % 3;
            
            switch (type1) {
                case 0: obj1 = cJSON_CreateFalse(); break;
                case 1: obj1 = cJSON_CreateTrue(); break;
                case 2: obj1 = cJSON_CreateNull(); break;
            }
            
            switch (type2) {
                case 0: obj2 = cJSON_CreateFalse(); break;
                case 1: obj2 = cJSON_CreateTrue(); break;
                case 2: obj2 = cJSON_CreateNull(); break;
            }
            
            if (obj1 && obj2) {
                compare_result = cJSON_Compare(obj1, obj2, case_sensitive);
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Number comparisons with various values
            // Test compare_double() function paths
            double num1 = fdp.ConsumeFloatingPoint<double>();
            double num2 = fdp.ConsumeFloatingPoint<double>();
            
            obj1 = cJSON_CreateNumber(num1);
            obj2 = cJSON_CreateNumber(num2);
            
            if (obj1 && obj2) {
                compare_result = cJSON_Compare(obj1, obj2, case_sensitive);
                
                // Test edge cases: NaN, infinity, very close numbers
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 8) {
                    cJSON* nan_obj = cJSON_CreateNumber(NAN);
                    cJSON* inf_obj = cJSON_CreateNumber(INFINITY);
                    cJSON* neg_inf_obj = cJSON_CreateNumber(-INFINITY);
                    
                    if (nan_obj && obj1) {
                        cJSON_Compare(nan_obj, obj1, case_sensitive);
                    }
                    if (inf_obj && obj1) {
                        cJSON_Compare(inf_obj, obj1, case_sensitive);
                    }
                    if (neg_inf_obj && obj1) {
                        cJSON_Compare(neg_inf_obj, obj1, case_sensitive);
                    }
                    
                    if (nan_obj) cJSON_Delete(nan_obj);
                    if (inf_obj) cJSON_Delete(inf_obj);
                    if (neg_inf_obj) cJSON_Delete(neg_inf_obj);
                }
            }
            break;
        }
        
        case 2: {
            // Scenario 2: String comparisons with case sensitivity
            std::string str1 = fdp.ConsumeRandomLengthString(128);
            std::string str2 = fdp.ConsumeRandomLengthString(128);
            
            // Optionally make strings identical or similar
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
                str2 = str1;  // Test identical strings
                if (fdp.ConsumeBool()) {
                    // Modify case for case-sensitive testing
                    if (!str2.empty()) {
                        str2[0] = islower(str2[0]) ? toupper(str2[0]) : tolower(str2[0]);
                    }
                }
            }
            
            obj1 = cJSON_CreateString(str1.c_str());
            obj2 = cJSON_CreateString(str2.c_str());
            
            if (obj1 && obj2) {
                compare_result = cJSON_Compare(obj1, obj2, case_sensitive);
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Raw JSON comparisons
            std::string raw1 = fdp.ConsumeRandomLengthString(128);
            std::string raw2 = fdp.ConsumeRandomLengthString(128);
            
            obj1 = cJSON_CreateRaw(raw1.c_str());
            obj2 = cJSON_CreateRaw(raw2.c_str());
            
            if (obj1 && obj2) {
                compare_result = cJSON_Compare(obj1, obj2, case_sensitive);
            }
            break;
        }
        
        case 4: {
            // Scenario 4: Array comparisons with different lengths and contents
            int array1_size = fdp.ConsumeIntegralInRange<int>(0, 10);
            int array2_size = fdp.ConsumeIntegralInRange<int>(0, 10);
            
            obj1 = cJSON_CreateArray();
            obj2 = cJSON_CreateArray();
            
            if (obj1 && obj2) {
                // Add items to first array
                for (int i = 0; i < array1_size && fdp.remaining_bytes() > 8; i++) {
                    if (i % 3 == 0) {
                        double num = fdp.ConsumeFloatingPoint<double>();
                        cJSON_AddItemToArray(obj1, cJSON_CreateNumber(num));
                    } else if (i % 3 == 1) {
                        std::string str = fdp.ConsumeRandomLengthString(50);
                        cJSON_AddItemToArray(obj1, cJSON_CreateString(str.c_str()));
                    } else {
                        cJSON_AddItemToArray(obj1, cJSON_CreateBool(fdp.ConsumeBool()));
                    }
                }
                
                // Add items to second array
                for (int i = 0; i < array2_size && fdp.remaining_bytes() > 8; i++) {
                    if (i % 3 == 0) {
                        double num = fdp.ConsumeFloatingPoint<double>();
                        cJSON_AddItemToArray(obj2, cJSON_CreateNumber(num));
                    } else if (i % 3 == 1) {
                        std::string str = fdp.ConsumeRandomLengthString(50);
                        cJSON_AddItemToArray(obj2, cJSON_CreateString(str.c_str()));
                    } else {
                        cJSON_AddItemToArray(obj2, cJSON_CreateBool(fdp.ConsumeBool()));
                    }
                }
                
                compare_result = cJSON_Compare(obj1, obj2, case_sensitive);
            }
            break;
        }
        
        case 5: {
            // Scenario 5: Object symmetry checks (testing O(n^2) algorithm)
            obj1 = cJSON_CreateObject();
            obj2 = cJSON_CreateObject();
            
            if (obj1 && obj2) {
                int num_items = fdp.ConsumeIntegralInRange<int>(0, 8);
                
                // Create objects with potentially overlapping keys
                for (int i = 0; i < num_items && fdp.remaining_bytes() > 20; i++) {
                    std::string key = fdp.ConsumeRandomLengthString(20);
                    
                    // Add to both objects with same or different values
                    if (fdp.ConsumeBool()) {
                        // Same value in both objects
                        double value = fdp.ConsumeFloatingPoint<double>();
                        cJSON_AddNumberToObject(obj1, key.c_str(), value);
                        cJSON_AddNumberToObject(obj2, key.c_str(), value);
                    } else {
                        // Different values
                        double value1 = fdp.ConsumeFloatingPoint<double>();
                        double value2 = fdp.ConsumeFloatingPoint<double>();
                        cJSON_AddNumberToObject(obj1, key.c_str(), value1);
                        cJSON_AddNumberToObject(obj2, key.c_str(), value2);
                    }
                }
                
                // Add some unique keys to each object
                int unique1 = fdp.ConsumeIntegralInRange<int>(0, 3);
                for (int i = 0; i < unique1 && fdp.remaining_bytes() > 20; i++) {
                    std::string key = "unique1_" + std::to_string(i);
                    double value = fdp.ConsumeFloatingPoint<double>();
                    cJSON_AddNumberToObject(obj1, key.c_str(), value);
                }
                
                int unique2 = fdp.ConsumeIntegralInRange<int>(0, 3);
                for (int i = 0; i < unique2 && fdp.remaining_bytes() > 20; i++) {
                    std::string key = "unique2_" + std::to_string(i);
                    double value = fdp.ConsumeFloatingPoint<double>();
                    cJSON_AddNumberToObject(obj2, key.c_str(), value);
                }
                
                compare_result = cJSON_Compare(obj1, obj2, case_sensitive);
            }
            break;
        }
        
        case 6: {
            // Scenario 6: Nested structures (objects within arrays, arrays within objects)
            obj1 = cJSON_CreateObject();
            obj2 = cJSON_CreateObject();
            
            if (obj1 && obj2) {
                // Add arrays to objects
                cJSON* array1 = cJSON_CreateArray();
                cJSON* array2 = cJSON_CreateArray();
                
                if (array1 && array2) {
                    // Fill arrays with mixed types
                    int array_size = fdp.ConsumeIntegralInRange<int>(1, 5);
                    for (int i = 0; i < array_size && fdp.remaining_bytes() > 16; i++) {
                        if (i % 2 == 0) {
                            double num = fdp.ConsumeFloatingPoint<double>();
                            cJSON_AddItemToArray(array1, cJSON_CreateNumber(num));
                            cJSON_AddItemToArray(array2, cJSON_CreateNumber(num));
                        } else {
                            std::string str = fdp.ConsumeRandomLengthString(30);
                            cJSON_AddItemToArray(array1, cJSON_CreateString(str.c_str()));
                            cJSON_AddItemToArray(array2, cJSON_CreateString(str.c_str()));
                        }
                    }
                    
                    cJSON_AddItemToObject(obj1, "nested_array", array1);
                    cJSON_AddItemToObject(obj2, "nested_array", array2);
                }
                
                // Add objects to arrays (if arrays were created)
                if (array1 && array2) {
                    cJSON* nested_obj1 = cJSON_CreateObject();
                    cJSON* nested_obj2 = cJSON_CreateObject();
                    
                    if (nested_obj1 && nested_obj2) {
                        std::string key = fdp.ConsumeRandomLengthString(20);
                        double value = fdp.ConsumeFloatingPoint<double>();
                        
                        cJSON_AddNumberToObject(nested_obj1, key.c_str(), value);
                        cJSON_AddNumberToObject(nested_obj2, key.c_str(), value);
                        
                        cJSON_AddItemToArray(array1, nested_obj1);
                        cJSON_AddItemToArray(array2, nested_obj2);
                    }
                }
                
                compare_result = cJSON_Compare(obj1, obj2, case_sensitive);
            }
            break;
        }
        
        case 7: {
            // Scenario 7: Edge cases and error conditions
            // Test null pointers, invalid types, identical objects
            
            if (fdp.ConsumeBool()) {
                // Test with null pointers
                compare_result = cJSON_Compare(nullptr, nullptr, case_sensitive);
                
                // Create a valid object and compare with null
                obj1 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                if (obj1) {
                    compare_result = cJSON_Compare(obj1, nullptr, case_sensitive);
                    compare_result = cJSON_Compare(nullptr, obj1, case_sensitive);
                }
            } else {
                // Test identical objects (a == b)
                obj1 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                if (obj1) {
                    compare_result = cJSON_Compare(obj1, obj1, case_sensitive);
                }
                
                // Test different types
                obj1 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                std::string test_string = fdp.ConsumeRandomLengthString(50);
                obj2 = cJSON_CreateString(test_string.c_str());
                if (obj1 && obj2) {
                    compare_result = cJSON_Compare(obj1, obj2, case_sensitive);
                }
            }
            break;
        }
    }
    
    // Cleanup
    if (obj1 != nullptr) {
        cJSON_Delete(obj1);
    }
    if (obj2 != nullptr) {
        cJSON_Delete(obj2);
    }
    
    return 0;
}

} // namespace harness_010
