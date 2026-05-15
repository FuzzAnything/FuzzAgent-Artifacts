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
//<ID> 55
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_IsInvalid","cJSON_AddFalseToObject","cJSON_AddRawToObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetArraySize","cJSON_Delete","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_GetArraySize","cJSON_Delete","cJSON_PrintUnformatted","cJSON_Delete","cJSON_CreateObject","cJSON_AddFalseToObject","cJSON_IsInvalid","cJSON_AddRawToObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_GetArraySize","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_PrintUnformatted","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_AddFalseToObject","cJSON_IsInvalid","cJSON_AddRawToObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_GetArraySize","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_PrintUnformatted","cJSON_Delete"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create null-terminated string from input data for parsing
    char *input_str = static_cast<char*>(malloc(size + 1));
    if (!input_str) {
	return 0;
	}
	
    memcpy(input_str, data, size);
    input_str[size] = '\0';
    
    // Parse the JSON input
    cJSON *parsed_json = cJSON_Parse(input_str);
    free(input_str);
    
    // If parsing succeeded, perform operations
    if (parsed_json) {
        // Check if the parsed JSON is invalid (should be false for valid JSON)
        cJSON_bool is_invalid = cJSON_IsInvalid(parsed_json);
        
        // Add a false boolean to the object
        cJSON *false_item = cJSON_AddFalseToObject(parsed_json, "added_false");
        
        // Add a raw JSON string to the object
        cJSON *raw_item = cJSON_AddRawToObject(parsed_json, "added_raw", "{\"key\": \"value\"}");
        
        // Detach an item from the object (case-sensitive)
        // Try to detach the "widget" object if it exists
        cJSON *detached_item = cJSON_DetachItemFromObjectCaseSensitive(parsed_json, "widget");
        if (detached_item) {
            // If we detached "widget", check if it contains arrays
            // First, try to get the "window" object
            cJSON *window = cJSON_GetObjectItemCaseSensitive(detached_item, "window");
            if (window) {
                // window is an object, not an array, so array size should be 0
                int array_size = cJSON_GetArraySize(window);
                (void)array_size; // Use the result to avoid unused variable warning
            }
            
            // Clean up the detached item
            cJSON_Delete(detached_item);
        } else {
            // If "widget" doesn't exist, try to detach something else
            // Create a temporary array to test cJSON_GetArraySize
            cJSON *temp_array = cJSON_CreateArray();
            if (temp_array) {
                // Add some items to the array
                cJSON_AddItemToArray(temp_array, cJSON_CreateString("array_item1"));
                cJSON_AddItemToArray(temp_array, cJSON_CreateNumber(42));
                
                // Get the array size
                int array_size = cJSON_GetArraySize(temp_array);
                (void)array_size; // Use the result
                
                // Clean up
                cJSON_Delete(temp_array);
            }
        }
        
        // Print the modified JSON (unformatted)
        char *output = cJSON_PrintUnformatted(parsed_json);
        if (output) {
            free(output);
        }
        
        // Clean up the main JSON object
        cJSON_Delete(parsed_json);
    } else {
        // If parsing failed, create a new object and test the APIs
        cJSON *new_obj = cJSON_CreateObject();
        if (new_obj) {
            // Add false boolean
            cJSON *false_item = cJSON_AddFalseToObject(new_obj, "false_item");
            
            // Check if the item is invalid (should be false for valid items)
            if (false_item) {
                cJSON_bool is_invalid = cJSON_IsInvalid(false_item);
                (void)is_invalid; // Use the result
            }
            
            // Add raw JSON
            cJSON *raw_item = cJSON_AddRawToObject(new_obj, "raw_item", "[1, 2, 3]");
            
            // Create an array to test cJSON_GetArraySize
            cJSON *test_array = cJSON_CreateArray();
            if (test_array) {
                cJSON_AddItemToArray(test_array, cJSON_CreateNumber(1));
                cJSON_AddItemToArray(test_array, cJSON_CreateNumber(2));
                cJSON_AddItemToObject(new_obj, "test_array", test_array);
                
                // Get array size
                int array_size = cJSON_GetArraySize(test_array);
                (void)array_size; // Use the result
            }
            
            // Try to detach an item (should return NULL since item doesn't exist)
            cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(new_obj, "non_existent");
            if (detached) {
                cJSON_Delete(detached);
            }
            
            // Print the object
            char *output = cJSON_PrintUnformatted(new_obj);
            if (output) {
                free(output);
            }
            
            // Clean up
            cJSON_Delete(new_obj);
        }
    }
    
    return 0;
}