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
//<ID> 108
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"cJSON_CreateDoubleArray":[[2723,12,2723,14,0,0,4,1]]},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_CreateTrue","cJSON_CreateArray","cJSON_CreateIntArray","cJSON_CreateDoubleArray","cJSON_CreateObject","cJSON_AddItemReferenceToObject","cJSON_IsNull","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_CreateTrue","cJSON_CreateArray","cJSON_CreateIntArray","cJSON_CreateDoubleArray","cJSON_IsArray","cJSON_GetArrayItem","cJSON_IsObject","cJSON_AddItemReferenceToObject","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsNull","cJSON_IsNull","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_CreateTrue","cJSON_CreateArray","cJSON_CreateIntArray","cJSON_CreateDoubleArray","cJSON_IsArray","cJSON_GetArrayItem","cJSON_IsObject","cJSON_AddItemReferenceToObject","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsNull","cJSON_IsNull","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


// Include cJSON header

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<177) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_1, fuzz_int32_t_array_size_1, fdp);
		FDPConsumeDoublingArray(fuzz_double_array_2, fuzz_double_array_size_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_4, fuzz_int32_t_array_size_4, fdp);
		FDPConsumeDoublingArray(fuzz_double_array_5, fuzz_double_array_size_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
	
		const int * fuzzer_var_cJSON_CreateIntArray_3_0 = fuzz_int32_t_array_1;
		int fuzzer_size_cJSON_CreateIntArray_3_0 = static_cast<int>(fuzz_int32_t_array_size_1);
		const double * fuzzer_var_cJSON_CreateDoubleArray_4_0 = fuzz_double_array_2;
		int fuzzer_size_cJSON_CreateDoubleArray_4_0 = static_cast<int>(fuzz_double_array_size_2);
		const int * fuzzer_var_cJSON_CreateIntArray_15_0 = fuzz_int32_t_array_4;
		int fuzzer_size_cJSON_CreateIntArray_15_0 = static_cast<int>(fuzz_int32_t_array_size_4);
		const double * fuzzer_var_cJSON_CreateDoubleArray_16_0 = fuzz_double_array_5;
		int fuzzer_size_cJSON_CreateDoubleArray_16_0 = static_cast<int>(fuzz_double_array_size_5);//fuzzer shim end}




    // Step 1: Parse the input JSON data using cJSON_ParseWithLengthOpts
    const char *parse_end = nullptr;
    cJSON *parsed_json = cJSON_ParseWithLengthOpts(
        reinterpret_cast<const char*>(data), 
        size, 
        &parse_end, 
        false
    );
    
    if (!parsed_json) {
        // If parsing fails, we still need to use other APIs
        // Create required items and clean up
        cJSON *true_item = cJSON_CreateTrue();
        cJSON *new_array = cJSON_CreateArray();
        
        // Create int and double arrays
        int int_numbers[] = {1, 2, 3, 4, 5};
        double double_numbers[] = {1.1, 2.2, 3.3, 4.4, 5.5};
        
        cJSON *int_array = cJSON_CreateIntArray(fuzzer_var_cJSON_CreateIntArray_3_0, fuzzer_size_cJSON_CreateIntArray_3_0);
        cJSON *double_array = cJSON_CreateDoubleArray(fuzzer_var_cJSON_CreateDoubleArray_4_0, fuzzer_size_cJSON_CreateDoubleArray_4_0);
        
        // Create a temporary object to use AddItemReferenceToObject
        cJSON *temp_object = cJSON_CreateObject();
        if (temp_object && true_item) {
            cJSON_AddItemReferenceToObject(temp_object, fuzz_str_3, true_item);
        }
        
        // Check if true_item is null (should be false)
        cJSON_IsNull(true_item);
        
        // Clean up
        if (temp_object) cJSON_Delete(temp_object);
        if (int_array) cJSON_Delete(int_array);
        if (double_array) cJSON_Delete(double_array);
        if (new_array) cJSON_Delete(new_array);
        if (true_item) cJSON_Delete(true_item);
        
        return 0;
    }
    
    // Step 2: Create new cJSON items
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *new_array = cJSON_CreateArray();
    
    // Step 3: Create int and double arrays
    int int_numbers[] = {10, 20, 30, 40, 50};
    double double_numbers[] = {10.5, 20.5, 30.5, 40.5, 50.5};
    
    cJSON *int_array = cJSON_CreateIntArray(fuzzer_var_cJSON_CreateIntArray_15_0, fuzzer_size_cJSON_CreateIntArray_15_0);
    cJSON *double_array = cJSON_CreateDoubleArray(fuzzer_var_cJSON_CreateDoubleArray_16_0, fuzzer_size_cJSON_CreateDoubleArray_16_0);
    
    // Step 4: Check if parsed_json is an array
    if (cJSON_IsArray(parsed_json)) {
        // Get the first item if available
        cJSON *first_item = cJSON_GetArrayItem(parsed_json, fuzz_int32_t_6);
        
        if (first_item && cJSON_IsObject(first_item)) {
            // Step 5: Add a reference to true_item in the first object
            cJSON_AddItemReferenceToObject(first_item, fuzz_str_7, true_item);
            
            // Step 6: Delete an item from the object (if it exists)
            cJSON_DeleteItemFromObjectCaseSensitive(first_item, fuzz_str_8);
            
            // Step 7: Check if a specific field is null
            cJSON *city_item = cJSON_GetObjectItemCaseSensitive(first_item, fuzz_str_9);
            if (city_item) {
                cJSON_IsNull(city_item);  // Should return false for valid city
            }
            
            // Also check if true_item is null (should be false)
            cJSON_IsNull(true_item);
        }
    }
    
    // Step 8: Clean up all allocated resources
    if (int_array) cJSON_Delete(int_array);
    if (double_array) cJSON_Delete(double_array);
    if (new_array) cJSON_Delete(new_array);
    
    // Note: true_item is deleted when parsed_json is deleted because it was added as a reference
    // But we need to delete it separately if it wasn't added to any object
    if (true_item && !true_item->child && !true_item->next && !true_item->prev) {
        cJSON_Delete(true_item);
    }
    
    if (parsed_json) cJSON_Delete(parsed_json);
    
    return 0;
}