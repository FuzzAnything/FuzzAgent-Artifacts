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
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"cJSON_CreateFalse":[[2486,8,2486,12,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_IsObject","cJSON_Delete","cJSON_CreateObject","cJSON_CreateTrue","cJSON_AddRawToObject","cJSON_IsString","cJSON_AddItemToObject","cJSON_CreateNumber","cJSON_IsNumber","cJSON_AddItemToObject","cJSON_CreateFalse","cJSON_IsFalse","cJSON_AddItemToObject","cJSON_DetachItemFromObject","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_IsObject","cJSON_Delete","cJSON_CreateObject","cJSON_CreateTrue","cJSON_AddRawToObject","cJSON_IsString","cJSON_AddItemToObject","cJSON_CreateNumber","cJSON_IsNumber","cJSON_AddItemToObject","cJSON_CreateFalse","cJSON_IsFalse","cJSON_AddItemToObject","cJSON_DetachItemFromObject","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":1}
/*Looking at this input, I can see it's a 3x3 matrix. However, I need to create a fuzz driver that uses the specified cJSON APIs. Let me write a fuzz driver that:

1. Parses input data as JSON
2. Creates and manipulates cJSON objects
3. Uses all the specified APIs

Here's the fuzz driver:

*/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}




    // Early return if input is too small
    if (size < 2) {
	return 0;
	}
	
    
    // 1. Parse input data as JSON using cJSON_ParseWithLength
    // We'll treat the input as a JSON string
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
	return 0;
	}
	
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *parsed_json = cJSON_ParseWithLength(json_str, size);
    free(json_str);
    
    // If parsing fails, try to create our own object
    cJSON *main_object = parsed_json;
    if (!main_object) {
        main_object = cJSON_CreateObject();
        if (!main_object) return 0;
    }
    
    // 2. Check if it's an object using cJSON_IsObject
    cJSON_bool is_object = cJSON_IsObject(main_object);
    
    // Create a new object if needed
    if (!is_object) {
        cJSON_Delete(main_object);
        main_object = cJSON_CreateObject();
        if (!main_object) return 0;
    }
    
    // 3. Create a true value using cJSON_CreateTrue()
    cJSON *true_value = cJSON_CreateTrue();
    
    // 4. Add raw JSON to object using cJSON_AddRawToObject
    // We'll use part of the input data as raw JSON
    size_t raw_len = size > 100 ? 100 : size;
    char *raw_data = (char *)malloc(raw_len + 1);
    if (raw_data) {
        memcpy(raw_data, data, raw_len);
        raw_data[raw_len] = '\0';
        cJSON *raw_item = cJSON_AddRawToObject(main_object, fuzz_str_1, raw_data);
        free(raw_data);
        
        // 5. Check if the raw item is a string using cJSON_IsString
        if (raw_item) {
            cJSON_bool is_string = cJSON_IsString(raw_item);
            (void)is_string; // Use the result to avoid unused variable warning
        }
    }
    
    // 6. Add the true value to the object
    if (true_value) {
        cJSON_AddItemToObject(main_object, fuzz_str_2, true_value);
    }
    
    // 7. Create a number item and check it using cJSON_IsNumber
    cJSON *number_item = cJSON_CreateNumber(42.5);
    if (number_item) {
        cJSON_bool is_number = cJSON_IsNumber(number_item);
        (void)is_number; // Use the result
        cJSON_AddItemToObject(main_object, fuzz_str_3, number_item);
    }
    
    // 8. Create a false item and check it using cJSON_IsFalse
    cJSON *false_item = cJSON_CreateFalse();
    if (false_item) {
        cJSON_bool is_false = cJSON_IsFalse(false_item);
        (void)is_false; // Use the result
        cJSON_AddItemToObject(main_object, fuzz_str_4, false_item);
    }
    
    // 9. Detach an item from the object using cJSON_DetachItemFromObject
    cJSON *detached = cJSON_DetachItemFromObject(main_object, fuzz_str_5);
    if (detached) {
        cJSON_Delete(detached);
    }
    
    // 10. Print the JSON using cJSON_Print
    char *printed_json = cJSON_Print(main_object);
    if (printed_json) {
        // Write to a file if needed
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed_json, 1, strlen(printed_json), out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(printed_json);
    }
    
    // 11. Clean up
    cJSON_Delete(main_object);
    
    return 0;
}