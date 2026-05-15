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
//<ID> 78
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 2
//<Quality> {"density":27,"unique_branches":{"cJSON_SetValuestring":[[457,16,457,58,0,0,4,1],[457,62,457,104,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_IsObject","cJSON_Delete","cJSON_DetachItemFromObject","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateTrue","cJSON_IsTrue","cJSON_GetStringValue","cJSON_SetValuestring","cJSON_Delete","cJSON_DeleteItemFromObject","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_CreateBool","cJSON_AddItemToObject","cJSON_IsTrue","cJSON_GetObjectItem","cJSON_SetValuestring","cJSON_DetachItemFromObject","cJSON_IsTrue","cJSON_Delete","cJSON_CreateString","cJSON_AddItemToObject","cJSON_DeleteItemFromObject","cJSON_Print","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_IsObject","cJSON_DetachItemFromObject","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateTrue","cJSON_IsTrue","cJSON_GetStringValue","cJSON_SetValuestring","cJSON_Delete","cJSON_DeleteItemFromObject","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_CreateBool","cJSON_AddItemToObject","cJSON_IsTrue","cJSON_GetObjectItem","cJSON_SetValuestring","cJSON_DetachItemFromObject","cJSON_IsTrue","cJSON_Delete","cJSON_CreateString","cJSON_AddItemToObject","cJSON_DeleteItemFromObject","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<114) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
	//fuzzer shim end}




    // Parse input JSON data
    cJSON *root = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (root == nullptr) {
        return 0; // Invalid JSON input
    }
    
    // Ensure root is an object for our operations
    if (!cJSON_IsObject(root)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Step 1: Detach an item from the object
    // We'll try to detach the "menu" item if it exists
    cJSON *detached_item = cJSON_DetachItemFromObject(root, fuzz_str_1);
    if (detached_item != nullptr) {
        // Step 2: Check if the detached item has a "header" that could represent a boolean value
        // First, check if it's an object
        if (cJSON_IsObject(detached_item)) {
            cJSON *header = cJSON_GetObjectItem(detached_item, fuzz_str_2);
            if (header != nullptr && cJSON_IsString(header)) {
                // Create a test boolean value to check
                cJSON *test_bool = cJSON_CreateTrue();
                cJSON_bool is_true = cJSON_IsTrue(test_bool);
                
                // Step 3: Set a new value string for the header
                // We'll modify the header value
                char *old_value = cJSON_GetStringValue(header);
                if (old_value != nullptr) {
                    // Create a modified value
                    char new_value[256];
                    snprintf(new_value, sizeof(new_value), "%s_modified_%d", 
                            old_value, is_true ? 1 : 0);
                    char *result = cJSON_SetValuestring(header, new_value);
                    // result contains the old value, but we don't need it
                }
                
                cJSON_Delete(test_bool);
            }
        }
        
        // Step 4: Delete an item from the detached object
        // Try to delete an "items" array if it exists
        cJSON_DeleteItemFromObject(detached_item, fuzz_str_3);
        
        // Step 5: Print the modified detached item
        char *printed_json = cJSON_Print(detached_item);
        if (printed_json != nullptr) {
            // We could write this to a file if needed
            cJSON_free(printed_json);
        }
        
        // Clean up the detached item
        cJSON_Delete(detached_item);
    }
    
    // Step 6: Now perform similar operations on the root object
    // Add a boolean item to the root
    cJSON *bool_item = cJSON_CreateBool(fuzz_int32_t_4);
    cJSON_AddItemToObject(root, fuzz_str_5, bool_item);
    
    // Check if it's true
    cJSON_bool is_true = cJSON_IsTrue(bool_item);
    
    // Get the boolean item back and set its value string representation
    cJSON *retrieved_bool = cJSON_GetObjectItem(root, fuzz_str_6);
    if (retrieved_bool != nullptr) {
        // Set value string for the boolean (though typically booleans don't have value strings)
        char bool_str[32];
        snprintf(bool_str, sizeof(bool_str), "is_true_%d", is_true);
        cJSON_SetValuestring(retrieved_bool, bool_str);
        
        // Detach this boolean item
        cJSON *detached_bool = cJSON_DetachItemFromObject(root, fuzz_str_7);
        if (detached_bool != nullptr) {
            // Check if it's still true after detachment
            cJSON_bool still_true = cJSON_IsTrue(detached_bool);
            
            // Delete it from our temporary reference
            cJSON_Delete(detached_bool);
        }
    }
    
    // Create and add a string item to delete
    cJSON *string_item = cJSON_CreateString(fuzz_str_8);
    cJSON_AddItemToObject(root, fuzz_str_9, string_item);
    
    // Delete the string item
    cJSON_DeleteItemFromObject(root, fuzz_str_10);
    
    // Final print of the modified root
    char *final_output = cJSON_Print(root);
    if (final_output != nullptr) {
        // Optionally write to file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file != nullptr) {
            fwrite(final_output, 1, strlen(final_output), out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(final_output);
    }
    
    // Clean up
    cJSON_Delete(root);
    
    return 0;
}