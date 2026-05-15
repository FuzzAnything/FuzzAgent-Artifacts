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
//<ID> 64
//<Prompt> []
/*<Combination>: [
*/
//<score> 33, nr_unique_branch: 2
//<Quality> {"density":33,"unique_branches":{"cJSON_SetValuestring":[[457,16,457,58,0,0,4,1],[457,62,457,104,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_CreateObject","cJSON_AddItemToObject","cJSON_AddNumberToObject","cJSON_AddNumberToObject","cJSON_AddStringToObject","cJSON_CreateObject","cJSON_AddItemToObject","cJSON_AddStringToObject","cJSON_AddNumberToObject","cJSON_AddStringToObject","cJSON_CreateArray","cJSON_AddItemToObject","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_CreateArray","cJSON_Delete","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_CreateObject","cJSON_AddTrueToObject","cJSON_IsTrue","cJSON_AddItemReferenceToArray","cJSON_CreateObject","cJSON_AddItemReferenceToObject","cJSON_AddItemToArray","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetStringValue","cJSON_SetValuestring","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetStringValue","cJSON_CreateString","cJSON_AddItemToArray","cJSON_Print","cJSON_Print","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_CreateObject","cJSON_AddItemToObject","cJSON_AddNumberToObject","cJSON_AddNumberToObject","cJSON_AddStringToObject","cJSON_CreateObject","cJSON_AddItemToObject","cJSON_AddStringToObject","cJSON_AddNumberToObject","cJSON_AddStringToObject","cJSON_CreateArray","cJSON_AddItemToObject","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_CreateArray","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_CreateObject","cJSON_AddTrueToObject","cJSON_IsTrue","cJSON_AddItemReferenceToArray","cJSON_CreateObject","cJSON_AddItemReferenceToObject","cJSON_AddItemToArray","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetStringValue","cJSON_SetValuestring","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetStringValue","cJSON_CreateString","cJSON_AddItemToArray","cJSON_Print","cJSON_Print","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse input JSON data using cJSON_ParseWithLength
    // The input data is treated as a null-terminated string for parsing
    char *json_string = static_cast<char*>(malloc(size + 1));
    if (!json_string) {
	return 0;
	}
	
    
    memcpy(json_string, data, size);
    json_string[size] = '\0'; // Null-terminate for cJSON parsing
    
    cJSON *root = cJSON_ParseWithLength(json_string, size);
    free(json_string);
    
    if (!root) {
        // If parsing fails, we can still demonstrate API usage with new objects
        root = cJSON_CreateObject();
        if (!root) {
	return 0;
	}
	
        
        // Create a simple object structure to work with
        cJSON *image = cJSON_CreateObject();
        cJSON_AddItemToObject(root, "Image", image);
        cJSON_AddNumberToObject(image, "Width", 800);
        cJSON_AddNumberToObject(image, "Height", 600);
        cJSON_AddStringToObject(image, "Title", "View from 15th Floor");
        
        cJSON *thumbnail = cJSON_CreateObject();
        cJSON_AddItemToObject(image, "Thumbnail", thumbnail);
        cJSON_AddStringToObject(thumbnail, "Url", "http://www.example.com/image/481989943");
        cJSON_AddNumberToObject(thumbnail, "Height", 125);
        cJSON_AddStringToObject(thumbnail, "Width", "100");
        
        cJSON *ids = cJSON_CreateArray();
        cJSON_AddItemToObject(image, "IDs", ids);
        cJSON_AddItemToArray(ids, cJSON_CreateNumber(116));
        cJSON_AddItemToArray(ids, cJSON_CreateNumber(943));
        cJSON_AddItemToArray(ids, cJSON_CreateNumber(234));
        cJSON_AddItemToArray(ids, cJSON_CreateNumber(38793));
    }
    
    // 2. Create a new array using cJSON_CreateArray()
    cJSON *new_array = cJSON_CreateArray();
    if (!new_array) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 3. Check if "Width" in Image is a number using cJSON_IsNumber()
    cJSON *image = cJSON_GetObjectItem(root, "Image");
    if (image) {
        cJSON *width = cJSON_GetObjectItem(image, "Width");
        if (width && cJSON_IsNumber(width)) {
            // Width is indeed a number - we could use this information
            // For demonstration, we'll add it to our new array
            cJSON *width_copy = cJSON_Duplicate(width, 1);
            if (width_copy) {
                cJSON_AddItemToArray(new_array, width_copy);
            }
        }
    }
    
    // 4. Add a true value to an object using cJSON_AddTrueToObject()
    cJSON *new_object = cJSON_CreateObject();
    if (new_object) {
        // Add a true value with name "fuzzer_added_true"
        cJSON *added_true = cJSON_AddTrueToObject(new_object, "fuzzer_added_true");
        
        // 5. Verify the added item is true using cJSON_IsTrue()
        if (added_true && cJSON_IsTrue(added_true)) {
            // Successfully added and verified true value
            
            // 6. Add a reference to this true value in the array using cJSON_AddItemReferenceToObject()
            // Note: cJSON_AddItemReferenceToObject adds to an object, not an array
            // We'll use cJSON_AddItemReferenceToArray instead, which is the array equivalent
            cJSON_AddItemReferenceToArray(new_array, added_true);
            
            // Alternatively, we can add the reference to an object
            cJSON *ref_object = cJSON_CreateObject();
            if (ref_object) {
                cJSON_AddItemReferenceToObject(ref_object, "true_reference", added_true);
                cJSON_AddItemToArray(new_array, ref_object);
            }
        }
    }
    
    // 7. Demonstrate cJSON_SetValuestring() on a string item
    if (image) {
        cJSON *title = cJSON_GetObjectItem(image, "Title");
        if (title && cJSON_IsString(title)) {
            // Create a modified title
            char *modified_title = static_cast<char*>(malloc(50));
            if (modified_title) {
                snprintf(modified_title, 50, "Modified: %s (fuzzed)", cJSON_GetStringValue(title));
                cJSON_SetValuestring(title, modified_title);
                free(modified_title);
            }
        }
    }
    
    // 8. For demonstration, let's also check if we can access the Thumbnail Width
    // which is stored as a string in the original JSON
    if (image) {
        cJSON *thumbnail = cJSON_GetObjectItem(image, "Thumbnail");
        if (thumbnail) {
            cJSON *thumb_width = cJSON_GetObjectItem(thumbnail, "Width");
            if (thumb_width) {
                // This should be a string, not a number
                if (cJSON_IsString(thumb_width)) {
                    // We can convert it if needed
                    const char *width_str = cJSON_GetStringValue(thumb_width);
                    // For demonstration, add it to our array
                    cJSON *str_item = cJSON_CreateString(width_str);
                    if (str_item) {
                        cJSON_AddItemToArray(new_array, str_item);
                    }
                }
            }
        }
    }
    
    // 9. Clean up: Print the modified JSON to demonstrate changes
    char *printed = cJSON_Print(root);
    if (printed) {
        // We could write this to a file if needed
        free(printed);
    }
    
    // 10. Also print the new array we created
    char *array_printed = cJSON_Print(new_array);
    if (array_printed) {
        free(array_printed);
    }
    
    // 11. Clean up all allocated resources
    cJSON_Delete(new_array);
    if (new_object) {
        cJSON_Delete(new_object);
    }
    cJSON_Delete(root);
    
    return 0;
}