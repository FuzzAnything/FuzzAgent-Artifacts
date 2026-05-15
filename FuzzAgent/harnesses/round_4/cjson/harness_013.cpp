#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_013 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for comprehensive raw string and printing testing
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Create root object
    cJSON* root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }
    
    // Determine test scenario based on input
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Helper function to create strings with control characters
    auto create_string_with_control_chars = [&fdp](size_t max_len = 256) -> std::string {
        if (fdp.remaining_bytes() == 0) return "";
        size_t len = fdp.ConsumeIntegralInRange<size_t>(1, max_len);
        if (fdp.remaining_bytes() < len) return "";
        
        std::string result;
        result.reserve(len);
        for (size_t i = 0; i < len; i++) {
            // Include control characters (0-31) and regular characters
            uint8_t c = fdp.ConsumeIntegral<uint8_t>();
            result.push_back(static_cast<char>(c));
        }
        return result;
    };
    
    // Helper function to create raw JSON strings (valid or malformed)
    auto create_raw_json = [&fdp]() -> std::string {
        if (fdp.remaining_bytes() < 8) return "";
        
        // Choose between valid JSON snippets and random malformed data
        bool create_valid = fdp.ConsumeBool();
        
        if (create_valid) {
            // Create simple valid JSON snippets
            uint8_t json_type = fdp.ConsumeIntegral<uint8_t>() % 6;
            switch (json_type) {
                case 0: return "null";
                case 1: return "true";
                case 2: return "false";
                case 3: return std::to_string(fdp.ConsumeIntegral<int>());
                case 4: {
                    std::string str = fdp.ConsumeRandomLengthString(32);
                    return "\"" + str + "\"";
                }
                case 5: {
                    return "{\"key\": " + std::to_string(fdp.ConsumeIntegral<int>()) + "}";
                }
                default: return "null";
            }
        } else {
            // Create malformed JSON
            return fdp.ConsumeRandomLengthString(64);
        }
    };
    
    // Test different scenarios
    switch (scenario) {
        case 0: {
            // Scenario 0: Test cJSON_AddRawToObject with various raw strings
            std::string key = fdp.ConsumeRandomLengthString(32);
            std::string raw_json = create_raw_json();
            
            cJSON* raw_item = cJSON_AddRawToObject(root, key.c_str(), raw_json.c_str());
            if (raw_item == NULL) {
                // Handle error - item might not have been added
                cJSON_Delete(root);
                return 0;
            }
            break;
        }
            
        case 1: {
            // Scenario 1: Test cJSON_AddStringToObject with control characters
            std::string key = fdp.ConsumeRandomLengthString(32);
            std::string value_with_control = create_string_with_control_chars(128);
            
            cJSON* string_item = cJSON_AddStringToObject(root, key.c_str(), value_with_control.c_str());
            if (string_item == NULL) {
                cJSON_Delete(root);
                return 0;
            }
            break;
        }
            
        case 2: {
            // Scenario 2: Test cJSON_PrintBuffered with NULL item
            // This should return NULL
            char* result = cJSON_PrintBuffered(NULL, 1024, fdp.ConsumeBool());
            if (result != NULL) {
                free(result); // cJSON_PrintBuffered allocates memory
            }
            break;
        }
            
        case 3: {
            // Scenario 3: Test cJSON_PrintBuffered with insufficient buffer size
            // Add some content to the object first
            std::string key = fdp.ConsumeRandomLengthString(16);
            std::string value = fdp.ConsumeRandomLengthString(64);
            cJSON_AddStringToObject(root, key.c_str(), value.c_str());
            
            // Test with very small buffer (should trigger reallocation or fail)
            int buffer_size = fdp.ConsumeIntegralInRange<int>(-10, 100);
            cJSON_bool format = fdp.ConsumeBool();
            
            char* printed = cJSON_PrintBuffered(root, buffer_size, format);
            if (printed != NULL) {
                free(printed);
            }
            break;
        }
            
        case 4: {
            // Scenario 4: Test cJSON_PrintPreallocated with NULL buffer
            // Add content to object
            std::string key = fdp.ConsumeRandomLengthString(16);
            std::string value = fdp.ConsumeRandomLengthString(64);
            cJSON_AddStringToObject(root, key.c_str(), value.c_str());
            
            // Test with NULL buffer (should return false)
            cJSON_bool success = cJSON_PrintPreallocated(root, NULL, 100, fdp.ConsumeBool());
            // success should be false for NULL buffer
            break;
        }
            
        case 5: {
            // Scenario 5: Test cJSON_PrintPreallocated with insufficient buffer
            // Add content to object
            std::string key = fdp.ConsumeRandomLengthString(16);
            std::string raw_json = create_raw_json();
            cJSON_AddRawToObject(root, key.c_str(), raw_json.c_str());
            
            // Allocate small buffer
            int buffer_size = fdp.ConsumeIntegralInRange<int>(1, 100);
            std::vector<char> buffer(buffer_size);
            cJSON_bool format = fdp.ConsumeBool();
            
            cJSON_bool success = cJSON_PrintPreallocated(root, buffer.data(), buffer_size, format);
            // success may be false if buffer is too small
            break;
        }
            
        case 6: {
            // Scenario 6: Test combination of raw strings and control characters
            // Add multiple items with different string types
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
                std::string key = fdp.ConsumeRandomLengthString(16);
                
                if (fdp.ConsumeBool()) {
                    // Add raw JSON
                    std::string raw_json = create_raw_json();
                    cJSON_AddRawToObject(root, key.c_str(), raw_json.c_str());
                } else {
                    // Add string with control characters
                    std::string value = create_string_with_control_chars(64);
                    cJSON_AddStringToObject(root, key.c_str(), value.c_str());
                }
            }
            break;
        }
            
        case 7: {
            // Scenario 7: Test printing with complex object containing various string types
            // Build a complex object with mixed content
            for (int i = 0; i < 4 && fdp.remaining_bytes() > 20; i++) {
                std::string key = fdp.ConsumeRandomLengthString(16);
                
                uint8_t item_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (item_type) {
                    case 0: {
                        std::string raw_json = create_raw_json();
                        cJSON_AddRawToObject(root, key.c_str(), raw_json.c_str());
                        break;
                    }
                    case 1: {
                        std::string value = create_string_with_control_chars(64);
                        cJSON_AddStringToObject(root, key.c_str(), value.c_str());
                        break;
                    }
                    case 2: {
                        double num = fdp.ConsumeFloatingPoint<double>();
                        cJSON_AddNumberToObject(root, key.c_str(), num);
                        break;
                    }
                    case 3: {
                        cJSON_AddBoolToObject(root, key.c_str(), fdp.ConsumeBool());
                        break;
                    }
                }
            }
            
            // Test both printing functions
            if (fdp.ConsumeBool()) {
                // Test cJSON_PrintBuffered
                int buffer_size = fdp.ConsumeIntegralInRange<int>(1, 4096);
                cJSON_bool format = fdp.ConsumeBool();
                char* printed = cJSON_PrintBuffered(root, buffer_size, format);
                if (printed != NULL) {
                    free(printed);
                }
            } else {
                // Test cJSON_PrintPreallocated
                int buffer_size = fdp.ConsumeIntegralInRange<int>(1, 4096);
                std::vector<char> buffer(buffer_size);
                cJSON_bool format = fdp.ConsumeBool();
                cJSON_bool success = cJSON_PrintPreallocated(root, buffer.data(), buffer_size, format);
            }
            break;
        }
    }
    
    // Clean up
    cJSON_Delete(root);
    
    return 0;
}

} // namespace harness_013
