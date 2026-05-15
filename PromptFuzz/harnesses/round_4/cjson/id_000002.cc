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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["cJSON_ParseWithOpts","cJSON_CreateFalse","cJSON_CreateNumber","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_CreateDoubleArray","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithOpts","cJSON_CreateFalse","cJSON_CreateNumber","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_CreateDoubleArray","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/*Here's a fuzz driver that uses all the specified cJSON APIs to process the given 3x3 matrix input:

*/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<159) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeDoublingArray(fuzz_double_array_4, fuzz_double_array_size_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
	
		const double * fuzzer_var_cJSON_CreateDoubleArray_11_0 = fuzz_double_array_4;
		int fuzzer_size_cJSON_CreateDoubleArray_11_0 = static_cast<int>(fuzz_double_array_size_4);//fuzzer shim end}




    // 1. Parse the input JSON array using cJSON_ParseWithOpts
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
	return 0;
	}
	
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    const char *parse_end = NULL;
    cJSON *parsed_array = cJSON_ParseWithOpts(json_str, &parse_end, fuzz_int32_t_1);
    
    // 2. Create various cJSON objects
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *number_item = cJSON_CreateNumber(3.14159);
    
    // 3. Create a double array from the input matrix
    double matrix_values[9] = {0};
    if (parsed_array && cJSON_IsArray(parsed_array)) {
        int row_count = cJSON_GetArraySize(parsed_array);
        for (int i = 0; i < row_count && i < 3; i++) {
            cJSON *row = cJSON_GetArrayItem(parsed_array, fuzz_int32_t_2);
            if (row && cJSON_IsArray(row)) {
                int col_count = cJSON_GetArraySize(row);
                for (int j = 0; j < col_count && j < 3; j++) {
                    cJSON *cell = cJSON_GetArrayItem(row, fuzz_int32_t_3);
                    if (cell && cJSON_IsNumber(cell)) {
                        matrix_values[i * 3 + j] = cJSON_GetNumberValue(cell);
                    }
                }
            }
        }
    }
    
    cJSON *double_array = cJSON_CreateDoubleArray(fuzzer_var_cJSON_CreateDoubleArray_11_0, fuzzer_size_cJSON_CreateDoubleArray_11_0);
    
    // 4. Create an object and add a string to it
    cJSON *main_object = cJSON_CreateObject();
    cJSON_AddStringToObject(main_object, fuzz_str_5, json_str);
    
    // Add our created items to the object
    if (false_item) {
        cJSON_AddItemToObject(main_object, fuzz_str_6, false_item);
    }
    
    if (number_item) {
        cJSON_AddItemToObject(main_object, fuzz_str_7, number_item);
    }
    
    if (double_array) {
        cJSON_AddItemToObject(main_object, fuzz_str_8, double_array);
    }
    
    if (parsed_array) {
        cJSON_AddItemToObject(main_object, fuzz_str_9, parsed_array);
    }
    
    // 5. Write results to output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        char *output = cJSON_Print(main_object);
        if (output) {
            fwrite(output, 1, strlen(output), out_file);
            cJSON_free(output);
        }
        assert_file_closed(&out_file);;
    }
    
    // 6. Clean up (cJSON_Delete recursively frees all items in the object)
    if (main_object) {
        cJSON_Delete(main_object);
    } else {
        // If main_object wasn't created, clean up individual items
        if (false_item) cJSON_Delete(false_item);
        if (number_item) cJSON_Delete(number_item);
        if (double_array) cJSON_Delete(double_array);
        if (parsed_array) cJSON_Delete(parsed_array);
    }
    
    free(json_str);
    assert_file_closed(&out_file);
	return 0;
}