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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_CreateArray","cJSON_AddObjectToObject","cJSON_GetObjectItem","cJSON_AddItemToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsFalse","cJSON_DeleteItemFromObject","cJSON_GetObjectItemCaseSensitive","cJSON_GetStringValue","cJSON_CreateIntArray","cJSON_AddItemToArray","cJSON_AddObjectToObject","cJSON_Print","cJSON_free","cJSON_malloc","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_CreateArray","cJSON_AddObjectToObject","cJSON_GetObjectItem","cJSON_AddItemToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsFalse","cJSON_DeleteItemFromObject","cJSON_GetObjectItemCaseSensitive","cJSON_GetStringValue","cJSON_CreateIntArray","cJSON_AddItemToArray","cJSON_AddObjectToObject","cJSON_Print","cJSON_free","cJSON_malloc","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse the input data as JSON
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
	return 0;
	}
	
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *root = cJSON_Parse(json_str);
    free(json_str);
    
    if (!root) {
	return 0;
	}
	
    
    // 2. Create a new object (cJSON_CreateObject)
    cJSON *new_obj = cJSON_CreateObject();
    
    // 3. Create an array (cJSON_CreateArray)
    cJSON *new_array = cJSON_CreateArray();
    
    // 4. Add the array to the new object as "numbers" (cJSON_AddObjectToObject)
    cJSON_AddObjectToObject(new_obj, "numbers");
    // Actually cJSON_AddObjectToObject returns the new object, so we need to get it properly
    cJSON *numbers_obj = cJSON_GetObjectItem(new_obj, "numbers");
    if (numbers_obj) {
        // Add the array to this numbers object
        cJSON_AddItemToObject(numbers_obj, "data", new_array);
    }
    
    // 5. Check if the "interlace" field is false (cJSON_IsFalse)
    cJSON *interlace = cJSON_GetObjectItemCaseSensitive(root, "interlace");
    if (interlace && cJSON_IsFalse(interlace)) {
        // 6. Delete the "interlace" item from object (cJSON_DeleteItemFromObject)
        cJSON_DeleteItemFromObject(root, "interlace");
    }
    
    // 7. Get string value of "name" field (cJSON_GetStringValue)
    cJSON *name = cJSON_GetObjectItemCaseSensitive(root, "name");
    char *name_value = NULL;
    if (name) {
        name_value = cJSON_GetStringValue(name);
        // We can optionally use the name_value here
    }
    
    // 8. Create an integer array (cJSON_CreateIntArray)
    int int_numbers[] = {1, 2, 3, 4, 5};
    cJSON *int_array = cJSON_CreateIntArray(int_numbers, 5);
    if (int_array && new_array) {
        // Add the integer array to our new array
        cJSON_AddItemToArray(new_array, int_array);
    }
    
    // 9. Create another array to test with cJSON_AddObjectToObject again
    cJSON_AddObjectToObject(new_obj, "metadata");
    
    // 10. Clean up all allocated resources
    // Print and free some items to test the APIs
    char *printed = cJSON_Print(root);
    if (printed) {
        cJSON_free(printed);
    }
    
    // Test cJSON_free with a dummy allocation
    void *dummy = cJSON_malloc(100);
    if (dummy) {
        cJSON_free(dummy);
    }
    
    // Clean up all JSON objects
    cJSON_Delete(root);
    cJSON_Delete(new_obj);
    
    // Note: new_array and int_array are now owned by new_obj, 
    // so they will be deleted when new_obj is deleted
    
    return 0;
}