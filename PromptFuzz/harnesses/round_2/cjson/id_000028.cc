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
//<ID> 169
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_free","cJSON_CreateObjectReference","cJSON_AddItemToObject","cJSON_HasObjectItem","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsTrue","cJSON_CreateString","cJSON_ReplaceItemInObject","cJSON_DetachItemFromObject","cJSON_Duplicate","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_SetValuestring","cJSON_AddItemToObject","cJSON_Delete","cJSON_CreateObject","cJSON_AddTrueToObject","cJSON_GetObjectItem","cJSON_IsTrue","cJSON_CreateString","cJSON_ReplaceItemInObject","cJSON_IsString","cJSON_SetValuestring","cJSON_HasObjectItem","cJSON_DetachItemFromObject","cJSON_CreateObjectReference","cJSON_AddItemToObject","cJSON_Delete","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_ParseWithLength","cJSON_CreateObjectReference","cJSON_AddItemToObject","cJSON_Delete","cJSON_AddItemToObject","cJSON_SetValuestring","cJSON_IsString","cJSON_GetObjectItem","cJSON_GetArrayItem","cJSON_GetArraySize","cJSON_IsArray","cJSON_GetObjectItem","cJSON_Duplicate","cJSON_DetachItemFromObject","cJSON_ReplaceItemInObject","cJSON_CreateString","cJSON_IsTrue","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_HasObjectItem","cJSON_CreateObject","cJSON_AddTrueToObject","cJSON_GetObjectItem","cJSON_IsTrue","cJSON_CreateString","cJSON_ReplaceItemInObject","cJSON_IsString","cJSON_SetValuestring","cJSON_HasObjectItem","cJSON_DetachItemFromObject","cJSON_CreateObjectReference","cJSON_AddItemToObject","cJSON_Delete","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure we have valid input
    if (size == 0 || data == nullptr) {
        return 0;
    }

    // Create null-terminated string for parsing
    char *json_string = (char *)cJSON_malloc(size + 1);
    if (json_string == nullptr) {
        return 0;
    }
    memcpy(json_string, data, size);
    json_string[size] = '\0';

    // Parse JSON from input data
    cJSON *root = cJSON_ParseWithLength(json_string, size);
    if (root == nullptr) {
        cJSON_free(json_string);
        return 0;
    }

    // Event: Manipulate a JSON object using the specified APIs
    // 1. Create an object reference
    cJSON *object_ref = cJSON_CreateObjectReference(root);
    if (object_ref != nullptr) {
        // Add the reference to the original object with a key
        cJSON_AddItemToObject(root, "reference_copy", object_ref);
    }

    // 2. Check for existence of specific items and manipulate them
    const char *keys_to_check[] = {"web-app", "servlet", "servlet-mapping"};
    for (const char *key : keys_to_check) {
        if (cJSON_HasObjectItem(root, key)) {
            cJSON *item = cJSON_GetObjectItem(root, key);
            
            // 3. If the item is an object with "useJSP" field, check if it's true
            if (cJSON_IsObject(item)) {
                cJSON *useJSP = cJSON_GetObjectItem(item, "useJSP");
                if (useJSP != nullptr && cJSON_IsTrue(useJSP)) {
                    // 4. Create a string replacement
                    cJSON *new_string = cJSON_CreateString("modified_by_fuzzer");
                    if (new_string != nullptr) {
                        // 5. Replace the original value
                        cJSON_ReplaceItemInObject(item, "useJSP", new_string);
                    }
                }
            }

            // 6. Try to detach an item if it exists
            cJSON *detached = cJSON_DetachItemFromObject(root, key);
            if (detached != nullptr) {
                // Create a modified version of the detached item
                cJSON *modified = cJSON_Duplicate(detached, 1);
                if (modified != nullptr) {
                    // Change some string values in the modified copy
                    cJSON *servlet_array = cJSON_GetObjectItem(modified, "servlet");
                    if (cJSON_IsArray(servlet_array)) {
                        int array_size = cJSON_GetArraySize(servlet_array);
                        for (int i = 0; i < array_size && i < 3; i++) {
                            cJSON *servlet_item = cJSON_GetArrayItem(servlet_array, i);
                            if (servlet_item != nullptr) {
                                cJSON *servlet_name = cJSON_GetObjectItem(servlet_item, "servlet-name");
                                if (servlet_name != nullptr && cJSON_IsString(servlet_name)) {
                                    // Use cJSON_SetValuestring to modify the value
                                    cJSON_SetValuestring(servlet_name, "fuzzed_servlet");
                                }
                            }
                        }
                    }
                    
                    // Re-attach the modified version
                    cJSON_AddItemToObject(root, key, modified);
                }
                // Clean up the detached original
                cJSON_Delete(detached);
            }
        }
    }

    // Additional API usage to ensure all required APIs are called
    // Create a new object and use the APIs
    cJSON *test_object = cJSON_CreateObject();
    if (test_object != nullptr) {
        // Add a boolean true value
        cJSON_AddTrueToObject(test_object, "test_true");
        
        // Check if it's true
        cJSON *true_item = cJSON_GetObjectItem(test_object, "test_true");
        if (true_item != nullptr && cJSON_IsTrue(true_item)) {
            // Replace with a string
            cJSON *new_item = cJSON_CreateString("was_true");
            cJSON_ReplaceItemInObject(test_object, "test_true", new_item);
            
            // Set valuestring
            if (cJSON_IsString(new_item)) {
                cJSON_SetValuestring(new_item, "modified_again");
            }
        }
        
        // Check for existence
        if (cJSON_HasObjectItem(test_object, "test_true")) {
            // Detach it
            cJSON *detached_test = cJSON_DetachItemFromObject(test_object, "test_true");
            if (detached_test != nullptr) {
                // Create a reference
                cJSON *test_ref = cJSON_CreateObjectReference(detached_test);
                if (test_ref != nullptr) {
                    cJSON_AddItemToObject(test_object, "test_ref", test_ref);
                }
                cJSON_Delete(detached_test);
            }
        }
        
        cJSON_Delete(test_object);
    }

    // Cleanup: print and delete the root object
    char *printed = cJSON_Print(root);
    if (printed != nullptr) {
        // Write to output file (for debugging)
        FILE *out_file = fopen("output_file", "wb");
        if (out_file != nullptr) {
            fwrite(printed, strlen(printed), 1, out_file);
            assert_file_closed(&out_file);;
        }
        cJSON_free(printed);
    }

    // Clean up all resources
    cJSON_Delete(root);
    cJSON_free(json_string);

    return 0;
}