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
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 130, nr_unique_branch: 27
//<Quality> {"density":20,"unique_branches":{"print_object":[[1797,24,1797,45,0,0,4,1],[1806,9,1806,30,0,0,4,1],[1814,13,1814,34,0,0,4,1],[1836,28,1836,49,0,0,4,1],[1843,13,1843,34,0,0,4,1],[1850,13,1850,54,0,0,4,0],[1857,28,1857,49,0,0,4,1],[1859,13,1859,35,0,0,4,0],[1868,13,1868,34,0,0,4,1],[1878,44,1878,65,0,0,4,1],[1879,9,1879,31,0,0,4,0],[1883,9,1883,30,0,0,4,1]],"cJSON_ParseWithOpts":[[1135,9,1135,22,0,0,4,1]],"cJSON_IsString":[[3034,9,3034,21,0,0,4,0]],"ensure":[[518,9,518,19,0,0,4,0]],"print_string_ptr":[[1008,9,1008,23,0,0,4,0]],"cJSON_IsTrue":[[2994,9,2994,21,0,0,4,1]],"get_array_item":[[1932,12,1932,35,0,0,4,1],[1932,39,1932,50,0,0,4,0]],"cJSON_PrintPreallocated":[[1352,9,1352,21,0,0,4,1],[1352,25,1352,41,0,0,4,1]],"cJSON_GetStringValue":[[101,9,101,30,0,0,4,0],[101,9,101,30,0,0,4,1]],"cJSON_AddNumberToObject":[[2201,9,2201,76,0,0,4,0]],"cJSON_IsNull":[[3014,9,3014,21,0,0,4,1]],"print_array":[[1632,32,1632,53,0,0,4,1],[1639,16,1639,37,0,0,4,1]]},"library_calls":["cJSON_Parse","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArrayItem","cJSON_GetStringValue","cJSON_GetObjectItemCaseSensitive","cJSON_IsNull","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_AddStringToObject","cJSON_PrintPreallocated","cJSON_Delete","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNull","cJSON_IsTrue","cJSON_GetObjectItemCaseSensitive","cJSON_GetStringValue","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToObject","cJSON_GetArrayItem","cJSON_GetStringValue","cJSON_IsTrue","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArrayItem","cJSON_GetStringValue","cJSON_GetObjectItemCaseSensitive","cJSON_IsNull","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_AddStringToObject","cJSON_PrintPreallocated","cJSON_Delete","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNull","cJSON_IsTrue","cJSON_GetObjectItemCaseSensitive","cJSON_GetStringValue","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToObject","cJSON_GetArrayItem","cJSON_GetStringValue","cJSON_IsTrue","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified cjson APIs to process JSON input:

*/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<116) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
	//fuzzer shim end}




    // Create a null-terminated string from the input data
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
	return 0;
	}
	
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    // Parse the JSON input
    cJSON *root = cJSON_Parse(json_str);
    if (!root) {
        free(json_str);
        return 0;
    }
    
    // Event: Process the JSON structure and create a modified version
    
    // 1. Get array item from "items" array if it exists
    cJSON *menu = cJSON_GetObjectItemCaseSensitive(root, fuzz_str_1);
    if (menu) {
        cJSON *items = cJSON_GetObjectItemCaseSensitive(menu, fuzz_str_2);
        if (items && cJSON_IsArray(items)) {
            // Use cJSON_GetArrayItem
            cJSON *first_item = cJSON_GetArrayItem(items, fuzz_int32_t_3);
            if (first_item) {
                // Use cJSON_GetStringValue
                const char *id_value = cJSON_GetStringValue(
                    cJSON_GetObjectItemCaseSensitive(first_item, fuzz_str_4));
                
                // Use cJSON_IsTrue (though our input doesn't have booleans, we'll check for null)
                cJSON_bool is_null = cJSON_IsNull(first_item);
                
                // Create a new object to demonstrate the APIs
                cJSON *new_obj = cJSON_CreateObject();
                if (new_obj) {
                    // Use cJSON_AddNumberToObject
                    cJSON_AddNumberToObject(new_obj, fuzz_str_5, 1.0);
                    
                    if (id_value) {
                        cJSON_AddStringToObject(new_obj, fuzz_str_6, id_value);
                    }
                    
                    // Use cJSON_PrintPreallocated
                    char buffer[1024];
                    cJSON_bool print_success = cJSON_PrintPreallocated(
                        new_obj, buffer, sizeof(buffer), fuzz_int32_t_7);
                    
                    // Clean up
                    cJSON_Delete(new_obj);
                }
            }
            
            // Process more array items
            int array_size = cJSON_GetArraySize(items);
            for (int i = 0; i < array_size && i < 5; i++) {
                cJSON *item = cJSON_GetArrayItem(items, fuzz_int32_t_8);
                if (item && !cJSON_IsNull(item)) {
                    // Check if it's a true boolean (won't be in our input, but demonstrates the API)
                    cJSON_bool is_true = cJSON_IsTrue(item);
                    
                    // Get string value of id if present
                    cJSON *id = cJSON_GetObjectItemCaseSensitive(item, fuzz_str_9);
                    if (id) {
                        const char *str_val = cJSON_GetStringValue(id);
                        // str_val could be used here
                    }
                }
            }
        }
    }
    
    // Create another test object to ensure all APIs are used
    cJSON *test_obj = cJSON_CreateObject();
    if (test_obj) {
        // Add a number
        cJSON_AddNumberToObject(test_obj, fuzz_str_10, 42.5);
        
        // Create an array and add items
        cJSON *test_array = cJSON_CreateArray();
        if (test_array) {
            cJSON_AddItemToArray(test_array, cJSON_CreateString(fuzz_str_11));
            cJSON_AddItemToArray(test_array, cJSON_CreateString(fuzz_str_12));
            cJSON_AddItemToObject(test_obj, fuzz_str_13, test_array);
            
            // Get array item
            cJSON *array_item = cJSON_GetArrayItem(test_array, fuzz_int32_t_14);
            if (array_item) {
                // Get string value
                const char *str_val = cJSON_GetStringValue(array_item);
                
                // Check if true (it's not, but demonstrates API usage)
                cJSON_bool is_true_check = cJSON_IsTrue(array_item);
            }
        }
        
        // Print using preallocated buffer
        char print_buffer[512];
        cJSON_PrintPreallocated(test_obj, print_buffer, sizeof(print_buffer), fuzz_int32_t_15);
        
        cJSON_Delete(test_obj);
    }
    
    // Clean up
    cJSON_Delete(root);
    free(json_str);
    
    return 0;
}