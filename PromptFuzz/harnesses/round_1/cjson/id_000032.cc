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
//<ID> 115
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"cJSON_IsBool":[[3005,9,3005,21,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_CreateRaw","cJSON_AddItemToObject","cJSON_AddFalseToObject","cJSON_AddNumberToObject","cJSON_AddObjectToObject","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsBool","cJSON_IsObject","cJSON_CreateBool","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_HasObjectItem","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_CreateStringArray","cJSON_AddItemToObject","cJSON_Duplicate","cJSON_Compare","cJSON_PrintUnformatted","cJSON_Minify","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_CreateRaw","cJSON_AddItemToObject","cJSON_AddFalseToObject","cJSON_AddNumberToObject","cJSON_AddObjectToObject","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsBool","cJSON_IsObject","cJSON_CreateBool","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_HasObjectItem","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_CreateStringArray","cJSON_AddItemToObject","cJSON_Duplicate","cJSON_Compare","cJSON_PrintUnformatted","cJSON_Minify","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse input JSON using cJSON_ParseWithLength
    cJSON *parsed_json = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    
    // 2. Create a new JSON object
    cJSON *root = cJSON_CreateObject();
    
    // 3. Create and add a raw JSON item
    const char *raw_data = "{\"test\":123}";
    cJSON *raw_item = cJSON_CreateRaw(raw_data);
    cJSON_AddItemToObject(root, "raw_item", raw_item);
    
    // 4. Add false boolean to object
    cJSON_AddFalseToObject(root, "false_item");
    
    // 5. Add number to object
    cJSON_AddNumberToObject(root, "number_item", 42.5);
    
    // 6. Add nested object to object
    cJSON *nested_obj = cJSON_AddObjectToObject(root, "nested_object");
    
    // 7. Get item from parsed JSON if it exists
    cJSON *parsed_item = NULL;
    if (parsed_json && cJSON_IsObject(parsed_json)) {
        parsed_item = cJSON_GetObjectItemCaseSensitive(parsed_json, "key");
    }
    
    // 8. Check if parsed item is boolean
    cJSON_bool is_bool = cJSON_IsBool(parsed_item);
    
    // 9. Check if parsed item is object
    cJSON_bool is_object = cJSON_IsObject(parsed_item);
    
    // 10. Create boolean item
    cJSON *bool_item = cJSON_CreateBool(1);
    
    // 11. Detach item from object
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "false_item");
    
    // 12. Delete detached item
    cJSON_Delete(detached);
    
    // 13. Check if object has item
    cJSON_bool has_item = cJSON_HasObjectItem(root, "number_item");
    
    // 14. Create double array
    double double_array[] = {1.1, 2.2, 3.3};
    cJSON *double_arr = cJSON_CreateDoubleArray(double_array, 3);
    cJSON_AddItemToObject(root, "double_array", double_arr);
    
    // 15. Create string array
    const char *string_array[] = {"str1", "str2", "str3"};
    cJSON *string_arr = cJSON_CreateStringArray(string_array, 3);
    cJSON_AddItemToObject(root, "string_array", string_arr);
    
    // 16. Duplicate JSON object
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    
    // 17. Compare original and duplicate
    cJSON_bool are_equal = cJSON_Compare(root, duplicate, 1);
    
    // 18. Print JSON unformatted
    char *json_str = cJSON_PrintUnformatted(root);
    
    // 19. Minify JSON string
    if (json_str) {
        cJSON_Minify(json_str);
    }
    
    // 20. Print to preallocated buffer
    const int buffer_size = 2048;
    char *buffer = reinterpret_cast<char*>(malloc(buffer_size));
    if (buffer) {
        cJSON_PrintPreallocated(root, buffer, buffer_size, 0);
        free(buffer);
    }
    
    // Cleanup
    if (json_str) {
        free(json_str);
    }
    cJSON_Delete(duplicate);
    cJSON_Delete(bool_item);
    cJSON_Delete(root);
    if (parsed_json) {
        cJSON_Delete(parsed_json);
    }
    
    // Create FILE* for input data (even if not used, to demonstrate the pattern)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        assert_file_closed(&in_file);;
    }
    
    // Create FILE* for output
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int fd = fuzz_fileno(out_file);
        assert_file_closed(&out_file);;
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}