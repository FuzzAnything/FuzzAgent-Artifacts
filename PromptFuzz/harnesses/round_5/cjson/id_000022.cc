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
//<ID> 64
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 2
//<Quality> {"density":28,"unique_branches":{"case_insensitive_strcmp":[[140,9,140,27,0,0,4,0]],"print_object":[[1791,9,1791,52,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_Delete","cJSON_CreateStringArray","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_AddItemToObjectCS","cJSON_AddItemToObjectCS","cJSON_AddStringToObject","cJSON_AddNumberToObject","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_IsString","cJSON_GetStringValue","cJSON_GetObjectItem","cJSON_IsBool","cJSON_CreateArrayReference","cJSON_AddItemToObject","cJSON_Print","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_CreateStringArray","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_AddItemToObjectCS","cJSON_AddItemToObjectCS","cJSON_AddStringToObject","cJSON_AddNumberToObject","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_IsString","cJSON_GetStringValue","cJSON_GetObjectItem","cJSON_IsBool","cJSON_CreateArrayReference","cJSON_AddItemToObject","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":1}
/*Here's a C++ fuzzer that uses all the specified cJSON APIs to process JSON data:

*/


// Include cJSON headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse input data as JSON
    char *json_string = (char*)malloc(size + 1);
    if (!json_string) {
	return 0;
	}
	
    memcpy(json_string, data, size);
    json_string[size] = '\0';
    
    cJSON *parsed_json = cJSON_Parse(json_string);
    free(json_string);
    
    if (!parsed_json) {
        return 0;  // Invalid JSON
    }
    
    // 2. Create a new object
    cJSON *root_object = cJSON_CreateObject();
    if (!root_object) {
        cJSON_Delete(parsed_json);
        return 0;
    }
    
    // 3. Create a string array
    const char *strings[] = {"str1", "str2", "str3"};
    cJSON *string_array = cJSON_CreateStringArray(strings, 3);
    
    // 4. Create a true boolean
    cJSON *true_item = cJSON_CreateTrue();
    
    // 5. Add items to the object using different methods
    cJSON_AddItemToObject(root_object, "parsed_data", parsed_json);
    cJSON_AddItemToObjectCS(root_object, "string_array", string_array);
    cJSON_AddItemToObjectCS(root_object, "bool_flag", true_item);
    
    // 6. Add more items using helper functions
    cJSON_AddStringToObject(root_object, "description", "Fuzzer test");
    cJSON_AddNumberToObject(root_object, "random_number", 42.5);
    
    // 7. Check if parsed_json is an array
    if (cJSON_IsArray(parsed_json)) {
        int array_size = cJSON_GetArraySize(parsed_json);
        
        // Process each item in the array
        for (int i = 0; i < array_size && i < 10; i++) {
            cJSON *array_item = cJSON_GetArrayItem(parsed_json, i);
            
            if (array_item && cJSON_IsObject(array_item)) {
                // Get specific fields from the object
                cJSON *city_item = cJSON_GetObjectItem(array_item, "City");
                cJSON *lat_item = cJSON_GetObjectItem(array_item, "Latitude");
                cJSON *precision_item = cJSON_GetObjectItem(array_item, "precision");
                
                // Check types
                if (lat_item && cJSON_IsNumber(lat_item)) {
                    // Modify the number value
                    double new_lat = lat_item->valuedouble + 0.1;
                    cJSON_SetNumberHelper(lat_item, new_lat);
                }
                
                if (precision_item && cJSON_IsString(precision_item)) {
                    // Get string value
                    char *str_value = cJSON_GetStringValue(precision_item);
                    // str_value could be used here
                }
            }
        }
    }
    
    // 8. Check if boolean flag is indeed boolean
    cJSON *bool_check = cJSON_GetObjectItem(root_object, "bool_flag");
    if (bool_check && cJSON_IsBool(bool_check)) {
        // Boolean check passed
    }
    
    // 9. Create an array reference
    cJSON *array_ref = cJSON_CreateArrayReference(parsed_json);
    if (array_ref) {
        cJSON_AddItemToObject(root_object, "array_reference", array_ref);
    }
    
    // 10. Print the final JSON
    char *printed_json = cJSON_Print(root_object);
    if (printed_json) {
        // Write to file using FILE*
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(printed_json, 1, strlen(printed_json), out_file);
            assert_file_closed(&out_file);;
        }
        
        // Also write to stdout for debugging
        fwrite(printed_json, 1, strlen(printed_json), stdout);
        
        // Free the printed string
        cJSON_free(printed_json);
    }
    
    // 11. Clean up
    cJSON_Delete(root_object);
    
    // Note: parsed_json is now owned by root_object, so don't delete it separately
    
    return 0;
}