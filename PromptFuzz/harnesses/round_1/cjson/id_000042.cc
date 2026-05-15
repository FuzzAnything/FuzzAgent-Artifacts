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
//<ID> 366
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 1
//<Quality> {"density":15,"unique_branches":{"print_value":[[1436,17,1436,31,0,0,4,0]]},"library_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_ParseWithLengthOpts","cJSON_IsInvalid","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_IsBool","cJSON_CreateObjectReference","cJSON_IsString","cJSON_IsObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_malloc","cJSON_PrintPreallocated","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_free","cJSON_ParseWithLength","cJSON_Delete","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_ParseWithLengthOpts","cJSON_IsInvalid","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_IsBool","cJSON_CreateObjectReference","cJSON_IsString","cJSON_IsObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_malloc","cJSON_PrintPreallocated","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_free","cJSON_ParseWithLength","cJSON_Delete","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified cJSON APIs to process JSON data:

*/


extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a mutable copy of the input data with null termination
    char *json_str = static_cast<char*>(cJSON_malloc(size + 1));
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, data, size);
    json_str[size] = '\0';

    // Parse JSON using cJSON_ParseWithLength
    cJSON *parsed_json1 = cJSON_ParseWithLength(json_str, size);
    
    // Parse JSON using cJSON_ParseWithLengthOpts
    const char *parse_end = nullptr;
    cJSON *parsed_json2 = cJSON_ParseWithLengthOpts(json_str, size, &parse_end, 0); // require_null_terminated = false
    
    // Check if parsed JSON is invalid
    if (parsed_json1) {
        cJSON_bool is_invalid = cJSON_IsInvalid(parsed_json1);
        
        // Create a new object for testing
        cJSON *test_object = cJSON_CreateObject();
        if (test_object) {
            // Add a number to the test object
            double test_number = 42.5;
            cJSON *added_number = cJSON_AddNumberToObject(test_object, "test_number", test_number);
            
            // Try to get number value from parsed JSON
            if (cJSON_IsNumber(parsed_json1)) {
                double num_value = cJSON_GetNumberValue(parsed_json1);
            }
            
            // Check if items are boolean
            if (parsed_json1->child) {
                cJSON_bool is_bool = cJSON_IsBool(parsed_json1->child);
            }
            
            // Create object reference
            cJSON *object_ref = cJSON_CreateObjectReference(parsed_json1);
            
            // Check if items are string
            if (parsed_json1->child) {
                cJSON_bool is_string = cJSON_IsString(parsed_json1->child);
            }
            
            // Try to detach an item (if object has children)
            if (cJSON_IsObject(parsed_json1) && parsed_json1->child) {
                cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(parsed_json1, "test");
                if (detached) {
                    cJSON_Delete(detached);
                }
            }
            
            // Print JSON to preallocated buffer
            const int buffer_size = 1024;
            char *buffer = static_cast<char*>(cJSON_malloc(buffer_size));
            if (buffer) {
                cJSON_bool print_success = cJSON_PrintPreallocated(parsed_json1, buffer, buffer_size, 1);
                cJSON_free(buffer);
            }
            
            // Clean up test objects
            cJSON_Delete(test_object);
            if (object_ref) {
                cJSON_Delete(object_ref);
            }
        }
        
        // Clean up parsed JSON
        cJSON_Delete(parsed_json1);
    }
    
    if (parsed_json2) {
        cJSON_Delete(parsed_json2);
    }
    
    // Free the copied string
    cJSON_free(json_str);
    
    // Test with FILE operations as required
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read and process some data from the file
        char temp_buffer[256];
        size_t read_bytes = fread(temp_buffer, 1, sizeof(temp_buffer) - 1, in_file);
        temp_buffer[read_bytes] = '\0';
        
        // Try parsing the read data
        if (read_bytes > 0) {
            cJSON *file_parsed = cJSON_ParseWithLength(temp_buffer, read_bytes);
            if (file_parsed) {
                cJSON_Delete(file_parsed);
            }
        }
        
        assert_file_closed(&in_file);;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Create a simple JSON to write
        cJSON *output_json = cJSON_CreateObject();
        if (output_json) {
            cJSON_AddNumberToObject(output_json, "fuzz_test", 123.456);
            char *output_str = cJSON_PrintUnformatted(output_json);
            if (output_str) {
                fwrite(output_str, 1, strlen(output_str), out_file);
                cJSON_free(output_str);
            }
            cJSON_Delete(output_json);
        }
        assert_file_closed(&out_file);;
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}