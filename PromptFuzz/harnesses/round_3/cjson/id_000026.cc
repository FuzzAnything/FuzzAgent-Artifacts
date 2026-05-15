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
//<ID> 134
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 1
//<Quality> {"density":22,"unique_branches":{"replace_item_in_object":[[2435,55,2435,84,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateRaw","cJSON_IsRaw","cJSON_CreateObject","cJSON_AddItemToObject","cJSON_Duplicate","cJSON_CreateBool","cJSON_CreateNumber","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_DetachItemFromArray","cJSON_AddItemReferenceToObject","cJSON_CreateString","cJSON_AddItemToObject","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_CreateObject","cJSON_CreateRaw","cJSON_IsRaw","cJSON_Duplicate","cJSON_CreateBool","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_AddItemReferenceToObject","cJSON_free","cJSON_CreateString","cJSON_CreateString","cJSON_AddItemToObject","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateRaw","cJSON_IsRaw","cJSON_CreateObject","cJSON_AddItemToObject","cJSON_Duplicate","cJSON_CreateBool","cJSON_CreateNumber","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_DetachItemFromArray","cJSON_AddItemReferenceToObject","cJSON_CreateString","cJSON_AddItemToObject","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_CreateObject","cJSON_CreateRaw","cJSON_IsRaw","cJSON_Duplicate","cJSON_CreateBool","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_AddItemReferenceToObject","cJSON_free","cJSON_CreateString","cJSON_CreateString","cJSON_AddItemToObject","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_Delete","cJSON_Delete"],"visited":0}
/*Looking at the input data, I can see it's HTML content, not JSON. However, I'll create a fuzzer that tests the specified cJSON APIs by using the input data in various ways. Here's a comprehensive fuzzer that uses all the required APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<177) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_15, fuzz_str_sz_15, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_16, fuzz_str_sz_16, fdp);
	//fuzzer shim end}




    // Early return if input is too small
    if (size < 10) {
	return 0;
	}
	
    
    // Create a null-terminated string from the input data
    char *input_str = (char *)malloc(size + 1);
    if (!input_str) {
	return 0;
	}
	
    
    memcpy(input_str, data, size);
    input_str[size] = '\0';
    
    // 1. First, try to parse the input as JSON
    cJSON *parsed_json = cJSON_ParseWithLength(input_str, size);
    
    // 2. Create a raw JSON item from part of the input
    // Use first 1/3 of data for raw JSON
    size_t raw_size = size / 3;
    if (raw_size > 0) {
        char *raw_data = (char *)malloc(raw_size + 1);
        if (raw_data) {
            memcpy(raw_data, data, raw_size);
            raw_data[raw_size] = '\0';
            cJSON *raw_item = cJSON_CreateRaw(raw_data);
            free(raw_data);
            
            // 3. Check if the created item is raw
            if (raw_item && cJSON_IsRaw(raw_item)) {
                // Create a parent object for testing
                cJSON *parent_object = cJSON_CreateObject();
                if (parent_object) {
                    // Add the raw item to the object
                    cJSON_AddItemToObject(parent_object, fuzz_str_1, raw_item);
                    
                    // 4. Create a duplicate of the raw item
                    cJSON *duplicated_raw = cJSON_Duplicate(raw_item, fuzz_int32_t_2);
                    
                    if (duplicated_raw) {
                        // 5. Create a boolean item
                        cJSON *bool_item = cJSON_CreateBool(fuzz_int32_t_3);
                        
                        // 6. Create a number item
                        cJSON *number_item = cJSON_CreateNumber(42.5);
                        
                        // 7. Create an array to test detach operation
                        cJSON *test_array = cJSON_CreateArray();
                        if (test_array) {
                            // Add some items to the array
                            cJSON_AddItemToArray(test_array, cJSON_CreateNumber(1.0));
                            cJSON_AddItemToArray(test_array, cJSON_CreateNumber(2.0));
                            cJSON_AddItemToArray(test_array, cJSON_CreateNumber(3.0));
                            
                            // 8. Detach an item from the array
                            cJSON *detached_item = cJSON_DetachItemFromArray(test_array, 1);
                            
                            if (detached_item) {
                                // 9. Add item reference to object
                                cJSON_AddItemReferenceToObject(parent_object, 
                                                              fuzz_str_4, 
                                                              detached_item);
                                
                                // 10. Replace item in object (case sensitive)
                                // First add a temporary item
                                cJSON *temp_item = cJSON_CreateString(fuzz_str_5);
                                if (temp_item) {
                                    cJSON_AddItemToObject(parent_object, 
                                                         "replace_me", 
                                                         temp_item);
                                    
                                    // Now replace it with the duplicated raw item
                                    cJSON_ReplaceItemInObjectCaseSensitive(parent_object,
                                                                          "replace_me",
                                                                          duplicated_raw);
                                }
                                
                                // Clean up detached_item (now owned by parent_object via reference)
                                cJSON_free(detached_item);
                            }
                            
                            // Clean up the array
                            cJSON_Delete(test_array);
                        }
                        
                        // Clean up temporary items
                        if (bool_item) cJSON_Delete(bool_item);
                        if (number_item) cJSON_Delete(number_item);
                    }
                    
                    // Print the resulting JSON for debugging (commented out for fuzzing)
                    // char *json_str = cJSON_Print(parent_object);
                    // if (json_str) {
                    //     free(json_str);
                    // }
                    
                    // Clean up parent object
                    cJSON_Delete(parent_object);
                }
            }
        }
    }
    
    // Alternative approach: Create an object from the parsed JSON if valid
    if (parsed_json) {
        // Create a new object for manipulation
        cJSON *test_object = cJSON_CreateObject();
        if (test_object) {
            // 1. Create a raw item from middle section of input
            size_t mid_start = size / 4;
            size_t mid_size = size / 2;
            if (mid_size > 0 && mid_start + mid_size <= size) {
                char *mid_data = (char *)malloc(mid_size + 1);
                if (mid_data) {
                    memcpy(mid_data, data + mid_start, mid_size);
                    mid_data[mid_size] = '\0';
                    
                    cJSON *mid_raw = cJSON_CreateRaw(mid_data);
                    free(mid_data);
                    
                    if (mid_raw && cJSON_IsRaw(mid_raw)) {
                        // 2. Duplicate the raw item
                        cJSON *dup_raw = cJSON_Duplicate(mid_raw, fuzz_int32_t_6);
                        
                        // 3. Create boolean and number items
                        cJSON *bool_item2 = cJSON_CreateBool(fuzz_int32_t_7);
                        cJSON *num_item2 = cJSON_CreateNumber(3.14159);
                        
                        // Add items to test object
                        cJSON_AddItemToObject(test_object, fuzz_str_8, mid_raw);
                        
                        if (dup_raw) {
                            cJSON_AddItemToObject(test_object, fuzz_str_9, dup_raw);
                        }
                        
                        if (bool_item2) {
                            cJSON_AddItemToObject(test_object, fuzz_str_10, bool_item2);
                        }
                        
                        if (num_item2) {
                            cJSON_AddItemToObject(test_object, fuzz_str_11, num_item2);
                        }
                        
                        // 4. Create an array and test detach
                        cJSON *array2 = cJSON_CreateArray();
                        if (array2) {
                            for (int i = 0; i < 5 && i * 10 < (int)size; i++) {
                                cJSON_AddItemToArray(array2, 
                                                    cJSON_CreateNumber(i * 10.0));
                            }
                            
                            // Add array to object
                            cJSON_AddItemToObject(test_object, fuzz_str_12, array2);
                            
                            // Detach an item from array
                            if (cJSON_GetArraySize(array2) > 2) {
                                cJSON *detached2 = cJSON_DetachItemFromArray(array2, fuzz_int32_t_13);
                                
                                if (detached2) {
                                    // Add as reference
                                    cJSON_AddItemReferenceToObject(test_object,
                                                                  fuzz_str_14,
                                                                  detached2);
                                    
                                    cJSON_free(detached2);
                                }
                            }
                        }
                        
                        // 5. Replace an item in the object
                        // Create a replacement item
                        cJSON *replacement = cJSON_CreateString(fuzz_str_15);
                        if (replacement) {
                            // First add an item to replace
                            cJSON *to_replace = cJSON_CreateString(fuzz_str_16);
                            if (to_replace) {
                                cJSON_AddItemToObject(test_object, 
                                                     "replace_target", 
                                                     to_replace);
                                
                                // Now replace it
                                cJSON_ReplaceItemInObjectCaseSensitive(test_object,
                                                                      "replace_target",
                                                                      replacement);
                            } else {
                                cJSON_Delete(replacement);
                            }
                        }
                        
                        // Clean up test object
                        cJSON_Delete(test_object);
                    } else {
                        if (mid_raw) cJSON_Delete(mid_raw);
                        cJSON_Delete(test_object);
                    }
                }
            } else {
                cJSON_Delete(test_object);
            }
        }
        
        // Clean up parsed JSON
        cJSON_Delete(parsed_json);
    }
    
    // Clean up
    free(input_str);
    
    return 0;
}