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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 43.5, nr_unique_branch: 3
//<Quality> {"density":29,"unique_branches":{"cJSON_IsInvalid":[[2974,9,2974,21,0,0,4,0],[2974,9,2974,21,0,0,4,1]],"cJSON_DetachItemViaPointer":[[2260,9,2260,25,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_IsInvalid","cJSON_Delete","cJSON_CreateObject","cJSON_AddArrayToObject","cJSON_AddObjectToObject","cJSON_CreateObjectReference","cJSON_AddItemToArray","cJSON_CreateArray","cJSON_AddNullToObject","cJSON_IsNull","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_HasObjectItem","cJSON_ReplaceItemInArray","cJSON_Duplicate","cJSON_DetachItemFromObject","cJSON_AddItemToObject","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_AddItemReferenceToArray","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_IsInvalid","cJSON_CreateObject","cJSON_AddArrayToObject","cJSON_AddObjectToObject","cJSON_CreateObjectReference","cJSON_AddItemToArray","cJSON_CreateArray","cJSON_AddNullToObject","cJSON_IsNull","cJSON_HasObjectItem","cJSON_ReplaceItemInArray","cJSON_Duplicate","cJSON_DetachItemFromObject","cJSON_AddItemToObject","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_AddItemReferenceToArray","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


// Include necessary cjson headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse input data using cJSON_ParseWithLength
    cJSON *parsed_json = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    
    // 2. Check if parsed JSON is invalid
    if (cJSON_IsInvalid(parsed_json)) {
        cJSON_Delete(parsed_json);
        return 0;
    }
    
    // 3. Create a new object
    cJSON *new_object = cJSON_CreateObject();
    
    // 4. Create an array and add it to the new object
    cJSON *array = cJSON_AddArrayToObject(new_object, "test_array");
    
    // 5. Create another object and add it to the new object
    cJSON *nested_object = cJSON_AddObjectToObject(new_object, "test_object");
    
    // 6. Create an object reference from parsed JSON
    cJSON *object_ref = cJSON_CreateObjectReference(parsed_json);
    
    // 7. Add the object reference to the array
    cJSON_AddItemToArray(array, object_ref);
    
    // 8. Create a new array
    cJSON *another_array = cJSON_CreateArray();
    
    // 9. Add a null item to the nested object
    cJSON *null_item = cJSON_AddNullToObject(nested_object, "null_field");
    
    // 10. Check if the null item is indeed null
    if (!cJSON_IsNull(null_item)) {
        // This shouldn't happen, but handle just in case
        cJSON_Delete(parsed_json);
        cJSON_Delete(new_object);
        cJSON_Delete(another_array);
        return 0;
    }
    
    // 11. Check if parsed JSON has "widget" object
    if (cJSON_HasObjectItem(parsed_json, "widget")) {
        // 12. Replace first item in array with parsed JSON
        cJSON_ReplaceItemInArray(array, 0, cJSON_Duplicate(parsed_json, 1));
    }
    
    // 13. Detach an item from the parsed JSON if it has "widget"
    cJSON *detached_item = cJSON_DetachItemFromObject(parsed_json, "widget");
    if (detached_item) {
        // 14. Add detached item to the new object
        cJSON_AddItemToObject(new_object, "detached_widget", detached_item);
    }
    
    // 15. Detach an item via pointer from array
    if (cJSON_GetArraySize(array) > 0) {
        cJSON *first_array_item = cJSON_GetArrayItem(array, 0);
        cJSON *detached_via_pointer = cJSON_DetachItemViaPointer(array, first_array_item);
        if (detached_via_pointer) {
            cJSON_Delete(detached_via_pointer);
        }
    }
    
    // 16. Add item reference to another array
    if (parsed_json) {
        cJSON_AddItemReferenceToArray(another_array, parsed_json);
    }
    
    // 17. Print the new object in unformatted JSON
    char *printed_json = cJSON_PrintUnformatted(new_object);
    
    // 18. Free the printed string
    cJSON_free(printed_json);
    
    // 19. Clean up all cJSON objects
    cJSON_Delete(parsed_json);
    cJSON_Delete(new_object);
    cJSON_Delete(another_array);
    
    return 0;
}