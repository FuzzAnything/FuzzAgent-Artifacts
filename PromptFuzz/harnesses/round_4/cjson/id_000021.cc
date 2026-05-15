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
//<ID> 91
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 2
//<Quality> {"density":16,"unique_branches":{"cJSON_SetNumberHelper":[[418,9,418,26,0,0,4,0],[422,14,422,39,0,0,4,0]]},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_ParseWithOpts","cJSON_CreateObject","cJSON_CreateStringArray","cJSON_AddArrayToObject","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_AddItemToObjectCS","cJSON_AddItemToArray","cJSON_CreateObjectReference","cJSON_AddItemToObjectCS","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsArray","cJSON_CreateArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNumber","cJSON_CreateNumber","cJSON_GetNumberValue","cJSON_SetNumberHelper","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_Print","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_ParseWithOpts","cJSON_CreateObject","cJSON_CreateStringArray","cJSON_AddArrayToObject","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_AddItemToObjectCS","cJSON_AddItemToArray","cJSON_CreateObjectReference","cJSON_AddItemToObjectCS","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsArray","cJSON_CreateArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNumber","cJSON_CreateNumber","cJSON_GetNumberValue","cJSON_SetNumberHelper","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_Print","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/*Looking at the input data (a 3x3 matrix) and the required APIs, I'll create a fuzzer that parses JSON data, creates objects with arrays and string arrays, and manipulates JSON structure using the specified APIs.

*/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<119) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
	
		const char * fuzzer_var_cJSON_CreateStringArray_3_0[6] = {fuzz_str_1,fuzz_str_2,fuzz_str_3,fuzz_str_4,fuzz_str_5,fuzz_str_6};//fuzzer shim end}




    // Create a null-terminated string from input data for parsing
    char *json_string = nullptr;
    if (size > 0) {
        json_string = (char *)malloc(size + 1);
        if (!json_string) {
	return 0;
	}
	
        memcpy(json_string, data, size);
        json_string[size] = '\0';
    } else {
        // If no data, create a simple JSON string for testing
        json_string = strdup("[[0,-1,0],[1,0,0],[0,0,1]]");
        if (!json_string) {
	return 0;
	}
	
        size = strlen(json_string);
    }
    
    // Create FILE* for reading input data if needed
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        free(json_string);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Parse JSON using cJSON_ParseWithLengthOpts
    const char *parse_end = nullptr;
    cJSON *parsed_json = cJSON_ParseWithLengthOpts(json_string, size, &parse_end, false);
    
    // 2. Also parse using cJSON_ParseWithOpts
    const char *parse_end2 = nullptr;
    cJSON *parsed_json2 = cJSON_ParseWithOpts(json_string, &parse_end2, false);
    
    // Create a new root object
    cJSON *root = cJSON_CreateObject();
    if (root) {
        // 3. Add a string array to object
        const char *string_array_data[] = {"row0", "row1", "row2", "col0", "col1", "col2"};
        cJSON *string_array = cJSON_CreateStringArray(fuzzer_var_cJSON_CreateStringArray_3_0, 6);
        
        // 4. Add array to object using cJSON_AddArrayToObject
        cJSON *matrix_array = cJSON_AddArrayToObject(root, fuzz_str_7);
        
        if (matrix_array && string_array) {
            // 5. Add string array to the matrix array using cJSON_AddItemToObjectCS style
            // First create a wrapper object for the string array
            cJSON *string_array_wrapper = cJSON_CreateObject();
            if (string_array_wrapper) {
                // 6. Add string to wrapper object
                cJSON_AddStringToObject(string_array_wrapper, fuzz_str_8, fuzz_str_9);
                
                // 7. Add the string array to wrapper using cJSON_AddItemToObjectCS
                cJSON_AddItemToObjectCS(string_array_wrapper, fuzz_str_10, string_array);
                
                // Add wrapper to matrix array
                cJSON_AddItemToArray(matrix_array, string_array_wrapper);
                
                // 8. Create object reference
                cJSON *array_ref = cJSON_CreateObjectReference(string_array_wrapper);
                if (array_ref) {
                    cJSON_AddItemToObjectCS(root, fuzz_str_11, array_ref);
                }
            }
            
            // Add parsed JSON data to matrix array if available
            if (parsed_json && cJSON_IsArray(parsed_json)) {
                for (int i = 0; i < cJSON_GetArraySize(parsed_json); i++) {
                    cJSON *row = cJSON_GetArrayItem(parsed_json, fuzz_int32_t_12);
                    if (row && cJSON_IsArray(row)) {
                        cJSON *new_row = cJSON_CreateArray();
                        for (int j = 0; j < cJSON_GetArraySize(row); j++) {
                            cJSON *element = cJSON_GetArrayItem(row, fuzz_int32_t_13);
                            if (cJSON_IsNumber(element)) {
                                // 9. Create number and use cJSON_SetNumberHelper
                                cJSON *num_obj = cJSON_CreateNumber(0);
                                if (num_obj) {
                                    double original_value = cJSON_GetNumberValue(element);
                                    // Transform the value for demonstration
                                    double new_value = cJSON_SetNumberHelper(num_obj, original_value * 2.0);
                                    (void)new_value; // Use value to avoid unused variable warning
                                    cJSON_AddItemToArray(new_row, num_obj);
                                }
                            }
                        }
                        cJSON_AddItemToArray(matrix_array, new_row);
                    }
                }
            }
        }
        
        // Create output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            // Print the JSON
            char *printed = cJSON_Print(root);
            if (printed) {
                fwrite(printed, 1, strlen(printed), out_file);
                free(printed);
            }
            assert_file_closed(&out_file);;
        }
        
        // Clean up
        cJSON_Delete(root);
    }
    
    // Clean up second parsed JSON
    if (parsed_json2) {
        cJSON_Delete(parsed_json2);
    }
    
    // Clean up first parsed JSON
    if (parsed_json) {
        cJSON_Delete(parsed_json);
    }
    
    // Clean up resources
    assert_file_closed(&in_file);;
    free(json_string);
    
    assert_file_closed(&in_file);
	return 0;
}