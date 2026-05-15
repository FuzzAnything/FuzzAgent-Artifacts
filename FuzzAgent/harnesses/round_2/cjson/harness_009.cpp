#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <functional>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_009 {

extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 128) {
        return 0;  // Need sufficient input for comprehensive testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Part 1: Test parse_number with edge cases (targeting 48 blocked branches)
    // Generate number strings from fuzzer input to test parse_number deeply
    int number_test_count = fdp.ConsumeIntegralInRange<int>(1, 10);
    std::vector<std::string> number_strings;
    
    for (int i = 0; i < number_test_count && fdp.remaining_bytes() > 0; i++) {
        // Generate different types of number representations from fuzzer input
        int num_type = fdp.ConsumeIntegralInRange<int>(0, 6);
        std::string num_str;
        
        switch (num_type) {
            case 0:  // Integer
                num_str = std::to_string(fdp.ConsumeIntegral<int64_t>());
                break;
            case 1:  // Floating point
                num_str = std::to_string(fdp.ConsumeFloatingPoint<double>());
                break;
            case 2:  // Scientific notation
                {
                    double base = fdp.ConsumeFloatingPoint<double>();
                    int exponent = fdp.ConsumeIntegralInRange<int>(-308, 308);
                    num_str = std::to_string(base) + "e" + (exponent >= 0 ? "+" : "") + std::to_string(exponent);
                }
                break;
            case 3:  // Decimal with many digits
                {
                    std::string int_part = std::to_string(fdp.ConsumeIntegral<uint64_t>());
                    std::string frac_part = std::to_string(fdp.ConsumeIntegral<uint64_t>());
                    // Limit fractional part length
                    if (frac_part.length() > 20) frac_part = frac_part.substr(0, 20);
                    num_str = int_part + "." + frac_part;
                }
                break;
            case 4:  // Negative number
                num_str = "-" + std::to_string(fdp.ConsumeIntegral<uint64_t>());
                break;
            case 5:  // Zero variants
                num_str = fdp.ConsumeBool() ? "0" : "0.0";
                break;
            case 6:  // Random string that might be parseable as number
                num_str = fdp.ConsumeRandomLengthString(30);
                // Remove characters that would definitely break JSON number parsing
                size_t pos = 0;
                while ((pos = num_str.find_first_of("[]{}\":, \t\n\r", pos)) != std::string::npos) {
                    num_str[pos] = '0';
                }
                break;
        }
        number_strings.push_back(num_str);
    }
    
    // Create JSON strings containing these numbers for parsing
    if (!number_strings.empty()) {
        std::string json_with_numbers = "{\"numbers\": [";
        for (size_t i = 0; i < number_strings.size(); i++) {
            if (i > 0) json_with_numbers += ", ";
            json_with_numbers += number_strings[i];
        }
        json_with_numbers += "]}";
        
        cJSON* parsed_with_numbers = cJSON_Parse(json_with_numbers.c_str());
        if (parsed_with_numbers != NULL) {
            cJSON_Delete(parsed_with_numbers);
        }
    }
    
    // Part 2: Test parse_string with escape sequences, Unicode, special characters (43 blocked branches)
    // Generate strings with potential escape sequences from fuzzer input
    int string_test_count = fdp.ConsumeIntegralInRange<int>(1, 5);
    std::vector<std::string> test_strings;
    
    for (int i = 0; i < string_test_count && fdp.remaining_bytes() > 0; i++) {
        std::string base_str = fdp.ConsumeRandomLengthString(100);
        std::string escaped_str;
        
        // Process string to potentially add escape sequences
        for (char c : base_str) {
            if (fdp.ConsumeProbability<double>() < 0.1) {  // 10% chance to escape a character
                switch (fdp.ConsumeIntegralInRange<int>(0, 7)) {
                    case 0: escaped_str += "\\n"; break;
                    case 1: escaped_str += "\\t"; break;
                    case 2: escaped_str += "\\\""; break;
                    case 3: escaped_str += "\\\\"; break;
                    case 4: escaped_str += "\\/"; break;
                    case 5: escaped_str += "\\b"; break;
                    case 6: escaped_str += "\\f"; break;
                    case 7: escaped_str += "\\r"; break;
                }
            } else if (fdp.ConsumeProbability<double>() < 0.05) {  // 5% chance for unicode escape
                uint16_t codepoint = fdp.ConsumeIntegral<uint16_t>();
                char hex[7];
                snprintf(hex, sizeof(hex), "\\u%04X", codepoint);
                escaped_str += hex;
            } else {
                escaped_str += c;
            }
        }
        test_strings.push_back(escaped_str);
    }
    
    // Create JSON with these strings
    if (!test_strings.empty()) {
        std::string json_with_strings = "{\"strings\": [";
        for (size_t i = 0; i < test_strings.size(); i++) {
            if (i > 0) json_with_strings += ", ";
            json_with_strings += "\"" + test_strings[i] + "\"";
        }
        json_with_strings += "]}";
        
        cJSON* parsed_with_strings = cJSON_Parse(json_with_strings.c_str());
        if (parsed_with_strings != NULL) {
            cJSON_Delete(parsed_with_strings);
        }
    }
    // Part 3: Test parse_value with various JSON types and nesting (41 blocked branches)
    // Generate complex JSON structures from fuzzer input using iterative approach
    std::string complex_json;
    int depth = 0;
    const int max_depth = 3;
    
    // Simple iterative generation without recursion
    int structure_type = fdp.ConsumeIntegralInRange<int>(0, 5);
    
    switch (structure_type) {
        case 0:  // Simple array
            {
                int array_size = fdp.ConsumeIntegralInRange<int>(0, 10);
                complex_json = "[";
                for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                    if (i > 0) complex_json += ", ";
                    int elem_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                    switch (elem_type) {
                        case 0:
                            complex_json += std::to_string(fdp.ConsumeFloatingPoint<double>());
                            break;
                        case 1:
                            complex_json += "\"" + fdp.ConsumeRandomLengthString(30) + "\"";
                            break;
                        case 2:
                            complex_json += fdp.ConsumeBool() ? "true" : "false";
                            break;
                        case 3:
                            complex_json += "null";
                            break;
                    }
                }
                complex_json += "]";
            }
            break;
        case 1:  // Simple object
            {
                int obj_size = fdp.ConsumeIntegralInRange<int>(0, 5);
                complex_json = "{";
                for (int i = 0; i < obj_size && fdp.remaining_bytes() > 0; i++) {
                    if (i > 0) complex_json += ", ";
                    std::string key = fdp.ConsumeRandomLengthString(20);
                    complex_json += "\"" + key + "\": ";
                    int value_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                    switch (value_type) {
                        case 0:
                            complex_json += std::to_string(fdp.ConsumeFloatingPoint<double>());
                            break;
                        case 1:
                            complex_json += "\"" + fdp.ConsumeRandomLengthString(30) + "\"";
                            break;
                        case 2:
                            complex_json += fdp.ConsumeBool() ? "true" : "false";
                            break;
                        case 3:
                            complex_json += "null";
                            break;
                    }
                }
                complex_json += "}";
            }
            break;
        case 2:  // Nested array
            {
                int outer_size = fdp.ConsumeIntegralInRange<int>(0, 3);
                complex_json = "[";
                for (int i = 0; i < outer_size && fdp.remaining_bytes() > 0; i++) {
                    if (i > 0) complex_json += ", ";
                    int inner_size = fdp.ConsumeIntegralInRange<int>(0, 3);
                    complex_json += "[";
                    for (int j = 0; j < inner_size && fdp.remaining_bytes() > 0; j++) {
                        if (j > 0) complex_json += ", ";
                        complex_json += std::to_string(fdp.ConsumeFloatingPoint<double>());
                    }
                    complex_json += "]";
                }
                complex_json += "]";
            }
            break;
        case 3:  // Nested object
            {
                complex_json = "{\"outer\": {\"inner\": ";
                complex_json += std::to_string(fdp.ConsumeFloatingPoint<double>());
                complex_json += "}}";
            }
            break;
        case 4:  // Mixed structure
            {
                complex_json = "{\"array\": [";
                int array_size = fdp.ConsumeIntegralInRange<int>(0, 3);
                for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                    if (i > 0) complex_json += ", ";
                    complex_json += fdp.ConsumeBool() ? "true" : "false";
                }
                complex_json += "], \"number\": ";
                complex_json += std::to_string(fdp.ConsumeFloatingPoint<double>());
                complex_json += ", \"string\": \"";
                complex_json += fdp.ConsumeRandomLengthString(40);
                complex_json += "\"}";
            }
            break;
        case 5:  // Raw fuzzer input as JSON
            complex_json = fdp.ConsumeRandomLengthString(200);
            break;
    }
    
    cJSON* parsed_complex = cJSON_Parse(complex_json.c_str());
    
    
    // Part 4: Test print_string_ptr with edge cases (45 blocked branches)
    if (parsed_complex != NULL) {
        // Test various printing functions
        char* printed_unformatted = cJSON_PrintUnformatted(parsed_complex);
        if (printed_unformatted != NULL) {
            // Test printing with different buffer sizes from fuzzer input
            int buffer_size = fdp.ConsumeIntegralInRange<int>(1, 4096);
            bool format = fdp.ConsumeBool();
            char* printed_buffered = cJSON_PrintBuffered(parsed_complex, buffer_size, format);
            if (printed_buffered != NULL) {
                cJSON_free(printed_buffered);
            }
            
            // Test print_string_ptr indirectly by creating string items and printing them
            cJSON* string_item = cJSON_CreateString(printed_unformatted);
            if (string_item != NULL) {
                char* string_printed = cJSON_Print(string_item);
                if (string_printed != NULL) {
                    cJSON_free(string_printed);
                }
                cJSON_Delete(string_item);
            }
            
            cJSON_free(printed_unformatted);
        }
    }
    
    // Part 5: Test cJSON_HasObjectItem with various keys
    cJSON* test_object = cJSON_CreateObject();
    if (test_object != NULL) {
        // Add some items with random keys from fuzzer input
        int num_items = fdp.ConsumeIntegralInRange<int>(1, 10);
        std::vector<std::string> keys_added;
        
        for (int i = 0; i < num_items && fdp.remaining_bytes() > 0; i++) {
            std::string key = fdp.ConsumeRandomLengthString(20);
            keys_added.push_back(key);
            
            // Add different types of values
            int value_type = fdp.ConsumeIntegralInRange<int>(0, 4);
            switch (value_type) {
                case 0:
                    cJSON_AddNumberToObject(test_object, key.c_str(), fdp.ConsumeFloatingPoint<double>());
                    break;
                case 1:
                    cJSON_AddStringToObject(test_object, key.c_str(), fdp.ConsumeRandomLengthString(50).c_str());
                    break;
                case 2:
                    cJSON_AddBoolToObject(test_object, key.c_str(), fdp.ConsumeBool());
                    break;
                case 3:
                    cJSON_AddNullToObject(test_object, key.c_str());
                    break;
                case 4:
                    {
                        cJSON* array = cJSON_CreateArray();
                        if (array != NULL) {
                            int array_size = fdp.ConsumeIntegralInRange<int>(0, 3);
                            for (int j = 0; j < array_size && fdp.remaining_bytes() > 0; j++) {
                                cJSON_AddItemToArray(array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                            }
                            cJSON_AddItemToObject(test_object, key.c_str(), array);
                        }
                    }
                    break;
            }
        }
        
        // Test cJSON_HasObjectItem on keys that exist
        for (const auto& key : keys_added) {
            cJSON_bool has_item = cJSON_HasObjectItem(test_object, key.c_str());
            // has_item should be 1 (true) for existing keys
        }
        
        // Test cJSON_HasObjectItem on keys that don't exist
        int num_missing_tests = fdp.ConsumeIntegralInRange<int>(1, 5);
        for (int i = 0; i < num_missing_tests && fdp.remaining_bytes() > 0; i++) {
            std::string missing_key = fdp.ConsumeRandomLengthString(25);
            cJSON_bool has_item = cJSON_HasObjectItem(test_object, missing_key.c_str());
            // has_item should be 0 (false) for non-existing keys
        }
        
        // Test cJSON_HasObjectItem with empty string key
        cJSON_bool has_empty = cJSON_HasObjectItem(test_object, "");
        
        // Test cJSON_HasObjectItem with NULL object
        if (fdp.ConsumeBool()) {
            cJSON_bool has_null_obj = cJSON_HasObjectItem(NULL, "test");
        }
        
        cJSON_Delete(test_object);
    }
    
    // Part 6: Call cJSON_Version and cJSON_GetErrorPtr
    // Get library version
    const char* version = cJSON_Version();
    // version should not be NULL
    
    // Test cJSON_GetErrorPtr by triggering parsing errors with malformed JSON from fuzzer input
    int num_error_tests = fdp.ConsumeIntegralInRange<int>(1, 10);
    for (int i = 0; i < num_error_tests && fdp.remaining_bytes() > 0; i++) {
        // Generate potentially malformed JSON
        std::string malformed_json;
        int malform_type = fdp.ConsumeIntegralInRange<int>(0, 6);
        
        switch (malform_type) {
            case 0:  // Unclosed object
                malformed_json = "{";
                break;
            case 1:  // Unclosed array
                malformed_json = "[";
                break;
            case 2:  // Missing value
                malformed_json = "{\"key\": }";
                break;
            case 3:  // Trailing comma
                malformed_json = "[1, 2, 3,]";
                break;
            case 4:  // Unclosed string
                malformed_json = "\"unclosed string";
                break;
            case 5:  // Invalid characters
                malformed_json = fdp.ConsumeRandomLengthString(50);
                break;
            case 6:  // Nested unclosed
                malformed_json = "{\"outer\": {";
                break;
        }
        
        // Mix with fuzzer data
        if (fdp.ConsumeBool()) {
            malformed_json += fdp.ConsumeRandomLengthString(30);
        }
        
        cJSON* failed_parse = cJSON_Parse(malformed_json.c_str());
        if (failed_parse == NULL) {
            const char* error_ptr = cJSON_GetErrorPtr();
            // error_ptr may be NULL or point to error location
        } else {
            cJSON_Delete(failed_parse);
        }
    }
    
    // Part 7: Exercise cJSON_GetArrayItem with boundary indices
    cJSON* test_array = cJSON_CreateArray();
    if (test_array != NULL) {
        // Add some items to the array from fuzzer input
        int array_size = fdp.ConsumeIntegralInRange<int>(0, 20);
        for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
            int item_type = fdp.ConsumeIntegralInRange<int>(0, 3);
            switch (item_type) {
                case 0:
                    cJSON_AddItemToArray(test_array, cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>()));
                    break;
                case 1:
                    cJSON_AddItemToArray(test_array, cJSON_CreateString(fdp.ConsumeRandomLengthString(30).c_str()));
                    break;
                case 2:
                    cJSON_AddItemToArray(test_array, cJSON_CreateBool(fdp.ConsumeBool()));
                    break;
                case 3:
                    cJSON_AddItemToArray(test_array, cJSON_CreateNull());
                    break;
            }
        }
        
        // Test cJSON_GetArrayItem with valid indices
        for (int i = 0; i < array_size; i++) {
            cJSON* item = cJSON_GetArrayItem(test_array, i);
            // item should not be NULL for valid indices
        }
        
        // Test cJSON_GetArrayItem with boundary and invalid indices
        // Negative index (should return NULL)
        cJSON* neg_item = cJSON_GetArrayItem(test_array, -1);
        
        // Index equal to array size (should return NULL)
        cJSON* size_item = cJSON_GetArrayItem(test_array, array_size);
        
        // Large positive index (should return NULL)
        cJSON* large_item = cJSON_GetArrayItem(test_array, 1000);
        
        // Test with random indices from fuzzer input
        int num_random_indices = fdp.ConsumeIntegralInRange<int>(1, 5);
        for (int i = 0; i < num_random_indices && fdp.remaining_bytes() > 0; i++) {
            int random_index = fdp.ConsumeIntegral<int>();
            cJSON* random_item = cJSON_GetArrayItem(test_array, random_index);
        }
        
        // Test with NULL array
        if (fdp.ConsumeBool()) {
            cJSON* null_array_item = cJSON_GetArrayItem(NULL, 0);
        }
        
        cJSON_Delete(test_array);
    }
    
    // Clean up
    if (parsed_complex != NULL) {
        cJSON_Delete(parsed_complex);
    }
    
    return 0;
}

} // namespace harness_009
