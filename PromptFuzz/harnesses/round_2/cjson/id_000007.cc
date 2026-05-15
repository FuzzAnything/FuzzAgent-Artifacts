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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 1
//<Quality> {"density":19,"unique_branches":{"cJSON_Compare":[[3074,24,3074,35,0,0,4,0]]},"library_calls":["cJSON_Version","cJSON_ParseWithLength","cJSON_IsArray","cJSON_CreateObject","cJSON_Delete","cJSON_AddStringToObject","cJSON_HasObjectItem","cJSON_CreateArrayReference","cJSON_AddArrayToObject","cJSON_AddItemToArray","cJSON_Print","cJSON_Minify","cJSON_Compare","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Version","cJSON_ParseWithLength","cJSON_IsArray","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_HasObjectItem","cJSON_CreateArrayReference","cJSON_AddArrayToObject","cJSON_AddItemToArray","cJSON_Print","cJSON_Minify","cJSON_Compare","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":1}
/*Looking at the input data, I need to create a fuzz driver that uses the specified cJSON APIs. The input appears to be a 3x3 matrix in JSON array format.

*/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a null-terminated string from input data for JSON parsing
    if (size == 0) {
	return 0;
	}
	
    
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
	return 0;
	}
	
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    // 1. Get cJSON version
    const char *version = cJSON_Version();
    if (!version) {
        free(json_str);
        return 0;
    }
    
    // 2. Parse the JSON input (which should be an array of arrays)
    cJSON *parsed = cJSON_ParseWithLength(json_str, size);
    if (!parsed) {
        free(json_str);
        return 0;
    }
    
    // 3. Check if the parsed JSON is an array
    cJSON_bool is_array = cJSON_IsArray(parsed);
    
    // 4. Create a new object to work with
    cJSON *root_object = cJSON_CreateObject();
    if (!root_object) {
        cJSON_Delete(parsed);
        free(json_str);
        return 0;
    }
    
    // 5. Add a string to the object (using version info)
    cJSON *version_string = cJSON_AddStringToObject(root_object, "cjson_version", version);
    
    // 6. Check if the object has the version item
    cJSON_bool has_version = cJSON_HasObjectItem(root_object, "cjson_version");
    
    // 7. Create an array reference from the parsed input
    cJSON *array_ref = NULL;
    if (is_array) {
        array_ref = cJSON_CreateArrayReference(parsed);
    }
    
    // 8. Add an array to the object
    cJSON *new_array = cJSON_AddArrayToObject(root_object, "matrix");
    if (new_array && is_array) {
        // Add the parsed array to the new array (as reference)
        cJSON_AddItemToArray(new_array, array_ref);
    }
    
    // 9. Print the object to a string and minify it
    char *printed = cJSON_Print(root_object);
    if (printed) {
        cJSON_Minify(printed);  // Minify in-place
        // Don't forget to free printed later
    }
    
    // 10. Compare the original parsed array with the array reference
    cJSON_bool are_equal = cJSON_Compare(parsed, array_ref, true);
    
    // 11. Detach an item via pointer if we have items in the array
    if (is_array && cJSON_GetArraySize(parsed) > 0) {
        cJSON *first_item = cJSON_GetArrayItem(parsed, 0);
        if (first_item) {
            cJSON *detached = cJSON_DetachItemViaPointer(parsed, first_item);
            if (detached) {
                // Clean up the detached item
                cJSON_Delete(detached);
            }
        }
    }
    
    // Create a file for output (as per requirement 6)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        if (printed) {
            fwrite(printed, strlen(printed), 1, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up all allocated resources
    if (printed) {
        free(printed);
    }
    cJSON_Delete(root_object);
    cJSON_Delete(parsed);
    free(json_str);
    
    // array_ref is a reference, doesn't need to be deleted separately
    // version points to static string, doesn't need freeing
    
    assert_file_closed(&out_file);
	return 0;
}