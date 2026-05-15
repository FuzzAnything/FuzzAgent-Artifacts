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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 147, nr_unique_branch: 8
//<Quality> {"density":21,"unique_branches":{"cJSON_AddStringToObject":[[2213,9,2213,76,0,0,4,0]],"cJSON_CreateBool":[[2499,22,2499,29,0,0,4,1]],"cJSON_IsTrue":[[2994,9,2994,21,0,0,4,1]],"cJSON_IsNull":[[3014,9,3014,21,0,0,4,1]],"cJSON_InsertItemInArray":[[2349,9,2349,39,0,0,4,0],[2349,43,2349,71,0,0,4,1],[2357,9,2357,39,0,0,4,1]],"cJSON_IsFalse":[[2984,9,2984,21,0,0,4,1]]},"library_calls":["cJSON_Parse","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNull","cJSON_AddItemToArray","cJSON_CreateTrue","cJSON_AddItemToArray","cJSON_CreateFalse","cJSON_AddItemToArray","cJSON_CreateString","cJSON_GetArrayItem","cJSON_IsNull","cJSON_CreateString","cJSON_ReplaceItemInArray","cJSON_GetArrayItem","cJSON_IsTrue","cJSON_CreateBool","cJSON_InsertItemInArray","cJSON_GetArrayItem","cJSON_IsFalse","cJSON_AddItemReferenceToArray","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateArray","cJSON_AddItemToArray","cJSON_CreateNull","cJSON_AddItemToArray","cJSON_CreateTrue","cJSON_AddItemToArray","cJSON_CreateFalse","cJSON_AddItemToArray","cJSON_CreateString","cJSON_GetArrayItem","cJSON_IsNull","cJSON_CreateString","cJSON_ReplaceItemInArray","cJSON_GetArrayItem","cJSON_IsTrue","cJSON_CreateBool","cJSON_InsertItemInArray","cJSON_GetArrayItem","cJSON_IsFalse","cJSON_AddItemReferenceToArray","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


// Include the cJSON header file

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE pointer from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read the JSON data from the file
    fseek(in_file, 0, SEEK_END);
    long fsize = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *json_string = (char *)malloc(fsize + 1);
    if (!json_string) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    fread(json_string, 1, fsize, in_file);
    json_string[fsize] = '\0';
    assert_file_closed(&in_file);;
    
    // Parse the JSON string
    cJSON *root = cJSON_Parse(json_string);
    free(json_string);
    
    if (!root) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create an array for testing
    cJSON *test_array = cJSON_CreateArray();
    
    // Add various items to the array
    cJSON_AddItemToArray(test_array, cJSON_CreateNull());
    cJSON_AddItemToArray(test_array, cJSON_CreateTrue());
    cJSON_AddItemToArray(test_array, cJSON_CreateFalse());
    cJSON_AddItemToArray(test_array, cJSON_CreateString("test"));
    
    // Check if items are null, true, or false
    cJSON *array_item;
    
    // Check first item (should be null)
    array_item = cJSON_GetArrayItem(test_array, 0);
    if (cJSON_IsNull(array_item)) {
        // Replace the null item with a new string
        cJSON *new_string = cJSON_CreateString("replaced");
        cJSON_ReplaceItemInArray(test_array, 0, new_string);
    }
    
    // Check second item (should be true)
    array_item = cJSON_GetArrayItem(test_array, 1);
    if (cJSON_IsTrue(array_item)) {
        // Insert a new boolean at position 1
        cJSON *new_bool = cJSON_CreateBool(0);
        cJSON_InsertItemInArray(test_array, 1, new_bool);
    }
    
    // Check third item (now at position 2, should be false)
    array_item = cJSON_GetArrayItem(test_array, 2);
    if (cJSON_IsFalse(array_item)) {
        // Add a reference to this item at the end of array
        cJSON_AddItemReferenceToArray(test_array, array_item);
    }
    
    // Create an object and add a string to it
    cJSON *test_object = cJSON_CreateObject();
    cJSON_AddStringToObject(test_object, "fuzz_key", "fuzz_value");
    
    // Clean up - delete all created JSON objects
    cJSON_Delete(root);
    cJSON_Delete(test_array);
    cJSON_Delete(test_object);
    
    assert_file_closed(&in_file);
	return 0;
}