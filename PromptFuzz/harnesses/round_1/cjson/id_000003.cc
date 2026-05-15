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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 144, nr_unique_branch: 18
//<Quality> {"density":24,"unique_branches":{"print_number":[[77,18,77,26,2,0,4,0]],"cJSON_CreateRaw":[[2584,8,2584,12,0,0,4,0],[2588,12,2588,30,0,0,4,1]],"cJSON_CreateNumber":[[2514,13,2514,27,0,0,4,0],[2518,18,2518,40,0,0,4,0]],"print_value":[[1464,9,1464,23,0,0,4,0],[1467,17,1467,42,0,0,4,1],[1474,17,1474,31,0,0,4,1]],"cJSON_CreateDoubleArray":[[2708,9,2708,20,0,0,4,1],[2708,24,2708,41,0,0,4,1],[2715,16,2715,17,0,0,4,0],[2715,21,2715,40,0,0,4,0],[2715,21,2715,40,0,0,4,1],[2718,12,2718,14,0,0,4,1],[2723,12,2723,14,0,0,4,0],[2723,12,2723,14,0,0,4,1],[2734,9,2734,10,0,0,4,0],[2734,14,2734,22,0,0,4,0]]},"library_calls":["cJSON_Version","cJSON_CreateNull","cJSON_CreateDoubleArray","cJSON_CreateObject","cJSON_AddItemToObject","cJSON_CreateRaw","cJSON_AddItemToObject","cJSON_Print","cJSON_ParseWithLengthOpts","cJSON_GetObjectItem","cJSON_IsArray","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_GetArraySize","cJSON_ReplaceItemInArray","cJSON_CreateNumber","cJSON_AddItemReferenceToArray","cJSON_GetArrayItem","cJSON_Delete","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_Delete","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Version","cJSON_CreateNull","cJSON_CreateDoubleArray","cJSON_CreateObject","cJSON_AddItemToObject","cJSON_CreateRaw","cJSON_AddItemToObject","cJSON_Print","cJSON_ParseWithLengthOpts","cJSON_GetObjectItem","cJSON_IsArray","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_GetArraySize","cJSON_ReplaceItemInArray","cJSON_CreateNumber","cJSON_AddItemReferenceToArray","cJSON_GetArrayItem","cJSON_Delete","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_Delete","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified cJSON APIs with the given input:

*/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Get cJSON version
    const char *version = cJSON_Version();
    if (version) {
        // Could use version string, but for fuzzing we just need to call it
        (void)version;
    }
    
    // Create a null item
    cJSON *null_item = cJSON_CreateNull();
    
    // Create a double array using the input data
    // We'll interpret the input as double values if possible
    double *doubles = nullptr;
    int count = 0;
    
    if (size >= sizeof(double)) {
        count = static_cast<int>(size / sizeof(double));
        doubles = (double*)malloc(size);
        if (doubles) {
            memcpy(doubles, data, size);
            
            // Create double array from input data
            cJSON *double_array = cJSON_CreateDoubleArray(doubles, count);
            
            if (double_array) {
                // Create an object to work with
                cJSON *main_object = cJSON_CreateObject();
                
                if (main_object) {
                    // Add double array to object
                    cJSON_AddItemToObject(main_object, "matrix", double_array);
                    
                    // Create raw JSON item
                    cJSON *raw_item = cJSON_CreateRaw("raw_data");
                    
                    if (raw_item) {
                        // Add raw item to object
                        cJSON_AddItemToObject(main_object, "raw", raw_item);
                        
                        // Parse the object back to string
                        char *json_string = cJSON_Print(main_object);
                        
                        if (json_string && strlen(json_string) > 0) {
                            // Parse the string back using cJSON_ParseWithLengthOpts
                            const char *parse_end = nullptr;
                            cJSON *parsed = cJSON_ParseWithLengthOpts(json_string, 
                                                                    strlen(json_string), 
                                                                    &parse_end, 
                                                                    true);
                            
                            if (parsed) {
                                // Get the array from parsed object
                                cJSON *array_in_parsed = cJSON_GetObjectItem(parsed, "matrix");
                                
                                if (array_in_parsed && cJSON_IsArray(array_in_parsed)) {
                                    // Create another array for manipulation
                                    cJSON *temp_array = cJSON_CreateArray();
                                    
                                    if (temp_array) {
                                        // Add some items to temp array
                                        cJSON_AddItemToArray(temp_array, cJSON_CreateNumber(1.0));
                                        cJSON_AddItemToArray(temp_array, cJSON_CreateNumber(2.0));
                                        
                                        // Replace an item in the original array
                                        if (cJSON_GetArraySize(array_in_parsed) > 0) {
                                            cJSON_ReplaceItemInArray(array_in_parsed, 0, 
                                                                    cJSON_CreateNumber(999.0));
                                        }
                                        
                                        // Add item reference to array
                                        cJSON_AddItemReferenceToArray(temp_array, 
                                                                    cJSON_GetArrayItem(array_in_parsed, 0));
                                        
                                        // Clean up temp array
                                        cJSON_Delete(temp_array);
                                    }
                                    
                                    // Delete an item from the main object (case sensitive)
                                    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "raw");
                                }
                                
                                // Clean up parsed object
                                cJSON_Delete(parsed);
                            }
                            
                            // Free the JSON string
                            cJSON_free(json_string);
                        }
                        
                        // Note: raw_item is owned by main_object, don't free separately
                    }
                    
                    // Clean up main object (this will free double_array and raw_item)
                    cJSON_Delete(main_object);
                }
            }
            
            // Clean up doubles array
            free(doubles);
        }
    }
    
    // Clean up null item
    if (null_item) {
        cJSON_Delete(null_item);
    }
    
    return 0;
}