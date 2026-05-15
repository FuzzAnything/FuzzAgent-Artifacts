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
//<ID> 66
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":5,"unique_branches":{"cJSON_IsNumber":[[3024,9,3024,21,0,0,4,0]]},"library_calls":["cJSON_ParseWithOpts","cJSON_CreateObject","cJSON_Delete","cJSON_AddNullToObject","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_PrintUnformatted","cJSON_free","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithOpts","cJSON_CreateObject","cJSON_AddNullToObject","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_PrintUnformatted","cJSON_free","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified cJSON APIs to achieve the described event:

*/


// Include cJSON header

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Parse JSON from input data using cJSON_ParseWithOpts
    const char *parse_end = nullptr;
    // Create a null-terminated string from the input data
    char *json_string = (char *)malloc(size + 1);
    if (!json_string) {
        return 0;
    }
    memcpy(json_string, data, size);
    json_string[size] = '\0';
    
    // Parse the JSON string using cJSON_ParseWithOpts
    cJSON *parsed_json = cJSON_ParseWithOpts(json_string, &parse_end, 1);
    free(json_string);
    
    // Create a new JSON object
    cJSON *new_object = cJSON_CreateObject();
    if (!new_object) {
        if (parsed_json) {
            cJSON_Delete(parsed_json);
        }
        return 0;
    }
    
    // Add a null item to the new object
    cJSON *null_item = cJSON_AddNullToObject(new_object, "test_null");
    
    // Check if parsed_json is a number
    cJSON_bool is_number = cJSON_IsNumber(parsed_json);
    
    // If parsed_json is a number, use cJSON_SetNumberHelper to modify it
    if (parsed_json && is_number) {
        double new_number = cJSON_SetNumberHelper(parsed_json, 42.5);
        
        // Create a string representation using cJSON_PrintUnformatted
        char *json_string = cJSON_PrintUnformatted(parsed_json);
        if (json_string) {
            // Write the unformatted JSON to a file
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                fwrite(json_string, 1, strlen(json_string), out_file);
                assert_file_closed(&out_file);;
            }
            cJSON_free(json_string);
        }
    }
    
    // Also print the new_object
    char *new_object_string = cJSON_PrintUnformatted(new_object);
    if (new_object_string) {
        FILE *out_file2 = fopen("output_file2", "wb");
        if (out_file2) {
            fwrite(new_object_string, 1, strlen(new_object_string), out_file2);
            assert_file_closed(&out_file2);;
        }
        cJSON_free(new_object_string);
    }
    
    // Clean up
    if (parsed_json) {
        cJSON_Delete(parsed_json);
    }
    if (new_object) {
        cJSON_Delete(new_object);
    }
    
    return 0;
}