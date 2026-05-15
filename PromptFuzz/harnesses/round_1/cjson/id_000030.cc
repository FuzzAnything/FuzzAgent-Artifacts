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
//<ID> 110
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"cJSON_Duplicate_rec":[[2797,9,2797,14,0,0,4,0],[2867,9,2867,24,0,0,4,1]]},"library_calls":["cJSON_ParseWithLength","cJSON_IsInvalid","cJSON_AddObjectToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_Duplicate","cJSON_ReplaceItemViaPointer","cJSON_CreateArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateNull","cJSON_AddItemToArray","cJSON_DetachItemFromArray","cJSON_AddItemReferenceToArray","cJSON_IsNull","cJSON_Delete","cJSON_AddRawToObject","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_DetachItemViaPointer","cJSON_Print","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_IsInvalid","cJSON_AddObjectToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_Duplicate","cJSON_ReplaceItemViaPointer","cJSON_CreateArray","cJSON_CreateString","cJSON_AddItemToArray","cJSON_CreateNull","cJSON_AddItemToArray","cJSON_DetachItemFromArray","cJSON_AddItemReferenceToArray","cJSON_IsNull","cJSON_Delete","cJSON_AddRawToObject","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_DetachItemViaPointer","cJSON_Print","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for safety
    char* input = static_cast<char*>(malloc(size + 1));
    if (!input) {
        return 0;
    }
    memcpy(input, data, size);
    input[size] = '\0';

    // 1. Parse input using cJSON_ParseWithLength
    cJSON* parsed = cJSON_ParseWithLength(input, size);
    if (!parsed) {
        free(input);
        return 0;
    }

    // 2. Check if parsed is invalid
    cJSON_bool is_invalid = cJSON_IsInvalid(parsed);

    // 3. Add new object to parsed object
    cJSON* new_obj = cJSON_AddObjectToObject(parsed, "new_object");

    // 4. Create float array and add to new object
    float float_values[] = {1.1f, 2.2f, 3.3f};
    cJSON* float_array = cJSON_CreateFloatArray(float_values, 3);
    if (float_array) {
        cJSON_AddItemToObject(new_obj, "float_array", float_array);
    }

    // 5. Get object item
    cJSON* widget_item = cJSON_GetObjectItem(parsed, "widget");
    
    // 6. Check if widget is object
    cJSON_bool is_object = cJSON_IsObject(widget_item);

    // 7. Duplicate widget item
    cJSON* duplicated_widget = cJSON_Duplicate(widget_item, 1); // recursive

    // 8. Replace widget with duplicated version
    if (duplicated_widget && widget_item) {
        cJSON_ReplaceItemViaPointer(parsed, widget_item, duplicated_widget);
    }

    // 9. Create new array
    cJSON* new_array = cJSON_CreateArray();

    // 10. Create string and add to array
    cJSON* string_item = cJSON_CreateString("test_string");
    if (string_item && new_array) {
        cJSON_AddItemToArray(new_array, string_item);
    }

    // 11. Create null and add to array
    cJSON* null_item = cJSON_CreateNull();
    if (null_item && new_array) {
        cJSON_AddItemToArray(new_array, null_item);
    }

    // 12. Detach item from array
    cJSON* detached_item = NULL;
    if (new_array) {
        detached_item = cJSON_DetachItemFromArray(new_array, 0);
    }

    // 13. Add reference to array
    if (detached_item && new_array) {
        cJSON_AddItemReferenceToArray(new_array, detached_item);
    }

    // 14. Check if detached item is null
    cJSON_bool is_null = cJSON_IsNull(detached_item);

    // 15. Delete detached item
    if (detached_item) {
        cJSON_Delete(detached_item);
    }

    // 16. Add raw data to parsed object
    if (parsed) {
        cJSON_AddRawToObject(parsed, "raw_data", "{\"raw\": true}");
    }

    // 17. Get array size
    int array_size = 0;
    if (new_array) {
        array_size = cJSON_GetArraySize(new_array);
    }

    // 18. Get array item
    cJSON* array_item = NULL;
    if (new_array && array_size > 0) {
        array_item = cJSON_GetArrayItem(new_array, 0);
    }

    // 19. Detach item via pointer
    cJSON* detached_via_pointer = NULL;
    if (new_array && array_item) {
        detached_via_pointer = cJSON_DetachItemViaPointer(new_array, array_item);
    }

    // 20. Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Print parsed JSON to file
        char* json_string = cJSON_Print(parsed);
        if (json_string) {
            fputs(json_string, out_file);
            free(json_string);
        }
        assert_file_closed(&out_file);;
    }

    // Clean up
    if (new_array) {
        cJSON_Delete(new_array);
    }
    if (detached_via_pointer) {
        cJSON_Delete(detached_via_pointer);
    }
    cJSON_Delete(parsed);
    free(input);

    assert_file_closed(&out_file);
	return 0;
}