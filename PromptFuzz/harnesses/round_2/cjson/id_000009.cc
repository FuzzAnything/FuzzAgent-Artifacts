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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 5.5, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_GetStringValue","cJSON_CreateStringArray","cJSON_CreateBool","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_AddItemReferenceToArray","cJSON_PrintPreallocated","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsBool","cJSON_CreateBool","cJSON_IsTrue","cJSON_Delete","cJSON_CreateStringArray","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_CreateBool","cJSON_AddItemReferenceToArray","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_GetStringValue","cJSON_CreateStringArray","cJSON_CreateBool","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_AddItemReferenceToArray","cJSON_PrintPreallocated","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsBool","cJSON_CreateBool","cJSON_IsTrue","cJSON_Delete","cJSON_CreateStringArray","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_CreateBool","cJSON_AddItemReferenceToArray","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":1}
/*Here's a fuzz driver that uses the specified cJSON APIs to achieve the event described:

*/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small
    if (size < 1) {
	return 0;
	}
	
    
    // Create a null-terminated string from input data
    char *input_str = (char*)malloc(size + 1);
    if (!input_str) {
	return 0;
	}
	
    
    memcpy(input_str, data, size);
    input_str[size] = '\0';
    
    // Parse input as JSON
    cJSON *root = cJSON_Parse(input_str);
    free(input_str);
    
    if (!root) {
	return 0;
	}
	
    
    // Extract the "name" field
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    if (name_item && cJSON_IsString(name_item)) {
        // Create a string array with the name
        const char *strings[] = {cJSON_GetStringValue(name_item)};
        cJSON *string_array = cJSON_CreateStringArray(strings, 1);
        
        if (string_array) {
            // Create a boolean value based on size (for demonstration)
            cJSON_bool bool_val = (size > 100) ? 1 : 0;
            cJSON *bool_item = cJSON_CreateBool(bool_val);
            
            if (bool_item) {
                // Detach first (and only) item from string array
                cJSON *detached = cJSON_DetachItemFromArray(string_array, 0);
                if (detached) {
                    cJSON_Delete(detached);
                }
                
                // Add reference of boolean item to array
                cJSON_AddItemReferenceToArray(string_array, bool_item);
                
                // Prepare buffer for printing
                int buffer_size = 1024;
                char *buffer = (char*)malloc(buffer_size);
                if (buffer) {
                    // Print the array to preallocated buffer
                    cJSON_PrintPreallocated(string_array, buffer, buffer_size - 1, 1);
                    
                    // Try to print the boolean item separately
                    char bool_buffer[256];
                    cJSON_PrintPreallocated(bool_item, bool_buffer, 255, 0);
                    
                    free(buffer);
                }
                
                // Clean up boolean item
                cJSON_Delete(bool_item);
            }
            
            // Clean up string array
            cJSON_Delete(string_array);
        }
    }
    
    // Additional operations with the parsed JSON
    cJSON *format = cJSON_GetObjectItemCaseSensitive(root, "format");
    if (format && cJSON_IsObject(format)) {
        // Create boolean for "interlace" field
        cJSON *interlace = cJSON_GetObjectItemCaseSensitive(format, "interlace");
        if (interlace && cJSON_IsBool(interlace)) {
            cJSON *new_bool = cJSON_CreateBool(cJSON_IsTrue(interlace));
            if (new_bool) {
                cJSON_Delete(new_bool);
            }
        }
        
        // Create string array with format keys
        cJSON *child = format->child;
        const char *keys[10];
        int key_count = 0;
        
        while (child && key_count < 10) {
            keys[key_count++] = child->string;
            child = child->next;
        }
        
        if (key_count > 0) {
            cJSON *key_array = cJSON_CreateStringArray(keys, key_count);
            if (key_array) {
                // Perform array operations
                if (key_count > 1) {
                    cJSON *detached_key = cJSON_DetachItemFromArray(key_array, 0);
                    if (detached_key) {
                        cJSON_Delete(detached_key);
                    }
                }
                
                // Create another boolean and add reference
                cJSON *format_bool = cJSON_CreateBool(0);
                if (format_bool) {
                    cJSON_AddItemReferenceToArray(key_array, format_bool);
                    
                    // Print to buffer
                    char small_buffer[512];
                    cJSON_PrintPreallocated(key_array, small_buffer, 511, 1);
                    
                    cJSON_Delete(format_bool);
                }
                
                cJSON_Delete(key_array);
            }
        }
    }
    
    // Clean up root JSON
    cJSON_Delete(root);
    
    return 0;
}