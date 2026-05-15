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
//<ID> 81
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"cJSON_IsArray":[[3044,9,3044,21,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArrayItem","cJSON_IsRaw","cJSON_CreateStringArray","cJSON_GetObjectItem","cJSON_CreateObjectReference","cJSON_CreateArray","cJSON_AddItemReferenceToArray","cJSON_Delete","cJSON_GetObjectItem","cJSON_ReplaceItemInObject","cJSON_AddItemToObject","cJSON_Delete","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArrayItem","cJSON_IsRaw","cJSON_CreateStringArray","cJSON_GetObjectItem","cJSON_CreateObjectReference","cJSON_CreateArray","cJSON_AddItemReferenceToArray","cJSON_Delete","cJSON_GetObjectItem","cJSON_ReplaceItemInObject","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified cJSON APIs to achieve the event of modifying a JSON structure:

*/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<109) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
	
		const char * fuzzer_var_cJSON_CreateStringArray_6_0[3] = {fuzz_str_4,fuzz_str_5,fuzz_str_6};//fuzzer shim end}




    // Create a null-terminated string from the input data
    char *json_string = (char *)malloc(size + 1);
    if (!json_string) {
	return 0;
	}
	
    memcpy(json_string, data, size);
    json_string[size] = '\0';
    
    // Parse the input JSON
    cJSON *root = cJSON_Parse(json_string);
    if (!root) {
        free(json_string);
        return 0;
    }
    
    // 1. Use cJSON_IsRaw to check if certain items are raw
    cJSON *web_app = cJSON_GetObjectItem(root, fuzz_str_1);
    if (web_app) {
        cJSON *servlet = cJSON_GetObjectItem(web_app, fuzz_str_2);
        if (cJSON_IsArray(servlet)) {
            // Check if first item is raw (it shouldn't be, but we call the API)
            cJSON *first_servlet = cJSON_GetArrayItem(servlet, fuzz_int32_t_3);
            if (first_servlet) {
                // This is just to call the API - we know it's not raw
                cJSON_IsRaw(first_servlet);
            }
        }
    }
    
    // 2. Create a string array using cJSON_CreateStringArray
    const char *test_strings[] = {"fuzz_string1", "fuzz_string2", "fuzz_string3"};
    cJSON *string_array = cJSON_CreateStringArray(fuzzer_var_cJSON_CreateStringArray_6_0, 3);
    
    // 3. Create an object reference using cJSON_CreateObjectReference
    cJSON *servlet_mapping = NULL;
    if (web_app) {
        servlet_mapping = cJSON_GetObjectItem(web_app, fuzz_str_7);
    }
    cJSON *servlet_mapping_ref = NULL;
    if (servlet_mapping) {
        servlet_mapping_ref = cJSON_CreateObjectReference(servlet_mapping);
    }
    
    // 4. Add item reference to array using cJSON_AddItemReferenceToArray
    if (string_array && servlet_mapping_ref) {
        // First, create a temporary array to test the API
        cJSON *temp_array = cJSON_CreateArray();
        if (temp_array) {
            cJSON_AddItemReferenceToArray(temp_array, servlet_mapping_ref);
            cJSON_Delete(temp_array);
        }
    }
    
    // 5. Replace an item in object using cJSON_ReplaceItemInObject
    if (web_app && string_array) {
        // Try to replace the "taglib" object with our string array
        cJSON *taglib = cJSON_GetObjectItem(web_app, fuzz_str_8);
        if (taglib) {
            cJSON_ReplaceItemInObject(web_app, "taglib", string_array);
            // string_array is now owned by web_app, don't delete it separately
        } else {
            // If taglib doesn't exist, add the string array as a new item
            cJSON_AddItemToObject(web_app, fuzz_str_9, string_array);
        }
    } else {
        // Clean up if we didn't use the string array
        if (string_array) cJSON_Delete(string_array);
    }
    
    // 6. Print unformatted JSON using cJSON_PrintUnformatted
    char *unformatted_json = cJSON_PrintUnformatted(root);
    if (unformatted_json) {
        // We could write this to a file if needed
        // FILE *out_file = fopen("output_file", "wb");
        // if (out_file) {
        //     fwrite(unformatted_json, 1, strlen(unformatted_json), out_file);
        //     assert_file_closed(&out_file);;
        // }
        cJSON_free(unformatted_json);
    }
    
    // Clean up
    if (servlet_mapping_ref) cJSON_Delete(servlet_mapping_ref);
    cJSON_Delete(root);
    free(json_string);
    
    return 0;
}