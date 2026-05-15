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
//<ID> 172
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 35
//<Quality> {"density":18,"unique_branches":{"cJSON_Compare":[[3082,9,3082,25,0,0,4,0],[3083,9,3083,24,0,0,4,0],[3084,9,3084,24,0,0,4,0],[3085,9,3085,26,0,0,4,0],[3086,9,3086,26,0,0,4,0],[3088,9,3088,25,0,0,4,1],[3089,9,3089,26,0,0,4,0],[3105,9,3105,25,0,0,4,0],[3106,9,3106,24,0,0,4,0],[3107,9,3107,24,0,0,4,0],[3110,9,3110,26,0,0,4,0],[3111,17,3111,63,0,0,4,0],[3111,17,3111,63,0,0,4,1],[3117,9,3117,26,0,0,4,0],[3119,17,3119,41,0,0,4,1],[3119,45,3119,69,0,0,4,1],[3123,17,3123,60,0,0,4,0],[3123,17,3123,60,0,0,4,1],[3130,9,3130,25,0,0,4,1],[3137,21,3137,73,0,0,4,1],[3147,17,3147,39,0,0,4,1],[3154,9,3154,26,0,0,4,0],[3162,21,3162,38,0,0,4,0],[3162,21,3162,38,0,0,4,1],[3167,21,3167,73,0,0,4,0],[3167,21,3167,73,0,0,4,1],[3178,21,3178,38,0,0,4,0],[3178,21,3178,38,0,0,4,1],[3183,21,3183,73,0,0,4,1],[296,58,296,73,29,0,4,0],[296,99,296,114,29,0,4,0],[296,99,296,114,29,0,4,1],[296,58,296,73,32,0,4,0],[296,99,296,114,32,0,4,0],[296,99,296,114,32,0,4,1]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_AddNullToObject","cJSON_GetObjectItem","cJSON_IsNull","cJSON_AddFalseToObject","cJSON_GetObjectItem","cJSON_IsFalse","cJSON_CreateStringReference","cJSON_AddItemToObject","cJSON_Duplicate","cJSON_Compare","cJSON_Compare","cJSON_Delete","cJSON_CreateObject","cJSON_AddNullToObject","cJSON_AddFalseToObject","cJSON_Compare","cJSON_AddTrueToObject","cJSON_Compare","cJSON_Delete","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateStringReference","cJSON_AddItemToObject","cJSON_IsFalse","cJSON_Print","cJSON_Delete","cJSON_Delete","cJSON_ParseWithLengthOpts","cJSON_Delete","cJSON_ParseWithLengthOpts","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_AddNullToObject","cJSON_GetObjectItem","cJSON_IsNull","cJSON_AddFalseToObject","cJSON_GetObjectItem","cJSON_IsFalse","cJSON_CreateStringReference","cJSON_AddItemToObject","cJSON_Duplicate","cJSON_Compare","cJSON_Compare","cJSON_Delete","cJSON_CreateObject","cJSON_AddNullToObject","cJSON_AddFalseToObject","cJSON_Compare","cJSON_AddTrueToObject","cJSON_Compare","cJSON_Delete","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateStringReference","cJSON_AddItemToObject","cJSON_IsFalse","cJSON_Print","cJSON_Delete","cJSON_Delete","cJSON_ParseWithLengthOpts","cJSON_Delete","cJSON_ParseWithLengthOpts","cJSON_Delete"],"visited":0}
/*Looking at the provided APIs and the requirements, I'll create a fuzz driver that tests the JSON parsing and manipulation capabilities of cjson. The driver will:

1. Parse the input JSON data
2. Create new JSON objects and use the specified APIs
3. Manipulate the JSON structure
4. Compare JSON objects
5. Clean up resources

Here's the complete implementation:

*/


