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
//<ID> 128
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_Delete","cJSON_CreateRaw","cJSON_AddRawToObject","cJSON_AddArrayToObject","cJSON_AddItemToArray","cJSON_CreateTrue","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_CreateTrue","cJSON_AddItemToArray","cJSON_DeleteItemFromArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_SetNumberHelper","cJSON_AddItemToObject","cJSON_PrintBuffered","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_CreateRaw","cJSON_AddRawToObject","cJSON_AddArrayToObject","cJSON_AddItemToArray","cJSON_CreateTrue","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_CreateTrue","cJSON_AddItemToArray","cJSON_DeleteItemFromArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_SetNumberHelper","cJSON_AddItemToObject","cJSON_PrintBuffered","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure we have minimum data to work with
    if (size == 0) {
        return 0;
    }

    // Create input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the input data into a string for parsing
    char *input_str = (char *)malloc(size + 1);
    if (!input_str) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(input_str, 1, size, in_file);
    input_str[bytes_read] = '\0';
    assert_file_closed(&in_file);;

    // Parse the input JSON
    cJSON *parsed_json = cJSON_ParseWithLength(input_str, bytes_read);
    if (!parsed_json) {
        free(input_str);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a new object for our operations
    cJSON *root_object = cJSON_CreateObject();
    if (!root_object) {
        cJSON_Delete(parsed_json);
        free(input_str);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create raw JSON from input string
    cJSON *raw_json = cJSON_CreateRaw(input_str);
    if (raw_json) {
        // Add raw object to main object
        cJSON_AddRawToObject(root_object, "raw_data", input_str);
    }

    // Create array and add to object
    cJSON *array = cJSON_AddArrayToObject(root_object, "test_array");
    if (array) {
        // Add some items to the array
        cJSON_AddItemToArray(array, cJSON_CreateTrue());
        cJSON_AddItemToArray(array, cJSON_CreateNumber(42.5));
        
        // Create another true value
        cJSON *true_item = cJSON_CreateTrue();
        if (true_item) {
            cJSON_AddItemToArray(array, true_item);
            
            // Delete the last item from array (the true we just added)
            cJSON_DeleteItemFromArray(array, 2);
        }
        
        // Add the raw json to array
        if (raw_json) {
            cJSON_AddItemToArray(array, raw_json);
        }
    }

    // Create a number object and use SetNumberHelper
    cJSON *number_obj = cJSON_CreateNumber(0.0);
    if (number_obj) {
        cJSON_SetNumberHelper(number_obj, 3.14159);
        cJSON_AddItemToObject(root_object, "pi", number_obj);
    }

    // Test PrintBuffered with formatting
    char *buffered_output = cJSON_PrintBuffered(root_object, 4096, 1);
    if (buffered_output) {
        free(buffered_output);
    }

    // Test PrintPreallocated
    char *prealloc_buffer = (char *)malloc(8192);
    if (prealloc_buffer) {
        cJSON_PrintPreallocated(root_object, prealloc_buffer, 8192, 0);
        free(prealloc_buffer);
    }

    // Create output file for testing
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Get file descriptor
        int fd = fuzz_fileno(out_file);
        (void)fd; // Use fd to avoid unused variable warning
        
        // Write something to output file
        fprintf(out_file, "Test output\n");
        assert_file_closed(&out_file);;
    }

    // Clean up all allocated resources
    cJSON_Delete(parsed_json);
    cJSON_Delete(root_object);
    free(input_str);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}