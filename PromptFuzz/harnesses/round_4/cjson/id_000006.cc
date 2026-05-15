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
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"cJSON_Compare":[[3178,21,3178,38,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_Delete","cJSON_GetObjectItem","cJSON_AddBoolToObject","cJSON_IsObject","cJSON_IsBool","cJSON_CreateArray","cJSON_AddItemReferenceToArray","cJSON_GetArrayItem","cJSON_Compare","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsObject","cJSON_CreateRaw","cJSON_IsRaw","cJSON_SetValuestring","cJSON_free","cJSON_Delete","cJSON_CreateNumber","cJSON_SetNumberHelper","cJSON_AddItemToObject","cJSON_Compare","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_GetObjectItem","cJSON_AddBoolToObject","cJSON_IsObject","cJSON_IsBool","cJSON_CreateArray","cJSON_AddItemReferenceToArray","cJSON_GetArrayItem","cJSON_Compare","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsObject","cJSON_CreateRaw","cJSON_IsRaw","cJSON_SetValuestring","cJSON_free","cJSON_Delete","cJSON_CreateNumber","cJSON_SetNumberHelper","cJSON_AddItemToObject","cJSON_Compare","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
	//fuzzer shim end}




    // Step 1: Parse input data as JSON
    cJSON *parsed = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!parsed) {
        return 0;
    }

    // Step 2: Create a new object for comparison and modification
    cJSON *new_obj = cJSON_CreateObject();
    if (!new_obj) {
        cJSON_Delete(parsed);
        return 0;
    }

    // Step 3: Get "menu" object from parsed JSON
    cJSON *menu = cJSON_GetObjectItem(parsed, fuzz_str_1);
    if (menu) {
        // Step 4: Add boolean item to new object
        cJSON *bool_item = cJSON_AddBoolToObject(new_obj, fuzz_str_2, cJSON_IsObject(menu));
        
        // Step 5: Check if the boolean item is indeed boolean
        if (cJSON_IsBool(bool_item)) {
            // Step 6: Create an array and add reference to it
            cJSON *array = cJSON_CreateArray();
            if (array) {
                // Add reference of bool_item to array
                cJSON_AddItemReferenceToArray(array, bool_item);
                
                // Step 7: Compare the original bool_item with the one in array
                cJSON *array_bool = cJSON_GetArrayItem(array, fuzz_int32_t_3);
                if (array_bool) {
                    cJSON_Compare(bool_item, array_bool, true);
                }
                
                cJSON_Delete(array);
            }
        }
    }

    // Step 8: Get "popup" object if it exists
    if (menu) {
        cJSON *popup = cJSON_GetObjectItem(menu, fuzz_str_4);
        if (popup && cJSON_IsObject(popup)) {
            // Step 9: Create a raw JSON string and check if it's raw
            cJSON *raw_item = cJSON_CreateRaw(fuzz_str_5);
            if (raw_item) {
                // This should return false since it's not a raw type in this context
                // but we still call the API
                cJSON_IsRaw(raw_item);
                
                // Step 10: Set value string on the raw item
                char *old_val = cJSON_SetValuestring(raw_item, fuzz_str_6);
                if (old_val) {
                    // Free the old value string if needed
                    cJSON_free(old_val);
                }
                
                cJSON_Delete(raw_item);
            }
        }
    }

    // Step 11: Create a number item and use SetNumberHelper
    cJSON *num_item = cJSON_CreateNumber(42.5);
    if (num_item) {
        // Set new number value
        cJSON_SetNumberHelper(num_item, 99.9);
        
        // Add to new_obj
        cJSON_AddItemToObject(new_obj, fuzz_str_7, num_item);
    }

    // Step 12: Compare the original parsed object with our new object
    cJSON_Compare(parsed, new_obj, true);

    // Step 13: Clean up
    cJSON_Delete(parsed);
    cJSON_Delete(new_obj);

    return 0;
}