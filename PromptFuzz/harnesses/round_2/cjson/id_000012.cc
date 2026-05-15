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
//<score> 8.5, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["cJSON_ParseWithLength","cJSON_Duplicate","cJSON_Delete","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_AddBoolToObject","cJSON_GetObjectItem","cJSON_SetNumberHelper","cJSON_AddItemToObject","cJSON_PrintBuffered","cJSON_free","cJSON_PrintBuffered","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_Duplicate","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_AddBoolToObject","cJSON_GetObjectItem","cJSON_SetNumberHelper","cJSON_AddItemToObject","cJSON_PrintBuffered","cJSON_free","cJSON_PrintBuffered","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Parse input JSON using cJSON_ParseWithLength
    cJSON *parsed = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!parsed) {
        return 0; // Invalid JSON input
    }

    // Create a duplicate of the parsed JSON with recursion
    cJSON *duplicate = cJSON_Duplicate(parsed, true);
    if (!duplicate) {
        cJSON_Delete(parsed);
        return 0;
    }

    // Create a new object to demonstrate API usage
    cJSON *new_obj = cJSON_CreateObject();
    
    // 1. Add a number to the new object
    cJSON_AddNumberToObject(new_obj, "fuzz_number", 42.5);
    
    // 2. Create a float array and add it to the object
    float float_array[] = {1.1f, 2.2f, 3.3f, 4.4f};
    cJSON *float_array_item = cJSON_CreateFloatArray(float_array, 4);
    cJSON_AddItemToObject(new_obj, "float_array", float_array_item);
    
    // 3. Add a boolean to the object
    cJSON_AddBoolToObject(new_obj, "fuzz_bool", true);
    
    // 4. Set number value on an existing item using helper
    cJSON *number_item = cJSON_GetObjectItem(new_obj, "fuzz_number");
    if (number_item) {
        cJSON_SetNumberHelper(number_item, 99.9);
    }
    
    // 5. Print the combined object using buffered printing
    // First merge duplicate into new_obj
    cJSON_AddItemToObject(new_obj, "original_data", duplicate);
    
    // Create a buffered print of the object
    char *printed = cJSON_PrintBuffered(new_obj, 4096, true);
    if (printed) {
        // Write the printed JSON to a file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed, strlen(printed), 1, out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(printed);
    }
    
    // Also parse the printed output to verify it's valid
    if (printed) {
        char *reprinted = cJSON_PrintBuffered(new_obj, 2048, false);
        if (reprinted) {
            cJSON_free(reprinted);
        }
    }
    
    // Clean up - delete all allocated cJSON objects
    cJSON_Delete(parsed);
    // Note: duplicate is already part of new_obj, so it will be deleted with new_obj
    cJSON_Delete(new_obj);
    
    return 0;
}