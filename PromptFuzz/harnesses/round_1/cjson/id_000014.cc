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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 6
//<Quality> {"density":24,"unique_branches":{"print_string_ptr":[[969,9,969,22,0,0,4,0],[972,13,972,27,0,0,4,1]],"cJSON_ParseWithLengthOpts":[[1156,26,1156,44,0,0,4,0],[1195,9,1195,21,0,0,4,1],[1210,18,1210,35,0,0,4,1]],"get_object_item":[[1955,9,1955,25,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsNull","cJSON_IsInvalid","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsInvalid","cJSON_CreateFloatArray","cJSON_AddObjectToObject","cJSON_AddItemToObject","cJSON_GetObjectItemCaseSensitive","cJSON_Duplicate","cJSON_AddItemToObject","cJSON_GetObjectItemCaseSensitive","cJSON_CreateString","cJSON_ReplaceItemViaPointer","cJSON_GetObjectItemCaseSensitive","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Delete","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateNull","cJSON_GetArrayItem","cJSON_IsNull","cJSON_IsInvalid","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsNull","cJSON_IsInvalid","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsInvalid","cJSON_CreateFloatArray","cJSON_AddObjectToObject","cJSON_AddItemToObject","cJSON_GetObjectItemCaseSensitive","cJSON_Duplicate","cJSON_AddItemToObject","cJSON_GetObjectItemCaseSensitive","cJSON_CreateString","cJSON_ReplaceItemViaPointer","cJSON_GetObjectItemCaseSensitive","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateNull","cJSON_GetArrayItem","cJSON_IsNull","cJSON_IsInvalid","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":2}
/*Here's a C++ fuzz driver that utilizes the specified cJSON APIs with the given input data:

*/


// Include cJSON headers

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Check minimum size
    if (size < 1) {
	return 0;
	}
	
    
    // Parse JSON from input data
    const char *json_str = reinterpret_cast<const char*>(data);
    size_t json_length = (size > 0 && json_str[size-1] == '\0') ? size-1 : size;
    
    cJSON *root = cJSON_ParseWithLength(json_str, json_length);
    if (!root) {
	return 0;
	}
	  // Failed to parse
    
    // 1. Check if certain items are null or invalid
    cJSON *widget = cJSON_GetObjectItem(root, "widget");
    if (widget) {
        // Check if debug item is null (it shouldn't be based on our input)
        cJSON *debug = cJSON_GetObjectItem(widget, "debug");
        if (debug) {
            cJSON_IsNull(debug);
            cJSON_IsInvalid(debug);
        }
        
        // Check if window object exists
        cJSON *window = cJSON_GetObjectItem(widget, "window");
        if (window) {
            // Check if title is invalid (it shouldn't be)
            cJSON *title = cJSON_GetObjectItem(window, "title");
            if (title) {
                cJSON_IsInvalid(title);
            }
        }
    }
    
    // 2. Create a new float array
    float float_numbers[] = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f};
    cJSON *float_array = cJSON_CreateFloatArray(float_numbers, 5);
    
    // 3. Add a new object to the root
    cJSON *new_object = cJSON_AddObjectToObject(root, "fuzzed_data");
    if (new_object) {
        // Add the float array to the new object
        cJSON_AddItemToObject(new_object, "float_values", float_array);
        
        // Create a duplicate of the window object
        cJSON *window = cJSON_GetObjectItemCaseSensitive(widget, "window");
        if (window) {
            cJSON *window_duplicate = cJSON_Duplicate(window, 1); // Deep copy
            if (window_duplicate) {
                cJSON_AddItemToObject(new_object, "window_copy", window_duplicate);
                
                // 4. Replace an item via pointer (replace title in duplicate)
                cJSON *title = cJSON_GetObjectItemCaseSensitive(window_duplicate, "title");
                if (title) {
                    cJSON *new_title = cJSON_CreateString("Fuzzed Title");
                    cJSON_ReplaceItemViaPointer(window_duplicate, title, new_title);
                }
                
                // 5. Detach an item via pointer
                cJSON *name = cJSON_GetObjectItemCaseSensitive(window_duplicate, "name");
                if (name) {
                    cJSON *detached_name = cJSON_DetachItemViaPointer(window_duplicate, name);
                    if (detached_name) {
                        cJSON_Delete(detached_name); // Clean up detached item
                    }
                }
            }
        }
    } else {
        // If we couldn't add object, clean up float_array
        cJSON_Delete(float_array);
    }
    
    // 6. Work with array operations (create a temporary array)
    cJSON *temp_array = cJSON_CreateArray();
    if (temp_array) {
        // Add some items to the array
        cJSON_AddItemToArray(temp_array, cJSON_CreateNumber(100));
        cJSON_AddItemToArray(temp_array, cJSON_CreateString("test"));
        cJSON_AddItemToArray(temp_array, cJSON_CreateNull());
        
        // Check if array item at index 2 is null
        cJSON *null_item = cJSON_GetArrayItem(temp_array, 2);
        if (null_item) {
            cJSON_IsNull(null_item);
            cJSON_IsInvalid(null_item);
        }
        
        // Detach item from array
        if (cJSON_GetArraySize(temp_array) > 0) {
            cJSON *detached = cJSON_DetachItemFromArray(temp_array, 0);
            if (detached) {
                cJSON_Delete(detached);
            }
        }
        
        // Clean up temporary array
        cJSON_Delete(temp_array);
    }
    
    // Create output file to write the modified JSON
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Print the modified JSON
        char *printed_json = cJSON_Print(root);
        if (printed_json) {
            fwrite(printed_json, strlen(printed_json), 1, out_file);
            cJSON_free(printed_json);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up
    cJSON_Delete(root);
    
    assert_file_closed(&out_file);
	return 0;
}