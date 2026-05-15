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
//<ID> 139
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["cJSON_Version","cJSON_ParseWithLength","cJSON_IsArray","cJSON_CreateObject","cJSON_Delete","cJSON_AddFalseToObject","cJSON_AddRawToObject","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_CreateArrayReference","cJSON_Compare","cJSON_Delete","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_Duplicate","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Delete","cJSON_CreateString","cJSON_AddItemToObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_Print","cJSON_Minify","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Version","cJSON_ParseWithLength","cJSON_IsArray","cJSON_CreateObject","cJSON_AddFalseToObject","cJSON_AddRawToObject","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_CreateArrayReference","cJSON_Compare","cJSON_Delete","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_Duplicate","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Delete","cJSON_CreateString","cJSON_AddItemToObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_Print","cJSON_Minify","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small for meaningful JSON parsing
    if (size < 2) {
	return 0;
	}
	
    
    // Step 1: Use version API
    const char *version = cJSON_Version();
    (void)version; // Prevent unused variable warning
    
    // Step 2: Parse input as JSON string
    cJSON *parsed = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!parsed) {
	return 0;
	}
	 // Failed to parse
    
    // Step 3: Check if parsed item is an array
    bool is_array = cJSON_IsArray(parsed);
    
    // Step 4: Create a new JSON object
    cJSON *new_obj = cJSON_CreateObject();
    if (!new_obj) {
        cJSON_Delete(parsed);
        return 0;
    }
    
    // Step 5: Add false value to object
    cJSON *false_item = cJSON_AddFalseToObject(new_obj, "fuzz_false");
    (void)false_item; // Prevent unused warning
    
    // Step 6: Add raw JSON to object (using portion of input data)
    // Use safe size to avoid overflow
    size_t raw_len = size < 100 ? size : 100;
    char *raw_str = reinterpret_cast<char*>(malloc(raw_len + 1));
    if (raw_str) {
        memcpy(raw_str, data, raw_len);
        raw_str[raw_len] = '\0';
        cJSON *raw_item = cJSON_AddRawToObject(new_obj, "fuzz_raw", raw_str);
        (void)raw_item; // Prevent unused warning
        free(raw_str);
    }
    
    // Step 7: Array operations if parsed is an array
    if (is_array) {
        // Get array size
        int array_size = cJSON_GetArraySize(parsed);
        
        // Get first array item if exists
        if (array_size > 0) {
            cJSON *first_item = cJSON_GetArrayItem(parsed, 0);
            
            // Create array reference
            cJSON *array_ref = cJSON_CreateArrayReference(first_item);
            
            // Compare original item with reference
            if (array_ref) {
                cJSON_bool compare_result = cJSON_Compare(first_item, array_ref, true);
                (void)compare_result; // Prevent unused warning
                cJSON_Delete(array_ref);
            }
            
            // Add item to new array
            cJSON *temp_array = cJSON_CreateArray();
            if (temp_array && first_item) {
                cJSON_AddItemToArray(temp_array, cJSON_Duplicate(first_item, true));
                
                // Detach item via pointer
                if (temp_array->child) {
                    cJSON *detached = cJSON_DetachItemViaPointer(temp_array, temp_array->child);
                    if (detached) {
                        cJSON_Delete(detached);
                    }
                }
                cJSON_Delete(temp_array);
            }
        }
    }
    
    // Step 8: Detach item from object (case sensitive)
    // First add an item to detach
    cJSON *item_to_detach = cJSON_CreateString("detachable");
    cJSON_AddItemToObject(new_obj, "detach_key", item_to_detach);
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(new_obj, "detach_key");
    if (detached) {
        cJSON_Delete(detached);
    }
    
    // Step 9: Print JSON for output
    char *printed = cJSON_Print(new_obj);
    if (printed) {
        // Step 10: Minify the printed JSON
        cJSON_Minify(printed);
        
        // Write to output file (for demonstration)
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed, strlen(printed), 1, out_file);
            assert_file_closed(&out_file);;
        }
        
        // Step 11: Free printed string
        cJSON_free(printed);
    }
    
    // Step 12: Read input data using FILE* (for demonstration)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        (void)fd; // Prevent unused warning
        
        // Read a small portion
        char buffer[4];
        fread(buffer, 1, sizeof(buffer), in_file);
        assert_file_closed(&in_file);;
    }
    
    // Step 13: Clean up all JSON objects
    cJSON_Delete(parsed);
    cJSON_Delete(new_obj);
    
    assert_file_closed(&in_file);
	return 0;
}