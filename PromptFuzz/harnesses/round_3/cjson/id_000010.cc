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
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 41
//<Quality> {"density":16,"unique_branches":{"cJSON_Delete":[[268,13,268,48,0,0,4,1]],"cJSON_CreateNumber":[[2514,13,2514,27,0,0,4,0],[2518,18,2518,40,0,0,4,0]],"cJSON_CreateDoubleArray":[[2708,9,2708,20,0,0,4,1],[2708,24,2708,41,0,0,4,1],[2715,16,2715,17,0,0,4,0],[2715,21,2715,40,0,0,4,0],[2715,21,2715,40,0,0,4,1],[2718,12,2718,14,0,0,4,1],[2723,12,2723,14,0,0,4,0],[2734,9,2734,10,0,0,4,0],[2734,14,2734,22,0,0,4,0],[2734,14,2734,22,0,0,4,1]],"cJSON_Compare":[[3074,9,3074,20,0,0,4,1],[3074,24,3074,35,0,0,4,1],[3074,39,3074,77,0,0,4,1],[3082,9,3082,25,0,0,4,1],[3083,9,3083,24,0,0,4,1],[3084,9,3084,24,0,0,4,1],[3085,9,3085,26,0,0,4,1],[3086,9,3086,26,0,0,4,1],[3087,9,3087,23,0,0,4,1],[3088,9,3088,25,0,0,4,1],[3089,9,3089,26,0,0,4,0],[3092,9,3092,16,0,0,4,1],[3097,9,3097,15,0,0,4,0]],"add_item_to_object":[[2092,9,2092,21,0,0,4,0]],"print_number":[[77,18,77,26,2,0,4,0]],"cJSON_IsRaw":[[3064,9,3064,21,0,0,4,1]],"cJSON_SetValuestring":[[441,9,441,25,0,0,4,1],[441,29,441,59,0,0,4,1],[441,63,441,97,0,0,4,1],[446,9,446,36,0,0,4,1],[446,40,446,59,0,0,4,1],[454,9,454,25,0,0,4,0],[454,9,454,25,0,0,4,1],[457,16,457,58,0,0,4,0],[457,16,457,58,0,0,4,1],[457,62,457,104,0,0,4,0],[465,9,465,21,0,0,4,1],[469,9,469,36,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_Delete","cJSON_CreateDoubleArray","cJSON_AddItemToObjectCS","cJSON_CreateString","cJSON_AddItemToObjectCS","cJSON_SetValuestring","cJSON_IsObject","cJSON_AddItemReferenceToObject","cJSON_IsRaw","cJSON_CreateRaw","cJSON_GetObjectItemCaseSensitive","cJSON_ReplaceItemViaPointer","cJSON_Compare","cJSON_Print","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_CreateDoubleArray","cJSON_AddItemToObjectCS","cJSON_CreateString","cJSON_AddItemToObjectCS","cJSON_SetValuestring","cJSON_IsObject","cJSON_AddItemReferenceToObject","cJSON_IsRaw","cJSON_CreateRaw","cJSON_GetObjectItemCaseSensitive","cJSON_ReplaceItemViaPointer","cJSON_Compare","cJSON_Print","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


// Include cJSON headers

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<78) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Read the input data as a string for parsing
    char *input_str = (char *)malloc(size + 1);
    if (!input_str) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    memcpy(input_str, data, size);
    input_str[size] = '\0';
    
    // Parse JSON from input string
    cJSON *parsed_json = cJSON_Parse(input_str);
    free(input_str);
    
    // Create main object
    cJSON *main_obj = cJSON_CreateObject();
    if (!main_obj) {
        if (parsed_json) cJSON_Delete(parsed_json);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a double array using data from input
    // Use first few bytes as doubles (with proper alignment)
    int double_count = (size >= sizeof(double)) ? 1 : 0;
    double double_value = 0.0;
    if (double_count > 0) {
        memcpy(&double_value, data, sizeof(double));
    }
    cJSON *double_array = cJSON_CreateDoubleArray(&double_value, double_count);
    
    // Add double array to main object with cJSON_AddItemToObjectCS
    cJSON_AddItemToObjectCS(main_obj, fuzz_str_1, double_array);
    
    // Create a string item
    cJSON *string_item = cJSON_CreateString(fuzz_str_2);
    if (string_item) {
        // Add string item to object
        cJSON_AddItemToObjectCS(main_obj, "string_key", string_item);
        
        // Use cJSON_SetValuestring to modify the string value
        // Use part of input data as new string value (ensure null termination)
        size_t str_len = (size > 0 && size < 100) ? size : 50;
        char *new_value = (char *)malloc(str_len + 1);
        if (new_value) {
            memcpy(new_value, data, str_len > size ? size : str_len);
            new_value[str_len] = '\0';
            cJSON_SetValuestring(string_item, new_value);
            free(new_value);
        }
    }
    
    // Create a reference item and add it with cJSON_AddItemReferenceToObject
    if (parsed_json && cJSON_IsObject(parsed_json)) {
        cJSON_AddItemReferenceToObject(main_obj, fuzz_str_3, parsed_json);
    }
    
    // Check if double_array is raw (it shouldn't be)
    cJSON_bool is_raw = cJSON_IsRaw(double_array);
    
    // Create replacement item
    cJSON *replacement = cJSON_CreateRaw(fuzz_str_4);
    
    // Use cJSON_ReplaceItemViaPointer to replace the string item
    cJSON *string_item_ptr = cJSON_GetObjectItemCaseSensitive(main_obj, "string_key");
    if (string_item_ptr && replacement) {
        cJSON_ReplaceItemViaPointer(main_obj, string_item_ptr, replacement);
    }
    
    // Compare objects using cJSON_Compare
    cJSON_bool compare_result = cJSON_Compare(main_obj, main_obj, true);
    
    // Write the resulting JSON to output file
    char *printed_json = cJSON_Print(main_obj);
    if (printed_json) {
        fputs(printed_json, out_file);
        free(printed_json);
    }
    
    // Clean up
    cJSON_Delete(main_obj);
    if (parsed_json) cJSON_Delete(parsed_json);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}