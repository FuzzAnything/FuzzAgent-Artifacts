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
//<ID> 153
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 1
//<Quality> {"density":24,"unique_branches":{"cJSON_CreateIntArray":[[2654,14,2654,22,0,0,4,1]]},"library_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_GetObjectItem","cJSON_IsBool","cJSON_CreateObjectReference","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNumber","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_AddNumberToObject","cJSON_AddObjectToObject","cJSON_AddNumberToObject","cJSON_AddStringToObject","cJSON_Print","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_GetObjectItem","cJSON_IsBool","cJSON_CreateObjectReference","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNumber","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_AddNumberToObject","cJSON_AddObjectToObject","cJSON_AddNumberToObject","cJSON_AddStringToObject","cJSON_Print","cJSON_Delete","cJSON_Delete"],"visited":1}
/*Looking at the requirements, I need to create a fuzz driver that uses the specified cJSON APIs to process JSON data. The goal is to create a new JSON object with arrays and object references based on the input data.

Here's my implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<162) return 0;

	
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
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
	//fuzzer shim end}




    // Create a null-terminated string from the input data
    if (size == 0) {
        return 0;
    }
    
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    // Parse the input JSON
    cJSON *root = cJSON_Parse(json_str);
    if (!root) {
        free(json_str);
        return 0;
    }
    
    // Create a new object that will contain our modifications
    cJSON *new_object = cJSON_CreateObject();
    
    // 1. Check if a specific item is boolean (using cJSON_IsBool)
    cJSON *image_obj = cJSON_GetObjectItem(root, fuzz_str_1);
    if (image_obj) {
        // Check if the Image object is a boolean (it's not, but we use the API)
        cJSON_bool is_bool = cJSON_IsBool(image_obj);
        // We'll use this result to decide what to do next
        
        // 2. Create an object reference to the Image object
        cJSON *image_ref = cJSON_CreateObjectReference(image_obj);
        
        // 3. Add the object reference to our new object
        if (image_ref) {
            cJSON_AddItemToObject(new_object, fuzz_str_2, image_ref);
        }
        
        // 4. Create arrays from data in the Image object
        cJSON *ids_array = cJSON_GetObjectItem(image_obj, fuzz_str_3);
        if (ids_array && cJSON_IsArray(ids_array)) {
            int array_size = cJSON_GetArraySize(ids_array);
            
            // Create int array from IDs
            int *int_numbers = (int *)malloc(sizeof(int) * array_size);
            double *double_numbers = (double *)malloc(sizeof(double) * array_size);
            
            if (int_numbers && double_numbers) {
                for (int i = 0; i < array_size; i++) {
                    cJSON *item = cJSON_GetArrayItem(ids_array, fuzz_int32_t_4);
                    if (item && cJSON_IsNumber(item)) {
                        int_numbers[i] = item->valueint;
                        double_numbers[i] = item->valuedouble;
                    } else {
                        int_numbers[i] = 0;
                        double_numbers[i] = 0.0;
                    }
                }
                
                // 5. Create int array using cJSON_CreateIntArray
                cJSON *int_array = cJSON_CreateIntArray(int_numbers, array_size);
                if (int_array) {
                    cJSON_AddItemToObject(new_object, fuzz_str_5, int_array);
                }
                
                // 6. Create double array using cJSON_CreateDoubleArray
                cJSON *double_array = cJSON_CreateDoubleArray(double_numbers, array_size);
                if (double_array) {
                    cJSON_AddItemToObject(new_object, fuzz_str_6, double_array);
                }
                
                free(int_numbers);
                free(double_numbers);
            }
        }
        
        // 7. Add a number to the new object using cJSON_AddNumberToObject
        cJSON *thumbnail = cJSON_GetObjectItem(image_obj, fuzz_str_7);
        if (thumbnail) {
            cJSON *width = cJSON_GetObjectItem(thumbnail, fuzz_str_8);
            if (width && cJSON_IsString(width)) {
                // Try to convert string width to number
                double width_value = atof(width->valuestring);
                cJSON_AddNumberToObject(new_object, fuzz_str_9, width_value);
            }
        }
        
        // 8. Add another object to the new object using cJSON_AddObjectToObject
        cJSON *added_object = cJSON_AddObjectToObject(new_object, fuzz_str_10);
        if (added_object) {
            // Add some properties to the added object
            cJSON_AddNumberToObject(added_object, fuzz_str_11, 42.5);
            cJSON_AddStringToObject(added_object, fuzz_str_12, fuzz_str_13);
        }
    }
    
    // Print the new object for debugging (optional)
    char *output = cJSON_Print(new_object);
    if (output) {
        free(output);
    }
    
    // Clean up
    cJSON_Delete(root);
    cJSON_Delete(new_object);
    free(json_str);
    
    return 0;
}