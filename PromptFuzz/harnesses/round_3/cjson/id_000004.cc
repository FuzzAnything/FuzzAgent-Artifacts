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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"cJSON_ReplaceItemViaPointer":[[2404,13,2404,38,0,0,4,1]]},"library_calls":["cJSON_Version","cJSON_ParseWithLength","cJSON_AddObjectToObject","cJSON_Delete","cJSON_AddStringToObject","cJSON_CreateBool","cJSON_CreateFalse","cJSON_CreateNull","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_IsNumber","cJSON_IsFalse","cJSON_GetObjectItem","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_ReplaceItemInObject","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Version","cJSON_ParseWithLength","cJSON_AddObjectToObject","cJSON_AddStringToObject","cJSON_CreateBool","cJSON_CreateFalse","cJSON_CreateNull","cJSON_IsNumber","cJSON_IsFalse","cJSON_GetObjectItem","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_ReplaceItemInObject","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<78) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
	//fuzzer shim end}




    // 1. Parse input JSON data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Read the entire input
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *json_string = (char *)malloc(file_size + 1);
    if (!json_string) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    fread(json_string, 1, file_size, in_file);
    json_string[file_size] = '\0';
    assert_file_closed(&in_file);;
    
    // 2. Get cJSON version (required API)
    const char *version = cJSON_Version();
    if (!version) {
        free(json_string);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Parse JSON
    cJSON *root = cJSON_ParseWithLength(json_string, file_size);
    if (!root) {
        free(json_string);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create a new object to add to the parsed JSON
    cJSON *new_object = cJSON_AddObjectToObject(root, fuzz_str_1);
    if (!new_object) {
        cJSON_Delete(root);
        free(json_string);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Add a string to the new object (required API)
    cJSON_AddStringToObject(new_object, fuzz_str_2, fuzz_str_3);
    
    // 6. Create boolean items (required APIs)
    cJSON *bool_item = cJSON_CreateBool(fuzz_int32_t_4);  // true
    cJSON *false_item = cJSON_CreateFalse(); // false
    cJSON *null_item = cJSON_CreateNull();   // null
    
    if (!bool_item || !false_item || !null_item) {
        if (bool_item) cJSON_Delete(bool_item);
        if (false_item) cJSON_Delete(false_item);
        if (null_item) cJSON_Delete(null_item);
        cJSON_Delete(root);
        free(json_string);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Check if certain items are numbers or false (required APIs)
    // Look for number items in the original JSON
    cJSON *current = NULL;
    cJSON_ArrayForEach(current, root) {
        if (cJSON_IsNumber(current)) {
            // Found a number item
            break;
        }
    }
    
    // Check if false_item is actually false (required API)
    if (cJSON_IsFalse(false_item)) {
        // This should always be true
    }
    
    // 8. Replace an item in the object (required API)
    // Find the first object item to replace
    cJSON *object_to_replace = cJSON_GetObjectItem(root, fuzz_str_5);
    if (object_to_replace) {
        // Create a replacement object
        cJSON *replacement = cJSON_CreateObject();
        if (replacement) {
            cJSON_AddStringToObject(replacement, fuzz_str_6, fuzz_str_7);
            cJSON_ReplaceItemInObject(root, "glossary", replacement);
        }
    }
    
    // 9. Clean up
    cJSON_Delete(bool_item);
    cJSON_Delete(false_item);
    cJSON_Delete(null_item);
    cJSON_Delete(root);
    free(json_string);
    
    assert_file_closed(&in_file);
	return 0;
}