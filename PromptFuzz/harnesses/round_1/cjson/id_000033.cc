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
//<ID> 126
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 1
//<Quality> {"density":30,"unique_branches":{"cJSON_IsFalse":[[2984,9,2984,21,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_GetObjectItemCaseSensitive","cJSON_IsFalse","cJSON_CreateObjectReference","cJSON_GetObjectItemCaseSensitive","cJSON_CreateTrue","cJSON_CreateRaw","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_CreateArrayReference","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_Duplicate","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_GetObjectItemCaseSensitive","cJSON_IsFalse","cJSON_CreateObjectReference","cJSON_GetObjectItemCaseSensitive","cJSON_CreateTrue","cJSON_CreateRaw","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_CreateArrayReference","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_Duplicate","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


// Include cJSON headers

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a null-terminated copy of input data for string APIs
    char *input_str = static_cast<char*>(malloc(size + 1));
    if (!input_str) {
	return 0;
	}
	
    memcpy(input_str, data, size);
    input_str[size] = '\0';
    
    // Parse the input JSON
    cJSON *root = cJSON_ParseWithLength(input_str, size);
    if (!root) {
        free(input_str);
        return 0;
    }
    
    // 1. Check if "debug" field exists and is not false
    cJSON *debug_item = cJSON_GetObjectItemCaseSensitive(root, "debug");
    cJSON_bool is_not_false = cJSON_IsFalse(debug_item) ? 0 : 1;
    
    // 2. Create object references
    cJSON *widget_ref = cJSON_CreateObjectReference(
        cJSON_GetObjectItemCaseSensitive(root, "widget"));
    
    // 3. Create a true boolean
    cJSON *true_item = cJSON_CreateTrue();
    
    // 4. Create raw JSON
    cJSON *raw_item = cJSON_CreateRaw("{\"raw\":\"data\"}");
    
    // 5. Create array reference
    cJSON *window = cJSON_GetObjectItemCaseSensitive(root, "widget");
    if (window) {
        window = cJSON_GetObjectItemCaseSensitive(window, "window");
    }
    cJSON *array_ref = cJSON_CreateArrayReference(window);
    
    // 6. Create an array and add items
    cJSON *new_array = cJSON_CreateArray();
    if (is_not_false) {
        // Add various items to the array
        cJSON_AddItemToArray(new_array, cJSON_Duplicate(widget_ref, 1));
        cJSON_AddItemToArray(new_array, cJSON_Duplicate(true_item, 1));
        cJSON_AddItemToArray(new_array, cJSON_Duplicate(raw_item, 1));
        cJSON_AddItemToArray(new_array, cJSON_Duplicate(array_ref, 1));
    }
    
    // 7. Detach an item from array via pointer
    if (cJSON_GetArraySize(new_array) > 0) {
        cJSON *first_item = cJSON_GetArrayItem(new_array, 0);
        cJSON *detached = cJSON_DetachItemViaPointer(new_array, first_item);
        if (detached) {
            cJSON_Delete(detached);
        }
    }
    
    // 8. Process output file if needed
    char *printed = cJSON_Print(new_array);
    if (printed) {
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed, strlen(printed), 1, out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(printed);
    }
    
    // Cleanup
    cJSON_Delete(root);
    cJSON_Delete(widget_ref);
    cJSON_Delete(true_item);
    cJSON_Delete(raw_item);
    cJSON_Delete(array_ref);
    cJSON_Delete(new_array);
    free(input_str);
    
    return 0;
}