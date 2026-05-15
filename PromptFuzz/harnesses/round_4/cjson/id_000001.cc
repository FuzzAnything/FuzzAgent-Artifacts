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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"cJSON_CreateDoubleArray":[[2734,14,2734,22,0,0,4,1]]},"library_calls":["cJSON_ParseWithOpts","cJSON_CreateObject","cJSON_CreateFalse","cJSON_AddItemToObject","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNumber","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_AddStringToObject","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithOpts","cJSON_CreateObject","cJSON_CreateFalse","cJSON_AddItemToObject","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNumber","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_AddStringToObject","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified cJSON APIs to achieve the described event:

*/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<66) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
	//fuzzer shim end}




    // Create a null-terminated string from the input data
    char *input_str = (char *)malloc(size + 1);
    if (!input_str) {
	return 0;
	}
	
    memcpy(input_str, data, size);
    input_str[size] = '\0';
    
    // Parse the input JSON data using cJSON_ParseWithOpts
    const char *parse_end = nullptr;
    cJSON *parsed_json = cJSON_ParseWithOpts(input_str, &parse_end, fuzz_int32_t_1);
    
    // Create a new JSON object to work with
    cJSON *root = cJSON_CreateObject();
    
    if (root) {
        // Add a false value to the object
        cJSON *false_item = cJSON_CreateFalse();
        if (false_item) {
            cJSON_AddItemToObject(root, fuzz_str_2, false_item);
        }
        
        // Create a double array from the parsed data (if available)
        if (parsed_json && cJSON_IsArray(parsed_json)) {
            int array_size = cJSON_GetArraySize(parsed_json);
            double *double_array = (double *)malloc(array_size * sizeof(double));
            
            if (double_array) {
                for (int i = 0; i < array_size && i < 10; i++) {
                    cJSON *item = cJSON_GetArrayItem(parsed_json, fuzz_int32_t_3);
                    if (item && cJSON_IsNumber(item)) {
                        double_array[i] = item->valuedouble;
                    } else {
                        double_array[i] = 0.0;
                    }
                }
                
                // Create double array JSON object
                cJSON *double_array_json = cJSON_CreateDoubleArray(double_array, 
                    array_size < 10 ? array_size : 10);
                if (double_array_json) {
                    cJSON_AddItemToObject(root, fuzz_str_4, double_array_json);
                }
                
                free(double_array);
            }
        }
        
        // Create a number from the first element of the input
        double number_value = 0.0;
        if (size > 0) {
            number_value = static_cast<double>(data[0]);
        }
        cJSON *number = cJSON_CreateNumber(number_value);
        if (number) {
            cJSON_AddItemToObject(root, fuzz_str_5, number);
        }
        
        // Add a string to the object
        char *string_value = (char *)malloc(size + 20);
        if (string_value) {
            strcpy(string_value, "Input size: ");
            char size_str[20];
            snprintf(size_str, sizeof(size_str), "%zu", size);
            strcat(string_value, size_str);
            
            cJSON_AddStringToObject(root, fuzz_str_6, string_value);
            free(string_value);
        }
        
        // Print the resulting JSON
        char *output = cJSON_Print(root);
        if (output) {
            // Write to output file
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                fwrite(output, strlen(output), 1, out_file);
                assert_file_closed(&out_file);;
            }
            cJSON_free(output);
        }
        
        // Clean up
        cJSON_Delete(root);
    }
    
    if (parsed_json) {
        cJSON_Delete(parsed_json);
    }
    
    free(input_str);
    
    return 0;
}