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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 13.5, nr_unique_branch: 0
//<Quality> {"density":27,"unique_branches":{},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_CreateObject","cJSON_CreateNumber","cJSON_CreateTrue","cJSON_CreateRaw","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_IsTrue","cJSON_GetObjectItem","cJSON_IsRaw","cJSON_GetObjectItem","cJSON_CreateArray","cJSON_AddItemToObject","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_ReplaceItemInArray","cJSON_CreateNumber","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_CreateNumber","cJSON_GetObjectItem","cJSON_ReplaceItemViaPointer","cJSON_CreateFalse","cJSON_Print","cJSON_Delete","cJSON_IsObject","cJSON_Delete","cJSON_CreateNumber","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_AddItemToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsTrue","cJSON_CreateFalse","cJSON_CreateTrue","cJSON_ReplaceItemViaPointer","cJSON_CreateRaw","cJSON_AddItemToObject","cJSON_IsRaw","cJSON_GetObjectItem","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateArray","cJSON_AddItemToObject","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_ReplaceItemInArray","cJSON_CreateNumber","cJSON_Print","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_IsObject","cJSON_CreateNumber","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsTrue","cJSON_CreateTrue","cJSON_CreateFalse","cJSON_ReplaceItemViaPointer","cJSON_CreateRaw","cJSON_AddItemToObject","cJSON_IsRaw","cJSON_GetObjectItem","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateArray","cJSON_AddItemToObject","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_ReplaceItemInArray","cJSON_CreateNumber","cJSON_Print","cJSON_Delete"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Parse the input data as JSON
    const char *parse_end = nullptr;
    cJSON *parsed = cJSON_ParseWithLengthOpts(
        reinterpret_cast<const char*>(data), 
        size, 
        &parse_end, 
        0
    );
    
    if (parsed == nullptr) {
        // If parsing fails, try to create a new object and use the APIs
        cJSON *root = cJSON_CreateObject();
        
        // Create some items
        cJSON *num_item = cJSON_CreateNumber(42.5);
        cJSON *true_item = cJSON_CreateTrue();
        cJSON *raw_item = cJSON_CreateRaw("\"raw data\"");
        
        // Add items to object
        cJSON_AddItemToObject(root, "number", num_item);
        cJSON_AddItemToObject(root, "boolean", true_item);
        cJSON_AddItemToObject(root, "raw", raw_item);
        
        // Check item types
        cJSON_bool is_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "boolean"));
        cJSON_bool is_raw = cJSON_IsRaw(cJSON_GetObjectItem(root, "raw"));
        
        // Create an array
        cJSON *array = cJSON_CreateArray();
        cJSON_AddItemToObject(root, "array", array);
        
        // Add some items to array
        for (int i = 0; i < 3; i++) {
            cJSON_AddItemToArray(array, cJSON_CreateNumber(i * 10.0));
        }
        
        // Replace item in array
        cJSON_ReplaceItemInArray(array, 1, cJSON_CreateNumber(999.0));
        
        // Replace item in object (case sensitive)
        cJSON_ReplaceItemInObjectCaseSensitive(root, "number", cJSON_CreateNumber(100.0));
        
        // Get item pointer and replace via pointer
        cJSON *bool_item = cJSON_GetObjectItem(root, "boolean");
        if (bool_item != nullptr) {
            cJSON_ReplaceItemViaPointer(root, bool_item, cJSON_CreateFalse());
        }
        
        // Print the result
        char *printed = cJSON_Print(root);
        if (printed != nullptr) {
            free(printed);
        }
        
        cJSON_Delete(root);
        return 0;
    }
    
    // If parsing succeeded, use the parsed object
    cJSON *root = parsed;
    
    // Check if root is an object
    if (!cJSON_IsObject(root)) {
        // If not an object, delete and exit
        cJSON_Delete(root);
        return 0;
    }
    
    // Create a new number item
    cJSON *new_num = cJSON_CreateNumber(3.14159);
    
    // Check for existing items
    cJSON *frame_rate = cJSON_GetObjectItemCaseSensitive(root, "frame rate");
    if (frame_rate != nullptr && cJSON_IsNumber(frame_rate)) {
        // Replace frame rate with new number
        cJSON_ReplaceItemInObjectCaseSensitive(root, "frame rate", new_num);
    } else {
        // Add new number item to object
        cJSON_AddItemToObject(root, "new_field", new_num);
    }
    
    // Check for true value
    cJSON *interlace = cJSON_GetObjectItemCaseSensitive(root, "interlace");
    if (interlace != nullptr) {
        cJSON_bool is_true = cJSON_IsTrue(interlace);
        
        // Create a replacement based on whether it's true or not
        cJSON *replacement = is_true ? cJSON_CreateFalse() : cJSON_CreateTrue();
        
        // Replace via pointer
        cJSON_ReplaceItemViaPointer(root, interlace, replacement);
    }
    
    // Check for raw items (unlikely in standard JSON, but we can create one)
    cJSON *raw_item = cJSON_CreateRaw("\"custom raw\"");
    cJSON_AddItemToObject(root, "raw_data", raw_item);
    
    // Check if the added raw item is indeed raw
    cJSON_bool is_raw = cJSON_IsRaw(cJSON_GetObjectItem(root, "raw_data"));
    
    // Handle format sub-object
    cJSON *format = cJSON_GetObjectItemCaseSensitive(root, "format");
    if (format != nullptr && cJSON_IsObject(format)) {
        // Create an array inside format
        cJSON *new_array = cJSON_CreateArray();
        cJSON_AddItemToObject(format, "dimensions", new_array);
        
        // Add some items to array
        cJSON_AddItemToArray(new_array, cJSON_CreateNumber(1920.0));
        cJSON_AddItemToArray(new_array, cJSON_CreateNumber(1080.0));
        
        // Replace item in array
        cJSON_ReplaceItemInArray(new_array, 0, cJSON_CreateNumber(3840.0));
    }
    
    // Print the modified JSON
    char *printed = cJSON_Print(root);
    if (printed != nullptr) {
        // Write to output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file != nullptr) {
            fwrite(printed, 1, strlen(printed), out_file);
            assert_file_closed(&out_file);;
        }
        free(printed);
    }
    
    // Clean up
    cJSON_Delete(root);
    
    return 0;
}