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
//<ID> 59
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"cJSON_Compare":[[3123,17,3123,60,0,0,4,0]]},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_CreateBool","cJSON_IsObject","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_Delete","cJSON_Delete","cJSON_CreateNull","cJSON_IsString","cJSON_SetValuestring","cJSON_IsString","cJSON_SetValuestring","cJSON_PrintPreallocated","cJSON_Duplicate","cJSON_Compare","cJSON_Compare","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_CreateBool","cJSON_IsObject","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_CreateNull","cJSON_IsString","cJSON_IsString","cJSON_SetValuestring","cJSON_PrintPreallocated","cJSON_Duplicate","cJSON_Compare","cJSON_Compare","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 1: Parse input data using cJSON_ParseWithLengthOpts
    const char **parse_end = nullptr;
    cJSON_bool require_null_terminated = 0;
    cJSON *parsed_json = cJSON_ParseWithLengthOpts(
        reinterpret_cast<const char*>(data), 
        size, 
        parse_end, 
        require_null_terminated
    );
    
    if (!parsed_json) {
        return 0; // Early return if parsing fails
    }
    
    // Step 2: Create a boolean item
    cJSON *bool_item = cJSON_CreateBool(1);
    
    // Step 3: Replace an item in the parsed object (if it's an object)
    if (cJSON_IsObject(parsed_json)) {
        // Find first child to replace
        cJSON *first_child = parsed_json->child;
        if (first_child && first_child->string) {
            cJSON_ReplaceItemInObjectCaseSensitive(
                parsed_json, 
                first_child->string, 
                bool_item
            );
            // bool_item is now owned by parsed_json, don't delete separately
        } else {
            cJSON_Delete(bool_item);
        }
    } else {
        cJSON_Delete(bool_item);
    }
    
    // Step 4: Create a null item for comparison
    cJSON *null_item = cJSON_CreateNull();
    
    // Step 5: Set valuestring on a string item if exists
    if (cJSON_IsString(parsed_json)) {
        cJSON_SetValuestring(parsed_json, "modified_value");
    } else {
        // Try to find a string item in the structure
        cJSON *current = parsed_json->child;
        while (current) {
            if (cJSON_IsString(current)) {
                cJSON_SetValuestring(current, "modified_value");
                break;
            }
            current = current->next;
        }
    }
    
    // Step 6: Create a buffer for printing
    const int buffer_size = 4096;
    char *print_buffer = static_cast<char*>(malloc(buffer_size));
    if (print_buffer) {
        // Step 7: Print to preallocated buffer
        cJSON_PrintPreallocated(parsed_json, print_buffer, buffer_size, 1);
        
        // Step 8: Create a duplicate for comparison
        cJSON *duplicate = cJSON_Duplicate(parsed_json, 1);
        
        if (duplicate) {
            // Step 9: Compare original and duplicate
            cJSON_bool are_equal = cJSON_Compare(parsed_json, duplicate, 1);
            
            // Step 10: Also compare with null item
            cJSON_bool null_comparison = cJSON_Compare(parsed_json, null_item, 1);
            
            cJSON_Delete(duplicate);
        }
        
        free(print_buffer);
    }
    
    // Step 11: Clean up
    cJSON_Delete(parsed_json);
    cJSON_Delete(null_item);
    
    return 0;
}