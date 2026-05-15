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
//<score> 174, nr_unique_branch: 72
//<Quality> {"density":29,"unique_branches":{"cJSON_DetachItemFromArray":[[2296,9,2296,18,0,0,4,1]],"cJSON_AddTrueToObject":[[2165,9,2165,74,0,0,4,0]],"cJSON_Duplicate_rec":[[2797,9,2797,14,0,0,4,1],[2803,9,2803,17,0,0,4,1],[2811,9,2811,26,0,0,4,0],[2811,9,2811,26,0,0,4,1],[2814,13,2814,34,0,0,4,1],[2819,9,2819,21,0,0,4,0],[2819,9,2819,21,0,0,4,1],[2821,27,2821,59,0,0,4,1],[2822,13,2822,29,0,0,4,1],[2828,9,2828,17,0,0,4,1],[2834,12,2834,25,0,0,4,0],[2834,12,2834,25,0,0,4,1],[2836,12,2836,41,0,0,4,1],[2840,13,2840,22,0,0,4,1],[2844,13,2844,25,0,0,4,0],[2844,13,2844,25,0,0,4,1],[2859,9,2859,16,0,0,4,0],[2859,20,2859,34,0,0,4,0],[2859,20,2859,34,0,0,4,1]],"cJSON_Compare":[[3074,39,3074,77,0,0,4,0],[3082,9,3082,25,0,0,4,0],[3083,9,3083,24,0,0,4,0],[3084,9,3084,24,0,0,4,0],[3085,9,3085,26,0,0,4,0],[3086,9,3086,26,0,0,4,0],[3088,9,3088,25,0,0,4,0],[3089,9,3089,26,0,0,4,1],[3097,9,3097,15,0,0,4,1],[3105,9,3105,25,0,0,4,0],[3105,9,3105,25,0,0,4,1],[3106,9,3106,24,0,0,4,0],[3106,9,3106,24,0,0,4,1],[3107,9,3107,24,0,0,4,0],[3107,9,3107,24,0,0,4,1],[3110,9,3110,26,0,0,4,0],[3110,9,3110,26,0,0,4,1],[3111,17,3111,63,0,0,4,0],[3111,17,3111,63,0,0,4,1],[3117,9,3117,26,0,0,4,0],[3117,9,3117,26,0,0,4,1],[3118,9,3118,23,0,0,4,1],[3119,17,3119,41,0,0,4,1],[3119,45,3119,69,0,0,4,1],[3123,17,3123,60,0,0,4,0],[3123,17,3123,60,0,0,4,1],[3130,9,3130,25,0,0,4,0],[3130,9,3130,25,0,0,4,1],[3135,20,3135,39,0,0,4,0],[3135,20,3135,39,0,0,4,1],[3135,43,3135,62,0,0,4,0],[3137,21,3137,73,0,0,4,0],[3137,21,3137,73,0,0,4,1],[3147,17,3147,39,0,0,4,1],[3154,9,3154,26,0,0,4,0],[3154,9,3154,26,0,0,4,1],[3162,21,3162,38,0,0,4,0],[3162,21,3162,38,0,0,4,1],[3167,21,3167,73,0,0,4,0],[3167,21,3167,73,0,0,4,1],[3178,21,3178,38,0,0,4,0],[3178,21,3178,38,0,0,4,1],[3183,21,3183,73,0,0,4,1],[3192,9,3192,16,0,0,4,1],[296,58,296,73,29,0,4,0],[296,99,296,114,29,0,4,0],[296,99,296,114,29,0,4,1],[296,58,296,73,32,0,4,0],[296,99,296,114,32,0,4,0],[296,99,296,114,32,0,4,1]],"cJSON_CreateStringReference":[[2551,9,2551,21,0,0,4,0]]},"library_calls":["cJSON_malloc","cJSON_Parse","cJSON_free","cJSON_Duplicate","cJSON_Delete","cJSON_free","cJSON_GetObjectItem","cJSON_IsObject","cJSON_AddTrueToObject","cJSON_CreateStringReference","cJSON_AddItemToObjectCS","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_Compare","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_Parse","cJSON_Duplicate","cJSON_GetObjectItem","cJSON_IsObject","cJSON_AddTrueToObject","cJSON_CreateStringReference","cJSON_AddItemToObjectCS","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_Compare","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<145) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
	//fuzzer shim end}




    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read the JSON data from the file
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *json_string = (char *)cJSON_malloc(file_size + 1);
    if (!json_string) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(json_string, 1, file_size, in_file);
    json_string[bytes_read] = '\0';
    assert_file_closed(&in_file);;
    
    // Parse the JSON data
    cJSON *parsed_json = cJSON_Parse(json_string);
    if (!parsed_json) {
        cJSON_free(json_string);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a duplicate of the parsed JSON (with recursion)
    cJSON *duplicate_json = cJSON_Duplicate(parsed_json, fuzz_int32_t_1);
    if (!duplicate_json) {
        cJSON_Delete(parsed_json);
        cJSON_free(json_string);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Find the glossary object to work with
    cJSON *glossary = cJSON_GetObjectItem(duplicate_json, fuzz_str_2);
    if (glossary && cJSON_IsObject(glossary)) {
        // Add a true value to the glossary object
        cJSON *true_item = cJSON_AddTrueToObject(glossary, fuzz_str_3);
        
        // Create a string reference
        const char *ref_string = "fuzzed_string_reference";
        cJSON *string_ref = cJSON_CreateStringReference(fuzz_str_4);
        
        // Add the string reference to the glossary object with case-sensitive addition
        if (string_ref) {
            cJSON_AddItemToObjectCS(glossary, fuzz_str_5, string_ref);
        }
        
        // Find and modify an array if it exists
        cJSON *gloss_see_also = NULL;
        cJSON *gloss_div = cJSON_GetObjectItem(glossary, fuzz_str_6);
        if (gloss_div && cJSON_IsObject(gloss_div)) {
            cJSON *gloss_list = cJSON_GetObjectItem(gloss_div, fuzz_str_7);
            if (gloss_list && cJSON_IsObject(gloss_list)) {
                cJSON *gloss_entry = cJSON_GetObjectItem(gloss_list, fuzz_str_8);
                if (gloss_entry && cJSON_IsObject(gloss_entry)) {
                    cJSON *gloss_def = cJSON_GetObjectItem(gloss_entry, fuzz_str_9);
                    if (gloss_def && cJSON_IsObject(gloss_def)) {
                        gloss_see_also = cJSON_GetObjectItem(gloss_def, fuzz_str_10);
                    }
                }
            }
        }
        
        // Delete an item from the array if it exists and has items
        if (gloss_see_also && cJSON_IsArray(gloss_see_also)) {
            int array_size = cJSON_GetArraySize(gloss_see_also);
            if (array_size > 0) {
                // Delete the first item from the array
                cJSON_DeleteItemFromArray(gloss_see_also, fuzz_int32_t_11);
            }
        }
    }
    
    // Compare the original and modified JSON (case-sensitive comparison)
    cJSON_bool are_equal = cJSON_Compare(parsed_json, duplicate_json, fuzz_int32_t_12);
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Print the modified JSON to the output file
        char *modified_json = cJSON_Print(duplicate_json);
        if (modified_json) {
            fwrite(modified_json, 1, strlen(modified_json), out_file);
            cJSON_free(modified_json);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up all allocated resources
    cJSON_Delete(parsed_json);
    cJSON_Delete(duplicate_json);
    cJSON_free(json_string);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}