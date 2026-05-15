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
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 120, nr_unique_branch: 13
//<Quality> {"density":24,"unique_branches":{"cJSON_AddNullToObject":[[2153,9,2153,69,0,0,4,0]],"cJSON_AddItemReferenceToArray":[[2132,9,2132,22,0,0,4,1]],"cJSON_CreateIntArray":[[2628,9,2628,20,0,0,4,1],[2628,24,2628,41,0,0,4,1],[2635,16,2635,17,0,0,4,0],[2635,21,2635,40,0,0,4,0],[2635,21,2635,40,0,0,4,1],[2638,13,2638,15,0,0,4,1],[2643,12,2643,14,0,0,4,0],[2643,12,2643,14,0,0,4,1],[2654,9,2654,10,0,0,4,0],[2654,14,2654,22,0,0,4,0]],"cJSON_CreateArrayReference":[[2573,9,2573,21,0,0,4,0]]},"library_calls":["cJSON_malloc","cJSON_Parse","cJSON_free","cJSON_GetObjectItem","cJSON_Delete","cJSON_free","cJSON_GetObjectItem","cJSON_IsArray","cJSON_Delete","cJSON_free","cJSON_CreateFloatArray","cJSON_CreateIntArray","cJSON_CreateNumber","cJSON_CreateArrayReference","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_CreateArray","cJSON_AddItemReferenceToArray","cJSON_Delete","cJSON_GetArraySize","cJSON_CreateString","cJSON_ReplaceItemInArray","cJSON_AddNullToObject","cJSON_AddBoolToObject","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_Parse","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsArray","cJSON_CreateFloatArray","cJSON_CreateIntArray","cJSON_CreateNumber","cJSON_CreateArrayReference","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_CreateArray","cJSON_AddItemReferenceToArray","cJSON_Delete","cJSON_GetArraySize","cJSON_CreateString","cJSON_ReplaceItemInArray","cJSON_AddNullToObject","cJSON_AddBoolToObject","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<107) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeFloatingArray(fuzz_float_array_3, fuzz_float_array_size_3, fdp);
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_4, fuzz_int32_t_array_size_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
	
		const float * fuzzer_var_cJSON_CreateFloatArray_10_0 = fuzz_float_array_3;
		int fuzzer_size_cJSON_CreateFloatArray_10_0 = static_cast<int>(fuzz_float_array_size_3);
		const int * fuzzer_var_cJSON_CreateIntArray_11_0 = fuzz_int32_t_array_4;
		int fuzzer_size_cJSON_CreateIntArray_11_0 = static_cast<int>(fuzz_int32_t_array_size_4);//fuzzer shim end}




    // Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the entire input into a buffer
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *input_buffer = (char *)cJSON_malloc(file_size + 1);
    if (!input_buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(input_buffer, 1, file_size, in_file);
    input_buffer[bytes_read] = '\0';
    assert_file_closed(&in_file);;

    // Parse the input JSON
    cJSON *root = cJSON_Parse(input_buffer);
    if (!root) {
        cJSON_free(input_buffer);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the "menu" object
    cJSON *menu = cJSON_GetObjectItem(root, fuzz_str_1);
    if (!menu) {
        cJSON_Delete(root);
        cJSON_free(input_buffer);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the "items" array from menu
    cJSON *items = cJSON_GetObjectItem(menu, fuzz_str_2);
    if (!items || !cJSON_IsArray(items)) {
        cJSON_Delete(root);
        cJSON_free(input_buffer);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a float array for demonstration
    float float_numbers[3] = {1.1f, 2.2f, 3.3f};
    cJSON *float_array = cJSON_CreateFloatArray(fuzzer_var_cJSON_CreateFloatArray_10_0, fuzzer_size_cJSON_CreateFloatArray_10_0);
    
    // Create an int array for demonstration
    int int_numbers[4] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(fuzzer_var_cJSON_CreateIntArray_11_0, fuzzer_size_cJSON_CreateIntArray_11_0);
    
    // Create a number
    cJSON *number_item = cJSON_CreateNumber(42.5);
    
    // Create an array reference for items array
    cJSON *array_ref = cJSON_CreateArrayReference(items);
    
    // Add item reference to array (add reference to the first item in items array)
    if (cJSON_GetArraySize(items) > 0) {
        cJSON *first_item = cJSON_GetArrayItem(items, fuzz_int32_t_5);
        if (first_item) {
            // Create a new array to add the reference to
            cJSON *new_array = cJSON_CreateArray();
            cJSON_AddItemReferenceToArray(new_array, first_item);
            cJSON_Delete(new_array);
        }
    }

    // Replace an item in the array (replace second item if it exists)
    if (cJSON_GetArraySize(items) >= 2) {
        // Create a new item to replace with
        cJSON *replacement = cJSON_CreateString(fuzz_str_6);
        cJSON_ReplaceItemInArray(items, 1, replacement);
    }

    // Add null to an object (add to menu object)
    cJSON_AddNullToObject(menu, fuzz_str_7);

    // Add boolean to an object (add to menu object)
    cJSON_AddBoolToObject(menu, fuzz_str_8, cJSON_True);

    // Clean up - delete all created objects
    if (float_array) cJSON_Delete(float_array);
    if (int_array) cJSON_Delete(int_array);
    if (number_item) cJSON_Delete(number_item);
    if (array_ref) cJSON_Delete(array_ref);
    
    // Delete the root object (this will clean up everything else)
    cJSON_Delete(root);
    
    // Free the input buffer
    cJSON_free(input_buffer);

    assert_file_closed(&in_file);
	return 0;
}