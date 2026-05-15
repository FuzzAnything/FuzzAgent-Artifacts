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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"get_object_item":[[1955,9,1955,25,0,0,4,0]]},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsTrue","cJSON_CreateNumber","cJSON_ReplaceItemViaPointer","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsRaw","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateString","cJSON_CreateString","cJSON_ReplaceItemInArray","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_CreateNumber","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_Print","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsTrue","cJSON_CreateNumber","cJSON_ReplaceItemViaPointer","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsRaw","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateString","cJSON_CreateString","cJSON_ReplaceItemInArray","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_CreateNumber","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0 || data == nullptr) {
        return 0;
    }

    // Create a null-terminated string from the input data
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, data, size);
    json_str[size] = '\0';

    // Parse the JSON using cJSON_ParseWithLengthOpts
    const char *parse_end = nullptr;
    cJSON *root = cJSON_ParseWithLengthOpts(json_str, size, &parse_end, false);
    
    if (!root) {
        free(json_str);
        return 0;
    }

    // Check if various types exist in the parsed JSON
    cJSON *format_obj = cJSON_GetObjectItemCaseSensitive(root, "format");
    if (format_obj && cJSON_IsObject(format_obj)) {
        // Check if "interlace" is true
        cJSON *interlace_item = cJSON_GetObjectItemCaseSensitive(format_obj, "interlace");
        if (interlace_item && cJSON_IsTrue(interlace_item)) {
            // Replace the true value with a new number
            cJSON *new_number = cJSON_CreateNumber(42.5);
            if (new_number) {
                cJSON_ReplaceItemViaPointer(format_obj, interlace_item, new_number);
                // old item (interlace_item) is now detached and should be deleted
                cJSON_Delete(interlace_item);
            }
        }

        // Try to check for raw type (though our input doesn't have raw)
        cJSON *frame_rate = cJSON_GetObjectItemCaseSensitive(format_obj, "frame rate");
        if (frame_rate && !cJSON_IsRaw(frame_rate)) {
            // This demonstrates the API usage even if condition is false
        }

        // Add a new item to the format object
        cJSON *new_item = cJSON_CreateNumber(100);
        if (new_item) {
            cJSON_AddItemToObject(format_obj, "new_field", new_item);
        }
    }

    // Handle array operations if "name" is treated as array
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    if (name_item && cJSON_IsString(name_item)) {
        // Create a temporary array for demonstration
        cJSON *temp_array = cJSON_CreateArray();
        if (temp_array) {
            cJSON_AddItemToArray(temp_array, cJSON_CreateString("item1"));
            cJSON_AddItemToArray(temp_array, cJSON_CreateString("item2"));
            cJSON_AddItemToArray(temp_array, cJSON_CreateString("item3"));
            
            // Replace an item in the array
            cJSON *replacement = cJSON_CreateString("replaced");
            if (replacement) {
                cJSON_ReplaceItemInArray(temp_array, 1, replacement);
            }
            
            cJSON_Delete(temp_array);
        }
    }

    // Replace an item in object (case sensitive)
    cJSON *width_item = cJSON_GetObjectItemCaseSensitive(format_obj, "width");
    if (width_item) {
        cJSON *new_width = cJSON_CreateNumber(2560);
        if (new_width) {
            cJSON_ReplaceItemInObjectCaseSensitive(format_obj, "width", new_width);
        }
    }

    // Print the modified JSON
    char *printed_json = cJSON_Print(root);
    if (printed_json) {
        // Create output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed_json, strlen(printed_json), 1, out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(printed_json);
    }

    // Clean up
    cJSON_Delete(root);
    free(json_str);

    return 0;
}