// Include cjson headers

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Convert input data to null-terminated string for parsing
    char *json_string = (char *)malloc(size + 1);
    if (!json_string) {
        return 0;
    }
    memcpy(json_string, data, size);
    json_string[size] = '\0';
    
    // Parse the JSON input using cJSON_ParseWithLength
    cJSON *parsed_json = cJSON_ParseWithLength(json_string, size);
    
    if (parsed_json) {
        // Test cJSON_IsFalse, cJSON_IsNull, and cJSON_Compare APIs
        
        // Create a test object to compare with
        cJSON *test_object = cJSON_CreateObject();
        
        if (test_object) {
            // 1. Add null item using cJSON_AddNullToObject
            cJSON_AddNullToObject(test_object, "null_item");
            
            // 2. Check if the null item is indeed null using cJSON_IsNull
            cJSON *null_item = cJSON_GetObjectItem(test_object, "null_item");
            if (null_item && cJSON_IsNull(null_item)) {
                // Null check successful
            }
            
            // 3. Create a false boolean item
            cJSON_AddFalseToObject(test_object, "false_item");
            
            // 4. Check if the false item is indeed false using cJSON_IsFalse
            cJSON *false_item = cJSON_GetObjectItem(test_object, "false_item");
            if (false_item && cJSON_IsFalse(false_item)) {
                // False check successful
            }
            
            // 5. Create a string reference using cJSON_CreateStringReference
            const char *test_string = "test_string_reference";
            cJSON *string_ref = cJSON_CreateStringReference(test_string);
            
            if (string_ref) {
                // Add the string reference to our test object
                cJSON_AddItemToObject(test_object, "string_ref", string_ref);
                
                // Create a duplicate of the original parsed JSON for comparison
                cJSON *parsed_json_duplicate = cJSON_Duplicate(parsed_json, 1);
                
                if (parsed_json_duplicate) {
                    // 6. Compare the original parsed JSON with its duplicate using cJSON_Compare
                    // Case sensitive comparison
                    cJSON_bool compare_result = cJSON_Compare(parsed_json, parsed_json_duplicate, 1);
                    
                    // Also try case-insensitive comparison
                    cJSON_bool compare_result_case_insensitive = cJSON_Compare(parsed_json, parsed_json_duplicate, 0);
                    
                    // Clean up duplicate
                    cJSON_Delete(parsed_json_duplicate);
                }
            }
            
            // Create another test object for comparison
            cJSON *test_object2 = cJSON_CreateObject();
            if (test_object2) {
                cJSON_AddNullToObject(test_object2, "null_item");
                cJSON_AddFalseToObject(test_object2, "false_item");
                
                // Compare the two test objects
                cJSON_bool test_objects_equal = cJSON_Compare(test_object, test_object2, 1);
                
                // Make them different and compare again
                cJSON_AddTrueToObject(test_object2, "extra_item");
                cJSON_bool test_objects_different = cJSON_Compare(test_object, test_object2, 1);
                
                // Clean up second test object
                cJSON_Delete(test_object2);
            }
            
            // Try to extract and manipulate the original parsed JSON
            if (cJSON_IsObject(parsed_json)) {
                // Get the widget object
                cJSON *widget = cJSON_GetObjectItem(parsed_json, "widget");
                if (widget && cJSON_IsObject(widget)) {
                    // Get the debug item
                    cJSON *debug = cJSON_GetObjectItem(widget, "debug");
                    if (debug && cJSON_IsString(debug)) {
                        // Create a string reference for debug value
                        const char *debug_value = debug->valuestring;
                        cJSON *debug_ref = cJSON_CreateStringReference(debug_value);
                        
                        if (debug_ref) {
                            // Add the debug reference to test object
                            cJSON_AddItemToObject(test_object, "debug_ref", debug_ref);
                        }
                    }
                    
                    // Check for false values in the original JSON
                    // The sample JSON doesn't have false values, but we can check anyway
                    cJSON *child = widget->child;
                    while (child) {
                        if (cJSON_IsFalse(child)) {
                            // Found a false value
                        }
                        child = child->next;
                    }
                }
            }
            
            // Try printing the test object
            char *test_print = cJSON_Print(test_object);
            if (test_print) {
                // Use the printed string if needed
                free(test_print);
            }
            
            // Clean up test object
            cJSON_Delete(test_object);
        }
        
        // Clean up parsed JSON
        cJSON_Delete(parsed_json);
    }
    
    // Also test parsing with other variants
    const char *parse_end = NULL;
    cJSON *parsed_json2 = cJSON_ParseWithLengthOpts(json_string, size, &parse_end, 1);
    if (parsed_json2) {
        cJSON_Delete(parsed_json2);
    }
    
    // Test with require_null_terminated = 0
    parse_end = NULL;
    cJSON *parsed_json3 = cJSON_ParseWithLengthOpts(json_string, size, &parse_end, 0);
    if (parsed_json3) {
        cJSON_Delete(parsed_json3);
    }
    
    // Free allocated resources
    free(json_string);
    
    return 0;
}