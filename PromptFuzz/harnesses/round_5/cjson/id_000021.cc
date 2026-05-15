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
//<ID> 62
//<Prompt> []
/*<Combination>: [
*/
//<score> 29, nr_unique_branch: 1
//<Quality> {"density":29,"unique_branches":{"cJSON_CreateStringArray":[[2774,14,2774,22,0,0,4,1]]},"library_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateBool","cJSON_IsBool","cJSON_AddItemToObjectCS","cJSON_CreateStringArray","cJSON_AddItemToObject","cJSON_AddNumberToObject","cJSON_AddStringToObject","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsBool","cJSON_GetArrayItem","cJSON_CreateArrayReference","cJSON_AddItemToObjectCS","cJSON_GetObjectItem","cJSON_SetNumberHelper","cJSON_Print","cJSON_Parse","cJSON_Delete","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateObject","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateBool","cJSON_IsBool","cJSON_AddItemToObjectCS","cJSON_CreateStringArray","cJSON_AddItemToObject","cJSON_AddNumberToObject","cJSON_AddStringToObject","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsBool","cJSON_GetArrayItem","cJSON_CreateArrayReference","cJSON_AddItemToObjectCS","cJSON_GetObjectItem","cJSON_SetNumberHelper","cJSON_Print","cJSON_Parse","cJSON_Delete","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":1}
/*Looking at the provided input data and the required APIs, I'll create a fuzz driver that performs operations to process and manipulate JSON data. Here's the implementation:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse the input JSON data
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
	return 0;
	}
	
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *parsed = cJSON_Parse(json_str);
    
    // 2. Create a new object to hold our processed data
    cJSON *new_obj = cJSON_CreateObject();
    
    if (parsed && new_obj) {
        // 3. Check if parsed data is an array
        if (cJSON_IsArray(parsed)) {
            // 4. Get array size and process each item
            int array_size = cJSON_GetArraySize(parsed);
            
            // 5. Create a string array from city names
            const char **cities = (const char **)malloc(array_size * sizeof(const char *));
            if (cities) {
                for (int i = 0; i < array_size; i++) {
                    cJSON *item = cJSON_GetArrayItem(parsed, i);
                    if (item) {
                        // 6. Get object items
                        cJSON *city_item = cJSON_GetObjectItem(item, "City");
                        if (city_item && cJSON_IsString(city_item)) {
                            cities[i] = city_item->valuestring;
                        } else {
                            cities[i] = "Unknown";
                        }
                        
                        // 7. Check boolean-like fields (treat precision "zip" as true)
                        cJSON *precision_item = cJSON_GetObjectItem(item, "precision");
                        if (precision_item && cJSON_IsString(precision_item)) {
                            // This is not a boolean, but we'll check it anyway
                            // For demonstration, we'll create a boolean field
                            cJSON *is_zip = cJSON_CreateBool(strcmp(precision_item->valuestring, "zip") == 0);
                            if (cJSON_IsBool(is_zip)) {
                                cJSON_AddItemToObjectCS(item, "IsZipPrecision", is_zip);
                            }
                        }
                    }
                }
                
                // 8. Create a string array from cities
                cJSON *city_array = cJSON_CreateStringArray(cities, array_size);
                if (city_array) {
                    cJSON_AddItemToObject(new_obj, "AllCities", city_array);
                }
                
                free(cities);
            }
            
            // 9. Add number to object
            cJSON_AddNumberToObject(new_obj, "ItemCount", array_size);
            
            // 10. Add string to object
            cJSON_AddStringToObject(new_obj, "DataSource", "FuzzerInput");
            
            // 11. Create true value and add to object
            cJSON *true_value = cJSON_CreateTrue();
            cJSON_AddItemToObject(new_obj, "IsProcessed", true_value);
            
            // 12. Check if IsProcessed is a boolean
            cJSON *bool_check = cJSON_GetObjectItem(new_obj, "IsProcessed");
            if (bool_check && cJSON_IsBool(bool_check)) {
                // Boolean check passed - we could do something here
            }
            
            // 13. Create array reference to first item
            if (array_size > 0) {
                cJSON *first_item = cJSON_GetArrayItem(parsed, 0);
                if (first_item) {
                    cJSON *array_ref = cJSON_CreateArrayReference(first_item);
                    cJSON_AddItemToObjectCS(new_obj, "FirstItemRef", array_ref);
                }
            }
            
            // 14. Modify a number value using SetNumberHelper
            cJSON *count_item = cJSON_GetObjectItem(new_obj, "ItemCount");
            if (count_item) {
                cJSON_SetNumberHelper(count_item, array_size * 2.0);
            }
        }
        
        // 15. Print the new object
        char *printed = cJSON_Print(new_obj);
        if (printed) {
            // Could write to output file if needed
            // FILE *out_file = fopen("output_file", "wb");
            // if (out_file) {
            //     fwrite(printed, strlen(printed), 1, out_file);
            //     assert_file_closed(&out_file);;
            // }
            
            // For fuzzing, we just want to exercise the code
            // We'll parse it back to ensure it's valid JSON
            cJSON *reparsed = cJSON_Parse(printed);
            if (reparsed) {
                cJSON_Delete(reparsed);
            }
            
            cJSON_free(printed);
        }
    }
    
    // 16. Clean up
    if (parsed) {
        cJSON_Delete(parsed);
    }
    if (new_obj) {
        cJSON_Delete(new_obj);
    }
    free(json_str);
    
    return 0;
}