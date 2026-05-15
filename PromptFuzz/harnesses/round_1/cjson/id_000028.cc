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
//<ID> 99
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"get_array_item":[[1926,9,1926,22,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_CreateFalse","cJSON_AddItemToObjectCS","cJSON_CreateTrue","cJSON_AddItemToObjectCS","cJSON_GetObjectItem","cJSON_GetArrayItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsTrue","cJSON_CreateNull","cJSON_AddItemToObjectCS","cJSON_CreateIntArray","cJSON_AddItemToObjectCS","cJSON_CreateDoubleArray","cJSON_AddItemToObjectCS","cJSON_CreateStringReference","cJSON_AddItemToObjectCS","cJSON_CreateArrayReference","cJSON_AddItemToObjectCS","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_CreateFalse","cJSON_AddItemToObjectCS","cJSON_CreateTrue","cJSON_AddItemToObjectCS","cJSON_GetObjectItem","cJSON_GetArrayItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsTrue","cJSON_CreateNull","cJSON_AddItemToObjectCS","cJSON_CreateIntArray","cJSON_AddItemToObjectCS","cJSON_CreateDoubleArray","cJSON_AddItemToObjectCS","cJSON_CreateStringReference","cJSON_AddItemToObjectCS","cJSON_CreateArrayReference","cJSON_AddItemToObjectCS","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // First, parse the input data as JSON
    cJSON *parsed = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    
    // Create a new object to add our items to
    cJSON *main_object = cJSON_CreateObject();
    
    // 1. Create and add false value
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObjectCS(main_object, "my_false", false_item);
    
    // 2. Create and add true value
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(main_object, "my_true", true_item);
    
    // 3. Check if the parsed JSON has true values
    if (parsed) {
        // Try to find a true value in the parsed JSON
        cJSON *item = cJSON_GetObjectItem(parsed, "web-app");
        if (item) {
            // Navigate to useJSP field (which is false in the example)
            cJSON *servlet = cJSON_GetArrayItem(cJSON_GetObjectItem(item, "servlet"), 0);
            if (servlet) {
                cJSON *init_param = cJSON_GetObjectItem(servlet, "init-param");
                if (init_param) {
                    cJSON *useJSP = cJSON_GetObjectItem(init_param, "useJSP");
                    if (useJSP) {
                        // Use cJSON_IsTrue API
                        cJSON_IsTrue(useJSP);
                    }
                }
            }
        }
    }
    
    // 4. Create and add null value
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(main_object, "my_null", null_item);
    
    // 5. Create int array (using sample data)
    int int_numbers[] = {1, 2, 3, 4, 5};
    cJSON *int_array = cJSON_CreateIntArray(int_numbers, 5);
    cJSON_AddItemToObjectCS(main_object, "int_array", int_array);
    
    // 6. Create double array (using sample data)
    double double_numbers[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    cJSON *double_array = cJSON_CreateDoubleArray(double_numbers, 5);
    cJSON_AddItemToObjectCS(main_object, "double_array", double_array);
    
    // 7. Create string reference
    const char *test_string = "test string reference";
    cJSON *string_ref = cJSON_CreateStringReference(test_string);
    cJSON_AddItemToObjectCS(main_object, "string_reference", string_ref);
    
    // 8. Create array reference (to the int array we just created)
    cJSON *array_ref = cJSON_CreateArrayReference(int_array);
    cJSON_AddItemToObjectCS(main_object, "array_reference", array_ref);
    
    // Print the result to verify
    char *output = cJSON_Print(main_object);
    if (output) {
        // Write to output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(output, strlen(output), 1, out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(output);
    }
    
    // Clean up
    cJSON_Delete(main_object);
    cJSON_Delete(parsed);
    
    return 0;
}