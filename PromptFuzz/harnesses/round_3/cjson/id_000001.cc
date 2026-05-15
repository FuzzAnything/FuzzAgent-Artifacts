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
//<score> 60, nr_unique_branch: 7
//<Quality> {"density":10,"unique_branches":{"cJSON_IsArray":[[3044,9,3044,21,0,0,4,1]],"get_object_item":[[1970,16,1970,41,0,0,4,1]],"cJSON_GetArrayItem":[[1943,9,1943,18,0,0,4,1]],"cJSON_CreateObject":[[2612,9,2612,13,0,0,4,0]],"print_value":[[1464,9,1464,23,0,0,4,0],[1467,17,1467,42,0,0,4,1],[1474,17,1474,31,0,0,4,1]]},"library_calls":["cJSON_ParseWithLength","cJSON_IsObject","cJSON_CreateObject","cJSON_Delete","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_AddRawToObject","cJSON_IsString","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_IsString","cJSON_IsFalse","cJSON_Print","cJSON_DetachItemFromObject","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_IsObject","cJSON_CreateObject","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_AddRawToObject","cJSON_IsString","cJSON_GetArraySize","cJSON_IsArray","cJSON_GetArrayItem","cJSON_IsNumber","cJSON_IsString","cJSON_IsFalse","cJSON_Print","cJSON_DetachItemFromObject","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


// Include cJSON headers (assuming they're available in the environment)

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<51) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
	//fuzzer shim end}




    // Ensure we have at least some data
    if (size == 0) {
	return 0;
	}
	
    
    // Create a null-terminated copy of the input data for parsing
    char *input_str = (char *)malloc(size + 1);
    if (!input_str) {
	return 0;
	}
	
    
    memcpy(input_str, data, size);
    input_str[size] = '\0';
    
    // 1. Parse the input using cJSON_ParseWithLength
    cJSON *parsed_json = cJSON_ParseWithLength(input_str, size);
    
    // Clean up the temporary string
    free(input_str);
    
    // If parsing failed, return early
    if (!parsed_json) {
	return 0;
	}
	
    
    // 2. Check if the parsed JSON is an object
    cJSON_bool is_object = cJSON_IsObject(parsed_json);
    
    // Create a new object to work with
    cJSON *new_object = cJSON_CreateObject();
    if (!new_object) {
        cJSON_Delete(parsed_json);
        return 0;
    }
    
    // 3. Add some items to the new object
    // Add a true value
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(new_object, fuzz_str_1, true_item);
    
    // Add a raw JSON string
    const char *raw_json = "{\"key\": 123}";
    cJSON *raw_item = cJSON_AddRawToObject(new_object, fuzz_str_2, fuzz_str_3);
    
    // 4. Check types of various items
    // Check if the raw item is a string (it should be a raw type, not string)
    cJSON_bool raw_is_string = cJSON_IsString(raw_item);
    
    // Get an item from the parsed JSON to check its type
    cJSON *first_item = NULL;
    if (cJSON_IsArray(parsed_json) && cJSON_GetArraySize(parsed_json) > 0) {
        first_item = cJSON_GetArrayItem(parsed_json, fuzz_int32_t_4);
    } else if (is_object) {
        first_item = cJSON_GetObjectItem(parsed_json, fuzz_str_5);
    }
    
    // Check if the first item is a number
    cJSON_bool is_number = first_item ? cJSON_IsNumber(first_item) : 0;
    
    // Check if the first item is a string
    cJSON_bool is_string = first_item ? cJSON_IsString(first_item) : 0;
    
    // Check if the true item is false (should be false)
    cJSON_bool is_false = cJSON_IsFalse(true_item);
    
    // 5. Print the new object
    char *printed_json = cJSON_Print(new_object);
    
    // 6. If we have a parsed object, try to detach an item from it
    if (is_object) {
        // Try to detach an item with a key that might exist
        cJSON *detached = cJSON_DetachItemFromObject(parsed_json, fuzz_str_6);
        if (detached) {
            cJSON_Delete(detached);
        }
    }
    
    // 7. Clean up resources
    if (printed_json) {
        free(printed_json);
    }
    
    cJSON_Delete(parsed_json);
    cJSON_Delete(new_object);
    
    return 0;
}