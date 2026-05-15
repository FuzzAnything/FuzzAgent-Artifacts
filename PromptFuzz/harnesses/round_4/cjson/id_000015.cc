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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 2
//<Quality> {"density":20,"unique_branches":{"cJSON_IsNull":[[3014,9,3014,21,0,0,4,0]],"cJSON_GetStringValue":[[101,9,101,30,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_GetObjectItemCaseSensitive","cJSON_Delete","cJSON_CreateArray","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_GetStringValue","cJSON_CreateString","cJSON_AddItemReferenceToArray","cJSON_Delete","cJSON_IsNull","cJSON_AddFalseToObject","cJSON_Delete","cJSON_Delete","cJSON_CreateNull","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_Delete","cJSON_Print","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_GetObjectItemCaseSensitive","cJSON_CreateArray","cJSON_GetObjectItemCaseSensitive","cJSON_GetStringValue","cJSON_CreateString","cJSON_AddItemReferenceToArray","cJSON_Delete","cJSON_IsNull","cJSON_AddFalseToObject","cJSON_CreateNull","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_Print","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


// Include cJSON headers (assuming they're available in the build environment)

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<35) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    // Create a temporary file to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the JSON data from the file
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *json_string = (char *)malloc(file_size + 1);
    if (!json_string) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t read_size = fread(json_string, 1, file_size, in_file);
    json_string[read_size] = '\0';
    assert_file_closed(&in_file);;

    // Parse the JSON string
    cJSON *root = cJSON_ParseWithLength(json_string, read_size);
    if (!root) {
        free(json_string);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the "widget" object
    cJSON *widget = cJSON_GetObjectItemCaseSensitive(root, fuzz_str_1);
    if (!widget) {
        cJSON_Delete(root);
        free(json_string);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a new array
    cJSON *new_array = cJSON_CreateArray();
    if (!new_array) {
        cJSON_Delete(root);
        free(json_string);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the "debug" item from widget
    cJSON *debug_item = cJSON_GetObjectItemCaseSensitive(widget, fuzz_str_2);
    if (debug_item) {
        // Get the string value of debug item
        char *debug_value = cJSON_GetStringValue(debug_item);
        if (debug_value) {
            // Create a string item with the debug value and add it to array
            cJSON *string_item = cJSON_CreateString(debug_value);
            if (string_item) {
                // Add reference to array (doesn't transfer ownership)
                cJSON_AddItemReferenceToArray(new_array, string_item);
                // Delete the original string item since we're using a reference
                cJSON_Delete(string_item);
            }
        }
    }

    // Check if "debug" item is null (it shouldn't be in our input)
    cJSON_bool is_debug_null = cJSON_IsNull(debug_item);

    // Add a false boolean to widget object
    cJSON *false_item = cJSON_AddFalseToObject(widget, fuzz_str_3);
    if (!false_item) {
        cJSON_Delete(new_array);
        cJSON_Delete(root);
        free(json_string);
        assert_file_closed(&in_file);
	return 0;
    }

    // Replace an item in the widget object - let's replace the "debug" field
    // First create a new null item to replace with
    cJSON *new_null_item = cJSON_CreateNull();
    if (new_null_item && debug_item) {
        // Replace the debug item with a null item
        cJSON_ReplaceItemInObjectCaseSensitive(widget, "debug", new_null_item);
    } else {
        if (new_null_item) cJSON_Delete(new_null_item);
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Print the modified JSON to file
        char *printed = cJSON_Print(root);
        if (printed) {
            fwrite(printed, 1, strlen(printed), out_file);
            free(printed);
        }
        assert_file_closed(&out_file);;
    }

    // Clean up
    cJSON_Delete(new_array);
    cJSON_Delete(root);
    free(json_string);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}