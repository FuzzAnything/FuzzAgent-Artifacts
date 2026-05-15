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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 3
//<Quality> {"density":30,"unique_branches":{"cJSON_InsertItemInArray":[[2344,9,2344,31,0,0,4,1],[2349,9,2349,39,0,0,4,1],[2357,9,2357,39,0,0,4,0]]},"library_calls":["cJSON_Version","cJSON_Parse","cJSON_ParseWithLength","cJSON_CreateObject","cJSON_IsObject","cJSON_GetObjectItem","cJSON_AddNullToObject","cJSON_AddArrayToObject","cJSON_IsArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_CreateRaw","cJSON_AddItemToArray","cJSON_GetArraySize","cJSON_ReplaceItemInArray","cJSON_CreateNumber","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_InsertItemInArray","cJSON_CreateString","cJSON_AddRawToObject","cJSON_Duplicate","cJSON_CreateArrayReference","cJSON_AddItemToObject","cJSON_Print","cJSON_malloc","cJSON_PrintPreallocated","cJSON_free","cJSON_Print","cJSON_free","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Version","cJSON_Parse","cJSON_ParseWithLength","cJSON_CreateObject","cJSON_IsObject","cJSON_GetObjectItem","cJSON_AddNullToObject","cJSON_AddArrayToObject","cJSON_IsArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_CreateRaw","cJSON_AddItemToArray","cJSON_GetArraySize","cJSON_ReplaceItemInArray","cJSON_CreateNumber","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_InsertItemInArray","cJSON_CreateString","cJSON_AddRawToObject","cJSON_Duplicate","cJSON_CreateArrayReference","cJSON_AddItemToObject","cJSON_Print","cJSON_malloc","cJSON_PrintPreallocated","cJSON_free","cJSON_Print","cJSON_free","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


// Include cJSON header

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<135) return 0;

	
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
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
	//fuzzer shim end}




    // Event: Parse JSON input, create/modify a JSON structure,
    // duplicate it, and serialize it back
  
    // 1. Get cJSON version
    const char *version = cJSON_Version();
    (void)version; // Use version to avoid unused variable warning
  
    // 2. Parse input data as JSON string (both with and without length)
    cJSON *parsed_json = NULL;
    if (size > 0 && data[size-1] == '\0') {
        // Input is null-terminated
        parsed_json = cJSON_Parse((const char*)data);
    }
    
    // Also try parsing with length
    cJSON *parsed_with_length = cJSON_ParseWithLength((const char*)data, size);
    
    // Use whichever parsing succeeded
    cJSON *root = parsed_with_length ? parsed_with_length : parsed_json;
    
    // 3. Create a new JSON object if parsing failed
    if (!root) {
        root = cJSON_CreateObject();
    }
    
    // 4. Check if root is an object
    if (cJSON_IsObject(root)) {
        // 5. Get an object item (if it exists)
        cJSON *widget_item = cJSON_GetObjectItem(root, fuzz_str_1);
        
        // 6. Add a null item to object
        cJSON_AddNullToObject(root, fuzz_str_2);
        
        // 7. Create a new array and add to object
        cJSON *new_array = cJSON_AddArrayToObject(root, fuzz_str_3);
        
        if (new_array && cJSON_IsArray(new_array)) {
            // 8. Add items to array
            cJSON_AddItemToArray(new_array, cJSON_CreateNumber(42.5));
            cJSON_AddItemToArray(new_array, cJSON_CreateString(fuzz_str_4));
            
            // 9. Create raw JSON and add to array
            cJSON *raw_json = cJSON_CreateRaw(fuzz_str_5);
            if (raw_json) {
                cJSON_AddItemToArray(new_array, raw_json);
            }
            
            // 10. Get array size
            int array_size = cJSON_GetArraySize(new_array);
            
            // 11. Modify array if it has items
            if (array_size > 0) {
                // Replace first item
                if (array_size > 0) {
                    cJSON_ReplaceItemInArray(new_array, 0, cJSON_CreateNumber(99.9));
                }
                
                // Detach middle item if exists
                if (array_size > 1) {
                    cJSON *detached = cJSON_DetachItemFromArray(new_array, fuzz_int32_t_6);
                    if (detached) {
                        cJSON_Delete(detached);
                    }
                }
                
                // Insert new item at beginning
                cJSON_InsertItemInArray(new_array, 0, cJSON_CreateString(fuzz_str_7));
            }
        }
        
        // 12. Add raw data to object
        cJSON_AddRawToObject(root, fuzz_str_8, fuzz_str_9);
        
        // 13. Duplicate the entire structure
        cJSON *duplicated = cJSON_Duplicate(root, fuzz_int32_t_10);
        
        // 14. Create array reference
        cJSON *array_ref = cJSON_CreateArrayReference(new_array);
        if (array_ref) {
            cJSON_AddItemToObject(root, fuzz_str_11, array_ref);
        }
        
        // 15. Serialize JSON using both print methods
        char *printed_json = cJSON_Print(root);
        if (printed_json) {
            // 16. Also try preallocated printing
            int buffer_size = strlen(printed_json) + 100;
            char *buffer = (char*)cJSON_malloc(buffer_size);
            if (buffer) {
                cJSON_PrintPreallocated(root, buffer, buffer_size, fuzz_int32_t_12);
                cJSON_free(buffer);
            }
            
            // 17. Print duplicated version too
            char *duplicated_json = cJSON_Print(duplicated);
            if (duplicated_json) {
                cJSON_free(duplicated_json);
            }
            
            cJSON_free(printed_json);
        }
        
        // 18. Clean up duplicated JSON
        if (duplicated) {
            cJSON_Delete(duplicated);
        }
    }
    
    // 19. Clean up
    if (parsed_json && parsed_json != root) {
        cJSON_Delete(parsed_json);
    }
    if (parsed_with_length && parsed_with_length != root) {
        cJSON_Delete(parsed_with_length);
    }
    if (root) {
        cJSON_Delete(root);
    }
    
    // 20. Demonstrate file operations as requested
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        (void)fd; // Use fd to avoid unused variable warning
        
        // Read a byte from the input
        int ch = fgetc(in_file);
        (void)ch; // Use ch to avoid unused variable warning
        
        assert_file_closed(&in_file);;
    }
    
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int fd = fuzz_fileno(out_file);
        (void)fd; // Use fd to avoid unused variable warning
        
        // Write something minimal
        fputc('X', out_file);
        assert_file_closed(&out_file);;
        
        // Clean up output file
        remove("output_file");
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}