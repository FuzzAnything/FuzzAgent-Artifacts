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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_IsArray","cJSON_Delete","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_CreateFloatArray","cJSON_CreateIntArray","cJSON_CreateString","cJSON_ReplaceItemInObject","cJSON_AddItemReferenceToObject","cJSON_CreateNumber","cJSON_AddItemToObjectCS","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_AddFalseToObject","cJSON_Delete","cJSON_Delete","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_ReplaceItemInArray","cJSON_Print","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_CreateFloatArray","cJSON_CreateIntArray","cJSON_CreateString","cJSON_ReplaceItemInObject","cJSON_AddItemReferenceToObject","cJSON_CreateNumber","cJSON_AddItemToObjectCS","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_AddFalseToObject","cJSON_Delete","cJSON_Delete","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_ReplaceItemInArray","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse JSON from input data
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
	
    
    // 2. Check if root is an array
    if (!cJSON_IsArray(root)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 3. Get array size and process elements
    int array_size = cJSON_GetArraySize(root);
    
    // Create some test arrays for demonstration
    float float_arr[3] = {1.1f, 2.2f, 3.3f};
    int int_arr[3] = {10, 20, 30};
    
    // Process each element in array
    for (int i = 0; i < array_size && i < 2; i++) {
        cJSON *item = cJSON_GetArrayItem(root, i);
        if (!item) continue;
        
        // 4. Get object items case sensitively
        cJSON *precision = cJSON_GetObjectItemCaseSensitive(item, "precision");
        cJSON *latitude = cJSON_GetObjectItemCaseSensitive(item, "Latitude");
        cJSON *longitude = cJSON_GetObjectItemCaseSensitive(item, "Longitude");
        
        // 5. Create new arrays for replacement
        cJSON *float_array = cJSON_CreateFloatArray(float_arr, 3);
        cJSON *int_array = cJSON_CreateIntArray(int_arr, 3);
        
        // 6. Replace items in object
        if (precision) {
            cJSON *new_precision = cJSON_CreateString("replaced");
            cJSON_ReplaceItemInObject(item, "precision", new_precision);
        }
        
        // 7. Add item references
        if (latitude) {
            cJSON_AddItemReferenceToObject(item, "LatitudeRef", latitude);
        }
        
        // 8. Add new item with CS
        cJSON *new_field = cJSON_CreateNumber(42.0);
        cJSON_AddItemToObjectCS(item, "NewField", new_field);
        
        // 9. Detach item
        cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(item, "City");
        if (detached) {
            cJSON_Delete(detached); // Free detached item
        }
        
        // 10. Add false boolean
        cJSON_AddFalseToObject(item, "Processed");
        
        // Cleanup arrays (they're owned by item now if added)
        // But we didn't add them to item, so need to free
        if (float_array) cJSON_Delete(float_array);
        if (int_array) cJSON_Delete(int_array);
    }
    
    // 11. Replace item in array (replace first element with new object)
    if (array_size > 0) {
        cJSON *new_obj = cJSON_CreateObject();
        cJSON_AddStringToObject(new_obj, "test", "replacement");
        cJSON_ReplaceItemInArray(root, 0, new_obj);
    }
    
    // 12. Print and free
    char *printed = cJSON_Print(root);
    if (printed) {
        // Could write to file if needed
        cJSON_free(printed);
    }
    
    // 13. Cleanup
    cJSON_Delete(root);
    
    return 0;
}