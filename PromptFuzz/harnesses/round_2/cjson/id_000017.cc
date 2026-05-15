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
//<ID> 56
//<Prompt> []
/*<Combination>: [
*/
//<score> 23, nr_unique_branch: 0
//<Quality> {"density":23,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_IsInvalid","cJSON_IsArray","cJSON_GetArraySize","cJSON_IsArray","cJSON_GetArraySize","cJSON_IsArray","cJSON_GetArraySize","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_AddFalseToObject","cJSON_AddRawToObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_CreateObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_GetArraySize","cJSON_AddItemToObject","cJSON_CreateObjectReference","cJSON_AddItemToObject","cJSON_PrintUnformatted","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_IsInvalid","cJSON_GetArraySize","cJSON_IsArray","cJSON_GetArraySize","cJSON_IsArray","cJSON_GetArraySize","cJSON_IsArray","cJSON_GetArraySize","cJSON_IsArray","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_AddFalseToObject","cJSON_AddRawToObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_CreateObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_GetArraySize","cJSON_AddItemToObject","cJSON_CreateObjectReference","cJSON_AddItemToObject","cJSON_PrintUnformatted","cJSON_Delete"],"visited":0}
/*Looking at the provided APIs and the input JSON data, I'll create a fuzz driver that manipulates the JSON structure using the specified APIs. The scenario will involve parsing the input JSON, modifying it by adding elements, detaching items, and checking array sizes.

*/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a null-terminated string from the input data
    char *input_str = (char *)malloc(size + 1);
    if (!input_str) {
	return 0;
	}
	
    
    memcpy(input_str, data, size);
    input_str[size] = '\0';
    
    // 1. Parse the input JSON string using cJSON_Parse
    cJSON *parsed_json = cJSON_Parse(input_str);
    free(input_str);
    
    if (!parsed_json) {
        // If parsing fails, cleanup and return
        return 0;
    }
    
    // 2. Check if the parsed item is invalid using cJSON_IsInvalid
    // (This should return false for valid JSON)
    cJSON_bool is_invalid = cJSON_IsInvalid(parsed_json);
    
    // 3. Get array size of any arrays in the JSON structure
    // We'll look for arrays recursively
    int total_array_size = 0;
    
    // Helper function to traverse JSON and count array sizes
    auto traverse_and_count_arrays = [](cJSON *item) {
        int count = 0;
        if (cJSON_IsArray(item)) {
            count += cJSON_GetArraySize(item);
        }
        
        // Check child elements
        if (item->child) {
            cJSON *child = item->child;
            while (child) {
                if (cJSON_IsArray(child)) {
                    count += cJSON_GetArraySize(child);
                }
                child = child->next;
            }
        }
        
        return count;
    };
    
    total_array_size = traverse_and_count_arrays(parsed_json);
    
    // 4. Add a false value to the widget object if it exists
    cJSON *widget = cJSON_GetObjectItemCaseSensitive(parsed_json, "widget");
    if (widget && cJSON_IsObject(widget)) {
        cJSON_AddFalseToObject(widget, "is_false_value");
        
        // 5. Add a raw JSON value to the widget object
        cJSON_AddRawToObject(widget, "raw_data", "\"This is raw JSON data\"");
        
        // 6. Try to detach an item from the widget object (case sensitive)
        // We'll try to detach the "debug" item if it exists
        cJSON *detached_item = cJSON_DetachItemFromObjectCaseSensitive(widget, "debug");
        if (detached_item) {
            cJSON_Delete(detached_item);
        }
    }
    
    // 7. Create a new object and add some array operations
    cJSON *new_object = cJSON_CreateObject();
    if (new_object) {
        // Create an array and add it to the new object
        cJSON *test_array = cJSON_CreateArray();
        if (test_array) {
            // Add some items to the array
            cJSON_AddItemToArray(test_array, cJSON_CreateNumber(1));
            cJSON_AddItemToArray(test_array, cJSON_CreateNumber(2));
            cJSON_AddItemToArray(test_array, cJSON_CreateNumber(3));
            
            cJSON_AddItemToObject(new_object, "test_array", test_array);
            
            // Get the array size using cJSON_GetArraySize
            int array_size = cJSON_GetArraySize(test_array);
            
            // Try to add the original parsed JSON to this new structure
            // (but we need to be careful not to create cycles)
            cJSON_AddItemToObject(new_object, "original_reference", 
                                 cJSON_CreateObjectReference(parsed_json));
        }
        
        // Add the new object to the parsed JSON
        cJSON_AddItemToObject(parsed_json, "fuzzer_added", new_object);
    }
    
    // 8. Print the modified JSON in unformatted format using cJSON_PrintUnformatted
    char *modified_json = cJSON_PrintUnformatted(parsed_json);
    if (modified_json) {
        // We could write this to a file or use it somehow
        // For fuzzing, we'll just ensure the API is called
        free(modified_json);
    }
    
    // 9. Clean up all allocated resources
    cJSON_Delete(parsed_json);
    
    return 0;
}