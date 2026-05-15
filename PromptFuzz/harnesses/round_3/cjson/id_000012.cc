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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 132, nr_unique_branch: 12
//<Quality> {"density":22,"unique_branches":{"cJSON_AddArrayToObject":[[2249,9,2249,70,0,0,4,0]],"skip_oneline_comment":[[2879,12,2879,31,0,0,4,0],[2881,13,2881,32,0,0,4,0],[2881,13,2881,32,0,0,4,1]],"cJSON_Minify":[[2945,21,2945,35,0,0,4,0]],"cJSON_ParseWithLengthOpts":[[1179,9,1179,32,0,0,4,0],[1182,13,1182,45,0,0,4,1],[1182,49,1182,85,0,0,4,0],[1182,49,1182,85,0,0,4,1],[1187,9,1187,25,0,0,4,0],[1215,13,1215,37,0,0,4,0]],"cJSON_IsBool":[[3005,9,3005,21,0,0,4,1]]},"library_calls":["cJSON_malloc","cJSON_ParseWithOpts","cJSON_free","cJSON_CreateObject","cJSON_AddArrayToObject","cJSON_CreateBool","cJSON_AddItemToArray","cJSON_IsBool","cJSON_AddItemReferenceToArray","cJSON_CreateString","cJSON_ReplaceItemInArray","cJSON_CreateString","cJSON_SetValuestring","cJSON_AddItemToArray","cJSON_Print","cJSON_Minify","cJSON_free","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsObject","cJSON_Delete","cJSON_AddArrayToObject","cJSON_CreateBool","cJSON_CreateString","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_IsBool","cJSON_AddItemReferenceToArray","cJSON_CreateNumber","cJSON_ReplaceItemInArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_SetValuestring","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsBool","cJSON_CreateBool","cJSON_ReplaceItemInArray","cJSON_Print","cJSON_malloc","cJSON_Minify","cJSON_free","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_malloc","cJSON_ParseWithOpts","cJSON_free","cJSON_GetObjectItem","cJSON_IsObject","cJSON_AddArrayToObject","cJSON_CreateBool","cJSON_CreateString","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_IsBool","cJSON_AddItemReferenceToArray","cJSON_CreateNumber","cJSON_ReplaceItemInArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_SetValuestring","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsBool","cJSON_CreateBool","cJSON_ReplaceItemInArray","cJSON_Print","cJSON_malloc","cJSON_Minify","cJSON_free","cJSON_free","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<125) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
	//fuzzer shim end}




    // Create null-terminated string from input data for parsing
    char *json_str = static_cast<char*>(cJSON_malloc(size + 1));
    if (!json_str) {
	return 0;
	}
	
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    // 1. Parse JSON with cJSON_ParseWithOpts
    const char *parse_end = nullptr;
    cJSON *parsed = cJSON_ParseWithOpts(json_str, &parse_end, fuzz_int32_t_1);
    cJSON_free(json_str);
    
    if (!parsed) {
        // If parsing fails, still try to use the input as string for other operations
        cJSON *temp_obj = cJSON_CreateObject();
        if (temp_obj) {
            // Create a minimal JSON structure to test the required APIs
            cJSON *array = cJSON_AddArrayToObject(temp_obj, fuzz_str_2);
            if (array) {
                cJSON *bool_item = cJSON_CreateBool(fuzz_int32_t_3);
                cJSON_AddItemToArray(array, bool_item);
                
                // Use cJSON_IsBool
                if (cJSON_IsBool(bool_item)) {
                    // Test cJSON_AddItemReferenceToArray
                    cJSON_AddItemReferenceToArray(array, bool_item);
                    
                    // Test cJSON_ReplaceItemInArray
                    cJSON *new_item = cJSON_CreateString(fuzz_str_4);
                    if (new_item) {
                        cJSON_ReplaceItemInArray(array, fuzz_int32_t_5, new_item);
                    }
                }
                
                // Test cJSON_SetValuestring
                cJSON *str_obj = cJSON_CreateString(fuzz_str_6);
                if (str_obj) {
                    cJSON_SetValuestring(str_obj, fuzz_str_7);
                    cJSON_AddItemToArray(array, str_obj);
                }
                
                // Test cJSON_Minify
                char *printed = cJSON_Print(temp_obj);
                if (printed) {
                    cJSON_Minify(printed);
                    cJSON_free(printed);
                }
            }
            cJSON_Delete(temp_obj);
        }
        return 0;
    }
    
    // Get the Image object
    cJSON *image = cJSON_GetObjectItem(parsed, fuzz_str_8);
    if (!image || !cJSON_IsObject(image)) {
        cJSON_Delete(parsed);
        return 0;
    }
    
    // 2. Use cJSON_AddArrayToObject - add a new array to Image object
    cJSON *new_array = cJSON_AddArrayToObject(image, fuzz_str_9);
    
    if (new_array) {
        // Add some items to the new array
        cJSON *bool_item = cJSON_CreateBool(fuzz_int32_t_10);
        cJSON *string_item = cJSON_CreateString(fuzz_str_11);
        
        if (bool_item && string_item) {
            cJSON_AddItemToArray(new_array, bool_item);
            cJSON_AddItemToArray(new_array, string_item);
            
            // 3. Use cJSON_IsBool
            if (cJSON_IsBool(bool_item)) {
                // 4. Use cJSON_AddItemReferenceToArray
                cJSON_AddItemReferenceToArray(new_array, bool_item);
            }
            
            // 5. Use cJSON_ReplaceItemInArray
            cJSON *replacement = cJSON_CreateNumber(42.5);
            if (replacement) {
                cJSON_ReplaceItemInArray(new_array, 1, replacement);
            }
            
            // 6. Use cJSON_SetValuestring
            cJSON *new_string = cJSON_CreateString(fuzz_str_12);
            if (new_string) {
                cJSON_AddItemToArray(new_array, new_string);
                cJSON_SetValuestring(new_string, fuzz_str_13);
            }
        }
        
        // Also work with the existing IDs array
        cJSON *ids = cJSON_GetObjectItem(image, fuzz_str_14);
        if (ids && cJSON_IsArray(ids)) {
            int array_size = cJSON_GetArraySize(ids);
            if (array_size > 0) {
                // Use cJSON_IsBool on a non-bool item (should return false)
                cJSON *first_item = cJSON_GetArrayItem(ids, fuzz_int32_t_15);
                cJSON_IsBool(first_item);
                
                // Try to replace an item if array has at least 2 elements
                if (array_size >= 2) {
                    cJSON *new_bool = cJSON_CreateBool(fuzz_int32_t_16);
                    if (new_bool) {
                        cJSON_ReplaceItemInArray(ids, 1, new_bool);
                    }
                }
            }
        }
    }
    
    // 7. Use cJSON_Minify
    char *printed_json = cJSON_Print(parsed);
    if (printed_json) {
        // Create a copy to minify
        char *minify_target = static_cast<char*>(cJSON_malloc(strlen(printed_json) + 1));
        if (minify_target) {
            strcpy(minify_target, printed_json);
            cJSON_Minify(minify_target);
            cJSON_free(minify_target);
        }
        cJSON_free(printed_json);
    }
    
    // Clean up
    cJSON_Delete(parsed);
    
    return 0;
}