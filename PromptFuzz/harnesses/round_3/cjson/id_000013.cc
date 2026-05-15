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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 46, nr_unique_branch: 3
//<Quality> {"density":23,"unique_branches":{"cJSON_Minify":[[2939,13,2939,22,0,0,4,0]],"skip_multiline_comment":[[2894,37,2894,57,0,0,4,0]],"skip_oneline_comment":[[2879,12,2879,31,0,0,4,1]]},"library_calls":["cJSON_ParseWithOpts","cJSON_Minify","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArrayItem","cJSON_IsBool","cJSON_CreateTrue","cJSON_ReplaceItemInArray","cJSON_GetArrayItem","cJSON_AddItemReferenceToArray","cJSON_AddArrayToObject","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_SetValuestring","cJSON_Print","cJSON_Minify","cJSON_PrintUnformatted","cJSON_Delete"],"critical_calls":["cJSON_ParseWithOpts","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArrayItem","cJSON_IsBool","cJSON_CreateTrue","cJSON_ReplaceItemInArray","cJSON_GetArrayItem","cJSON_AddItemReferenceToArray","cJSON_AddArrayToObject","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_SetValuestring","cJSON_Print","cJSON_Minify","cJSON_PrintUnformatted","cJSON_Delete"],"visited":1}
/**/


// Include cJSON header

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<73) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
	//fuzzer shim end}




    // Create a null-terminated string from the input data
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
	return 0;
	}
	
    memcpy(json_str, data, size);
    json_str[size] = '\0';

    // Parse the JSON string using cJSON_ParseWithOpts
    const char *parse_end = nullptr;
    cJSON *root = cJSON_ParseWithOpts(json_str, &parse_end, fuzz_int32_t_1);
    free(json_str);
    
    if (!root) {
        // If parsing fails, try to minify the input string (though it's not valid JSON)
        char *minify_input = (char *)malloc(size + 1);
        if (minify_input) {
            memcpy(minify_input, data, size);
            minify_input[size] = '\0';
            cJSON_Minify(minify_input);
            free(minify_input);
        }
        return 0;
    }

    // Get the "Image" object
    cJSON *image = cJSON_GetObjectItemCaseSensitive(root, fuzz_str_2);
    if (image && cJSON_IsObject(image)) {
        // Get the "IDs" array from Image
        cJSON *ids = cJSON_GetObjectItemCaseSensitive(image, fuzz_str_3);
        if (ids && cJSON_IsArray(ids)) {
            // Check if the first element is a boolean
            cJSON *first_item = cJSON_GetArrayItem(ids, fuzz_int32_t_4);
            if (first_item && cJSON_IsBool(first_item)) {
                // Create a new boolean item (true)
                cJSON *new_bool = cJSON_CreateTrue();
                if (new_bool) {
                    // Replace the first item in the array
                    cJSON_ReplaceItemInArray(ids, fuzz_int32_t_5, new_bool);
                }
            }
            
            // Add a reference of the first item to the array (if it exists)
            cJSON *first_item_ref = cJSON_GetArrayItem(ids, fuzz_int32_t_6);
            if (first_item_ref) {
                cJSON_AddItemReferenceToArray(ids, first_item_ref);
            }
        }
        
        // Add a new array to the Image object
        cJSON *new_array = cJSON_AddArrayToObject(image, fuzz_str_7);
        if (new_array) {
            // Add some items to the new array
            cJSON_AddItemToArray(new_array, cJSON_CreateNumber(1.0));
            cJSON_AddItemToArray(new_array, cJSON_CreateString(fuzz_str_8));
        }
        
        // Get the "Title" field and set its value string
        cJSON *title = cJSON_GetObjectItemCaseSensitive(image, fuzz_str_9);
        if (title && cJSON_IsString(title)) {
            char *new_value = cJSON_SetValuestring(title, fuzz_str_10);
            // Note: cJSON_SetValuestring returns the old valuestring, which we should free
            if (new_value != title->valuestring) {
                // The old string was replaced, but cJSON manages its memory
                // No explicit free needed here as cJSON handles it
            }
        }
    }
    
    // Print the modified JSON
    char *printed = cJSON_Print(root);
    if (printed) {
        // Minify the printed JSON
        cJSON_Minify(printed);
        free(printed);
    }
    
    // Create output file and write the modified JSON
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        char *unformatted = cJSON_PrintUnformatted(root);
        if (unformatted) {
            fwrite(unformatted, strlen(unformatted), 1, out_file);
            free(unformatted);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up
    cJSON_Delete(root);
    
    assert_file_closed(&out_file);
	return 0;
}