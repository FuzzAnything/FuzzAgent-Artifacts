#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_003 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful testing of duplication and printing operations
    // Need enough data for: operation type, JSON data, duplication mode, buffer sizes, etc.
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Determine the main operation to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // First, create a JSON structure to work with
    cJSON* root = NULL;
    
    // Determine the type of JSON structure to create
    uint8_t structure_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    switch (structure_type) {
        case 0: {
            // Create a complex nested object
            root = cJSON_CreateObject();
            if (root == NULL) return 0;
            
            // Add various items to create a complex structure
            int num_items = fdp.ConsumeIntegralInRange<int>(1, 5);
            for (int i = 0; i < num_items && fdp.remaining_bytes() > 0; i++) {
                // Generate key
                size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                if (fdp.remaining_bytes() < key_len) break;
                std::string key = fdp.ConsumeBytesAsString(key_len);
                
                // Determine value type
                uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 6;
                
                switch (value_type) {
                    case 0:
                        cJSON_AddNullToObject(root, key.c_str());
                        break;
                    case 1:
                        cJSON_AddTrueToObject(root, key.c_str());
                        break;
                    case 2:
                        cJSON_AddFalseToObject(root, key.c_str());
                        break;
                    case 3: {
                        double num_val = fdp.ConsumeFloatingPoint<double>();
                        cJSON_AddNumberToObject(root, key.c_str(), num_val);
                        break;
                    }
                    case 4: {
                        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                        if (fdp.remaining_bytes() < str_len) break;
                        std::string str_val = fdp.ConsumeBytesAsString(str_len);
                        cJSON_AddStringToObject(root, key.c_str(), str_val.c_str());
                        break;
                    }
                    case 5: {
                        // Add nested array
                        cJSON* nested_array = cJSON_AddArrayToObject(root, key.c_str());
                        if (nested_array != NULL && fdp.remaining_bytes() > 0) {
                            int array_size = fdp.ConsumeIntegralInRange<int>(1, 3);
                            for (int j = 0; j < array_size && fdp.remaining_bytes() > 0; j++) {
                                double array_val = fdp.ConsumeFloatingPoint<double>();
                                cJSON_AddItemToArray(nested_array, cJSON_CreateNumber(array_val));
                            }
                        }
                        break;
                    }
                }
            }
            break;
        }
        case 1: {
            // Create a complex nested array
            root = cJSON_CreateArray();
            if (root == NULL) return 0;
            
            int array_size = fdp.ConsumeIntegralInRange<int>(1, 5);
            for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                uint8_t item_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                
                switch (item_type) {
                    case 0:
                        cJSON_AddItemToArray(root, cJSON_CreateNull());
                        break;
                    case 1:
                        cJSON_AddItemToArray(root, cJSON_CreateBool(fdp.ConsumeBool()));
                        break;
                    case 2: {
                        double num_val = fdp.ConsumeFloatingPoint<double>();
                        cJSON_AddItemToArray(root, cJSON_CreateNumber(num_val));
                        break;
                    }
                    case 3: {
                        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                        if (fdp.remaining_bytes() < str_len) break;
                        std::string str_val = fdp.ConsumeBytesAsString(str_len);
                        cJSON_AddItemToArray(root, cJSON_CreateString(str_val.c_str()));
                        break;
                    }
                }
            }
            break;
        }
        case 2: {
            // Parse JSON from input string
            size_t json_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
            if (json_size == 0) return 0;
            
            std::string json_data = fdp.ConsumeBytesAsString(json_size);
            root = cJSON_Parse(json_data.c_str());
            if (root == NULL) {
                // Parsing failed, nothing to test
                return 0;
            }
            break;
        }
    }
    
    if (root == NULL) {
        return 0;
    }
    
    // Now test the target APIs based on the operation
    switch (operation) {
        case 0: {
            // Test cJSON_Duplicate with various modes
            bool duplicate_recursive = fdp.ConsumeBool();
            
            // Duplicate the root item
            cJSON* duplicate = cJSON_Duplicate(root, duplicate_recursive ? 1 : 0);
            
            if (duplicate != NULL) {
                // Test that the duplicate is valid
                bool is_valid = !cJSON_IsInvalid(duplicate);
                
                // Test printing the duplicate
                bool print_formatted = fdp.ConsumeBool();
                char* printed = NULL;
                
                if (print_formatted) {
                    printed = cJSON_Print(duplicate);
                } else {
                    printed = cJSON_PrintUnformatted(duplicate);
                }
                
                if (printed != NULL) {
                    free(printed);
                }
                
                // Clean up duplicate
                cJSON_Delete(duplicate);
            }
            break;
        }
        case 1: {
            // Test cJSON_Minify
            // First print the JSON to a string
            char* json_str = cJSON_PrintUnformatted(root);
            if (json_str != NULL) {
                // Create a copy to minify (minify modifies in-place)
                size_t json_len = strlen(json_str);
                char* minify_buffer = (char*)malloc(json_len + 1);
                if (minify_buffer != NULL) {
                    strcpy(minify_buffer, json_str);
                    
                    // Apply minification
                    cJSON_Minify(minify_buffer);
                    
                    // Try to parse the minified version
                    cJSON* minified_parsed = cJSON_Parse(minify_buffer);
                    if (minified_parsed != NULL) {
                        cJSON_Delete(minified_parsed);
                    }
                    
                    free(minify_buffer);
                }
                free(json_str);
            }
            break;
        }
        case 2: {
            // Test cJSON_PrintBuffered
            bool print_formatted = fdp.ConsumeBool();
            int prebuffer = fdp.ConsumeIntegralInRange<int>(1, 1024);
            
            char* buffered = cJSON_PrintBuffered(root, prebuffer, print_formatted ? 1 : 0);
            if (buffered != NULL) {
                free(buffered);
            }
            break;
        }
        case 3: {
            // Test cJSON_PrintPreallocated
            bool print_formatted = fdp.ConsumeBool();
            int buffer_size = fdp.ConsumeIntegralInRange<int>(64, 4096);
            
            // Allocate buffer for preallocated printing
            char* buffer = (char*)malloc(buffer_size);
            if (buffer != NULL) {
                cJSON_bool print_success = cJSON_PrintPreallocated(root, buffer, buffer_size, print_formatted ? 1 : 0);
                
                // If printing was successful, the buffer contains JSON
                if (print_success) {
                    // Try to parse what was printed
                    cJSON* reparsed = cJSON_Parse(buffer);
                    if (reparsed != NULL) {
                        cJSON_Delete(reparsed);
                    }
                }
                
                free(buffer);
            }
            break;
        }
    }
    
    // Test cJSON_Version (always available, no dependencies)
    const char* version = cJSON_Version();
    
    // Test related modification operations on the original
    if (cJSON_IsObject(root) && fdp.remaining_bytes() > 0) {
        // Try to replace an item
        size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
        if (fdp.remaining_bytes() >= key_len) {
            std::string replace_key = fdp.ConsumeBytesAsString(key_len);
            cJSON* existing = cJSON_GetObjectItem(root, replace_key.c_str());
            if (existing != NULL) {
                // Replace with a new value
                cJSON_ReplaceItemInObject(root, replace_key.c_str(), cJSON_CreateBool(fdp.ConsumeBool()));
            }
        }
    }
    
    // Test array modification if root is an array
    if (cJSON_IsArray(root) && fdp.remaining_bytes() > 0) {
        int array_size = cJSON_GetArraySize(root);
        if (array_size > 0) {
            int replace_index = fdp.ConsumeIntegralInRange<int>(0, array_size - 1);
            cJSON* array_item = cJSON_GetArrayItem(root, replace_index);
            if (array_item != NULL) {
                cJSON_ReplaceItemInArray(root, replace_index, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
            }
        }
    }
    
    // Clean up root
    cJSON_Delete(root);
    
    return 0;
}

} // namespace harness_003
