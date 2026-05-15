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
//<ID> 86
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 1
//<Quality> {"density":24,"unique_branches":{"cJSON_IsNumber":[[3024,9,3024,21,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_IsArray","cJSON_Delete","cJSON_GetArraySize","cJSON_Delete","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_CreateObject","cJSON_Delete","cJSON_Delete","cJSON_AddObjectToObject","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_AddStringToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_ReplaceItemInArray","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_IsArray","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_CreateObject","cJSON_AddObjectToObject","cJSON_AddStringToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_ReplaceItemInArray","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


// Include cJSON header

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<58) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    // Parse the input JSON data
    cJSON *root = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (root == nullptr) {
        // If parsing fails, return early
        return 0;
    }

    // Ensure root is an array (as per the input example)
    if (!cJSON_IsArray(root)) {
        cJSON_Delete(root);
        return 0;
    }

    // Get array size
    int array_size = cJSON_GetArraySize(root);
    if (array_size < 2) {
        cJSON_Delete(root);
        return 0;
    }

    // 1. Detach an item from the array (second item)
    cJSON *detached_item = cJSON_DetachItemFromArray(root, fuzz_int32_t_1);
    if (detached_item == nullptr) {
        cJSON_Delete(root);
        return 0;
    }

    // 2. Create a new object to replace the detached item
    cJSON *new_item = cJSON_CreateObject();
    if (new_item == nullptr) {
        cJSON_Delete(detached_item);
        cJSON_Delete(root);
        return 0;
    }

    // 3. Add a nested object to the new item
    cJSON *nested_object = cJSON_AddObjectToObject(new_item, fuzz_str_2);
    if (nested_object == nullptr) {
        cJSON_Delete(new_item);
        cJSON_Delete(detached_item);
        cJSON_Delete(root);
        return 0;
    }

    // 4. Add a string to the nested object
    cJSON_AddStringToObject(nested_object, fuzz_str_3, fuzz_str_4);

    // 5. Use cJSON_SetNumberHelper to modify a number in the detached item
    // Find a number field in the detached item (e.g., Latitude or Longitude)
    cJSON *latitude = cJSON_GetObjectItemCaseSensitive(detached_item, fuzz_str_5);
    if (cJSON_IsNumber(latitude)) {
        // Modify the latitude value using cJSON_SetNumberHelper
        cJSON_SetNumberHelper(latitude, 42.0); // Set to a fixed value for demonstration
    }

    // 6. Replace the first array item with the new item
    cJSON_bool replace_success = cJSON_ReplaceItemInArray(root, fuzz_int32_t_6, new_item);
    if (!replace_success) {
        cJSON_Delete(new_item);
        cJSON_Delete(detached_item);
        cJSON_Delete(root);
        return 0;
    }

    // Cleanup: Delete the detached item and the root (which includes new_item)
    cJSON_Delete(detached_item);
    cJSON_Delete(root);

    return 0;
}