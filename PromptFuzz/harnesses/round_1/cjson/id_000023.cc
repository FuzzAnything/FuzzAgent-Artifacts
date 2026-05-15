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
//<ID> 71
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 1
//<Quality> {"density":19,"unique_branches":{"cJSON_ReplaceItemViaPointer":[[2370,81,2370,95,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsArray","cJSON_Delete","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_CreateRaw","cJSON_AddRawToObject","cJSON_Delete","cJSON_Delete","cJSON_CreateString","cJSON_AddItemToArray","cJSON_Delete","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_CreateString","cJSON_ReplaceItemViaPointer","cJSON_Delete","cJSON_CreateString","cJSON_ReplaceItemInObject","cJSON_Delete","cJSON_Print","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_CreateRaw","cJSON_AddRawToObject","cJSON_Delete","cJSON_Delete","cJSON_CreateString","cJSON_AddItemToArray","cJSON_Delete","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_CreateString","cJSON_ReplaceItemViaPointer","cJSON_Delete","cJSON_CreateString","cJSON_ReplaceItemInObject","cJSON_Delete","cJSON_Print","cJSON_Delete"],"visited":0}
/*We'll create a fuzz driver that manipulates the provided JSON structure using the specified cJSON APIs. The goal is to modify the JSON by:
1. Deleting an item from the "IDs" array
2. Adding a raw JSON item to the "Image" object
3. Adding a string to the "IDs" array
4. Replacing items via pointer and key

Here's the complete fuzz driver:

*/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE pointer to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the input data into a buffer
    fseek(in_file, 0, SEEK_END);
    long input_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *input_buffer = (char *)malloc(input_size + 1);
    if (!input_buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    fread(input_buffer, 1, input_size, in_file);
    input_buffer[input_size] = '\0';
    assert_file_closed(&in_file);;

    // Parse the JSON input
    cJSON *root = cJSON_Parse(input_buffer);
    free(input_buffer);
    
    if (!root) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the "Image" object
    cJSON *image = cJSON_GetObjectItem(root, "Image");
    if (!image) {
        cJSON_Delete(root);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the "IDs" array from Image object
    cJSON *ids_array = cJSON_GetObjectItem(image, "IDs");
    if (!ids_array || !cJSON_IsArray(ids_array)) {
        cJSON_Delete(root);
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Delete an item from the array (if it has at least 2 elements)
    int array_size = cJSON_GetArraySize(ids_array);
    if (array_size >= 2) {
        // Delete the second element (index 1)
        cJSON_DeleteItemFromArray(ids_array, 1);
    }

    // 2. Create a raw JSON item
    cJSON *raw_item = cJSON_CreateRaw("\"raw_data\": 12345");
    if (raw_item) {
        // 3. Add the raw item to the Image object
        if (!cJSON_AddRawToObject(image, "RawData", "\"raw_data\": 12345")) {
            cJSON_Delete(raw_item);
        }
        // Note: cJSON_AddRawToObject creates its own copy, so we can delete our raw_item
        cJSON_Delete(raw_item);
    }

    // 4. Create a string item
    cJSON *string_item = cJSON_CreateString("fuzz_string");
    if (string_item) {
        // 5. Add the string to the IDs array
        if (!cJSON_AddItemToArray(ids_array, string_item)) {
            cJSON_Delete(string_item);
        }
    }

    // Get the "Thumbnail" object
    cJSON *thumbnail = cJSON_GetObjectItem(image, "Thumbnail");
    if (thumbnail) {
        // Get the "Url" item from Thumbnail
        cJSON *url_item = cJSON_GetObjectItem(thumbnail, "Url");
        if (url_item) {
            // 6. Replace the Url item via pointer
            cJSON *new_url = cJSON_CreateString("http://fuzz.example.com");
            if (new_url) {
                if (!cJSON_ReplaceItemViaPointer(thumbnail, url_item, new_url)) {
                    cJSON_Delete(new_url);
                }
            }
        }
    }

    // 7. Replace the "Title" in Image object
    cJSON *new_title = cJSON_CreateString("Fuzzed Title");
    if (new_title) {
        if (!cJSON_ReplaceItemInObject(image, "Title", new_title)) {
            cJSON_Delete(new_title);
        }
    }

    // Create an output file to write the modified JSON
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        char *json_str = cJSON_Print(root);
        if (json_str) {
            fwrite(json_str, 1, strlen(json_str), out_file);
            free(json_str);
        }
        
        int fd = fuzz_fileno(out_file);
        if (fd != -1) {
            // Example: Use the file descriptor if needed
            // fsync(fd); // This is just an example
        }
        
        assert_file_closed(&out_file);;
    }

    // Clean up
    cJSON_Delete(root);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}