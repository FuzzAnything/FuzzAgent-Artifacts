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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["cJSON_malloc","cJSON_Parse","cJSON_free","cJSON_CreateTrue","cJSON_IsBool","cJSON_CreateStringArray","cJSON_CreateArrayReference","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_Parse","cJSON_CreateTrue","cJSON_IsBool","cJSON_CreateStringArray","cJSON_CreateArrayReference","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_Delete","cJSON_Delete","cJSON_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the JSON data from the file
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *json_string = (char *)cJSON_malloc(file_size + 1);
    if (!json_string) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t read_size = fread(json_string, 1, file_size, in_file);
    json_string[read_size] = '\0';
    assert_file_closed(&in_file);;

    // Parse the JSON
    cJSON *root = cJSON_Parse(json_string);
    if (!root) {
        cJSON_free(json_string);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a true value
    cJSON *true_item = cJSON_CreateTrue();
    if (true_item) {
        // Check if it's a boolean
        if (cJSON_IsBool(true_item)) {
            // Create string array
            const char *strings[] = {"item1", "item2", "item3"};
            cJSON *string_array = cJSON_CreateStringArray(strings, 3);
            
            if (string_array) {
                // Create array reference
                cJSON *array_ref = cJSON_CreateArrayReference(string_array);
                
                if (array_ref) {
                    // Create a new object to add items to
                    cJSON *new_object = cJSON_CreateObject();
                    if (new_object) {
                        // Add string to object
                        cJSON_AddStringToObject(new_object, "status", "active");
                        
                        // Add the array reference to the object
                        cJSON_AddItemToObject(new_object, "ref_array", array_ref);
                        
                        // Add the true item to the object
                        cJSON_AddItemToObject(new_object, "flag", true_item);
                        
                        // Add string array to the object
                        cJSON_AddItemToObject(new_object, "string_array", string_array);
                        
                        // Clean up the new object
                        cJSON_Delete(new_object);
                    } else {
                        cJSON_Delete(array_ref);
                        cJSON_Delete(string_array);
                        cJSON_Delete(true_item);
                    }
                } else {
                    cJSON_Delete(string_array);
                    cJSON_Delete(true_item);
                }
            } else {
                cJSON_Delete(true_item);
            }
        } else {
            cJSON_Delete(true_item);
        }
    }

    // Clean up
    cJSON_Delete(root);
    cJSON_free(json_string);
    
    assert_file_closed(&in_file);
	return 0;
}