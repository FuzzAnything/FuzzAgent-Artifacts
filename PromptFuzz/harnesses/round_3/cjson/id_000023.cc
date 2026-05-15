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
//<ID> 100
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 3
//<Quality> {"density":20,"unique_branches":{"cJSON_InsertItemInArray":[[2344,9,2344,31,0,0,4,1],[2349,9,2349,39,0,0,4,1],[2357,9,2357,39,0,0,4,0]]},"library_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_CreateArray","cJSON_CreateFalse","cJSON_CreateNull","cJSON_AddNullToObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_InsertItemInArray","cJSON_InsertItemInArray","cJSON_InsertItemInArray","cJSON_Print","cJSON_free","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_CreateArray","cJSON_CreateFalse","cJSON_CreateNull","cJSON_AddNullToObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_InsertItemInArray","cJSON_InsertItemInArray","cJSON_InsertItemInArray","cJSON_Print","cJSON_free","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_free"],"visited":0}
/*Here's a fuzz driver that uses the specified cJSON APIs to process the input data:

*/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    // Create a null-terminated string from the input data
    if (size == 0) {
	return 0;
	}
	
    
    char *json_str = static_cast<char*>(cJSON_malloc(size + 1));
    if (!json_str) {
	return 0;
	}
	
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    // 1. Parse the input JSON using the provided data
    cJSON *parsed_json = cJSON_ParseWithLength(json_str, size);
    
    // 2. Create a new array (first API from the list)
    cJSON *new_array = cJSON_CreateArray();
    
    // 3. Create false and null items for insertion
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *null_item = cJSON_CreateNull();
    
    if (parsed_json && new_array && false_item && null_item) {
        // 4. Add a null item to the parsed object
        cJSON_AddNullToObject(parsed_json, fuzz_str_1);
        
        // 5. Detach an item from the object (try to detach "Thumbnail" if it exists)
        cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(parsed_json, fuzz_str_2);
        
        // If we successfully detached something, add it to our new array
        if (detached) {
            cJSON_InsertItemInArray(new_array, 0, detached);
        }
        
        // 6. Insert the false and null items into the array
        cJSON_InsertItemInArray(new_array, 0, false_item);
        cJSON_InsertItemInArray(new_array, 0, null_item);
        
        // Clean up detached item (already inserted into array, will be cleaned with array)
        // The array now owns the detached item
        
        // Print the result for debugging (optional, can be removed for fuzzing)
        char *printed = cJSON_Print(parsed_json);
        if (printed) {
            cJSON_free(printed);
        }
        
        // Print the array
        char *array_printed = cJSON_Print(new_array);
        if (array_printed) {
            cJSON_free(array_printed);
        }
        
        // Clean up
        cJSON_Delete(parsed_json);
        cJSON_Delete(new_array);
        // Note: false_item and null_item are owned by new_array, so they're deleted with it
    } else {
        // Clean up any partially created items
        if (parsed_json) cJSON_Delete(parsed_json);
        if (new_array) cJSON_Delete(new_array);
        if (false_item) cJSON_Delete(false_item);
        if (null_item) cJSON_Delete(null_item);
    }
    
    // Free the allocated string
    cJSON_free(json_str);
    
    return 0;
}