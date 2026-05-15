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
//<ID> 103
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 3
//<Quality> {"density":20,"unique_branches":{"cJSON_Duplicate_rec":[[2836,12,2836,41,0,0,4,0],[2840,13,2840,22,0,0,4,0],[2867,9,2867,24,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_CreateStringReference","cJSON_CreateObjectReference","cJSON_CreateObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddTrueToObject","cJSON_Duplicate","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_HasObjectItem","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_IsArray","cJSON_DeleteItemFromArray","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateStringReference","cJSON_CreateObjectReference","cJSON_CreateObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddTrueToObject","cJSON_Duplicate","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_HasObjectItem","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_IsArray","cJSON_DeleteItemFromArray","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_Delete","cJSON_Delete"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified cJSON APIs with the given input data:

*/


// Include cjson header

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a null-terminated string from input data
    char *input_string = (char *)malloc(size + 1);
    if (!input_string) {
        return 0;
    }
    memcpy(input_string, data, size);
    input_string[size] = '\0';

    // Parse the input JSON string (3x3 matrix)
    cJSON *parsed_json = cJSON_Parse(input_string);
    if (!parsed_json) {
        free(input_string);
        return 0;
    }

    // Create a string reference
    const char *test_string = "test_string";
    cJSON *string_ref = cJSON_CreateStringReference(test_string);
    
    // Create an object reference from parsed JSON
    cJSON *object_ref = cJSON_CreateObjectReference(parsed_json);

    // Create a new object to work with
    cJSON *main_object = cJSON_CreateObject();
    
    // Add items to the object
    cJSON_AddItemToObject(main_object, "original_array", parsed_json);
    cJSON_AddItemToObject(main_object, "string_reference", string_ref);
    cJSON_AddItemToObject(main_object, "object_reference", object_ref);
    
    // Add a true value to the object
    cJSON *true_item = cJSON_AddTrueToObject(main_object, "added_true");
    
    // Create a duplicate of the object (with recursion)
    cJSON *duplicate = cJSON_Duplicate(main_object, 1);
    
    if (duplicate) {
        // Try to detach an item from the duplicated object
        cJSON *detached_item = cJSON_DetachItemFromObjectCaseSensitive(
            duplicate, "added_true");
        if (detached_item) {
            cJSON_Delete(detached_item);
        }
        
        // Also detach from original to ensure API usage
        if (cJSON_HasObjectItem(main_object, "added_true")) {
            cJSON *detached_original = cJSON_DetachItemFromObjectCaseSensitive(
                main_object, "added_true");
            if (detached_original) {
                cJSON_Delete(detached_original);
            }
        }
        
        // Try to delete an item from an array
        if (cJSON_IsArray(parsed_json)) {
            // Delete the first item from the parsed array
            cJSON_DeleteItemFromArray(parsed_json, 0);
            
            // Check if we have at least 2 more items to delete
            if (cJSON_GetArraySize(parsed_json) >= 2) {
                cJSON_DeleteItemFromArray(parsed_json, 1);
            }
        }
        
        // Clean up duplicate
        cJSON_Delete(duplicate);
    }

    // Clean up
    cJSON_Delete(main_object);
    free(input_string);

    return 0;
}