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
//<ID> 185
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":27,"unique_branches":{"cJSON_IsString":[[3034,9,3034,21,0,0,4,0]]},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_Duplicate","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_AddNumberToObject","cJSON_AddBoolToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_CreateString","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_AddNumberToObject","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_malloc","cJSON_malloc","cJSON_CreateIntArray","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_PrintBuffered","cJSON_free","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_Duplicate","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_AddNumberToObject","cJSON_AddBoolToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_CreateString","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_AddNumberToObject","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_malloc","cJSON_malloc","cJSON_CreateIntArray","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_PrintBuffered","cJSON_free","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":2}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Parse input JSON using cJSON_ParseWithLengthOpts
    const char *parse_end = nullptr;
    cJSON *parsed = cJSON_ParseWithLengthOpts(
        reinterpret_cast<const char*>(data), 
        size, 
        &parse_end, 
        false
    );
    
    if (!parsed) {
        return 0;
    }

    // Duplicate the parsed object
    cJSON *duplicated = cJSON_Duplicate(parsed, 1);
    if (!duplicated) {
        cJSON_Delete(parsed);
        return 0;
    }

    // Get object item case sensitively
    cJSON *glossary_item = cJSON_GetObjectItemCaseSensitive(duplicated, "glossary");
    if (cJSON_IsObject(glossary_item)) {
        // Delete item from object case sensitively
        cJSON_DeleteItemFromObjectCaseSensitive(glossary_item, "title");
        
        // Add number and boolean to object
        cJSON_AddNumberToObject(glossary_item, "fuzz_number", 42.5);
        cJSON_AddBoolToObject(glossary_item, "fuzz_bool", 1);
        
        // Get and check nested items
        cJSON *gloss_div = cJSON_GetObjectItemCaseSensitive(glossary_item, "GlossDiv");
        if (cJSON_IsObject(gloss_div)) {
            cJSON *title = cJSON_GetObjectItemCaseSensitive(gloss_div, "title");
            if (cJSON_IsString(title)) {
                // Replace item in object case sensitively
                cJSON *new_string = cJSON_CreateString("FuzzedTitle");
                if (new_string) {
                    cJSON_ReplaceItemInObjectCaseSensitive(gloss_div, "title", new_string);
                }
            }
            
            // Check if a value is number and modify it
            cJSON *dummy_number = cJSON_GetObjectItemCaseSensitive(gloss_div, "fuzz_dummy");
            if (!dummy_number) {
                dummy_number = cJSON_AddNumberToObject(gloss_div, "fuzz_dummy", 0.0);
            }
            if (cJSON_IsNumber(dummy_number)) {
                cJSON_SetNumberHelper(dummy_number, 99.9);
            }
        }
    }

    // Create arrays using cJSON_malloc for raw data
    int int_array_data[] = {1, 2, 3, 4, 5};
    float float_array_data[] = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f};
    
    // Allocate memory using cJSON_malloc
    int *int_array_copy = static_cast<int*>(cJSON_malloc(sizeof(int_array_data)));
    float *float_array_copy = static_cast<float*>(cJSON_malloc(sizeof(float_array_data)));
    
    if (int_array_copy && float_array_copy) {
        memcpy(int_array_copy, int_array_data, sizeof(int_array_data));
        memcpy(float_array_copy, float_array_data, sizeof(float_array_data));
        
        // Create JSON arrays
        cJSON *int_array = cJSON_CreateIntArray(int_array_copy, 5);
        cJSON *float_array = cJSON_CreateFloatArray(float_array_copy, 5);
        
        if (int_array && float_array) {
            cJSON_AddItemToObject(duplicated, "fuzz_int_array", int_array);
            cJSON_AddItemToObject(duplicated, "fuzz_float_array", float_array);
        }
    }

    // Print the modified JSON using buffered printing
    char *printed = cJSON_PrintBuffered(duplicated, 2048, true);
    if (printed) {
        // Write to output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed, strlen(printed), 1, out_file);
            assert_file_closed(&out_file);;
        }
        free(printed);
    }

    // Clean up allocated memory
    if (int_array_copy) {
        cJSON_free(int_array_copy);
    }
    if (float_array_copy) {
        cJSON_free(float_array_copy);
    }
    
    // Delete JSON objects
    cJSON_Delete(duplicated);
    cJSON_Delete(parsed);

    return 0;
}