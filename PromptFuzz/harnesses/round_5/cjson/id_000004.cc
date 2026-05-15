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
//<score> 32, nr_unique_branch: 0
//<Quality> {"density":32,"unique_branches":{},"library_calls":["cJSON_malloc","cJSON_Parse","cJSON_free","cJSON_CreateStringReference","cJSON_CreateFalse","cJSON_IsFalse","cJSON_CreateString","cJSON_CreateRaw","cJSON_Duplicate","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_malloc","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_Parse","cJSON_CreateStringReference","cJSON_CreateFalse","cJSON_IsFalse","cJSON_CreateString","cJSON_CreateRaw","cJSON_Duplicate","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_malloc","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_free"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified cJSON APIs with the given input data:

*/


// cJSON headers
extern "C" {
    #include "cJSON.h"
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a null-terminated string from the input data
    char *input_str = (char *)cJSON_malloc(size + 1);
    if (!input_str) {
	return 0;
	}
	
    
    memcpy(input_str, data, size);
    input_str[size] = '\0';
    
    // Parse the JSON input using cJSON_Parse
    cJSON *root = cJSON_Parse(input_str);
    if (!root) {
        cJSON_free(input_str);
        return 0;
    }
    
    // Create a string reference (API #1)
    const char *ref_str = "string_reference";
    cJSON *string_ref = cJSON_CreateStringReference(ref_str);
    
    // Create a false value (API #2)
    cJSON *false_item = cJSON_CreateFalse();
    
    // Check if the item is false (API #3)
    if (cJSON_IsFalse(false_item)) {
        // Create a string (API #4)
        cJSON *string_item = cJSON_CreateString("test_string");
        
        // Create a raw JSON item (API #5)
        cJSON *raw_item = cJSON_CreateRaw("\"raw_json_data\"");
        
        // Duplicate the root item (API #6)
        cJSON *duplicate = cJSON_Duplicate(root, 1); // 1 for recursive
        
        // Try to get number values from the parsed JSON
        if (duplicate) {
            // Navigate through the JSON structure to find numbers
            cJSON *widget = cJSON_GetObjectItemCaseSensitive(duplicate, "widget");
            if (widget) {
                cJSON *window = cJSON_GetObjectItemCaseSensitive(widget, "window");
                if (window) {
                    cJSON *width = cJSON_GetObjectItemCaseSensitive(window, "width");
                    cJSON *height = cJSON_GetObjectItemCaseSensitive(window, "height");
                    
                    // Get number values (API #7)
                    if (width && cJSON_IsNumber(width)) {
                        double width_val = cJSON_GetNumberValue(width);
                        // Use the value to prevent optimization
                        volatile double unused = width_val;
                    }
                    
                    if (height && cJSON_IsNumber(height)) {
                        double height_val = cJSON_GetNumberValue(height);
                        // Use the value to prevent optimization
                        volatile double unused = height_val;
                    }
                }
                
                cJSON *image = cJSON_GetObjectItemCaseSensitive(widget, "image");
                if (image) {
                    cJSON *hOffset = cJSON_GetObjectItemCaseSensitive(image, "hOffset");
                    cJSON *vOffset = cJSON_GetObjectItemCaseSensitive(image, "vOffset");
                    
                    if (hOffset && cJSON_IsNumber(hOffset)) {
                        double hOffset_val = cJSON_GetNumberValue(hOffset);
                        volatile double unused = hOffset_val;
                    }
                    
                    if (vOffset && cJSON_IsNumber(vOffset)) {
                        double vOffset_val = cJSON_GetNumberValue(vOffset);
                        volatile double unused = vOffset_val;
                    }
                }
                
                cJSON *text = cJSON_GetObjectItemCaseSensitive(widget, "text");
                if (text) {
                    cJSON *size_item = cJSON_GetObjectItemCaseSensitive(text, "size");
                    cJSON *hOffset = cJSON_GetObjectItemCaseSensitive(text, "hOffset");
                    cJSON *vOffset = cJSON_GetObjectItemCaseSensitive(text, "vOffset");
                    
                    if (size_item && cJSON_IsNumber(size_item)) {
                        double size_val = cJSON_GetNumberValue(size_item);
                        volatile double unused = size_val;
                    }
                    
                    if (hOffset && cJSON_IsNumber(hOffset)) {
                        double hOffset_val = cJSON_GetNumberValue(hOffset);
                        volatile double unused = hOffset_val;
                    }
                    
                    if (vOffset && cJSON_IsNumber(vOffset)) {
                        double vOffset_val = cJSON_GetNumberValue(vOffset);
                        volatile double unused = vOffset_val;
                    }
                }
            }
            
            cJSON_Delete(duplicate);
        }
        
        // Clean up created items
        cJSON_Delete(string_item);
        cJSON_Delete(raw_item);
    }
    
    // Additional cJSON_malloc usage (API #8)
    void *extra_buffer = cJSON_malloc(256);
    if (extra_buffer) {
        memset(extra_buffer, 0, 256);
        cJSON_free(extra_buffer);
    }
    
    // Clean up all allocated resources
    cJSON_Delete(root);
    cJSON_Delete(string_ref);
    cJSON_Delete(false_item);
    cJSON_free(input_str);
    
    return 0;
}