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
//<ID> 91
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 1
//<Quality> {"density":17,"unique_branches":{"skip_oneline_comment":[[2879,12,2879,31,0,0,4,1]]},"library_calls":["cJSON_Version","cJSON_ParseWithLength","cJSON_CreateNull","cJSON_CreateNumber","cJSON_IsArray","cJSON_GetArraySize","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_Delete","cJSON_CreateObject","cJSON_AddBoolToObject","cJSON_AddRawToObject","cJSON_PrintBuffered","cJSON_Minify","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Version","cJSON_ParseWithLength","cJSON_CreateNull","cJSON_CreateNumber","cJSON_IsArray","cJSON_GetArraySize","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_CreateObject","cJSON_AddBoolToObject","cJSON_AddRawToObject","cJSON_PrintBuffered","cJSON_Minify","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Get cJSON version
    const char *version = cJSON_Version();
    
    // Parse input data as JSON
    cJSON *parsed = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!parsed) {
        return 0;
    }
    
    // Create a new null item
    cJSON *null_item = cJSON_CreateNull();
    
    // Create a new number
    cJSON *number_item = cJSON_CreateNumber(42.5);
    
    // Check if parsed JSON is an array
    cJSON_bool is_array = cJSON_IsArray(parsed);
    
    // If it's an array, get its size
    int array_size = 0;
    if (is_array) {
        array_size = cJSON_GetArraySize(parsed);
        
        // Add null item to array
        cJSON_AddItemToArray(parsed, null_item);
        
        // Add number item to array
        cJSON_AddItemToArray(parsed, number_item);
        
        // Detach an item from array (detach the first item)
        if (array_size > 0) {
            cJSON *detached = cJSON_DetachItemFromArray(parsed, 0);
            if (detached) {
                cJSON_Delete(detached);
            }
        }
    } else {
        // If not an array, create a new array and add items
        cJSON *new_array = cJSON_CreateArray();
        cJSON_AddItemToArray(new_array, null_item);
        cJSON_AddItemToArray(new_array, number_item);
        cJSON_Delete(new_array);
    }
    
    // Create a new object
    cJSON *new_object = cJSON_CreateObject();
    
    // Add boolean to object
    cJSON_AddBoolToObject(new_object, "fuzzed_bool", 1);
    
    // Add raw JSON string to object
    const char *raw_json = "{\"raw\": \"data\"}";
    cJSON_AddRawToObject(new_object, "raw_data", raw_json);
    
    // Print the parsed JSON with buffering
    char *printed = cJSON_PrintBuffered(parsed, 1024, 1);
    if (printed) {
        // Minify the printed JSON in place
        cJSON_Minify(printed);
        
        // Free the printed string
        cJSON_free(printed);
    }
    
    // Clean up
    cJSON_Delete(parsed);
    cJSON_Delete(new_object);
    
    // Note: null_item and number_item were either added to arrays (and thus owned by them)
    // or deleted when we deleted the temporary array
    
    return 0;
}