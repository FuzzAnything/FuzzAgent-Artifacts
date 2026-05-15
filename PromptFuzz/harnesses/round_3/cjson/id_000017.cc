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
//<ID> 44
//<Prompt> []
/*<Combination>: [
*/
//<score> 58, nr_unique_branch: 1
//<Quality> {"density":29,"unique_branches":{"cJSON_Compare":[[3183,21,3183,73,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_Duplicate","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_AddTrueToObject","cJSON_CreateStringReference","cJSON_IsObject","cJSON_AddItemToObjectCS","cJSON_AddItemToObjectCS","cJSON_Compare","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_Duplicate","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_AddTrueToObject","cJSON_CreateStringReference","cJSON_IsObject","cJSON_AddItemToObjectCS","cJSON_Compare","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<153) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
	//fuzzer shim end}




    // Step 1: Parse the input JSON data
    cJSON *parsed = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!parsed) {
        // Input is not valid JSON, return early
        return 0;
    }
    
    // Step 2: Create a duplicate of the parsed JSON (with recursion)
    cJSON *duplicate = cJSON_Duplicate(parsed, fuzz_int32_t_1); // recurse = 1
    if (!duplicate) {
        cJSON_Delete(parsed);
        return 0;
    }
    
    // Step 3: Add a true value to an object in the duplicate
    // Find an object to add to - let's try to get the "glossary" object
    cJSON *glossary = cJSON_GetObjectItemCaseSensitive(duplicate, fuzz_str_2);
    if (glossary && cJSON_IsObject(glossary)) {
        cJSON_AddTrueToObject(glossary, fuzz_str_3);
    }
    
    // Step 4: Create a string reference and add it to an object
    cJSON *string_ref = cJSON_CreateStringReference(fuzz_str_4);
    if (string_ref) {
        // Add the string reference to the glossary object using case-sensitive addition
        if (glossary && cJSON_IsObject(glossary)) {
            cJSON_AddItemToObjectCS(glossary, fuzz_str_5, string_ref);
        } else {
            // If glossary doesn't exist, add to root
            cJSON_AddItemToObjectCS(duplicate, fuzz_str_6, string_ref);
        }
    }
    
    // Step 5: Compare the original and modified JSON
    cJSON_bool comparison_result = cJSON_Compare(parsed, duplicate, fuzz_int32_t_7); // case_sensitive = 1
    
    // Step 6: Delete an item from an array if one exists
    // Try to find the GlossSeeAlso array in the duplicate
    cJSON *gloss_see_also = NULL;
    cJSON *glossary_dup = cJSON_GetObjectItemCaseSensitive(duplicate, fuzz_str_8);
    if (glossary_dup && cJSON_IsObject(glossary_dup)) {
        cJSON *gloss_div = cJSON_GetObjectItemCaseSensitive(glossary_dup, fuzz_str_9);
        if (gloss_div && cJSON_IsObject(gloss_div)) {
            cJSON *gloss_list = cJSON_GetObjectItemCaseSensitive(gloss_div, fuzz_str_10);
            if (gloss_list && cJSON_IsObject(gloss_list)) {
                cJSON *gloss_entry = cJSON_GetObjectItemCaseSensitive(gloss_list, fuzz_str_11);
                if (gloss_entry && cJSON_IsObject(gloss_entry)) {
                    cJSON *gloss_def = cJSON_GetObjectItemCaseSensitive(gloss_entry, fuzz_str_12);
                    if (gloss_def && cJSON_IsObject(gloss_def)) {
                        gloss_see_also = cJSON_GetObjectItemCaseSensitive(gloss_def, fuzz_str_13);
                    }
                }
            }
        }
    }
    
    // If we found an array, delete the first item
    if (gloss_see_also && cJSON_IsArray(gloss_see_also)) {
        int array_size = cJSON_GetArraySize(gloss_see_also);
        if (array_size > 0) {
            cJSON_DeleteItemFromArray(gloss_see_also, fuzz_int32_t_14);
        }
    }
    
    // Additional step: Try to print the modified JSON to verify it's valid
    char *printed = cJSON_Print(duplicate);
    if (printed) {
        // Could write to a file if needed
        // FILE *out_file = fopen("output_file", "wb");
        // if (out_file) {
        //     fwrite(printed, 1, strlen(printed), out_file);
        //     assert_file_closed(&out_file);;
        // }
        cJSON_free(printed);
    }
    
    // Clean up all allocated resources
    cJSON_Delete(parsed);
    cJSON_Delete(duplicate);
    
    return 0;
}