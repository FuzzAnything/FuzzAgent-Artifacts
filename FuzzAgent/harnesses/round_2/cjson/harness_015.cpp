#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_015 {

extern "C" {
}

// Helper to generate strings with specific characteristics
// Helper to generate strings with specific characteristics
std::string generate_test_string(FuzzedDataProvider& fdp, int string_type) {
    switch (string_type % 12) {
        case 0: // NULL string (empty string as placeholder, we'll handle NULL separately)
            return "";
        case 1: // Empty string
            return "";
        case 2: // String with actual escape sequence characters
            {
                std::string result;
                int length = fdp.ConsumeIntegralInRange<int>(1, 50);
                for (int i = 0; i < length; i++) {
                    uint8_t esc_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                    switch (esc_type) {
                        case 0: result += '\\'; break;      // backslash
                        case 1: result += '\"'; break;      // double quote
                        case 2: result += '\b'; break;      // backspace
                        case 3: result += '\f'; break;      // form feed
                        case 4: result += '\n'; break;      // newline
                        case 5: result += '\r'; break;      // carriage return
                        case 6: result += '\t'; break;      // tab
                        case 7: result += '\\'; break;      // another backslash
                    }
                }
                return result;
            }
        case 3: // String with control characters (0-31) that trigger UTF-16 escapes
            {
                std::string result;
                int length = fdp.ConsumeIntegralInRange<int>(1, 30);
                for (int i = 0; i < length; i++) {
                    uint8_t ctrl_char = fdp.ConsumeIntegralInRange<uint8_t>(0, 31);
                    result += static_cast<char>(ctrl_char);
                }
                return result;
            }
        case 4: // Very long string (test buffer reallocation)
            {
                int length = fdp.ConsumeIntegralInRange<int>(100, 1000);
                std::string result;
                result.reserve(length);
                for (int i = 0; i < length; i++) {
                    // Use fuzzer input to determine character
                    if (fdp.remaining_bytes() > 0) {
                        result += static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(32, 126));
                    } else {
                        result += 'A' + (i % 26);
                    }
                }
                return result;
            }
        case 5: // Mixed escape sequences and normal text
            {
                std::string result;
                int total_length = fdp.ConsumeIntegralInRange<int>(10, 100);
                for (int i = 0; i < total_length; i++) {
                    if (fdp.ConsumeBool()) {
                        // Add normal character
                        result += static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(32, 126));
                    } else {
                        // Add escape character
                        uint8_t esc_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                        switch (esc_type) {
                            case 0: result += '\\'; break;
                            case 1: result += '\"'; break;
                            case 2: result += '\b'; break;
                            case 3: result += '\f'; break;
                            case 4: result += '\n'; break;
                            case 5: result += '\r'; break;
                            case 6: result += '\t'; break;
                            case 7: result += '\\'; break;
                        }
                    }
                }
                return result;
            }
        case 6: // Unicode characters and special symbols
            {
                std::string result;
                int length = fdp.ConsumeIntegralInRange<int>(10, 100);
                for (int i = 0; i < length; i++) {
                    // Use extended ASCII range
                    result += static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(128, 255));
                }
                return result;
            }
        case 7: // String with all possible escape characters
            {
                std::string result;
                result += '\\';  // backslash
                result += '\"';  // double quote
                result += '\b';  // backspace
                result += '\f';  // form feed
                result += '\n';  // newline
                result += '\r';  // carriage return
                result += '\t';  // tab
                return result;
            }
        case 8: // String with backslash-quote sequences
            {
                std::string result;
                int count = fdp.ConsumeIntegralInRange<int>(1, 10);
                for (int i = 0; i < count; i++) {
                    result += '\\';
                    result += '\"';
                }
                return result;
            }
        case 9: // String with only quote characters
            {
                int count = fdp.ConsumeIntegralInRange<int>(1, 20);
                return std::string(count, '\"');
            }
        case 10: // String with only backslash characters
            {
                int count = fdp.ConsumeIntegralInRange<int>(1, 20);
                return std::string(count, '\\');
            }
        case 11: // Random string from fuzzer input
        default:
            return fdp.ConsumeRandomLengthString(100);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;  // Need minimum input for configuration
    }

    FuzzedDataProvider fdp(data, size);
    
    // Determine test scenario
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    if (scenario == 0) {
        // Scenario 0: Test NULL string inputs specifically
        cJSON* obj = cJSON_CreateObject();
        if (obj != NULL) {
            // Add a string item with NULL valuestring
            cJSON* null_string_item = cJSON_CreateString(NULL);
            if (null_string_item != NULL) {
                cJSON_AddItemToObject(obj, "null_string", null_string_item);
            }
            
            // Test printing with NULL string
            char* printed = NULL;
            uint8_t print_type = fdp.ConsumeIntegral<uint8_t>() % 4;
            switch (print_type) {
                case 0:
                    printed = cJSON_Print(obj);
                    break;
                case 1:
                    printed = cJSON_PrintUnformatted(obj);
                    break;
                case 2:
                    {
                        int prebuffer = fdp.ConsumeIntegralInRange<int>(1, 100);
                        printed = cJSON_PrintBuffered(obj, prebuffer, fdp.ConsumeBool());
                    }
                    break;
                case 3:
                    {
                        size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 200);
                        std::vector<char> buffer(buffer_size);
                        cJSON_PrintPreallocated(obj, buffer.data(), buffer.size(), fdp.ConsumeBool());
                    }
                    break;
            }
            
            if (printed != NULL) {
                free(printed);
            }
            cJSON_Delete(obj);
        }
        return 0;
    }
    
    // Create JSON objects with various string types
    cJSON* root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }
    
    // Add different types of string items
    int num_strings = fdp.ConsumeIntegralInRange<int>(1, 10);
    for (int i = 0; i < num_strings && fdp.remaining_bytes() > 0; i++) {
        std::string key = "str" + std::to_string(i);
        int string_type = fdp.ConsumeIntegralInRange<int>(0, 11);
        
        std::string str_value = generate_test_string(fdp, string_type);
        
        int create_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        cJSON* string_item = NULL;
        
        switch (create_type) {
            case 0:
                string_item = cJSON_CreateString(str_value.c_str());
                break;
            case 1:
                string_item = cJSON_CreateRaw(str_value.c_str());
                break;
            case 2:
                // Create object or array with string content
                if (fdp.ConsumeBool()) {
                    string_item = cJSON_CreateObject();
                    if (string_item != NULL) {
                        cJSON_AddStringToObject(string_item, "content", str_value.c_str());
                    }
                } else {
                    string_item = cJSON_CreateArray();
                    if (string_item != NULL) {
                        cJSON_AddItemToArray(string_item, cJSON_CreateString(str_value.c_str()));
                    }
                }
                break;
        }
        
        if (string_item != NULL) {
            cJSON_AddItemToObject(root, key.c_str(), string_item);
        }
    }
    
    // Test all printing APIs with different configurations
    uint8_t api_test = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    switch (api_test) {
        case 0: // cJSON_Print
            {
                char* printed = cJSON_Print(root);
                if (printed != NULL) {
                    free(printed);
                }
            }
            break;
            
        case 1: // cJSON_PrintUnformatted
            {
                char* printed = cJSON_PrintUnformatted(root);
                if (printed != NULL) {
                    free(printed);
                }
            }
            break;
            
        case 2: // cJSON_PrintBuffered with varying buffer sizes
            {
                // Test with very small buffer sizes to trigger ensure() failures
                int buffer_size = fdp.ConsumeIntegralInRange<int>(1, 200);
                if (fdp.ConsumeBool()) {
                    // Sometimes use extremely small buffers
                    buffer_size = fdp.ConsumeIntegralInRange<int>(1, 10);
                }
                bool format = fdp.ConsumeBool();
                char* printed = cJSON_PrintBuffered(root, buffer_size, format);
                if (printed != NULL) {
                    free(printed);
                }
            }
            break;
            
        case 3: // cJSON_PrintPreallocated with preallocated buffers
            {
                size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                std::vector<char> buffer(buffer_size);
                bool format = fdp.ConsumeBool();
                
                // Test with NULL buffer (should fail gracefully)
                if (fdp.ConsumeBool() && buffer_size > 10) {
                    cJSON_PrintPreallocated(root, NULL, buffer_size, format);
                } else {
                    cJSON_PrintPreallocated(root, buffer.data(), buffer.size(), format);
                }
            }
            break;
            
        case 4: // Comprehensive test - test multiple APIs
            {
                // Test cJSON_Print
                char* printed1 = cJSON_Print(root);
                if (printed1 != NULL) free(printed1);
                
                // Test cJSON_PrintUnformatted
                char* printed2 = cJSON_PrintUnformatted(root);
                if (printed2 != NULL) free(printed2);
                
                // Test cJSON_PrintBuffered with small buffer
                int small_buffer = fdp.ConsumeIntegralInRange<int>(1, 20);
                char* printed3 = cJSON_PrintBuffered(root, small_buffer, false);
                if (printed3 != NULL) free(printed3);
                
                // Test cJSON_PrintPreallocated
                size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(50, 500);
                std::vector<char> buffer(buffer_size);
                cJSON_PrintPreallocated(root, buffer.data(), buffer.size(), true);
            }
            break;
    }
    
    // Cleanup
    cJSON_Delete(root);
    
    return 0;
}

} // namespace harness_015
