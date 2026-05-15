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
//<ID> 219
//<Prompt> []
/*<Combination>: [
*/
//<score> 50, nr_unique_branch: 1
//<Quality> {"density":25,"unique_branches":{"print_value":[[1474,17,1474,31,0,0,4,0]]},"library_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_free","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsFalse","cJSON_AddNullToObject","cJSON_AddRawToObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_SetValuestring","cJSON_PrintUnformatted","cJSON_free","cJSON_Print","cJSON_Minify","cJSON_free","cJSON_PrintPreallocated","cJSON_CreateString","cJSON_ReplaceItemInObject","cJSON_Delete","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsFalse","cJSON_AddNullToObject","cJSON_AddRawToObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_SetValuestring","cJSON_PrintUnformatted","cJSON_free","cJSON_Print","cJSON_Minify","cJSON_free","cJSON_PrintPreallocated","cJSON_CreateString","cJSON_ReplaceItemInObject","cJSON_Delete","cJSON_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<100) return 0;

	
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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
	//fuzzer shim end}




    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read the entire input into a buffer
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *json_buffer = (char *)cJSON_malloc(file_size + 1);
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
        cJSON_free(json_buffer);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Create a true value and add it to object
    cJSON *true_item = cJSON_CreateTrue();
    if (true_item) {
        cJSON_AddItemToObject(root, fuzz_str_1, true_item);
    }
    
    // 2. Check if an item is false
    cJSON *false_check_item = cJSON_GetObjectItem(root, fuzz_str_2);
    if (false_check_item) {
        cJSON_bool is_false = cJSON_IsFalse(false_check_item);
        // Result not used, just called for coverage
    }
    
    // 3. Add a null to the object
    cJSON *null_item = cJSON_AddNullToObject(root, fuzz_str_3);
    
    // 4. Add a raw string to the object
    const char *raw_string = "raw_fuzz_data";
    cJSON *raw_item = cJSON_AddRawToObject(root, "fuzz_raw", fuzz_str_4);
    
    // 5. Set valuestring on an item (if it's a string type)
    cJSON *string_item = cJSON_GetObjectItem(root, fuzz_str_5);
    if (string_item && cJSON_IsObject(string_item)) {
        cJSON *title_item = cJSON_GetObjectItem(string_item, fuzz_str_6);
        if (title_item && cJSON_IsString(title_item)) {
            char *new_valuestring = cJSON_SetValuestring(title_item, fuzz_str_7);
            // new_valuestring is the old value, we don't need it
        }
    }
    
    // 6. Print unformatted JSON
    char *unformatted = cJSON_PrintUnformatted(root);
    if (unformatted) {
        // Create output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(unformatted, 1, strlen(unformatted), out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(unformatted);
    }
    
    // 7. Minify a JSON string
    char *json_to_minify = cJSON_Print(root);
    if (json_to_minify) {
        cJSON_Minify(json_to_minify);
        cJSON_free(json_to_minify);
    }
    
    // 8. Print using preallocated buffer
    char prealloc_buffer[4096];
    cJSON_bool print_success = cJSON_PrintPreallocated(root, prealloc_buffer, sizeof(prealloc_buffer), fuzz_int32_t_8);
    // Result not used, just called for coverage
    
    // 9. Replace an item in the object
    cJSON *new_replacement = cJSON_CreateString(fuzz_str_9);
    if (new_replacement && raw_item) {
        cJSON_ReplaceItemInObject(root, "fuzz_raw", new_replacement);
    } else if (new_replacement) {
        cJSON_Delete(new_replacement);
    }
    
    // Cleanup
    cJSON_Delete(root);
    cJSON_free(json_buffer);
    
    assert_file_closed(&in_file);
	return 0;
}