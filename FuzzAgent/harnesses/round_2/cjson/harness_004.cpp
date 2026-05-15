#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_004 {

extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Minimum input size for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Create mutable JSON strings with various content for minification testing
    // Strategy: Use fuzzer input to generate JSON with whitespace, comments, strings, numbers, arrays, objects
    
    // Step 1: Determine the structure of JSON to create from fuzzer input
    int json_type = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    std::string mutable_json;
    
    // Build JSON with whitespace and comments for minification using fuzzer data
    switch (json_type) {
        case 0:  // Simple object with whitespace
            {
                std::string key1 = fdp.ConsumeRandomLengthString(20);
                std::string value1 = fdp.ConsumeRandomLengthString(20);
                int int_value = fdp.ConsumeIntegral<int>();
                bool bool_value = fdp.ConsumeBool();
                
                mutable_json = "{\n  \"" + key1 + "\": \"" + value1 + "\",\n  " +
                              "\"number\": " + std::to_string(int_value) + ",\n  " +
                              "\"bool\": " + (bool_value ? "true" : "false") + "\n}";
            }
            break;
        case 1:  // Array with whitespace and comments
            {
                double double_value = fdp.ConsumeFloatingPoint<double>();
                std::string string_value = fdp.ConsumeRandomLengthString(30);
                bool bool_value = fdp.ConsumeBool();
                
                mutable_json = "[\n  /* " + fdp.ConsumeRandomLengthString(20) + " */\n  " +
                              std::to_string(double_value) + ",\n  " +
                              "\"" + string_value + "\",\n  " +
                              "null,\n  // " + fdp.ConsumeRandomLengthString(15) + "\n  " +
                              (bool_value ? "true" : "false") + "\n]";
            }
            break;
        case 2:  // Nested structure
            {
                int inner_value = fdp.ConsumeIntegral<int>();
                std::string comment_text = fdp.ConsumeRandomLengthString(25);
                
                mutable_json = "{\n  \"outer\": {\n    \"inner\": " +
                              std::to_string(inner_value) + ",\n    " +
                              "\"array\": [" + 
                              std::to_string(fdp.ConsumeIntegral<int>()) + ", " +
                              std::to_string(fdp.ConsumeIntegral<int>()) + ", " +
                              std::to_string(fdp.ConsumeIntegral<int>()) + ", " +
                              std::to_string(fdp.ConsumeIntegral<int>()) + "]\n  },\n  " +
                              "\"comment\": /* " + comment_text + " */ \"" +
                              fdp.ConsumeRandomLengthString(20) + "\"\n}";
            }
            break;
        case 3:  // Complex JSON with mixed content
            {
                std::string string_val = fdp.ConsumeRandomLengthString(40);
                double number_val = fdp.ConsumeFloatingPoint<double>();
                bool bool_val = fdp.ConsumeBool();
                std::string header_comment = fdp.ConsumeRandomLengthString(30);
                std::string footer_comment = fdp.ConsumeRandomLengthString(25);
                
                mutable_json = "/* " + header_comment + " */\n{\n  \"string\": \"" +
                              string_val + "\",\n  " +
                              "\"number\": " + std::to_string(number_val) + ",\n  " +
                              "\"bool\": " + (bool_val ? "true" : "false") + ",\n  " +
                              "\"null\": null,\n  " +
                              "\"array\": [\n    // " + fdp.ConsumeRandomLengthString(15) + "\n    " +
                              "{\"nested\": " + (fdp.ConsumeBool() ? "true" : "false") + "},\n    " +
                              std::to_string(fdp.ConsumeFloatingPoint<double>()) + ",\n    " +
                              "\"" + fdp.ConsumeRandomLengthString(10) + "\"\n  ]\n}\n// " +
                              footer_comment;
            }
            break;
    }
    
    // Step 2: Create mutable copy for minification
    char* json_copy = strdup(mutable_json.c_str());
    if (json_copy == NULL) {
        return 0;
    }
    
    // Step 3: Call cJSON_Minify to remove whitespace and comments
    cJSON_Minify(json_copy);
    
    // Step 4: Parse the minified JSON using length-based parsing functions
    cJSON* parsed_json = NULL;
    
    int parse_method = fdp.ConsumeIntegralInRange<int>(0, 2);
    
    switch (parse_method) {
        case 0:  // cJSON_ParseWithLength
            parsed_json = cJSON_ParseWithLength(json_copy, strlen(json_copy));
            break;
        case 1:  // cJSON_ParseWithLengthOpts without requiring null termination
            {
                const char* parse_end = NULL;
                parsed_json = cJSON_ParseWithLengthOpts(json_copy, strlen(json_copy), &parse_end, 0);
            }
            break;
        case 2:  // cJSON_ParseWithLengthOpts requiring null termination
            {
                const char* parse_end = NULL;
                parsed_json = cJSON_ParseWithLengthOpts(json_copy, strlen(json_copy), &parse_end, 1);
            }
            break;
    }
    
    // Step 5: If parsing succeeds, optionally extract values
    if (parsed_json != NULL) {
        // Test value extraction functions on the parsed JSON
        if (cJSON_IsObject(parsed_json)) {
            // Try to find a number value
            cJSON* number_item = cJSON_GetObjectItem(parsed_json, "number");
            if (number_item != NULL && cJSON_IsNumber(number_item)) {
                double num_value = cJSON_GetNumberValue(number_item);
                (void)num_value;  // Use value to avoid unused variable warning
            }
            
            // Try to find a string value
            cJSON* string_item = cJSON_GetObjectItem(parsed_json, "string");
            if (string_item != NULL && cJSON_IsString(string_item)) {
                char* str_value = cJSON_GetStringValue(string_item);
                (void)str_value;  // Use value to avoid unused variable warning
            }
        } else if (cJSON_IsArray(parsed_json)) {
            // Get first array item if available
            int array_size = cJSON_GetArraySize(parsed_json);
            if (array_size > 0) {
                cJSON* first_item = cJSON_GetArrayItem(parsed_json, 0);
                if (first_item != NULL) {
                    if (cJSON_IsNumber(first_item)) {
                        double num = cJSON_GetNumberValue(first_item);
                        (void)num;
                    } else if (cJSON_IsString(first_item)) {
                        char* str = cJSON_GetStringValue(first_item);
                        (void)str;
                    }
                }
            }
        }
        
        // Clean up parsed JSON
        cJSON_Delete(parsed_json);
    }
    
    // Clean up
    free(json_copy);
    
    return 0;
}

} // namespace harness_004
