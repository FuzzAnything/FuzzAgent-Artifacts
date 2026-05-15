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
//<ID> 159
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 3
//<Quality> {"density":19,"unique_branches":{"cJSON_InsertItemInArray":[[2349,9,2349,39,0,0,4,0],[2349,43,2349,71,0,0,4,1],[2357,9,2357,39,0,0,4,1]]},"library_calls":["cJSON_ParseWithLength","cJSON_GetObjectItem","cJSON_IsObject","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsArray","cJSON_Delete","cJSON_DetachItemFromObject","cJSON_Delete","cJSON_AddFalseToObject","cJSON_AddNullToObject","cJSON_CreateStringReference","cJSON_GetArraySize","cJSON_InsertItemInArray","cJSON_AddItemToArray","cJSON_CreateStringReference","cJSON_AddItemToArray","cJSON_Print","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_DetachItemFromObject","cJSON_Delete","cJSON_AddFalseToObject","cJSON_AddNullToObject","cJSON_CreateStringReference","cJSON_GetArraySize","cJSON_InsertItemInArray","cJSON_CreateStringReference","cJSON_AddItemToArray","cJSON_Print","cJSON_Delete"],"visited":1}
/**/


// Include cJSON headers

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<113) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
	//fuzzer shim end}




    // Create input file from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the entire input into a buffer
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *json_buffer = (char*)malloc(file_size + 1);
    if (!json_buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(json_buffer, 1, file_size, in_file);
    json_buffer[bytes_read] = '\0';
    assert_file_closed(&in_file);;

    // Parse the JSON input
    cJSON *root = cJSON_ParseWithLength(json_buffer, bytes_read);
    if (!root) {
        free(json_buffer);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the "menu" object from the root
    cJSON *menu = cJSON_GetObjectItem(root, fuzz_str_1);
    if (!menu || !cJSON_IsObject(menu)) {
        cJSON_Delete(root);
        free(json_buffer);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the "items" array from the menu
    cJSON *items = cJSON_GetObjectItem(menu, fuzz_str_2);
    if (!items || !cJSON_IsArray(items)) {
        cJSON_Delete(root);
        free(json_buffer);
        assert_file_closed(&in_file);
	return 0;
    }

    // Event: Manipulate the JSON structure using the specified APIs
    // 1. Detach an item from the menu object
    cJSON *detached_header = cJSON_DetachItemFromObject(menu, fuzz_str_3);
    if (detached_header) {
        cJSON_Delete(detached_header);
    }

    // 2. Add a false boolean to the menu object
    cJSON *false_item = cJSON_AddFalseToObject(menu, fuzz_str_4);

    // 3. Add a null to the menu object
    cJSON *null_item = cJSON_AddNullToObject(menu, fuzz_str_5);

    // 4. Create a string reference
    const char *ref_string = "fuzzed_string_reference";
    cJSON *string_ref = cJSON_CreateStringReference(fuzz_str_6);

    // 5. Insert the string reference into the items array at index 2
    if (cJSON_GetArraySize(items) >= 2) {
        cJSON_InsertItemInArray(items, fuzz_int32_t_7, string_ref);
    } else {
        // If array is too small, add to the end
        cJSON_AddItemToArray(items, string_ref);
    }

    // 6. Add another item to the items array (duplicate the string reference)
    cJSON *string_ref2 = cJSON_CreateStringReference(fuzz_str_8);
    cJSON_AddItemToArray(items, string_ref2);

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Print the modified JSON to the output file
        char *printed_json = cJSON_Print(root);
        if (printed_json) {
            fwrite(printed_json, 1, strlen(printed_json), out_file);
            free(printed_json);
        }
        assert_file_closed(&out_file);;
    }

    // Clean up allocated resources
    cJSON_Delete(root);
    free(json_buffer);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}