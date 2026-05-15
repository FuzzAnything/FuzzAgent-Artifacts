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
//<ID> 27
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 0
//<Quality> {"density":32,"unique_branches":{},"library_calls":["cJSON_malloc","cJSON_Parse","cJSON_free","cJSON_CreateObject","cJSON_GetObjectItem","cJSON_IsRaw","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_AddBoolToObject","cJSON_IsBool","cJSON_CreateArray","cJSON_AddItemReferenceToArray","cJSON_Compare","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_SetNumberHelper","cJSON_SetValuestring","cJSON_Duplicate","cJSON_Compare","cJSON_Delete","cJSON_Delete","cJSON_Compare","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_Parse","cJSON_CreateObject","cJSON_GetObjectItem","cJSON_IsRaw","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_AddBoolToObject","cJSON_IsBool","cJSON_CreateArray","cJSON_AddItemReferenceToArray","cJSON_Compare","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_SetNumberHelper","cJSON_SetValuestring","cJSON_Duplicate","cJSON_Compare","cJSON_Delete","cJSON_Delete","cJSON_Compare","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<79) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
	//fuzzer shim end}




    // Create a FILE pointer from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the JSON data from the file
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *json_str = (char *)cJSON_malloc(file_size + 1);
    if (!json_str) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t read_size = fread(json_str, 1, file_size, in_file);
    json_str[read_size] = '\0';
    assert_file_closed(&in_file);;

    // Parse the JSON string
    cJSON *root = cJSON_Parse(json_str);
    if (!root) {
        cJSON_free(json_str);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a new JSON object for comparison operations
    cJSON *comparison_obj = cJSON_CreateObject();
    
    // Get the "menu" object from the parsed JSON
    cJSON *menu = cJSON_GetObjectItem(root, fuzz_str_1);
    if (menu) {
        // Check if menu is a raw JSON (though it's not in this case)
        cJSON_bool is_raw = cJSON_IsRaw(menu);
        
        // Get the "popup" object from menu
        cJSON *popup = cJSON_GetObjectItem(menu, fuzz_str_2);
        if (popup) {
            // Get the "menuitem" array from popup
            cJSON *menuitem = cJSON_GetObjectItem(popup, fuzz_str_3);
            if (cJSON_IsArray(menuitem)) {
                int array_size = cJSON_GetArraySize(menuitem);
                if (array_size > 0) {
                    // Add a reference to the first item in the array to comparison_obj
                    cJSON *first_item = cJSON_GetArrayItem(menuitem, fuzz_int32_t_4);
                    if (first_item) {
                        // Add a boolean item to comparison_obj
                        cJSON *bool_item = cJSON_AddBoolToObject(comparison_obj, fuzz_str_5, fuzz_int32_t_6);
                        
                        // Check if the bool_item is actually a boolean
                        cJSON_bool is_bool = cJSON_IsBool(bool_item);
                        
                        // Add a reference to the first item to a new array
                        cJSON *new_array = cJSON_CreateArray();
                        if (new_array) {
                            cJSON_bool ref_added = cJSON_AddItemReferenceToArray(new_array, first_item);
                            
                            // Compare the original first item with the referenced one
                            cJSON_bool are_equal = cJSON_Compare(first_item, 
                                                              cJSON_GetArrayItem(new_array, fuzz_int32_t_7), 
                                                              true);
                            
                            // Set a number value using the helper
                            if (cJSON_IsObject(first_item)) {
                                cJSON *value_item = cJSON_GetObjectItem(first_item, fuzz_str_8);
                                if (value_item && cJSON_IsString(value_item)) {
                                    // First set it as a number
                                    double number_result = cJSON_SetNumberHelper(value_item, 42.5);
                                    
                                    // Then change it back to a string
                                    char *set_result = cJSON_SetValuestring(value_item, fuzz_str_9);
                                    
                                    // Create a duplicate for comparison
                                    cJSON *duplicate = cJSON_Duplicate(value_item, true);
                                    if (duplicate) {
                                        cJSON_bool strings_equal = cJSON_Compare(value_item, duplicate, true);
                                        cJSON_Delete(duplicate);
                                    }
                                }
                            }
                            
                            cJSON_Delete(new_array);
                        }
                    }
                }
            }
        }
    }

    // Compare the original root with our modified comparison object
    cJSON_bool compare_result = cJSON_Compare(root, comparison_obj, false);
    
    // Create an output file to write some results (optional for fuzzing)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        char *printed = cJSON_Print(root);
        if (printed) {
            fwrite(printed, 1, strlen(printed), out_file);
            cJSON_free(printed);
        }
        assert_file_closed(&out_file);;
    }

    // Clean up all allocated resources
    cJSON_Delete(root);
    cJSON_Delete(comparison_obj);
    cJSON_free(json_str);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}