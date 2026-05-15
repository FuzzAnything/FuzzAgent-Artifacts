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
//<ID> 85
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 1
//<Quality> {"density":24,"unique_branches":{"cJSON_GetNumberValue":[[111,9,111,30,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_AddTrueToObject","cJSON_AddNumberToObject","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_CreateBool","cJSON_AddItemReferenceToObject","cJSON_CreateStringReference","cJSON_AddItemToObject","cJSON_IsArray","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetNumberValue","cJSON_PrintBuffered","cJSON_Delete","cJSON_Delete","cJSON_IsArray","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_CreateObject","cJSON_AddTrueToObject","cJSON_GetArrayItem","cJSON_IsArray","cJSON_GetArrayItem","cJSON_GetNumberValue","cJSON_AddNumberToObject","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_CreateBool","cJSON_AddItemReferenceToObject","cJSON_CreateStringReference","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetNumberValue","cJSON_AddItemToObject","cJSON_PrintBuffered","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_IsArray","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_CreateObject","cJSON_AddTrueToObject","cJSON_GetArrayItem","cJSON_IsArray","cJSON_GetArrayItem","cJSON_GetNumberValue","cJSON_AddNumberToObject","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_CreateBool","cJSON_AddItemReferenceToObject","cJSON_CreateStringReference","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetNumberValue","cJSON_AddItemToObject","cJSON_PrintBuffered","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


// Include cJSON header

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<302) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_3, fuzz_int32_t_array_size_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_17, fuzz_str_sz_17, fdp);
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_18, fuzz_int32_t_array_size_18, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_19, fuzz_str_sz_19, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_20, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_21, fuzz_str_sz_21, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_22, fuzz_str_sz_22, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_23, fuzz_str_sz_23, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_24, fuzz_str_sz_24, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_25, fuzz_str_sz_25, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_26, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_27, fdp);
	
		const int * fuzzer_var_cJSON_CreateIntArray_4_0 = fuzz_int32_t_array_3;
		int fuzzer_size_cJSON_CreateIntArray_4_0 = static_cast<int>(fuzz_int32_t_array_size_3);
		const int * fuzzer_var_cJSON_CreateIntArray_29_0 = fuzz_int32_t_array_18;
		int fuzzer_size_cJSON_CreateIntArray_29_0 = static_cast<int>(fuzz_int32_t_array_size_18);//fuzzer shim end}




    // 1. Create null-terminated string from input data for parsing
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
	return 0;
	}
	
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    // 2. Parse JSON using cJSON_Parse
    cJSON *parsed = cJSON_Parse(json_str);
    free(json_str);
    
    if (!parsed) {
        // If parsing fails, still test other APIs with created objects
        // Create a new object to test other APIs
        cJSON *root = cJSON_CreateObject();
        
        // Add various items to object
        cJSON_AddTrueToObject(root, fuzz_str_1);
        cJSON_AddNumberToObject(root, fuzz_str_2, 42.5);
        
        // Create and add integer array
        int int_array[] = {1, 2, 3, 4, 5};
        cJSON *int_array_obj = cJSON_CreateIntArray(fuzzer_var_cJSON_CreateIntArray_4_0, fuzzer_size_cJSON_CreateIntArray_4_0);
        cJSON_AddItemToObject(root, fuzz_str_4, int_array_obj);
        
        // Create boolean and add as reference
        cJSON *bool_ref = cJSON_CreateBool(fuzz_int32_t_5);
        cJSON_AddItemReferenceToObject(root, fuzz_str_6, bool_ref);
        
        // Create string reference
        cJSON *string_ref = cJSON_CreateStringReference(fuzz_str_7);
        cJSON_AddItemToObject(root, fuzz_str_8, string_ref);
        
        // Test array operations on the integer array
        if (cJSON_IsArray(int_array_obj)) {
            int array_size = cJSON_GetArraySize(int_array_obj);
            if (array_size > 0) {
                cJSON_DeleteItemFromArray(int_array_obj, fuzz_int32_t_9);
            }
        }
        
        // Get object item and check if it's a string
        cJSON *string_item = cJSON_GetObjectItem(root, fuzz_str_10);
        if (string_item && cJSON_IsString(string_item)) {
            // Get number value (will return 0 for non-numbers)
            double num_val = cJSON_GetNumberValue(string_item);
            (void)num_val; // Use variable to avoid warning
        }
        
        // Print the object
        char *printed = cJSON_PrintBuffered(root, fuzz_int32_t_11, fuzz_int32_t_12);
        if (printed) {
            free(printed);
        }
        
        // Cleanup
        cJSON_Delete(root);
        cJSON_Delete(bool_ref);
        // string_ref is owned by root, don't delete separately
        
        return 0;
    }
    
    // 3. If parsing succeeded, work with the parsed JSON
    // Check if parsed item is an array
    if (cJSON_IsArray(parsed)) {
        int array_size = cJSON_GetArraySize(parsed);
        
        // Delete first item if array has items
        if (array_size > 0) {
            cJSON_DeleteItemFromArray(parsed, fuzz_int32_t_13);
        }
    }
    
    // 4. Create a new object for testing other APIs
    cJSON *new_obj = cJSON_CreateObject();
    
    // 5. Add various items to the new object
    // Add true value
    cJSON_AddTrueToObject(new_obj, fuzz_str_14);
    
    // Add number - use a value from the parsed data if possible
    cJSON *first_item = cJSON_GetArrayItem(parsed, fuzz_int32_t_15);
    double num_value = 0.0;
    if (first_item && cJSON_IsArray(first_item)) {
        cJSON *num_item = cJSON_GetArrayItem(first_item, fuzz_int32_t_16);
        if (num_item) {
            num_value = cJSON_GetNumberValue(num_item);
        }
    }
    cJSON_AddNumberToObject(new_obj, fuzz_str_17, num_value);
    
    // 6. Create and add integer array
    int test_ints[] = {10, 20, 30, 40, 50};
    cJSON *int_array = cJSON_CreateIntArray(fuzzer_var_cJSON_CreateIntArray_29_0, fuzzer_size_cJSON_CreateIntArray_29_0);
    cJSON_AddItemToObject(new_obj, fuzz_str_19, int_array);
    
    // 7. Create boolean and add as reference
    cJSON *bool_obj = cJSON_CreateBool(fuzz_int32_t_20);
    cJSON_AddItemReferenceToObject(new_obj, fuzz_str_21, bool_obj);
    
    // 8. Create string reference
    cJSON *str_ref = cJSON_CreateStringReference(fuzz_str_22);
    cJSON_AddItemToObject(new_obj, fuzz_str_23, str_ref);
    
    // 9. Get object item and check type
    cJSON *str_item = cJSON_GetObjectItem(new_obj, fuzz_str_24);
    if (str_item && cJSON_IsString(str_item)) {
        // Get number value (will be 0 for strings)
        double str_as_num = cJSON_GetNumberValue(str_item);
        (void)str_as_num; // Use variable
    }
    
    // 10. Add the parsed object as a sub-object
    cJSON_AddItemToObject(new_obj, fuzz_str_25, parsed);
    
    // 11. Print the final object
    char *output = cJSON_PrintBuffered(new_obj, fuzz_int32_t_26, fuzz_int32_t_27);
    if (output) {
        free(output);
    }
    
    // 12. Cleanup
    // Note: parsed is now owned by new_obj, so don't delete separately
    cJSON_Delete(new_obj);
    cJSON_Delete(bool_obj);
    // str_ref is owned by new_obj
    
    return 0;
}