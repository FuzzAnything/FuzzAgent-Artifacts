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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"cJSON_InsertItemInArray":[[2344,9,2344,31,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_IsArray","cJSON_GetArrayItem","cJSON_IsObject","cJSON_CreateNumber","cJSON_InsertItemInArray","cJSON_GetArrayItem","cJSON_CreateNumber","cJSON_ReplaceItemViaPointer","cJSON_CreateDoubleArray","cJSON_AddItemToArray","cJSON_Print","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_IsArray","cJSON_GetArrayItem","cJSON_IsObject","cJSON_CreateNumber","cJSON_InsertItemInArray","cJSON_GetArrayItem","cJSON_CreateNumber","cJSON_ReplaceItemViaPointer","cJSON_CreateDoubleArray","cJSON_AddItemToArray","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create a FILE* from input data for potential use
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Parse input data as JSON
    char *json_str = new char[size + 1];
    if (!json_str) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *parsed_json = cJSON_ParseWithLength(json_str, size);
    
    // 3. Check if parsing succeeded and we have an array
    if (parsed_json && cJSON_IsArray(parsed_json)) {
        // 4. Get first array item using cJSON_GetArrayItem
        cJSON *array_item = cJSON_GetArrayItem(parsed_json, 0);
        
        // 5. Check if the item is an object (required API call)
        cJSON_bool is_obj = cJSON_IsObject(array_item);
        
        // 6. Create a new number for insertion
        cJSON *new_number = cJSON_CreateNumber(42.5);
        
        if (new_number) {
            // 7. Insert the new number into the array at position 1
            cJSON_InsertItemInArray(parsed_json, 1, new_number);
            
            // 8. Get the item we just inserted to replace it
            cJSON *inserted_item = cJSON_GetArrayItem(parsed_json, 1);
            
            if (inserted_item) {
                // 9. Create replacement number
                cJSON *replacement = cJSON_CreateNumber(99.9);
                
                if (replacement) {
                    // 10. Replace the inserted item using cJSON_ReplaceItemViaPointer
                    cJSON_ReplaceItemViaPointer(parsed_json, inserted_item, replacement);
                }
            }
        }
        
        // 11. Create a double array (required API call)
        double double_array_data[] = {1.1, 2.2, 3.3};
        cJSON *double_array = cJSON_CreateDoubleArray(double_array_data, 3);
        
        if (double_array) {
            // Add the double array to the main array
            cJSON_AddItemToArray(parsed_json, double_array);
        }
        
        // 12. Create output file for demonstration
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            // Print the modified JSON
            char *printed = cJSON_Print(parsed_json);
            if (printed) {
                fwrite(printed, strlen(printed), 1, out_file);
                cJSON_free(printed);
            }
            assert_file_closed(&out_file);;
        }
    }
    
    // 13. Clean up all resources
    if (parsed_json) {
        cJSON_Delete(parsed_json);
    }
    delete[] json_str;
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}