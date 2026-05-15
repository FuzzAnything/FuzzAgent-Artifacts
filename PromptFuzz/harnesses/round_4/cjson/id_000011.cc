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
//<ID> 45
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_IsInvalid","cJSON_CreateTrue","cJSON_IsTrue","cJSON_Delete","cJSON_CreateRaw","cJSON_IsRaw","cJSON_Delete","cJSON_HasObjectItem","cJSON_CreateObjectReference","cJSON_Delete","cJSON_Delete","cJSON_HasObjectItem","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_CreateObjectReference","cJSON_GetObjectItemCaseSensitive","cJSON_IsTrue","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsRaw","cJSON_IsInvalid","cJSON_IsInvalid","cJSON_Delete","cJSON_AddItemToObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_CreateTrue","cJSON_IsTrue","cJSON_Delete","cJSON_CreateRaw","cJSON_IsRaw","cJSON_Delete","cJSON_CreateObjectReference","cJSON_Delete","cJSON_IsInvalid","cJSON_HasObjectItem","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_HasObjectItem","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_CreateObjectReference","cJSON_GetObjectItemCaseSensitive","cJSON_IsTrue","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsRaw","cJSON_IsInvalid","cJSON_IsInvalid","cJSON_Delete","cJSON_AddItemToObject","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_CreateTrue","cJSON_IsTrue","cJSON_Delete","cJSON_CreateRaw","cJSON_IsRaw","cJSON_Delete","cJSON_CreateObjectReference","cJSON_Delete","cJSON_IsInvalid","cJSON_HasObjectItem","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<133) return 0;

	
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
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
	//fuzzer shim end}




    // 1. Parse input JSON data
    cJSON *root = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!root) {
        // If parsing fails, we can still test some APIs with manually created objects
        cJSON *temp_obj = cJSON_CreateObject();
        if (temp_obj) {
            // Test cJSON_IsInvalid on NULL
            cJSON_IsInvalid(nullptr);
            
            // Test cJSON_IsTrue on a true value
            cJSON *true_item = cJSON_CreateTrue();
            if (true_item) {
                cJSON_IsTrue(true_item);
                cJSON_Delete(true_item);
            }
            
            // Test cJSON_IsRaw on a raw value
            cJSON *raw_item = cJSON_CreateRaw(fuzz_str_1);
            if (raw_item) {
                cJSON_IsRaw(raw_item);
                cJSON_Delete(raw_item);
            }
            
            // Test cJSON_HasObjectItem
            cJSON_HasObjectItem(temp_obj, fuzz_str_2);
            
            // Test cJSON_CreateObjectReference
            cJSON *ref = cJSON_CreateObjectReference(temp_obj);
            if (ref) {
                cJSON_Delete(ref);
            }
            
            cJSON_Delete(temp_obj);
        }
        return 0;
    }
    
    // 2. Test cJSON_HasObjectItem - check if widget exists
    cJSON_bool has_widget = cJSON_HasObjectItem(root, fuzz_str_3);
    
    if (has_widget) {
        // 3. Get the widget object
        cJSON *widget = cJSON_GetObjectItemCaseSensitive(root, fuzz_str_4);
        
        if (widget && cJSON_IsObject(widget)) {
            // 4. Test cJSON_DetachItemFromObjectCaseSensitive - detach window
            cJSON *detached_window = cJSON_DetachItemFromObjectCaseSensitive(widget, fuzz_str_5);
            
            if (detached_window) {
                // 5. Test cJSON_CreateObjectReference - create reference to detached window
                cJSON *window_ref = cJSON_CreateObjectReference(detached_window);
                
                // 6. Test cJSON_IsTrue on debug field (it's "on" string, not true, but we'll test API)
                cJSON *debug_item = cJSON_GetObjectItemCaseSensitive(widget, fuzz_str_6);
                if (debug_item) {
                    // This will return false since it's a string, but we're testing the API call
                    cJSON_IsTrue(debug_item);
                }
                
                // 7. Test cJSON_IsRaw - check if any field is raw (none should be)
                cJSON *image = cJSON_GetObjectItemCaseSensitive(widget, fuzz_str_7);
                if (image && cJSON_IsObject(image)) {
                    cJSON *src = cJSON_GetObjectItemCaseSensitive(image, fuzz_str_8);
                    if (src) {
                        cJSON_IsRaw(src);  // Will return false since it's a string
                    }
                }
                
                // 8. Test cJSON_IsInvalid on various items
                cJSON_IsInvalid(nullptr);  // Test with NULL
                cJSON_IsInvalid(detached_window);  // Test with valid object (should return false)
                
                // Clean up references
                if (window_ref) {
                    cJSON_Delete(window_ref);
                }
                
                // Re-attach window to maintain structure for cleanup
                cJSON_AddItemToObject(widget, fuzz_str_9, detached_window);
            }
            
            // Additional test: Try to detach non-existent item
            cJSON *nonexistent = cJSON_DetachItemFromObjectCaseSensitive(widget, fuzz_str_10);
            if (nonexistent) {
                cJSON_Delete(nonexistent);
            }
        }
    }
    
    // 9. Create some additional test items to ensure all required APIs are called
    
    // Create a true value and test cJSON_IsTrue
    cJSON *true_item = cJSON_CreateTrue();
    if (true_item) {
        cJSON_IsTrue(true_item);
        cJSON_Delete(true_item);
    }
    
    // Create a raw value and test cJSON_IsRaw
    cJSON *raw_item = cJSON_CreateRaw(fuzz_str_11);
    if (raw_item) {
        cJSON_IsRaw(raw_item);
        cJSON_Delete(raw_item);
    }
    
    // Create an object reference for testing
    cJSON *ref = cJSON_CreateObjectReference(root);
    if (ref) {
        cJSON_Delete(ref);
    }
    
    // Test cJSON_IsInvalid with NULL
    cJSON_IsInvalid(nullptr);
    
    // Test cJSON_HasObjectItem with non-existent item
    cJSON_HasObjectItem(root, fuzz_str_12);
    
    // 10. Clean up
    cJSON_Delete(root);
    
    return 0;
}