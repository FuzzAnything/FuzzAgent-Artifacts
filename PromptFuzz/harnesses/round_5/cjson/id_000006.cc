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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_Version","cJSON_IsArray","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_IsRaw","cJSON_Delete","cJSON_CreateObject","cJSON_AddBoolToObject","cJSON_AddRawToObject","cJSON_CreateNull","cJSON_AddItemToObject","cJSON_PrintBuffered","cJSON_Minify","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_Version","cJSON_IsArray","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_IsRaw","cJSON_Delete","cJSON_CreateObject","cJSON_AddBoolToObject","cJSON_AddRawToObject","cJSON_CreateNull","cJSON_AddItemToObject","cJSON_PrintBuffered","cJSON_Minify","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


/* Include cJSON header */

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    /* Consume input data as JSON string */
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    /* Read entire file content */
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *json_str = (char *)malloc(file_size + 1);
    if (!json_str) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t read_size = fread(json_str, 1, file_size, in_file);
    json_str[read_size] = '\0';
    assert_file_closed(&in_file);;
    
    /* Parse JSON */
    cJSON *root = cJSON_Parse(json_str);
    free(json_str);
    
    if (!root) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    /* API 1: Get cJSON version */
    const char *version = cJSON_Version();
    /* Can optionally use version string */
    
    /* API 2: Check if root is an array and detach item if possible */
    if (cJSON_IsArray(root)) {
        int array_size = cJSON_GetArraySize(root);
        if (array_size > 0) {
            /* Detach first item from array */
            cJSON *detached_item = cJSON_DetachItemFromArray(root, 0);
            if (detached_item) {
                /* API 3: Check if detached item is raw */
                cJSON_bool is_raw = cJSON_IsRaw(detached_item);
                (void)is_raw; /* Use variable to avoid unused warning */
                cJSON_Delete(detached_item);
            }
        }
    }
    
    /* Create a new object to test remaining APIs */
    cJSON *test_object = cJSON_CreateObject();
    if (test_object) {
        /* API 4: Add boolean to object */
        cJSON_AddBoolToObject(test_object, "fuzz_bool", 1);
        
        /* API 5: Add raw string to object */
        cJSON_AddRawToObject(test_object, "fuzz_raw", "{\"raw\":\"data\"}");
        
        /* API 6: Create null item */
        cJSON *null_item = cJSON_CreateNull();
        if (null_item) {
            cJSON_AddItemToObject(test_object, "fuzz_null", null_item);
        }
        
        /* API 7: Print JSON with buffering */
        char *printed = cJSON_PrintBuffered(test_object, 1024, true);
        if (printed) {
            /* API 8: Minify the printed JSON */
            cJSON_Minify(printed);
            free(printed);
        }
        
        cJSON_Delete(test_object);
    }
    
    /* Clean up */
    cJSON_Delete(root);
    
    assert_file_closed(&in_file);
	return 0;
}