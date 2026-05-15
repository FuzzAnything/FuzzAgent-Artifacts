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
//<score> 35, nr_unique_branch: 1
//<Quality> {"density":35,"unique_branches":{"cJSON_CreateNumber":[[2514,13,2514,27,0,0,4,0]]},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_IsObject","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsObject","cJSON_Delete","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_CreateTrue","cJSON_IsTrue","cJSON_AddItemToObject","cJSON_Delete","cJSON_CreateRaw","cJSON_IsRaw","cJSON_AddItemToObject","cJSON_Delete","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_CreateArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_GetArraySize","cJSON_CreateString","cJSON_ReplaceItemInArray","cJSON_AddItemToObject","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_CreateNumber","cJSON_GetNumberValue","cJSON_ReplaceItemViaPointer","cJSON_AddBoolToObject","cJSON_Print","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_CreateTrue","cJSON_IsTrue","cJSON_AddItemToObject","cJSON_CreateRaw","cJSON_IsRaw","cJSON_AddItemToObject","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_CreateArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_GetArraySize","cJSON_CreateString","cJSON_ReplaceItemInArray","cJSON_AddItemToObject","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_CreateNumber","cJSON_GetNumberValue","cJSON_ReplaceItemViaPointer","cJSON_AddBoolToObject","cJSON_Print","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Too small to be meaningful JSON
    
    // Step 1: Parse input JSON using cJSON_ParseWithLengthOpts
    const char *parse_end = nullptr;
    cJSON *json = cJSON_ParseWithLengthOpts(reinterpret_cast<const char*>(data), size, &parse_end, false);
    if (!json) {
	return 0;
	}
	
    
    // Step 2: Check if root is an object using cJSON_IsObject
    if (!cJSON_IsObject(json)) {
        cJSON_Delete(json);
        return 0;
    }
    
    // Step 3: Get specific object items using cJSON_GetObjectItem
    cJSON *widget = cJSON_GetObjectItem(json, "widget");
    if (!widget || !cJSON_IsObject(widget)) {
        cJSON_Delete(json);
        return 0;
    }
    
    cJSON *debug = cJSON_GetObjectItem(widget, "debug");
    cJSON *window = cJSON_GetObjectItem(widget, "window");
    cJSON *image = cJSON_GetObjectItem(widget, "image");
    cJSON *text = cJSON_GetObjectItem(widget, "text");
    
    // Step 4: Use cJSON_IsTrue on boolean values (checking non-existent boolean)
    // Create a boolean object first to test cJSON_IsTrue
    cJSON *test_bool = cJSON_CreateTrue();
    if (cJSON_IsTrue(test_bool)) {
        // Add this boolean to the widget object using cJSON_AddItemToObject
        cJSON_AddItemToObject(widget, "test_bool", test_bool);
    } else {
        cJSON_Delete(test_bool);
    }
    
    // Step 5: Create and add a raw JSON value using cJSON_IsRaw
    cJSON *raw_json = cJSON_CreateRaw("{\"raw\": \"value\"}");
    if (cJSON_IsRaw(raw_json)) {
        cJSON_AddItemToObject(widget, "raw_data", raw_json);
    } else {
        cJSON_Delete(raw_json);
    }
    
    // Step 6: Create a number using cJSON_CreateNumber and add to object
    double random_num = static_cast<double>(size) / 100.0;
    cJSON *new_number = cJSON_CreateNumber(random_num);
    cJSON_AddItemToObject(widget, "fuzz_number", new_number);
    
    // Step 7: Create an array and manipulate it
    cJSON *new_array = cJSON_CreateArray();
    
    // Add items to array
    for (int i = 0; i < 3; i++) {
        cJSON *array_item = cJSON_CreateNumber(i * 10.0);
        cJSON_AddItemToArray(new_array, array_item);
    }
    
    // Get array size using cJSON_GetArraySize
    int array_size = cJSON_GetArraySize(new_array);
    
    // Replace an item in array using cJSON_ReplaceItemInArray if array has items
    if (array_size > 0) {
        cJSON *replacement = cJSON_CreateString("replaced");
        cJSON_ReplaceItemInArray(new_array, 0, replacement);
    }
    
    // Add array to widget object
    cJSON_AddItemToObject(widget, "fuzz_array", new_array);
    
    // Step 8: Check if certain items are numbers using cJSON_IsNumber
    if (window && cJSON_IsObject(window)) {
        cJSON *width = cJSON_GetObjectItem(window, "width");
        if (width && cJSON_IsNumber(width)) {
            // Create a modified width using pointer replacement
            cJSON *new_width = cJSON_CreateNumber(cJSON_GetNumberValue(width) + 100);
            cJSON_ReplaceItemViaPointer(window, width, new_width);
        }
    }
    
    // Step 9: Create and add a boolean object using cJSON_AddBoolToObject
    cJSON_AddBoolToObject(widget, "fuzz_flag", (size % 2) == 0);
    
    // Step 10: Print the modified JSON using cJSON_Print
    char *printed_json = cJSON_Print(json);
    if (printed_json) {
        // Write to output file (for debugging purposes)
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed_json, strlen(printed_json), 1, out_file);
            assert_file_closed(&out_file);;
        }
        free(printed_json);
    }
    
    // Step 11: Clean up using cJSON_Delete
    cJSON_Delete(json);
    
    return 0;
}