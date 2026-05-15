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
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 29, nr_unique_branch: 1
//<Quality> {"density":29,"unique_branches":{"cJSON_ReplaceItemViaPointer":[[2370,81,2370,95,0,0,4,0]]},"library_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_free","cJSON_IsArray","cJSON_Delete","cJSON_free","cJSON_GetArraySize","cJSON_Delete","cJSON_free","cJSON_GetArrayItem","cJSON_IsObject","cJSON_Delete","cJSON_free","cJSON_GetArrayItem","cJSON_IsObject","cJSON_Delete","cJSON_free","cJSON_DetachItemFromObject","cJSON_AddNullToObject","cJSON_AddItemReferenceToObject","cJSON_CreateNull","cJSON_ReplaceItemInObject","cJSON_Delete","cJSON_InsertItemInArray","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_CreateNull","cJSON_Delete","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetArrayItem","cJSON_IsObject","cJSON_DetachItemFromObject","cJSON_AddNullToObject","cJSON_AddItemReferenceToObject","cJSON_CreateNull","cJSON_ReplaceItemInObject","cJSON_Delete","cJSON_InsertItemInArray","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_CreateNull","cJSON_Delete","cJSON_Delete","cJSON_free"],"visited":1}
/*Looking at your requirements, I'll create a fuzz driver that processes JSON data using the cjson library. The driver will focus on manipulating JSON structures using the specified APIs.

*/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure we have enough data to work with
    if (size < 10) {
        return 0;
    }

    // Create input file from data buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the data into a buffer for JSON parsing
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (file_size <= 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    char *buffer = (char *)cJSON_malloc(file_size + 1);
    if (!buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(buffer, 1, file_size, in_file);
    buffer[bytes_read] = '\0';
    assert_file_closed(&in_file);;

    // Parse JSON from the buffer
    cJSON *root = cJSON_ParseWithLength(buffer, bytes_read);
    if (!root) {
        cJSON_free(buffer);
        assert_file_closed(&in_file);
	return 0;
    }

    // Check if root is an array
    if (!cJSON_IsArray(root)) {
        cJSON_Delete(root);
        cJSON_free(buffer);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get array size
    int array_size = cJSON_GetArraySize(root);
    if (array_size < 2) {
        cJSON_Delete(root);
        cJSON_free(buffer);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get first object in array
    cJSON *first_object = cJSON_GetArrayItem(root, 0);
    if (!cJSON_IsObject(first_object)) {
        cJSON_Delete(root);
        cJSON_free(buffer);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get second object in array
    cJSON *second_object = cJSON_GetArrayItem(root, 1);
    if (!cJSON_IsObject(second_object)) {
        cJSON_Delete(root);
        cJSON_free(buffer);
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Detach "City" from second object
    cJSON *detached_city = cJSON_DetachItemFromObject(second_object, "City");
    if (detached_city) {
        // 2. Add a null field to first object
        cJSON *null_field = cJSON_AddNullToObject(first_object, "NewNullField");
        
        // 3. Add reference to detached city in first object
        cJSON_bool ref_added = cJSON_AddItemReferenceToObject(first_object, "CityReference", detached_city);
        
        // 4. Replace "State" in first object with new null item
        cJSON *new_null = cJSON_CreateNull();
        if (new_null) {
            cJSON_bool replaced = cJSON_ReplaceItemInObject(first_object, "State", new_null);
            
            // Clean up the old state if replacement was successful
            if (!replaced) {
                cJSON_Delete(new_null);
            }
        }
        
        // 5. Insert detached city back into array at position 1
        cJSON_bool inserted = cJSON_InsertItemInArray(root, 1, detached_city);
        
        // If insertion failed, delete the detached item
        if (!inserted && detached_city) {
            cJSON_Delete(detached_city);
        }
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Get the modified JSON as a string
        char *output_json = cJSON_Print(root);
        if (output_json) {
            fwrite(output_json, 1, strlen(output_json), out_file);
            cJSON_free(output_json);
        }
        
        // Get file descriptor for output (though not used in this example)
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd; // Mark as used to avoid compiler warning
        
        assert_file_closed(&out_file);;
    }

    // Create another null item for demonstration
    cJSON *extra_null = cJSON_CreateNull();
    if (extra_null) {
        cJSON_Delete(extra_null);
    }

    // Clean up all allocated resources
    cJSON_Delete(root);
    cJSON_free(buffer);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}