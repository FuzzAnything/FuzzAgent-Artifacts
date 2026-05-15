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
//<ID> 41
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"cJSON_Compare":[[3147,17,3147,39,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_Duplicate","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_AddTrueToObject","cJSON_CreateStringReference","cJSON_AddItemToObjectCS","cJSON_Compare","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_Duplicate","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_AddTrueToObject","cJSON_CreateStringReference","cJSON_AddItemToObjectCS","cJSON_Compare","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified cjson APIs with the provided input data:

*/


// Include cjson headers

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<130) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
	//fuzzer shim end}




    // Create a FILE pointer from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read the input data into a buffer
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *json_buffer = (char *)malloc(file_size + 1);
    if (!json_buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(json_buffer, 1, file_size, in_file);
    json_buffer[bytes_read] = '\0';
    assert_file_closed(&in_file);;
    
    // Parse the JSON data
    cJSON *root = cJSON_ParseWithLength(json_buffer, bytes_read);
    if (!root) {
        free(json_buffer);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a duplicate of the root object
    cJSON *duplicate = cJSON_Duplicate(root, fuzz_int32_t_1); // recurse = true
    if (!duplicate) {
        cJSON_Delete(root);
        free(json_buffer);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Navigate to the GlossDef object to access the GlossSeeAlso array
    cJSON *glossary = cJSON_GetObjectItemCaseSensitive(root, fuzz_str_2);
    if (glossary) {
        cJSON *glossDiv = cJSON_GetObjectItemCaseSensitive(glossary, fuzz_str_3);
        if (glossDiv) {
            cJSON *glossList = cJSON_GetObjectItemCaseSensitive(glossDiv, fuzz_str_4);
            if (glossList) {
                cJSON *glossEntry = cJSON_GetObjectItemCaseSensitive(glossList, fuzz_str_5);
                if (glossEntry) {
                    cJSON *glossDef = cJSON_GetObjectItemCaseSensitive(glossEntry, fuzz_str_6);
                    if (glossDef) {
                        cJSON *glossSeeAlso = cJSON_GetObjectItemCaseSensitive(glossDef, fuzz_str_7);
                        
                        // 1. Delete an item from the GlossSeeAlso array
                        if (glossSeeAlso && cJSON_IsArray(glossSeeAlso)) {
                            int array_size = cJSON_GetArraySize(glossSeeAlso);
                            if (array_size > 0) {
                                // Delete the first item from the array
                                cJSON_DeleteItemFromArray(glossSeeAlso, fuzz_int32_t_8);
                            }
                        }
                        
                        // 2. Add a true value to the GlossDef object
                        cJSON_AddTrueToObject(glossDef, fuzz_str_9);
                        
                        // 3. Create a string reference and add it to GlossDef
                        const char *ref_string = "fuzzed_reference";
                        cJSON *string_ref = cJSON_CreateStringReference(fuzz_str_10);
                        if (string_ref) {
                            // Add with case-sensitive key
                            cJSON_AddItemToObjectCS(glossDef, fuzz_str_11, string_ref);
                        }
                    }
                }
            }
        }
    }
    
    // 4. Compare the original and modified objects
    cJSON_bool are_equal = cJSON_Compare(root, duplicate, fuzz_int32_t_12); // case_sensitive = true
    
    // Create output file to write the modified JSON
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        char *modified_json = cJSON_Print(duplicate);
        if (modified_json) {
            fwrite(modified_json, 1, strlen(modified_json), out_file);
            cJSON_free(modified_json);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up all allocated resources
    cJSON_Delete(root);
    cJSON_Delete(duplicate);
    free(json_buffer);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}