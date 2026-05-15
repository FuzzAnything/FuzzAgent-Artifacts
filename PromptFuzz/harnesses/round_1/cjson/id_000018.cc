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
//<ID> 55
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 1
//<Quality> {"density":19,"unique_branches":{"cJSON_DetachItemViaPointer":[[2281,14,2281,32,0,0,4,0]]},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_ParseWithOpts","cJSON_Delete","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_IsBool","cJSON_IsRaw","cJSON_AddArrayToObject","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_DetachItemFromObject","cJSON_Delete","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_PrintUnformatted","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_ParseWithOpts","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_IsBool","cJSON_IsRaw","cJSON_AddArrayToObject","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_DetachItemFromObject","cJSON_Delete","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_PrintUnformatted","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


// Include cJSON headers

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Create a FILE* to read input data (as per instruction 4)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Read data from file into buffer
    char *json_data = (char *)malloc(size + 1);
    if (!json_data) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t read_size = fread(json_data, 1, size, in_file);
    json_data[read_size] = '\0';
    assert_file_closed(&in_file);;
    
    // API 1: Parse using cJSON_ParseWithLengthOpts
    const char *parse_end1;
    cJSON *json1 = cJSON_ParseWithLengthOpts(json_data, read_size, &parse_end1, false);
    
    // API 2: Parse using cJSON_ParseWithOpts
    const char *parse_end2;
    cJSON *json2 = cJSON_ParseWithOpts(json_data, &parse_end2, true);
    
    // Open output file (as per instruction 4)
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        free(json_data);
        if (json1) cJSON_Delete(json1);
        if (json2) cJSON_Delete(json2);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor (as per instruction 5)
    int out_fd = fuzz_fileno(out_file);
    (void)out_fd; // Use to avoid unused variable warning
    
    // Work with json1 if valid
    if (json1) {
        // API 3: Get and check boolean item
        cJSON *bool_item = cJSON_GetObjectItemCaseSensitive(json1, "menu");
        if (bool_item && cJSON_IsObject(bool_item)) {
            // API 4: Check if an item is boolean
            cJSON_IsBool(bool_item); // Will return false since it's an object
            
            // API 5: Check if an item is raw
            cJSON_IsRaw(bool_item); // Will return false
            
            // API 6: Add array to object
            cJSON *new_array = cJSON_AddArrayToObject(bool_item, "fuzzed_array");
            if (new_array) {
                // Add some items to the new array
                cJSON_AddItemToArray(new_array, cJSON_CreateNumber(42.0));
                cJSON_AddItemToArray(new_array, cJSON_CreateString("fuzz"));
            }
            
            // API 7: Detach item from object (case sensitive)
            cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(bool_item, "header");
            if (detached) {
                cJSON_Delete(detached);
            }
            
            // API 8: Detach item from object
            cJSON *detached2 = cJSON_DetachItemFromObject(bool_item, "items");
            if (detached2) {
                cJSON_Delete(detached2);
            }
            
            // API 9: Delete item from object (case sensitive)
            cJSON_DeleteItemFromObjectCaseSensitive(bool_item, "fuzzed_array");
        }
        
        // API 10: Print unformatted JSON
        char *printed = cJSON_PrintUnformatted(json1);
        if (printed) {
            fwrite(printed, 1, strlen(printed), out_file);
            free(printed);
        }
    }
    
    // Cleanup
    assert_file_closed(&out_file);;
    free(json_data);
    if (json1) cJSON_Delete(json1);
    if (json2) cJSON_Delete(json2);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	return 0;
}