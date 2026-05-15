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
//<ID> 47
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 1
//<Quality> {"density":21,"unique_branches":{"cJSON_DetachItemViaPointer":[[2260,9,2260,25,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObjectReference","cJSON_Delete","cJSON_HasObjectItem","cJSON_GetObjectItemCaseSensitive","cJSON_IsInvalid","cJSON_HasObjectItem","cJSON_HasObjectItem","cJSON_GetObjectItemCaseSensitive","cJSON_DetachItemFromObjectCaseSensitive","cJSON_IsString","cJSON_AddItemToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_CreateTrue","cJSON_IsTrue","cJSON_Delete","cJSON_CreateRaw","cJSON_IsRaw","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObjectReference","cJSON_HasObjectItem","cJSON_GetObjectItemCaseSensitive","cJSON_IsInvalid","cJSON_HasObjectItem","cJSON_HasObjectItem","cJSON_GetObjectItemCaseSensitive","cJSON_DetachItemFromObjectCaseSensitive","cJSON_IsString","cJSON_AddItemToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_CreateTrue","cJSON_IsTrue","cJSON_Delete","cJSON_CreateRaw","cJSON_IsRaw","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<99) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
	//fuzzer shim end}




    // Create a FILE* to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the JSON data from the file
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *json_buffer = (char *)malloc(file_size + 1);
    if (!json_buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(json_buffer, 1, file_size, in_file);
    json_buffer[bytes_read] = '\0';
    assert_file_closed(&in_file);;

    // Parse the JSON data
    cJSON *root = cJSON_ParseWithLength(json_buffer, bytes_read);
    if (!root) {
        free(json_buffer);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create an object reference
    cJSON *widget_ref = cJSON_CreateObjectReference(root);
    if (!widget_ref) {
        cJSON_Delete(root);
        free(json_buffer);
        assert_file_closed(&in_file);
	return 0;
    }

    // Check if the root object has a "widget" item
    cJSON_bool has_widget = cJSON_HasObjectItem(root, fuzz_str_1);
    
    if (has_widget) {
        // Get the widget object
        cJSON *widget = cJSON_GetObjectItemCaseSensitive(root, fuzz_str_2);
        
        // Check for invalid items in widget
        cJSON_bool widget_invalid = cJSON_IsInvalid(widget);
        
        // Check if widget has specific items and their types
        cJSON_bool has_debug = cJSON_HasObjectItem(widget, fuzz_str_3);
        cJSON_bool has_window = cJSON_HasObjectItem(widget, fuzz_str_4);
        
        // Process window object if it exists
        if (has_window) {
            cJSON *window = cJSON_GetObjectItemCaseSensitive(widget, fuzz_str_5);
            
            // Try to detach "title" from window (case-sensitive)
            cJSON *detached_title = cJSON_DetachItemFromObjectCaseSensitive(window, fuzz_str_6);
            if (detached_title) {
                // Check if the detached title is a string
                cJSON_bool is_title_string = cJSON_IsString(detached_title);
                
                // Re-attach the title
                cJSON_AddItemToObject(window, fuzz_str_7, detached_title);
            }
            
            // Check for boolean values in window
            // Note: window in the example doesn't have boolean values, 
            // but we can check for other types
            cJSON *width = cJSON_GetObjectItemCaseSensitive(window, fuzz_str_8);
            if (width) {
                cJSON_bool is_width_number = cJSON_IsNumber(width);
            }
        }
        
        // Process image object if it exists
        cJSON *image = cJSON_GetObjectItemCaseSensitive(widget, fuzz_str_9);
        if (image) {
            // Check for alignment string
            cJSON *alignment = cJSON_GetObjectItemCaseSensitive(image, fuzz_str_10);
            if (alignment) {
                cJSON_bool is_alignment_string = cJSON_IsString(alignment);
            }
        }
        
        // Create a temporary true value for testing cJSON_IsTrue
        cJSON *temp_true = cJSON_CreateTrue();
        cJSON_bool is_temp_true = cJSON_IsTrue(temp_true);
        cJSON_Delete(temp_true);
        
        // Create a temporary raw value for testing cJSON_IsRaw
        cJSON *temp_raw = cJSON_CreateRaw(fuzz_str_11);
        cJSON_bool is_temp_raw = cJSON_IsRaw(temp_raw);
        cJSON_Delete(temp_raw);
    }

    // Print the JSON for verification
    char *printed = cJSON_Print(root);
    if (printed) {
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed, 1, strlen(printed), out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(printed);
    }

    // Cleanup
    cJSON_Delete(widget_ref);
    cJSON_Delete(root);
    free(json_buffer);
    
    assert_file_closed(&in_file);
	return 0;
}