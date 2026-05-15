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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 0
//<Quality> {"density":24,"unique_branches":{},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_Version","cJSON_CreateNull","cJSON_IsObject","cJSON_CreateObject","cJSON_Delete","cJSON_Delete","cJSON_AddBoolToObject","cJSON_AddRawToObject","cJSON_IsRaw","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_Duplicate","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_AddItemToObject","cJSON_PrintBuffered","cJSON_Minify","cJSON_Parse","cJSON_Delete","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_Version","cJSON_CreateNull","cJSON_CreateObject","cJSON_IsObject","cJSON_AddBoolToObject","cJSON_AddRawToObject","cJSON_IsRaw","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_Duplicate","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_AddItemToObject","cJSON_PrintBuffered","cJSON_Minify","cJSON_Parse","cJSON_Delete","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


// Include cJSON headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse the input JSON data
    cJSON *parsed_json = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (parsed_json == NULL) {
        // If parsing fails, we can still test other APIs
        // Continue with a newly created object instead
        parsed_json = cJSON_CreateObject();
        if (parsed_json == NULL) {
            return 0;  // Early exit if we can't create object
        }
    }
    
    // 2. Get cJSON version (using cJSON_Version)
    const char *version = cJSON_Version();
    (void)version;  // Avoid unused variable warning
    
    // 3. Create a null item (using cJSON_CreateNull)
    cJSON *null_item = cJSON_CreateNull();
    
    // 4. Add boolean to object (using cJSON_AddBoolToObject)
    // We'll add to the parsed JSON if it's an object, otherwise to a new object
    cJSON *target_object = cJSON_IsObject(parsed_json) ? parsed_json : cJSON_CreateObject();
    if (target_object != parsed_json && target_object == NULL) {
        cJSON_Delete(parsed_json);
        if (null_item) cJSON_Delete(null_item);
        return 0;
    }
    
    cJSON *bool_item = cJSON_AddBoolToObject(target_object, "fuzzer_bool", size % 2);
    
    // 5. Add raw JSON to object (using cJSON_AddRawToObject)
    const char *raw_json = "{\"raw\": \"data\"}";
    cJSON *raw_item = cJSON_AddRawToObject(target_object, "fuzzer_raw", raw_json);
    
    // 6. Check if raw item is raw (using cJSON_IsRaw)
    if (raw_item) {
        cJSON_bool is_raw = cJSON_IsRaw(raw_item);
        (void)is_raw;  // Avoid unused variable warning
    }
    
    // 7. Handle arrays - create an array and detach an item
    cJSON *array = cJSON_CreateArray();
    if (array) {
        // Add some items to the array
        cJSON_AddItemToArray(array, cJSON_CreateNumber(1.0));
        cJSON_AddItemToArray(array, cJSON_CreateString("test"));
        if (bool_item) {
            cJSON_AddItemToArray(array, cJSON_Duplicate(bool_item, 1));
        }
        
        // Detach an item from array (using cJSON_DetachItemFromArray)
        if (cJSON_GetArraySize(array) > 0) {
            cJSON *detached = cJSON_DetachItemFromArray(array, 0);
            if (detached) {
                cJSON_Delete(detached);
            }
        }
        
        // Add array to target object
        cJSON_AddItemToObject(target_object, "fuzzer_array", array);
    }
    
    // 8. Print JSON with buffering (using cJSON_PrintBuffered)
    char *printed_json = cJSON_PrintBuffered(target_object, 1024, 1);
    if (printed_json) {
        // 9. Minify the JSON (using cJSON_Minify)
        cJSON_Minify(printed_json);
        
        // Parse the minified version to verify it's still valid
        cJSON *minified_parsed = cJSON_Parse(printed_json);
        if (minified_parsed) {
            cJSON_Delete(minified_parsed);
        }
        
        cJSON_free(printed_json);
    }
    
    // 10. Clean up
    if (null_item) {
        cJSON_Delete(null_item);
    }
    
    // Only delete target_object if it's different from parsed_json
    if (target_object != parsed_json) {
        cJSON_Delete(target_object);
    }
    
    cJSON_Delete(parsed_json);
    
    return 0;
}