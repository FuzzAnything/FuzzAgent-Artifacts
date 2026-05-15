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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 10
//<Quality> {"density":17,"unique_branches":{"cJSON_CreateDoubleArray":[[2708,9,2708,20,0,0,4,1],[2708,24,2708,41,0,0,4,1],[2715,16,2715,17,0,0,4,0],[2715,21,2715,40,0,0,4,0],[2715,21,2715,40,0,0,4,1],[2718,12,2718,14,0,0,4,1],[2723,12,2723,14,0,0,4,0],[2723,12,2723,14,0,0,4,1],[2734,9,2734,10,0,0,4,0],[2734,14,2734,22,0,0,4,0]]},"library_calls":["cJSON_ParseWithOpts","cJSON_CreateFalse","cJSON_CreateNumber","cJSON_CreateDoubleArray","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithOpts","cJSON_CreateFalse","cJSON_CreateNumber","cJSON_CreateDoubleArray","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<160) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeDoublingArray(fuzz_double_array_2, fuzz_double_array_size_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
	
		const double * fuzzer_var_cJSON_CreateDoubleArray_3_0 = fuzz_double_array_2;
		int fuzzer_size_cJSON_CreateDoubleArray_3_0 = static_cast<int>(fuzz_double_array_size_2);//fuzzer shim end}




    // Early exit if no data
    if (size == 0) {
        return 0;
    }

    // Create null-terminated string from input data for parsing
    char *input_str = static_cast<char*>(malloc(size + 1));
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, data, size);
    input_str[size] = '\0';

    // Parse input using cJSON_ParseWithOpts
    const char *parse_end = nullptr;
    cJSON *parsed_json = cJSON_ParseWithOpts(input_str, &parse_end, fuzz_int32_t_1);
    free(input_str);

    // Create various cJSON objects
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *number_item = cJSON_CreateNumber(3.14159);

    // Create a double array - using the matrix values from the example
    double matrix[] = {0.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
    cJSON *double_array = cJSON_CreateDoubleArray(fuzzer_var_cJSON_CreateDoubleArray_3_0, fuzzer_size_cJSON_CreateDoubleArray_3_0);

    // Create a main object to hold everything
    cJSON *main_object = cJSON_CreateObject();
    if (main_object) {
        // Add string to object
        cJSON_AddStringToObject(main_object, fuzz_str_3, fuzz_str_4);
        
        // Add other items to object
        if (false_item) {
            cJSON_AddItemToObject(main_object, fuzz_str_5, false_item);
        }
        if (double_array) {
            cJSON_AddItemToObject(main_object, fuzz_str_6, double_array);
        }
        if (number_item) {
            cJSON_AddItemToObject(main_object, fuzz_str_7, number_item);
        }

        // If we successfully parsed input, add it to the object
        if (parsed_json) {
            cJSON_AddItemToObject(main_object, fuzz_str_8, parsed_json);
        }

        // Optional: Print the object to a buffer (for fuzzing coverage)
        char *printed = cJSON_Print(main_object);
        if (printed) {
            // Could write to output file if needed
            // FILE *out_file = fopen("output_file", "wb");
            // if (out_file) {
            //     fwrite(printed, 1, strlen(printed), out_file);
            //     assert_file_closed(&out_file);;
            // }
            cJSON_free(printed);
        }

        // Clean up main object (this will delete all child items)
        cJSON_Delete(main_object);
    } else {
        // Clean up individually if main_object creation failed
        if (false_item) cJSON_Delete(false_item);
        if (double_array) cJSON_Delete(double_array);
        if (number_item) cJSON_Delete(number_item);
        if (parsed_json) cJSON_Delete(parsed_json);
    }

    return 0;
}