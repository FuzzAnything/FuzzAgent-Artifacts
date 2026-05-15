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
//<ID> 39
//<Prompt> []
/*<Combination>: [
*/
//<score> 5.6666665, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_CreateStringArray","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsBool","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsArray","cJSON_CreateArrayReference","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_AddItemToObject","cJSON_Delete","cJSON_Delete","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateStringArray","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsBool","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsArray","cJSON_CreateArrayReference","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_AddItemToObject","cJSON_Delete","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":2}
/**/


// cjson headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure we have enough data for meaningful parsing
    if (size < 10) {
	return 0;
	}
	
    
    // Create a null-terminated string from the input data
    char* json_str = (char*)malloc(size + 1);
    if (!json_str) {
	return 0;
	}
	
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    // Parse the input JSON using cJSON_Parse
    cJSON* parsed_json = cJSON_Parse(json_str);
    
    // Free the temporary string
    free(json_str);
    
    if (!parsed_json) {
        // If parsing fails, try to get error information
        const char* error_ptr = cJSON_GetErrorPtr();
        return 0;
    }
    
    // 1. Create a string array using cJSON_CreateStringArray
    const char* test_strings[] = {"string1", "string2", "string3"};
    cJSON* string_array = cJSON_CreateStringArray(test_strings, 3);
    
    // 2. Check if an item from parsed JSON is a boolean using cJSON_IsBool
    cJSON* menu_item = cJSON_GetObjectItem(parsed_json, "menu");
    if (menu_item) {
        // Try to find a boolean value in the structure
        // We'll check the type of various elements
        cJSON* items_array = cJSON_GetObjectItem(menu_item, "items");
        if (items_array && cJSON_IsArray(items_array)) {
            // Check each item in the array
            for (int i = 0; i < cJSON_GetArraySize(items_array); i++) {
                cJSON* item = cJSON_GetArrayItem(items_array, i);
                if (item && cJSON_IsBool(item)) {
                    // Found a boolean item
                    break;
                }
            }
        }
    }
    
    // 3. Create an array reference using cJSON_CreateArrayReference
    cJSON* array_ref = NULL;
    cJSON* items = cJSON_GetObjectItem(cJSON_GetObjectItem(parsed_json, "menu"), "items");
    if (items && cJSON_IsArray(items)) {
        array_ref = cJSON_CreateArrayReference(items);
    }
    
    // 4. Add a string to an object using cJSON_AddStringToObject
    // First create a new object
    cJSON* new_object = cJSON_CreateObject();
    if (new_object) {
        cJSON_AddStringToObject(new_object, "fuzzer_key", "fuzzer_value");
        
        // Also add the string array we created earlier
        cJSON_AddItemToObject(new_object, "string_array", string_array);
        
        // Clean up the new object
        cJSON_Delete(new_object);
    } else {
        // Clean up string_array if we didn't add it to an object
        cJSON_Delete(string_array);
    }
    
    // 5. Create a true boolean value using cJSON_CreateTrue
    cJSON* true_value = cJSON_CreateTrue();
    
    // Add the true value to the parsed JSON if possible
    if (parsed_json) {
        cJSON_AddItemToObject(parsed_json, "fuzzer_bool", true_value);
    } else {
        cJSON_Delete(true_value);
    }
    
    // Additional testing: Try to print the modified JSON
    char* printed_json = cJSON_Print(parsed_json);
    if (printed_json) {
        free(printed_json);
    }
    
    // Clean up all allocated resources
    if (array_ref) {
        // Note: array_ref is a reference, don't delete the referenced data
        // Just free the reference container
        cJSON_free(array_ref);
    }
    
    if (parsed_json) {
        cJSON_Delete(parsed_json);
    }
    
    return 0;
}