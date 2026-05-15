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
//<ID> 92
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 3
//<Quality> {"density":21,"unique_branches":{"cJSON_ParseWithLengthOpts":[[1156,26,1156,44,0,0,4,0],[1195,9,1195,21,0,0,4,1],[1210,18,1210,35,0,0,4,1]]},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_ParseWithOpts","cJSON_CreateObject","cJSON_Delete","cJSON_Delete","cJSON_IsArray","cJSON_CreateObjectReference","cJSON_AddItemToObjectCS","cJSON_CreateStringArray","cJSON_AddItemToObjectCS","cJSON_AddStringToObject","cJSON_AddArrayToObject","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_ParseWithOpts","cJSON_CreateObject","cJSON_IsArray","cJSON_CreateObjectReference","cJSON_AddItemToObjectCS","cJSON_CreateStringArray","cJSON_AddItemToObjectCS","cJSON_AddStringToObject","cJSON_AddArrayToObject","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


// Include cJSON headers

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<130) return 0;

	
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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
	
		const char * fuzzer_var_cJSON_CreateStringArray_8_0[3] = {fuzz_str_2,fuzz_str_3,fuzz_str_4};//fuzzer shim end}




    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read the input data into a buffer
    char *buffer = (char *)malloc(size + 1);
    if (!buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    size_t bytes_read = fread(buffer, 1, size, in_file);
    buffer[bytes_read] = '\0';
    assert_file_closed(&in_file);;
    
    // Parse the input JSON data using cJSON_ParseWithLengthOpts
    const char *parse_end1 = nullptr;
    cJSON *parsed1 = cJSON_ParseWithLengthOpts(
        buffer, 
        bytes_read, 
        &parse_end1, 
        false  // don't require null terminated
    );
    
    // Also parse using cJSON_ParseWithOpts
    const char *parse_end2 = nullptr;
    cJSON *parsed2 = cJSON_ParseWithOpts(
        buffer, 
        &parse_end2, 
        false  // don't require null terminated
    );
    
    // Create a new object to work with
    cJSON *root_object = cJSON_CreateObject();
    if (!root_object) {
        free(buffer);
        if (parsed1) cJSON_Delete(parsed1);
        if (parsed2) cJSON_Delete(parsed2);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Use parsed1 if valid
    if (parsed1 && cJSON_IsArray(parsed1)) {
        // Create an object reference to the parsed array
        cJSON *array_ref = cJSON_CreateObjectReference(parsed1);
        if (array_ref) {
            // Add the reference to our root object
            cJSON_AddItemToObjectCS(root_object, fuzz_str_1, array_ref);
        }
    }
    
    // Add a string array to the object
    const char *string_array_data[] = {"element1", "element2", "element3"};
    cJSON *string_array = cJSON_CreateStringArray(fuzzer_var_cJSON_CreateStringArray_8_0, 3);
    if (string_array) {
        cJSON_AddItemToObjectCS(root_object, fuzz_str_5, string_array);
    }
    
    // Add a regular string to the object
    cJSON_AddStringToObject(root_object, fuzz_str_6, fuzz_str_7);
    
    // Add an array to the object
    cJSON *new_array = cJSON_AddArrayToObject(root_object, fuzz_str_8);
    if (new_array) {
        // Add some elements to the array
        cJSON_AddItemToArray(new_array, cJSON_CreateNumber(42.0));
        cJSON_AddItemToArray(new_array, cJSON_CreateString(fuzz_str_9));
        
        // Use cJSON_SetNumberHelper on a number element
        if (cJSON_GetArraySize(new_array) > 0) {
            cJSON *first_element = cJSON_GetArrayItem(new_array, fuzz_int32_t_10);
            if (first_element && cJSON_IsNumber(first_element)) {
                cJSON_SetNumberHelper(first_element, 99.9);
            }
        }
    }
    
    // Cleanup: delete all JSON objects
    cJSON_Delete(root_object);  // This will delete all child objects
    if (parsed1) cJSON_Delete(parsed1);
    if (parsed2) cJSON_Delete(parsed2);
    free(buffer);
    
    assert_file_closed(&in_file);
	return 0;
}