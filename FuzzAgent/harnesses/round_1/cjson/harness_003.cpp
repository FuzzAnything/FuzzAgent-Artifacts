#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include "cjson/cJSON.h"

namespace harness_003 {

extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size: need enough data for basic operations
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation mode to test different comparison scenarios
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume case sensitivity flag for comparison
    cJSON_bool case_sensitive = fdp.ConsumeBool() ? 1 : 0;
    
    // Create first object/array based on operation mode
    cJSON* obj1 = nullptr;
    cJSON* obj2 = nullptr;
    
    switch (operation_mode) {
        case 0: {
            // Mode 0: Compare two objects with similar but different structures
            obj1 = cJSON_CreateObject();
            obj2 = cJSON_CreateObject();
            
            if (obj1 == nullptr || obj2 == nullptr) {
                if (obj1) cJSON_Delete(obj1);
                if (obj2) cJSON_Delete(obj2);
                return 0;
            }
            
            // Add similar items to both objects with slight differences
            int num_items = fdp.ConsumeIntegralInRange<int>(1, 10);
            
            for (int i = 0; i < num_items && fdp.remaining_bytes() > 20; i++) {
                std::string key = fdp.ConsumeRandomLengthString(20);
                
                // Add to first object
                if (i % 2 == 0) {
                    double num_val = fdp.ConsumeFloatingPoint<double>();
                    cJSON_AddNumberToObject(obj1, key.c_str(), num_val);
                    
                    // Add slightly different value to second object
                    double num_val2 = num_val + (fdp.ConsumeFloatingPoint<double>() * 0.1);
                    cJSON_AddNumberToObject(obj2, key.c_str(), num_val2);
                } else {
                    std::string str_val = fdp.ConsumeRandomLengthString(50);
                    cJSON_AddStringToObject(obj1, key.c_str(), str_val.c_str());
                    
                    // Add different string to second object
                    std::string str_val2 = fdp.ConsumeRandomLengthString(50);
                    cJSON_AddStringToObject(obj2, key.c_str(), str_val2.c_str());
                }
            }
            
            // Test cJSON_SetValuestring on string items
            if (fdp.remaining_bytes() > 10) {
                // Create a test key from fuzzer input
                std::string test_key = fdp.ConsumeRandomLengthString(20);
                cJSON* string_item = cJSON_GetObjectItem(obj1, test_key.c_str());
                if (string_item == nullptr) {
                    // Create a string item if it doesn't exist with fuzzer-provided initial value
                    std::string initial_value = fdp.ConsumeRandomLengthString(50);
                    string_item = cJSON_CreateString(initial_value.c_str());
                    cJSON_AddItemToObject(obj1, test_key.c_str(), string_item);
                }
                
                if (cJSON_IsString(string_item)) {
                    std::string new_value = fdp.ConsumeRandomLengthString(100);
                    char* result = cJSON_SetValuestring(string_item, new_value.c_str());
                    (void)result; // Ignore return value
                }
            }
            break;
        }
        
        case 1: {
            // Mode 1: Compare arrays with float/double/string arrays
            // Create float array
            int float_array_size = fdp.ConsumeIntegralInRange<int>(1, 20);
            std::vector<float> float_numbers(float_array_size);
            for (int i = 0; i < float_array_size; i++) {
                float_numbers[i] = fdp.ConsumeFloatingPoint<float>();
            }
            obj1 = cJSON_CreateFloatArray(float_numbers.data(), float_array_size);
            
            // Create double array (similar but different)
            int double_array_size = fdp.ConsumeIntegralInRange<int>(1, 20);
            std::vector<double> double_numbers(double_array_size);
            for (int i = 0; i < double_array_size; i++) {
                double_numbers[i] = fdp.ConsumeFloatingPoint<double>();
            }
            obj2 = cJSON_CreateDoubleArray(double_numbers.data(), double_array_size);
            
            if (obj1 == nullptr || obj2 == nullptr) {
                if (obj1) cJSON_Delete(obj1);
                if (obj2) cJSON_Delete(obj2);
                return 0;
            }
            break;
        }
        
        case 2: {
            // Mode 2: Compare string arrays
            int str_array_size = fdp.ConsumeIntegralInRange<int>(1, 10);
            if (str_array_size * 50 > fdp.remaining_bytes()) {
                return 0;
            }
            
            std::vector<const char*> string_array1(str_array_size);
            std::vector<const char*> string_array2(str_array_size);
            std::vector<std::string> string_storage1(str_array_size);
            std::vector<std::string> string_storage2(str_array_size);
            
            for (int i = 0; i < str_array_size; i++) {
                string_storage1[i] = fdp.ConsumeRandomLengthString(50);
                string_array1[i] = string_storage1[i].c_str();
                
                string_storage2[i] = fdp.ConsumeRandomLengthString(50);
                string_array2[i] = string_storage2[i].c_str();
            }
            
            obj1 = cJSON_CreateStringArray(string_array1.data(), str_array_size);
            obj2 = cJSON_CreateStringArray(string_array2.data(), str_array_size);
            
            if (obj1 == nullptr || obj2 == nullptr) {
                if (obj1) cJSON_Delete(obj1);
                if (obj2) cJSON_Delete(obj2);
                return 0;
            }
            break;
        }
        
        case 3: {
            // Mode 3: Compare nested structures (objects containing arrays)
            obj1 = cJSON_CreateObject();
            obj2 = cJSON_CreateObject();
            
            if (obj1 == nullptr || obj2 == nullptr) {
                if (obj1) cJSON_Delete(obj1);
                if (obj2) cJSON_Delete(obj2);
                return 0;
            }
            
            // Add arrays to both objects
            int array_size = fdp.ConsumeIntegralInRange<int>(1, 5);
            
            // Create int array for first object
            std::vector<int> int_array(array_size);
            for (int i = 0; i < array_size; i++) {
                int_array[i] = fdp.ConsumeIntegral<int>();
            }
            cJSON* int_array_obj = cJSON_CreateIntArray(int_array.data(), array_size);
            if (int_array_obj) {
                cJSON_AddItemToObject(obj1, "int_array", int_array_obj);
            }
            
            // Create float array for second object
            std::vector<float> float_array(array_size);
            for (int i = 0; i < array_size; i++) {
                float_array[i] = fdp.ConsumeFloatingPoint<float>();
            }
            cJSON* float_array_obj = cJSON_CreateFloatArray(float_array.data(), array_size);
            if (float_array_obj) {
                cJSON_AddItemToObject(obj2, "float_array", float_array_obj);
            }
            
            // Add some common properties
            std::string common_key = "common";
            double common_value = fdp.ConsumeFloatingPoint<double>();
            cJSON_AddNumberToObject(obj1, common_key.c_str(), common_value);
            cJSON_AddNumberToObject(obj2, common_key.c_str(), common_value);
            
            break;
        }
    }
    
    // Perform comparison if both objects were created successfully
    if (obj1 != nullptr && obj2 != nullptr) {
        // Test cJSON_Compare
        cJSON_bool compare_result = cJSON_Compare(obj1, obj2, case_sensitive);
        (void)compare_result;
        
        // Test cJSON_Compare with swapped parameters
        cJSON_bool compare_result_swapped = cJSON_Compare(obj2, obj1, case_sensitive);
        (void)compare_result_swapped;
        
        // Test cJSON_PrintPreallocated
        if (fdp.remaining_bytes() > 50) {
            int buffer_size = fdp.ConsumeIntegralInRange<int>(100, 1000);
            std::vector<char> buffer(buffer_size);
            cJSON_bool format = fdp.ConsumeBool() ? 1 : 0;
            
            cJSON_bool print_result = cJSON_PrintPreallocated(obj1, buffer.data(), buffer_size, format);
            (void)print_result;
            
            // Try with smaller buffer to test edge cases
            if (buffer_size > 10) {
                cJSON_bool print_result_small = cJSON_PrintPreallocated(obj1, buffer.data(), 10, format);
                (void)print_result_small;
            }
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

} // namespace harness_003
