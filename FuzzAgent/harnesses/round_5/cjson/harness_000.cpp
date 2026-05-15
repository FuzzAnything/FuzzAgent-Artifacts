#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include "cjson/cJSON.h"

namespace harness_000 {
/*
 * Harness for cJSON library - bootstrap harness targeting core JSON parsing,
 * creation, and manipulation APIs.
 * 
 * This harness exercises the main entry points of cJSON library:
 * - JSON parsing (cJSON_Parse, cJSON_ParseWithOpts)
 * - JSON creation (cJSON_CreateObject, cJSON_CreateArray, etc.)
 * - JSON manipulation (cJSON_AddItemToObject, cJSON_AddItemToArray, etc.)
 * - JSON query (cJSON_GetObjectItem, cJSON_GetArrayItem, etc.)
 * - JSON printing (cJSON_Print, cJSON_PrintUnformatted, etc.)
 * - JSON cleanup (cJSON_Delete)
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Part 1: Parse JSON from fuzzer input
    // ------------------------------------
    std::string json_input = fdp.ConsumeRemainingBytesAsString();
    
    // Try parsing with different options
    cJSON* parsed_json1 = cJSON_Parse(json_input.c_str());
    cJSON* parsed_json2 = NULL;
    const char* parse_end = NULL;
    cJSON_bool require_null_terminated = fdp.ConsumeBool();
    
    if (fdp.remaining_bytes() > 0) {
        // Reset fdp for more operations
        FuzzedDataProvider fdp2(data, size);
        std::string json_input2 = fdp2.ConsumeRemainingBytesAsString();
        parsed_json2 = cJSON_ParseWithOpts(json_input2.c_str(), &parse_end, require_null_terminated);
    }

    // Clean up parsed JSON objects
    if (parsed_json1 != NULL) {
        cJSON_Delete(parsed_json1);
    }
    if (parsed_json2 != NULL) {
        cJSON_Delete(parsed_json2);
    }

    // Reset fdp for creation operations
    FuzzedDataProvider fdp3(data, size);

    // Part 2: Create and manipulate JSON objects
    // ------------------------------------------
    
    // Create basic JSON items
    cJSON* null_item = cJSON_CreateNull();
    cJSON* true_item = cJSON_CreateTrue();
    cJSON* false_item = cJSON_CreateFalse();
    
    // Create boolean with random value
    cJSON_bool random_bool = fdp3.ConsumeBool();
    cJSON* bool_item = cJSON_CreateBool(random_bool);
    
    // Create number with random value
    double random_number = fdp3.ConsumeFloatingPoint<double>();
    cJSON* number_item = cJSON_CreateNumber(random_number);
    
    // Create string with random content
    std::string random_string = fdp3.ConsumeRandomLengthString(256);
    cJSON* string_item = cJSON_CreateString(random_string.c_str());
    
    // Create raw JSON string
    std::string random_raw = fdp3.ConsumeRandomLengthString(128);
    cJSON* raw_item = cJSON_CreateRaw(random_raw.c_str());
    
    // Create object and array
    cJSON* object_item = cJSON_CreateObject();
    cJSON* array_item = cJSON_CreateArray();

    // Part 3: Add items to object
    // ----------------------------
    if (object_item != NULL) {
        // Generate random keys
        std::string key1 = fdp3.ConsumeRandomLengthString(32);
        std::string key2 = fdp3.ConsumeRandomLengthString(32);
        std::string key3 = fdp3.ConsumeRandomLengthString(32);
        std::string key4 = fdp3.ConsumeRandomLengthString(32);
        std::string key5 = fdp3.ConsumeRandomLengthString(32);
        
        // Add various items to object
        if (null_item != NULL) {
            cJSON_AddItemToObject(object_item, key1.c_str(), null_item);
        }
        if (true_item != NULL) {
            cJSON_AddItemToObject(object_item, key2.c_str(), true_item);
        }
        if (false_item != NULL) {
            cJSON_AddItemToObject(object_item, key3.c_str(), false_item);
        }
        if (number_item != NULL) {
            cJSON_AddItemToObject(object_item, key4.c_str(), number_item);
        }
        if (string_item != NULL) {
            cJSON_AddItemToObject(object_item, key5.c_str(), string_item);
        }
        
        // Use helper functions to add items
        std::string key6 = fdp3.ConsumeRandomLengthString(32);
        std::string key7 = fdp3.ConsumeRandomLengthString(32);
        cJSON_AddNullToObject(object_item, key6.c_str());
        cJSON_AddTrueToObject(object_item, key7.c_str());
        
        // Query object items
        cJSON* queried_item = cJSON_GetObjectItem(object_item, key1.c_str());
        cJSON* queried_item_case = cJSON_GetObjectItemCaseSensitive(object_item, key2.c_str());
        cJSON_bool has_item = cJSON_HasObjectItem(object_item, key3.c_str());
        
        // Test item type checking
        if (queried_item != NULL) {
            cJSON_bool is_null = cJSON_IsNull(queried_item);
            cJSON_bool is_bool = cJSON_IsBool(queried_item);
            cJSON_bool is_number = cJSON_IsNumber(queried_item);
            cJSON_bool is_string = cJSON_IsString(queried_item);
            cJSON_bool is_array = cJSON_IsArray(queried_item);
            cJSON_bool is_object = cJSON_IsObject(queried_item);
            cJSON_bool is_raw = cJSON_IsRaw(queried_item);
        }
    }

    // Part 4: Manipulate array
    // -------------------------
    if (array_item != NULL) {
        // Add items to array
        if (bool_item != NULL) {
            cJSON_AddItemToArray(array_item, bool_item);
        }
        if (raw_item != NULL) {
            cJSON_AddItemToArray(array_item, raw_item);
        }
        
        // Create and add more items to array
        std::string array_string = fdp3.ConsumeRandomLengthString(64);
        cJSON* array_string_item = cJSON_CreateString(array_string.c_str());
        if (array_string_item != NULL) {
            cJSON_AddItemToArray(array_item, array_string_item);
        }
        
        // Get array size
        int array_size = cJSON_GetArraySize(array_item);
        
        // Access array items
        for (int i = 0; i < array_size && i < 10; i++) {
            cJSON* array_element = cJSON_GetArrayItem(array_item, i);
            if (array_element != NULL) {
                // Test value extraction
                char* str_value = cJSON_GetStringValue(array_element);
                double num_value = cJSON_GetNumberValue(array_element);
            }
        }
        
        // Create string array
        std::vector<std::string> strings;
        for (int i = 0; i < 5 && fdp3.remaining_bytes() > 0; i++) {
            strings.push_back(fdp3.ConsumeRandomLengthString(32));
        }
        std::vector<const char*> cstrings;
        for (const auto& s : strings) {
            cstrings.push_back(s.c_str());
        }
        cJSON* string_array = cJSON_CreateStringArray(cstrings.data(), cstrings.size());
        if (string_array != NULL) {
            cJSON_Delete(string_array);
        }
    }

    // Part 5: Print JSON and test printing functions
    // ----------------------------------------------
    if (object_item != NULL) {
        // Test different printing functions
        char* printed_formatted = cJSON_Print(object_item);
        char* printed_unformatted = cJSON_PrintUnformatted(object_item);
        
        // Test buffered printing with random buffer size
        int prebuffer_size = fdp3.ConsumeIntegralInRange<int>(1, 1024);
        cJSON_bool format_option = fdp3.ConsumeBool();
        char* printed_buffered = cJSON_PrintBuffered(object_item, prebuffer_size, format_option);
        
        // Clean up printed strings
        if (printed_formatted != NULL) {
            free(printed_formatted);
        }
        if (printed_unformatted != NULL) {
            free(printed_unformatted);
        }
        if (printed_buffered != NULL) {
            free(printed_buffered);
        }
    }
    
    if (array_item != NULL) {
        char* array_printed = cJSON_Print(array_item);
        if (array_printed != NULL) {
            free(array_printed);
        }
    }

    // Part 6: Test duplicate and compare functions
    // --------------------------------------------
    if (object_item != NULL && array_item != NULL) {
        // Duplicate objects
        cJSON* duplicated_object = cJSON_Duplicate(object_item, fdp3.ConsumeBool());
        cJSON* duplicated_array = cJSON_Duplicate(array_item, fdp3.ConsumeBool());
        
        // Compare objects
        cJSON_bool objects_equal = cJSON_Compare(object_item, duplicated_object, fdp3.ConsumeBool());
        cJSON_bool arrays_equal = cJSON_Compare(array_item, duplicated_array, fdp3.ConsumeBool());
        
        // Clean up duplicates
        if (duplicated_object != NULL) {
            cJSON_Delete(duplicated_object);
        }
        if (duplicated_array != NULL) {
            cJSON_Delete(duplicated_array);
        }
    }

    // Part 7: Test minify function
    // -----------------------------
    if (fdp3.remaining_bytes() > 10) {
        std::string json_to_minify = fdp3.ConsumeRemainingBytesAsString();
        // Minify requires writable buffer, so copy the string
        char* minify_buffer = (char*)malloc(json_to_minify.size() + 1);
        if (minify_buffer != NULL) {
            memcpy(minify_buffer, json_to_minify.c_str(), json_to_minify.size() + 1);
            cJSON_Minify(minify_buffer);
            free(minify_buffer);
        }
    }

    // Part 8: Clean up all created items
    // -----------------------------------
    // Note: Items added to object/array are automatically deleted when parent is deleted
    if (object_item != NULL) {
        cJSON_Delete(object_item);
    } else {
        // If object creation failed, clean up individual items
        if (null_item != NULL) cJSON_Delete(null_item);
        if (true_item != NULL) cJSON_Delete(true_item);
        if (false_item != NULL) cJSON_Delete(false_item);
        if (bool_item != NULL) cJSON_Delete(bool_item);
        if (number_item != NULL) cJSON_Delete(number_item);
        if (string_item != NULL) cJSON_Delete(string_item);
        if (raw_item != NULL) cJSON_Delete(raw_item);
    }
    
    if (array_item != NULL) {
        cJSON_Delete(array_item);
    }

    return 0;
}

} // namespace harness_000
