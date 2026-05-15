#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "cjson/cJSON.h"

namespace harness_007 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful testing
    // We need enough data for operation selection and some malformed JSON
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;  // 8 different error scenarios
    bool use_null_input = fdp.ConsumeBool();
    bool use_zero_length = fdp.ConsumeBool();
    bool use_invalid_pointer = fdp.ConsumeBool();
    bool test_return_parse_end = fdp.ConsumeBool();
    
    // Consume malformed JSON data
    std::string malformed_json = fdp.ConsumeRandomLengthString(1024);
    std::string partial_json = fdp.ConsumeRandomLengthString(512);
    
    // Variables for testing different scenarios
    const char* json_input = NULL;
    size_t buffer_length = 0;
    const char** return_parse_end = NULL;
    cJSON_bool require_null_terminated = fdp.ConsumeBool();
    
    // Setup test scenarios based on consumed data
    if (use_null_input) {
        json_input = NULL;
    } else if (use_invalid_pointer) {
        // Create a pointer that's not NULL but invalid (e.g., pointer to middle of nowhere)
        // We'll use a stack variable address but ensure it's not a valid string
        static char invalid_buffer[1] = {0};
        json_input = invalid_buffer;
    } else {
        // Use actual JSON data
        if (malformed_json.empty()) {
            json_input = "";
        } else {
            json_input = malformed_json.c_str();
        }
    }
    
    if (use_zero_length) {
        buffer_length = 0;
    } else if (use_invalid_pointer) {
        // When using invalid pointer (1-byte buffer), limit buffer length to 1
        buffer_length = 1;
    } else {
        buffer_length = malformed_json.length();
    }
    
    if (test_return_parse_end) {
        // Create a persistent variable for the parse end pointer
        // The library will set this to point into the input string
        static const char* parse_end_value = NULL;
        return_parse_end = &parse_end_value;
    }
    
    // Use operation to test different parsing APIs with error conditions
    switch (operation) {
        case 0: {
            // Test cJSON_Parse with NULL/empty input
            cJSON* result = cJSON_Parse(json_input);
            if (result != NULL) {
                cJSON_Delete(result);
            }
            break;
        }
        
        case 1: {
            // Test cJSON_ParseWithLength with NULL/zero-length input
            cJSON* result = cJSON_ParseWithLength(json_input, buffer_length);
            if (result != NULL) {
                cJSON_Delete(result);
            }
            break;
        }
        
        case 2: {
            // Test cJSON_ParseWithOpts with NULL input and parse end
            cJSON* result = cJSON_ParseWithOpts(json_input, return_parse_end, require_null_terminated);
            if (result != NULL) {
                cJSON_Delete(result);
            }
            break;
        }
        
        case 3: {
            // Test cJSON_ParseWithLengthOpts with various invalid parameters
            cJSON* result = cJSON_ParseWithLengthOpts(json_input, buffer_length, return_parse_end, require_null_terminated);
            if (result != NULL) {
                cJSON_Delete(result);
            }
            break;
        }
        
        case 4: {
            // Test parse_number error path by parsing JSON with just number content
            // but using NULL buffer setup
            std::string number_json = "{" + std::to_string(fdp.ConsumeIntegral<int>()) + "}";
            cJSON* obj = cJSON_Parse(number_json.c_str());
            if (obj != NULL) {
                cJSON_Delete(obj);
            }
            break;
        }
        
        case 5: {
            // Test with extremely malformed JSON that might trigger allocation failures
            // Create JSON with deeply nested structure that might cause issues
            std::string deep_json;
            int depth = fdp.ConsumeIntegralInRange<int>(1, 100);
            for (int i = 0; i < depth; i++) {
                deep_json += "{" + std::to_string(i) + ":";
            }
            deep_json += "null";
            for (int i = 0; i < depth; i++) {
                deep_json += "}";
            }
            
            cJSON* result = cJSON_Parse(deep_json.c_str());
            if (result != NULL) {
                cJSON_Delete(result);
            }
            break;
        }
        
        case 6: {
            // Test with JSON containing invalid number formats
            std::vector<std::string> invalid_numbers = {
                "123.e",
                "123..456",
                "+-123",
                "123e",
                "123e+",
                "123e-",
                ".123",
                "123.",
                "1.2.3",
                "inf",
                "NaN",
                "1e999",  // Potential overflow
                "-1e999"  // Potential underflow
            };
            
            size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, invalid_numbers.size() - 1);
            std::string invalid_json = "{\"num\": " + invalid_numbers[idx] + "}";
            
            cJSON* result = cJSON_Parse(invalid_json.c_str());
            if (result != NULL) {
                cJSON_Delete(result);
            }
            break;
        }
        
        case 7: {
            // Test memory allocation failure simulation by parsing very large JSON
            // that might trigger allocation limits
            size_t large_size = fdp.ConsumeIntegralInRange<size_t>(1024, 65536);
            std::string large_json(large_size, '[');
            large_json += "null";
            large_json.append(large_size, ']');
            
            cJSON* result = cJSON_ParseWithLength(large_json.c_str(), large_json.length());
            if (result != NULL) {
                cJSON_Delete(result);
            }
            break;
        }
    }
    return 0;
}

} // namespace harness_007
