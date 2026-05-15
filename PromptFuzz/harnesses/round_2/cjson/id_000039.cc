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
//<ID> 329
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 1
//<Quality> {"density":15,"unique_branches":{"case_insensitive_strcmp":[[140,9,140,27,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_Delete","cJSON_AddStringToObject","cJSON_CreateNumber","cJSON_AddItemToObjectCS","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_AddItemReferenceToObject","cJSON_IsArray","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_AddItemToArray","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_PrintUnformatted","cJSON_free","cJSON_PrintPreallocated","cJSON_Minify","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_CreateNumber","cJSON_AddItemToObjectCS","cJSON_GetArraySize","cJSON_IsArray","cJSON_GetArrayItem","cJSON_AddItemReferenceToObject","cJSON_GetArraySize","cJSON_IsArray","cJSON_DetachItemFromArray","cJSON_AddItemToArray","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_PrintUnformatted","cJSON_free","cJSON_PrintPreallocated","cJSON_Minify","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure we have enough data for basic operations
    if (size == 0) {
        return 0;
    }
    
    // Create a null-terminated string from input data for parsing
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
	return 0;
	}
	
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    // Parse JSON using cJSON_ParseWithLength (uses size information)
    cJSON *root = cJSON_ParseWithLength(json_str, size);
    if (!root) {
        free(json_str);
        return 0;
    }
    
    // Create a new object for various operations
    cJSON *new_obj = cJSON_CreateObject();
    if (!new_obj) {
        cJSON_Delete(root);
        free(json_str);
        return 0;
    }
    
    // 1. Use cJSON_AddStringToObject
    cJSON *str_item = cJSON_AddStringToObject(new_obj, "fuzzed_string", "test_value");
    
    // 2. Use cJSON_AddItemToObjectCS (case-sensitive)
    // Create a number item first
    cJSON *num_item = cJSON_CreateNumber(42.5);
    if (num_item) {
        cJSON_AddItemToObjectCS(new_obj, "CaseSensitiveKey", num_item);
    }
    
    // 3. Use cJSON_AddItemReferenceToObject
    // Add a reference to the string item from root (if root is an array/object)
    if (cJSON_IsArray(root) && cJSON_GetArraySize(root) > 0) {
        cJSON *array_item = cJSON_GetArrayItem(root, 0);
        if (array_item) {
            cJSON_AddItemReferenceToObject(new_obj, "referenced_item", array_item);
        }
    }
    
    // 4. Use cJSON_DetachItemFromArray (if root is an array)
    cJSON *detached_item = nullptr;
    if (cJSON_IsArray(root) && cJSON_GetArraySize(root) > 1) {
        detached_item = cJSON_DetachItemFromArray(root, 1);
    }
    
    // 5. Use cJSON_DetachItemViaPointer
    if (detached_item) {
        // Re-attach it first to test DetachItemViaPointer
        cJSON_AddItemToArray(root, detached_item);
        // Now detach using pointer
        cJSON_DetachItemViaPointer(root, detached_item);
        // Clean up the detached item
        cJSON_Delete(detached_item);
    }
    
    // 6. Use cJSON_SetNumberHelper
    // Find a number item in new_obj and modify it
    cJSON *number_in_obj = cJSON_GetObjectItem(new_obj, "CaseSensitiveKey");
    if (cJSON_IsNumber(number_in_obj)) {
        cJSON_SetNumberHelper(number_in_obj, 99.9);
    }
    
    // 7. Use cJSON_PrintPreallocated
    // First get the length needed
    char *unformatted = cJSON_PrintUnformatted(new_obj);
    int needed_len = unformatted ? strlen(unformatted) + 1 : 256;
    cJSON_free(unformatted);
    
    // Allocate buffer and print preallocated
    char *buffer = (char *)malloc(needed_len);
    if (buffer) {
        cJSON_bool print_ok = cJSON_PrintPreallocated(new_obj, buffer, needed_len, 0);
        if (print_ok) {
            // 8. Use cJSON_Minify on the printed buffer
            cJSON_Minify(buffer);
        }
        free(buffer);
    }
    
    // Write output to a file (optional, for demonstration)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        char *output = cJSON_Print(root);
        if (output) {
            fwrite(output, 1, strlen(output), out_file);
            cJSON_free(output);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up
    cJSON_Delete(root);
    cJSON_Delete(new_obj);
    free(json_str);
    
    assert_file_closed(&out_file);
	return 0;
}