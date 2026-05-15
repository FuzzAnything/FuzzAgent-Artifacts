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
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 187, nr_unique_branch: 36
//<Quality> {"density":17,"unique_branches":{"cJSON_CreateBool":[[2497,8,2497,12,0,0,4,0],[2499,22,2499,29,0,0,4,0]],"cJSON_AddArrayToObject":[[2249,9,2249,70,0,0,4,0]],"cJSON_ReplaceItemViaPointer":[[2370,9,2370,25,0,0,4,1],[2370,29,2370,52,0,0,4,1],[2370,56,2370,77,0,0,4,1],[2370,81,2370,95,0,0,4,1],[2375,9,2375,28,0,0,4,1],[2383,9,2383,34,0,0,4,0],[2383,9,2383,34,0,0,4,1],[2387,9,2387,30,0,0,4,0],[2389,13,2389,49,0,0,4,0],[2389,13,2389,49,0,0,4,1]],"cJSON_AddBoolToObject":[[2189,9,2189,74,0,0,4,0]],"cJSON_CreateStringArray":[[2748,9,2748,20,0,0,4,1],[2748,24,2748,41,0,0,4,1],[2755,17,2755,18,0,0,4,0],[2755,22,2755,41,0,0,4,0],[2755,22,2755,41,0,0,4,1],[2758,12,2758,14,0,0,4,1],[2763,12,2763,14,0,0,4,0],[2763,12,2763,14,0,0,4,1],[2774,9,2774,10,0,0,4,0],[2774,14,2774,22,0,0,4,0]],"cJSON_AddRawToObject":[[2225,9,2225,73,0,0,4,0]],"cJSON_ReplaceItemInArray":[[2419,9,2419,18,0,0,4,1]],"cJSON_SetNumberHelper":[[413,9,413,23,0,0,4,1],[418,9,418,26,0,0,4,1],[422,14,422,39,0,0,4,1]],"replace_item_in_object":[[2429,9,2429,30,0,0,4,1],[2429,34,2429,50,0,0,4,1],[2435,9,2435,51,0,0,4,0],[2435,55,2435,84,0,0,4,1],[2440,9,2440,36,0,0,4,1]],"cJSON_CreateString":[[2534,8,2534,12,0,0,4,0],[2538,12,2538,30,0,0,4,1]]},"library_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_free","cJSON_IsArray","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_CreateObject","cJSON_CreateBool","cJSON_AddBoolToObject","cJSON_Delete","cJSON_CreateStringArray","cJSON_AddArrayToObject","cJSON_GetObjectItem","cJSON_AddItemToObject","cJSON_Delete","cJSON_IsArray","cJSON_GetArraySize","cJSON_CreateString","cJSON_ReplaceItemInArray","cJSON_CreateNumber","cJSON_ReplaceItemInObject","cJSON_AddRawToObject","cJSON_CreateString","cJSON_AddItemToObject","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_IsArray","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_CreateObject","cJSON_CreateBool","cJSON_AddBoolToObject","cJSON_Delete","cJSON_CreateStringArray","cJSON_AddArrayToObject","cJSON_GetObjectItem","cJSON_AddItemToObject","cJSON_IsArray","cJSON_GetArraySize","cJSON_CreateString","cJSON_ReplaceItemInArray","cJSON_CreateNumber","cJSON_ReplaceItemInObject","cJSON_AddRawToObject","cJSON_CreateString","cJSON_AddItemToObject","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Read the entire input data
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *input_buffer = (char *)cJSON_malloc(file_size + 1);
    if (!input_buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    fread(input_buffer, 1, file_size, in_file);
    input_buffer[file_size] = '\0';
    assert_file_closed(&in_file);;
    
    // Parse the input JSON data
    cJSON *parsed_json = cJSON_ParseWithLength(input_buffer, file_size);
    if (!parsed_json) {
        cJSON_free(input_buffer);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Use cJSON_SetNumberHelper - modify a number in the parsed JSON
    // Look for a number field in the first object
    if (cJSON_IsArray(parsed_json)) {
        cJSON *first_item = cJSON_GetArrayItem(parsed_json, 0);
        if (first_item && cJSON_IsObject(first_item)) {
            cJSON *latitude = cJSON_GetObjectItemCaseSensitive(first_item, "Latitude");
            if (latitude && cJSON_IsNumber(latitude)) {
                double new_lat = cJSON_SetNumberHelper(latitude, 42.0);
                (void)new_lat; // Use the variable
            }
        }
    }
    
    // 2. Create a new JSON object to work with
    cJSON *new_object = cJSON_CreateObject();
    
    // 3. Use cJSON_CreateBool and cJSON_AddBoolToObject
    cJSON *bool_item = cJSON_CreateBool(1);
    cJSON_AddBoolToObject(new_object, "is_valid", 1);
    cJSON_Delete(bool_item); // Clean up the standalone bool
    
    // 4. Use cJSON_CreateStringArray and cJSON_AddArrayToObject
    const char *tags[] = {"tag1", "tag2", "tag3"};
    cJSON *string_array = cJSON_CreateStringArray(tags, 3);
    cJSON_AddArrayToObject(new_object, "tags_array");
    cJSON *tags_in_object = cJSON_GetObjectItem(new_object, "tags_array");
    if (tags_in_object) {
        cJSON_AddItemToObject(new_object, "tags", string_array);
    } else {
        cJSON_Delete(string_array);
    }
    
    // 5. Use cJSON_ReplaceItemInArray
    // Add new_object to parsed_json if it's an array
    if (cJSON_IsArray(parsed_json)) {
        int array_size = cJSON_GetArraySize(parsed_json);
        if (array_size > 0) {
            cJSON *replacement = cJSON_CreateString("replaced_item");
            cJSON_ReplaceItemInArray(parsed_json, 0, replacement);
        }
    }
    
    // 6. Use cJSON_ReplaceItemInObject
    // Replace a field in new_object
    cJSON *new_field = cJSON_CreateNumber(100.5);
    cJSON_ReplaceItemInObject(new_object, "is_valid", new_field);
    
    // 7. Use cJSON_AddRawToObject
    const char *raw_json = "{\"raw\":\"data\"}";
    cJSON_AddRawToObject(new_object, "raw_data", raw_json);
    
    // 8. Use cJSON_AddItemToObject (additional usage)
    cJSON *extra_item = cJSON_CreateString("extra");
    cJSON_AddItemToObject(new_object, "extra_field", extra_item);
    
    // Create output FILE*
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Print the modified JSON
        char *printed = cJSON_Print(parsed_json);
        if (printed) {
            fwrite(printed, 1, strlen(printed), out_file);
            cJSON_free(printed);
        }
        assert_file_closed(&out_file);;
    }
    
    // Cleanup - release all allocated resources
    cJSON_Delete(parsed_json);
    cJSON_Delete(new_object);
    cJSON_free(input_buffer);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}