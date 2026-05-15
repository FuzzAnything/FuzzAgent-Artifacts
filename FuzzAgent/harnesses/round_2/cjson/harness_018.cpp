#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_018 {

extern "C" {
}

// Helper to create strings with control characters (0-31) using fuzzer input
std::string create_string_with_control_chars(FuzzedDataProvider& fdp) {
    std::string result;
    
    // Determine string length
    size_t length = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    
    for (size_t i = 0; i < length && fdp.remaining_bytes() > 0; i++) {
        // Use fuzzer input to decide character type
        uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (char_type) {
            case 0: // Control character (0-31)
                result.push_back(static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(0, 31)));
                break;
            case 1: // Special escape character
                {
                    uint8_t esc_type = fdp.ConsumeIntegral<uint8_t>() % 6;
                    switch (esc_type) {
                        case 0: result.push_back('\\'); break;
                        case 1: result.push_back('\"'); break;
                        case 2: result.push_back('\b'); break;
                        case 3: result.push_back('\f'); break;
                        case 4: result.push_back('\n'); break;
                        case 5: result.push_back('\r'); break;
                        case 6: result.push_back('\t'); break;
                    }
                }
                break;
            case 2: // Regular character
                result.push_back(fdp.ConsumeIntegralInRange<char>(32, 126));
                break;
            case 3: // Potential Unicode character
                // Simple surrogate pair simulation using UTF-8 encoding
                if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 2) {
                    // Create a simple multi-byte UTF-8 character
                    result.push_back(static_cast<char>(0xC0 | (fdp.ConsumeIntegral<uint8_t>() & 0x1F)));
                    result.push_back(static_cast<char>(0x80 | (fdp.ConsumeIntegral<uint8_t>() & 0x3F)));
                } else {
                    result.push_back(fdp.ConsumeIntegralInRange<char>(32, 126));
                }
                break;
        }
    }
    
    return result;
}

// Helper to create number strings with various formats
std::string create_number_string(FuzzedDataProvider& fdp) {
    std::string result;
    
    uint8_t num_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (num_type) {
        case 0: // Regular integer
            result = std::to_string(fdp.ConsumeIntegral<int64_t>());
            break;
        case 1: // Regular floating point
            result = std::to_string(fdp.ConsumeFloatingPoint<double>());
            break;
        case 2: // Scientific notation
            result = std::to_string(fdp.ConsumeFloatingPoint<double>()) + 
                     (fdp.ConsumeBool() ? "e" : "E") +
                     (fdp.ConsumeBool() ? "+" : "-") +
                     std::to_string(fdp.ConsumeIntegralInRange<int>(0, 308));
            break;
        case 3: // Multiple decimal points (invalid)
            result = std::to_string(fdp.ConsumeIntegral<int>()) + ".1.2.3";
            break;
        case 4: // Leading/trailing zeros
            result = std::string(fdp.ConsumeIntegralInRange<int>(0, 10), '0') +
                     std::to_string(fdp.ConsumeIntegral<int>()) +
                     "." +
                     std::to_string(fdp.ConsumeIntegral<int>()) +
                     std::string(fdp.ConsumeIntegralInRange<int>(0, 10), '0');
            break;
        case 5: // Extreme values
            if (fdp.ConsumeBool()) {
                // Very large positive
                result = "1" + std::string(fdp.ConsumeIntegralInRange<int>(100, 300), '0');
            } else {
                // Very small negative
                result = "-0." + std::string(fdp.ConsumeIntegralInRange<int>(100, 300), '0') + "1";
            }
            break;
        case 6: // NaN/Infinity variations
            {
                uint8_t special_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (special_type) {
                    case 0: result = "NaN"; break;
                    case 1: result = "Infinity"; break;
                    case 2: result = "-Infinity"; break;
                    case 3: result = "+Infinity"; break;
                }
            }
            break;
        case 7: // Invalid number formats
            {
                uint8_t invalid_type = fdp.ConsumeIntegral<uint8_t>() % 6;
                switch (invalid_type) {
                    case 0: result = "123abc"; break;
                    case 1: result = "1.2.3"; break;
                    case 2: result = "++123"; break;
                    case 3: result = "--456"; break;
                    case 4: result = "12e"; break;
                    case 5: result = "e12"; break;
                }
            }
            break;
    }
    
    return result;
}

