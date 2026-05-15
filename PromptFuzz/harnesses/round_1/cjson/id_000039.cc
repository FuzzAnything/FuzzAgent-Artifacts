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
//<ID> 185
//<Prompt> []
/*<Combination>: [
*/
//<score> 84, nr_unique_branch: 1
//<Quality> {"density":42,"unique_branches":{"print_object":[[1791,9,1791,52,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_IsInvalid","cJSON_CreateObject","cJSON_Delete","cJSON_GetObjectItem","cJSON_GetObjectItemCaseSensitive","cJSON_Duplicate","cJSON_CreateNull","cJSON_AddItemToObject","cJSON_AddBoolToObject","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_CreateString","cJSON_AddItemToObject","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_CreateObjectReference","cJSON_AddItemToObject","cJSON_AddObjectToObject","cJSON_AddNumberToObject","cJSON_IsArray","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_InsertItemInArray","cJSON_GetArrayItem","cJSON_CreateString","cJSON_ReplaceItemViaPointer","cJSON_CreateNumber","cJSON_ReplaceItemInArray","cJSON_AddStringToObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToObject","cJSON_IsRaw","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_IsInvalid","cJSON_CreateObject","cJSON_GetObjectItem","cJSON_GetObjectItemCaseSensitive","cJSON_Duplicate","cJSON_CreateNull","cJSON_AddItemToObject","cJSON_AddBoolToObject","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_CreateString","cJSON_AddItemToObject","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_CreateObjectReference","cJSON_AddItemToObject","cJSON_AddObjectToObject","cJSON_AddNumberToObject","cJSON_IsArray","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_InsertItemInArray","cJSON_GetArrayItem","cJSON_CreateString","cJSON_ReplaceItemViaPointer","cJSON_CreateNumber","cJSON_ReplaceItemInArray","cJSON_AddStringToObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToObject","cJSON_IsRaw","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure we have enough data to work with
    if (size == 0) {
        return 0;
    }

    // Create a FILE* for reading input if needed (though we'll use memory APIs directly)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Parse input JSON using cJSON_ParseWithLength
    char *json_string = (char *)malloc(size + 1);
    if (!json_string) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    fread(json_string, 1, size, in_file);
    json_string[size] = '\0';
    assert_file_closed(&in_file);;

    cJSON *parsed_json = cJSON_ParseWithLength(json_string, size);
    free(json_string);

    // Check if parsing succeeded
    if (!parsed_json) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Check if the parsed item is invalid
    cJSON_bool is_invalid = cJSON_IsInvalid(parsed_json);

    // 3. Create a new JSON object
    cJSON *new_obj = cJSON_CreateObject();
    if (!new_obj) {
        cJSON_Delete(parsed_json);
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Get object items from parsed JSON using both methods
    cJSON *widget_item = cJSON_GetObjectItem(parsed_json, "widget");
    cJSON *debug_item = cJSON_GetObjectItemCaseSensitive(parsed_json, "widget");

    // 5. Duplicate the parsed JSON
    cJSON *duplicated = cJSON_Duplicate(parsed_json, 1);

    // 6. Create a null item
    cJSON *null_item = cJSON_CreateNull();

    // 7. Add items to new object
    if (null_item) {
        cJSON_AddItemToObject(new_obj, "null_field", null_item);
    }

    // 8. Add boolean to object
    cJSON *bool_item = cJSON_AddBoolToObject(new_obj, "boolean_field", !is_invalid);

    // 9. Create number and add to object
    cJSON *number_item = cJSON_CreateNumber(size);
    if (number_item) {
        cJSON_AddItemToObject(new_obj, "size_field", number_item);
    }

    // 10. Create string and add to object
    cJSON *string_item = cJSON_CreateString("fuzzed_string");
    if (string_item) {
        cJSON_AddItemToObject(new_obj, "string_field", string_item);
    }

    // 11. Create int array
    int int_array[] = {1, 2, 3, 4, 5};
    cJSON *int_array_item = cJSON_CreateIntArray(int_array, 5);
    if (int_array_item) {
        cJSON_AddItemToObject(new_obj, "int_array", int_array_item);
    }

    // 12. Create float array
    float float_array[] = {1.1f, 2.2f, 3.3f};
    cJSON *float_array_item = cJSON_CreateFloatArray(float_array, 3);
    if (float_array_item) {
        cJSON_AddItemToObject(new_obj, "float_array", float_array_item);
    }

    // 13. Create object reference
    cJSON *obj_ref = cJSON_CreateObjectReference(new_obj);
    if (obj_ref) {
        cJSON_AddItemToObject(new_obj, "self_reference", obj_ref);
    }

    // 14. Add object to object
    cJSON *nested_obj = cJSON_AddObjectToObject(new_obj, "nested_object");

    // 15. Add number to nested object
    if (nested_obj) {
        cJSON_AddNumberToObject(nested_obj, "nested_number", 42.0);
    }

    // 16. Check if an item is array and manipulate it
    if (widget_item && cJSON_IsArray(widget_item)) {
        int array_size = cJSON_GetArraySize(widget_item);
        
        if (array_size > 0) {
            // Detach an item
            cJSON *detached = cJSON_DetachItemFromArray(widget_item, 0);
            
            // Insert item back
            if (detached) {
                cJSON_InsertItemInArray(widget_item, 0, detached);
            }
            
            // Get array item
            cJSON *array_item = cJSON_GetArrayItem(widget_item, 0);
            
            // Replace item via pointer
            if (array_item) {
                cJSON *replacement = cJSON_CreateString("replaced");
                if (replacement) {
                    cJSON_ReplaceItemViaPointer(widget_item, array_item, replacement);
                }
            }
            
            // Replace item in array by index
            cJSON *another_replacement = cJSON_CreateNumber(999);
            if (another_replacement) {
                cJSON_ReplaceItemInArray(widget_item, 0, another_replacement);
            }
        }
    }

    // 17. Add string to object (different API)
    cJSON_AddStringToObject(new_obj, "another_string", "test");

    // 18. Create an array and add items to it
    cJSON *new_array = cJSON_CreateArray();
    if (new_array) {
        cJSON_AddItemToArray(new_array, cJSON_CreateNumber(1));
        cJSON_AddItemToArray(new_array, cJSON_CreateString("array_item"));
        cJSON_AddItemToObject(new_obj, "new_array", new_array);
    }

    // 19. Check if item is raw (though we likely don't have raw items)
    cJSON_bool is_raw = cJSON_IsRaw(parsed_json);

    // 20. Print the resulting JSON
    char *printed_json = cJSON_Print(new_obj);
    
    // Create output file if needed
    FILE *out_file = fopen("output_file", "wb");
    if (out_file && printed_json) {
        fwrite(printed_json, 1, strlen(printed_json), out_file);
        assert_file_closed(&out_file);;
    }

    // 21. Free allocated resources
    if (printed_json) {
        cJSON_free(printed_json);
    }
    
    // Clean up all cJSON objects
    cJSON_Delete(parsed_json);
    cJSON_Delete(new_obj);
    if (duplicated) {
        cJSON_Delete(duplicated);
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}