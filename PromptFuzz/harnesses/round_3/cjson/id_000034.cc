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
//<ID> 250
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"case_insensitive_strcmp":[[140,9,140,27,0,0,4,0]]},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_AddObjectToObject","cJSON_Delete","cJSON_AddStringToObject","cJSON_CreateBool","cJSON_AddItemToObject","cJSON_IsArray","cJSON_GetArraySize","cJSON_AddStringToObject","cJSON_IsBool","cJSON_IsFalse","cJSON_DeleteItemFromObject","cJSON_PrintUnformatted","cJSON_free","cJSON_PrintUnformatted","cJSON_free","cJSON_PrintPreallocated","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_AddObjectToObject","cJSON_AddStringToObject","cJSON_CreateBool","cJSON_AddItemToObject","cJSON_IsArray","cJSON_GetArraySize","cJSON_AddStringToObject","cJSON_IsBool","cJSON_IsFalse","cJSON_DeleteItemFromObject","cJSON_PrintUnformatted","cJSON_free","cJSON_PrintUnformatted","cJSON_free","cJSON_PrintPreallocated","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<74) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
	//fuzzer shim end}




    // Step 1: Parse input data as JSON
    const char *parse_end = nullptr;
    cJSON *root = cJSON_ParseWithLengthOpts(reinterpret_cast<const char*>(data), size, 
                                           &parse_end, false);
    if (!root) {
        return 0; // Failed to parse, exit early
    }
    
    // Step 2: Create a new object to work with
    cJSON *new_object = cJSON_AddObjectToObject(root, fuzz_str_1);
    if (!new_object) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Step 3: Add various elements to the new object
    cJSON_AddStringToObject(new_object, fuzz_str_2, fuzz_str_3);
    
    // Create and add a boolean value
    cJSON *bool_item = cJSON_CreateBool(fuzz_int32_t_4); // false
    if (bool_item) {
        cJSON_AddItemToObject(new_object, fuzz_str_5, bool_item);
    }
    
    // Step 4: Check array size if root has arrays
    if (cJSON_IsArray(root)) {
        int array_size = cJSON_GetArraySize(root);
        // Use array_size in some way (e.g., add as string)
        char size_str[20];
        snprintf(size_str, sizeof(size_str), "%d", array_size);
        cJSON_AddStringToObject(new_object, fuzz_str_6, size_str);
    }
    
    // Step 5: Check boolean values in the object
    cJSON *item = new_object->child;
    while (item) {
        if (cJSON_IsBool(item)) {
            if (cJSON_IsFalse(item)) {
                // Delete this false boolean item
                cJSON_DeleteItemFromObject(new_object, item->string);
                break; // Delete one and exit loop
            }
        }
        item = item->next;
    }
    
    // Step 6: Print unformatted JSON
    char *unformatted = cJSON_PrintUnformatted(root);
    if (unformatted) {
        // Could write to file or use in some way
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(unformatted, strlen(unformatted), 1, out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(unformatted);
    }
    
    // Step 7: Use PrintPreallocated
    // First get required length by printing
    char *temp_print = cJSON_PrintUnformatted(root);
    int required_len = temp_print ? strlen(temp_print) + 1 : 1024;
    if (temp_print) cJSON_free(temp_print);
    
    // Allocate buffer and print
    char *buffer = (char*)malloc(required_len);
    if (buffer) {
        cJSON_bool success = cJSON_PrintPreallocated(root, buffer, required_len, false);
        if (success) {
            // Could write buffer to file or use it
            FILE *out_file2 = fopen("output_file2", "wb");
            if (out_file2) {
                fwrite(buffer, strlen(buffer), 1, out_file2);
                assert_file_closed(&out_file2);;
            }
        }
        free(buffer);
    }
    
    // Step 8: Clean up
    cJSON_Delete(root);
    
    return 0;
}