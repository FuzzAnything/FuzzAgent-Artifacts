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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 95, nr_unique_branch: 12
//<Quality> {"density":19,"unique_branches":{"print_object":[[1799,9,1799,31,0,0,4,0],[1818,17,1818,39,0,0,4,0],[1830,13,1830,83,0,0,4,0],[1838,13,1838,35,0,0,4,0]],"print_array":[[1614,9,1614,31,0,0,4,0],[1625,13,1625,57,0,0,4,0],[1634,17,1634,39,0,0,4,0],[1650,9,1650,31,0,0,4,0]],"print_number":[[641,9,641,31,0,0,4,0]],"print_value":[[1436,17,1436,31,0,0,4,0],[1445,17,1445,31,0,0,4,0],[1454,17,1454,31,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArrayItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetStringValue","cJSON_malloc","cJSON_PrintPreallocated","cJSON_CreateTrue","cJSON_IsTrue","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_malloc","cJSON_PrintPreallocated","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_free","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsTrue","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArrayItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetStringValue","cJSON_malloc","cJSON_PrintPreallocated","cJSON_CreateTrue","cJSON_IsTrue","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_malloc","cJSON_PrintPreallocated","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_free","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsTrue","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<58) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    // Use the input data as a JSON string for parsing
    const char* json_string = reinterpret_cast<const char*>(data);
    
    // Parse the input JSON data
    cJSON *root = cJSON_ParseWithLength(json_string, size);
    if (!root) {
        // If parsing fails, just return
        return 0;
    }
    
    // 1. Use cJSON_GetArrayItem to get an array item
    // First, try to get the "items" array from the "menu" object
    cJSON *menu = cJSON_GetObjectItem(root, fuzz_str_1);
    if (menu && cJSON_IsObject(menu)) {
        cJSON *items = cJSON_GetObjectItem(menu, fuzz_str_2);
        if (items && cJSON_IsArray(items)) {
            // Get the first array item
            cJSON *first_item = cJSON_GetArrayItem(items, fuzz_int32_t_3);
            if (first_item) {
                // 2. Use cJSON_GetStringValue to get a string value
                cJSON *id_field = cJSON_GetObjectItem(first_item, fuzz_str_4);
                if (id_field && cJSON_IsString(id_field)) {
                    const char *id_value = cJSON_GetStringValue(id_field);
                    // Use the string value in some way (even if just checking)
                    if (id_value) {
                        // Do something with id_value
                        (void)id_value; // Suppress unused warning
                    }
                }
            }
        }
    }
    
    // 3. Use cJSON_PrintPreallocated to print the JSON to a buffer
    // Create a buffer for printing
    const int buffer_size = 1024;
    char *buffer = (char*)cJSON_malloc(buffer_size);
    if (buffer) {
        cJSON_PrintPreallocated(root, buffer, buffer_size, fuzz_int32_t_5);
        // We could use buffer here if needed
        
        // 5. Use cJSON_IsTrue to check for boolean true values
        // Create a true boolean value for testing
        cJSON *true_item = cJSON_CreateTrue();
        if (true_item) {
            cJSON_bool is_true = cJSON_IsTrue(true_item);
            // Use the result in some way
            (void)is_true; // Suppress unused warning
            
            // 4. Use cJSON_AddNumberToObject to add a number
            // Create a new object and add a number to it
            cJSON *new_object = cJSON_CreateObject();
            if (new_object) {
                cJSON_AddNumberToObject(new_object, fuzz_str_6, 42.5);
                
                // Print the new object as well
                char *new_buffer = (char*)cJSON_malloc(buffer_size);
                if (new_buffer) {
                    cJSON_PrintPreallocated(new_object, new_buffer, buffer_size, fuzz_int32_t_7);
                    cJSON_free(new_buffer);
                }
                
                cJSON_Delete(new_object);
            }
            
            cJSON_Delete(true_item);
        }
        
        cJSON_free(buffer);
    }
    
    // 6. Additional test: Check if any item in the array is true (though our example has none)
    // This ensures cJSON_IsTrue is called on the input data too
    if (menu) {
        cJSON *items = cJSON_GetObjectItem(menu, fuzz_str_8);
        if (items && cJSON_IsArray(items)) {
            int array_size = cJSON_GetArraySize(items);
            for (int i = 0; i < array_size; i++) {
                cJSON *item = cJSON_GetArrayItem(items, fuzz_int32_t_9);
                if (item) {
                    cJSON_bool is_true = cJSON_IsTrue(item);
                    (void)is_true; // Suppress unused warning
                }
            }
        }
    }
    
    // Clean up
    cJSON_Delete(root);
    
    return 0;
}