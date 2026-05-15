#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <cJSON.h>
}
//<ID> 89
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 3
//<Quality> {"density":38,"unique_branches":{"cJSON_Compare":[[3087,9,3087,23,0,0,4,0],[3118,9,3118,23,0,0,4,0]],"print_value":[[1474,17,1474,31,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_Delete","cJSON_CreateRaw","cJSON_AddItemToObject","cJSON_AddFalseToObject","cJSON_AddNumberToObject","cJSON_AddObjectToObject","cJSON_AddNumberToObject","cJSON_AddFalseToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsBool","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsBool","cJSON_CreateBool","cJSON_IsBool","cJSON_AddItemToObject","cJSON_ParseWithLength","cJSON_GetObjectItemCaseSensitive","cJSON_IsBool","cJSON_Delete","cJSON_Print","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_CreateStringArray","cJSON_AddItemToObject","cJSON_Duplicate","cJSON_Compare","cJSON_Delete","cJSON_PrintUnformatted","cJSON_Minify","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_CreateRaw","cJSON_AddItemToObject","cJSON_AddFalseToObject","cJSON_AddNumberToObject","cJSON_AddObjectToObject","cJSON_AddNumberToObject","cJSON_AddFalseToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsBool","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsBool","cJSON_CreateBool","cJSON_IsBool","cJSON_AddItemToObject","cJSON_ParseWithLength","cJSON_GetObjectItemCaseSensitive","cJSON_IsBool","cJSON_Delete","cJSON_Print","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_CreateStringArray","cJSON_AddItemToObject","cJSON_Duplicate","cJSON_Compare","cJSON_Delete","cJSON_PrintUnformatted","cJSON_Minify","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete"],"visited":2}
/*To create a comprehensive fuzz driver for cjson that focuses on the specified APIs and follows all requirements, I'll implement a fuzzer that:

1. Parses JSON from the input data
2. Creates new cJSON objects using the specified APIs
3. Tests various cJSON operations
4. Properly cleans up all resources

Here's the complete implementation:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a null-terminated string from the input data for parsing
    char *input_str = (char *)malloc(size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, data, size);
    input_str[size] = '\0';

    // Parse the input JSON data
    cJSON *root = cJSON_ParseWithLength(input_str, size);
    if (!root) {
        free(input_str);
        return 0;
    }

    // Create a new cJSON object to work with
    cJSON *new_object = cJSON_CreateObject();
    if (!new_object) {
        cJSON_Delete(root);
        free(input_str);
        return 0;
    }

    // 1. Use cJSON_CreateRaw to create a raw JSON string
    cJSON *raw_json = cJSON_CreateRaw(input_str);
    if (raw_json) {
        // Add the raw JSON to our new object using cJSON_AddItemToObject
        cJSON_AddItemToObject(new_object, "raw_input", raw_json);
    }

    // 2. Use cJSON_AddFalseToObject to add a false boolean
    cJSON_AddFalseToObject(new_object, "false_value");

    // 3. Use cJSON_AddNumberToObject to add a number
    double num_value = (double)size / 100.0;
    cJSON_AddNumberToObject(new_object, "size_derived", num_value);

    // 4. Use cJSON_AddObjectToObject to add a nested object
    cJSON *nested_object = cJSON_AddObjectToObject(new_object, "nested");
    if (nested_object) {
        // Add some items to the nested object
        cJSON_AddNumberToObject(nested_object, "depth", 1.0);
        cJSON_AddFalseToObject(nested_object, "nested_false");
    }

    // 5. Check if various items are booleans using cJSON_IsBool
    cJSON *false_item = cJSON_GetObjectItemCaseSensitive(new_object, "false_value");
    if (false_item && cJSON_IsBool(false_item)) {
        // This should pass since we added a false boolean
        // No action needed, just testing the API
    }

    // Check items in the original parsed JSON for boolean types
    cJSON *widget = cJSON_GetObjectItemCaseSensitive(root, "widget");
    if (widget && cJSON_IsObject(widget)) {
        cJSON *debug = cJSON_GetObjectItemCaseSensitive(widget, "debug");
        if (debug) {
            // Test cJSON_IsBool on a string (should return false)
            if (!cJSON_IsBool(debug)) {
                // Expected: debug is a string, not a bool
            }
        }
    }

    // Create another boolean item and test it
    cJSON *bool_test = cJSON_CreateBool(1);
    if (bool_test && cJSON_IsBool(bool_test)) {
        cJSON_AddItemToObject(new_object, "test_bool", bool_test);
    }

    // Test with FILE operations as specified in requirements
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read the data back from the file
        uint8_t *buffer = (uint8_t *)malloc(size);
        if (buffer) {
            fread(buffer, 1, size, in_file);
            
            // Create another JSON from the file data
            char *file_str = (char *)malloc(size + 1);
            if (file_str) {
                memcpy(file_str, buffer, size);
                file_str[size] = '\0';
                
                cJSON *file_json = cJSON_ParseWithLength(file_str, size);
                if (file_json) {
                    // Test boolean check on this JSON
                    cJSON *item = cJSON_GetObjectItemCaseSensitive(file_json, "widget");
                    if (item) {
                        cJSON_IsBool(item); // Test the API
                    }
                    cJSON_Delete(file_json);
                }
                free(file_str);
            }
            free(buffer);
        }
        assert_file_closed(&in_file);;
    }

    // Write output to a file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        char *printed = cJSON_Print(new_object);
        if (printed) {
            fwrite(printed, 1, strlen(printed), out_file);
            free(printed);
        }
        
        // Get file descriptor
        int fd = fuzz_fileno(out_file);
        (void)fd; // Use the descriptor to avoid unused variable warning
        
        assert_file_closed(&out_file);;
    }

    // Additional testing: Create arrays with various types
    int int_array[] = {1, 2, 3, 4, 5};
    cJSON *int_array_json = cJSON_CreateIntArray(int_array, 5);
    if (int_array_json) {
        cJSON_AddItemToObject(new_object, "int_array", int_array_json);
    }

    double double_array[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    cJSON *double_array_json = cJSON_CreateDoubleArray(double_array, 5);
    if (double_array_json) {
        cJSON_AddItemToObject(new_object, "double_array", double_array_json);
    }

    // Test string array
    const char *string_array[] = {"test1", "test2", "test3"};
    cJSON *string_array_json = cJSON_CreateStringArray(string_array, 3);
    if (string_array_json) {
        cJSON_AddItemToObject(new_object, "string_array", string_array_json);
    }

    // Test duplication and comparison
    cJSON *duplicate = cJSON_Duplicate(new_object, 1);
    if (duplicate) {
        // Compare the objects
        cJSON_Compare(new_object, duplicate, 1);
        cJSON_Delete(duplicate);
    }

    // Test minify on a JSON string
    char *json_to_minify = cJSON_PrintUnformatted(new_object);
    if (json_to_minify) {
        cJSON_Minify(json_to_minify);
        free(json_to_minify);
    }

    // Test preallocated printing
    char print_buffer[1024];
    cJSON_PrintPreallocated(new_object, print_buffer, sizeof(print_buffer), 1);

    // Clean up all resources
    cJSON_Delete(root);
    cJSON_Delete(new_object);
    free(input_str);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}