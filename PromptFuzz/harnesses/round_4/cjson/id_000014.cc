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
//<ID> 59
//<Prompt> []
/*<Combination>: [
*/
//<score> 114, nr_unique_branch: 10
//<Quality> {"density":19,"unique_branches":{"cJSON_IsFalse":[[2984,9,2984,21,0,0,4,1]],"print_object":[[1799,9,1799,31,0,0,4,0],[1859,13,1859,35,0,0,4,0],[1879,9,1879,31,0,0,4,0]],"print_value":[[1436,17,1436,31,0,0,4,0],[1445,17,1445,31,0,0,4,0],[1454,17,1454,31,0,0,4,0]],"print_array":[[1614,9,1614,31,0,0,4,0],[1650,9,1650,31,0,0,4,0]],"print_number":[[641,9,641,31,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_CreateStringArray","cJSON_CreateObject","cJSON_Delete","cJSON_AddItemToObject","cJSON_AddFalseToObject","cJSON_IsFalse","cJSON_CreateNumber","cJSON_GetNumberValue","cJSON_CreateBool","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_Delete","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_AddItemReferenceToObject","cJSON_GetObjectItemCaseSensitive","cJSON_AddItemToObject","cJSON_PrintUnformatted","cJSON_free","cJSON_Print","cJSON_free","cJSON_PrintBuffered","cJSON_free","cJSON_PrintPreallocated","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateStringArray","cJSON_CreateObject","cJSON_AddItemToObject","cJSON_AddFalseToObject","cJSON_IsFalse","cJSON_CreateNumber","cJSON_GetNumberValue","cJSON_CreateBool","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_Delete","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_AddItemReferenceToObject","cJSON_GetObjectItemCaseSensitive","cJSON_AddItemToObject","cJSON_PrintUnformatted","cJSON_free","cJSON_Print","cJSON_free","cJSON_PrintBuffered","cJSON_free","cJSON_PrintPreallocated","cJSON_Delete"],"visited":0}
/*Looking at the provided APIs and the input data (an array of day names), I'll create a fuzz driver that uses all the specified APIs. The input data will be treated as JSON that needs to be parsed and manipulated.

*/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<133) return 0;

	
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
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
	
		const char * fuzzer_var_cJSON_CreateStringArray_1_0[7] = {fuzz_str_1,fuzz_str_2,fuzz_str_3,fuzz_str_4,fuzz_str_5,fuzz_str_6,fuzz_str_7};//fuzzer shim end}




    // Ensure we have valid data
    if (data == nullptr || size == 0) {
        return 0;
    }
    
    // Add null terminator to make a valid C string
    char *json_str = static_cast<char*>(malloc(size + 1));
    if (json_str == nullptr) {
        return 0;
    }
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    // Parse the input JSON (which should be an array of strings)
    cJSON *parsed_array = cJSON_Parse(json_str);
    free(json_str);
    
    if (parsed_array == nullptr) {
        // If parsing fails, try to create a new array instead
        const char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
        parsed_array = cJSON_CreateStringArray(fuzzer_var_cJSON_CreateStringArray_1_0, 7);
    }
    
    // Create a main object to work with
    cJSON *main_object = cJSON_CreateObject();
    if (main_object == nullptr) {
        cJSON_Delete(parsed_array);
        return 0;
    }
    
    // API: cJSON_CreateStringArray - Already used above if parsing failed
    
    // Add the parsed array to the main object
    cJSON_AddItemToObject(main_object, fuzz_str_8, parsed_array);
    
    // API: cJSON_AddFalseToObject - Add a false boolean to the object
    cJSON *false_item = cJSON_AddFalseToObject(main_object, "is_weekend");
    
    // API: cJSON_IsFalse - Check if the item is false
    if (false_item != nullptr && cJSON_IsFalse(false_item)) {
        // Create a number to store in the object
        cJSON *number_item = cJSON_CreateNumber(42.5);
        if (number_item != nullptr) {
            // API: cJSON_GetNumberValue - Get the number value
            double num_val = cJSON_GetNumberValue(number_item);
            
            // Create a boolean based on the number value
            cJSON_bool bool_val = (num_val > 0) ? 1 : 0;
            
            // API: cJSON_CreateBool - Create a boolean item
            cJSON *bool_item = cJSON_CreateBool(bool_val);
            
            // API: cJSON_ReplaceItemInObjectCaseSensitive - Replace the false item with the boolean
            cJSON_ReplaceItemInObjectCaseSensitive(main_object, "is_weekend", bool_item);
            
            // Don't forget to delete the old number item
            cJSON_Delete(number_item);
        }
    }
    
    // Create a reference object
    cJSON *reference_object = cJSON_CreateObject();
    if (reference_object != nullptr) {
        // Add some data to the reference object
        cJSON_AddStringToObject(reference_object, fuzz_str_9, fuzz_str_10);
        
        // API: cJSON_AddItemReferenceToObject - Add a reference to the parsed array
        cJSON_AddItemReferenceToObject(reference_object, fuzz_str_11, 
                                      cJSON_GetObjectItemCaseSensitive(main_object, fuzz_str_12));
        
        // Add the reference object to main object
        cJSON_AddItemToObject(main_object, fuzz_str_13, reference_object);
    }
    
    // API: cJSON_PrintUnformatted - Print the object as unformatted JSON
    char *unformatted_json = cJSON_PrintUnformatted(main_object);
    if (unformatted_json != nullptr) {
        // We could write this to a file if needed
        // FILE *out_file = fopen("output_file", "wb");
        // if (out_file != nullptr) {
        //     fwrite(unformatted_json, strlen(unformatted_json), 1, out_file);
        //     assert_file_closed(&out_file);;
        // }
        
        // For fuzzing, we'll just free the memory
        cJSON_free(unformatted_json);
    }
    
    // Also test with formatted printing
    char *formatted_json = cJSON_Print(main_object);
    if (formatted_json != nullptr) {
        cJSON_free(formatted_json);
    }
    
    // Test buffered printing
    char *buffered_json = cJSON_PrintBuffered(main_object, fuzz_int32_t_14, fuzz_int32_t_15);
    if (buffered_json != nullptr) {
        cJSON_free(buffered_json);
    }
    
    // Test with preallocated buffer
    char prealloc_buffer[2048];
    if (cJSON_PrintPreallocated(main_object, prealloc_buffer, sizeof(prealloc_buffer), fuzz_int32_t_16)) {
        // Successfully printed to preallocated buffer
    }
    
    // Clean up
    cJSON_Delete(main_object);
    
    return 0;
}