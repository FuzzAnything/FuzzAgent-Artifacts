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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 90, nr_unique_branch: 6
//<Quality> {"density":30,"unique_branches":{"add_item_to_object":[[2087,9,2087,25,0,0,4,0]],"cJSON_AddObjectToObject":[[2237,9,2237,76,0,0,4,0],[2237,9,2237,76,0,0,4,1]],"cJSON_DetachItemViaPointer":[[2270,9,2270,27,0,0,4,1]],"cJSON_DetachItemFromArray":[[2296,9,2296,18,0,0,4,1]],"cJSON_IsInvalid":[[2974,9,2974,21,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_IsInvalid","cJSON_Delete","cJSON_AddObjectToObject","cJSON_Delete","cJSON_CreateFloatArray","cJSON_Delete","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_Duplicate","cJSON_ReplaceItemViaPointer","cJSON_GetObjectItem","cJSON_IsObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToObject","cJSON_DetachItemFromArray","cJSON_IsNull","cJSON_Delete","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_DetachItemViaPointer","cJSON_IsNull","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_IsInvalid","cJSON_AddObjectToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_Duplicate","cJSON_ReplaceItemViaPointer","cJSON_GetObjectItem","cJSON_IsObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToObject","cJSON_DetachItemFromArray","cJSON_IsNull","cJSON_Delete","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_DetachItemViaPointer","cJSON_IsNull","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 1: Parse input JSON using cJSON_ParseWithLength
    const char *json_str = reinterpret_cast<const char*>(data);
    cJSON *root = cJSON_ParseWithLength(json_str, size);
    
    // Check if parsing succeeded
    if (cJSON_IsInvalid(root)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Step 2: Create a new object to work with
    cJSON *new_obj = cJSON_AddObjectToObject(root, "fuzz_operations");
    if (!new_obj) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Step 3: Create a float array
    float float_numbers[] = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f};
    cJSON *float_array = cJSON_CreateFloatArray(float_numbers, 5);
    if (!float_array) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Step 4: Add float array to new object
    cJSON_AddItemToObject(new_obj, "float_data", float_array);
    
    // Step 5: Duplicate a part of the structure
    cJSON *widget = cJSON_GetObjectItem(root, "widget");
    if (widget && cJSON_IsObject(widget)) {
        cJSON *window = cJSON_GetObjectItem(widget, "window");
        if (window) {
            // Create a duplicate of window
            cJSON *window_duplicate = cJSON_Duplicate(window, true);
            if (window_duplicate) {
                // Replace the original window with duplicate using pointer
                cJSON_ReplaceItemViaPointer(widget, window, window_duplicate);
            }
        }
        
        // Step 6: Check and handle array operations
        cJSON *text = cJSON_GetObjectItem(widget, "text");
        if (text && cJSON_IsObject(text)) {
            // Create a dummy array item for demonstration
            cJSON *dummy_array = cJSON_CreateArray();
            if (dummy_array) {
                cJSON_AddItemToArray(dummy_array, cJSON_CreateString("array_item1"));
                cJSON_AddItemToArray(dummy_array, cJSON_CreateString("array_item2"));
                cJSON_AddItemToObject(text, "dummy_array", dummy_array);
                
                // Detach item from array by index
                cJSON *detached_by_index = cJSON_DetachItemFromArray(dummy_array, 0);
                if (detached_by_index) {
                    // Check if detached item is null
                    if (!cJSON_IsNull(detached_by_index)) {
                        cJSON_Delete(detached_by_index);
                    }
                }
                
                // Detach item via pointer (detach remaining item)
                if (cJSON_GetArraySize(dummy_array) > 0) {
                    cJSON *array_item = cJSON_GetArrayItem(dummy_array, 0);
                    if (array_item) {
                        cJSON *detached_by_pointer = cJSON_DetachItemViaPointer(dummy_array, array_item);
                        if (detached_by_pointer) {
                            // Check if detached item is null
                            if (!cJSON_IsNull(detached_by_pointer)) {
                                cJSON_Delete(detached_by_pointer);
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Step 7: Clean up - free all allocated resources
    cJSON_Delete(root);
    
    return 0;
}