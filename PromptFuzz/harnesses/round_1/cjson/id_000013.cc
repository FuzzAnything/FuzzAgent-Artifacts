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
//<ID> 36
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 1
//<Quality> {"density":33,"unique_branches":{"add_item_to_object":[[2108,48,2108,70,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_IsInvalid","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsObject","cJSON_Delete","cJSON_AddObjectToObject","cJSON_Delete","cJSON_IsNull","cJSON_GetObjectItem","cJSON_IsObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_Duplicate","cJSON_AddItemToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_DetachItemFromArray","cJSON_GetArrayItem","cJSON_CreateNumber","cJSON_ReplaceItemViaPointer","cJSON_GetArrayItem","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetStringValue","cJSON_CreateStringArray","cJSON_AddItemToObject","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_IsInvalid","cJSON_GetObjectItem","cJSON_IsObject","cJSON_AddObjectToObject","cJSON_IsNull","cJSON_GetObjectItem","cJSON_IsObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_Duplicate","cJSON_AddItemToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_DetachItemFromArray","cJSON_GetArrayItem","cJSON_CreateNumber","cJSON_ReplaceItemViaPointer","cJSON_GetArrayItem","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetStringValue","cJSON_CreateStringArray","cJSON_AddItemToObject","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse the input JSON data
    cJSON *root = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!root) {
        return 0; // Invalid JSON, early return
    }
    
    // 2. Check if the root is invalid (using cJSON_IsInvalid)
    if (cJSON_IsInvalid(root)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 3. Get the widget object
    cJSON *widget = cJSON_GetObjectItem(root, "widget");
    if (!widget || !cJSON_IsObject(widget)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 4. Create a new object and add it to widget (using cJSON_AddObjectToObject)
    cJSON *new_obj = cJSON_AddObjectToObject(widget, "new_section");
    if (!new_obj) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 5. Check if the newly added object is null (using cJSON_IsNull)
    if (cJSON_IsNull(new_obj)) {
        // This shouldn't happen since we just created it, but demonstrate API usage
    }
    
    // 6. Get the text object and its data array (if exists)
    cJSON *text = cJSON_GetObjectItem(widget, "text");
    if (text && cJSON_IsObject(text)) {
        // 7. Create a float array (using cJSON_CreateFloatArray)
        float float_numbers[] = {1.1f, 2.2f, 3.3f, 4.4f};
        cJSON *float_array = cJSON_CreateFloatArray(float_numbers, 4);
        if (float_array) {
            // 8. Add the float array to the new object
            cJSON_AddItemToObject(new_obj, "float_data", float_array);
            
            // 9. Duplicate the float array (using cJSON_Duplicate)
            cJSON *duplicated_array = cJSON_Duplicate(float_array, true);
            if (duplicated_array) {
                // Add duplicated array to new object
                cJSON_AddItemToObject(new_obj, "duplicated_floats", duplicated_array);
                
                // 10. Detach an item from array (using cJSON_DetachItemFromArray)
                // First, create an array to work with
                float more_numbers[] = {10.0f, 20.0f, 30.0f};
                cJSON *detachable_array = cJSON_CreateFloatArray(more_numbers, 3);
                if (detachable_array) {
                    // Add to new object
                    cJSON_AddItemToObject(new_obj, "detachable", detachable_array);
                    
                    // Detach the first item
                    cJSON *detached_item = cJSON_DetachItemFromArray(detachable_array, 0);
                    if (detached_item) {
                        // 11. Replace item via pointer (using cJSON_ReplaceItemViaPointer)
                        // Get the second item in the array
                        cJSON *second_item = cJSON_GetArrayItem(detachable_array, 0);
                        if (second_item) {
                            // Create a replacement
                            cJSON *replacement = cJSON_CreateNumber(99.99);
                            if (replacement) {
                                // Replace the second item
                                cJSON_ReplaceItemViaPointer(detachable_array, second_item, replacement);
                            }
                        }
                        
                        // 12. Detach item via pointer (using cJSON_DetachItemViaPointer)
                        // Get the new first item (which was originally at index 2)
                        cJSON *first_item_now = cJSON_GetArrayItem(detachable_array, 0);
                        if (first_item_now) {
                            cJSON *detached_via_pointer = cJSON_DetachItemViaPointer(detachable_array, first_item_now);
                            if (detached_via_pointer) {
                                cJSON_Delete(detached_via_pointer);
                            }
                        }
                        
                        cJSON_Delete(detached_item);
                    }
                }
            }
        }
    }
    
    // 13. Check if the image object exists and has an array-like structure
    cJSON *image = cJSON_GetObjectItem(widget, "image");
    if (image && cJSON_IsObject(image)) {
        // Get the alignment value which might be a string array
        cJSON *alignment = cJSON_GetObjectItem(image, "alignment");
        if (alignment && cJSON_IsString(alignment)) {
            // Create a string array from the alignment value
            const char *align_str = cJSON_GetStringValue(alignment);
            if (align_str) {
                const char *strings[] = {align_str, "left", "right"};
                cJSON *string_array = cJSON_CreateStringArray(strings, 3);
                if (string_array) {
                    cJSON_AddItemToObject(new_obj, "alignments", string_array);
                    
                    // Test detaching from this array too
                    cJSON *detached_string = cJSON_DetachItemFromArray(string_array, 1);
                    if (detached_string) {
                        cJSON_Delete(detached_string);
                    }
                }
            }
        }
    }
    
    // 14. Create output file and write the modified JSON
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        char *json_str = cJSON_Print(root);
        if (json_str) {
            fwrite(json_str, strlen(json_str), 1, out_file);
            cJSON_free(json_str);
        }
        
        // Get file descriptor if needed (demonstrating usage)
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd; // Suppress unused variable warning
        
        assert_file_closed(&out_file);;
    }
    
    // 15. Clean up all allocated resources
    cJSON_Delete(root);
    
    assert_file_closed(&out_file);
	return 0;
}