#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"
#include "cjson/cJSON_Utils.h"

namespace harness_000 {

extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 4) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Split input: first portion for JSON parsing, second portion for JSON creation
    size_t split_point = fdp.ConsumeIntegralInRange<size_t>(0, size);
    std::vector<uint8_t> parse_data = fdp.ConsumeBytes<uint8_t>(split_point);
    std::vector<uint8_t> create_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Test 1: Parse JSON from first portion of input
    if (!parse_data.empty()) {
        FuzzedDataProvider parse_fdp(parse_data.data(), parse_data.size());
        std::string json_input = parse_fdp.ConsumeRemainingBytesAsString();
        cJSON* parsed_json = cJSON_Parse(json_input.c_str());
        
        // If parsing succeeds, test various operations on the parsed JSON
        if (parsed_json != NULL) {
            // Test printing functions
            int print_option = parse_fdp.ConsumeIntegralInRange<int>(0, 2);
            char* printed = NULL;
            switch (print_option) {
                case 0:
                    printed = cJSON_Print(parsed_json);
                    break;
                case 1:
                    printed = cJSON_PrintUnformatted(parsed_json);
                    break;
                case 2: {
                    int prebuffer = parse_fdp.ConsumeIntegralInRange<int>(1, 1024);
                    printed = cJSON_PrintBuffered(parsed_json, prebuffer, parse_fdp.ConsumeBool());
                    break;
                }
            }
            if (printed != NULL) {
                free(printed);
            }

            // Test utility functions
            if (cJSON_IsArray(parsed_json)) {
                int array_size = cJSON_GetArraySize(parsed_json);
                if (array_size > 0) {
                    int index = parse_fdp.ConsumeIntegralInRange<int>(0, array_size - 1);
                    cJSON* array_item = cJSON_GetArrayItem(parsed_json, index);
                    // Do nothing with array_item, just testing the function
                }
            }

            if (cJSON_IsObject(parsed_json)) {
                // Create a random key to check
                std::string test_key = parse_fdp.ConsumeRandomLengthString(32);
                cJSON* object_item = cJSON_GetObjectItem(parsed_json, test_key.c_str());
                // Do nothing with object_item, just testing the function
            }

            cJSON_Delete(parsed_json);
        }
    }
    
    // Test 2: Create JSON objects using second portion of input
    if (!create_data.empty()) {
        FuzzedDataProvider create_fdp(create_data.data(), create_data.size());
        
        // Create a new JSON object with random structure
        cJSON* root = cJSON_CreateObject();
        if (root != NULL) {
            // Add various types of items to the object
            int num_items = create_fdp.ConsumeIntegralInRange<int>(0, 10);
            for (int i = 0; i < num_items; i++) {
                std::string key = create_fdp.ConsumeRandomLengthString(16);
                int item_type = create_fdp.ConsumeIntegralInRange<int>(0, 6);
                
                switch (item_type) {
                    case 0:
                        cJSON_AddNullToObject(root, key.c_str());
                        break;
                    case 1:
                        cJSON_AddTrueToObject(root, key.c_str());
                        break;
                    case 2:
                        cJSON_AddFalseToObject(root, key.c_str());
                        break;
                    case 3:
                        cJSON_AddNumberToObject(root, key.c_str(), create_fdp.ConsumeFloatingPoint<double>());
                        break;
                    case 4:
                        cJSON_AddStringToObject(root, key.c_str(), create_fdp.ConsumeRandomLengthString(32).c_str());
                        break;
                    case 5: {
                        cJSON* array = cJSON_CreateArray();
                        if (array != NULL) {
                            int array_size = create_fdp.ConsumeIntegralInRange<int>(0, 5);
                            for (int j = 0; j < array_size; j++) {
                                int array_item_type = create_fdp.ConsumeIntegralInRange<int>(0, 2);
                                if (array_item_type == 0) {
                                    cJSON_AddItemToArray(array, cJSON_CreateNumber(create_fdp.ConsumeFloatingPoint<double>()));
                                } else if (array_item_type == 1) {
                                    cJSON_AddItemToArray(array, cJSON_CreateString(create_fdp.ConsumeRandomLengthString(16).c_str()));
                                } else {
                                    cJSON_AddItemToArray(array, cJSON_CreateBool(create_fdp.ConsumeBool()));
                                }
                            }
                            cJSON_AddItemToObject(root, key.c_str(), array);
                        }
                        break;
                    }
                    case 6: {
                        cJSON* nested_object = cJSON_CreateObject();
                        if (nested_object != NULL) {
                            cJSON_AddStringToObject(nested_object, "nested_key", create_fdp.ConsumeRandomLengthString(16).c_str());
                            cJSON_AddItemToObject(root, key.c_str(), nested_object);
                        }
                        break;
                    }
                }
            }

            // Test printing the created object
            char* created_json_str = cJSON_Print(root);
            if (created_json_str != NULL) {
                free(created_json_str);
            }

            cJSON_Delete(root);
        }

        // Test array creation APIs
        cJSON* array = cJSON_CreateArray();
        if (array != NULL) {
            int array_size = create_fdp.ConsumeIntegralInRange<int>(0, 8);
            for (int i = 0; i < array_size; i++) {
                int item_type = create_fdp.ConsumeIntegralInRange<int>(0, 4);
                if (item_type == 0) {
                    cJSON_AddItemToArray(array, cJSON_CreateNumber(create_fdp.ConsumeFloatingPoint<double>()));
                } else if (item_type == 1) {
                    cJSON_AddItemToArray(array, cJSON_CreateString(create_fdp.ConsumeRandomLengthString(16).c_str()));
                } else if (item_type == 2) {
                    cJSON_AddItemToArray(array, cJSON_CreateBool(create_fdp.ConsumeBool()));
                } else if (item_type == 3) {
                    cJSON_AddItemToArray(array, cJSON_CreateNull());
                } else {
                    cJSON_AddItemToArray(array, cJSON_CreateArray());
                }
            }
            cJSON_Delete(array);
        }
    }

    return 0;
}

} // namespace harness_000
