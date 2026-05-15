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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_CreateArray","cJSON_CreateIntArray","cJSON_AddObjectToObject","cJSON_GetObjectItem","cJSON_GetStringValue","cJSON_malloc","cJSON_free","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsFalse","cJSON_DeleteItemFromObject","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_CreateArray","cJSON_CreateIntArray","cJSON_AddObjectToObject","cJSON_GetObjectItem","cJSON_GetStringValue","cJSON_malloc","cJSON_free","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsFalse","cJSON_DeleteItemFromObject","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Use the input data as a JSON string for parsing
    const char* json_str = reinterpret_cast<const char*>(data);
    
    // Parse the JSON input
    cJSON* root = cJSON_ParseWithLength(json_str, size);
    if (!root) {
        // Invalid JSON, return early
        return 0;
    }
    
    // Create a new object to work with
    cJSON* new_object = cJSON_CreateObject();
    
    // Create an array
    cJSON* new_array = cJSON_CreateArray();
    
    // Create an integer array (using some sample integers)
    int int_numbers[] = {1, 2, 3, 4, 5};
    cJSON* int_array = cJSON_CreateIntArray(int_numbers, 5);
    
    // Add the integer array to the main array (note: this is not in the API list but needed)
    // Since cJSON_AddItemToArray isn't in our required list, we'll keep int_array separate
    // and use it for other operations
    
    // Add an object to our new object
    cJSON* added_object = cJSON_AddObjectToObject(new_object, "nested_object");
    
    // Get string value from the original JSON's name field if it exists
    cJSON* name_item = cJSON_GetObjectItem(root, "name");
    if (name_item) {
        char* name_value = cJSON_GetStringValue(name_item);
        if (name_value) {
            // We got the string value, now free it (simulating usage)
            // Note: cJSON_GetStringValue returns a pointer to the existing string,
            // so we don't allocate new memory. We'll demonstrate cJSON_free with 
            // something we actually allocate.
            char* copied_string = (char*)cJSON_malloc(strlen(name_value) + 1);
            if (copied_string) {
                strcpy(copied_string, name_value);
                // Use the copied string in some way
                // Then free it with cJSON_free
                cJSON_free(copied_string);
            }
        }
    }
    
    // Check if the "interlace" field exists and is false in the original JSON
    cJSON* format = cJSON_GetObjectItem(root, "format");
    if (format) {
        cJSON* interlace = cJSON_GetObjectItem(format, "interlace");
        if (interlace && cJSON_IsFalse(interlace)) {
            // Delete the interlace item from the format object
            cJSON_DeleteItemFromObject(format, "interlace");
        }
    }
    
    // Clean up all allocated cJSON objects
    cJSON_Delete(root);
    cJSON_Delete(new_object);
    cJSON_Delete(new_array);
    cJSON_Delete(int_array);
    
    return 0;
}