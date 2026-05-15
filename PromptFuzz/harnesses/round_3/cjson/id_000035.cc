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
//<ID> 270
//<Prompt> []
/*<Combination>: [
*/
//<score> 68, nr_unique_branch: 3
//<Quality> {"density":17,"unique_branches":{"cJSON_IsTrue":[[2994,9,2994,21,0,0,4,0]],"cJSON_IsNull":[[3014,9,3014,21,0,0,4,0]],"cJSON_IsRaw":[[3064,9,3064,21,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_IsNull","cJSON_IsRaw","cJSON_IsTrue","cJSON_CreateObject","cJSON_AddNullToObject","cJSON_IsNull","cJSON_AddBoolToObject","cJSON_IsTrue","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_CreateArrayReference","cJSON_IsRaw","cJSON_Delete","cJSON_Delete","cJSON_CreateRaw","cJSON_IsRaw","cJSON_Delete","cJSON_Delete","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsNull","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsRaw","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsTrue","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_AddNullToObject","cJSON_IsNull","cJSON_AddBoolToObject","cJSON_IsTrue","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_CreateArrayReference","cJSON_IsRaw","cJSON_Delete","cJSON_Delete","cJSON_CreateRaw","cJSON_IsRaw","cJSON_Delete","cJSON_Delete","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsNull","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsRaw","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsTrue","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<106) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
	//fuzzer shim end}




    // Create a null-terminated string from the input data
    char *json_string = (char *)malloc(size + 1);
    if (!json_string) {
	return 0;
	}
	
    memcpy(json_string, data, size);
    json_string[size] = '\0';
    
    // Parse the JSON string
    cJSON *root = cJSON_Parse(json_string);
    free(json_string);
    
    if (!root) {
        // If parsing fails, we can still test some APIs with NULL input
        cJSON_IsNull(NULL);
        cJSON_IsRaw(NULL);
        cJSON_IsTrue(NULL);
        return 0;
    }
    
    // Create a new object to test the required APIs
    cJSON *test_object = cJSON_CreateObject();
    if (test_object) {
        // 1. Add a null item to the object
        cJSON *null_item = cJSON_AddNullToObject(test_object, fuzz_str_1);
        if (null_item) {
            // Check if it's actually null
            cJSON_IsNull(null_item);
        }
        
        // 2. Add a boolean item to the object
        cJSON *bool_item = cJSON_AddBoolToObject(test_object, fuzz_str_2, fuzz_int32_t_3);
        if (bool_item) {
            // Check if it's true
            cJSON_IsTrue(bool_item);
        }
        
        // 3. Create an array reference from the original JSON
        // First, find an array in the parsed JSON
        cJSON *widget = cJSON_GetObjectItem(root, fuzz_str_4);
        if (widget) {
            cJSON *window = cJSON_GetObjectItem(widget, fuzz_str_5);
            if (window) {
                // Create an array reference (though window is an object, not an array)
                // We'll create an array from scratch for testing
                cJSON *array = cJSON_CreateArray();
                if (array) {
                    cJSON_AddItemToArray(array, cJSON_CreateNumber(1));
                    cJSON_AddItemToArray(array, cJSON_CreateNumber(2));
                    cJSON_AddItemToArray(array, cJSON_CreateNumber(3));
                    
                    // Create array reference
                    cJSON *array_ref = cJSON_CreateArrayReference(array);
                    if (array_ref) {
                        // Check if the reference is raw (it won't be, but we call the API)
                        cJSON_IsRaw(array_ref);
                        cJSON_Delete(array_ref);
                    }
                    cJSON_Delete(array);
                }
            }
        }
        
        // 4. Check for raw items in the original JSON
        // The original JSON doesn't have raw items, but we can create one
        cJSON *raw_item = cJSON_CreateRaw(fuzz_str_6);
        if (raw_item) {
            cJSON_IsRaw(raw_item);
            cJSON_Delete(raw_item);
        }
        
        // Clean up test object
        cJSON_Delete(test_object);
    }
    
    // Additional testing: traverse the parsed JSON and use the required APIs
    if (cJSON_IsObject(root)) {
        cJSON *widget = cJSON_GetObjectItem(root, fuzz_str_7);
        if (widget && cJSON_IsObject(widget)) {
            // Check debug field (it's a string, not null, but we call the API)
            cJSON *debug = cJSON_GetObjectItem(widget, fuzz_str_8);
            if (debug) {
                cJSON_IsNull(debug);  // Will return false
            }
            
            // Check window object
            cJSON *window = cJSON_GetObjectItem(widget, fuzz_str_9);
            if (window && cJSON_IsObject(window)) {
                // Check title field
                cJSON *title = cJSON_GetObjectItem(window, fuzz_str_10);
                if (title) {
                    cJSON_IsRaw(title);  // Will return false
                }
            }
            
            // Check text object for boolean-like values
            cJSON *text = cJSON_GetObjectItem(widget, fuzz_str_11);
            if (text && cJSON_IsObject(text)) {
                // Check style field (it's "bold", not a boolean, but we call the API)
                cJSON *style = cJSON_GetObjectItem(text, fuzz_str_12);
                if (style) {
                    cJSON_IsTrue(style);  // Will return false
                }
            }
        }
    }
    
    // Clean up
    cJSON_Delete(root);
    
    return 0;
}