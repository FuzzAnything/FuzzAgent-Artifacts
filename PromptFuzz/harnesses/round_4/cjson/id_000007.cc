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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 37
//<Quality> {"density":18,"unique_branches":{"minify_string":[[2908,12,2908,31,0,0,4,0],[2908,12,2908,31,0,0,4,1],[2911,13,2911,32,0,0,4,0],[2911,13,2911,32,0,0,4,1],[2916,20,2916,41,0,0,4,0],[2916,20,2916,41,0,0,4,1],[2916,45,2916,66,0,0,4,0],[2916,45,2916,66,0,0,4,1]],"skip_oneline_comment":[[2879,12,2879,31,0,0,4,0],[2881,13,2881,32,0,0,4,0],[2881,13,2881,32,0,0,4,1]],"cJSON_Minify":[[2928,9,2928,21,0,0,4,1],[2933,12,2933,27,0,0,4,0],[2933,12,2933,27,0,0,4,1],[2937,13,2937,21,0,0,4,0],[2937,13,2937,21,0,0,4,1],[2938,13,2938,22,0,0,4,0],[2938,13,2938,22,0,0,4,1],[2939,13,2939,22,0,0,4,1],[2940,13,2940,22,0,0,4,0],[2940,13,2940,22,0,0,4,1],[2944,13,2944,21,0,0,4,0],[2944,13,2944,21,0,0,4,1],[2945,21,2945,35,0,0,4,0],[2945,21,2945,35,0,0,4,1],[2949,26,2949,40,0,0,4,0],[2949,26,2949,40,0,0,4,1],[2957,13,2957,22,0,0,4,0],[2957,13,2957,22,0,0,4,1],[2961,13,2961,20,0,0,4,0],[2961,13,2961,20,0,0,4,1]],"skip_multiline_comment":[[2892,12,2892,31,0,0,4,0],[2892,12,2892,31,0,0,4,1],[2894,13,2894,33,0,0,4,0],[2894,13,2894,33,0,0,4,1],[2894,37,2894,57,0,0,4,0],[2894,37,2894,57,0,0,4,1]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateFloatArray","cJSON_CreateNull","cJSON_IsNull","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_AddItemReferenceToObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateString","cJSON_DetachItemFromArray","cJSON_IsInvalid","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Print","cJSON_Minify","cJSON_free","cJSON_CreateString","cJSON_AddItemToObjectCS","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateFloatArray","cJSON_CreateNull","cJSON_IsNull","cJSON_AddItemReferenceToObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateString","cJSON_DetachItemFromArray","cJSON_IsInvalid","cJSON_Delete","cJSON_Delete","cJSON_Print","cJSON_Minify","cJSON_free","cJSON_CreateString","cJSON_AddItemToObjectCS","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


// Include cJSON headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<153) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeFloatingArray(fuzz_float_array_1, fuzz_float_array_size_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
	
		const float * fuzzer_var_cJSON_CreateFloatArray_1_0 = fuzz_float_array_1;
		int fuzzer_size_cJSON_CreateFloatArray_1_0 = static_cast<int>(fuzz_float_array_size_1);//fuzzer shim end}




    // Step 1: Parse the input JSON data
    cJSON *parsed = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!parsed) {
        // If parsing fails, return early
        return 0;
    }

    // Step 2: Create a float array using cJSON_CreateFloatArray
    float float_array[] = {1.1f, 2.2f, 3.3f, 4.4f};
    cJSON *float_array_obj = cJSON_CreateFloatArray(fuzzer_var_cJSON_CreateFloatArray_1_0, fuzzer_size_cJSON_CreateFloatArray_1_0);
    
    // Step 3: Create a null object using cJSON_CreateNull
    cJSON *null_obj = cJSON_CreateNull();
    
    // Step 4: Check if the null object is indeed null using cJSON_IsNull
    if (!cJSON_IsNull(null_obj)) {
        // Clean up and return if check fails
        cJSON_Delete(float_array_obj);
        cJSON_Delete(null_obj);
        cJSON_Delete(parsed);
        return 0;
    }
    
    // Step 5: Add the float array as a reference to the parsed object using cJSON_AddItemReferenceToObject
    // We'll add it with key "float_array_ref"
    cJSON_AddItemReferenceToObject(parsed, fuzz_str_2, float_array_obj);
    
    // Step 6: Check if a detached item is invalid using cJSON_IsInvalid
    // First, ensure parsed has an array to detach from
    cJSON *array_item = cJSON_GetObjectItem(parsed, fuzz_str_3);
    if (array_item && cJSON_IsObject(array_item)) {
        cJSON *window = cJSON_GetObjectItem(array_item, fuzz_str_4);
        if (window && cJSON_IsObject(window)) {
            // Create an array to test detachment
            cJSON *test_array = cJSON_CreateArray();
            cJSON_AddItemToArray(test_array, cJSON_CreateString(fuzz_str_5));
            cJSON_AddItemToArray(test_array, cJSON_CreateString(fuzz_str_6));
            
            // Detach an item from the array
            cJSON *detached = cJSON_DetachItemFromArray(test_array, fuzz_int32_t_7);
            
            // Check if the detached item is invalid (it shouldn't be)
            if (cJSON_IsInvalid(detached)) {
                // Handle invalid case
                cJSON_Delete(detached);
            } else {
                // Reattach or clean up
                cJSON_Delete(detached);
            }
            
            // Clean up test array
            cJSON_Delete(test_array);
        }
    }
    
    // Step 7: Minify a JSON string using cJSON_Minify
    char *json_string = cJSON_Print(parsed);
    if (json_string) {
        cJSON_Minify(json_string);
        // Clean up the string
        cJSON_free(json_string);
    }
    
    // Step 8: Add an item to object with case-sensitive key using cJSON_AddItemToObjectCS
    cJSON *cs_item = cJSON_CreateString(fuzz_str_8);
    cJSON_AddItemToObjectCS(parsed, fuzz_str_9, cs_item);
    
    // Step 9: Verify the added item exists
    cJSON *retrieved_cs = cJSON_GetObjectItemCaseSensitive(parsed, fuzz_str_10);
    if (!retrieved_cs) {
        // If not found, try with different case
        retrieved_cs = cJSON_GetObjectItemCaseSensitive(parsed, fuzz_str_11);
    }
    
    // Step 10: Clean up all resources
    cJSON_Delete(float_array_obj); // Note: This is a reference, but we still need to free the original
    cJSON_Delete(null_obj);
    cJSON_Delete(parsed); // This will also clean up cs_item and the reference to float_array_obj
    
    return 0;
}