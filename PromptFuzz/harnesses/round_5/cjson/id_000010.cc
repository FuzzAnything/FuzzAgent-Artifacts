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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 29, nr_unique_branch: 0
//<Quality> {"density":29,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_IsArray","cJSON_Delete","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_CreateString","cJSON_CreateString","cJSON_ReplaceItemInObject","cJSON_Delete","cJSON_ReplaceItemInObject","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_AddItemReferenceToObject","cJSON_CreateNumber","cJSON_AddItemToObjectCS","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_AddFalseToObject","cJSON_CreateFloatArray","cJSON_CreateIntArray","cJSON_ReplaceItemInArray","cJSON_Delete","cJSON_AddItemToArray","cJSON_Print","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_CreateString","cJSON_CreateString","cJSON_ReplaceItemInObject","cJSON_ReplaceItemInObject","cJSON_GetObjectItemCaseSensitive","cJSON_AddItemReferenceToObject","cJSON_CreateNumber","cJSON_AddItemToObjectCS","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_AddFalseToObject","cJSON_CreateFloatArray","cJSON_CreateIntArray","cJSON_ReplaceItemInArray","cJSON_AddItemToArray","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 1: Parse input JSON data
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
	return 0;
	}
	
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *parsed_json = cJSON_Parse(json_str);
    free(json_str);
    
    if (!parsed_json) {
	return 0;
	}
	
    
    // Step 2: Verify it's an array
    if (!cJSON_IsArray(parsed_json)) {
        cJSON_Delete(parsed_json);
        return 0;
    }
    
    // Step 3: Get array size and process elements
    int array_size = cJSON_GetArraySize(parsed_json);
    
    // Process at least first two elements if available
    for (int i = 0; i < array_size && i < 2; i++) {
        cJSON *array_item = cJSON_GetArrayItem(parsed_json, i);
        if (!array_item || !cJSON_IsObject(array_item)) continue;
        
        // Get object items case sensitively
        cJSON *city_item = cJSON_GetObjectItemCaseSensitive(array_item, "City");
        cJSON *state_item = cJSON_GetObjectItemCaseSensitive(array_item, "State");
        
        // Create replacement items
        cJSON *new_city = cJSON_CreateString("FUZZED_CITY");
        cJSON *new_state = cJSON_CreateString("FUZZED_STATE");
        
        // Replace items in object
        if (city_item) {
            cJSON_ReplaceItemInObject(array_item, "City", new_city);
        } else {
            cJSON_Delete(new_city);
        }
        
        if (state_item) {
            cJSON_ReplaceItemInObject(array_item, "State", new_state);
        } else {
            cJSON_Delete(new_state);
        }
        
        // Add item reference to object
        cJSON *lat_item = cJSON_GetObjectItemCaseSensitive(array_item, "Latitude");
        if (lat_item) {
            cJSON_AddItemReferenceToObject(array_item, "LatitudeRef", lat_item);
        }
        
        // Add new item with case-sensitive key
        cJSON *new_item = cJSON_CreateNumber(42.0);
        cJSON_AddItemToObjectCS(array_item, "FuzzedKey", new_item);
        
        // Detach and delete an item
        cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(array_item, "precision");
        if (detached) {
            cJSON_Delete(detached);
        }
        
        // Add false value to object
        cJSON_AddFalseToObject(array_item, "IsFuzzed");
    }
    
    // Step 4: Create and manipulate arrays
    float float_array[] = {1.1f, 2.2f, 3.3f};
    cJSON *float_json_array = cJSON_CreateFloatArray(float_array, 3);
    
    int int_array[] = {10, 20, 30};
    cJSON *int_json_array = cJSON_CreateIntArray(int_array, 3);
    
    // Replace first array element if array has elements
    if (array_size > 0) {
        cJSON_ReplaceItemInArray(parsed_json, 0, float_json_array);
    } else {
        cJSON_Delete(float_json_array);
    }
    
    // Add int array as new element
    if (int_json_array) {
        cJSON_AddItemToArray(parsed_json, int_json_array);
    }
    
    // Step 5: Print and cleanup
    char *printed = cJSON_Print(parsed_json);
    if (printed) {
        // Write to output file (as per requirement)
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed, strlen(printed), 1, out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(printed);
    }
    
    // Clean up
    cJSON_Delete(parsed_json);
    
    return 0;
}