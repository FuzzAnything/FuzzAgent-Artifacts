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
//<ID> 34
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"cJSON_IsTrue":[[2994,9,2994,21,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateArray","cJSON_Delete","cJSON_IsTrue","cJSON_AddItemReferenceToArray","cJSON_IsString","cJSON_GetStringValue","cJSON_AddTrueToObject","cJSON_IsTrue","cJSON_GetObjectItemCaseSensitive","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateArray","cJSON_IsTrue","cJSON_AddItemReferenceToArray","cJSON_IsString","cJSON_GetStringValue","cJSON_AddTrueToObject","cJSON_IsTrue","cJSON_GetObjectItemCaseSensitive","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
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
    
    char *json_string = (char *)malloc(file_size + 1);
    if (!json_string) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(json_string, 1, file_size, in_file);
    json_string[bytes_read] = '\0';
    assert_file_closed(&in_file);;

    // Parse the JSON string
    cJSON *parsed_json = cJSON_ParseWithLength(json_string, bytes_read);
    if (!parsed_json) {
        free(json_string);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a new array
    cJSON *new_array = cJSON_CreateArray();
    if (!new_array) {
        cJSON_Delete(parsed_json);
        free(json_string);
        assert_file_closed(&in_file);
	return 0;
    }

    // Traverse the parsed JSON to find boolean true values
    // and add references to them in the new array
    cJSON *current = NULL;
    cJSON_ArrayForEach(current, parsed_json) {
        if (cJSON_IsTrue(current)) {
            cJSON_AddItemReferenceToArray(new_array, current);
        }
    }

    // Find the first string value in the JSON
    const char *string_value = NULL;
    current = NULL;
    cJSON_ArrayForEach(current, parsed_json) {
        if (cJSON_IsString(current)) {
            string_value = cJSON_GetStringValue(current);
            if (string_value && string_value[0] != '\0') {
                break;
            }
        }
    }

    // If we found a string value, use it as a key to add a true value
    if (string_value && string_value[0] != '\0') {
        cJSON *true_object = cJSON_AddTrueToObject(parsed_json, string_value);
        if (true_object) {
            // Verify the added item is indeed true
            if (cJSON_IsTrue(cJSON_GetObjectItemCaseSensitive(parsed_json, string_value))) {
                // Delete the item we just added using case-sensitive deletion
                cJSON_DeleteItemFromObjectCaseSensitive(parsed_json, string_value);
            }
        }
    }

    // Clean up
    cJSON_Delete(new_array);
    cJSON_Delete(parsed_json);
    free(json_string);

    assert_file_closed(&in_file);
	return 0;
}