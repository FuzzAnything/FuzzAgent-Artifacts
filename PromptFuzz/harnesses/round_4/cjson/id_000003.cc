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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":28,"unique_branches":{"cJSON_ReplaceItemViaPointer":[[2389,13,2389,49,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_ParseWithLength","cJSON_Version","cJSON_CreateObject","cJSON_Delete","cJSON_AddArrayToObject","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_CreateArrayReference","cJSON_AddItemToObject","cJSON_AddRawToObject","cJSON_IsObject","cJSON_Duplicate","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_CreateNumber","cJSON_ReplaceItemInArray","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_PrintPreallocated","cJSON_PrintPreallocated","cJSON_Print","cJSON_free","cJSON_Parse","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_ParseWithLength","cJSON_Version","cJSON_CreateObject","cJSON_AddArrayToObject","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_CreateArrayReference","cJSON_AddItemToObject","cJSON_AddRawToObject","cJSON_IsObject","cJSON_Duplicate","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_GetArraySize","cJSON_IsArray","cJSON_CreateNumber","cJSON_ReplaceItemInArray","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_PrintPreallocated","cJSON_PrintPreallocated","cJSON_Print","cJSON_free","cJSON_Parse","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":1}
/*To create a comprehensive fuzz driver that uses all the specified cJSON APIs, I'll design a scenario that parses JSON input, modifies the structure using various APIs, and tests edge cases. Here's the complete implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<115) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
	//fuzzer shim end}




    // 1. Parse the input data as JSON
    if (size == 0) {
        return 0;
    }
    
    // Ensure null termination for string operations
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    // Parse with different methods to test various parsing APIs
    cJSON *parsed_json = cJSON_Parse(json_str);
    if (!parsed_json) {
        // Try parsing with length
        parsed_json = cJSON_ParseWithLength(json_str, size);
    }
    
    if (!parsed_json) {
        free(json_str);
        return 0;
    }
    
    // 2. Get cJSON version
    const char *version = cJSON_Version();
    
    // 3. Create a new object to work with
    cJSON *root_object = cJSON_CreateObject();
    if (!root_object) {
        cJSON_Delete(parsed_json);
        free(json_str);
        return 0;
    }
    
    // 4. Add an array to the object using cJSON_AddArrayToObject
    cJSON *new_array = cJSON_AddArrayToObject(root_object, fuzz_str_1);
    
    // 5. Create array reference and add items
    if (new_array) {
        // Add some items to the array
        cJSON_AddItemToArray(new_array, cJSON_CreateNumber(42));
        cJSON_AddItemToArray(new_array, cJSON_CreateString(fuzz_str_2));
        
        // Create an array reference
        cJSON *array_ref = cJSON_CreateArrayReference(new_array);
        if (array_ref) {
            // Add the reference to the root object
            cJSON_AddItemToObject(root_object, fuzz_str_3, array_ref);
        }
    }
    
    // 6. Add raw JSON to object
    cJSON_AddRawToObject(root_object, fuzz_str_4, fuzz_str_5);
    
    // 7. Process the parsed JSON - add items to object
    if (cJSON_IsObject(parsed_json)) {
        // Get the first item from parsed JSON
        cJSON *first_item = parsed_json->child;
        if (first_item) {
            // Create a copy of the first item
            cJSON *item_copy = cJSON_Duplicate(first_item, fuzz_int32_t_6);
            if (item_copy) {
                // Add the copied item to our root object
                cJSON_AddItemToObject(root_object, fuzz_str_7, item_copy);
            }
        }
        
        // Try to get array from parsed JSON and modify it
        cJSON *ids_array = cJSON_GetObjectItem(parsed_json, fuzz_str_8);
        if (cJSON_IsArray(ids_array) && cJSON_GetArraySize(ids_array) > 0) {
            // Replace first item in array
            cJSON *new_item = cJSON_CreateNumber(999);
            if (new_item) {
                cJSON_ReplaceItemInArray(ids_array, fuzz_int32_t_9, new_item);
            }
            
            // Detach an item from array
            if (cJSON_GetArraySize(ids_array) > 1) {
                cJSON *detached = cJSON_DetachItemFromArray(ids_array, fuzz_int32_t_10);
                if (detached) {
                    cJSON_Delete(detached);
                }
            }
        }
    }
    
    // 8. Test PrintPreallocated with different buffer sizes
    int buffer_size = 1024;
    char *print_buffer = (char *)malloc(buffer_size);
    if (print_buffer) {
        // Try with formatted output
        cJSON_bool print_result = cJSON_PrintPreallocated(root_object, print_buffer, buffer_size, fuzz_int32_t_11);
        
        // Try with unformatted output using a different buffer
        char *small_buffer = (char *)malloc(10);
        if (small_buffer) {
            // This should fail due to small buffer, testing error handling
            cJSON_PrintPreallocated(root_object, small_buffer, 10, fuzz_int32_t_12);
            free(small_buffer);
        }
        
        free(print_buffer);
    }
    
    // 9. Create a file and write JSON to it
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        char *printed = cJSON_Print(root_object);
        if (printed) {
            fwrite(printed, 1, strlen(printed), out_file);
            cJSON_free(printed);
        }
        assert_file_closed(&out_file);;
    }
    
    // 10. Read from a file-like memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        (void)fd; // Use fd to avoid unused variable warning
        
        // Read and parse from the memory file
        fseek(in_file, 0, SEEK_SET);
        char *file_buffer = (char *)malloc(size + 1);
        if (file_buffer) {
            size_t read_size = fread(file_buffer, 1, size, in_file);
            file_buffer[read_size] = '\0';
            
            // Parse the file content
            cJSON *file_parsed = cJSON_Parse(file_buffer);
            if (file_parsed) {
                cJSON_Delete(file_parsed);
            }
            free(file_buffer);
        }
        assert_file_closed(&in_file);;
    }
    
    // 11. Clean up - delete all cJSON objects
    cJSON_Delete(parsed_json);
    cJSON_Delete(root_object);
    free(json_str);
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
}