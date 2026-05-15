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
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":34,"unique_branches":{},"library_calls":["cJSON_ParseWithLength","cJSON_IsArray","cJSON_IsObject","cJSON_Delete","cJSON_CreateObject","cJSON_CreateObject","cJSON_IsObject","cJSON_CreateObject","cJSON_Delete","cJSON_Delete","cJSON_IsObject","cJSON_Delete","cJSON_CreateDoubleArray","cJSON_Delete","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_Delete","cJSON_GetArrayItem","cJSON_CreateNumber","cJSON_ReplaceItemViaPointer","cJSON_Delete","cJSON_CreateNumber","cJSON_InsertItemInArray","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArrayItem","cJSON_CreateNumber","cJSON_ReplaceItemViaPointer","cJSON_CreateNumber","cJSON_InsertItemInArray","cJSON_Print","cJSON_free","cJSON_PrintPreallocated","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_IsObject","cJSON_IsArray","cJSON_Delete","cJSON_CreateObject","cJSON_IsObject","cJSON_CreateObject","cJSON_Delete","cJSON_IsObject","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArrayItem","cJSON_CreateNumber","cJSON_ReplaceItemViaPointer","cJSON_Delete","cJSON_CreateNumber","cJSON_InsertItemInArray","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArrayItem","cJSON_CreateNumber","cJSON_ReplaceItemViaPointer","cJSON_CreateNumber","cJSON_InsertItemInArray","cJSON_Print","cJSON_free","cJSON_PrintPreallocated","cJSON_Delete"],"visited":1}
/*I'll create a fuzz driver that uses the specified cJSON APIs to manipulate a JSON structure. Here's the implementation:

*/


// cJSON headers
extern "C" {
    #include "cJSON.h"
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // First, try to parse the input as JSON
    cJSON *parsed_json = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    
    // If parsing failed, create a default JSON structure to work with
    cJSON *root = nullptr;
    if (parsed_json) {
        // If parsed successfully and it's an array or object, use it
        if (cJSON_IsArray(parsed_json) || cJSON_IsObject(parsed_json)) {
            root = parsed_json;
        } else {
            // Create a new object if parsed JSON is not an array or object
            cJSON_Delete(parsed_json);
            root = cJSON_CreateObject();
        }
    } else {
        // Create a new object if parsing failed
        root = cJSON_CreateObject();
    }
    
    if (!root) {
        return 0;
    }
    
    // Ensure we have an object to work with
    if (!cJSON_IsObject(root)) {
        // Convert root to an object if it's not already one
        cJSON *new_root = cJSON_CreateObject();
        if (new_root) {
            cJSON_Delete(root);
            root = new_root;
        } else {
            cJSON_Delete(root);
            return 0;
        }
    }
    
    // Use cJSON_IsObject API (required)
    if (!cJSON_IsObject(root)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Create a simple array with numbers
    double numbers[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    
    // Use cJSON_CreateDoubleArray API (required)
    cJSON *double_array = cJSON_CreateDoubleArray(numbers, 5);
    if (!double_array) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Add the double array to the object
    cJSON_AddItemToObject(root, "double_array", double_array);
    
    // Get the array back from the object
    cJSON *array_item = cJSON_GetObjectItem(root, "double_array");
    if (!array_item || !cJSON_IsArray(array_item)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Use cJSON_GetArrayItem API (required)
    cJSON *first_array_item = cJSON_GetArrayItem(array_item, 0);
    if (first_array_item) {
        // Use cJSON_CreateNumber API (required)
        cJSON *new_number = cJSON_CreateNumber(99.99);
        if (new_number) {
            // Use cJSON_ReplaceItemViaPointer API (required)
            // Replace the first item in the array with a new number
            if (cJSON_ReplaceItemViaPointer(array_item, first_array_item, new_number)) {
                // Successfully replaced
            } else {
                cJSON_Delete(new_number);
            }
        }
    }
    
    // Try to insert a new item at index 2
    // Use cJSON_InsertItemInArray API (required)
    cJSON *insert_number = cJSON_CreateNumber(42.0);
    if (insert_number) {
        // Insert at position 2 (0-based index)
        cJSON_InsertItemInArray(array_item, 2, insert_number);
    }
    
    // Create another array to test more operations
    cJSON *test_array = cJSON_CreateArray();
    if (test_array) {
        cJSON_AddItemToArray(test_array, cJSON_CreateNumber(10.0));
        cJSON_AddItemToArray(test_array, cJSON_CreateNumber(20.0));
        cJSON_AddItemToArray(test_array, cJSON_CreateNumber(30.0));
        
        // Add test_array to root object
        cJSON_AddItemToObject(root, "test_array", test_array);
        
        // Get and manipulate test_array items
        cJSON *test_array_item = cJSON_GetObjectItem(root, "test_array");
        if (test_array_item && cJSON_IsArray(test_array_item)) {
            // Get item at index 1
            cJSON *array_item_1 = cJSON_GetArrayItem(test_array_item, 1);
            if (array_item_1) {
                // Replace it with a new number
                cJSON *replacement = cJSON_CreateNumber(25.0);
                if (replacement) {
                    cJSON_ReplaceItemViaPointer(test_array_item, array_item_1, replacement);
                }
            }
            
            // Insert at index 0
            cJSON *insert_at_zero = cJSON_CreateNumber(5.0);
            if (insert_at_zero) {
                cJSON_InsertItemInArray(test_array_item, 0, insert_at_zero);
            }
        }
    }
    
    // Create output to verify our manipulations work
    char *json_output = cJSON_Print(root);
    if (json_output) {
        // Write to a file (as per requirement 4)
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(json_output, 1, strlen(json_output), out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(json_output);
    }
    
    // Alternative: print to buffer
    char buffer[1024];
    if (cJSON_PrintPreallocated(root, buffer, sizeof(buffer), 1)) {
        // Buffer contains the formatted JSON
    }
    
    // Clean up
    cJSON_Delete(root);
    
    return 0;
}