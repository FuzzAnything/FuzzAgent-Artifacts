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
//<ID> 75
//<Prompt> []
/*<Combination>: [
*/
//<score> 52, nr_unique_branch: 1
//<Quality> {"density":26,"unique_branches":{"cJSON_ParseWithLengthOpts":[[1182,13,1182,45,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_ParseWithLengthOpts","cJSON_Delete","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateTrue","cJSON_IsTrue","cJSON_Delete","cJSON_DetachItemFromObject","cJSON_SetValuestring","cJSON_AddItemToObject","cJSON_HasObjectItem","cJSON_DeleteItemFromObject","cJSON_Print","cJSON_free","cJSON_GetObjectItemCaseSensitive","cJSON_Duplicate","cJSON_Compare","cJSON_Delete","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_ParseWithLengthOpts","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateTrue","cJSON_IsTrue","cJSON_Delete","cJSON_DetachItemFromObject","cJSON_SetValuestring","cJSON_AddItemToObject","cJSON_HasObjectItem","cJSON_DeleteItemFromObject","cJSON_Print","cJSON_free","cJSON_GetObjectItemCaseSensitive","cJSON_Duplicate","cJSON_Compare","cJSON_Delete","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<87) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
	//fuzzer shim end}




    // Create a FILE pointer from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    if (in_fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Read the entire input into a buffer
    char *json_buffer = (char *)malloc(size + 1);
    if (!json_buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    size_t bytes_read = fread(json_buffer, 1, size, in_file);
    json_buffer[bytes_read] = '\0';
    assert_file_closed(&in_file);;
    
    // Parse JSON using different parsing methods
    cJSON *root1 = cJSON_Parse(json_buffer);
    if (!root1) {
        free(json_buffer);
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Parse again with length option
    const char *parse_end = NULL;
    cJSON *root2 = cJSON_ParseWithLengthOpts(json_buffer, size, &parse_end, fuzz_int32_t_1);
    
    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        cJSON_Delete(root1);
        if (root2) cJSON_Delete(root2);
        free(json_buffer);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    int out_fd = fuzz_fileno(out_file);
    
    // Event: Modify and manipulate JSON structure using required APIs
    if (root1) {
        // Get "menu" object
        cJSON *menu = cJSON_GetObjectItem(root1, fuzz_str_2);
        if (menu && cJSON_IsObject(menu)) {
            // Get "items" array
            cJSON *items = cJSON_GetObjectItem(menu, fuzz_str_3);
            if (items && cJSON_IsArray(items)) {
                // Iterate through array items
                for (int i = 0; i < cJSON_GetArraySize(items); i++) {
                    cJSON *item = cJSON_GetArrayItem(items, fuzz_int32_t_4);
                    if (item && cJSON_IsObject(item)) {
                        // Get "id" field
                        cJSON *id_item = cJSON_GetObjectItem(item, fuzz_str_5);
                        if (id_item && cJSON_IsString(id_item)) {
                            // 1. Use cJSON_IsTrue on a boolean field (though we don't have one in this JSON)
                            // Create a temporary boolean item for demonstration
                            cJSON *temp_bool = cJSON_CreateTrue();
                            cJSON_IsTrue(temp_bool);
                            cJSON_Delete(temp_bool);
                            
                            // 2. Use cJSON_DetachItemFromObject
                            cJSON *detached = cJSON_DetachItemFromObject(item, fuzz_str_6);
                            if (detached) {
                                // 3. Use cJSON_SetValuestring on the detached item
                                cJSON_SetValuestring(detached, fuzz_str_7);
                                
                                // Add it back with a different key
                                cJSON_AddItemToObject(item, fuzz_str_8, detached);
                            }
                        }
                        
                        // Check if there's a "label" field to delete
                        if (cJSON_HasObjectItem(item, fuzz_str_9)) {
                            // 4. Use cJSON_DeleteItemFromObject
                            cJSON_DeleteItemFromObject(item, fuzz_str_10);
                        }
                    }
                }
                
                // 5. Use cJSON_Print to generate modified JSON string
                char *printed_json = cJSON_Print(root1);
                if (printed_json) {
                    // Write to output file
                    fwrite(printed_json, 1, strlen(printed_json), out_file);
                    cJSON_free(printed_json);
                }
            }
        }
    }
    
    // Process second parsed object if available
    if (root2) {
        // Similar operations on root2
        cJSON *menu2 = cJSON_GetObjectItemCaseSensitive(root2, fuzz_str_11);
        if (menu2) {
            // Create a duplicate for comparison
            cJSON *duplicate = cJSON_Duplicate(menu2, fuzz_int32_t_12);
            if (duplicate) {
                // Compare original and duplicate
                cJSON_Compare(menu2, duplicate, fuzz_int32_t_13);
                cJSON_Delete(duplicate);
            }
        }
        
        // Print unformatted version
        char *unformatted = cJSON_PrintUnformatted(root2);
        if (unformatted) {
            // Write to stderr for demonstration
            fwrite(unformatted, 1, strlen(unformatted), stderr);
            cJSON_free(unformatted);
        }
    }
    
    // Clean up
    assert_file_closed(&out_file);;
    cJSON_Delete(root1);
    if (root2) cJSON_Delete(root2);
    free(json_buffer);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}