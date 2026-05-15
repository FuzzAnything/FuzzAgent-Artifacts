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
//<ID> 280
//<Prompt> []
/*<Combination>: [
*/
//<score> 70, nr_unique_branch: 2
//<Quality> {"density":35,"unique_branches":{"cJSON_Compare":[[3087,9,3087,23,0,0,4,0],[3118,9,3118,23,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_HasObjectItem","cJSON_GetObjectItem","cJSON_AddRawToObject","cJSON_AddNumberToObject","cJSON_HasObjectItem","cJSON_GetObjectItem","cJSON_CreateStringReference","cJSON_CreateObjectReference","cJSON_CreateArray","cJSON_AddItemReferenceToArray","cJSON_Compare","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_HasObjectItem","cJSON_GetObjectItem","cJSON_Duplicate","cJSON_Compare","cJSON_Delete","cJSON_HasObjectItem","cJSON_GetObjectItem","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateBool","cJSON_AddItemReferenceToArray","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_Compare","cJSON_Delete","cJSON_CreateObject","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_AddRawToObject","cJSON_AddNumberToObject","cJSON_AddRawToObject","cJSON_Compare","cJSON_CreateObjectReference","cJSON_CreateStringReference","cJSON_Compare","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_HasObjectItem","cJSON_GetObjectItem","cJSON_AddRawToObject","cJSON_AddNumberToObject","cJSON_HasObjectItem","cJSON_GetObjectItem","cJSON_CreateStringReference","cJSON_CreateObjectReference","cJSON_CreateArray","cJSON_AddItemReferenceToArray","cJSON_Compare","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_HasObjectItem","cJSON_GetObjectItem","cJSON_Duplicate","cJSON_Compare","cJSON_Delete","cJSON_HasObjectItem","cJSON_GetObjectItem","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateBool","cJSON_AddItemReferenceToArray","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_Compare","cJSON_Delete","cJSON_CreateObject","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_AddRawToObject","cJSON_AddNumberToObject","cJSON_AddRawToObject","cJSON_Compare","cJSON_CreateObjectReference","cJSON_CreateStringReference","cJSON_Compare","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<261) return 0;

	
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
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_15, fuzz_str_sz_15, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_16, fuzz_str_sz_16, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_17, fuzz_str_sz_17, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_18, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_19, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_20, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_21, fuzz_str_sz_21, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_22, fuzz_str_sz_22, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_23, fuzz_str_sz_23, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_24, fuzz_str_sz_24, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_25, fuzz_str_sz_25, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_26, fuzz_str_sz_26, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_27, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_28, fuzz_str_sz_28, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_29, fdp);
	//fuzzer shim end}




    // Parse the input JSON data
    cJSON *root = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!root) {
        return 0;
    }

    // Event: Modify the JSON structure using the specified APIs
    
    // 1. Check if the root object has a specific item
    cJSON_bool has_widget = cJSON_HasObjectItem(root, fuzz_str_1);
    
    if (has_widget) {
        cJSON *widget = cJSON_GetObjectItem(root, fuzz_str_2);
        
        // 2. Add a raw JSON string to the widget object
        cJSON *raw_item = cJSON_AddRawToObject(widget, fuzz_str_3, 
            fuzz_str_4);
        
        // 3. Add a number to the widget object
        cJSON *number_item = cJSON_AddNumberToObject(widget, fuzz_str_5, 42.5);
        
        // Check if widget has an "image" object
        if (cJSON_HasObjectItem(widget, fuzz_str_6)) {
            cJSON *image = cJSON_GetObjectItem(widget, fuzz_str_7);
            
            // 4. Create references for use with reference APIs
            cJSON *string_ref = cJSON_CreateStringReference(fuzz_str_8);
            cJSON *object_ref = cJSON_CreateObjectReference(image);
            
            // Create a temporary array for testing reference APIs
            cJSON *temp_array = cJSON_CreateArray();
            if (temp_array) {
                // 5. Add item reference to array
                cJSON_AddItemReferenceToArray(temp_array, image);
                
                // 6. Compare two JSON objects
                cJSON_bool compare_result = cJSON_Compare(image, object_ref, fuzz_int32_t_9);
                
                // 7. Delete item from array (if array has items)
                if (cJSON_GetArraySize(temp_array) > 0) {
                    cJSON_DeleteItemFromArray(temp_array, fuzz_int32_t_10);
                }
                
                // Clean up temporary array
                cJSON_Delete(temp_array);
            }
            
            // Clean up references
            cJSON_Delete(string_ref);
            cJSON_Delete(object_ref);
        }
        
        // Check if widget has nested objects for additional operations
        if (cJSON_HasObjectItem(widget, fuzz_str_11)) {
            cJSON *window = cJSON_GetObjectItem(widget, fuzz_str_12);
            
            // Create another object to compare with window
            cJSON *window_copy = cJSON_Duplicate(window, fuzz_int32_t_13);
            if (window_copy) {
                // Compare the original and copy
                cJSON_bool compare_windows = cJSON_Compare(window, window_copy, fuzz_int32_t_14);
                cJSON_Delete(window_copy);
            }
        }
        
        // Check for text object to test array operations
        if (cJSON_HasObjectItem(widget, fuzz_str_15)) {
            cJSON *text = cJSON_GetObjectItem(widget, fuzz_str_16);
            
            // Create an array and populate it
            cJSON *test_array = cJSON_CreateArray();
            if (test_array) {
                // Add some items to the array
                cJSON_AddItemToArray(test_array, cJSON_CreateString(fuzz_str_17));
                cJSON_AddItemToArray(test_array, cJSON_CreateNumber(100));
                cJSON_AddItemToArray(test_array, cJSON_CreateBool(fuzz_int32_t_18));
                
                // Add item reference to array
                cJSON_AddItemReferenceToArray(test_array, text);
                
                // Delete an item from the array if it has enough items
                int array_size = cJSON_GetArraySize(test_array);
                if (array_size > 0) {
                    // Delete item at index 0
                    cJSON_DeleteItemFromArray(test_array, fuzz_int32_t_19);
                }
                
                // Compare array with itself
                cJSON_bool array_compare = cJSON_Compare(test_array, test_array, fuzz_int32_t_20);
                
                cJSON_Delete(test_array);
            }
        }
    }
    
    // Create additional test objects for comparison operations
    cJSON *test_obj1 = cJSON_CreateObject();
    cJSON *test_obj2 = cJSON_CreateObject();
    
    if (test_obj1 && test_obj2) {
        // Add items to test objects
        cJSON_AddNumberToObject(test_obj1, fuzz_str_21, 123.45);
        cJSON_AddRawToObject(test_obj1, fuzz_str_22, fuzz_str_23);
        
        cJSON_AddNumberToObject(test_obj2, fuzz_str_24, 123.45);
        cJSON_AddRawToObject(test_obj2, fuzz_str_25, fuzz_str_26);
        
        // Compare the two test objects
        cJSON_bool obj_compare = cJSON_Compare(test_obj1, test_obj2, fuzz_int32_t_27);
        
        // Create references for the test objects
        cJSON *obj_ref = cJSON_CreateObjectReference(test_obj1);
        cJSON *string_ref2 = cJSON_CreateStringReference(fuzz_str_28);
        
        if (obj_ref && string_ref2) {
            // Compare object with its reference
            cJSON_bool ref_compare = cJSON_Compare(test_obj1, obj_ref, fuzz_int32_t_29);
            
            // Clean up references
            cJSON_Delete(obj_ref);
            cJSON_Delete(string_ref2);
        }
    }
    
    // Clean up test objects
    if (test_obj1) cJSON_Delete(test_obj1);
    if (test_obj2) cJSON_Delete(test_obj2);
    
    // Clean up the root object
    cJSON_Delete(root);
    
    return 0;
}