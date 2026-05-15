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
//<ID> 48
//<Prompt> []
/*<Combination>: [
*/
//<score> 31, nr_unique_branch: 2
//<Quality> {"density":31,"unique_branches":{"cJSON_HasObjectItem":[[1995,12,1995,47,0,0,4,0],[1995,12,1995,47,0,0,4,1]]},"library_calls":["cJSON_malloc","cJSON_free","cJSON_Parse","cJSON_free","cJSON_IsObject","cJSON_Delete","cJSON_free","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_Delete","cJSON_free","cJSON_HasObjectItem","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_CreateBool","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_SetValuestring","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_Parse","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_HasObjectItem","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_CreateBool","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_SetValuestring","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a temporary file to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read the input data into a buffer
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *json_buffer = (char *)cJSON_malloc(file_size + 1);
    if (!json_buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t read_size = fread(json_buffer, 1, file_size, in_file);
    assert_file_closed(&in_file);;
    
    if (read_size != (size_t)file_size) {
        cJSON_free(json_buffer);
        assert_file_closed(&in_file);
	return 0;
    }
    
    json_buffer[file_size] = '\0';
    
    // Parse the JSON data
    cJSON *parsed_json = cJSON_Parse(json_buffer);
    if (!parsed_json) {
        cJSON_free(json_buffer);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Check if parsed_json is an object
    if (!cJSON_IsObject(parsed_json)) {
        cJSON_Delete(parsed_json);
        cJSON_free(json_buffer);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get the Image object
    cJSON *image_object = cJSON_GetObjectItemCaseSensitive(parsed_json, "Image");
    if (!image_object || !cJSON_IsObject(image_object)) {
        cJSON_Delete(parsed_json);
        cJSON_free(json_buffer);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Check if Image object has "Width" using cJSON_HasObjectItem
    cJSON_bool has_width = cJSON_HasObjectItem(image_object, "Width");
    
    // Get the current Width value
    cJSON *width_item = cJSON_GetObjectItemCaseSensitive(image_object, "Width");
    if (width_item && cJSON_IsNumber(width_item)) {
        // Use cJSON_SetNumberHelper to modify the width value
        double new_width = cJSON_SetNumberHelper(width_item, 1024.0);
        
        // Create a new boolean item
        cJSON *new_bool_item = cJSON_CreateBool(1);
        if (new_bool_item) {
            // Replace the Width item with the boolean item
            // Note: This is just for demonstration - in practice you might not want to do this
            cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(
                image_object, "Width", new_bool_item);
            
            // If replacement failed, clean up the boolean item
            if (!replaced) {
                cJSON_Delete(new_bool_item);
            }
        }
    }
    
    // Get the Thumbnail object
    cJSON *thumbnail_object = cJSON_GetObjectItemCaseSensitive(image_object, "Thumbnail");
    if (thumbnail_object && cJSON_IsObject(thumbnail_object)) {
        cJSON *url_item = cJSON_GetObjectItemCaseSensitive(thumbnail_object, "Url");
        if (url_item && cJSON_IsString(url_item)) {
            // Use cJSON_SetValuestring to modify the URL
            char *new_url = cJSON_SetValuestring(url_item, "https://example.com/new_image");
            if (!new_url) {
                // Handle allocation failure
            }
        }
    }
    
    // Get the IDs array
    cJSON *ids_array = cJSON_GetObjectItemCaseSensitive(image_object, "IDs");
    if (ids_array && cJSON_IsArray(ids_array)) {
        // Use cJSON_GetArraySize to get the size of the array
        int array_size = cJSON_GetArraySize(ids_array);
        
        // Do something with the array size
        if (array_size > 0) {
            // Example: Get the first item
            cJSON *first_item = cJSON_GetArrayItem(ids_array, 0);
        }
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Print the modified JSON to the output file
        char *printed_json = cJSON_Print(parsed_json);
        if (printed_json) {
            fwrite(printed_json, 1, strlen(printed_json), out_file);
            cJSON_free(printed_json);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up allocated resources
    cJSON_Delete(parsed_json);
    cJSON_free(json_buffer);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}