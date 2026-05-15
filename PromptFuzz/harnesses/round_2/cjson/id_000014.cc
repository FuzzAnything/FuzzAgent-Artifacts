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
//<ID> 45
//<Prompt> []
/*<Combination>: [
*/
//<score> 11.333333, nr_unique_branch: 0
//<Quality> {"density":34,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_Delete","cJSON_CreateDoubleArray","cJSON_AddItemToObjectCS","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_CreateNull","cJSON_AddItemToObjectCS","cJSON_IsNull","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToObjectCS","cJSON_DeleteItemFromArray","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_SetValuestring","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_SetValuestring","cJSON_Print","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateDoubleArray","cJSON_AddItemToObjectCS","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_CreateNull","cJSON_AddItemToObjectCS","cJSON_IsNull","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToObjectCS","cJSON_DeleteItemFromArray","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_SetValuestring","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_SetValuestring","cJSON_Print","cJSON_Delete"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Parse input JSON using the provided data
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
	return 0;
	}
	
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *root = cJSON_Parse(json_str);
    free(json_str);
    
    if (!root) {
	return 0;
	}
	
    
    // Get the widget object
    cJSON *widget = cJSON_GetObjectItemCaseSensitive(root, "widget");
    if (!widget || !cJSON_IsObject(widget)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 1. Create a double array
    double numbers[] = {1.5, 2.7, 3.14, 4.0, 5.99};
    cJSON *double_array = cJSON_CreateDoubleArray(numbers, 5);
    
    // 2. Add the double array to the widget object using AddItemToObjectCS
    // (case-sensitive version)
    cJSON_AddItemToObjectCS(widget, "newDoubleArray", double_array);
    
    // 3. Check if certain items are numbers using cJSON_IsNumber
    cJSON *width = cJSON_GetObjectItemCaseSensitive(widget, "window");
    if (width && cJSON_IsObject(width)) {
        cJSON *width_value = cJSON_GetObjectItemCaseSensitive(width, "width");
        if (width_value && cJSON_IsNumber(width_value)) {
            // width is a number - confirmed
        }
    }
    
    // 4. Check if certain items are null using cJSON_IsNull
    // First add a null item to test
    cJSON *null_test = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(widget, "testNull", null_test);
    
    // Verify it's null
    if (cJSON_IsNull(null_test)) {
        // This is indeed null
    }
    
    // 5. Delete an item from an array (if we have one)
    // Get or create an array to work with
    cJSON *text = cJSON_GetObjectItemCaseSensitive(widget, "text");
    if (!text || !cJSON_IsObject(text)) {
        // Create a simple array for demonstration
        cJSON *demo_array = cJSON_CreateArray();
        cJSON_AddItemToArray(demo_array, cJSON_CreateNumber(1));
        cJSON_AddItemToArray(demo_array, cJSON_CreateNumber(2));
        cJSON_AddItemToArray(demo_array, cJSON_CreateNumber(3));
        cJSON_AddItemToObjectCS(widget, "demoArray", demo_array);
        
        // Delete the middle item (index 1)
        cJSON_DeleteItemFromArray(demo_array, 1);
    }
    
    // 6. Set a value string using cJSON_SetValuestring
    // Find a string item to modify
    cJSON *debug = cJSON_GetObjectItemCaseSensitive(widget, "debug");
    if (debug && cJSON_IsString(debug)) {
        cJSON_SetValuestring(debug, "modified");
    }
    
    // 7. Let's also test with the image object's name field
    cJSON *image = cJSON_GetObjectItemCaseSensitive(widget, "image");
    if (image && cJSON_IsObject(image)) {
        cJSON *name = cJSON_GetObjectItemCaseSensitive(image, "name");
        if (name && cJSON_IsString(name)) {
            cJSON_SetValuestring(name, "new_image_name");
        }
    }
    
    // Create output file and write modified JSON
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        char *printed = cJSON_Print(root);
        if (printed) {
            fwrite(printed, strlen(printed), 1, out_file);
            free(printed);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up - all items are owned by root
    cJSON_Delete(root);
    
    assert_file_closed(&out_file);
	return 0;
}