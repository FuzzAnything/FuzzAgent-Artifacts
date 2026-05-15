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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_free","cJSON_CreateObject","cJSON_Delete","cJSON_free","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_AddNullToObject","cJSON_AddTrueToObject","cJSON_AddFalseToObject","cJSON_AddItemReferenceToObject","cJSON_DeleteItemFromObject","cJSON_Print","cJSON_Minify","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_CreateObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_AddNullToObject","cJSON_AddTrueToObject","cJSON_AddFalseToObject","cJSON_AddItemReferenceToObject","cJSON_DeleteItemFromObject","cJSON_Print","cJSON_Minify","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Open input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the entire input file
    fseek(in_file, 0, SEEK_END);
    long input_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *json_str = (char *)cJSON_malloc(input_size + 1);
    if (!json_str) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t read_size = fread(json_str, 1, input_size, in_file);
    json_str[read_size] = '\0';
    assert_file_closed(&in_file);;

    // Parse the JSON string
    cJSON *parsed_json = cJSON_ParseWithLength(json_str, read_size);
    if (!parsed_json) {
        cJSON_free(json_str);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a new object for our modifications
    cJSON *new_object = cJSON_CreateObject();
    if (!new_object) {
        cJSON_Delete(parsed_json);
        cJSON_free(json_str);
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Check if an existing item is a string
    cJSON *widget_item = cJSON_GetObjectItem(parsed_json, "widget");
    if (widget_item && cJSON_IsObject(widget_item)) {
        cJSON *debug_item = cJSON_GetObjectItem(widget_item, "debug");
        if (debug_item) {
            // Use cJSON_IsString API
            bool is_string = cJSON_IsString(debug_item) != 0;
            (void)is_string; // Prevent unused variable warning
        }
    }

    // 2. Add items to the new object
    cJSON *null_item = cJSON_AddNullToObject(new_object, "null_field");
    (void)null_item; // Mark as used
    
    cJSON *true_item = cJSON_AddTrueToObject(new_object, "true_field");
    (void)true_item; // Mark as used
    
    cJSON *false_item = cJSON_AddFalseToObject(new_object, "false_field");
    (void)false_item; // Mark as used

    // 3. Add a reference to an existing item
    if (widget_item) {
        cJSON_AddItemReferenceToObject(new_object, "widget_ref", widget_item);
    }

    // 4. Delete an item from the new object
    cJSON_DeleteItemFromObject(new_object, "true_field");

    // 5. Create a JSON string and minify it
    char *printed_json = cJSON_Print(new_object);
    if (printed_json) {
        // Use cJSON_Minify API
        cJSON_Minify(printed_json);
        
        // Write to output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed_json, 1, strlen(printed_json), out_file);
            assert_file_closed(&out_file);;
        }
        
        cJSON_free(printed_json);
    }

    // Clean up
    cJSON_Delete(parsed_json);
    cJSON_Delete(new_object);
    cJSON_free(json_str);

    assert_file_closed(&in_file);
	return 0;
}