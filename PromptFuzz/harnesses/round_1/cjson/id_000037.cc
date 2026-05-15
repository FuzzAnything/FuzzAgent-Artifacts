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
//<ID> 166
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["cJSON_ParseWithLength","cJSON_Delete","cJSON_Parse","cJSON_Delete","cJSON_Compare","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsRaw","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArrayItem","cJSON_CreateRaw","cJSON_InsertItemInArray","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_GetStringValue","cJSON_CreateString","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_Delete","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_Parse","cJSON_Compare","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsRaw","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArrayItem","cJSON_CreateRaw","cJSON_InsertItemInArray","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_GetStringValue","cJSON_CreateString","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_Delete","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse input using cJSON_ParseWithLength
    cJSON *parsed_with_length = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!parsed_with_length) {
        return 0; // Invalid input, exit early
    }

    // 2. Parse the same data using cJSON_Parse (requires null-terminated string)
    // We need to create a null-terminated copy since data might not be null-terminated
    char *null_terminated_data = static_cast<char*>(malloc(size + 1));
    if (!null_terminated_data) {
        cJSON_Delete(parsed_with_length);
        return 0;
    }
    memcpy(null_terminated_data, data, size);
    null_terminated_data[size] = '\0';
    
    cJSON *parsed_simple = cJSON_Parse(null_terminated_data);
    free(null_terminated_data);
    
    if (!parsed_simple) {
        cJSON_Delete(parsed_with_length);
        return 0;
    }

    // 3. Compare the two parsed objects
    cJSON_bool compare_result = cJSON_Compare(parsed_with_length, parsed_simple, true);

    // 4. Check if the "Image.Thumbnail.Width" field is raw (it should be string in the example)
    cJSON *image = cJSON_GetObjectItemCaseSensitive(parsed_with_length, "Image");
    if (image && cJSON_IsObject(image)) {
        cJSON *thumbnail = cJSON_GetObjectItemCaseSensitive(image, "Thumbnail");
        if (thumbnail && cJSON_IsObject(thumbnail)) {
            cJSON *width = cJSON_GetObjectItemCaseSensitive(thumbnail, "Width");
            if (width) {
                cJSON_bool is_raw = cJSON_IsRaw(width);
                (void)is_raw; // Prevent unused warning
            }
        }
    }

    // 5. Insert a new item into the "IDs" array at index 0
    if (image) {
        cJSON *ids = cJSON_GetObjectItemCaseSensitive(image, "IDs");
        if (ids && cJSON_IsArray(ids)) {
            // Get array item at index 0
            cJSON *first_id = cJSON_GetArrayItem(ids, 0);
            if (first_id) {
                // Create new raw item to insert
                cJSON *new_raw = cJSON_CreateRaw("\"inserted_raw\"");
                // Insert at position 0
                cJSON_bool insert_result = cJSON_InsertItemInArray(ids, 0, new_raw);
                if (!insert_result) {
                    cJSON_Delete(new_raw);
                }
            }
        }
    }

    // 6. Replace "Image.Title" with a new item
    if (image) {
        cJSON *old_title = cJSON_GetObjectItemCaseSensitive(image, "Title");
        if (old_title) {
            // Get old string value
            char *old_title_value = cJSON_GetStringValue(old_title);
            (void)old_title_value; // Prevent unused warning
            
            // Create new string item
            cJSON *new_title = cJSON_CreateString("Replaced Title");
            // Replace using case-sensitive function
            cJSON_bool replace_result = cJSON_ReplaceItemInObjectCaseSensitive(image, "Title", new_title);
            if (!replace_result) {
                cJSON_Delete(new_title);
            }
        }
    }

    // 7. Print unformatted JSON of the modified object
    char *printed_json = cJSON_PrintUnformatted(parsed_with_length);
    if (printed_json) {
        // We could write to a file, but for fuzzing we just free it
        cJSON_free(printed_json);
    }

    // 8. Cleanup
    cJSON_Delete(parsed_with_length);
    cJSON_Delete(parsed_simple);

    return 0;
}