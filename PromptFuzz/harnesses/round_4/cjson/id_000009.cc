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
//<ID> 38
//<Prompt> []
/*<Combination>: [
*/
//<score> 87, nr_unique_branch: 2
//<Quality> {"density":29,"unique_branches":{"print_object":[[1791,9,1791,52,0,0,4,0]],"cJSON_InsertItemInArray":[[2344,9,2344,31,0,0,4,0]]},"library_calls":["cJSON_Version","cJSON_ParseWithLength","cJSON_ParseWithLengthOpts","cJSON_CreateObject","cJSON_AddArrayToObject","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_CreateRaw","cJSON_AddItemToArray","cJSON_AddNullToObject","cJSON_AddRawToObject","cJSON_CreateArrayReference","cJSON_AddItemToObject","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_InsertItemInArray","cJSON_ReplaceItemInArray","cJSON_CreateString","cJSON_Delete","cJSON_Print","cJSON_malloc","cJSON_PrintPreallocated","cJSON_free","cJSON_free","cJSON_PrintUnformatted","cJSON_free","cJSON_malloc","cJSON_Parse","cJSON_Delete","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_Version","cJSON_ParseWithLength","cJSON_ParseWithLengthOpts","cJSON_CreateObject","cJSON_AddArrayToObject","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_CreateRaw","cJSON_AddItemToArray","cJSON_AddNullToObject","cJSON_AddRawToObject","cJSON_CreateArrayReference","cJSON_AddItemToObject","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_InsertItemInArray","cJSON_ReplaceItemInArray","cJSON_CreateString","cJSON_Delete","cJSON_Print","cJSON_malloc","cJSON_PrintPreallocated","cJSON_free","cJSON_free","cJSON_PrintUnformatted","cJSON_free","cJSON_malloc","cJSON_Parse","cJSON_Delete","cJSON_free","cJSON_Delete"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<128) return 0;

	
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
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
	//fuzzer shim end}




    // 1. Get cJSON version
    const char* version = cJSON_Version();
    
    // 2. Parse input JSON using multiple parsing methods
    cJSON* parsed_json = nullptr;
    const char* parse_end = nullptr;
    
    // Try different parsing methods
    if (size > 0) {
        // Method 1: Parse with length
        parsed_json = cJSON_ParseWithLength((const char*)data, size);
        
        // Method 2: Parse with opts (if first method failed)
        if (!parsed_json) {
            parsed_json = cJSON_ParseWithLengthOpts((const char*)data, size, &parse_end, false);
        }
    }
    
    // 3. Create a new object for manipulation
    cJSON* new_object = cJSON_CreateObject();
    
    // 4. Add an array to the new object
    cJSON* new_array = cJSON_AddArrayToObject(new_object, fuzz_str_1);
    
    // 5. If we successfully parsed input, work with it
    if (parsed_json) {
        // Check if parsed item is an object
        if (cJSON_IsObject(parsed_json)) {
            // Get an object item
            cJSON* widget_item = cJSON_GetObjectItem(parsed_json, fuzz_str_2);
            
            if (widget_item && cJSON_IsObject(widget_item)) {
                // Create a duplicate of widget
                cJSON* widget_duplicate = cJSON_Duplicate(widget_item, true);
                
                // Add the duplicate to our new array
                cJSON_AddItemToArray(new_array, widget_duplicate);
                
                // Create and add more items to array
                cJSON_AddItemToArray(new_array, cJSON_CreateNumber(42.5));
                cJSON_AddItemToArray(new_array, cJSON_CreateString(fuzz_str_3));
                
                // Create a raw JSON item
                cJSON* raw_item = cJSON_CreateRaw(fuzz_str_4);
                cJSON_AddItemToArray(new_array, raw_item);
                
                // Add a null item to object
                cJSON_AddNullToObject(new_object, fuzz_str_5);
                
                // Add raw data to object
                cJSON_AddRawToObject(new_object, fuzz_str_6, fuzz_str_7);
                
                // Create array reference
                cJSON* array_ref = cJSON_CreateArrayReference(new_array);
                cJSON_AddItemToObject(new_object, fuzz_str_8, array_ref);
                
                // Array manipulation
                int array_size = cJSON_GetArraySize(new_array);
                if (array_size > 0) {
                    // Detach an item
                    cJSON* detached = cJSON_DetachItemFromArray(new_array, fuzz_int32_t_9);
                    if (detached) {
                        // Re-insert at different position
                        cJSON_InsertItemInArray(new_array, array_size - 1, detached);
                    }
                    
                    // Replace an item if array is big enough
                    if (array_size > 2) {
                        cJSON_ReplaceItemInArray(new_array, fuzz_int32_t_10, cJSON_CreateString(fuzz_str_11));
                    }
                }
            }
        }
        
        // Clean up parsed JSON
        cJSON_Delete(parsed_json);
    }
    
    // 6. Print the resulting JSON
    char* printed_json = cJSON_Print(new_object);
    if (printed_json) {
        // Also try preallocated printing
        int buffer_size = strlen(printed_json) + 100;
        char* prealloc_buffer = (char*)cJSON_malloc(buffer_size);
        if (prealloc_buffer) {
            cJSON_PrintPreallocated(new_object, prealloc_buffer, buffer_size, true);
            cJSON_free(prealloc_buffer);
        }
        
        cJSON_free(printed_json);
    }
    
    // 7. Write to output file
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        char* unformatted = cJSON_PrintUnformatted(new_object);
        if (unformatted) {
            fwrite(unformatted, 1, strlen(unformatted), out_file);
            cJSON_free(unformatted);
        }
        assert_file_closed(&out_file);;
    }
    
    // 8. Read input data as file
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        (void)fd; // Use fd to avoid unused variable warning
        
        // Read and parse from file
        if (size > 0) {
            char* file_buffer = (char*)cJSON_malloc(size + 1);
            if (file_buffer) {
                fread(file_buffer, 1, size, in_file);
                file_buffer[size] = '\0';
                
                cJSON* file_parsed = cJSON_Parse(file_buffer);
                if (file_parsed) {
                    cJSON_Delete(file_parsed);
                }
                cJSON_free(file_buffer);
            }
        }
        assert_file_closed(&in_file);;
    }
    
    // 9. Clean up
    cJSON_Delete(new_object);
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
}