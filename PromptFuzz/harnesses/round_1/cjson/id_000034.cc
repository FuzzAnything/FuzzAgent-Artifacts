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
//<ID> 133
//<Prompt> []
/*<Combination>: [
*/
//<score> 49.5, nr_unique_branch: 2
//<Quality> {"density":33,"unique_branches":{"cJSON_SetValuestring":[[441,29,441,59,0,0,4,0]],"case_insensitive_strcmp":[[140,9,140,27,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_Delete","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_AddNumberToObject","cJSON_AddStringToObject","cJSON_AddTrueToObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetStringValue","cJSON_CreateString","cJSON_AddItemToArray","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_SetValuestring","cJSON_CreateObject","cJSON_CreateTrue","cJSON_AddItemToObjectCS","cJSON_GetObjectItem","cJSON_IsTrue","cJSON_Print","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_AddNumberToObject","cJSON_AddStringToObject","cJSON_AddTrueToObject","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetStringValue","cJSON_CreateString","cJSON_AddItemToArray","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsNumber","cJSON_SetValuestring","cJSON_CreateObject","cJSON_CreateTrue","cJSON_AddItemToObjectCS","cJSON_GetObjectItem","cJSON_IsTrue","cJSON_Print","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse input JSON using cJSON_ParseWithLength
    cJSON *parsed = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (parsed == nullptr) {
        return 0;
    }

    // 2. Create a new object
    cJSON *new_obj = cJSON_CreateObject();
    if (new_obj == nullptr) {
        cJSON_Delete(parsed);
        return 0;
    }

    // 3. Add items to the new object
    cJSON *num_item = cJSON_CreateNumber(42.5);
    if (num_item != nullptr) {
        cJSON_AddItemToObject(new_obj, "test_number", num_item);
    }
    
    cJSON_AddNumberToObject(new_obj, "direct_number", 99.9);
    cJSON_AddStringToObject(new_obj, "test_string", "fuzz_string");
    cJSON_AddTrueToObject(new_obj, "test_bool");

    // 4. Create an array
    cJSON *array = cJSON_CreateArray();
    if (array != nullptr) {
        // Add parsed JSON to array
        cJSON_AddItemToArray(array, cJSON_Duplicate(parsed, 1));
        // Add new object to array
        cJSON_AddItemToArray(array, cJSON_Duplicate(new_obj, 1));
        
        // Add some numbers to array
        cJSON_AddItemToArray(array, cJSON_CreateNumber(1.0));
        cJSON_AddItemToArray(array, cJSON_CreateNumber(2.0));
        cJSON_AddItemToArray(array, cJSON_CreateNumber(3.0));
    }

    // 5. Get object items from parsed JSON
    cJSON *widget = cJSON_GetObjectItem(parsed, "widget");
    if (widget != nullptr) {
        cJSON *debug = cJSON_GetObjectItem(widget, "debug");
        if (debug != nullptr && cJSON_IsString(debug)) {
            char *debug_str = cJSON_GetStringValue(debug);
            // Create a new string based on debug value
            cJSON *new_string = cJSON_CreateString(debug_str);
            if (new_string != nullptr) {
                cJSON_AddItemToArray(array, new_string);
            }
        }

        cJSON *window = cJSON_GetObjectItem(widget, "window");
        if (window != nullptr) {
            cJSON *width = cJSON_GetObjectItem(window, "width");
            if (width != nullptr && cJSON_IsNumber(width)) {
                // Modify width value
                cJSON_SetValuestring(width, "modified");
            }
        }
    }

    // 6. Create another object with case-sensitive addition
    cJSON *cs_obj = cJSON_CreateObject();
    if (cs_obj != nullptr) {
        cJSON *true_item = cJSON_CreateTrue();
        if (true_item != nullptr) {
            cJSON_AddItemToObjectCS(cs_obj, "CaseSensitiveKey", true_item);
            
            // Check if it's true
            cJSON *check_item = cJSON_GetObjectItem(cs_obj, "CaseSensitiveKey");
            if (check_item != nullptr && cJSON_IsTrue(check_item)) {
                // Success - item is true
            }
        }
    }

    // 7. Print everything
    char *json_str = cJSON_Print(array);
    if (json_str != nullptr) {
        // Use FILE* for writing output
        FILE *out_file = fopen("output_file", "wb");
        if (out_file != nullptr) {
            int fd = fuzz_fileno(out_file);
            fwrite(json_str, strlen(json_str), 1, out_file);
            assert_file_closed(&out_file);;
        }
        free(json_str);
    }

    // 8. Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(new_obj);
    if (array != nullptr) {
        cJSON_Delete(array);
    }
    if (cs_obj != nullptr) {
        cJSON_Delete(cs_obj);
    }

    return 0;
}