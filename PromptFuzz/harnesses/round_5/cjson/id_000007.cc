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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["cJSON_Version","cJSON_ParseWithLength","cJSON_CreateNull","cJSON_AddBoolToObject","cJSON_AddRawToObject","cJSON_GetObjectItem","cJSON_IsRaw","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_PrintBuffered","cJSON_Minify","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Version","cJSON_ParseWithLength","cJSON_CreateNull","cJSON_AddBoolToObject","cJSON_AddRawToObject","cJSON_GetObjectItem","cJSON_IsRaw","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_PrintBuffered","cJSON_Minify","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get cJSON version
    const char *version = cJSON_Version();
    
    // 2. Parse input data
    cJSON *parsed_json = cJSON_ParseWithLength((const char*)data, size);
    if (parsed_json == NULL) {
        return 0; // Invalid JSON input
    }
    
    // 3. Create a null item
    cJSON *null_item = cJSON_CreateNull();
    
    // 4. Add boolean to object
    cJSON_AddBoolToObject(parsed_json, "fuzzed_bool", 1);
    
    // 5. Add raw string to object
    cJSON_AddRawToObject(parsed_json, "fuzzed_raw", "{\"raw_data\":\"value\"}");
    
    // 6. Check if an item is raw
    cJSON *raw_item = cJSON_GetObjectItem(parsed_json, "fuzzed_raw");
    if (raw_item != NULL) {
        cJSON_bool is_raw = cJSON_IsRaw(raw_item);
    }
    
    // 7. Create an array for detaching items
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToArray(array, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(array, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(array, cJSON_CreateNumber(3.0));
    
    // 8. Detach item from array
    cJSON *detached_item = cJSON_DetachItemFromArray(array, 1);
    if (detached_item != NULL) {
        cJSON_Delete(detached_item);
    }
    
    // 9. Print JSON with buffering
    char *printed_json = cJSON_PrintBuffered(parsed_json, 4096, 1);
    if (printed_json != NULL) {
        // 10. Minify the printed JSON
        cJSON_Minify(printed_json);
        cJSON_free(printed_json);
    }
    
    // 11. Cleanup
    cJSON_Delete(null_item);
    cJSON_Delete(array);
    cJSON_Delete(parsed_json);
    
    return 0;
}