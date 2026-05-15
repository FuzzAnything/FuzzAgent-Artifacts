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
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 2
//<Quality> {"density":7,"unique_branches":{"cJSON_SetNumberHelper":[[418,9,418,26,0,0,4,0],[422,14,422,39,0,0,4,0]]},"library_calls":["cJSON_ParseWithOpts","cJSON_CreateObject","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_AddNullToObject","cJSON_PrintUnformatted","cJSON_free","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithOpts","cJSON_CreateObject","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_AddNullToObject","cJSON_PrintUnformatted","cJSON_free","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Check for minimum size
    if (size == 0) {
        return 0;
    }

    // 1. Create a FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Read the input data into a buffer
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *buffer = (char *)malloc(file_size + 1);
    if (!buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(buffer, 1, file_size, in_file);
    buffer[bytes_read] = '\0';
    assert_file_closed(&in_file);;

    // 3. Parse the input using cJSON_ParseWithOpts
    const char *parse_end = nullptr;
    cJSON *parsed_json = cJSON_ParseWithOpts(buffer, &parse_end, 1);
    
    // 4. Create a new object using cJSON_CreateObject
    cJSON *new_object = cJSON_CreateObject();
    
    if (parsed_json && new_object) {
        // 5. Check if the parsed JSON is a number using cJSON_IsNumber
        if (cJSON_IsNumber(parsed_json)) {
            // 6. Modify the number using cJSON_SetNumberHelper
            cJSON_SetNumberHelper(parsed_json, parsed_json->valuedouble * 2);
        }
        
        // 7. Add a null item to the new object using cJSON_AddNullToObject
        cJSON_AddNullToObject(new_object, "fuzz_null");
        
        // 8. Print the modified parsed JSON unformatted using cJSON_PrintUnformatted
        char *printed_json = cJSON_PrintUnformatted(parsed_json);
        if (printed_json) {
            // Optional: Write to output file if needed
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                fwrite(printed_json, 1, strlen(printed_json), out_file);
                assert_file_closed(&out_file);;
            }
            cJSON_free(printed_json);
        }
        
        // 9. Print the new object as well
        char *printed_object = cJSON_PrintUnformatted(new_object);
        if (printed_object) {
            cJSON_free(printed_object);
        }
        
        // Clean up
        cJSON_Delete(parsed_json);
        cJSON_Delete(new_object);
    } else {
        // Clean up on failure
        if (parsed_json) cJSON_Delete(parsed_json);
        if (new_object) cJSON_Delete(new_object);
    }
    
    // 10. Release all allocated resources
    free(buffer);
    
    assert_file_closed(&in_file);
	return 0;
}