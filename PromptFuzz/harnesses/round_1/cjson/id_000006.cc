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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 16.5, nr_unique_branch: 30
//<Quality> {"density":11,"unique_branches":{"cJSON_CreateStringReference":[[2551,9,2551,21,0,0,4,0]],"cJSON_Compare":[[3082,9,3082,25,0,0,4,0],[3083,9,3083,24,0,0,4,0],[3084,9,3084,24,0,0,4,0],[3085,9,3085,26,0,0,4,0],[3089,9,3089,26,0,0,4,0],[3105,9,3105,25,0,0,4,0],[3106,9,3106,24,0,0,4,0],[3107,9,3107,24,0,0,4,0],[3110,9,3110,26,0,0,4,0],[3111,17,3111,63,0,0,4,0],[3111,17,3111,63,0,0,4,1],[3123,17,3123,60,0,0,4,1],[3135,20,3135,39,0,0,4,1],[3147,17,3147,39,0,0,4,1],[3154,9,3154,26,0,0,4,0],[3162,21,3162,38,0,0,4,0],[3162,21,3162,38,0,0,4,1],[3167,21,3167,73,0,0,4,0],[3167,21,3167,73,0,0,4,1],[3178,21,3178,38,0,0,4,0],[3178,21,3178,38,0,0,4,1],[3183,21,3183,73,0,0,4,0],[3183,21,3183,73,0,0,4,1],[296,58,296,73,29,0,4,0],[296,99,296,114,29,0,4,0],[296,99,296,114,29,0,4,1],[296,58,296,73,32,0,4,0],[296,99,296,114,32,0,4,0],[296,99,296,114,32,0,4,1]]},"library_calls":["cJSON_ParseWithLength","cJSON_PrintUnformatted","cJSON_Delete","cJSON_Parse","cJSON_CreateStringArray","cJSON_CreateArrayReference","cJSON_GetStringValue","cJSON_GetArrayItem","cJSON_CreateStringReference","cJSON_Compare","cJSON_Compare","cJSON_Compare","cJSON_GetArrayItem","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_PrintUnformatted","cJSON_Parse","cJSON_CreateStringArray","cJSON_CreateArrayReference","cJSON_GetStringValue","cJSON_GetArrayItem","cJSON_CreateStringReference","cJSON_Compare","cJSON_Compare","cJSON_Compare","cJSON_GetArrayItem","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":1}
/*Here's a step-by-step C++ fuzz driver that achieves the specified event using the required cJSON APIs:

*/


// Include cJSON headers

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Read the entire input into a buffer
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *buffer = (char *)malloc(file_size + 1);
    if (!buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    fread(buffer, 1, file_size, in_file);
    buffer[file_size] = '\0';
    assert_file_closed(&in_file);;
    
    // Step 1: Parse the input JSON data
    cJSON *root = cJSON_ParseWithLength(buffer, file_size);
    free(buffer);
    
    if (!root) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Step 2: Create an unformatted string representation
    char *unformatted_json = cJSON_PrintUnformatted(root);
    if (!unformatted_json) {
        cJSON_Delete(root);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: Parse the unformatted string back to cJSON
    cJSON *parsed_unformatted = cJSON_Parse(unformatted_json);
    
    // Step 4: Create a string array
    const char *strings[] = {"item1", "item2", "item3"};
    cJSON *string_array = cJSON_CreateStringArray(strings, 3);
    
    // Step 5: Create an array reference
    cJSON *array_ref = cJSON_CreateArrayReference(string_array);
    
    // Step 6: Get string values for comparison
    const char *original_string = cJSON_GetStringValue(cJSON_GetArrayItem(string_array, 0));
    
    // Create a copy of the first item for comparison
    cJSON *first_item_copy = cJSON_CreateStringReference(original_string);
    
    // Step 7: Perform comparison
    // Compare root with parsed_unformatted (should be equal if parsing is consistent)
    cJSON_bool compare_result1 = cJSON_Compare(root, parsed_unformatted, false);
    
    // Compare string array with array reference (should be equal)
    cJSON_bool compare_result2 = cJSON_Compare(string_array, array_ref, false);
    
    // Compare first item with its copy
    cJSON_bool compare_result3 = cJSON_Compare(
        cJSON_GetArrayItem(string_array, 0), 
        first_item_copy, 
        false
    );
    
    // Event: Demonstrate that unformatted JSON parsing, string array creation,
    // array referencing, and string value retrieval all work together to
    // maintain data consistency and enable comparison operations
    
    // Write some debug output to a file if needed
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        fprintf(out_file, "Comparison results: %d, %d, %d\n", 
                compare_result1, compare_result2, compare_result3);
        if (unformatted_json) {
            fprintf(out_file, "Unformatted JSON length: %zu\n", strlen(unformatted_json));
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up all allocated resources
    if (unformatted_json) cJSON_free(unformatted_json);
    if (root) cJSON_Delete(root);
    if (parsed_unformatted) cJSON_Delete(parsed_unformatted);
    if (string_array) cJSON_Delete(string_array);
    if (array_ref) cJSON_Delete(array_ref);
    if (first_item_copy) cJSON_Delete(first_item_copy);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}