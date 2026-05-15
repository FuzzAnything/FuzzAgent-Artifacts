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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"cJSON_AddObjectToObject":[[2237,9,2237,76,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_Delete","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateStringReference","cJSON_Delete","cJSON_AddObjectToObject","cJSON_AddNumberToObject","cJSON_AddTrueToObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_PrintBuffered","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateStringReference","cJSON_Delete","cJSON_AddObjectToObject","cJSON_AddNumberToObject","cJSON_AddTrueToObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_PrintBuffered","cJSON_Delete"],"visited":1}
/*To create a fuzz driver using the specified cjson APIs, I'll design an event that processes the input JSON data, modifies it, and prints the result. The event will:

1. Parse the input data as JSON
2. Modify the structure by adding new elements and removing existing ones
3. Print the modified JSON using buffered printing

Here's the implementation:

*/


// Include cjson headers
extern "C" {
    #include "cJSON.h"
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<96) return 0;

	
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
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_8, fuzz_int32_t_array_size_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
	
		const int * fuzzer_var_cJSON_CreateIntArray_13_0 = fuzz_int32_t_array_8;
		int fuzzer_size_cJSON_CreateIntArray_13_0 = static_cast<int>(fuzz_int32_t_array_size_8);//fuzzer shim end}




    // Early return for insufficient data
    if (size == 0) {
	return 0;
	}
	
    
    // Create a null-terminated string from input data
    char* json_str = (char*)malloc(size + 1);
    if (!json_str) {
	return 0;
	}
	
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    // Parse the JSON input
    cJSON* root = cJSON_Parse(json_str);
    if (!root) {
        free(json_str);
        return 0;
    }
    
    // Get the Image object
    cJSON* image = cJSON_GetObjectItem(root, fuzz_str_1);
    if (!image) {
        cJSON_Delete(root);
        free(json_str);
        return 0;
    }
    
    // 1. Check if Thumbnail's Url is a string using cJSON_IsString
    cJSON* thumbnail = cJSON_GetObjectItem(image, fuzz_str_2);
    if (thumbnail) {
        cJSON* url = cJSON_GetObjectItem(thumbnail, fuzz_str_3);
        if (url && cJSON_IsString(url)) {
            // Create a string reference for the Url
            cJSON* url_ref = cJSON_CreateStringReference(url->valuestring);
            // We don't actually use this reference further as per requirements
            // but we created it as required
            cJSON_Delete(url_ref);
        }
    }
    
    // 2. Add a new object to the Image using cJSON_AddObjectToObject
    cJSON* new_obj = cJSON_AddObjectToObject(image, fuzz_str_4);
    
    // 3. Add a number to the new object using cJSON_AddNumberToObject
    if (new_obj) {
        cJSON_AddNumberToObject(new_obj, fuzz_str_5, 2.1);
    }
    
    // 4. Add a true value to the new object using cJSON_AddTrueToObject
    if (new_obj) {
        cJSON_AddTrueToObject(new_obj, fuzz_str_6);
    }
    
    // 5. Get the IDs array and create a new int array
    cJSON* ids_array = cJSON_GetObjectItem(image, fuzz_str_7);
    if (cJSON_IsArray(ids_array)) {
        // Create a new integer array
        int new_numbers[] = {999, 1000, 1001};
        cJSON* new_int_array = cJSON_CreateIntArray(fuzzer_var_cJSON_CreateIntArray_13_0, fuzzer_size_cJSON_CreateIntArray_13_0);
        
        // Add the new array to the Image object
        if (new_int_array) {
            cJSON_AddItemToObject(image, fuzz_str_9, new_int_array);
        }
        
        // 6. Delete the first item from the IDs array using cJSON_DeleteItemFromArray
        if (cJSON_GetArraySize(ids_array) > 0) {
            cJSON_DeleteItemFromArray(ids_array, fuzz_int32_t_10);
        }
    }
    
    // 7. Print the modified JSON using cJSON_PrintBuffered
    char* printed_json = cJSON_PrintBuffered(root, fuzz_int32_t_11, true);
    
    // Clean up
    if (printed_json) {
        free(printed_json);
    }
    
    cJSON_Delete(root);
    free(json_str);
    
    return 0;
}