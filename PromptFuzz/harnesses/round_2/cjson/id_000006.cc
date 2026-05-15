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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_IsArray","cJSON_Version","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_AddArrayToObject","cJSON_HasObjectItem","cJSON_GetArraySize","cJSON_CreateArrayReference","cJSON_Compare","cJSON_AddItemToArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Print","cJSON_Minify","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_IsArray","cJSON_Version","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_AddArrayToObject","cJSON_HasObjectItem","cJSON_GetArraySize","cJSON_CreateArrayReference","cJSON_Compare","cJSON_AddItemToArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Print","cJSON_Minify","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


// Include cJSON headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if no data
    if (size == 0) {
	return 0;
	}
	

    // Create FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Read the entire input into a buffer
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *json_input = (char *)malloc(file_size + 1);
    if (!json_input) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t read_size = fread(json_input, 1, file_size, in_file);
    json_input[read_size] = '\0'; // Null-terminate
    assert_file_closed(&in_file);;

    // 1. Parse JSON using cJSON_Parse
    cJSON *parsed = cJSON_Parse(json_input);
    free(json_input); // Free the input buffer
    
    if (!parsed) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 2. Check if parsed item is an array
    cJSON_bool is_array = cJSON_IsArray(parsed);

    // 3. Get cJSON version
    const char *version = cJSON_Version();

    // 4. Create a new object and add items to it
    cJSON *new_object = cJSON_CreateObject();
    if (new_object) {
        // Add a string to the object
        cJSON *string_item = cJSON_AddStringToObject(new_object, "version", version);
        
        // Add an array to the object
        cJSON *array_item = cJSON_AddArrayToObject(new_object, "input_array");
        
        // Check if the object has the "version" item
        cJSON_bool has_version = cJSON_HasObjectItem(new_object, "version");
        
        // If we have an array in the parsed JSON, use it as reference
        if (is_array && cJSON_GetArraySize(parsed) > 0) {
            // Create array reference from parsed array
            cJSON *array_ref = cJSON_CreateArrayReference(parsed);
            
            // Compare the parsed array with the array reference
            cJSON_bool compare_result = cJSON_Compare(parsed, array_ref, cJSON_True);
            
            // Add the array reference to our array item
            if (array_item && array_ref) {
                cJSON_AddItemToArray(array_item, array_ref);
            }
            
            // Try to detach an item via pointer
            if (cJSON_GetArraySize(parsed) > 0) {
                cJSON *first_item = cJSON_GetArrayItem(parsed, 0);
                if (first_item) {
                    cJSON *detached = cJSON_DetachItemViaPointer(parsed, first_item);
                    if (detached) {
                        cJSON_Delete(detached); // Clean up detached item
                    }
                }
            }
        }

        // 5. Create minified JSON
        char *json_string = cJSON_Print(new_object);
        if (json_string) {
            // Minify the JSON string
            cJSON_Minify(json_string);
            
            // Write minified JSON to output file
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                int fd = fuzz_fileno(out_file);
                (void)fd; // Use fd to avoid unused variable warning
                fwrite(json_string, 1, strlen(json_string), out_file);
                assert_file_closed(&out_file);;
            }
            
            cJSON_free(json_string); // Free printed string
        }
        
        // 6. Clean up new_object
        cJSON_Delete(new_object);
    }

    // 7. Clean up parsed JSON
    cJSON_Delete(parsed);

    assert_file_closed(&in_file);
	return 0;
}