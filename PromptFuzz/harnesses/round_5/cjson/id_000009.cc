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
//<ID> 19
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 6
//<Quality> {"density":10,"unique_branches":{"cJSON_ParseWithLengthOpts":[[1179,9,1179,32,0,0,4,0],[1182,13,1182,45,0,0,4,1],[1182,49,1182,85,0,0,4,0],[1182,49,1182,85,0,0,4,1],[1187,9,1187,25,0,0,4,0],[1215,13,1215,37,0,0,4,0]]},"library_calls":["cJSON_Version","cJSON_ParseWithOpts","cJSON_CreateObject","cJSON_Delete","cJSON_CreateNull","cJSON_AddItemToObject","cJSON_AddBoolToObject","cJSON_AddRawToObject","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_IsRaw","cJSON_Delete","cJSON_Delete","cJSON_Print","cJSON_Minify","cJSON_PrintBuffered","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Version","cJSON_ParseWithOpts","cJSON_CreateObject","cJSON_CreateNull","cJSON_AddItemToObject","cJSON_AddBoolToObject","cJSON_AddRawToObject","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_IsRaw","cJSON_Delete","cJSON_Delete","cJSON_Print","cJSON_Minify","cJSON_PrintBuffered","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


// Include cJSON header
extern "C" {
    #include "cJSON.h"
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a null-terminated string from the input data
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
	return 0;
	}
	
    memcpy(json_str, data, size);
    json_str[size] = '\0';

    // Get cJSON version (first required API)
    const char *version = cJSON_Version();

    // Parse JSON with options
    const char *parse_end = nullptr;
    cJSON *parsed_json = cJSON_ParseWithOpts(json_str, &parse_end, 1);
    free(json_str);
    if (!parsed_json) {
	return 0;
	}
	

    // Create a new object to demonstrate various APIs
    cJSON *new_obj = cJSON_CreateObject();
    if (!new_obj) {
        cJSON_Delete(parsed_json);
        return 0;
    }

    // Add a null item using cJSON_CreateNull() (second required API)
    cJSON *null_item = cJSON_CreateNull();
    if (null_item) {
        cJSON_AddItemToObject(new_obj, "fuzzer_null", null_item);
    }

    // Add a boolean item (third required API)
    cJSON_AddBoolToObject(new_obj, "fuzzer_bool", 1);

    // Add a raw item (fourth required API)
    cJSON_AddRawToObject(new_obj, "fuzzer_raw", "[1,2,3]");

    // Check if the parsed JSON has the "widget" object
    cJSON *widget = cJSON_GetObjectItem(parsed_json, "widget");
    if (widget) {
        // Try to get the "text" object which might contain an array-like structure
        cJSON *text = cJSON_GetObjectItem(widget, "text");
        if (text && cJSON_IsObject(text)) {
            // Create a temporary array to demonstrate detachment
            cJSON *temp_array = cJSON_CreateArray();
            if (temp_array) {
                // Add some items to the array
                cJSON_AddItemToArray(temp_array, cJSON_CreateNumber(42));
                cJSON_AddItemToArray(temp_array, cJSON_CreateString("test"));
                
                // Detach an item from array (fifth required API)
                if (cJSON_GetArraySize(temp_array) > 0) {
                    cJSON *detached = cJSON_DetachItemFromArray(temp_array, 0);
                    if (detached) {
                        // Check if detached item is raw (sixth required API)
                        // Note: This will likely return false since we added a number
                        cJSON_bool is_raw = cJSON_IsRaw(detached);
                        (void)is_raw; // Mark as used
                        cJSON_Delete(detached);
                    }
                }
                
                // Clean up the temp array
                cJSON_Delete(temp_array);
            }
        }
    }

    // Create a minifiable JSON string
    char *json_to_minify = cJSON_Print(parsed_json);
    if (json_to_minify) {
        // Minify the JSON (seventh required API)
        cJSON_Minify(json_to_minify);
        free(json_to_minify);
    }

    // Use PrintBuffered (eighth required API)
    char *buffered_print = cJSON_PrintBuffered(new_obj, 1024, 1);
    if (buffered_print) {
        free(buffered_print);
    }

    // Clean up
    cJSON_Delete(parsed_json);
    cJSON_Delete(new_obj);

    return 0;
}