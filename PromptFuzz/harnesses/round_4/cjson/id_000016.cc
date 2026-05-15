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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_Delete","cJSON_AddNullToObject","cJSON_AddRawToObject","cJSON_AddArrayToObject","cJSON_CreateFloatArray","cJSON_AddItemToArray","cJSON_Delete","cJSON_AddBoolToObject","cJSON_Duplicate","cJSON_GetObjectItem","cJSON_ReplaceItemViaPointer","cJSON_AddItemToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_Print","cJSON_free","cJSON_GetObjectItem","cJSON_IsBool","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_AddNullToObject","cJSON_AddRawToObject","cJSON_AddArrayToObject","cJSON_CreateFloatArray","cJSON_AddItemToArray","cJSON_AddBoolToObject","cJSON_Duplicate","cJSON_GetObjectItem","cJSON_ReplaceItemViaPointer","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_Print","cJSON_free","cJSON_GetObjectItem","cJSON_IsBool","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<187) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeFloatingArray(fuzz_float_array_5, fuzz_float_array_size_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeFloatingArray(fuzz_float_array_11, fuzz_float_array_size_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
	
		const float * fuzzer_var_cJSON_CreateFloatArray_6_0 = fuzz_float_array_5;
		int fuzzer_size_cJSON_CreateFloatArray_6_0 = static_cast<int>(fuzz_float_array_size_5);
		const float * fuzzer_var_cJSON_CreateFloatArray_14_0 = fuzz_float_array_11;
		int fuzzer_size_cJSON_CreateFloatArray_14_0 = static_cast<int>(fuzz_float_array_size_11);//fuzzer shim end}




    // Step 1: Parse the input data as JSON
    cJSON *parsed = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!parsed) {
        // If parsing fails, we can still proceed with creating new objects
        // but we won't have a valid parsed object to work with
    }
    
    // Step 2: Create a new object to work with
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        cJSON_Delete(parsed);
        return 0;
    }
    
    // Step 3: Add a null value to the object using cJSON_AddNullToObject
    cJSON_AddNullToObject(root, fuzz_str_1);
    
    // Step 4: Add a raw JSON string to the object using cJSON_AddRawToObject
    const char *raw_json = "{\"raw\":\"data\"}";
    cJSON_AddRawToObject(root, fuzz_str_2, fuzz_str_3);
    
    // Step 5: Add an array to the object using cJSON_AddArrayToObject
    cJSON *array_obj = cJSON_AddArrayToObject(root, fuzz_str_4);
    
    // Step 6: Create a float array using cJSON_CreateFloatArray
    float float_numbers[] = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f};
    int count = 5;
    cJSON *float_array = cJSON_CreateFloatArray(fuzzer_var_cJSON_CreateFloatArray_6_0, fuzzer_size_cJSON_CreateFloatArray_6_0);
    
    // Step 7: Add the float array to our array object
    if (array_obj && float_array) {
        cJSON_AddItemToArray(array_obj, float_array);
    } else {
        if (float_array) cJSON_Delete(float_array);
    }
    
    // Step 8: Add a boolean to the object using cJSON_AddBoolToObject
    cJSON_AddBoolToObject(root, fuzz_str_6, fuzz_int32_t_7); // true
    
    // Step 9: Duplicate the parsed input (if it was valid) using cJSON_Duplicate
    cJSON *duplicated = NULL;
    if (parsed) {
        duplicated = cJSON_Duplicate(parsed, fuzz_int32_t_8); // Deep copy
    }
    
    // Step 10: Replace an item in the root object using cJSON_ReplaceItemViaPointer
    // Find the null field we added earlier
    cJSON *null_field = cJSON_GetObjectItem(root, fuzz_str_9);
    if (null_field && duplicated) {
        // Replace the null field with the duplicated object
        cJSON_ReplaceItemViaPointer(root, null_field, duplicated);
    } else {
        // If we couldn't find the null field or don't have a duplicated object,
        // add the duplicated object as a new field
        if (duplicated) {
            cJSON_AddItemToObject(root, fuzz_str_10, duplicated);
        }
    }
    
    // Step 11: Create another float array and add it directly to root
    float more_floats[] = {6.6f, 7.7f, 8.8f};
    cJSON *another_float_array = cJSON_CreateFloatArray(fuzzer_var_cJSON_CreateFloatArray_14_0, fuzzer_size_cJSON_CreateFloatArray_14_0);
    if (another_float_array) {
        cJSON_AddItemToObject(root, fuzz_str_12, another_float_array);
    }
    
    // Step 12: Verify our structure by printing it (optional, for debugging)
    char *printed = cJSON_Print(root);
    if (printed) {
        // We could write this to a file if needed
        // FILE *out_file = fopen("output_file", "wb");
        // if (out_file) {
        //     fwrite(printed, 1, strlen(printed), out_file);
        //     assert_file_closed(&out_file);;
        // }
        cJSON_free(printed);
    }
    
    // Step 13: Test the boolean field we added
    cJSON *bool_field = cJSON_GetObjectItem(root, fuzz_str_13);
    if (bool_field) {
        // Verify it's actually a boolean
        if (!cJSON_IsBool(bool_field)) {
            // This shouldn't happen if our code is correct
        }
    }
    
    // Step 14: Clean up all resources
    cJSON_Delete(root);
    // Note: duplicated is already part of root if it was replaced,
    // or was added as a child, so it will be deleted with root
    
    if (parsed) {
        cJSON_Delete(parsed);
    }
    
    return 0;
}