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
//<ID> 74
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_AddFalseToObject","cJSON_PrintUnformatted","cJSON_free","cJSON_GetObjectItemCaseSensitive","cJSON_IsFalse","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_GetArrayItem","cJSON_GetNumberValue","cJSON_DetachItemFromArray","cJSON_InsertItemInArray","cJSON_Delete","cJSON_CreateObject","cJSON_CreateString","cJSON_AddItemToObject","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_AddFalseToObject","cJSON_PrintUnformatted","cJSON_free","cJSON_GetObjectItemCaseSensitive","cJSON_IsFalse","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_CreateString","cJSON_GetArrayItem","cJSON_GetNumberValue","cJSON_DetachItemFromArray","cJSON_InsertItemInArray","cJSON_Delete","cJSON_CreateObject","cJSON_CreateString","cJSON_AddItemToObject","cJSON_DetachItemViaPointer","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


// Include cJSON headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // If size is 0, there's nothing to parse
    if (size == 0) {
        return 0;
    }

    // Parse input data as JSON
    cJSON *parsed_json = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (parsed_json == nullptr) {
        return 0;
    }

    // 1. Create a new object and add a false value to it
    cJSON *new_object = cJSON_CreateObject();
    cJSON_AddFalseToObject(new_object, "fuzzer_added_false");

    // 2. Print the new object unformatted
    char *unformatted_str = cJSON_PrintUnformatted(new_object);
    if (unformatted_str != nullptr) {
        // Write to output file as required
        FILE *out_file = fopen("output_file", "wb");
        if (out_file != nullptr) {
            fwrite(unformatted_str, strlen(unformatted_str), 1, out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(unformatted_str);
    }

    // 3. Check if the added value is false
    cJSON *false_item = cJSON_GetObjectItemCaseSensitive(new_object, "fuzzer_added_false");
    if (false_item != nullptr) {
        cJSON_bool is_false = cJSON_IsFalse(false_item);
        (void)is_false; // Use variable to avoid unused warning
    }

    // 4. Create an array and add items to it
    cJSON *array = cJSON_CreateArray();
    if (array != nullptr) {
        // Add some items to the array
        cJSON_AddItemToArray(array, cJSON_CreateNumber(42.0));
        cJSON_AddItemToArray(array, cJSON_CreateString("fuzz_string"));
        
        // 5. Get a number value from the array
        cJSON *number_item = cJSON_GetArrayItem(array, 0);
        if (number_item != nullptr) {
            double num_val = cJSON_GetNumberValue(number_item);
            (void)num_val; // Use variable to avoid unused warning
        }

        // 6. Detach an item from array by index
        cJSON *detached_item = cJSON_DetachItemFromArray(array, 0);
        if (detached_item != nullptr) {
            // 7. Insert item back into array at different position
            cJSON_InsertItemInArray(array, 1, detached_item);
        }

        // Clean up the array
        cJSON_Delete(array);
    }

    // 8. For DetachItemViaPointer, we need a parent-child relationship
    // Create a parent object with a child
    cJSON *parent = cJSON_CreateObject();
    cJSON *child = cJSON_CreateString("child_value");
    cJSON_AddItemToObject(parent, "child_key", child);
    
    // Detach using pointer
    cJSON *detached_via_pointer = cJSON_DetachItemViaPointer(parent, child);
    if (detached_via_pointer != nullptr) {
        cJSON_Delete(detached_via_pointer);
    }
    cJSON_Delete(parent);

    // Clean up
    cJSON_Delete(new_object);
    cJSON_Delete(parsed_json);
    
    return 0;
}