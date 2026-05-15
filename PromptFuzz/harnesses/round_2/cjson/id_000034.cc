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
//<ID> 221
//<Prompt> []
/*<Combination>: [
*/
//<score> 13.5, nr_unique_branch: 3
//<Quality> {"density":9,"unique_branches":{"cJSON_SetNumberHelper":[[418,9,418,26,0,0,4,0],[422,14,422,39,0,0,4,0]],"print_number":[[77,18,77,26,2,0,4,0]]},"library_calls":["cJSON_ParseWithOpts","cJSON_CreateTrue","cJSON_IsObject","cJSON_CreateObject","cJSON_Delete","cJSON_AddNullToObject","cJSON_AddItemReferenceToObject","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_CreateNumber","cJSON_SetNumberHelper","cJSON_AddItemToObject","cJSON_Delete","cJSON_PrintUnformatted","cJSON_Minify","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithOpts","cJSON_CreateTrue","cJSON_IsObject","cJSON_CreateObject","cJSON_Delete","cJSON_AddNullToObject","cJSON_AddItemReferenceToObject","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_CreateNumber","cJSON_SetNumberHelper","cJSON_AddItemToObject","cJSON_PrintUnformatted","cJSON_Minify","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


// Include cjson headers
extern "C" {
    #include "cJSON.h"
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a null-terminated string from the input data
    char *input_string = static_cast<char*>(malloc(size + 1));
    if (!input_string) {
	return 0;
	}
	
    memcpy(input_string, data, size);
    input_string[size] = '\0';
    
    // 1. Parse the input using cJSON_ParseWithOpts
    const char *parse_end = nullptr;
    cJSON *parsed_json = cJSON_ParseWithOpts(input_string, &parse_end, 0);  // require_null_terminated = false
    
    // 2. Create a true value
    cJSON *true_value = cJSON_CreateTrue();
    
    // 3. Create an object if parsing failed or we have valid JSON
    cJSON *root_object = nullptr;
    if (parsed_json && cJSON_IsObject(parsed_json)) {
        root_object = parsed_json;
    } else {
        root_object = cJSON_CreateObject();
        if (parsed_json) {
            cJSON_Delete(parsed_json);
        }
    }
    
    // 4. Add a null item to the object
    cJSON *null_item = cJSON_AddNullToObject(root_object, "fuzz_null");
    
    // 5. Add reference to true value
    if (true_value && root_object) {
        cJSON_AddItemReferenceToObject(root_object, "fuzz_true_ref", true_value);
    }
    
    // 6. Detach an item via pointer if we have items
    if (null_item) {
        cJSON *detached = cJSON_DetachItemViaPointer(root_object, null_item);
        if (detached) {
            cJSON_Delete(detached);
        }
    }
    
    // 7. Create a number and use cJSON_SetNumberHelper
    cJSON *number_item = cJSON_CreateNumber(0.0);
    if (number_item) {
        // Generate a random number based on input data
        double random_number = 0.0;
        if (size >= sizeof(double)) {
            memcpy(&random_number, data, sizeof(double));
        } else if (size > 0) {
            random_number = static_cast<double>(data[0]);
        }
        cJSON_SetNumberHelper(number_item, random_number);
        
        if (root_object) {
            cJSON_AddItemToObject(root_object, "fuzz_number", number_item);
        } else {
            cJSON_Delete(number_item);
        }
    }
    
    // 8. Minify JSON string if we have an object
    if (root_object) {
        char *json_string = cJSON_PrintUnformatted(root_object);
        if (json_string) {
            cJSON_Minify(json_string);
            free(json_string);
        }
    }
    
    // Cleanup
    if (true_value) cJSON_Delete(true_value);
    if (root_object) cJSON_Delete(root_object);
    free(input_string);
    
    return 0;
}