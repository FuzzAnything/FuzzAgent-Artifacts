#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "cjson/cJSON.h"

namespace harness_004 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful testing
    // We need enough data for JSON input, buffer sizes, and operation selection
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool formatted = fdp.ConsumeBool();
    bool test_edge_cases = fdp.ConsumeBool();
    int buffer_size = fdp.ConsumeIntegralInRange<int>(1, 4096);
    int prebuffer_size = fdp.ConsumeIntegralInRange<int>(1, 2048);
    
    // Consume JSON input for parsing and minification
    std::string json_input = fdp.ConsumeRandomLengthString(2048);
    
    // Consume strings for object creation
    std::string key_name = fdp.ConsumeRandomLengthString(128);
    std::string string_value = fdp.ConsumeRandomLengthString(256);
    std::string raw_json = fdp.ConsumeRandomLengthString(512);
    
    // Consume numeric values
    double number_value = fdp.ConsumeFloatingPoint<double>();
    bool bool_value = fdp.ConsumeBool();
    
    // Use operation to determine API flow
    switch (operation) {
        case 0: {
            // Comprehensive test of cJSON_PrintPreallocated with various buffer sizes
            cJSON* root = cJSON_CreateObject();
            if (root != NULL) {
                // Add various types of data to the object
                cJSON_AddStringToObject(root, key_name.c_str(), string_value.c_str());
                cJSON_AddNumberToObject(root, "number", number_value);
                cJSON_AddBoolToObject(root, "bool", bool_value);
                cJSON_AddRawToObject(root, "raw", raw_json.c_str());
                
                // Test cJSON_PrintPreallocated with different buffer sizes
                char* buffer = (char*)malloc(buffer_size);
                if (buffer != NULL) {
                    // Test with formatted output
                    cJSON_bool success = cJSON_PrintPreallocated(root, buffer, buffer_size, formatted);
                    
                    // Test with unformatted output
                    if (test_edge_cases && buffer_size > 100) {
                        cJSON_bool success2 = cJSON_PrintPreallocated(root, buffer, buffer_size / 2, !formatted);
                    }
                    
                    free(buffer);
                }
                
                // Test edge cases: zero buffer size, negative buffer size
                if (test_edge_cases) {
                    char small_buffer[1];
                    cJSON_bool zero_success = cJSON_PrintPreallocated(root, small_buffer, 0, formatted);
                    
                    // Test with NULL buffer
                    cJSON_bool null_success = cJSON_PrintPreallocated(root, NULL, buffer_size, formatted);
                }
                
                cJSON_Delete(root);
            }
            break;
        }
        
        case 1: {
            // Test cJSON_PrintBuffered with various prebuffer sizes
            cJSON* item = cJSON_Parse(json_input.c_str());
            if (item != NULL) {
                // Test with different prebuffer sizes
                char* printed = cJSON_PrintBuffered(item, prebuffer_size, formatted);
                if (printed != NULL) {
                    free(printed);
                }
                
                // Test edge cases: small prebuffer
                if (test_edge_cases) {
                    char* small_print = cJSON_PrintBuffered(item, 1, formatted);
                    if (small_print != NULL) {
                        free(small_print);
                    }
                    
                    // Test with zero prebuffer
                    char* zero_print = cJSON_PrintBuffered(item, 0, formatted);
                    if (zero_print != NULL) {
                        free(zero_print);
                    }
                }
                
                cJSON_Delete(item);
            }
            break;
        }
        
        case 2: {
            // Comprehensive cJSON_Minify testing with various JSON inputs
            if (!json_input.empty()) {
                // Test 1: Minify regular JSON
                char* minify_buffer1 = (char*)malloc(json_input.size() + 1);
                if (minify_buffer1 != NULL) {
                    memcpy(minify_buffer1, json_input.c_str(), json_input.size());
                    minify_buffer1[json_input.size()] = '\0';
                    
                    cJSON_Minify(minify_buffer1);
                    
                    free(minify_buffer1);
                }
                
                // Test 2: Create JSON with whitespace and comments for minification
                std::string json_with_whitespace = "  {  \n  \"key\"  :  \"" + string_value + "\"  ,  \n  \"number\"  :  " + 
                                                   std::to_string(number_value) + "  \n  }  ";
                
                char* minify_buffer2 = (char*)malloc(json_with_whitespace.size() + 1);
                if (minify_buffer2 != NULL) {
                    memcpy(minify_buffer2, json_with_whitespace.c_str(), json_with_whitespace.size());
                    minify_buffer2[json_with_whitespace.size()] = '\0';
                    
                    cJSON_Minify(minify_buffer2);
                    
                    free(minify_buffer2);
                }
                
                // Test 3: JSON with comments
                if (test_edge_cases) {
                    std::string json_with_comments = "/* comment */ { \"key\": \"" + string_value + 
                                                     "\" } // line comment";
                    
                    char* minify_buffer3 = (char*)malloc(json_with_comments.size() + 1);
                    if (minify_buffer3 != NULL) {
                        memcpy(minify_buffer3, json_with_comments.c_str(), json_with_comments.size());
                        minify_buffer3[json_with_comments.size()] = '\0';
                        
                        cJSON_Minify(minify_buffer3);
                        
                        free(minify_buffer3);
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Test combination of parsing, minifying, and printing
            if (!json_input.empty()) {
                // Parse JSON
                cJSON* parsed = cJSON_Parse(json_input.c_str());
                if (parsed != NULL) {
                    // Print it with different methods
                    char* printed1 = formatted ? cJSON_Print(parsed) : cJSON_PrintUnformatted(parsed);
                    if (printed1 != NULL) {
                        free(printed1);
                    }
                    
                    // Print buffered
                    char* printed2 = cJSON_PrintBuffered(parsed, buffer_size, formatted);
                    if (printed2 != NULL) {
                        free(printed2);
                    }
                    
                    // Print preallocated
                    char* prealloc_buffer = (char*)malloc(buffer_size);
                    if (prealloc_buffer != NULL) {
                        cJSON_PrintPreallocated(parsed, prealloc_buffer, buffer_size, formatted);
                        free(prealloc_buffer);
                    }
                    
                    cJSON_Delete(parsed);
                }
                
                // Also test minify on the same input
                char* minify_test = (char*)malloc(json_input.size() + 1);
                if (minify_test != NULL) {
                    memcpy(minify_test, json_input.c_str(), json_input.size());
                    minify_test[json_input.size()] = '\0';
                    
                    cJSON_Minify(minify_test);
                    
                    free(minify_test);
                }
            }
            break;
        }
        
        case 4: {
            // Test cJSON_PrintPreallocated with complex nested structures
            cJSON* root = cJSON_CreateObject();
            if (root != NULL) {
                // Create nested structure
                cJSON* nested_obj = cJSON_AddObjectToObject(root, "nested");
                if (nested_obj != NULL) {
                    cJSON_AddStringToObject(nested_obj, "inner_key", string_value.c_str());
                    cJSON_AddNumberToObject(nested_obj, "inner_number", number_value);
                    
                    // Add array inside nested object
                    cJSON* inner_array = cJSON_AddArrayToObject(nested_obj, "inner_array");
                    if (inner_array != NULL) {
                        for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                            double array_val = fdp.ConsumeFloatingPoint<double>();
                            cJSON* array_item = cJSON_CreateNumber(array_val);
                            cJSON_AddItemToArray(inner_array, array_item);
                        }
                    }
                }
                
                // Add array to root
                cJSON* root_array = cJSON_AddArrayToObject(root, "root_array");
                if (root_array != NULL) {
                    for (int i = 0; i < 2 && fdp.remaining_bytes() > 0; i++) {
                        std::string array_str = fdp.ConsumeRandomLengthString(64);
                        cJSON* array_item = cJSON_CreateString(array_str.c_str());
                        cJSON_AddItemToArray(root_array, array_item);
                    }
                }
                
                // Test printing with preallocated buffer
                char* buffer = (char*)malloc(buffer_size);
                if (buffer != NULL) {
                    // Test with exact buffer size
                    cJSON_bool success = cJSON_PrintPreallocated(root, buffer, buffer_size, formatted);
                    
                    // Test with half buffer size (potential truncation)
                    if (test_edge_cases && buffer_size > 10) {
                        cJSON_bool success_half = cJSON_PrintPreallocated(root, buffer, buffer_size / 2, formatted);
                    }
                    
                    free(buffer);
                }
                
                cJSON_Delete(root);
            }
            break;
        }
        
        case 5: {
            // Test edge cases for minify function
            // Test 1: Empty string
            char empty_buffer[1] = {'\0'};
            cJSON_Minify(empty_buffer);
            
            // Test 2: String with only whitespace
            std::string whitespace_only = "   \n\t\r  ";
            char* whitespace_buffer = (char*)malloc(whitespace_only.size() + 1);
            if (whitespace_buffer != NULL) {
                memcpy(whitespace_buffer, whitespace_only.c_str(), whitespace_only.size());
                whitespace_buffer[whitespace_only.size()] = '\0';
                
                cJSON_Minify(whitespace_buffer);
                
                free(whitespace_buffer);
            }
            
            // Test 3: String with only comments
            if (test_edge_cases) {
                std::string comments_only = "/* multi-line comment */\n// single line comment\n";
                char* comments_buffer = (char*)malloc(comments_only.size() + 1);
                if (comments_buffer != NULL) {
                    memcpy(comments_buffer, comments_only.c_str(), comments_only.size());
                    comments_buffer[comments_only.size()] = '\0';
                    
                    cJSON_Minify(comments_buffer);
                    
                    free(comments_buffer);
                }
            }
            
            // Test 4: Malformed JSON (should still minify what it can)
            std::string malformed_json = "{ \"key\": \"value\" // missing closing brace and quote";
            char* malformed_buffer = (char*)malloc(malformed_json.size() + 1);
            if (malformed_buffer != NULL) {
                memcpy(malformed_buffer, malformed_json.c_str(), malformed_json.size());
                malformed_buffer[malformed_json.size()] = '\0';
                
                cJSON_Minify(malformed_buffer);
                
                free(malformed_buffer);
            }
            
            break;
        }
        
        case 6: {
            // Test cJSON_PrintPreallocated with various object types
            // Test with string
            cJSON* string_item = cJSON_CreateString(string_value.c_str());
            if (string_item != NULL) {
                char* buffer = (char*)malloc(buffer_size);
                if (buffer != NULL) {
                    cJSON_PrintPreallocated(string_item, buffer, buffer_size, formatted);
                    free(buffer);
                }
                cJSON_Delete(string_item);
            }
            
            // Test with number
            cJSON* number_item = cJSON_CreateNumber(number_value);
            if (number_item != NULL) {
                char* buffer = (char*)malloc(buffer_size);
                if (buffer != NULL) {
                    cJSON_PrintPreallocated(number_item, buffer, buffer_size, formatted);
                    free(buffer);
                }
                cJSON_Delete(number_item);
            }
            
            // Test with boolean
            cJSON* bool_item = cJSON_CreateBool(bool_value);
            if (bool_item != NULL) {
                char* buffer = (char*)malloc(buffer_size);
                if (buffer != NULL) {
                    cJSON_PrintPreallocated(bool_item, buffer, buffer_size, formatted);
                    free(buffer);
                }
                cJSON_Delete(bool_item);
            }
            
            // Test with null
            cJSON* null_item = cJSON_CreateNull();
            if (null_item != NULL) {
                char* buffer = (char*)malloc(buffer_size);
                if (buffer != NULL) {
                    cJSON_PrintPreallocated(null_item, buffer, buffer_size, formatted);
                    free(buffer);
                }
                cJSON_Delete(null_item);
            }
            
            break;
        }
        
        case 7: {
            // Test printing functions with invalid/edge case inputs
            // Test with NULL item
            if (test_edge_cases) {
                char* buffer = (char*)malloc(buffer_size);
                if (buffer != NULL) {
                    // These should handle NULL gracefully
                    cJSON_PrintPreallocated(NULL, buffer, buffer_size, formatted);
                    free(buffer);
                }
                
                // Test printing NULL
                char* printed_null = cJSON_Print(NULL);
                if (printed_null != NULL) {
                    free(printed_null);
                }
                
                char* printed_buffered_null = cJSON_PrintBuffered(NULL, buffer_size, formatted);
                if (printed_buffered_null != NULL) {
                    free(printed_buffered_null);
                }
            }
            
            // Test minify with NULL
            if (test_edge_cases) {
                cJSON_Minify(NULL);
            }
            
            // Test with large buffer sizes
            if (test_edge_cases && fdp.remaining_bytes() > 100) {
                cJSON* simple = cJSON_CreateString("test");
                if (simple != NULL) {
                    int large_buffer_size = fdp.ConsumeIntegralInRange<int>(10000, 100000);
                    char* large_buffer = (char*)malloc(large_buffer_size);
                    if (large_buffer != NULL) {
                        cJSON_PrintPreallocated(simple, large_buffer, large_buffer_size, formatted);
                        free(large_buffer);
                    }
                    cJSON_Delete(simple);
                }
            }
            
            break;
        }
    }

    return 0;
}

} // namespace harness_004
