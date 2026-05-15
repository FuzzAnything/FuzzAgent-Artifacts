#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "cjson/cJSON.h"

namespace harness_005 {

extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing: need at least some bytes for keys and values
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation mode to test different scenarios
    // 0: Test all convenience functions sequentially
    // 1: Test random selection of convenience functions
    // 2: Test nested object/array structures
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Create root object
    cJSON* root_object = cJSON_CreateObject();
    if (root_object == nullptr) {
        return 0;
    }
    
    // Store key strings to keep them alive for the duration of the test
    std::vector<std::string> key_strings;
    
    switch (operation_mode) {
        case 0: {
            // Mode 0: Test all convenience functions sequentially with different keys
            int num_functions = 7; // 7 target functions
            
            for (int i = 0; i < num_functions && fdp.remaining_bytes() > 10; i++) {
                // Consume key name
                key_strings.push_back(fdp.ConsumeRandomLengthString(32));
                const char* key_name = key_strings.back().c_str();
                
                switch (i) {
                    case 0: {
                        // Test cJSON_AddObjectToObject - adds a new nested object
                        cJSON_AddObjectToObject(root_object, key_name);
                        break;
                    }
                    case 1: {
                        // Test cJSON_AddArrayToObject - adds a new array
                        cJSON_AddArrayToObject(root_object, key_name);
                        break;
                    }
                    case 2: {
                        // Test cJSON_AddTrueToObject - adds a true boolean
                        cJSON_AddTrueToObject(root_object, key_name);
                        break;
                    }
                    case 3: {
                        // Test cJSON_AddFalseToObject - adds a false boolean
                        cJSON_AddFalseToObject(root_object, key_name);
                        break;
                    }
                    case 4: {
                        // Test cJSON_AddItemReferenceToObject - adds a reference to existing item
                        // First create an item to reference
                        cJSON* ref_item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                        if (ref_item != nullptr) {
                            cJSON_AddItemReferenceToObject(root_object, key_name, ref_item);
                            // Note: ref_item is not freed here - it's managed by the object
                        }
                        break;
                    }
                    case 5: {
                        // Test cJSON_AddNullToObject - adds a null value
                        cJSON_AddNullToObject(root_object, key_name);
                        break;
                    }
                    case 6: {
                        // Test cJSON_AddBoolToObject - adds a boolean with specified value
                        cJSON_bool bool_value = fdp.ConsumeBool() ? 1 : 0;
                        cJSON_AddBoolToObject(root_object, key_name, bool_value);
                        break;
                    }
                }
            }
            
            // Optionally test cJSON_AddStringToObject and cJSON_AddNumberToObject
            // (already covered in other harnesses but included for completeness)
            if (fdp.remaining_bytes() > 10) {
                key_strings.push_back(fdp.ConsumeRandomLengthString(32));
                std::string string_value = fdp.ConsumeRandomLengthString(64);
                cJSON_AddStringToObject(root_object, key_strings.back().c_str(), string_value.c_str());
            }
            
            if (fdp.remaining_bytes() > 10) {
                key_strings.push_back(fdp.ConsumeRandomLengthString(32));
                double number_value = fdp.ConsumeFloatingPoint<double>();
                cJSON_AddNumberToObject(root_object, key_strings.back().c_str(), number_value);
            }
            break;
        }
        
        case 1: {
            // Mode 1: Test random selection of convenience functions
            int num_operations = fdp.ConsumeIntegralInRange<int>(1, 10);
            
            for (int i = 0; i < num_operations && fdp.remaining_bytes() > 10; i++) {
                // Consume key name
                key_strings.push_back(fdp.ConsumeRandomLengthString(32));
                const char* key_name = key_strings.back().c_str();
                
                // Select random function to test
                uint8_t func_select = fdp.ConsumeIntegral<uint8_t>() % 9; // 7 target + 2 optional
                
                switch (func_select) {
                    case 0: {
                        cJSON_AddObjectToObject(root_object, key_name);
                        break;
                    }
                    case 1: {
                        cJSON_AddArrayToObject(root_object, key_name);
                        break;
                    }
                    case 2: {
                        cJSON_AddTrueToObject(root_object, key_name);
                        break;
                    }
                    case 3: {
                        cJSON_AddFalseToObject(root_object, key_name);
                        break;
                    }
                    case 4: {
                        // For reference, create a simple item to reference
                        cJSON* ref_item = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
                        if (ref_item != nullptr) {
                            cJSON_AddItemReferenceToObject(root_object, key_name, ref_item);
                        }
                        break;
                    }
                    case 5: {
                        cJSON_AddNullToObject(root_object, key_name);
                        break;
                    }
                    case 6: {
                        cJSON_AddBoolToObject(root_object, key_name, fdp.ConsumeBool() ? 1 : 0);
                        break;
                    }
                    case 7: {
                        // Optional: AddStringToObject
                        cJSON_AddStringToObject(root_object, key_name, fdp.ConsumeRandomLengthString(64).c_str());
                        break;
                    }
                    case 8: {
                        // Optional: AddNumberToObject
                        cJSON_AddNumberToObject(root_object, key_name, fdp.ConsumeFloatingPoint<double>());
                        break;
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Mode 2: Test nested object/array structures using convenience functions
            // Create a nested object structure
            key_strings.push_back(fdp.ConsumeRandomLengthString(32));
            cJSON* nested_object = cJSON_AddObjectToObject(root_object, key_strings.back().c_str());
            
            if (nested_object != nullptr) {
                // Add various items to the nested object
                key_strings.push_back(fdp.ConsumeRandomLengthString(32));
                cJSON_AddTrueToObject(nested_object, key_strings.back().c_str());
                
                key_strings.push_back(fdp.ConsumeRandomLengthString(32));
                cJSON_AddFalseToObject(nested_object, key_strings.back().c_str());
                
                key_strings.push_back(fdp.ConsumeRandomLengthString(32));
                cJSON_AddNullToObject(nested_object, key_strings.back().c_str());
                
                key_strings.push_back(fdp.ConsumeRandomLengthString(32));
                cJSON_AddBoolToObject(nested_object, key_strings.back().c_str(), fdp.ConsumeBool() ? 1 : 0);
                
                // Create a nested array within the nested object
                key_strings.push_back(fdp.ConsumeRandomLengthString(32));
                cJSON* nested_array = cJSON_AddArrayToObject(nested_object, key_strings.back().c_str());
                
                if (nested_array != nullptr) {
                    // Add some items to the nested array using regular array functions
                    cJSON* array_item1 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                    cJSON* array_item2 = cJSON_CreateString(fdp.ConsumeRandomLengthString(32).c_str());
                    
                    if (array_item1) cJSON_AddItemToArray(nested_array, array_item1);
                    if (array_item2) cJSON_AddItemToArray(nested_array, array_item2);
                }
                
                // Test reference addition in nested structure
                key_strings.push_back(fdp.ConsumeRandomLengthString(32));
                cJSON* ref_target = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                if (ref_target != nullptr) {
                    // Add the item normally first
                    cJSON_AddItemToObject(nested_object, "reference_target", ref_target);
                    
                    // Now add a reference to it
                    key_strings.push_back(fdp.ConsumeRandomLengthString(32));
                    cJSON_AddItemReferenceToObject(nested_object, key_strings.back().c_str(), ref_target);
                }
            }
            break;
        }
    }
    
    // Clean up - this will free all memory including referenced items
    cJSON_Delete(root_object);
    
    return 0;
}

} // namespace harness_005
