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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 2
//<Quality> {"density":22,"unique_branches":{"cJSON_Compare":[[3135,43,3135,62,0,0,4,1]],"get_object_item":[[1955,9,1955,25,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_Duplicate","cJSON_Delete","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_AddTrueToObject","cJSON_CreateStringReference","cJSON_AddItemToObjectCS","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_Compare","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_Duplicate","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_AddTrueToObject","cJSON_CreateStringReference","cJSON_AddItemToObjectCS","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArraySize","cJSON_DeleteItemFromArray","cJSON_Compare","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<179) return 0;

	
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
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
	//fuzzer shim end}




    // Parse input JSON data
    cJSON *parsed_json = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (parsed_json == nullptr) {
        return 0;
    }

    // Create a duplicate of the parsed JSON with recursion
    cJSON *duplicated_json = cJSON_Duplicate(parsed_json, fuzz_int32_t_1);
    if (duplicated_json == nullptr) {
        cJSON_Delete(parsed_json);
        return 0;
    }

    // Find the glossary object to work with
    cJSON *glossary = cJSON_GetObjectItem(duplicated_json, fuzz_str_2);
    if (glossary != nullptr) {
        cJSON *gloss_div = cJSON_GetObjectItem(glossary, fuzz_str_3);
        if (gloss_div != nullptr) {
            cJSON *gloss_list = cJSON_GetObjectItem(gloss_div, fuzz_str_4);
            if (gloss_list != nullptr) {
                cJSON *gloss_entry = cJSON_GetObjectItem(gloss_list, fuzz_str_5);
                if (gloss_entry != nullptr) {
                    // 1. Add a true value to the GlossEntry object
                    cJSON *added_true = cJSON_AddTrueToObject(gloss_entry, fuzz_str_6);
                    
                    // 2. Create a string reference and add it to GlossEntry with case-sensitive key
                    const char *ref_string = "reference_string";
                    cJSON *string_ref = cJSON_CreateStringReference(fuzz_str_7);
                    if (string_ref != nullptr) {
                        cJSON_AddItemToObjectCS(gloss_entry, fuzz_str_8, string_ref);
                    }
                }
            }
        }
    }

    // Find and modify an array if it exists
    cJSON *gloss_see_also = nullptr;
    cJSON *gloss_entry = cJSON_GetObjectItemCaseSensitive(
        cJSON_GetObjectItemCaseSensitive(
            cJSON_GetObjectItemCaseSensitive(
                cJSON_GetObjectItemCaseSensitive(duplicated_json, fuzz_str_12),
                fuzz_str_11),
            fuzz_str_10),
        fuzz_str_9);
    
    if (gloss_entry != nullptr) {
        cJSON *gloss_def = cJSON_GetObjectItemCaseSensitive(gloss_entry, fuzz_str_13);
        if (gloss_def != nullptr) {
            gloss_see_also = cJSON_GetObjectItemCaseSensitive(gloss_def, fuzz_str_14);
        }
    }

    // 3. Delete an item from the GlossSeeAlso array if it exists
    if (gloss_see_also != nullptr && cJSON_IsArray(gloss_see_also)) {
        int array_size = cJSON_GetArraySize(gloss_see_also);
        if (array_size > 0) {
            // Delete the first item from the array
            cJSON_DeleteItemFromArray(gloss_see_also, fuzz_int32_t_15);
        }
    }

    // 4. Compare the original and duplicated JSON (they should be different after modifications)
    cJSON_bool are_equal = cJSON_Compare(parsed_json, duplicated_json, fuzz_int32_t_16);

    // Clean up - delete both JSON objects
    cJSON_Delete(parsed_json);
    cJSON_Delete(duplicated_json);

    return 0;
}