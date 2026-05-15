#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_000 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful testing
    if (size < 8) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume a portion of input for JSON data
    size_t json_size = fdp.ConsumeIntegralInRange<size_t>(1, size - 4);
    std::string json_data = fdp.ConsumeBytesAsString(json_size);
    // Parse JSON data
    cJSON* root = cJSON_Parse(json_data.c_str());
    if (root == NULL) {
        // Parsing failed, try to get error pointer
        const char* error_ptr = cJSON_GetErrorPtr();
        return 0;
    }

    // Test various cJSON APIs based on remaining input

    // 1. Test cJSON_Print and variants
    bool print_formatted = fdp.ConsumeBool();
    bool print_buffered = fdp.ConsumeBool();
    
    char* printed_json = NULL;
    if (print_buffered) {
        int prebuffer = fdp.ConsumeIntegralInRange<int>(1, 1024);
        printed_json = cJSON_PrintBuffered(root, prebuffer, print_formatted);
    } else {
        if (print_formatted) {
            printed_json = cJSON_Print(root);
        } else {
            printed_json = cJSON_PrintUnformatted(root);
        }
    }
    
    if (printed_json != NULL) {
        free(printed_json);
    }

    // 2. Test item retrieval APIs
    if (cJSON_IsObject(root)) {
        // Get object size
        int object_size = cJSON_GetArraySize(root);
        
        // Try to get some object items
        if (object_size > 0 && fdp.remaining_bytes() > 0) {
            // Consume a key name from remaining input
            std::string key = fdp.ConsumeRandomLengthString(64);
            cJSON* item = cJSON_GetObjectItem(root, key.c_str());
            
            // Also try case-sensitive version
            cJSON* item_case = cJSON_GetObjectItemCaseSensitive(root, key.c_str());
            
            // Check if object has the item
            cJSON_bool has_item = cJSON_HasObjectItem(root, key.c_str());
        }
    }

    // 3. Test array operations if root is an array
    if (cJSON_IsArray(root)) {
        int array_size = cJSON_GetArraySize(root);
        
        if (array_size > 0 && fdp.remaining_bytes() > 0) {
            // Get a random array item
            int index = fdp.ConsumeIntegralInRange<int>(0, array_size - 1);
            cJSON* array_item = cJSON_GetArrayItem(root, index);
            
            // If we have an item, test type checking functions
            if (array_item != NULL) {
                // Test type checking APIs
                cJSON_bool is_invalid = cJSON_IsInvalid(array_item);
                cJSON_bool is_false = cJSON_IsFalse(array_item);
                cJSON_bool is_true = cJSON_IsTrue(array_item);
                cJSON_bool is_bool = cJSON_IsBool(array_item);
                cJSON_bool is_null = cJSON_IsNull(array_item);
                cJSON_bool is_number = cJSON_IsNumber(array_item);
                cJSON_bool is_string = cJSON_IsString(array_item);
                cJSON_bool is_array = cJSON_IsArray(array_item);
                cJSON_bool is_object = cJSON_IsObject(array_item);
                cJSON_bool is_raw = cJSON_IsRaw(array_item);
                
                // Test value extraction APIs
                if (is_string) {
                    char* str_val = cJSON_GetStringValue(array_item);
                }
                if (is_number) {
                    double num_val = cJSON_GetNumberValue(array_item);
                }
            }
        }
    }

    // 4. Test iteration if we have enough bytes
    if (fdp.remaining_bytes() > 0) {
        // Create a simple iteration test
        cJSON* child = root->child;
        while (child != NULL && fdp.remaining_bytes() > 0) {
            // Consume a small amount of data for each iteration
            bool test_bool = fdp.ConsumeBool();
            child = child->next;
        }
    }

    // 5. Test cJSON_Minify if we have remaining input
    if (fdp.remaining_bytes() > 0) {
        std::string minify_data = fdp.ConsumeRemainingBytesAsString();
        if (minify_data.size() > 0) {
            // Create writable copy for minify (it modifies input)
            char* writable = (char*)malloc(minify_data.size() + 1);
            if (writable != NULL) {
                memcpy(writable, minify_data.c_str(), minify_data.size());
                writable[minify_data.size()] = '\0';
                cJSON_Minify(writable);
                free(writable);
            }
        }
    }

    // Clean up
    cJSON_Delete(root);

    return 0;
}

} // namespace harness_000
