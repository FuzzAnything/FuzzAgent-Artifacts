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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 4
//<Quality> {"density":22,"unique_branches":{"cJSON_AddItemReferenceToArray":[[2132,9,2132,22,0,0,4,1]],"cJSON_ReplaceItemViaPointer":[[2387,9,2387,30,0,0,4,1],[2400,13,2400,38,0,0,4,0],[2404,13,2404,38,0,0,4,1]]},"library_calls":["cJSON_Parse","cJSON_CreateArray","cJSON_Delete","cJSON_CreateTrue","cJSON_CreateFalse","cJSON_CreateNull","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_CreateString","cJSON_GetArrayItem","cJSON_GetArrayItem","cJSON_GetArrayItem","cJSON_IsTrue","cJSON_IsFalse","cJSON_IsNull","cJSON_CreateNumber","cJSON_InsertItemInArray","cJSON_CreateBool","cJSON_ReplaceItemInArray","cJSON_GetArrayItem","cJSON_AddItemReferenceToArray","cJSON_AddStringToObject","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateArray","cJSON_CreateTrue","cJSON_CreateFalse","cJSON_CreateNull","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_CreateString","cJSON_GetArrayItem","cJSON_GetArrayItem","cJSON_GetArrayItem","cJSON_IsTrue","cJSON_IsFalse","cJSON_IsNull","cJSON_CreateNumber","cJSON_InsertItemInArray","cJSON_CreateBool","cJSON_ReplaceItemInArray","cJSON_GetArrayItem","cJSON_AddItemReferenceToArray","cJSON_AddStringToObject","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is empty
    if (size == 0) {
        return 0;
    }

    // Create null-terminated string from input data
    char *input_str = (char *)malloc(size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, data, size);
    input_str[size] = '\0';

    // Parse JSON from input
    cJSON *root = cJSON_Parse(input_str);
    if (!root) {
        free(input_str);
        return 0;
    }

    // Create a new array for testing
    cJSON *test_array = cJSON_CreateArray();
    if (!test_array) {
        cJSON_Delete(root);
        free(input_str);
        return 0;
    }

    // Add various items to the array
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *null_item = cJSON_CreateNull();
    
    cJSON_AddItemToArray(test_array, true_item);
    cJSON_AddItemToArray(test_array, false_item);
    cJSON_AddItemToArray(test_array, null_item);
    cJSON_AddItemToArray(test_array, cJSON_CreateString("test_string"));

    // Check types using the specified APIs
    cJSON *item0 = cJSON_GetArrayItem(test_array, 0);
    cJSON *item1 = cJSON_GetArrayItem(test_array, 1);
    cJSON *item2 = cJSON_GetArrayItem(test_array, 2);
    
    if (item0 && cJSON_IsTrue(item0)) {
        // Valid true item
    }
    
    if (item1 && cJSON_IsFalse(item1)) {
        // Valid false item
    }
    
    if (item2 && cJSON_IsNull(item2)) {
        // Valid null item
    }

    // Insert a new item at index 1
    cJSON *new_item = cJSON_CreateNumber(42.5);
    if (new_item) {
        cJSON_InsertItemInArray(test_array, 1, new_item);
    }

    // Replace item at index 2
    cJSON *replacement = cJSON_CreateBool(1); // true
    if (replacement) {
        cJSON_ReplaceItemInArray(test_array, 2, replacement);
    }

    // Add reference to existing item
    cJSON *ref_item = cJSON_GetArrayItem(test_array, 0);
    if (ref_item) {
        cJSON_AddItemReferenceToArray(test_array, ref_item);
    }

    // Add string to object (add test_array to root object with a name)
    cJSON_AddStringToObject(root, "fuzzer_test", "test_value");

    // Cleanup
    cJSON_Delete(test_array);
    cJSON_Delete(root);
    free(input_str);
    
    return 0;
}