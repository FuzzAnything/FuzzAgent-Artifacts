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
//<ID> 57
//<Prompt> []
/*<Combination>: [
*/
//<score> 13.333333, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"cJSON_IsInvalid":[[2974,9,2974,21,0,0,4,0]]},"library_calls":["cJSON_malloc","cJSON_Parse","cJSON_IsInvalid","cJSON_free","cJSON_Delete","cJSON_free","cJSON_GetArraySize","cJSON_IsObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_DetachItemFromObjectCaseSensitive","cJSON_IsObject","cJSON_AddFalseToObject","cJSON_AddRawToObject","cJSON_IsArray","cJSON_GetArraySize","cJSON_Delete","cJSON_IsArray","cJSON_DetachItemFromArray","cJSON_AddItemToArray","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_Parse","cJSON_IsInvalid","cJSON_GetArraySize","cJSON_IsObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_DetachItemFromObjectCaseSensitive","cJSON_IsObject","cJSON_AddFalseToObject","cJSON_AddRawToObject","cJSON_IsArray","cJSON_GetArraySize","cJSON_Delete","cJSON_IsArray","cJSON_DetachItemFromArray","cJSON_AddItemToArray","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_free"],"visited":2}
/*Here's a C++ fuzz driver that implements the specified functionality using the cJSON library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Handle empty input
    if (size == 0) {
        return 0;
    }
    
    // Create a null-terminated string from the input data
    char *json_str = (char *)cJSON_malloc(size + 1);
    if (!json_str) {
        return 0;
    }
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    // Parse JSON using cJSON_Parse
    cJSON *parsed = cJSON_Parse(json_str);
    
    // Check if the parsed item is invalid
    if (cJSON_IsInvalid(parsed)) {
        cJSON_free(json_str);
        if (parsed) {
            cJSON_Delete(parsed);
        }
        return 0;
    }
    
    if (!parsed) {
        cJSON_free(json_str);
        return 0;
    }
    
    // Get array size if the parsed item is an array
    int array_size = cJSON_GetArraySize(parsed);
    
    // Detach an item from object (if it's an object)
    cJSON *detached_item = NULL;
    if (cJSON_IsObject(parsed)) {
        // Try to detach a common field name
        detached_item = cJSON_DetachItemFromObjectCaseSensitive(parsed, "widget");
        if (!detached_item) {
            detached_item = cJSON_DetachItemFromObjectCaseSensitive(parsed, "debug");
        }
    }
    
    // If we have a parsed object, add false and raw items
    if (cJSON_IsObject(parsed)) {
        // Add a false boolean to the object
        cJSON_AddFalseToObject(parsed, "fuzz_false");
        
        // Add a raw JSON string to the object
        const char *raw_json = "{\"raw_data\": \"test\"}";
        cJSON_AddRawToObject(parsed, "fuzz_raw", raw_json);
    }
    
    // Process detached item if we have one
    if (detached_item) {
        // If detached item is an array, get its size
        if (cJSON_IsArray(detached_item)) {
            int detached_array_size = cJSON_GetArraySize(detached_item);
            (void)detached_array_size; // Use the variable to avoid compiler warning
        }
        
        // Delete the detached item
        cJSON_Delete(detached_item);
    }
    
    // If parsed object is an array, try to detach an array item and re-add it
    if (cJSON_IsArray(parsed)) {
        if (array_size > 0) {
            // Detach the first item
            cJSON *array_item = cJSON_DetachItemFromArray(parsed, 0);
            if (array_item) {
                // Re-add it to the end of the array
                cJSON_AddItemToArray(parsed, array_item);
            }
        }
    }
    
    // Print the modified JSON (unformatted)
    char *printed = cJSON_PrintUnformatted(parsed);
    if (printed) {
        // Clean up printed string
        cJSON_free(printed);
    }
    
    // Clean up
    cJSON_Delete(parsed);
    cJSON_free(json_str);
    
    return 0;
}