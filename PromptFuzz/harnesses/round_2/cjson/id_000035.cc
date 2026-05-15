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
//<ID> 275
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":28,"unique_branches":{"cJSON_IsRaw":[[3064,9,3064,21,0,0,4,0]]},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateNumber","cJSON_CreateTrue","cJSON_CreateString","cJSON_GetObjectItemCaseSensitive","cJSON_ReplaceItemViaPointer","cJSON_Delete","cJSON_CreateTrue","cJSON_IsTrue","cJSON_AddItemToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_SetValuestring","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_GetArraySize","cJSON_ReplaceItemInArray","cJSON_CreateNumber","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_Delete","cJSON_IsRaw","cJSON_GetObjectItemCaseSensitive","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_CreateNumber","cJSON_Delete","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateNumber","cJSON_CreateTrue","cJSON_CreateString","cJSON_GetObjectItemCaseSensitive","cJSON_ReplaceItemViaPointer","cJSON_CreateTrue","cJSON_IsTrue","cJSON_AddItemToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_SetValuestring","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_GetArraySize","cJSON_ReplaceItemInArray","cJSON_CreateNumber","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_Delete","cJSON_IsRaw","cJSON_GetObjectItemCaseSensitive","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_CreateNumber","cJSON_Delete","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Step 1: Parse the input JSON data
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts((const char *)data, size, &parse_end, false);
    if (root == NULL) {
        return 0;
    }
    
    // Step 2: Check if root is an object
    if (cJSON_IsObject(root)) {
        // Step 3: Get object item case sensitively
        cJSON *image = cJSON_GetObjectItemCaseSensitive(root, "Image");
        if (image != NULL && cJSON_IsObject(image)) {
            // Step 4: Create replacement items
            cJSON *new_number = cJSON_CreateNumber(1024.5);
            cJSON *new_true = cJSON_CreateTrue();
            cJSON *new_string = cJSON_CreateString("Modified Title");
            
            // Step 5: Replace items in object
            cJSON *width_item = cJSON_GetObjectItemCaseSensitive(image, "Width");
            if (width_item != NULL) {
                // Replace using pointer
                cJSON_ReplaceItemViaPointer(image, width_item, new_number);
                // Don't delete width_item, it's taken over by the replacement
            } else {
                cJSON_Delete(new_number);
            }
            
            // Step 6: Check if item is true
            cJSON *dummy_true = cJSON_CreateTrue();
            if (cJSON_IsTrue(dummy_true)) {
                // Add to object
                cJSON_AddItemToObject(image, "DummyTrue", dummy_true);
            }
            
            // Step 7: Check if item is string
            cJSON *title_item = cJSON_GetObjectItemCaseSensitive(image, "Title");
            if (title_item != NULL && cJSON_IsString(title_item)) {
                // Set new value string
                char *old_valuestring = title_item->valuestring;
                char *new_val = cJSON_SetValuestring(title_item, "Completely New Title");
                (void)new_val; // Suppress unused warning
            }
            
            // Step 8: Create and manipulate arrays
            cJSON *ids = cJSON_GetObjectItemCaseSensitive(image, "IDs");
            if (ids != NULL && cJSON_IsArray(ids)) {
                // Create new array
                cJSON *new_array = cJSON_CreateArray();
                
                // Add items to array
                cJSON_AddItemToArray(new_array, cJSON_CreateNumber(1));
                cJSON_AddItemToArray(new_array, cJSON_CreateNumber(2));
                cJSON_AddItemToArray(new_array, cJSON_CreateNumber(3));
                
                // Replace item in array
                if (cJSON_GetArraySize(ids) > 0) {
                    cJSON_ReplaceItemInArray(ids, 0, cJSON_CreateNumber(999));
                }
                
                // Create double array
                double double_nums[] = {1.1, 2.2, 3.3};
                cJSON *double_array = cJSON_CreateDoubleArray(double_nums, 3);
                cJSON_AddItemToObject(image, "DoubleArray", double_array);
                
                // Cleanup
                cJSON_Delete(new_array);
            }
            
            // Step 9: Check if item is raw (won't be in our test data)
            if (cJSON_IsRaw(title_item)) {
                // This won't execute since title is a string, not raw
            }
            
            // Step 10: Replace item in object case sensitive
            cJSON *height_item = cJSON_GetObjectItemCaseSensitive(image, "Height");
            if (height_item != NULL) {
                cJSON_ReplaceItemInObjectCaseSensitive(image, "Height", cJSON_CreateNumber(700));
            }
            
            // Cleanup created items
            cJSON_Delete(new_true);
            cJSON_Delete(new_string);
        }
        
        // Step 11: Print the modified JSON
        char *printed = cJSON_Print(root);
        if (printed != NULL) {
            // We could write to a file if needed
            FILE *out_file = fopen("output_file", "wb");
            if (out_file != NULL) {
                fwrite(printed, strlen(printed), 1, out_file);
                assert_file_closed(&out_file);;
            }
            cJSON_free(printed);
        }
    }
    
    // Step 12: Final cleanup
    cJSON_Delete(root);
    
    return 0;
}