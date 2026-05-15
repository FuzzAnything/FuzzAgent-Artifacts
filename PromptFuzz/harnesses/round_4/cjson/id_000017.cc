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
//<ID> 71
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_Duplicate","cJSON_AddNullToObject","cJSON_AddRawToObject","cJSON_AddBoolToObject","cJSON_AddArrayToObject","cJSON_malloc","cJSON_CreateFloatArray","cJSON_ReplaceItemViaPointer","cJSON_free","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_Duplicate","cJSON_AddNullToObject","cJSON_AddRawToObject","cJSON_AddBoolToObject","cJSON_AddArrayToObject","cJSON_malloc","cJSON_CreateFloatArray","cJSON_ReplaceItemViaPointer","cJSON_free","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<52) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}




    // Use the input data to parse JSON
    cJSON *parsed_json = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    
    // If parsing fails, create a new object to work with
    cJSON *root_object = (parsed_json != nullptr) ? parsed_json : cJSON_CreateObject();
    
    if (root_object == nullptr) {
        // If we can't create an object, return early
        return 0;
    }
    
    // Create a duplicate of the root object for manipulation
    cJSON *duplicated = cJSON_Duplicate(root_object, fuzz_int32_t_1);
    
    if (duplicated != nullptr) {
        // Add various types of items to the duplicated object
        cJSON_AddNullToObject(duplicated, fuzz_str_2);
        
        // Create a raw JSON string from the input data (limited to 100 chars)
        char raw_buffer[101];
        size_t raw_len = size < 100 ? size : 100;
        if (raw_len > 0) {
            memcpy(raw_buffer, data, raw_len);
            raw_buffer[raw_len] = '\0';
            cJSON_AddRawToObject(duplicated, fuzz_str_3, raw_buffer);
        }
        
        // Add a boolean value (use first byte to determine true/false)
        cJSON_bool bool_val = (size > 0 && (data[0] % 2 == 0)) ? 1 : 0;
        cJSON_AddBoolToObject(duplicated, fuzz_str_4, bool_val);
        
        // Add an array to the object
        cJSON *array_item = cJSON_AddArrayToObject(duplicated, fuzz_str_5);
        
        // Create a float array using data from input
        // We'll use up to 10 floats from the input data
        int float_count = (size / sizeof(float)) < 10 ? (size / sizeof(float)) : 10;
        if (float_count > 0) {
            float *float_numbers = static_cast<float*>(cJSON_malloc(float_count * sizeof(float)));
            if (float_numbers != nullptr) {
                // Copy floats from input data
                memcpy(float_numbers, data, float_count * sizeof(float));
                cJSON *float_array = cJSON_CreateFloatArray(float_numbers, float_count);
                
                // If we have an array item, replace it with the float array
                if (array_item != nullptr && float_array != nullptr) {
                    cJSON_ReplaceItemViaPointer(duplicated, array_item, float_array);
                }
                
                cJSON_free(float_numbers);
            }
        }
        
        // Create output file and write the JSON
        FILE *out_file = fopen("output_file", "wb");
        if (out_file != nullptr) {
            char *json_string = cJSON_Print(duplicated);
            if (json_string != nullptr) {
                fwrite(json_string, 1, strlen(json_string), out_file);
                cJSON_free(json_string);
            }
            assert_file_closed(&out_file);;
        }
        
        // Clean up duplicated object
        cJSON_Delete(duplicated);
    }
    
    // Clean up the original parsed object
    if (parsed_json != nullptr) {
        cJSON_Delete(parsed_json);
    } else if (root_object != nullptr) {
        // If we created the object ourselves, delete it
        cJSON_Delete(root_object);
    }
    
    return 0;
}