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
//<ID> 195
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 2
//<Quality> {"density":13,"unique_branches":{"cJSON_SetValuestring":[[457,16,457,58,0,0,4,1],[457,62,457,104,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateRaw","cJSON_CreateFalse","cJSON_CreateBool","cJSON_CreateNull","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateObjectReference","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_CreateArrayReference","cJSON_Delete","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_CreateString","cJSON_InsertItemInArray","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_SetValuestring","cJSON_IsFalse","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateRaw","cJSON_CreateFalse","cJSON_CreateBool","cJSON_CreateNull","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateObjectReference","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_CreateArrayReference","cJSON_Delete","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_CreateString","cJSON_InsertItemInArray","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_SetValuestring","cJSON_IsFalse","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0 || data == nullptr) {
        return 0;
    }

    // Step 1: Parse input JSON data using cJSON_ParseWithLength
    cJSON *parsed = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!parsed) {
        return 0;  // Invalid JSON input
    }

    // Step 2: Create various cJSON items using the specified APIs
    cJSON *raw_item = cJSON_CreateRaw("\"raw_value\"");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *bool_item = cJSON_CreateBool(1);  // true boolean
    cJSON *null_item = cJSON_CreateNull();

    // Step 3: Create array and object references from parsed data
    // First find if there's an array in the parsed object
    cJSON *widget_array = cJSON_GetObjectItemCaseSensitive(parsed, "widget");
    cJSON *array_ref = nullptr;
    cJSON *object_ref = nullptr;
    
    if (widget_array && cJSON_IsObject(widget_array)) {
        // Create references
        object_ref = cJSON_CreateObjectReference(widget_array);
        
        // Look for an array inside the widget (there isn't one in the input, so we'll create one)
        cJSON *window = cJSON_GetObjectItemCaseSensitive(widget_array, "window");
        if (window && cJSON_IsObject(window)) {
            // Create an array reference to window's items (though window is an object, not an array)
            // We'll create a dummy array instead
            cJSON *dummy_array = cJSON_CreateArray();
            cJSON_AddItemToArray(dummy_array, cJSON_CreateNumber(1));
            cJSON_AddItemToArray(dummy_array, cJSON_CreateNumber(2));
            array_ref = cJSON_CreateArrayReference(dummy_array);
            cJSON_Delete(dummy_array);  // Reference doesn't own the array
        }
    }

    // Step 4: Create a new array and insert items using cJSON_InsertItemInArray
    cJSON *new_array = cJSON_CreateArray();
    cJSON_AddItemToArray(new_array, raw_item);  // Add at index 0
    cJSON_AddItemToArray(new_array, false_item); // Add at index 1
    cJSON_AddItemToArray(new_array, bool_item);  // Add at index 2
    cJSON_AddItemToArray(new_array, null_item);  // Add at index 3
    
    // Insert a new item at index 1 (shifts others)
    cJSON *inserted_item = cJSON_CreateString("inserted");
    cJSON_InsertItemInArray(new_array, 1, inserted_item);

    // Step 5: Modify a string value using cJSON_SetValuestring
    if (widget_array && cJSON_IsObject(widget_array)) {
        cJSON *debug_item = cJSON_GetObjectItemCaseSensitive(widget_array, "debug");
        if (debug_item && cJSON_IsString(debug_item)) {
            // Create a copy of the string to avoid modifying original buffer
            char *new_value = strdup("modified");
            if (new_value) {
                char *old_value = cJSON_SetValuestring(debug_item, new_value);
                free(new_value);  // cJSON makes its own copy
            }
        }
    }

    // Step 6: Check if false_item is indeed false using cJSON_IsFalse
    if (cJSON_IsFalse(false_item)) {
        // This is expected
        volatile int check = 1;  // Prevent optimization
        (void)check;
    }

    // Step 7: Clean up all allocated resources
    // Note: array_ref and object_ref are references, they don't own the memory
    if (array_ref) {
        cJSON_Delete(array_ref);
    }
    if (object_ref) {
        cJSON_Delete(object_ref);
    }
    
    // Clean up the array and all its items
    cJSON_Delete(new_array);
    
    // Clean up parsed JSON
    cJSON_Delete(parsed);

    return 0;
}