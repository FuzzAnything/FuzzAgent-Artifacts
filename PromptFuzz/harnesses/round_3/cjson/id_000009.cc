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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 23, nr_unique_branch: 0
//<Quality> {"density":23,"unique_branches":{},"library_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_free","cJSON_CreateArray","cJSON_Delete","cJSON_free","cJSON_CreateStringArray","cJSON_Delete","cJSON_Delete","cJSON_free","cJSON_InsertItemInArray","cJSON_HasObjectItem","cJSON_HasObjectItem","cJSON_GetObjectItem","cJSON_IsObject","cJSON_AddFalseToObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArrayItem","cJSON_IsInvalid","cJSON_CreateNumber","cJSON_SetNumberHelper","cJSON_ReplaceItemViaPointer","cJSON_PrintBuffered","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_CreateArray","cJSON_CreateStringArray","cJSON_InsertItemInArray","cJSON_HasObjectItem","cJSON_HasObjectItem","cJSON_GetObjectItem","cJSON_IsObject","cJSON_AddFalseToObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArrayItem","cJSON_IsInvalid","cJSON_CreateNumber","cJSON_SetNumberHelper","cJSON_ReplaceItemViaPointer","cJSON_PrintBuffered","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<80) return 0;

	
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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
	
		const char * fuzzer_var_cJSON_CreateStringArray_6_0[3] = {fuzz_str_1,fuzz_str_2,fuzz_str_3};//fuzzer shim end}




    // Create a FILE pointer from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the entire input into a buffer
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *input_buffer = (char *)cJSON_malloc(file_size + 1);
    if (!input_buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(input_buffer, 1, file_size, in_file);
    input_buffer[bytes_read] = '\0';
    assert_file_closed(&in_file);;

    // Parse the JSON input
    cJSON *parsed_json = cJSON_ParseWithLength(input_buffer, bytes_read);
    if (!parsed_json) {
        cJSON_free(input_buffer);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a new array
    cJSON *new_array = cJSON_CreateArray();
    if (!new_array) {
        cJSON_Delete(parsed_json);
        cJSON_free(input_buffer);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a string array
    const char *string_array_data[] = {"fuzz", "test", "data"};
    cJSON *string_array = cJSON_CreateStringArray(fuzzer_var_cJSON_CreateStringArray_6_0, 3);
    if (!string_array) {
        cJSON_Delete(new_array);
        cJSON_Delete(parsed_json);
        cJSON_free(input_buffer);
        assert_file_closed(&in_file);
	return 0;
    }

    // Insert the string array into the new array
    cJSON_InsertItemInArray(new_array, 0, string_array);

    // Check if the parsed JSON has specific object items
    cJSON_bool has_menu = cJSON_HasObjectItem(parsed_json, fuzz_str_4);
    cJSON_bool has_header = cJSON_HasObjectItem(parsed_json, fuzz_str_5);

    // Get the menu object
    cJSON *menu = cJSON_GetObjectItem(parsed_json, fuzz_str_6);
    if (menu && cJSON_IsObject(menu)) {
        // Add a false boolean to the menu object
        cJSON_AddFalseToObject(menu, fuzz_str_7);
        
        // Check if items exist in menu
        cJSON *items = cJSON_GetObjectItem(menu, fuzz_str_8);
        if (items && cJSON_IsArray(items)) {
            // Get first array item
            cJSON *first_item = cJSON_GetArrayItem(items, fuzz_int32_t_9);
            if (first_item) {
                // Check if the first item is invalid (it shouldn't be)
                cJSON_bool is_invalid = cJSON_IsInvalid(first_item);
                
                // If we have a valid item, try to replace it via pointer
                if (!is_invalid && first_item->child) {
                    // Create a replacement number item
                    cJSON *replacement = cJSON_CreateNumber(42.5);
                    if (replacement) {
                        // Use the number helper to set the value
                        cJSON_SetNumberHelper(replacement, 99.9);
                        
                        // Replace the child of the first item
                        cJSON_ReplaceItemViaPointer(first_item, first_item->child, replacement);
                    }
                }
            }
        }
    }

    // Print the modified JSON with buffering
    char *printed_json = cJSON_PrintBuffered(parsed_json, fuzz_int32_t_10, fuzz_int32_t_11);
    if (printed_json) {
        // Create output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed_json, 1, strlen(printed_json), out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(printed_json);
    }

    // Clean up
    cJSON_Delete(new_array);
    cJSON_Delete(parsed_json);
    cJSON_free(input_buffer);

    assert_file_closed(&in_file);
	return 0;
}