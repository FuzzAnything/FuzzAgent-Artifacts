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
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["cJSON_ParseWithLength","cJSON_Duplicate","cJSON_Delete","cJSON_AddNumberToObject","cJSON_SetNumberHelper","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_AddBoolToObject","cJSON_PrintBuffered","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_Duplicate","cJSON_AddNumberToObject","cJSON_SetNumberHelper","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_AddBoolToObject","cJSON_PrintBuffered","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Parse input data as JSON
    cJSON *root = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!root) {
        return 0;
    }

    // Create a duplicate of the parsed JSON with recursion
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    if (!duplicate) {
        cJSON_Delete(root);
        return 0;
    }

    // Add a new number field to the duplicate object
    cJSON *new_num_field = cJSON_AddNumberToObject(duplicate, "fuzz_number", 42.5);
    if (new_num_field) {
        // Modify the number using SetNumberHelper
        cJSON_SetNumberHelper(new_num_field, 99.9);
    }

    // Create and add a float array to the duplicate object
    float float_array[] = {1.1f, 2.2f, 3.3f, 4.4f};
    cJSON *float_array_obj = cJSON_CreateFloatArray(float_array, 4);
    if (float_array_obj) {
        cJSON_AddItemToObject(duplicate, "fuzz_float_array", float_array_obj);
    }

    // Add a boolean field to the duplicate object
    cJSON_AddBoolToObject(duplicate, "fuzz_bool", 1);

    // Serialize the modified JSON using buffered printing
    char *printed_json = cJSON_PrintBuffered(duplicate, 2048, 1);
    if (printed_json) {
        // Optional: Write to output file if needed
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed_json, 1, strlen(printed_json), out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(printed_json);
    }

    // Clean up
    cJSON_Delete(duplicate);
    cJSON_Delete(root);

    return 0;
}