// Helper to create JSON with complex strings/numbers for round-trip testing
std::string create_complex_json(FuzzedDataProvider& fdp) {
    std::string json = "{";
    
    // Add string items with control characters
    size_t string_count = fdp.ConsumeIntegralInRange<size_t>(0, 5);
    for (size_t i = 0; i < string_count && fdp.remaining_bytes() > 0; i++) {
        std::string key = "\"str" + std::to_string(i) + "\"";
        std::string value = create_string_with_control_chars(fdp);
        
        // Escape quotes in value for JSON
        std::string escaped_value;
        for (char c : value) {
            if (c == '\"') escaped_value += "\\\"";
            else if (c == '\\') escaped_value += "\\\\";
            else escaped_value.push_back(c);
        }
        
        json += (i > 0 ? "," : "") + key + ":\"" + escaped_value + "\"";
    }
    
    // Add number items with various formats
    size_t number_count = fdp.ConsumeIntegralInRange<size_t>(0, 5);
    for (size_t i = 0; i < number_count && fdp.remaining_bytes() > 0; i++) {
        std::string key = "\"num" + std::to_string(i) + "\"";
        std::string value = create_number_string(fdp);
        
        json += (string_count > 0 || i > 0 ? "," : "") + key + ":" + value;
    }
    
    json += "}";
    return json;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;  // Minimum input size
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Part 1: String Printing Deep Dive
    {
        // Create strings with various characteristics
        uint8_t string_test_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        
        cJSON* string_item = nullptr;
        switch (string_test_type) {
            case 0: // String with control characters
                {
                    std::string test_str = create_string_with_control_chars(fdp);
                    string_item = cJSON_CreateString(test_str.c_str());
                }
                break;
            case 1: // NULL string
                string_item = cJSON_CreateString(nullptr);
                break;
            case 2: // Empty string
                string_item = cJSON_CreateString("");
                break;
            case 3: // Raw JSON string
                {
                    std::string raw_str = fdp.ConsumeRandomLengthString(256);
                    string_item = cJSON_CreateRaw(raw_str.c_str());
                }
                break;
            case 4: // Regular string
                string_item = cJSON_CreateString(fdp.ConsumeRandomLengthString(256).c_str());
                break;
        }
        
        if (string_item != nullptr) {
            // Test different printing methods with varying buffer sizes
            uint8_t print_method = fdp.ConsumeIntegral<uint8_t>() % 4;
            
            switch (print_method) {
                case 0: // cJSON_PrintBuffered with small buffer
                    {
                        int buffer_size = fdp.ConsumeIntegralInRange<int>(1, 64);
                        char* printed = cJSON_PrintBuffered(string_item, buffer_size, fdp.ConsumeBool());
                        if (printed != nullptr) {
                            free(printed);
                        }
                    }
                    break;
                case 1: // cJSON_PrintBuffered with medium buffer
                    {
                        int buffer_size = fdp.ConsumeIntegralInRange<int>(65, 1024);
                        char* printed = cJSON_PrintBuffered(string_item, buffer_size, fdp.ConsumeBool());
                        if (printed != nullptr) {
                            free(printed);
                        }
                    }
                    break;
                case 2: // cJSON_PrintPreallocated with small buffer
                    {
                        size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                        char* buffer = (char*)malloc(buffer_size);
                        if (buffer != nullptr) {
                            cJSON_PrintPreallocated(string_item, buffer, buffer_size, fdp.ConsumeBool());
                            free(buffer);
                        }
                    }
                    break;
                case 3: // Standard print
                    {
                        char* printed = cJSON_Print(string_item);
                        if (printed != nullptr) {
                            free(printed);
                        }
                    }
                    break;
            }
            
            cJSON_Delete(string_item);
        }
    }
    
    // Part 2: Number Parsing Edge Cases
    if (fdp.remaining_bytes() > 0) {
        // Create JSON with number strings
        std::string number_json = "{\"number\":" + create_number_string(fdp) + "}";
        
        cJSON* parsed_number = cJSON_Parse(number_json.c_str());
        if (parsed_number != nullptr) {
            // Get and print the number
            cJSON* number_item = cJSON_GetObjectItem(parsed_number, "number");
            if (number_item != nullptr && cJSON_IsNumber(number_item)) {
                char* printed = cJSON_Print(number_item);
                if (printed != nullptr) {
                    free(printed);
                }
            }
            
            cJSON_Delete(parsed_number);
        }
    }
    
    // Part 3: Round-trip Validation
    if (fdp.remaining_bytes() > 0) {
        std::string complex_json = create_complex_json(fdp);
        
        // Parse the JSON
        cJSON* parsed = cJSON_Parse(complex_json.c_str());
        if (parsed != nullptr) {
            // Print it back
            char* printed = cJSON_Print(parsed);
            if (printed != nullptr) {
                // Optionally parse the printed version again
                if (fdp.ConsumeBool()) {
                    cJSON* reparsed = cJSON_Parse(printed);
                    if (reparsed != nullptr) {
                        cJSON_Delete(reparsed);
                    }
                }
                free(printed);
            }
            
            cJSON_Delete(parsed);
        }
    }
    
    // Part 4: Buffer Stress Testing
    if (fdp.remaining_bytes() > 0) {
        // Create a simple object
        cJSON* test_obj = cJSON_CreateObject();
        if (test_obj != nullptr) {
            // Add some items
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                std::string key = fdp.ConsumeRandomLengthString(16);
                std::string value = create_string_with_control_chars(fdp);
                cJSON_AddStringToObject(test_obj, key.c_str(), value.c_str());
            }
            
            // Test with intentionally small buffers
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                char* buffer = (char*)malloc(buffer_size);
                if (buffer != nullptr) {
                    // Test both formatted and unformatted
                    cJSON_PrintPreallocated(test_obj, buffer, buffer_size, fdp.ConsumeBool());
                    free(buffer);
                }
            }
            
            cJSON_Delete(test_obj);
        }
    }
    
    return 0;
}

} // namespace harness_018
