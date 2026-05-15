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
//<ID> 224
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 1
//<Quality> {"density":27,"unique_branches":{"cJSON_SetValuestring":[[441,9,441,25,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_Delete","cJSON_AddArrayToObject","cJSON_CreateArray","cJSON_CreateBool","cJSON_AddItemToArray","cJSON_CreateDoubleArray","cJSON_AddItemToArray","cJSON_CreateFloatArray","cJSON_AddItemToArray","cJSON_AddItemToObject","cJSON_malloc","cJSON_AddRawToObject","cJSON_AddStringToObject","cJSON_free","cJSON_CreateArray","cJSON_CreateNumber","cJSON_CreateNumber","cJSON_CreateNumber","cJSON_SetValuestring","cJSON_GetArrayItem","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_Print","cJSON_free","cJSON_PrintBuffered","cJSON_free","cJSON_DetachItemFromObject","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_AddArrayToObject","cJSON_CreateArray","cJSON_CreateBool","cJSON_AddItemToArray","cJSON_CreateDoubleArray","cJSON_AddItemToArray","cJSON_CreateFloatArray","cJSON_AddItemToArray","cJSON_AddItemToObject","cJSON_malloc","cJSON_AddRawToObject","cJSON_AddStringToObject","cJSON_free","cJSON_CreateArray","cJSON_CreateNumber","cJSON_CreateNumber","cJSON_CreateNumber","cJSON_SetValuestring","cJSON_GetArrayItem","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_Print","cJSON_free","cJSON_PrintBuffered","cJSON_free","cJSON_DetachItemFromObject","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<127) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeDoublingArray(fuzz_double_array_3, fuzz_double_array_size_3, fdp);
		FDPConsumeFloatingArray(fuzz_float_array_4, fuzz_float_array_size_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
	
		const double * fuzzer_var_cJSON_CreateDoubleArray_7_0 = fuzz_double_array_3;
		int fuzzer_size_cJSON_CreateDoubleArray_7_0 = static_cast<int>(fuzz_double_array_size_3);
		const float * fuzzer_var_cJSON_CreateFloatArray_9_0 = fuzz_float_array_4;
		int fuzzer_size_cJSON_CreateFloatArray_9_0 = static_cast<int>(fuzz_float_array_size_4);//fuzzer shim end}




    // 1. Parse input data as JSON
    cJSON *parsed_json = nullptr;
    if (size > 0) {
        parsed_json = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    }
    
    // 2. Create a new cJSON object
    cJSON *root_object = cJSON_CreateObject();
    if (!root_object) {
        if (parsed_json) cJSON_Delete(parsed_json);
        return 0;
    }
    
    // 3. Add an array to the object
    cJSON *array = cJSON_AddArrayToObject(root_object, fuzz_str_1);
    
    // 4. Create another array and add items to it
    cJSON *temp_array = cJSON_CreateArray();
    if (temp_array) {
        // Add boolean item
        cJSON *bool_item = cJSON_CreateBool(fuzz_int32_t_2);
        cJSON_AddItemToArray(temp_array, bool_item);
        
        // Add double array
        double double_nums[] = {1.1, 2.2, 3.3};
        cJSON *double_array = cJSON_CreateDoubleArray(fuzzer_var_cJSON_CreateDoubleArray_7_0, fuzzer_size_cJSON_CreateDoubleArray_7_0);
        cJSON_AddItemToArray(temp_array, double_array);
        
        // Add float array
        float float_nums[] = {4.4f, 5.5f, 6.6f};
        cJSON *float_array = cJSON_CreateFloatArray(fuzzer_var_cJSON_CreateFloatArray_9_0, fuzzer_size_cJSON_CreateFloatArray_9_0);
        cJSON_AddItemToArray(temp_array, float_array);
        
        // Add temp_array to root object
        cJSON_AddItemToObject(root_object, fuzz_str_5, temp_array);
    }
    
    // 5. Add raw and string objects
    if (size > 0) {
        // Create a null-terminated string from input data
        char *input_str = static_cast<char*>(cJSON_malloc(size + 1));
        if (input_str) {
            memcpy(input_str, data, size);
            input_str[size] = '\0';
            
            cJSON_AddRawToObject(root_object, fuzz_str_6, input_str);
            cJSON_AddStringToObject(root_object, fuzz_str_7, input_str);
            
            cJSON_free(input_str);
        }
    }
    
    // 6. Add items to the matrix array (using the 3x3 matrix format)
    if (array) {
        for (int i = 0; i < 3; i++) {
            cJSON *row_array = cJSON_CreateArray();
            if (row_array) {
                cJSON *num1 = cJSON_CreateNumber(0);
                cJSON *num2 = cJSON_CreateNumber(-1);
                cJSON *num3 = cJSON_CreateNumber(0);
                
                if (i == 1) {
                    cJSON_SetValuestring(cJSON_GetArrayItem(row_array, fuzz_int32_t_9), fuzz_str_8);
                }
                
                cJSON_AddItemToArray(row_array, num1);
                cJSON_AddItemToArray(row_array, num2);
                cJSON_AddItemToArray(row_array, num3);
                cJSON_AddItemToArray(array, row_array);
            }
        }
    }
    
    // 7. Print the JSON object in different formats
    char *printed_json = cJSON_Print(root_object);
    if (printed_json) {
        cJSON_free(printed_json);
    }
    
    char *buffered_json = cJSON_PrintBuffered(root_object, fuzz_int32_t_10, fuzz_int32_t_11);
    if (buffered_json) {
        cJSON_free(buffered_json);
    }
    
    // 8. Detach an item and delete it
    cJSON *detached = cJSON_DetachItemFromObject(root_object, fuzz_str_12);
    if (detached) {
        cJSON_Delete(detached);
    }
    
    // 9. Use FILE operations if needed
    if (size > 0) {
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            int fd = fuzz_fileno(in_file);
            assert_file_closed(&in_file);;
        }
    }
    
    // 10. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int fd = fuzz_fileno(out_file);
        assert_file_closed(&out_file);;
    }
    
    // 11. Clean up
    if (parsed_json) {
        cJSON_Delete(parsed_json);
    }
    if (root_object) {
        cJSON_Delete(root_object);
    }
    
    assert_file_closed(&out_file);
	return 0;
}