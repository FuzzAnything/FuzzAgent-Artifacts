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
//<ID> 54
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_Delete","cJSON_IsInvalid","cJSON_CreateString","cJSON_IsInvalid","cJSON_Delete","cJSON_CreateStringReference","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_CreateArrayReference","cJSON_CreateArray","cJSON_CreateString","cJSON_CreateString","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_InsertItemInArray","cJSON_AddItemToObject","cJSON_AddRawToObject","cJSON_IsInvalid","cJSON_AddStringToObject","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_IsInvalid","cJSON_CreateString","cJSON_IsInvalid","cJSON_Delete","cJSON_CreateStringReference","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_CreateArrayReference","cJSON_CreateArray","cJSON_CreateString","cJSON_CreateString","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_InsertItemInArray","cJSON_AddItemToObject","cJSON_AddRawToObject","cJSON_IsInvalid","cJSON_AddStringToObject","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the entire input file content
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *json_str = (char *)malloc(file_size + 1);
    if (!json_str) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(json_str, 1, file_size, in_file);
    json_str[bytes_read] = '\0';
    assert_file_closed(&in_file);;

    // Parse the JSON data
    cJSON *root = cJSON_Parse(json_str);
    if (!root) {
        free(json_str);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a new cJSON object for manipulation
    cJSON *new_object = cJSON_CreateObject();
    if (!new_object) {
        cJSON_Delete(root);
        free(json_str);
        assert_file_closed(&in_file);
	return 0;
    }

    // Event: Create a raw JSON string, manipulate array references, and handle invalid items
    
    // 1. First, check if the parsed root contains any invalid items
    // We'll iterate through the parsed JSON to check for invalid items
    if (cJSON_IsInvalid(root)) {
        // This shouldn't happen since we successfully parsed, but we call the API
        // For demonstration, we create an invalid item manually
        cJSON *dummy = cJSON_CreateString("dummy");
        if (dummy) {
            dummy->type = 0; // Mark as invalid
            cJSON_IsInvalid(dummy);
            dummy->type = cJSON_String; // Restore to avoid issues
            cJSON_Delete(dummy);
        }
    }

    // 2. Create a string reference
    const char *ref_string = "reference_string";
    cJSON *string_ref = cJSON_CreateStringReference(ref_string);
    if (string_ref) {
        // Add the string reference to our new object
        cJSON_AddItemToObject(new_object, "string_ref", string_ref);
    }

    // 3. Extract the "widget" object from parsed JSON
    cJSON *widget = cJSON_GetObjectItem(root, "widget");
    if (widget && cJSON_IsObject(widget)) {
        // Extract the "text" array from widget if it exists
        cJSON *text = cJSON_GetObjectItem(widget, "text");
        
        // 4. Create an array reference from the text object
        cJSON *array_ref = cJSON_CreateArrayReference(text);
        if (array_ref) {
            // Create a new array to insert into
            cJSON *new_array = cJSON_CreateArray();
            if (new_array) {
                // Add some items to the new array
                cJSON *item1 = cJSON_CreateString("item1");
                cJSON *item2 = cJSON_CreateString("item2");
                
                if (item1) cJSON_AddItemToArray(new_array, item1);
                if (item2) cJSON_AddItemToArray(new_array, item2);
                
                // 5. Insert the array reference into the new array at position 1
                cJSON_InsertItemInArray(new_array, 1, array_ref);
                
                // Add the array to our new object
                cJSON_AddItemToObject(new_object, "mixed_array", new_array);
            }
        }

        // 6. Add a raw JSON string to the widget object
        const char *raw_json = "{\"raw_key\": \"raw_value\"}";
        cJSON *raw_item = cJSON_AddRawToObject(widget, "raw_data", raw_json);
        
        // Check if the raw item is invalid (it shouldn't be)
        if (raw_item) {
            cJSON_IsInvalid(raw_item); // Should return false
        }

        // 7. Delete an item from the widget object (case-sensitive)
        // First, let's add an item to delete
        cJSON *temp_item = cJSON_AddStringToObject(widget, "TempItem", "ToBeDeleted");
        
        // Now delete it case-sensitively
        if (temp_item) {
            cJSON_DeleteItemFromObjectCaseSensitive(widget, "TempItem");
        }
    }

    // Clean up: Delete the created objects
    cJSON_Delete(root);
    cJSON_Delete(new_object);
    free(json_str);

    assert_file_closed(&in_file);
	return 0;
}