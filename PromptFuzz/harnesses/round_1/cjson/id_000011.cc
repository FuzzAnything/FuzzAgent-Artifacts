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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 58, nr_unique_branch: 1
//<Quality> {"density":29,"unique_branches":{"cJSON_GetNumberValue":[[111,9,111,30,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_IsInvalid","cJSON_CreateObjectReference","cJSON_IsBool","cJSON_GetNumberValue","cJSON_IsString","cJSON_SetValuestring","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_PrintBuffered","cJSON_PrintPreallocated","cJSON_Delete","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsBool","cJSON_GetNumberValue","cJSON_IsString","cJSON_SetValuestring","cJSON_IsObject","cJSON_CreateObjectReference","cJSON_GetObjectItem","cJSON_IsObject","cJSON_AddNumberToObject","cJSON_PrintBuffered","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetNumberValue","cJSON_Delete","cJSON_PrintPreallocated","cJSON_PrintBuffered","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_IsInvalid","cJSON_CreateObjectReference","cJSON_IsBool","cJSON_GetNumberValue","cJSON_IsString","cJSON_SetValuestring","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_PrintBuffered","cJSON_PrintPreallocated","cJSON_Delete","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsBool","cJSON_GetNumberValue","cJSON_IsString","cJSON_SetValuestring","cJSON_IsObject","cJSON_CreateObjectReference","cJSON_GetObjectItem","cJSON_IsObject","cJSON_AddNumberToObject","cJSON_PrintBuffered","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetNumberValue","cJSON_Delete","cJSON_PrintPreallocated","cJSON_PrintBuffered","cJSON_Delete","cJSON_Delete"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified cJSON APIs to test various JSON manipulation scenarios:

*/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Convert input to string for parsing
    char *input_string = nullptr;
    if (size > 0) {
        input_string = (char *)malloc(size + 1);
        if (!input_string) {
	return 0;
	}
	
        memcpy(input_string, data, size);
        input_string[size] = '\0';
    } else {
        return 0;
    }
    
    // Parse the input JSON
    cJSON *root = cJSON_ParseWithLength(input_string, size);
    if (!root) {
        free(input_string);
        return 0;
    }
    
    // Event: Test various JSON manipulation operations
    // 1. Check if root is invalid (should return false for valid JSON)
    cJSON_bool is_invalid = cJSON_IsInvalid(root);
    
    // 2. Create an object reference
    cJSON *object_ref = cJSON_CreateObjectReference(root);
    
    // 3. Test boolean checking
    cJSON_bool is_bool = cJSON_IsBool(root);
    
    // 4. Get number value if available
    double number_value = cJSON_GetNumberValue(root);
    
    // 5. Try to set a new valuestring (if the item is a string type)
    if (cJSON_IsString(root)) {
        char *new_string = cJSON_SetValuestring(root, "modified_value");
    }
    
    // 6. Add a number to a new object
    cJSON *new_object = cJSON_CreateObject();
    if (new_object) {
        cJSON *added_number = cJSON_AddNumberToObject(new_object, "fuzz_number", 3.14159);
        
        // 7. Test PrintBuffered API
        char *buffered_print = cJSON_PrintBuffered(new_object, 1024, 1);
        if (buffered_print) {
            free(buffered_print);
        }
        
        // 8. Test PrintPreallocated API
        char *prealloc_buffer = (char *)malloc(2048);
        if (prealloc_buffer) {
            cJSON_bool print_success = cJSON_PrintPreallocated(new_object, prealloc_buffer, 2048, 1);
            free(prealloc_buffer);
        }
        
        cJSON_Delete(new_object);
    }
    
    // 9. Try to find and process array items
    if (cJSON_IsArray(root)) {
        int array_size = cJSON_GetArraySize(root);
        if (array_size > 0) {
            for (int i = 0; i < array_size && i < 10; i++) {
                cJSON *array_item = cJSON_GetArrayItem(root, i);
                if (array_item) {
                    // Test boolean checking on array items
                    cJSON_bool item_is_bool = cJSON_IsBool(array_item);
                    
                    // Get number value if available
                    double item_number = cJSON_GetNumberValue(array_item);
                    
                    // Try to set valuestring if it's a string
                    if (cJSON_IsString(array_item)) {
                        char *item_new_string = cJSON_SetValuestring(array_item, "array_item_modified");
                    }
                }
            }
        }
    }
    
    // 10. Try to find and process object items
    if (cJSON_IsObject(root)) {
        // Create another reference for testing
        cJSON *child_ref = cJSON_CreateObjectReference(root->child);
        
        // Try to find specific object items
        cJSON *web_app = cJSON_GetObjectItem(root, "web-app");
        if (web_app && cJSON_IsObject(web_app)) {
            // Add a number to the web-app object
            cJSON *fuzz_item = cJSON_AddNumberToObject(web_app, "fuzzed_field", 42.0);
            
            // Test PrintBuffered on this object
            char *web_app_print = cJSON_PrintBuffered(web_app, 512, 0);
            if (web_app_print) {
                free(web_app_print);
            }
            
            // Get number values from nested objects
            cJSON *servlet_mapping = cJSON_GetObjectItem(web_app, "servlet-mapping");
            if (servlet_mapping) {
                cJSON *cofaxCDS = cJSON_GetObjectItem(servlet_mapping, "cofaxCDS");
                if (cofaxCDS && cJSON_IsString(cofaxCDS)) {
                    // This would typically be a string, not a number
                    // but we'll still call GetNumberValue (returns 0 for non-numbers)
                    double mapping_value = cJSON_GetNumberValue(cofaxCDS);
                }
            }
        }
        
        if (child_ref) {
            cJSON_Delete(child_ref);
        }
    }
    
    // 11. Create a file pointer for additional testing if needed
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read and process file content if needed
        assert_file_closed(&in_file);;
    }
    
    // 12. Create an output file for testing
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write something to the file if needed
        assert_file_closed(&out_file);;
    }
    
    // 13. Print the final JSON using PrintPreallocated
    char *final_buffer = (char *)malloc(4096);
    if (final_buffer) {
        cJSON_bool final_print = cJSON_PrintPreallocated(root, final_buffer, 4096, 1);
        free(final_buffer);
    }
    
    // 14. Alternative printing with PrintBuffered
    char *final_print = cJSON_PrintBuffered(root, 2048, 1);
    if (final_print) {
        free(final_print);
    }
    
    // Clean up
    if (object_ref) {
        cJSON_Delete(object_ref);
    }
    cJSON_Delete(root);
    free(input_string);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}