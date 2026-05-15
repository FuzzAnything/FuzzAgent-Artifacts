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
//<ID> 56
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 0
//<Quality> {"density":28,"unique_branches":{},"library_calls":["cJSON_malloc","cJSON_ParseWithOpts","cJSON_ParseWithLengthOpts","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_DetachItemFromObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_IsBool","cJSON_IsRaw","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_AddArrayToObject","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_PrintUnformatted","cJSON_PrintUnformatted","cJSON_ParseWithOpts","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_Delete","cJSON_free","cJSON_ParseWithLengthOpts","cJSON_Delete","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_ParseWithOpts","cJSON_ParseWithLengthOpts","cJSON_DetachItemFromObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_IsBool","cJSON_IsRaw","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_AddArrayToObject","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_PrintUnformatted","cJSON_PrintUnformatted","cJSON_ParseWithOpts","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_Delete","cJSON_free","cJSON_ParseWithLengthOpts","cJSON_Delete","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_free"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified cjson APIs to achieve the described event:

*/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a null-terminated string from the input data
    char *json_str = static_cast<char*>(cJSON_malloc(size + 1));
    if (!json_str) {
	return 0;
	}
	
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    // Parse the JSON using cJSON_ParseWithOpts
    const char *parse_end = nullptr;
    cJSON *root1 = cJSON_ParseWithOpts(json_str, &parse_end, false);
    
    // Also parse using cJSON_ParseWithLengthOpts
    const char *parse_end2 = nullptr;
    cJSON *root2 = cJSON_ParseWithLengthOpts(json_str, size, &parse_end2, false);
    
    if (!root1 || !root2) {
        cJSON_free(json_str);
        if (root1) cJSON_Delete(root1);
        if (root2) cJSON_Delete(root2);
        return 0;
    }
    
    // Try to detach "menu" object using different APIs
    cJSON *menu_detached = cJSON_DetachItemFromObject(root1, "menu");
    cJSON *menu_detached_case = cJSON_DetachItemFromObjectCaseSensitive(root2, "menu");
    
    // Check if detached items are of certain types
    if (menu_detached) {
        // Check if the item is a boolean (unlikely for "menu", but demonstrates API usage)
        cJSON_bool is_bool = cJSON_IsBool(menu_detached);
        
        // Check if the item is raw
        cJSON_bool is_raw = cJSON_IsRaw(menu_detached);
        
        // Delete from object case sensitive version
        cJSON_DeleteItemFromObjectCaseSensitive(root1, "header");  // Try to delete non-existent
    }
    
    // Add an array to object
    cJSON *new_array = cJSON_AddArrayToObject(root1, "fuzzer_array");
    if (new_array) {
        // Add some items to the array
        cJSON_AddItemToArray(new_array, cJSON_CreateNumber(1.0));
        cJSON_AddItemToArray(new_array, cJSON_CreateString("fuzz"));
    }
    
    // Create print unformatted versions
    char *unformatted1 = cJSON_PrintUnformatted(root1);
    char *unformatted2 = cJSON_PrintUnformatted(root2);
    
    // Try to parse the unformatted output again
    if (unformatted1) {
        const char *parse_end3 = nullptr;
        cJSON *root3 = cJSON_ParseWithOpts(unformatted1, &parse_end3, true);
        if (root3) {
            // Delete an item from object case sensitive
            cJSON_DeleteItemFromObjectCaseSensitive(root3, "fuzzer_array");
            cJSON_Delete(root3);
        }
        cJSON_free(unformatted1);
    }
    
    if (unformatted2) {
        const char *parse_end4 = nullptr;
        cJSON *root4 = cJSON_ParseWithLengthOpts(unformatted2, strlen(unformatted2), &parse_end4, true);
        if (root4) {
            cJSON_Delete(root4);
        }
        cJSON_free(unformatted2);
    }
    
    // Create a FILE* for reading data (not strictly needed but demonstrates usage)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        
        // Read a small amount to demonstrate file usage
        char buf[4];
        if (fread(buf, 1, sizeof(buf), in_file) > 0) {
            // Could use buf for something
        }
        assert_file_closed(&in_file);;
    }
    
    // Create a FILE* for writing (not strictly needed but demonstrates usage)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int fd = fuzz_fileno(out_file);
        if (unformatted1) {
            // Write something to demonstrate file usage
            fwrite("test", 1, 4, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up
    if (menu_detached) cJSON_Delete(menu_detached);
    if (menu_detached_case) cJSON_Delete(menu_detached_case);
    cJSON_Delete(root1);
    cJSON_Delete(root2);
    cJSON_free(json_str);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}