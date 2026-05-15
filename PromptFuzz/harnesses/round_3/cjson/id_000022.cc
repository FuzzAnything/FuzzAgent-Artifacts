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
//<ID> 87
//<Prompt> []
/*<Combination>: [
*/
//<score> 63, nr_unique_branch: 2
//<Quality> {"density":21,"unique_branches":{"cJSON_AddObjectToObject":[[2237,9,2237,76,0,0,4,1]],"add_item_to_object":[[2087,9,2087,25,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_IsArray","cJSON_Delete","cJSON_GetArraySize","cJSON_Delete","cJSON_AddObjectToObject","cJSON_AddStringToObject","cJSON_GetArrayItem","cJSON_IsObject","cJSON_AddObjectToObject","cJSON_GetObjectItem","cJSON_AddStringToObject","cJSON_DetachItemFromArray","cJSON_ReplaceItemInArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_Print","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_IsArray","cJSON_GetArraySize","cJSON_AddObjectToObject","cJSON_AddStringToObject","cJSON_GetArrayItem","cJSON_IsObject","cJSON_AddObjectToObject","cJSON_GetObjectItem","cJSON_AddStringToObject","cJSON_DetachItemFromArray","cJSON_ReplaceItemInArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<92) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
	//fuzzer shim end}




    // Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read the input data into a buffer
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(json_str, 1, size, in_file);
    json_str[bytes_read] = '\0';
    assert_file_closed(&in_file);;
    
    // Parse the JSON string
    cJSON *root = cJSON_ParseWithLength(json_str, bytes_read);
    if (!root) {
        free(json_str);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Check if the root is an array
    if (!cJSON_IsArray(root)) {
        cJSON_Delete(root);
        free(json_str);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get array size
    int array_size = cJSON_GetArraySize(root);
    if (array_size < 2) {
        cJSON_Delete(root);
        free(json_str);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a new object to add to the array elements
    cJSON *new_object = cJSON_AddObjectToObject(NULL, fuzz_str_1);
    if (new_object) {
        // Add a string to the new object
        cJSON_AddStringToObject(new_object, fuzz_str_2, fuzz_str_3);
        
        // Add the new object to each array element
        for (int i = 0; i < array_size; i++) {
            cJSON *item = cJSON_GetArrayItem(root, fuzz_int32_t_4);
            if (cJSON_IsObject(item)) {
                cJSON_AddObjectToObject(item, fuzz_str_5);
                cJSON *metadata = cJSON_GetObjectItem(item, fuzz_str_6);
                if (metadata) {
                    cJSON_AddStringToObject(metadata, fuzz_str_7, fuzz_str_8);
                }
            }
        }
    }
    
    // Detach the first item from the array
    cJSON *detached_item = cJSON_DetachItemFromArray(root, fuzz_int32_t_9);
    
    // Replace the second item (now at index 0 after detachment) with the detached item
    if (detached_item && array_size > 1) {
        cJSON_ReplaceItemInArray(root, 0, detached_item);
    }
    
    // Update a number value in the first object
    if (cJSON_GetArraySize(root) > 0) {
        cJSON *first_item = cJSON_GetArrayItem(root, fuzz_int32_t_10);
        if (first_item && cJSON_IsObject(first_item)) {
            cJSON *latitude = cJSON_GetObjectItem(first_item, fuzz_str_11);
            if (latitude && cJSON_IsNumber(latitude)) {
                // Use cJSON_SetNumberHelper to update the value
                cJSON_SetNumberHelper(latitude, 42.0);
            }
        }
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Print the modified JSON
        char *output = cJSON_Print(root);
        if (output) {
            fwrite(output, 1, strlen(output), out_file);
            cJSON_free(output);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up
    cJSON_Delete(root);
    free(json_str);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}