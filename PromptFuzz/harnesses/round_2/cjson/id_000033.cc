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
//<ID> 220
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["cJSON_CreateTrue","cJSON_ParseWithOpts","cJSON_IsObject","cJSON_AddNullToObject","cJSON_IsArray","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Minify","cJSON_CreateNumber","cJSON_SetNumberHelper","cJSON_IsObject","cJSON_AddItemReferenceToObject","cJSON_ParseWithOpts","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_CreateTrue","cJSON_ParseWithOpts","cJSON_IsObject","cJSON_AddNullToObject","cJSON_IsArray","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Minify","cJSON_CreateNumber","cJSON_SetNumberHelper","cJSON_IsObject","cJSON_AddItemReferenceToObject","cJSON_ParseWithOpts","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


// Include cJSON headers

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a null-terminated string from input data for cJSON parsing
    // Allocate buffer with extra byte for null terminator
    char *json_string = (char*)malloc(size + 1);
    if (!json_string) {
	return 0;
	}
	
    
    memcpy(json_string, data, size);
    json_string[size] = '\0';
    
    // 1. Create a true value using cJSON_CreateTrue
    cJSON *true_item = cJSON_CreateTrue();
    if (!true_item) {
        free(json_string);
        return 0;
    }
    
    // 2. Parse the input data using cJSON_ParseWithOpts
    const char *parse_end = nullptr;
    cJSON *parsed_json = cJSON_ParseWithOpts(json_string, &parse_end, false);
    
    // 3. Add a null item to an object (if we have a valid parsed object)
    if (parsed_json && cJSON_IsObject(parsed_json)) {
        cJSON *null_item = cJSON_AddNullToObject(parsed_json, "fuzz_null");
        (void)null_item; // Suppress unused variable warning
    }
    
    // 4. Detach an item via pointer (if we have appropriate structure)
    if (parsed_json) {
        if (cJSON_IsArray(parsed_json) && parsed_json->child) {
            cJSON *detached = cJSON_DetachItemViaPointer(parsed_json, parsed_json->child);
            if (detached) {
                cJSON_Delete(detached);
            }
        }
    }
    
    // 5. Minify JSON string (in-place operation)
    // We'll minify our input copy
    cJSON_Minify(json_string);
    
    // 6. Set number helper - create a number item and modify it
    cJSON *number_item = cJSON_CreateNumber(0.0);
    if (number_item) {
        double new_value = 42.5;
        if (size > 0) {
            // Use first byte to influence the number (for fuzzing diversity)
            new_value = data[0] * 0.1;
        }
        cJSON_SetNumberHelper(number_item, new_value);
    }
    
    // 7. Add item reference to object
    if (parsed_json && cJSON_IsObject(parsed_json) && true_item) {
        cJSON_AddItemReferenceToObject(parsed_json, "true_reference", true_item);
    }
    
    // Additional fuzzing: try to parse the minified string
    if (strlen(json_string) > 0) {
        cJSON *minified_parsed = cJSON_ParseWithOpts(json_string, nullptr, false);
        if (minified_parsed) {
            cJSON_Delete(minified_parsed);
        }
    }
    
    // Cleanup all allocated resources
    if (true_item) {
        cJSON_Delete(true_item);
    }
    if (number_item) {
        cJSON_Delete(number_item);
    }
    if (parsed_json) {
        cJSON_Delete(parsed_json);
    }
    free(json_string);
    
    return 0;
}