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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"cJSON_Duplicate_rec":[[2828,9,2828,17,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_IsInvalid","cJSON_CreateObject","cJSON_AddObjectToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_Duplicate","cJSON_ReplaceItemViaPointer","cJSON_CreateArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateNull","cJSON_AddItemToArray","cJSON_DetachItemFromArray","cJSON_AddItemReferenceToArray","cJSON_IsNull","cJSON_Delete","cJSON_AddRawToObject","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_CreateNull","cJSON_GetObjectItem","cJSON_IsObject","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_AddObjectToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_Duplicate","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateNull","cJSON_DetachItemFromArray","cJSON_AddItemReferenceToArray","cJSON_IsNull","cJSON_GetArrayItem","cJSON_AddRawToObject","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_CreateNull","cJSON_ReplaceItemViaPointer","cJSON_GetObjectItem","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_CreateObject","cJSON_AddObjectToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_Duplicate","cJSON_CreateArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateNull","cJSON_AddItemToArray","cJSON_DetachItemFromArray","cJSON_AddItemReferenceToArray","cJSON_IsNull","cJSON_AddRawToObject","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_CreateNull","cJSON_ReplaceItemViaPointer","cJSON_GetObjectItem","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_IsInvalid","cJSON_CreateObject","cJSON_AddObjectToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_Duplicate","cJSON_ReplaceItemViaPointer","cJSON_CreateArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateNull","cJSON_AddItemToArray","cJSON_DetachItemFromArray","cJSON_AddItemReferenceToArray","cJSON_IsNull","cJSON_Delete","cJSON_AddRawToObject","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_CreateNull","cJSON_GetObjectItem","cJSON_IsObject","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_CreateObject","cJSON_AddObjectToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_Duplicate","cJSON_CreateArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateNull","cJSON_AddItemToArray","cJSON_DetachItemFromArray","cJSON_AddItemReferenceToArray","cJSON_IsNull","cJSON_AddRawToObject","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_CreateNull","cJSON_ReplaceItemViaPointer","cJSON_GetObjectItem","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse input JSON using cJSON_ParseWithLength
    const char* json_str = reinterpret_cast<const char*>(data);
    cJSON* parsed = cJSON_ParseWithLength(json_str, size);
    
    // 2. Check if parsed item is invalid
    if (cJSON_IsInvalid(parsed)) {
        // Create a new object since parsing failed
        cJSON* new_obj = cJSON_CreateObject();
        
        // 3. Add an object to the new object
        cJSON* nested_obj = cJSON_AddObjectToObject(new_obj, "fuzzed_object");
        
        // 4. Create a float array
        float float_array_data[] = {1.1f, 2.2f, 3.3f};
        cJSON* float_array = cJSON_CreateFloatArray(float_array_data, 3);
        
        // 5. Add float array to nested object
        cJSON_AddItemToObject(nested_obj, "float_array", float_array);
        
        // 6. Get an item from the nested object
        cJSON* retrieved_item = cJSON_GetObjectItem(nested_obj, "float_array");
        
        // 7. Check if retrieved item is an object (it's actually an array)
        cJSON_bool is_obj = cJSON_IsObject(retrieved_item);
        
        // 8. Duplicate the nested object
        cJSON* duplicated = cJSON_Duplicate(nested_obj, 1);
        
        // 9. Replace an item via pointer (replace float_array with duplicated object)
        if (retrieved_item) {
            cJSON_ReplaceItemViaPointer(nested_obj, retrieved_item, duplicated);
        }
        
        // 10. Create a new array
        cJSON* new_array = cJSON_CreateArray();
        
        // 11. Add items to the array
        cJSON* array_string = cJSON_CreateString("fuzz_string");
        cJSON_AddItemToArray(new_array, array_string);
        
        cJSON* array_null = cJSON_CreateNull();
        cJSON_AddItemToArray(new_array, array_null);
        
        // 12. Detach an item from array
        cJSON* detached = cJSON_DetachItemFromArray(new_array, 0);
        
        // 13. Add item reference back to array
        cJSON_AddItemReferenceToArray(new_array, detached);
        
        // 14. Check if null item is null
        cJSON_bool is_null = cJSON_IsNull(array_null);
        
        // 15. Clean up detached item (no longer owned by array)
        cJSON_Delete(detached);
        
        // 16. Add raw string to object
        const char* raw_data = "raw_data";
        cJSON_AddRawToObject(new_obj, "raw_field", raw_data);
        
        // 17. Get array size
        int array_size = cJSON_GetArraySize(new_array);
        
        // 18. Get array item at index
        if (array_size > 0) {
            cJSON* array_item = cJSON_GetArrayItem(new_array, 0);
        }
        
        // 19. Create another null item
        cJSON* another_null = cJSON_CreateNull();
        
        // 20. Detach item via pointer from the nested object
        cJSON* to_detach = cJSON_GetObjectItem(nested_obj, "float_array");
        if (to_detach && cJSON_IsObject(to_detach)) {
            cJSON* detached_via_ptr = cJSON_DetachItemViaPointer(nested_obj, to_detach);
            if (detached_via_ptr) {
                cJSON_Delete(detached_via_ptr);
            }
        }
        
        // Clean up
        cJSON_Delete(new_array);
        cJSON_Delete(another_null);
        cJSON_Delete(new_obj);
    } else if (parsed) {
        // If parsing succeeded, we can work with the parsed object
        
        // Create a new object to add to the parsed one
        cJSON* new_object = cJSON_AddObjectToObject(parsed, "fuzzed_addition");
        
        // Create float array
        float float_data[] = {4.4f, 5.5f};
        cJSON* float_array = cJSON_CreateFloatArray(float_data, 2);
        
        // Add float array to new object
        cJSON_AddItemToObject(new_object, "fuzzed_floats", float_array);
        
        // Get an item from the original parsed object
        cJSON* widget_item = cJSON_GetObjectItem(parsed, "widget");
        
        // Check if it's an object
        cJSON_bool is_widget_obj = cJSON_IsObject(widget_item);
        
        // Duplicate the widget object
        cJSON* duplicated_widget = cJSON_Duplicate(widget_item, 1);
        
        // Create an array
        cJSON* new_array = cJSON_CreateArray();
        
        // Add items to array
        cJSON_AddItemToArray(new_array, cJSON_CreateString("array_string"));
        cJSON_AddItemToArray(new_array, cJSON_CreateNull());
        
        // Detach item from array
        cJSON* detached_array_item = cJSON_DetachItemFromArray(new_array, 0);
        
        // Add reference back
        cJSON_AddItemReferenceToArray(new_array, detached_array_item);
        
        // Check null
        cJSON_bool null_check = cJSON_IsNull(cJSON_GetArrayItem(new_array, 1));
        
        // Add raw data
        cJSON_AddRawToObject(parsed, "raw_addition", "raw_content");
        
        // Get array size
        int arr_size = cJSON_GetArraySize(new_array);
        
        // Get array item
        if (arr_size > 0) {
            cJSON* arr_item = cJSON_GetArrayItem(new_array, 0);
        }
        
        // Create null
        cJSON* null_item = cJSON_CreateNull();
        
        // Replace an item via pointer
        if (widget_item && duplicated_widget) {
            cJSON_ReplaceItemViaPointer(parsed, widget_item, duplicated_widget);
        }
        
        // Detach via pointer
        cJSON* to_detach_ptr = cJSON_GetObjectItem(parsed, "raw_addition");
        if (to_detach_ptr) {
            cJSON* detached_ptr_item = cJSON_DetachItemViaPointer(parsed, to_detach_ptr);
            if (detached_ptr_item) {
                cJSON_Delete(detached_ptr_item);
            }
        }
        
        // Clean up
        cJSON_Delete(new_array);
        cJSON_Delete(detached_array_item);
        cJSON_Delete(null_item);
        cJSON_Delete(parsed);
    }
    
    // If parsed was NULL from the beginning, we still need to use the APIs
    if (!parsed) {
        // Create a new object
        cJSON* root = cJSON_CreateObject();
        
        // Add object to object
        cJSON* child = cJSON_AddObjectToObject(root, "child");
        
        // Create float array
        float floats[] = {0.0f};
        cJSON* float_arr = cJSON_CreateFloatArray(floats, 1);
        
        // Add item to object
        cJSON_AddItemToObject(child, "floats", float_arr);
        
        // Get object item
        cJSON* retrieved = cJSON_GetObjectItem(child, "floats");
        
        // Check if object (it's actually an array)
        cJSON_bool obj_check = cJSON_IsObject(retrieved);
        
        // Duplicate
        cJSON* dup = cJSON_Duplicate(child, 0);
        
        // Create array
        cJSON* arr = cJSON_CreateArray();
        
        // Add items to array
        cJSON* str_item = cJSON_CreateString("test");
        cJSON_AddItemToArray(arr, str_item);
        
        cJSON* null_item = cJSON_CreateNull();
        cJSON_AddItemToArray(arr, null_item);
        
        // Detach from array
        cJSON* detached = cJSON_DetachItemFromArray(arr, 0);
        
        // Add reference back
        cJSON_AddItemReferenceToArray(arr, detached);
        
        // Check null
        cJSON_bool null_check = cJSON_IsNull(null_item);
        
        // Add raw
        cJSON_AddRawToObject(root, "raw", "data");
        
        // Get array size
        int arr_size = cJSON_GetArraySize(arr);
        
        // Get array item
        if (arr_size > 0) {
            cJSON* arr_elem = cJSON_GetArrayItem(arr, 0);
        }
        
        // Create another null
        cJSON* another_null = cJSON_CreateNull();
        
        // Replace via pointer
        cJSON_ReplaceItemViaPointer(root, child, dup);
        
        // Detach via pointer
        cJSON* to_detach = cJSON_GetObjectItem(root, "raw");
        if (to_detach) {
            cJSON* detached_ptr = cJSON_DetachItemViaPointer(root, to_detach);
            if (detached_ptr) {
                cJSON_Delete(detached_ptr);
            }
        }
        
        // Clean up
        cJSON_Delete(detached);
        cJSON_Delete(arr);
        cJSON_Delete(another_null);
        cJSON_Delete(root);
    }
    
    return 0;
}