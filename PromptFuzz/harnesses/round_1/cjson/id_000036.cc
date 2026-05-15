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
//<ID> 165
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 0
//<Quality> {"density":27,"unique_branches":{},"library_calls":["cJSON_ParseWithLength","cJSON_PrintUnformatted","cJSON_Delete","cJSON_Parse","cJSON_free","cJSON_Delete","cJSON_Compare","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_CreateNumber","cJSON_InsertItemInArray","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_GetStringValue","cJSON_GetObjectItemCaseSensitive","cJSON_IsRaw","cJSON_IsString","cJSON_CreateNumber","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_CreateRaw","cJSON_IsRaw","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_ParseWithLength","cJSON_PrintUnformatted","cJSON_Parse","cJSON_Compare","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_CreateNumber","cJSON_InsertItemInArray","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_GetStringValue","cJSON_GetObjectItemCaseSensitive","cJSON_IsRaw","cJSON_IsString","cJSON_CreateNumber","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_CreateRaw","cJSON_IsRaw","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse input using cJSON_ParseWithLength
    cJSON *parsed_json = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!parsed_json) {
        // Input might be invalid JSON, early exit
        return 0;
    }

    // 2. Get string representation using cJSON_PrintUnformatted
    char *json_string = cJSON_PrintUnformatted(parsed_json);
    if (!json_string) {
        cJSON_Delete(parsed_json);
        return 0;
    }

    // 3. Parse the string again using cJSON_Parse
    cJSON *reparsed_json = cJSON_Parse(json_string);
    if (!reparsed_json) {
        cJSON_free(json_string);
        cJSON_Delete(parsed_json);
        return 0;
    }

    // 4. Compare the two parsed JSON objects
    cJSON_bool are_equal = cJSON_Compare(parsed_json, reparsed_json, 1); // case-sensitive

    // 5. Work with arrays if present
    cJSON *image = cJSON_GetObjectItemCaseSensitive(parsed_json, "Image");
    if (image && cJSON_IsObject(image)) {
        cJSON *ids = cJSON_GetObjectItemCaseSensitive(image, "IDs");
        if (ids && cJSON_IsArray(ids)) {
            // Get array size and an item
            int array_size = cJSON_GetArraySize(ids);
            if (array_size > 0) {
                // Get first array item
                cJSON *first_item = cJSON_GetArrayItem(ids, 0);
                
                // Create new item to insert
                cJSON *new_item = cJSON_CreateNumber(999);
                if (new_item) {
                    // Insert at position 1 (second position)
                    cJSON_InsertItemInArray(ids, 1, new_item);
                }
            }
        }
    }

    // 6. Work with object replacement
    cJSON *thumbnail = cJSON_GetObjectItemCaseSensitive(image, "Thumbnail");
    if (thumbnail && cJSON_IsObject(thumbnail)) {
        // Get string value
        cJSON *url_item = cJSON_GetObjectItemCaseSensitive(thumbnail, "Url");
        if (url_item) {
            char *url_value = cJSON_GetStringValue(url_item);
            // URL value could be used here if needed
        }

        // Check if Width is raw (though it's a string in the input)
        cJSON *width_item = cJSON_GetObjectItemCaseSensitive(thumbnail, "Width");
        if (width_item) {
            cJSON_bool is_raw = cJSON_IsRaw(width_item);
            // For demonstration, replace with a number if it's not raw
            if (!is_raw && cJSON_IsString(width_item)) {
                cJSON *new_width = cJSON_CreateNumber(150.5);
                if (new_width) {
                    cJSON_ReplaceItemInObjectCaseSensitive(thumbnail, "Width", new_width);
                }
            }
        }
    }

    // 7. Create a separate JSON with raw type for testing cJSON_IsRaw
    cJSON *raw_test = cJSON_CreateRaw("raw data");
    if (raw_test) {
        cJSON_bool is_raw_check = cJSON_IsRaw(raw_test);
        cJSON_Delete(raw_test);
    }

    // 8. Cleanup
    cJSON_Delete(parsed_json);
    cJSON_Delete(reparsed_json);
    cJSON_free(json_string);

    return 0;
}