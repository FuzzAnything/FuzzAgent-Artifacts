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
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 84, nr_unique_branch: 3
//<Quality> {"density":21,"unique_branches":{"skip_oneline_comment":[[2879,12,2879,31,0,0,4,1]],"cJSON_Minify":[[2939,13,2939,22,0,0,4,0]],"cJSON_Compare":[[3097,9,3097,15,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsBool","cJSON_CreateObject","cJSON_Delete","cJSON_CreateRaw","cJSON_AddItemToObject","cJSON_AddFalseToObject","cJSON_Delete","cJSON_Delete","cJSON_AddNumberToObject","cJSON_Delete","cJSON_Delete","cJSON_AddObjectToObject","cJSON_AddNumberToObject","cJSON_AddNumberToObject","cJSON_IsObject","cJSON_AddItemReferenceToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsFalse","cJSON_Duplicate","cJSON_Delete","cJSON_Compare","cJSON_PrintUnformatted","cJSON_Minify","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsBool","cJSON_CreateObject","cJSON_CreateRaw","cJSON_AddItemToObject","cJSON_AddFalseToObject","cJSON_AddNumberToObject","cJSON_AddObjectToObject","cJSON_AddNumberToObject","cJSON_AddNumberToObject","cJSON_IsObject","cJSON_AddItemReferenceToObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsFalse","cJSON_Duplicate","cJSON_Delete","cJSON_Compare","cJSON_PrintUnformatted","cJSON_Minify","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


// Include necessary headers for cjson
extern "C" {
    #include "cJSON.h"
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create a copy of input data with null termination for cJSON parsing
    char *input_str = (char *)malloc(size + 1);
    if (!input_str) {
	return 0;
	}
	
    
    memcpy(input_str, data, size);
    input_str[size] = '\0';
    
    // Parse input JSON data
    cJSON *parsed_json = cJSON_Parse(input_str);
    if (!parsed_json) {
        free(input_str);
        return 0;
    }
    
    // API 1: Check if a specific item is boolean
    cJSON *widget_item = cJSON_GetObjectItemCaseSensitive(parsed_json, "widget");
    if (widget_item && cJSON_IsObject(widget_item)) {
        cJSON *debug_item = cJSON_GetObjectItemCaseSensitive(widget_item, "debug");
        if (debug_item) {
            // Note: debug is "on" (string) not boolean, so cJSON_IsBool should return false
            // This demonstrates the use of cJSON_IsBool
            cJSON_bool is_bool = cJSON_IsBool(debug_item);
            // This check is expected to fail since "on" is a string
        }
    }
    
    // Create a new cJSON object for our modifications
    cJSON *new_object = cJSON_CreateObject();
    if (!new_object) {
        cJSON_Delete(parsed_json);
        free(input_str);
        return 0;
    }
    
    // API 2: Create raw JSON and add to new object
    // Use a portion of input data as raw string (ensuring it's null-terminated)
    size_t raw_len = size > 50 ? 50 : size;
    char *raw_data = (char *)malloc(raw_len + 1);
    if (raw_data) {
        memcpy(raw_data, data, raw_len);
        raw_data[raw_len] = '\0';
        
        cJSON *raw_item = cJSON_CreateRaw(raw_data);
        if (raw_item) {
            cJSON_AddItemToObject(new_object, "raw_input", raw_item);
        }
        free(raw_data);
    }
    
    // API 3: Add false boolean to object
    cJSON *false_item = cJSON_AddFalseToObject(new_object, "is_valid");
    if (!false_item) {
        // Clean up and continue
        cJSON_Delete(new_object);
        cJSON_Delete(parsed_json);
        free(input_str);
        return 0;
    }
    
    // API 4: Add number to object
    // Derive a number from input data (use first 4 bytes as float/double)
    double number_value = 0.0;
    if (size >= sizeof(double)) {
        memcpy(&number_value, data, sizeof(double));
    } else if (size >= sizeof(float)) {
        float float_val;
        memcpy(&float_val, data, sizeof(float));
        number_value = float_val;
    } else if (size > 0) {
        number_value = data[0];
    }
    
    cJSON *number_item = cJSON_AddNumberToObject(new_object, "data_value", number_value);
    if (!number_item) {
        cJSON_Delete(new_object);
        cJSON_Delete(parsed_json);
        free(input_str);
        return 0;
    }
    
    // API 5: Add object to object (nested object)
    cJSON *nested_object = cJSON_AddObjectToObject(new_object, "nested_data");
    if (nested_object) {
        // Add some values to the nested object using input data
        if (size > 0) {
            cJSON_AddNumberToObject(nested_object, "size", (double)size);
            cJSON_AddNumberToObject(nested_object, "first_byte", (double)data[0]);
        }
    }
    
    // Add the original parsed JSON (or part of it) to our new object
    if (widget_item && cJSON_IsObject(widget_item)) {
        cJSON_AddItemReferenceToObject(new_object, "original_widget", widget_item);
    }
    
    // Verify our boolean addition worked correctly
    cJSON *is_valid_item = cJSON_GetObjectItemCaseSensitive(new_object, "is_valid");
    if (is_valid_item) {
        cJSON_bool is_false = cJSON_IsFalse(is_valid_item);
        // Should be true since we added false
    }
    
    // Print the result (for debugging, but skip in actual fuzzing to avoid overhead)
    // char *printed = cJSON_Print(new_object);
    // if (printed) {
    //     cJSON_free(printed);
    // }
    
    // Additional: Test duplication with recursion
    cJSON *duplicate = cJSON_Duplicate(new_object, 1);
    if (duplicate) {
        cJSON_Delete(duplicate);
    }
    
    // Test comparison
    cJSON_bool compare_result = cJSON_Compare(new_object, new_object, 1);
    // Should return true (1) for identical objects
    
    // Minify test (if we had a string to minify)
    char *json_str = cJSON_PrintUnformatted(new_object);
    if (json_str) {
        cJSON_Minify(json_str);  // Minify in place
        cJSON_free(json_str);
    }
    
    // Clean up all allocated resources
    cJSON_Delete(new_object);
    cJSON_Delete(parsed_json);
    free(input_str);
    
    return 0;
}