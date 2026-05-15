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
//<ID> 46
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 0
//<Quality> {"density":28,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_HasObjectItem","cJSON_GetObjectItemCaseSensitive","cJSON_HasObjectItem","cJSON_DetachItemFromObjectCaseSensitive","cJSON_IsInvalid","cJSON_CreateObjectReference","cJSON_CreateTrue","cJSON_IsTrue","cJSON_IsRaw","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_HasObjectItem","cJSON_CreateObjectReference","cJSON_HasObjectItem","cJSON_Delete","cJSON_DetachItemFromObjectCaseSensitive","cJSON_IsInvalid","cJSON_IsRaw","cJSON_Delete","cJSON_CreateBool","cJSON_IsTrue","cJSON_CreateRaw","cJSON_IsRaw","cJSON_IsInvalid","cJSON_CreateObjectReference","cJSON_HasObjectItem","cJSON_DetachItemFromObjectCaseSensitive","cJSON_IsInvalid","cJSON_IsRaw","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_HasObjectItem","cJSON_GetObjectItemCaseSensitive","cJSON_HasObjectItem","cJSON_DetachItemFromObjectCaseSensitive","cJSON_IsInvalid","cJSON_CreateObjectReference","cJSON_CreateTrue","cJSON_IsTrue","cJSON_IsRaw","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_HasObjectItem","cJSON_CreateObjectReference","cJSON_HasObjectItem","cJSON_Delete","cJSON_DetachItemFromObjectCaseSensitive","cJSON_IsInvalid","cJSON_IsRaw","cJSON_Delete","cJSON_CreateBool","cJSON_IsTrue","cJSON_CreateRaw","cJSON_IsRaw","cJSON_IsInvalid","cJSON_CreateObjectReference","cJSON_HasObjectItem","cJSON_DetachItemFromObjectCaseSensitive","cJSON_IsInvalid","cJSON_IsRaw","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified cjson APIs to achieve the event of parsing JSON input, manipulating the parsed object, and checking various conditions:

*/


// Include cjson headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<104) return 0;

	
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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
	//fuzzer shim end}




    // Create a FILE pointer from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the input data as a string
    char *input_str = (char *)malloc(size + 1);
    if (!input_str) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t read_size = fread(input_str, 1, size, in_file);
    input_str[read_size] = '\0';
    assert_file_closed(&in_file);;

    // Parse the JSON string using cjson
    cJSON *root = cJSON_Parse(input_str);
    if (!root) {
        free(input_str);
        assert_file_closed(&in_file);
	return 0;
    }

    // Use cJSON_HasObjectItem to check for specific object items
    cJSON_bool has_widget = cJSON_HasObjectItem(root, fuzz_str_1);
    
    if (has_widget) {
        cJSON *widget = cJSON_GetObjectItemCaseSensitive(root, fuzz_str_2);
        
        // Use cJSON_HasObjectItem again to check for debug
        cJSON_bool has_debug = cJSON_HasObjectItem(widget, fuzz_str_3);
        
        if (has_debug) {
            // Detach the debug item using cJSON_DetachItemFromObjectCaseSensitive
            cJSON *debug_item = cJSON_DetachItemFromObjectCaseSensitive(widget, fuzz_str_4);
            
            if (debug_item) {
                // Check if the debug item is invalid
                cJSON_bool is_invalid = cJSON_IsInvalid(debug_item);
                
                // Create an object reference to the debug item
                cJSON *debug_ref = cJSON_CreateObjectReference(debug_item);
                
                // For demonstration, create a boolean item and check if it's true
                cJSON *true_item = cJSON_CreateTrue();
                cJSON_bool is_true = cJSON_IsTrue(true_item);
                
                // Check if debug_ref is raw (though it's an object reference, not raw)
                cJSON_bool is_raw = cJSON_IsRaw(debug_ref);
                
                // Clean up temporary items
                cJSON_Delete(true_item);
                cJSON_Delete(debug_ref);
                cJSON_Delete(debug_item);
            }
        }
        
        // Try to access and manipulate other parts of the JSON
        cJSON *window = cJSON_GetObjectItemCaseSensitive(widget, fuzz_str_5);
        if (window) {
            // Check if window has a title
            cJSON_bool has_title = cJSON_HasObjectItem(window, fuzz_str_6);
            
            // Create an object reference to window
            cJSON *window_ref = cJSON_CreateObjectReference(window);
            
            if (window_ref) {
                // Use window_ref to check for width
                cJSON_bool has_width = cJSON_HasObjectItem(window_ref, fuzz_str_7);
                
                // Clean up the reference
                cJSON_Delete(window_ref);
            }
            
            // Detach width item if it exists
            cJSON *width_item = cJSON_DetachItemFromObjectCaseSensitive(window, fuzz_str_8);
            if (width_item) {
                // Check if width item is invalid (should be false for a number)
                cJSON_bool width_invalid = cJSON_IsInvalid(width_item);
                
                // Check if width item is raw (should be false for a number)
                cJSON_bool width_raw = cJSON_IsRaw(width_item);
                
                cJSON_Delete(width_item);
            }
        }
    }

    // Create a boolean value and check if it's true
    cJSON *bool_test = cJSON_CreateBool(fuzz_int32_t_9);
    cJSON_bool bool_is_true = cJSON_IsTrue(bool_test);
    
    // Create a raw JSON string and check if it's raw
    cJSON *raw_test = cJSON_CreateRaw(fuzz_str_10);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_test);
    
    // Check if root is invalid (should be false for valid JSON)
    cJSON_bool root_invalid = cJSON_IsInvalid(root);
    
    // Create an object reference to root
    cJSON *root_ref = cJSON_CreateObjectReference(root);
    if (root_ref) {
        // Check if root_ref has any items
        cJSON_bool has_items = cJSON_HasObjectItem(root_ref, fuzz_str_11);
        
        // Detach an item from root_ref if widget exists
        cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root_ref, fuzz_str_12);
        if (detached) {
            // Check if detached item is invalid
            cJSON_bool detached_invalid = cJSON_IsInvalid(detached);
            
            // Check if detached item is raw
            cJSON_bool detached_raw = cJSON_IsRaw(detached);
            
            cJSON_Delete(detached);
        }
        
        cJSON_Delete(root_ref);
    }
    
    // Clean up
    cJSON_Delete(bool_test);
    cJSON_Delete(raw_test);
    cJSON_Delete(root);
    free(input_str);
    
    assert_file_closed(&in_file);
	return 0;
}