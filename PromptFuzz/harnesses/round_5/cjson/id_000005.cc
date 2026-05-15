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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["cJSON_malloc","cJSON_Parse","cJSON_CreateRaw","cJSON_CreateStringReference","cJSON_CreateFalse","cJSON_CreateString","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_Duplicate","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_Duplicate","cJSON_Delete","cJSON_IsFalse","cJSON_malloc","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_free","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_Parse","cJSON_CreateRaw","cJSON_CreateStringReference","cJSON_CreateFalse","cJSON_CreateString","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_Duplicate","cJSON_Delete","cJSON_IsFalse","cJSON_malloc","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_free","cJSON_free"],"visited":0}
/**/


// Include cJSON headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create null-terminated string from input data for parsing
    char *json_string = (char *)cJSON_malloc(size + 1);
    if (!json_string) {
        return 0;
    }
    memcpy(json_string, data, size);
    json_string[size] = '\0';

    // 1. Parse JSON using the input data
    cJSON *parsed_json = cJSON_Parse(json_string);
    
    // 2. Create various cJSON objects using required APIs
    cJSON *raw_item = cJSON_CreateRaw("raw_data");
    cJSON *string_ref = cJSON_CreateStringReference("string_reference");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *string_item = cJSON_CreateString("test_string");
    
    // 3. Use cJSON_GetNumberValue if we have parsed JSON
    if (parsed_json) {
        // Try to get a number value from the parsed JSON
        // Look for numeric fields in the example JSON structure
        cJSON *widget = cJSON_GetObjectItem(parsed_json, "widget");
        if (widget) {
            cJSON *window = cJSON_GetObjectItem(widget, "window");
            if (window) {
                cJSON *width = cJSON_GetObjectItem(window, "width");
                if (width && cJSON_IsNumber(width)) {
                    double width_value = cJSON_GetNumberValue(width);
                    // Use the value to avoid compiler optimization
                    if (width_value > 1000) {
                        // This branch is unlikely but prevents optimization
                    }
                }
                
                cJSON *height = cJSON_GetObjectItem(window, "height");
                if (height && cJSON_IsNumber(height)) {
                    double height_value = cJSON_GetNumberValue(height);
                    // Use the value
                    if (height_value < 0) {
                        // This branch is unlikely but prevents optimization
                    }
                }
            }
        }
    }
    
    // 4. Duplicate the parsed JSON (if exists) or create a new one
    cJSON *duplicated = NULL;
    if (parsed_json) {
        duplicated = cJSON_Duplicate(parsed_json, 1); // Deep copy
    } else {
        // Create a simple JSON to duplicate if parsing failed
        cJSON *temp = cJSON_CreateObject();
        cJSON_AddNumberToObject(temp, "test", 42);
        duplicated = cJSON_Duplicate(temp, 1);
        cJSON_Delete(temp);
    }
    
    // 5. Check if the false item is indeed false
    cJSON_bool is_false = cJSON_IsFalse(false_item);
    if (!is_false) {
        // This should never happen, but handle it
    }
    
    // 6. Use cJSON_malloc for additional memory allocation
    void *extra_memory = cJSON_malloc(128);
    if (extra_memory) {
        // Initialize and use the memory
        memset(extra_memory, 0, 128);
        // Do something with the memory to avoid optimization
        char *mem_as_char = (char *)extra_memory;
        mem_as_char[0] = 't';
        mem_as_char[1] = 'e';
        mem_as_char[2] = 's';
        mem_as_char[3] = 't';
    }
    
    // 7. Create a FILE* to read the input data (as requested)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read a small portion to demonstrate usage
        char buffer[16];
        size_t read_bytes = fread(buffer, 1, sizeof(buffer), in_file);
        // Use the read data to avoid optimization
        if (read_bytes > 0) {
            buffer[read_bytes % sizeof(buffer)] = '\0';
        }
        assert_file_closed(&in_file);;
    }
    
    // 8. Create a FILE* for output (as requested)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write something to the file
        const char *test_output = "fuzz_test_output";
        fwrite(test_output, 1, strlen(test_output), out_file);
        assert_file_closed(&out_file);;
    }
    
    // 9. Get file descriptors (as requested)
    if (in_file) {
        int fd_in = fuzz_fileno(in_file);
        // fd_in would be used if we kept the file open
    }
    
    if (out_file) {
        int fd_out = fuzz_fileno(out_file);
        // fd_out would be used if we kept the file open
    }
    
    // 10. Clean up all allocated resources
    if (parsed_json) {
        cJSON_Delete(parsed_json);
    }
    
    if (duplicated) {
        cJSON_Delete(duplicated);
    }
    
    if (raw_item) {
        cJSON_Delete(raw_item);
    }
    
    if (string_ref) {
        // Note: string_ref doesn't own the string, so we don't delete the string
        cJSON_Delete(string_ref);
    }
    
    if (false_item) {
        cJSON_Delete(false_item);
    }
    
    if (string_item) {
        cJSON_Delete(string_item);
    }
    
    if (json_string) {
        cJSON_free(json_string);
    }
    
    if (extra_memory) {
        cJSON_free(extra_memory);
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}