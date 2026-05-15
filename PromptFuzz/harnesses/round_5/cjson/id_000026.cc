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
//<ID> 124
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 1
//<Quality> {"density":19,"unique_branches":{"cJSON_IsNumber":[[3024,9,3024,21,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_Delete","cJSON_CreateStringArray","cJSON_AddItemToObject","cJSON_AddTrueToObject","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_CreateStringReference","cJSON_AddItemToObject","cJSON_AddObjectToObject","cJSON_AddStringToObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateStringReference","cJSON_ReplaceItemInObject","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_CreateDoubleArray","cJSON_ReplaceItemInObject","cJSON_Print","cJSON_free","cJSON_Parse","cJSON_AddTrueToObject","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_CreateStringArray","cJSON_AddItemToObject","cJSON_AddTrueToObject","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_CreateStringReference","cJSON_AddItemToObject","cJSON_AddObjectToObject","cJSON_AddStringToObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateStringReference","cJSON_ReplaceItemInObject","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_CreateDoubleArray","cJSON_ReplaceItemInObject","cJSON_Print","cJSON_free","cJSON_Parse","cJSON_AddTrueToObject","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":1}
/*To create a fuzz driver that uses the specified cJSON APIs to process JSON input data, I'll write a program that:

1. Parses the input JSON data
2. Creates various cJSON structures using the required APIs
3. Manipulates the parsed data
4. Cleans up all allocated resources

Here's the complete implementation:

*/


// Include cJSON header
extern "C" {
    #include "cJSON.h"
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

    // Create a null-terminated string from the input data
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, data, size);
    json_str[size] = '\0';

    // Parse the JSON input
    cJSON *parsed_json = cJSON_Parse(json_str);
    if (!parsed_json) {
        free(json_str);
        return 0;
    }

    // Create a new object to work with
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        cJSON_Delete(parsed_json);
        free(json_str);
        return 0;
    }

    // 1. Create a string array (using cJSON_CreateStringArray)
    const char *string_array_data[] = {"item1", "item2", "item3"};
    cJSON *string_array = cJSON_CreateStringArray(string_array_data, 3);
    if (string_array) {
        cJSON_AddItemToObject(root, "string_array", string_array);
    }

    // 2. Add a true value to the object (using cJSON_AddTrueToObject)
    cJSON_AddTrueToObject(root, "is_valid");

    // 3. Create a double array (using cJSON_CreateDoubleArray)
    double double_array_data[] = {1.1, 2.2, 3.3, 4.4};
    cJSON *double_array = cJSON_CreateDoubleArray(double_array_data, 4);
    if (double_array) {
        cJSON_AddItemToObject(root, "double_array", double_array);
    }

    // 4. Create a string reference (using cJSON_CreateStringReference)
    const char *ref_string = "reference_string";
    cJSON *string_ref = cJSON_CreateStringReference(ref_string);
    if (string_ref) {
        cJSON_AddItemToObject(root, "string_reference", string_ref);
    }

    // 5. Add an object to the object (using cJSON_AddObjectToObject)
    cJSON *nested_object = cJSON_AddObjectToObject(root, "nested_object");
    if (nested_object) {
        // Add some data to the nested object
        cJSON_AddStringToObject(nested_object, "nested_key", "nested_value");
    }

    // 6. Create an array (using cJSON_CreateArray)
    cJSON *new_array = cJSON_CreateArray();
    if (new_array) {
        // Add some items to the array
        cJSON_AddItemToArray(new_array, cJSON_CreateNumber(100));
        cJSON_AddItemToArray(new_array, cJSON_CreateString("array_item"));
        cJSON_AddItemToObject(root, "new_array", new_array);
    }

    // Process the parsed JSON to extract data for our structures
    // Try to get the "widget" object from the parsed JSON
    cJSON *widget = cJSON_GetObjectItem(parsed_json, "widget");
    if (widget) {
        // Try to extract strings from the widget object
        cJSON *window = cJSON_GetObjectItem(widget, "window");
        if (window) {
            cJSON *title = cJSON_GetObjectItem(window, "title");
            if (cJSON_IsString(title) && title->valuestring) {
                // Update the string reference with data from input
                cJSON *new_string_ref = cJSON_CreateStringReference(title->valuestring);
                if (new_string_ref) {
                    // Replace the old string reference
                    cJSON_ReplaceItemInObject(root, "string_reference", new_string_ref);
                }
            }
        }

        // Try to extract numbers for the double array
        cJSON *text = cJSON_GetObjectItem(widget, "text");
        if (text) {
            cJSON *size_item = cJSON_GetObjectItem(text, "size");
            if (cJSON_IsNumber(size_item)) {
                // Create a new double array with data from input
                double new_double_data[] = {
                    size_item->valuedouble,
                    size_item->valuedouble * 2,
                    size_item->valuedouble * 3
                };
                cJSON *new_double_array = cJSON_CreateDoubleArray(new_double_data, 3);
                if (new_double_array) {
                    cJSON_ReplaceItemInObject(root, "double_array", new_double_array);
                }
            }
        }
    }

    // Create a FILE* for output (as specified in the requirements)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Print the resulting JSON to the file
        char *printed_json = cJSON_Print(root);
        if (printed_json) {
            fwrite(printed_json, strlen(printed_json), 1, out_file);
            cJSON_free(printed_json);
        }
        assert_file_closed(&out_file);;
    }

    // Create a FILE* for input using fmemopen (as specified in the requirements)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read and process data from the file
        char buffer[256];
        size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, in_file);
        buffer[bytes_read] = '\0';
        
        // Parse this data as well
        cJSON *additional_json = cJSON_Parse(buffer);
        if (additional_json) {
            // Add a marker to indicate we processed the file data
            cJSON_AddTrueToObject(root, "file_data_processed");
            cJSON_Delete(additional_json);
        }
        
        // Get file descriptor (as specified in the requirements)
        int fd = fuzz_fileno(in_file);
        (void)fd; // Mark as used to avoid compiler warnings
        
        assert_file_closed(&in_file);;
    }

    // Clean up all allocated resources
    cJSON_Delete(root);
    cJSON_Delete(parsed_json);
    free(json_str);

    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
}