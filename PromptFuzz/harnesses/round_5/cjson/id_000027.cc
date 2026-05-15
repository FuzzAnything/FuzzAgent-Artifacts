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
//<ID> 162
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"cJSON_IsBool":[[3005,9,3005,21,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_CreateStringArray","cJSON_AddItemToObject","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_CreateString","cJSON_InsertItemInArray","cJSON_CreateArrayReference","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsBool","cJSON_CreateFalse","cJSON_AddItemToObject","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_CreateBool","cJSON_AddItemToObject","cJSON_AddStringToObject","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_CreateStringArray","cJSON_AddItemToObject","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_CreateString","cJSON_InsertItemInArray","cJSON_CreateArrayReference","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsBool","cJSON_CreateFalse","cJSON_AddItemToObject","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_CreateBool","cJSON_AddItemToObject","cJSON_AddStringToObject","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create null-terminated string from input data for parsing
    char *input_str = (char *)malloc(size + 1);
    if (!input_str) {
	return 0;
	}
	
    memcpy(input_str, data, size);
    input_str[size] = '\0';

    // 1. Parse the input JSON string
    cJSON *root = cJSON_Parse(input_str);
    
    // Create a new object for our operations
    cJSON *new_object = cJSON_CreateObject();
    
    if (root) {
        // 2. Get object item from parsed JSON
        cJSON *format_item = cJSON_GetObjectItem(root, "format");
        
        // 3. Check if format is an object (it should be)
        if (cJSON_IsObject(format_item)) {
            // 4. Create a string array
            const char *strings[] = {"item1", "item2", "item3"};
            cJSON *string_array = cJSON_CreateStringArray(strings, 3);
            
            // 5. Add string array to new_object
            cJSON_AddItemToObject(new_object, "string_array", string_array);
            
            // 6. Check if the string_array is actually an array
            if (cJSON_IsArray(string_array)) {
                // 7. Get array size
                int array_size = cJSON_GetArraySize(string_array);
                
                // 8. Get array item if array has elements
                if (array_size > 0) {
                    cJSON *first_item = cJSON_GetArrayItem(string_array, 0);
                }
                
                // 9. Insert an item into the array
                cJSON *new_string = cJSON_CreateString("inserted_item");
                cJSON_InsertItemInArray(string_array, 1, new_string);
            }
            
            // 10. Create array reference
            cJSON *array_ref = cJSON_CreateArrayReference(string_array);
            cJSON_AddItemToObject(new_object, "array_reference", array_ref);
        }
        
        // 11. Check if "interlace" is boolean (from original JSON)
        cJSON *interlace_item = cJSON_GetObjectItem(root, "format");
        if (interlace_item) {
            cJSON *interlace = cJSON_GetObjectItem(interlace_item, "interlace");
            if (cJSON_IsBool(interlace)) {
                // Add boolean items to new_object
                cJSON *false_item = cJSON_CreateFalse();
                cJSON_AddItemToObject(new_object, "false_value", false_item);
                
                cJSON *true_item = cJSON_CreateTrue();
                cJSON_AddItemToObject(new_object, "true_value", true_item);
                
                // Create boolean with CreateBool API
                cJSON *custom_bool = cJSON_CreateBool(1);
                cJSON_AddItemToObject(new_object, "custom_bool", custom_bool);
            }
        }
        
        // 12. Add string to object
        cJSON_AddStringToObject(new_object, "added_string", "Hello from fuzzer");
        
        // 13. Print the new object
        char *printed_json = cJSON_Print(new_object);
        if (printed_json) {
            // Open output file for writing
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                fwrite(printed_json, 1, strlen(printed_json), out_file);
                assert_file_closed(&out_file);;
                
                // Get file descriptor (for demonstration)
                int fd = fuzz_fileno(out_file);
                (void)fd; // Suppress unused variable warning
            }
            cJSON_free(printed_json);
        }
        
        // Clean up original parsed JSON
        cJSON_Delete(root);
    }
    
    // Clean up new_object
    cJSON_Delete(new_object);
    
    // Free allocated input string
    free(input_str);
    
    return 0;
}