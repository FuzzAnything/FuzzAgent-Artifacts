#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "cjson/cJSON.h"

namespace harness_000 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    uint8_t num_items = fdp.ConsumeIntegralInRange<uint8_t>(1, 10);
    bool formatted = fdp.ConsumeBool();
    bool require_null_terminated = fdp.ConsumeBool();
    double number_value = fdp.ConsumeFloatingPoint<double>();
    int array_index = fdp.ConsumeIntegralInRange<int>(0, num_items - 1);

    // Consume variable-size data
    std::string json_input = fdp.ConsumeRandomLengthString(4096);
    std::string key_name = fdp.ConsumeRandomLengthString(256);
    std::string string_value = fdp.ConsumeRandomLengthString(512);
    std::string raw_json = fdp.ConsumeRandomLengthString(2048);
    std::string nested_key_name = fdp.ConsumeRandomLengthString(128);
    std::string nested_value = fdp.ConsumeRandomLengthString(256);

    // Use operation to determine API flow
    switch (operation) {
        case 0: {
            // Parse JSON and print it
            cJSON* json = cJSON_Parse(json_input.c_str());
            if (json != NULL) {
                char* printed = formatted ? cJSON_Print(json) : cJSON_PrintUnformatted(json);
                if (printed != NULL) {
                    free(printed);
                }
                cJSON_Delete(json);
            }
            break;
        }
        
        case 1: {
            // Parse with options
            cJSON* json = cJSON_ParseWithOpts(json_input.c_str(), NULL, require_null_terminated);
            if (json != NULL) {
                char* printed = cJSON_PrintBuffered(json, 1024, formatted);
                if (printed != NULL) {
                    free(printed);
                }
                cJSON_Delete(json);
            }
            break;
        }
        
        case 2: {
            // Create and manipulate a JSON object
            cJSON* root = cJSON_CreateObject();
            if (root != NULL) {
                // Consume more strings for keys
                std::string null_key = fdp.ConsumeRandomLengthString(64);
                std::string true_key = fdp.ConsumeRandomLengthString(64);
                std::string false_key = fdp.ConsumeRandomLengthString(64);
                std::string bool_key = fdp.ConsumeRandomLengthString(64);
                std::string number_key = fdp.ConsumeRandomLengthString(64);
                std::string string_key = fdp.ConsumeRandomLengthString(64);
                std::string raw_key = fdp.ConsumeRandomLengthString(64);
                std::string nested_object_key = fdp.ConsumeRandomLengthString(64);
                std::string array_key = fdp.ConsumeRandomLengthString(64);
                
                // Add various types to object
                cJSON_AddNullToObject(root, null_key.c_str());
                cJSON_AddTrueToObject(root, true_key.c_str());
                cJSON_AddFalseToObject(root, false_key.c_str());
                cJSON_AddBoolToObject(root, bool_key.c_str(), fdp.ConsumeBool());
                cJSON_AddNumberToObject(root, number_key.c_str(), number_value);
                cJSON_AddStringToObject(root, string_key.c_str(), string_value.c_str());
                cJSON_AddRawToObject(root, raw_key.c_str(), raw_json.c_str());
                
                // Create nested objects
                cJSON* nested_obj = cJSON_AddObjectToObject(root, nested_object_key.c_str());
                if (nested_obj != NULL) {
                    cJSON_AddStringToObject(nested_obj, nested_key_name.c_str(), nested_value.c_str());
                }
                
                // Create array
                cJSON* array = cJSON_AddArrayToObject(root, array_key.c_str());
                if (array != NULL) {
                    for (int i = 0; i < num_items; i++) {
                        // Use fuzzer input for array values
                        double array_val = fdp.ConsumeFloatingPoint<double>();
                        cJSON* item = cJSON_CreateNumber(array_val);
                        cJSON_AddItemToArray(array, item);
                    }
                }
                
                // Print and cleanup
                char* printed = cJSON_Print(root);
                if (printed != NULL) {
                    free(printed);
                }
                cJSON_Delete(root);
            }
            break;
        }
        
        case 3: {
            // Create and manipulate JSON arrays
            cJSON* array = cJSON_CreateArray();
            if (array != NULL) {
                // Add various types to array
                for (int i = 0; i < num_items; i++) {
                    switch (i % 5) {
                        case 0:
                            cJSON_AddItemToArray(array, cJSON_CreateNull());
                            break;
                        case 1:
                            cJSON_AddItemToArray(array, cJSON_CreateBool(fdp.ConsumeBool()));
                            break;
                        case 2:
                            cJSON_AddItemToArray(array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                            break;
                        case 3:
                            cJSON_AddItemToArray(array, cJSON_CreateString(string_value.c_str()));
                            break;
                        case 4:
                            cJSON_AddItemToArray(array, cJSON_CreateObject());
                            break;
                    }
                }
                
                // Test array operations
                int array_size = cJSON_GetArraySize(array);
                if (array_size > 0) {
                    cJSON* item = cJSON_GetArrayItem(array, array_index % array_size);
                    if (item != NULL) {
                        // Check type
                        if (cJSON_IsNull(item)) {
                            // Do nothing
                        } else if (cJSON_IsBool(item)) {
                            bool is_true = cJSON_IsTrue(item);
                        } else if (cJSON_IsNumber(item)) {
                            double num = cJSON_GetNumberValue(item);
                        } else if (cJSON_IsString(item)) {
                            char* str_val = cJSON_GetStringValue(item);
                        }
                    }
                    
                    // Detach and delete an item
                    cJSON* detached = cJSON_DetachItemFromArray(array, 0);
                    if (detached != NULL) {
                        cJSON_Delete(detached);
                    }
                }
                
                // Print and cleanup
                char* printed = cJSON_Print(array);
                if (printed != NULL) {
                    free(printed);
                }
                cJSON_Delete(array);
            }
            break;
        }
        
        case 4: {
            // Test object item retrieval and manipulation
            cJSON* root = cJSON_CreateObject();
            if (root != NULL) {
                // Consume key for test_number
                std::string test_number_key = fdp.ConsumeRandomLengthString(64);
                
                // Add some items
                cJSON_AddStringToObject(root, key_name.c_str(), string_value.c_str());
                cJSON_AddNumberToObject(root, test_number_key.c_str(), number_value);
                
                // Retrieve items
                cJSON* string_item = cJSON_GetObjectItem(root, key_name.c_str());
                if (string_item != NULL && cJSON_IsString(string_item)) {
                    char* str_val = cJSON_GetStringValue(string_item);
                }
                
                cJSON* number_item = cJSON_GetObjectItem(root, test_number_key.c_str());
                if (number_item != NULL && cJSON_IsNumber(number_item)) {
                    double num_val = cJSON_GetNumberValue(number_item);
                }
                
                // Check if object has item
                bool has_item = cJSON_HasObjectItem(root, key_name.c_str());
                
                // Replace an item
                bool new_bool_value = fdp.ConsumeBool();
                cJSON* new_item = cJSON_CreateBool(new_bool_value);
                cJSON_ReplaceItemInObject(root, key_name.c_str(), new_item);
                
                // Delete an item
                cJSON_DeleteItemFromObject(root, test_number_key.c_str());
                
                cJSON_Delete(root);
            }
            break;
        }
        
        case 5: {
            // Test duplication and comparison
            cJSON* original = cJSON_CreateObject();
            if (original != NULL) {
                // Consume keys and values
                std::string key1 = fdp.ConsumeRandomLengthString(64);
                std::string value1 = fdp.ConsumeRandomLengthString(128);
                std::string key2 = fdp.ConsumeRandomLengthString(64);
                double value2 = fdp.ConsumeFloatingPoint<double>();
                
                cJSON_AddStringToObject(original, key1.c_str(), value1.c_str());
                cJSON_AddNumberToObject(original, key2.c_str(), value2);
                
                // Duplicate with recursion
                cJSON* duplicate = cJSON_Duplicate(original, 1);
                if (duplicate != NULL) {
                    // Compare
                    bool are_equal = cJSON_Compare(original, duplicate, 1);
                    
                    cJSON_Delete(duplicate);
                }
                
                cJSON_Delete(original);
            }
            break;
        }
        
        case 6: {
            // Test minify function
            if (!json_input.empty()) {
                // Create writable copy for minify
                char* minify_buffer = (char*)malloc(json_input.size() + 1);
                if (minify_buffer != NULL) {
                    memcpy(minify_buffer, json_input.c_str(), json_input.size());
                    minify_buffer[json_input.size()] = '\0';
                    
                    cJSON_Minify(minify_buffer);
                    
                    free(minify_buffer);
                }
            }
            break;
        }
        
        case 7: {
            // Test reference creation and arrays
            cJSON* string_ref = cJSON_CreateStringReference(string_value.c_str());
            if (string_ref != NULL) {
                cJSON_Delete(string_ref);
            }
            
            // Create number array
            std::vector<double> numbers;
            for (int i = 0; i < num_items; i++) {
                numbers.push_back(fdp.ConsumeFloatingPoint<double>());
            }
            cJSON* double_array = cJSON_CreateDoubleArray(numbers.data(), numbers.size());
            if (double_array != NULL) {
                cJSON_Delete(double_array);
            }
            
            break;
        }
    }

    return 0;
}

} // namespace harness_000
