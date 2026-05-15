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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 91, nr_unique_branch: 44
//<Quality> {"density":13,"unique_branches":{"cJSON_CreateArrayReference":[[2573,9,2573,21,0,0,4,0]],"get_object_item":[[1963,45,1963,78,0,0,4,1],[1976,38,1976,71,0,0,4,0]],"print_array":[[1632,32,1632,53,0,0,4,1],[1639,16,1639,37,0,0,4,1]],"cJSON_GetStringValue":[[101,9,101,30,0,0,4,0],[101,9,101,30,0,0,4,1]],"cJSON_Compare":[[3074,9,3074,20,0,0,4,1],[3074,24,3074,35,0,0,4,1],[3074,39,3074,77,0,0,4,0],[3074,39,3074,77,0,0,4,1],[3082,9,3082,25,0,0,4,1],[3083,9,3083,24,0,0,4,1],[3084,9,3084,24,0,0,4,1],[3085,9,3085,26,0,0,4,1],[3086,9,3086,26,0,0,4,0],[3086,9,3086,26,0,0,4,1],[3087,9,3087,23,0,0,4,1],[3088,9,3088,25,0,0,4,0],[3088,9,3088,25,0,0,4,1],[3089,9,3089,26,0,0,4,1],[3092,9,3092,16,0,0,4,1],[3097,9,3097,15,0,0,4,1],[3105,9,3105,25,0,0,4,1],[3106,9,3106,24,0,0,4,1],[3107,9,3107,24,0,0,4,1],[3110,9,3110,26,0,0,4,1],[3117,9,3117,26,0,0,4,0],[3117,9,3117,26,0,0,4,1],[3118,9,3118,23,0,0,4,1],[3119,17,3119,41,0,0,4,1],[3119,45,3119,69,0,0,4,1],[3123,17,3123,60,0,0,4,0],[3130,9,3130,25,0,0,4,0],[3130,9,3130,25,0,0,4,1],[3135,20,3135,39,0,0,4,0],[3135,43,3135,62,0,0,4,0],[3135,43,3135,62,0,0,4,1],[3137,21,3137,73,0,0,4,0],[3137,21,3137,73,0,0,4,1],[3147,17,3147,39,0,0,4,0],[3154,9,3154,26,0,0,4,1],[3192,9,3192,16,0,0,4,1]],"cJSON_IsString":[[3034,9,3034,21,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_GetObjectItemCaseSensitive","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_GetStringValue","cJSON_Delete","cJSON_CreateStringArray","cJSON_Delete","cJSON_CreateArrayReference","cJSON_Delete","cJSON_Delete","cJSON_Compare","cJSON_CreateStringArray","cJSON_Compare","cJSON_GetArrayItem","cJSON_GetStringValue","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetStringValue","cJSON_CreateStringArray","cJSON_CreateArrayReference","cJSON_Compare","cJSON_CreateStringArray","cJSON_Compare","cJSON_GetArrayItem","cJSON_GetStringValue","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Parse the input JSON data
    cJSON *root = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!root) {
        return 0;
    }

    // Get the "text" object from the parsed JSON
    cJSON *text_obj = cJSON_GetObjectItemCaseSensitive(root, "text");
    if (!text_obj) {
        cJSON_Delete(root);
        return 0;
    }

    // Get the "data" string value from the text object
    cJSON *data_item = cJSON_GetObjectItemCaseSensitive(text_obj, "data");
    const char *data_string = cJSON_GetStringValue(data_item);
    if (!data_string) {
        cJSON_Delete(root);
        return 0;
    }

    // Create a string array using the string values from the JSON
    const char *strings[] = {
        data_string,
        "Click Here",  // Expected value from input
        "Test String"
    };
    int count = sizeof(strings) / sizeof(strings[0]);
    
    // Create a string array using cJSON_CreateStringArray
    cJSON *string_array = cJSON_CreateStringArray(strings, count);
    if (!string_array) {
        cJSON_Delete(root);
        return 0;
    }

    // Create an array reference using cJSON_CreateArrayReference
    cJSON *array_reference = cJSON_CreateArrayReference(string_array);
    if (!array_reference) {
        cJSON_Delete(string_array);
        cJSON_Delete(root);
        return 0;
    }

    // Compare the original array with its reference
    // They should be identical but are different objects
    cJSON_bool are_identical = cJSON_Compare(string_array, array_reference, true);
    if (!are_identical) {
        // Arrays should be identical but references may differ in implementation
        // Continue execution for fuzzing purposes
    }

    // Compare string array with a simple array to test comparison
    cJSON *simple_array = cJSON_CreateStringArray(strings, 1);
    cJSON_bool are_different = cJSON_Compare(string_array, simple_array, true);

    // Get string value from array item
    cJSON *first_array_item = cJSON_GetArrayItem(string_array, 0);
    const char *first_string = cJSON_GetStringValue(first_array_item);

    // Print the string array in unformatted JSON using cJSON_PrintUnformatted
    char *unformatted_json = cJSON_PrintUnformatted(string_array);
    if (unformatted_json) {
        // For fuzzing purposes, we can write this to a file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(unformatted_json, strlen(unformatted_json), 1, out_file);
            int fd = fuzz_fileno(out_file);
            (void)fd; // Use fd to avoid unused variable warning
            assert_file_closed(&out_file);;
        }
        cJSON_free(unformatted_json);
    }

    // Clean up all allocated resources
    cJSON_Delete(simple_array);
    cJSON_Delete(array_reference);  // This doesn't delete string_array
    cJSON_Delete(string_array);
    cJSON_Delete(root);

    return 0;
}