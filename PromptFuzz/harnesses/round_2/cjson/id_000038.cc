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
//<ID> 309
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"cJSON_CreateStringArray":[[2774,14,2774,22,0,0,4,1]]},"library_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_Delete","cJSON_GetObjectItem","cJSON_CreateObjectReference","cJSON_AddItemToObjectCS","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNumber","cJSON_CreateStringArray","cJSON_AddItemReferenceToObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_AddItemToObjectCS","cJSON_Print","cJSON_free","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_GetObjectItem","cJSON_CreateObjectReference","cJSON_AddItemToObjectCS","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNumber","cJSON_CreateStringArray","cJSON_AddItemReferenceToObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_AddItemToObjectCS","cJSON_Print","cJSON_free","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/*To achieve the specified event using the provided cjson APIs, we'll create a fuzz driver that:

1. Parses the input data as JSON
2. Creates a new object and performs operations using the specified APIs
3. Cleans up all allocated resources

*/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if data is too small
    if (size < 2) {
	return 0;
	}
	

    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Read the entire input data
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);

    if (file_size <= 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    char *json_string = (char *)malloc(file_size + 1);
    if (!json_string) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    size_t bytes_read = fread(json_string, 1, file_size, in_file);
    assert_file_closed(&in_file);;
    
    if (bytes_read != (size_t)file_size) {
        free(json_string);
        assert_file_closed(&in_file);
	return 0;
    }
    json_string[file_size] = '\0';

    // Parse the JSON input
    cJSON *root = cJSON_Parse(json_string);
    free(json_string);
    
    if (!root) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create a new object to work with
    cJSON *new_object = cJSON_CreateObject();
    if (!new_object) {
        cJSON_Delete(root);
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Create an object reference
    cJSON *image_object = cJSON_GetObjectItem(root, "Image");
    if (image_object) {
        // Create an object reference to the Image object
        cJSON *image_ref = cJSON_CreateObjectReference(image_object);
        if (image_ref) {
            // Add the reference to our new object
            cJSON_AddItemToObjectCS(new_object, "ImageRef", image_ref);
        }
    }

    // 2. Create a string array
    // Extract array data if available
    cJSON *ids_array = NULL;
    if (image_object) {
        ids_array = cJSON_GetObjectItem(image_object, "IDs");
    }
    
    const char *string_array_data[4] = {"str1", "str2", "str3", "str4"};
    int array_size = 4;
    
    // If we have an IDs array, use its values to create strings
    if (ids_array && cJSON_IsArray(ids_array)) {
        array_size = cJSON_GetArraySize(ids_array);
        if (array_size > 4) array_size = 4; // Limit to 4 elements
        
        for (int i = 0; i < array_size; i++) {
            cJSON *item = cJSON_GetArrayItem(ids_array, i);
            if (item && cJSON_IsNumber(item)) {
                char buffer[32];
                snprintf(buffer, sizeof(buffer), "id_%d", (int)item->valuedouble);
                // Note: In a real implementation, we'd need to allocate these strings
                // For fuzzing purposes, we'll use the static strings above
            }
        }
    }
    
    // Create the string array
    cJSON *string_array = cJSON_CreateStringArray(string_array_data, array_size);
    if (string_array) {
        // Add the string array as a reference to our object
        cJSON_AddItemReferenceToObject(new_object, "StringArrayRef", string_array);
    }

    // 3. Detach an item from the original object (if available)
    cJSON *detached_item = NULL;
    if (image_object) {
        detached_item = cJSON_DetachItemFromObjectCaseSensitive(image_object, "Thumbnail");
        if (detached_item) {
            // Add the detached item to our new object
            cJSON_AddItemToObjectCS(new_object, "DetachedThumbnail", detached_item);
        }
    }

    // 4. Process the new object by creating and printing it
    char *printed_json = cJSON_Print(new_object);
    if (printed_json) {
        // For fuzzing, we could write to a file, but we'll just free it
        // FILE *out_file = fopen("output_file", "wb");
        // if (out_file) {
        //     fwrite(printed_json, 1, strlen(printed_json), out_file);
        //     assert_file_closed(&out_file);;
        // }
        cJSON_free(printed_json);
    }

    // 5. Alternative: Print using preallocated buffer
    if (new_object) {
        const int buffer_size = 1024;
        char *buffer = (char *)malloc(buffer_size);
        if (buffer) {
            cJSON_PrintPreallocated(new_object, buffer, buffer_size - 1, 1);
            free(buffer);
        }
    }

    // Clean up - delete the string array separately since we only added a reference
    if (string_array) {
        cJSON_Delete(string_array);
    }
    
    // Note: image_ref and detached_item are owned by new_object now
    
    // Clean up all objects
    cJSON_Delete(new_object);
    cJSON_Delete(root);

    assert_file_closed(&in_file);
	return 0;
}