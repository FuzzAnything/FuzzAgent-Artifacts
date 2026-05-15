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
//<ID> 200
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 1
//<Quality> {"density":21,"unique_branches":{"cJSON_IsNull":[[3014,9,3014,21,0,0,4,0]]},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_CreateObject","cJSON_Delete","cJSON_AddItemToObject","cJSON_HasObjectItem","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_CreateRaw","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsNull","cJSON_CreateRaw","cJSON_ReplaceItemInObject","cJSON_GetObjectItem","cJSON_IsRaw","cJSON_DetachItemFromObject","cJSON_Delete","cJSON_Print","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_CreateObject","cJSON_AddItemToObject","cJSON_HasObjectItem","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_CreateRaw","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsNull","cJSON_CreateRaw","cJSON_ReplaceItemInObject","cJSON_GetObjectItem","cJSON_IsRaw","cJSON_DetachItemFromObject","cJSON_Delete","cJSON_Print","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse input data as JSON string
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
	return 0;
	}
	
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    // Parse with length opts
    const char *parse_end = nullptr;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_str, size, &parse_end, true);
    free(json_str);
    
    if (!parsed) {
        return 0;
    }
    
    // 2. Create a new object to work with
    cJSON *root_object = cJSON_CreateObject();
    if (!root_object) {
        cJSON_Delete(parsed);
        return 0;
    }
    
    // 3. Add parsed array to object with key "days"
    cJSON_AddItemToObject(root_object, "days", parsed);
    
    // 4. Use cJSON_HasObjectItem - check if object has "days"
    if (cJSON_HasObjectItem(root_object, "days")) {
        // 5. Create true value and add to object
        cJSON *true_item = cJSON_CreateTrue();
        cJSON_AddItemToObject(root_object, "has_days", true_item);
        
        // 6. Create int array and add to object
        int numbers[] = {1, 2, 3, 4, 5, 6, 7};
        cJSON *int_array = cJSON_CreateIntArray(numbers, 7);
        cJSON_AddItemToObject(root_object, "indices", int_array);
        
        // 7. Create raw JSON and add to object
        cJSON *raw_item = cJSON_CreateRaw("\"raw_data\"");
        cJSON_AddItemToObject(root_object, "raw_data", raw_item);
        
        // 8. Check if null item exists
        cJSON *null_check = cJSON_GetObjectItem(root_object, "non_existent");
        cJSON_bool is_null = cJSON_IsNull(null_check); // Should be true (cJSON_IsNull handles NULL input)
        
        // 9. Replace raw item with new raw item
        cJSON *new_raw = cJSON_CreateRaw("\"new_raw\"");
        cJSON_ReplaceItemInObject(root_object, "raw_data", new_raw);
        
        // 10. Check if replaced item is raw
        cJSON *replaced_item = cJSON_GetObjectItem(root_object, "raw_data");
        cJSON_bool is_raw = cJSON_IsRaw(replaced_item);
        
        // 11. Detach int array from object
        cJSON *detached_array = cJSON_DetachItemFromObject(root_object, "indices");
        if (detached_array) {
            cJSON_Delete(detached_array);
        }
        
        // 12. Create output file and write formatted JSON
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            char *output = cJSON_Print(root_object);
            if (output) {
                fwrite(output, 1, strlen(output), out_file);
                free(output);
            }
            assert_file_closed(&out_file);;
        }
        
        // 13. Try using input file descriptor
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            // Read and discard first byte to demonstrate file usage
            int fd = fuzz_fileno(in_file);
            (void)fd; // Use fd to avoid unused variable warning
            char first_byte;
            fread(&first_byte, 1, 1, in_file);
            assert_file_closed(&in_file);;
        }
    }
    
    // Clean up
    cJSON_Delete(root_object);
    
    return 0;
}