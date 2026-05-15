#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "cjson/cJSON.h"

namespace harness_005 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful testing
    // We need enough data for JSON input, length values, and operation selection
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool require_null_terminated = fdp.ConsumeBool();
    bool test_edge_cases = fdp.ConsumeBool();
    
    // Consume various length values for testing boundary conditions
    size_t buffer_length = fdp.ConsumeIntegralInRange<size_t>(0, 4096);
    size_t json_length = fdp.ConsumeIntegralInRange<size_t>(0, 2048);
    size_t offset_adjustment = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
    
    // Consume JSON input for parsing
    std::string json_input = fdp.ConsumeRandomLengthString(4096);
    
    // Use operation to determine API flow
    switch (operation) {
        case 0: {
            // Test cJSON_ParseWithLength with various buffer lengths
            // Test 1: Normal case - buffer_length equals actual string length
            size_t actual_length = json_input.length();
            cJSON* json1 = cJSON_ParseWithLength(json_input.c_str(), actual_length);
            if (json1 != NULL) {
                cJSON_Delete(json1);
            }
            
            // Test 2: Buffer length shorter than actual string (should fail)
            if (actual_length > 0) {
                size_t short_length = actual_length / 2;
                if (short_length > 0) {
                    cJSON* json2 = cJSON_ParseWithLength(json_input.c_str(), short_length);
                    if (json2 != NULL) {
                        cJSON_Delete(json2);
                    }
                }
            }
            
            // Test 3: Buffer length equal to actual string length (safe)
            size_t long_length = actual_length;
            cJSON* json3 = cJSON_ParseWithLength(json_input.c_str(), long_length);
            if (json3 != NULL) {
                cJSON_Delete(json3);
            }
            
            // Test 4: Zero buffer length
            cJSON* json4 = cJSON_ParseWithLength(json_input.c_str(), 0);
            if (json4 != NULL) {
                cJSON_Delete(json4);
            }
            
            // Test 5: NULL input with non-zero length
            cJSON* json5 = cJSON_ParseWithLength(NULL, 100);
            if (json5 != NULL) {
                cJSON_Delete(json5);
            }
            
            // Test 6: NULL input with zero length
            cJSON* json6 = cJSON_ParseWithLength(NULL, 0);
            if (json6 != NULL) {
                cJSON_Delete(json6);
            }
            break;
        }
        
        case 1: {
            // Test cJSON_ParseWithLengthOpts with return_parse_end parameter
            const char* return_parse_end = NULL;
            size_t actual_length = json_input.length();
            
            // Test with require_null_terminated = false
            cJSON* json1 = cJSON_ParseWithLengthOpts(json_input.c_str(), actual_length, &return_parse_end, false);
            if (json1 != NULL) {
                // Check if return_parse_end points to end of parsed content
                if (return_parse_end != NULL) {
                    // Verify the parse end is within the input
                    ptrdiff_t parsed_size = return_parse_end - json_input.c_str();
                    (void)parsed_size; // Suppress unused variable warning
                }
                cJSON_Delete(json1);
            }
            
            // Test with require_null_terminated = true
            // Need to ensure the string is null-terminated within the buffer length
            if (actual_length > 0 && json_input.c_str()[actual_length - 1] == '\0') {
                cJSON* json2 = cJSON_ParseWithLengthOpts(json_input.c_str(), actual_length, NULL, true);
                if (json2 != NULL) {
                    cJSON_Delete(json2);
                }
            }
            
            // Test with NULL return_parse_end
            cJSON* json3 = cJSON_ParseWithLengthOpts(json_input.c_str(), actual_length, NULL, require_null_terminated);
            if (json3 != NULL) {
                cJSON_Delete(json3);
            }
            // Test error handling - get error pointer after failed parse
            // Use a subset of the input as invalid JSON
            size_t invalid_json_length = fdp.ConsumeIntegralInRange<size_t>(1, 100);
            if (invalid_json_length > fdp.remaining_bytes()) {
                invalid_json_length = fdp.remaining_bytes();
            }
            std::string invalid_json = fdp.ConsumeBytesAsString(invalid_json_length);
            cJSON* json4 = cJSON_ParseWithLengthOpts(invalid_json.c_str(), invalid_json.length(), NULL, false);
            if (json4 == NULL) {
                // Get error pointer
                const char* error_ptr = cJSON_GetErrorPtr();
                (void)error_ptr; // Suppress unused variable warning
            } else {
                cJSON_Delete(json4);
            }
            break;
        }
        
        case 2: {
            // Test length-bounded parsing with partial JSON strings
            // Parse subsets of the fuzzer input at different lengths
            size_t actual_length = json_input.length();
            
            // Test parsing different prefixes of the JSON input
            if (actual_length > 10) {
                for (size_t i = 10; i < actual_length && i < 100; i += 15) {
                    cJSON* json = cJSON_ParseWithLength(json_input.c_str(), i);
                    if (json != NULL) {
                        cJSON_Delete(json);
                    }
                }
            }
            
            // Also test with small lengths for edge cases
            for (size_t i = 1; i <= 10 && i <= actual_length; i++) {
                cJSON* json = cJSON_ParseWithLength(json_input.c_str(), i);
                if (json != NULL) {
                    cJSON_Delete(json);
                }
            }
            break;
        }
        
        case 3: {
            // Test boundary conditions around buffer_length
            size_t actual_length = json_input.length();
            
            if (actual_length > 0) {
                // Test with buffer_length = 1
                cJSON* json1 = cJSON_ParseWithLength(json_input.c_str(), 1);
                if (json1 != NULL) {
                    cJSON_Delete(json1);
                }
                
                // Test with buffer_length = actual_length - 1
                cJSON* json2 = cJSON_ParseWithLength(json_input.c_str(), actual_length - 1);
                if (json2 != NULL) {
                    cJSON_Delete(json2);
                }
                
                // Test with buffer_length = actual_length (safe boundary)
                cJSON* json3 = cJSON_ParseWithLength(json_input.c_str(), actual_length);
                if (json3 != NULL) {
                    cJSON_Delete(json3);
                }
            }
            
            // Test with very large buffer_length (should still work if memory allows)
            if (test_edge_cases) {
                // Use actual buffer length instead of fixed huge value to avoid buffer overflow
                size_t huge_length = json_input.length();
                cJSON* json4 = cJSON_ParseWithLength(json_input.c_str(), huge_length);
                if (json4 != NULL) {
                    cJSON_Delete(json4);
                }
            }
            break;
        }
        
        case 4: {
            // Test interaction between ParseWithLength and other cJSON APIs
            size_t actual_length = json_input.length();
            cJSON* json = cJSON_ParseWithLength(json_input.c_str(), actual_length);
            
            if (json != NULL) {
                // Test various cJSON APIs on the parsed result
                // Get array size if it's an array
                if (cJSON_IsArray(json)) {
                    int array_size = cJSON_GetArraySize(json);
                    (void)array_size;
                    
                    // Try to get array items
                    for (int i = 0; i < array_size && i < 10; i++) {
                        cJSON* item = cJSON_GetArrayItem(json, i);
                        (void)item;
                    }
                }
                
                // Get object items if it's an object
                if (cJSON_IsObject(json)) {
                    // Try to get object items
                    cJSON* item = cJSON_GetObjectItem(json, "key");
                    (void)item;
                    
                    // Check if object has specific item
                    cJSON_bool has_item = cJSON_HasObjectItem(json, "key");
                    (void)has_item;
                }
                
                // Test type checking functions
                cJSON_bool is_invalid = cJSON_IsInvalid(json);
                cJSON_bool is_string = cJSON_IsString(json);
                cJSON_bool is_number = cJSON_IsNumber(json);
                cJSON_bool is_object = cJSON_IsObject(json);
                cJSON_bool is_array = cJSON_IsArray(json);
                (void)is_invalid;
                (void)is_string;
                (void)is_number;
                (void)is_object;
                (void)is_array;
                
                // Get string or number values if applicable
                if (cJSON_IsString(json)) {
                    char* str_val = cJSON_GetStringValue(json);
                    (void)str_val;
                }
                
                if (cJSON_IsNumber(json)) {
                    double num_val = cJSON_GetNumberValue(json);
                    (void)num_val;
                }
                
                cJSON_Delete(json);
            }
            break;
        }
        
        case 5: {
            // Test with non-ASCII and binary data in the buffer
            // Consume remaining bytes as binary data
            std::vector<uint8_t> binary_data = fdp.ConsumeRemainingBytes<uint8_t>();
            
            if (!binary_data.empty()) {
                // Test parsing binary data as if it were JSON
                cJSON* json = cJSON_ParseWithLength(reinterpret_cast<const char*>(binary_data.data()), 
                                                    binary_data.size());
                if (json != NULL) {
                    cJSON_Delete(json);
                }
                
                // Test with mixed binary and text
                if (binary_data.size() > 100) {
                    // Use first 100 bytes
                    cJSON* json2 = cJSON_ParseWithLength(reinterpret_cast<const char*>(binary_data.data()), 100);
                    if (json2 != NULL) {
                        cJSON_Delete(json2);
                    }
                }
            }
            break;
        }
        
        case 6: {
            // Test memory allocation edge cases
            // Create a very large JSON string to test memory limits
            std::string large_json;
            if (test_edge_cases && json_input.length() > 100) {
                // Repeat the input to create a larger string
                for (int i = 0; i < 10 && large_json.length() < 10000; i++) {
                    large_json += json_input;
                }
                
                if (!large_json.empty()) {
                    cJSON* json = cJSON_ParseWithLength(large_json.c_str(), large_json.length());
                    if (json != NULL) {
                        cJSON_Delete(json);
                    }
                }
            }
            
            // Test with empty string
            cJSON* json2 = cJSON_ParseWithLength("", 0);
            if (json2 != NULL) {
                cJSON_Delete(json2);
            }
            
            // Test with whitespace-only string
            cJSON* json3 = cJSON_ParseWithLength("   \t\n\r  ", 8);
            if (json3 != NULL) {
                cJSON_Delete(json3);
            }
            break;
        }
        
        case 7: {
            // Test combination of ParseWithLength and printing functions
            size_t actual_length = json_input.length();
            cJSON* json = cJSON_ParseWithLength(json_input.c_str(), actual_length);
            
            if (json != NULL) {
                // Test printing the parsed JSON
                char* printed_formatted = cJSON_Print(json);
                if (printed_formatted != NULL) {
                    free(printed_formatted);
                }
                
                char* printed_unformatted = cJSON_PrintUnformatted(json);
                if (printed_unformatted != NULL) {
                    free(printed_unformatted);
                }
                
                // Test buffered printing
                int prebuffer = fdp.ConsumeIntegralInRange<int>(1, 1024);
                char* printed_buffered = cJSON_PrintBuffered(json, prebuffer, fdp.ConsumeBool());
                if (printed_buffered != NULL) {
                    free(printed_buffered);
                }
                
                // Test preallocated printing
                int buffer_size = fdp.ConsumeIntegralInRange<int>(1, 2048);
                char* print_buffer = (char*)malloc(buffer_size);
                if (print_buffer != NULL) {
                    cJSON_bool print_success = cJSON_PrintPreallocated(json, print_buffer, buffer_size, fdp.ConsumeBool());
                    (void)print_success;
                    free(print_buffer);
                }
                
                cJSON_Delete(json);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_005
