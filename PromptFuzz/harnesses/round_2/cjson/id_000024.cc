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
//<ID> 123
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_CreateArray","cJSON_IsObject","cJSON_IsArray","cJSON_CreateStringReference","cJSON_malloc","cJSON_CreateIntArray","cJSON_AddItemReferenceToObject","cJSON_CreateString","cJSON_ReplaceItemInObject","cJSON_Delete","cJSON_free","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNull","cJSON_CreateString","cJSON_InsertItemInArray","cJSON_AddItemReferenceToObject","cJSON_GetArraySize","cJSON_CreateNull","cJSON_InsertItemInArray","cJSON_Print","cJSON_free","cJSON_Print","cJSON_free","cJSON_malloc","cJSON_ParseWithLength","cJSON_Delete","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_CreateArray","cJSON_IsObject","cJSON_IsArray","cJSON_CreateStringReference","cJSON_malloc","cJSON_CreateIntArray","cJSON_AddItemReferenceToObject","cJSON_CreateString","cJSON_ReplaceItemInObject","cJSON_Delete","cJSON_free","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNull","cJSON_CreateString","cJSON_InsertItemInArray","cJSON_AddItemReferenceToObject","cJSON_Print","cJSON_free","cJSON_Print","cJSON_free","cJSON_malloc","cJSON_ParseWithLength","cJSON_Delete","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 1: Parse input data using cJSON_ParseWithLength
    cJSON *parsed_json = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    
    // Create a root object to work with
    cJSON *root_object = cJSON_CreateObject();
    cJSON *root_array = cJSON_CreateArray();
    
    // Step 2: Check if parsed_json is an object or array
    cJSON_bool is_object = cJSON_IsObject(parsed_json);
    cJSON_bool is_array = cJSON_IsArray(parsed_json);
    
    // Step 3: Create a string reference
    const char *ref_string = "reference_string";
    cJSON *string_ref = cJSON_CreateStringReference(ref_string);
    
    // Step 4: Create integer array using cJSON_malloc
    int *int_numbers = static_cast<int*>(cJSON_malloc(5 * sizeof(int)));
    if (int_numbers) {
        for (int i = 0; i < 5; i++) {
            int_numbers[i] = i * 10;
        }
        cJSON *int_array = cJSON_CreateIntArray(int_numbers, 5);
        
        // Step 5: Add item reference to object
        cJSON_AddItemReferenceToObject(root_object, "int_array_ref", int_array);
        
        // Step 6: Replace item in object
        cJSON *replacement = cJSON_CreateString("replaced_value");
        cJSON_ReplaceItemInObject(root_object, "int_array_ref", replacement);
        
        // Clean up
        cJSON_Delete(int_array);
        cJSON_free(int_numbers);
    }
    
    // Step 7: Work with parsed JSON if valid
    if (parsed_json) {
        if (is_object) {
            // Get array size if it's actually an array (checking child elements)
            cJSON *child = parsed_json->child;
            while (child) {
                if (cJSON_IsArray(child)) {
                    int array_size = cJSON_GetArraySize(child);
                    
                    // Get array item if available
                    if (array_size > 0) {
                        cJSON *array_item = cJSON_GetArrayItem(child, 0);
                        if (array_item && cJSON_IsNull(array_item)) {
                            // Insert new item in array
                            cJSON *new_item = cJSON_CreateString("inserted");
                            cJSON_InsertItemInArray(child, 0, new_item);
                        }
                    }
                }
                child = child->next;
            }
            
            // Add string reference to parsed object
            cJSON_AddItemReferenceToObject(parsed_json, "string_reference", string_ref);
        } else if (is_array) {
            // Work with array
            int array_size = cJSON_GetArraySize(parsed_json);
            
            // Insert item in array
            cJSON *insert_item = cJSON_CreateNull();
            cJSON_InsertItemInArray(parsed_json, array_size > 0 ? array_size - 1 : 0, insert_item);
        }
    }
    
    // Step 8: Print the root object
    char *printed_json = cJSON_Print(root_object);
    if (printed_json) {
        // Write to file using FILE* (simulating file operations)
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed_json, strlen(printed_json), 1, out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(printed_json);
    }
    
    // Step 9: Print the parsed JSON if it exists
    if (parsed_json) {
        char *parsed_printed = cJSON_Print(parsed_json);
        if (parsed_printed) {
            // Get file descriptor using fuzz_fileno
            FILE *temp_file = fopen("temp_output", "wb");
            if (temp_file) {
                int fd = fuzz_fileno(temp_file);
                (void)fd; // Use fd to avoid unused variable warning
                fwrite(parsed_printed, strlen(parsed_printed), 1, temp_file);
                assert_file_closed(&temp_file);;
            }
            cJSON_free(parsed_printed);
        }
    }
    
    // Step 10: Read input data using fmemopen (simulating file operations)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read and parse again to demonstrate multiple uses
        fseek(in_file, 0, SEEK_END);
        long file_size = ftell(in_file);
        fseek(in_file, 0, SEEK_SET);
        
        if (file_size > 0) {
            char *buffer = static_cast<char*>(cJSON_malloc(file_size + 1));
            if (buffer) {
                fread(buffer, 1, file_size, in_file);
                buffer[file_size] = '\0';
                
                // Parse again with the buffer
                cJSON *reparsed = cJSON_ParseWithLength(buffer, file_size);
                if (reparsed) {
                    cJSON_Delete(reparsed);
                }
                cJSON_free(buffer);
            }
        }
        assert_file_closed(&in_file);;
    }
    
    // Step 11: Clean up all allocated resources
    cJSON_Delete(parsed_json);
    cJSON_Delete(root_object);
    cJSON_Delete(root_array);
    cJSON_Delete(string_ref); // Note: string_ref doesn't own the string
    
    assert_file_closed(&in_file);
	return 0;
}