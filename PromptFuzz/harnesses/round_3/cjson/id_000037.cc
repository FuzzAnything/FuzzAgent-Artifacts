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
//<ID> 350
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"add_item_to_object":[[2108,48,2108,70,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_GetObjectItemCaseSensitive","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_Delete","cJSON_CreateNull","cJSON_GetArraySize","cJSON_ReplaceItemInArray","cJSON_AddItemToArray","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_AddBoolToObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_PrintBuffered","cJSON_Minify","cJSON_free","cJSON_AddItemToObject","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_CreateNull","cJSON_GetArraySize","cJSON_ReplaceItemInArray","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_AddBoolToObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_PrintBuffered","cJSON_Minify","cJSON_free","cJSON_AddItemToObject","cJSON_Delete"],"visited":0}
/**/


// Include cJSON headers

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<94) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeFloatingArray(fuzz_float_array_3, fuzz_float_array_size_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
	
		const float * fuzzer_var_cJSON_CreateFloatArray_9_0 = fuzz_float_array_3;
		int fuzzer_size_cJSON_CreateFloatArray_9_0 = static_cast<int>(fuzz_float_array_size_3);//fuzzer shim end}




    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read the entire input into a buffer
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *input_buffer = (char *)malloc(file_size + 1);
    if (!input_buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(input_buffer, 1, file_size, in_file);
    input_buffer[bytes_read] = '\0';
    assert_file_closed(&in_file);;
    
    // Parse the input JSON
    cJSON *parsed_json = cJSON_ParseWithLength(input_buffer, bytes_read);
    if (!parsed_json) {
        free(input_buffer);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get the "Image" object
    cJSON *image_obj = cJSON_GetObjectItemCaseSensitive(parsed_json, fuzz_str_1);
    if (!image_obj) {
        cJSON_Delete(parsed_json);
        free(input_buffer);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get the "IDs" array from Image
    cJSON *ids_array = cJSON_GetObjectItemCaseSensitive(image_obj, fuzz_str_2);
    if (!ids_array) {
        cJSON_Delete(parsed_json);
        free(input_buffer);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Create a null item
    cJSON *null_item = cJSON_CreateNull();
    
    // 2. Replace the first item in IDs array with null
    // Check if array has at least one element
    if (cJSON_GetArraySize(ids_array) > 0) {
        cJSON_ReplaceItemInArray(ids_array, 0, null_item);
    } else {
        // If array is empty, add the null item
        cJSON_AddItemToArray(ids_array, null_item);
    }
    
    // 3. Create a float array
    float float_numbers[] = {1.1f, 2.2f, 3.3f, 4.4f};
    cJSON *float_array = cJSON_CreateFloatArray(fuzzer_var_cJSON_CreateFloatArray_9_0, fuzzer_size_cJSON_CreateFloatArray_9_0);
    
    // 4. Add the float array to the Image object
    cJSON_AddItemToObject(image_obj, fuzz_str_4, float_array);
    
    // 5. Add a boolean to the Image object
    cJSON_AddBoolToObject(image_obj, fuzz_str_5, fuzz_int32_t_6);
    
    // 6. Detach the Thumbnail object
    cJSON *thumbnail = cJSON_DetachItemFromObjectCaseSensitive(image_obj, fuzz_str_7);
    
    // 7. Print the JSON with buffering
    char *printed_json = cJSON_PrintBuffered(parsed_json, fuzz_int32_t_8, fuzz_int32_t_9);
    
    // 8. Minify the printed JSON
    if (printed_json) {
        cJSON_Minify(printed_json);
        
        // Write the minified JSON to output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed_json, 1, strlen(printed_json), out_file);
            assert_file_closed(&out_file);;
        }
        
        cJSON_free(printed_json);
    }
    
    // 9. Add the detached thumbnail back using AddItemToObject
    if (thumbnail) {
        cJSON_AddItemToObject(image_obj, fuzz_str_10, thumbnail);
    }
    
    // Clean up
    cJSON_Delete(parsed_json);
    free(input_buffer);
    
    assert_file_closed(&in_file);
	return 0;
}