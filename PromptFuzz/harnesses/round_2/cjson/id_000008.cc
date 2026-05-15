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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_CreateObject","cJSON_CreateArray","cJSON_IsObject","cJSON_AddObjectToObject","cJSON_IsArray","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_Delete","cJSON_AddNullToObject","cJSON_DeleteItemFromObject","cJSON_AddTrueToObject","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_ParseWithLengthOpts","cJSON_Delete","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_CreateObject","cJSON_CreateArray","cJSON_IsObject","cJSON_AddObjectToObject","cJSON_IsArray","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_Delete","cJSON_AddNullToObject","cJSON_DeleteItemFromObject","cJSON_AddTrueToObject","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_ParseWithLengthOpts","cJSON_Delete","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/*To achieve the event using the specified cJSON APIs, I'll create a fuzz driver that parses input JSON, creates a new object, manipulates it, and then cleans up. The event I'll simulate is: parse input JSON, create a new JSON object with nested structures, validate types, modify the object, and handle memory allocation.

*/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Use cJSON_malloc to allocate memory (required by API list)
    char *input_copy = static_cast<char*>(cJSON_malloc(size + 1));
    if (!input_copy) {
	return 0;
	}
	
    
    // Copy input data and null-terminate for parsing
    memcpy(input_copy, data, size);
    input_copy[size] = '\0';
    
    // 1. Parse input using cJSON_ParseWithLength
    cJSON *parsed = cJSON_ParseWithLength(input_copy, size);
    
    // 2. Create new JSON object and array
    cJSON *new_obj = cJSON_CreateObject();
    cJSON *new_array = cJSON_CreateArray();
    
    if (parsed && new_obj && new_array) {
        // 3. Check if parsed item is an object
        if (cJSON_IsObject(parsed)) {
            // 4. Add a nested object to new_obj
            cJSON *nested_obj = cJSON_AddObjectToObject(new_obj, "nested");
            
            // 5. Check if parsed item is an array (though it's not in this case)
            cJSON_IsArray(parsed);  // This returns false but we call it anyway
            
            // 6. Check if a specific field in parsed is a string
            cJSON *widget_item = cJSON_GetObjectItem(parsed, "widget");
            if (widget_item && cJSON_IsString(widget_item)) {
                // Create another object to demonstrate string handling
                cJSON *string_check_obj = cJSON_CreateObject();
                cJSON_AddStringToObject(string_check_obj, "test", "value");
                cJSON_Delete(string_check_obj);
            }
            
            // 7. Add null to the object
            cJSON *null_item = cJSON_AddNullToObject(new_obj, "null_field");
            
            // 8. Delete the null item we just added
            if (null_item) {
                cJSON_DeleteItemFromObject(new_obj, "null_field");
            }
            
            // 9. Add true to the object
            cJSON_AddTrueToObject(new_obj, "true_field");
            
            // 10. Create integer array
            int int_values[] = {1, 2, 3, 4, 5};
            cJSON *int_array = cJSON_CreateIntArray(int_values, 5);
            if (int_array) {
                cJSON_AddItemToObject(new_obj, "int_array", int_array);
            }
            
            // 11. Parse the input again with different options
            const char *parse_end = nullptr;
            cJSON *parsed_again = cJSON_ParseWithLengthOpts(input_copy, size, &parse_end, true);
            if (parsed_again) {
                cJSON_Delete(parsed_again);
            }
        }
    }
    
    // 12. Clean up all allocated resources
    cJSON_free(input_copy);
    if (parsed) cJSON_Delete(parsed);
    if (new_obj) cJSON_Delete(new_obj);
    if (new_array) cJSON_Delete(new_array);
    
    return 0;
}