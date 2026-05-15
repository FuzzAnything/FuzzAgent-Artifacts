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
//<ID> 17
//<Prompt> []
/*<Combination>: [
*/
//<score> 56, nr_unique_branch: 10
//<Quality> {"density":24,"unique_branches":{"create_reference":[[2009,9,2009,21,0,0,4,1],[2015,9,2015,26,0,0,4,1]],"print_string_ptr":[[972,13,972,27,0,0,4,0]],"cJSON_Delete":[[259,13,259,46,0,0,4,1],[263,13,263,46,0,0,4,1]],"cJSON_GetNumberValue":[[111,9,111,30,0,0,4,0],[111,9,111,30,0,0,4,1]],"cJSON_AddItemReferenceToObject":[[2142,9,2142,25,0,0,4,1],[2142,29,2142,45,0,0,4,1]],"cJSON_CreateObjectReference":[[2563,9,2563,21,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_IsArray","cJSON_GetArraySize","cJSON_CreateObject","cJSON_CreateString","cJSON_AddItemToObject","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsNull","cJSON_GetStringValue","cJSON_AddStringToObject","cJSON_GetObjectItemCaseSensitive","cJSON_GetNumberValue","cJSON_AddStringToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsNull","cJSON_AddItemReferenceToObject","cJSON_IsTrue","cJSON_AddStringToObject","cJSON_CreateObjectReference","cJSON_AddItemReferenceToObject","cJSON_Delete","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_IsArray","cJSON_GetArraySize","cJSON_CreateObject","cJSON_CreateString","cJSON_AddItemToObject","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsNull","cJSON_GetStringValue","cJSON_AddStringToObject","cJSON_GetObjectItemCaseSensitive","cJSON_GetNumberValue","cJSON_AddStringToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsNull","cJSON_AddItemReferenceToObject","cJSON_IsTrue","cJSON_AddStringToObject","cJSON_CreateObjectReference","cJSON_AddItemReferenceToObject","cJSON_Delete","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<76) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    // Step 1: Create null-terminated string from input data
    if (size == 0) {
	return 0;
	}
	
    char *json_str = static_cast<char*>(malloc(size + 1));
    if (!json_str) {
	return 0;
	}
	
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    // Step 2: Parse JSON using cJSON_Parse
    cJSON *root = cJSON_Parse(json_str);
    free(json_str);
    
    if (!root) {
	return 0;
	}
	
    
    // Step 3: Check if root is array using cJSON_IsArray
    if (cJSON_IsArray(root)) {
        // Step 4: Get array size using cJSON_GetArraySize
        int array_size = cJSON_GetArraySize(root);
        
        // Step 5: Create new result object using cJSON_CreateObject
        cJSON *result = cJSON_CreateObject();
        
        // Create a string for array size using cJSON_CreateString
        char size_str[20];
        snprintf(size_str, sizeof(size_str), "%d", array_size);
        cJSON *size_string = cJSON_CreateString(size_str);
        
        // Add string to object using cJSON_AddItemToObject
        cJSON_AddItemToObject(result, fuzz_str_1, size_string);
        
        // Step 6: Process each item in array
        for (int i = 0; i < array_size; i++) {
            // Get array item using cJSON_GetArrayItem
            cJSON *item = cJSON_GetArrayItem(root, fuzz_int32_t_2);
            
            if (cJSON_IsObject(item)) {
                // Get "City" field using cJSON_GetObjectItemCaseSensitive
                cJSON *city_item = cJSON_GetObjectItemCaseSensitive(item, fuzz_str_3);
                
                if (city_item) {
                    // Check if city is null using cJSON_IsNull
                    if (!cJSON_IsNull(city_item)) {
                        // Get string value using cJSON_GetStringValue
                        const char *city_str = cJSON_GetStringValue(city_item);
                        
                        if (city_str) {
                            // Add city to result object using cJSON_AddStringToObject
                            char key[20];
                            snprintf(key, sizeof(key), "city_%d", i);
                            cJSON_AddStringToObject(result, key, city_str);
                        }
                    }
                }
                
                // Get "Latitude" field and check if it's a number
                cJSON *lat_item = cJSON_GetObjectItemCaseSensitive(item, fuzz_str_4);
                if (lat_item) {
                    // Get number value using cJSON_GetNumberValue
                    double lat = cJSON_GetNumberValue(lat_item);
                    
                    // Create a string representation of latitude
                    char lat_str[50];
                    snprintf(lat_str, sizeof(lat_str), "%.6f", lat);
                    
                    // Add to result using cJSON_AddStringToObject
                    char key[20];
                    snprintf(key, sizeof(key), "lat_%d", i);
                    cJSON_AddStringToObject(result, key, lat_str);
                }
                
                // Check if "State" field exists and is not null
                cJSON *state_item = cJSON_GetObjectItemCaseSensitive(item, fuzz_str_5);
                if (state_item && !cJSON_IsNull(state_item)) {
                    // Add a reference to state item using cJSON_AddItemReferenceToObject
                    char key[20];
                    snprintf(key, sizeof(key), "state_ref_%d", i);
                    cJSON_AddItemReferenceToObject(result, key, state_item);
                }
                
                // Check for any true values in the object
                // (In our test data, there are no boolean values, so this will always return false)
                cJSON *child = item->child;
                while (child) {
                    // Use cJSON_IsTrue to check for true values
                    if (cJSON_IsTrue(child)) {
                        // If we found a true value, add a marker
                        cJSON_AddStringToObject(result, fuzz_str_6, fuzz_str_7);
                    }
                    child = child->next;
                }
            }
        }
        
        // Step 7: Create an object reference using cJSON_CreateObjectReference
        cJSON *obj_ref = cJSON_CreateObjectReference(result);
        if (obj_ref) {
            // Add the reference to itself (just for demonstration)
            cJSON_AddItemReferenceToObject(result, fuzz_str_8, obj_ref);
            cJSON_Delete(obj_ref); // We need to delete the reference object
        }
        
        // Step 8: Print the result using cJSON_PrintPreallocated
        int buffer_size = 4096;
        char *buffer = static_cast<char*>(malloc(buffer_size));
        if (buffer) {
            cJSON_PrintPreallocated(result, buffer, buffer_size, fuzz_int32_t_9);
            // In a real fuzzer, we might do something with the buffer here
            free(buffer);
        }
        
        // Step 9: Clean up
        cJSON_Delete(result);
    }
    
    // Clean up root
    cJSON_Delete(root);
    
    return 0;
}