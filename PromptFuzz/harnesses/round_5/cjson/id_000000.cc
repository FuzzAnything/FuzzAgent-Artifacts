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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 4
//<Quality> {"density":11,"unique_branches":{"print_number":[[77,18,77,26,2,0,4,0]],"cJSON_CreateNumber":[[2518,18,2518,40,0,0,4,0]],"cJSON_InsertItemInArray":[[2349,9,2349,39,0,0,4,1],[2357,9,2357,39,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_IsArray","cJSON_GetArrayItem","cJSON_IsObject","cJSON_CreateNumber","cJSON_ReplaceItemViaPointer","cJSON_CreateDoubleArray","cJSON_InsertItemInArray","cJSON_Print","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_IsArray","cJSON_GetArrayItem","cJSON_IsObject","cJSON_CreateNumber","cJSON_ReplaceItemViaPointer","cJSON_CreateDoubleArray","cJSON_InsertItemInArray","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Parse input data as JSON
    cJSON *root = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!root) {
        return 0;
    }

    // Event: Manipulate a JSON array by replacing an object element and inserting a new array
    // 1. Get array item at index
    // 2. Check if it's an object
    // 3. Create replacement number
    // 4. Replace via pointer
    // 5. Create double array
    // 6. Insert into array
    
    // Ensure root is an array for this event
    if (cJSON_IsArray(root)) {
        // Get array item at index 1
        cJSON *array_item = cJSON_GetArrayItem(root, 1);
        
        if (array_item && cJSON_IsObject(array_item)) {
            // Create replacement number using data from input
            double replacement_num = 0.0;
            if (size >= sizeof(double)) {
                memcpy(&replacement_num, data, sizeof(double));
            } else if (size > 0) {
                replacement_num = static_cast<double>(data[0]);
            }
            
            cJSON *new_number = cJSON_CreateNumber(replacement_num);
            
            // Replace the object with the number via pointer
            cJSON_ReplaceItemViaPointer(root, array_item, new_number);
            
            // Create double array using input data
            const size_t max_doubles = 3;
            double double_array[max_doubles];
            size_t num_doubles = (size / sizeof(double)) < max_doubles ? 
                                (size / sizeof(double)) : max_doubles;
            
            if (num_doubles > 0) {
                memcpy(double_array, data, num_doubles * sizeof(double));
            } else {
                // Fallback if no double data
                for (size_t i = 0; i < max_doubles; i++) {
                    double_array[i] = static_cast<double>(i);
                }
                num_doubles = max_doubles;
            }
            
            cJSON *new_double_array = cJSON_CreateDoubleArray(double_array, num_doubles);
            
            // Insert the double array at index 0
            cJSON_InsertItemInArray(root, 0, new_double_array);
        }
        
        // Cleanup - the replaced and inserted items are now owned by root
        // so they will be deleted with root
    }
    
    // Optional: Verify the structure is valid by printing (not necessary for fuzzing)
    char *printed = cJSON_Print(root);
    if (printed) {
        cJSON_free(printed);
    }
    
    // Clean up
    cJSON_Delete(root);
    
    return 0;
}