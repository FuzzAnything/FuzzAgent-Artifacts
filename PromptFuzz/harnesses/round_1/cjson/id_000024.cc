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
//<ID> 82
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"cJSON_Compare":[[3074,9,3074,20,0,0,4,0]]},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_ParseWithLength","cJSON_Compare","cJSON_CreateFloatArray","cJSON_IsObject","cJSON_AddTrueToObject","cJSON_AddItemToObject","cJSON_Delete","cJSON_Print","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_ParseWithLength","cJSON_Compare","cJSON_CreateFloatArray","cJSON_IsObject","cJSON_AddTrueToObject","cJSON_AddItemToObject","cJSON_Print","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Create a null-terminated string from the input data
    char *input_str = (char *)malloc(size + 1);
    if (!input_str) {
	return 0;
	}
	
    
    memcpy(input_str, data, size);
    input_str[size] = '\0';
    
    // API 1: Parse the input using cJSON_ParseWithLengthOpts
    const char *parse_end = nullptr;
    cJSON *parsed_json1 = cJSON_ParseWithLengthOpts(
        input_str, 
        size, 
        &parse_end, 
        false  // require_null_terminated
    );
    
    // API 2: Also parse using cJSON_ParseWithLength for comparison
    cJSON *parsed_json2 = cJSON_ParseWithLength(input_str, size);
    
    // API 3: Compare the two parsed JSON objects
    cJSON_bool are_equal = cJSON_Compare(parsed_json1, parsed_json2, true);
    
    // Create some test data for float array
    float float_numbers[3] = {1.1f, 2.2f, 3.3f};
    
    // API 4: Create a float array
    cJSON *float_array = cJSON_CreateFloatArray(float_numbers, 3);
    
    if (parsed_json1 && cJSON_IsObject(parsed_json1)) {
        // API 5: Add a true value to the parsed object
        cJSON_AddTrueToObject(parsed_json1, "fuzz_test_true");
        
        // Also add the float array to the object
        cJSON_AddItemToObject(parsed_json1, "fuzz_float_array", float_array);
        float_array = nullptr; // Ownership transferred
    } else if (float_array) {
        cJSON_Delete(float_array);
        float_array = nullptr;
    }
    
    // Create output file to write the results
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        if (parsed_json1) {
            char *printed = cJSON_Print(parsed_json1);
            if (printed) {
                fwrite(printed, 1, strlen(printed), out_file);
                free(printed);
            }
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up all allocated resources
    if (parsed_json1) cJSON_Delete(parsed_json1);
    if (parsed_json2) cJSON_Delete(parsed_json2);
    if (float_array) cJSON_Delete(float_array);
    free(input_str);
    
    assert_file_closed(&out_file);
	return 0;
}