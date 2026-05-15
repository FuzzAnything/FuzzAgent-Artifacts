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
//<ID> 215
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"cJSON_SetValuestring":[[441,29,441,59,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_Delete","cJSON_AddRawToObject","cJSON_CreateTrue","cJSON_SetValuestring","cJSON_AddItemToObject","cJSON_IsFalse","cJSON_AddNullToObject","cJSON_Duplicate","cJSON_ReplaceItemInObject","cJSON_PrintUnformatted","cJSON_Minify","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_AddRawToObject","cJSON_CreateTrue","cJSON_SetValuestring","cJSON_AddItemToObject","cJSON_IsFalse","cJSON_AddNullToObject","cJSON_Duplicate","cJSON_ReplaceItemInObject","cJSON_PrintUnformatted","cJSON_Minify","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


// Include cJSON header

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<66) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    // Early exit if input is too small
    if (size < 2) {
	return 0;
	}
	
    
    // Create a null-terminated string from input data for parsing
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
	return 0;
	}
	
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    // Parse JSON from input data
    cJSON *parsed_json = cJSON_ParseWithLength(json_str, size);
    free(json_str);
    
    if (!parsed_json) {
	return 0;
	}
	
    
    // Create a new object for manipulation
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        cJSON_Delete(parsed_json);
        return 0;
    }
    
    // 1. Add raw JSON data to object
    char raw_data[] = "{\"temp\":\"value\"}";
    cJSON *raw_item = cJSON_AddRawToObject(root, fuzz_str_1, fuzz_str_2);
    
    // 2. Create and add a true value
    cJSON *true_item = cJSON_CreateTrue();
    if (true_item) {
        // 4. Try to set valuestring on true item (though unusual for boolean)
        cJSON_SetValuestring(true_item, fuzz_str_3);
        cJSON_AddItemToObject(root, fuzz_str_4, true_item);
    }
    
    // 3. Check if items are false
    cJSON_bool is_false = cJSON_IsFalse(true_item);
    
    // 5. Add null to object
    cJSON *null_item = cJSON_AddNullToObject(root, "nullField");
    
    // 6. Replace the null item with a new item from parsed JSON
    if (null_item && parsed_json->child) {
        cJSON *replacement = cJSON_Duplicate(parsed_json->child, fuzz_int32_t_5);
        if (replacement) {
            cJSON_ReplaceItemInObject(root, "nullField", replacement);
        }
    }
    
    // 7. Print unformatted JSON
    char *unformatted = cJSON_PrintUnformatted(root);
    if (unformatted) {
        // 8. Minify the unformatted JSON
        cJSON_Minify(unformatted);
        
        // 9. Print using preallocated buffer
        int buffer_size = strlen(unformatted) * 2 + 1;
        char *buffer = (char *)malloc(buffer_size);
        if (buffer) {
            cJSON_PrintPreallocated(root, buffer, buffer_size, fuzz_int32_t_6);
            free(buffer);
        }
        
        free(unformatted);
    }
    
    // Clean up
    cJSON_Delete(root);
    cJSON_Delete(parsed_json);
    
    return 0;
}