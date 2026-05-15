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
//<ID> 173
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 1
//<Quality> {"density":26,"unique_branches":{"cJSON_IsArray":[[3044,9,3044,21,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_IsInvalid","cJSON_Delete","cJSON_CreateObject","cJSON_Delete","cJSON_GetObjectItem","cJSON_GetObjectItemCaseSensitive","cJSON_Duplicate","cJSON_CreateNull","cJSON_AddItemToObject","cJSON_AddBoolToObject","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_CreateString","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_AddObjectToObject","cJSON_AddNumberToObject","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_InsertItemInArray","cJSON_GetArrayItem","cJSON_GetArrayItem","cJSON_CreateString","cJSON_ReplaceItemViaPointer","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_AddStringToObject","cJSON_AddItemToArray","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_IsInvalid","cJSON_CreateObject","cJSON_GetObjectItem","cJSON_GetObjectItemCaseSensitive","cJSON_Duplicate","cJSON_CreateNull","cJSON_AddItemToObject","cJSON_AddBoolToObject","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_CreateString","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_AddObjectToObject","cJSON_AddNumberToObject","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_DetachItemFromArray","cJSON_InsertItemInArray","cJSON_GetArrayItem","cJSON_GetArrayItem","cJSON_CreateString","cJSON_ReplaceItemViaPointer","cJSON_CreateObject","cJSON_AddStringToObject","cJSON_AddStringToObject","cJSON_AddItemToArray","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Parse input JSON using cJSON_ParseWithLength
    cJSON *root = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (root == nullptr) {
        return 0; // Early return if parsing fails
    }

    // Check if the root is invalid using cJSON_IsInvalid
    if (cJSON_IsInvalid(root)) {
        cJSON_Delete(root);
        return 0;
    }

    // Create a new object to hold modifications
    cJSON *modified_root = cJSON_CreateObject();
    if (modified_root == nullptr) {
        cJSON_Delete(root);
        return 0;
    }

    // Get "menu" object using both case-insensitive and case-sensitive variants
    cJSON *menu_item = cJSON_GetObjectItem(root, "menu");
    cJSON *menu_item_cs = cJSON_GetObjectItemCaseSensitive(root, "menu");
    
    // Duplicate the menu object with recursion
    cJSON *duplicated_menu = nullptr;
    if (menu_item != nullptr) {
        duplicated_menu = cJSON_Duplicate(menu_item, 1);
    }

    // Add new fields to the modified root
    // Create and add a null item
    cJSON *null_item = cJSON_CreateNull();
    if (null_item != nullptr) {
        cJSON_AddItemToObject(modified_root, "null_field", null_item);
    }

    // Create and add a boolean item
    cJSON_AddBoolToObject(modified_root, "bool_field", 1);

    // Create and add a number item
    cJSON *number_item = cJSON_CreateNumber(42.5);
    if (number_item != nullptr) {
        cJSON_AddItemToObject(modified_root, "number_field", number_item);
    }

    // Create and add a string item
    cJSON *string_item = cJSON_CreateString("fuzz_string");
    if (string_item != nullptr) {
        cJSON_AddItemToObject(modified_root, "string_field", string_item);
    }

    // Add the duplicated menu to modified root
    if (duplicated_menu != nullptr) {
        cJSON_AddItemToObject(modified_root, "duplicated_menu", duplicated_menu);
    }

    // Create an integer array
    int int_array_data[] = {1, 2, 3, 4, 5};
    cJSON *int_array = cJSON_CreateIntArray(int_array_data, 5);
    if (int_array != nullptr) {
        cJSON_AddItemToObject(modified_root, "int_array", int_array);
    }

    // Create a float array
    float float_array_data[] = {1.1f, 2.2f, 3.3f};
    cJSON *float_array = cJSON_CreateFloatArray(float_array_data, 3);
    if (float_array != nullptr) {
        cJSON_AddItemToObject(modified_root, "float_array", float_array);
    }

    // Add a new object to the modified root
    cJSON *new_object = cJSON_AddObjectToObject(modified_root, "new_object");
    if (new_object != nullptr) {
        // Add items to the new object
        cJSON_AddNumberToObject(new_object, "inner_number", 99.9);
    }

    // Process arrays within the original JSON
    if (menu_item != nullptr) {
        cJSON *popup = cJSON_GetObjectItem(menu_item, "popup");
        if (popup != nullptr) {
            cJSON *menuitem = cJSON_GetObjectItem(popup, "menuitem");
            if (cJSON_IsArray(menuitem)) {
                int array_size = cJSON_GetArraySize(menuitem);
                
                if (array_size > 0) {
                    // Detach and re-insert an array item
                    cJSON *detached_item = cJSON_DetachItemFromArray(menuitem, 0);
                    if (detached_item != nullptr) {
                        cJSON_InsertItemInArray(menuitem, array_size - 1, detached_item);
                    }

                    // Replace an item via pointer if array has at least 2 items
                    if (array_size >= 2) {
                        cJSON *first_item = cJSON_GetArrayItem(menuitem, 0);
                        cJSON *second_item = cJSON_GetArrayItem(menuitem, 1);
                        if (first_item != nullptr && second_item != nullptr) {
                            cJSON *replacement = cJSON_CreateString("replaced_value");
                            if (replacement != nullptr) {
                                cJSON_ReplaceItemViaPointer(menuitem, second_item, replacement);
                            }
                        }
                    }

                    // Add a new item to the array
                    cJSON *new_array_item = cJSON_CreateObject();
                    if (new_array_item != nullptr) {
                        cJSON_AddStringToObject(new_array_item, "value", "Fuzzed");
                        cJSON_AddStringToObject(new_array_item, "onclick", "FuzzAction()");
                        cJSON_AddItemToArray(menuitem, new_array_item);
                    }
                }
            }
        }
    }

    // Print the modified JSON
    char *printed_json = cJSON_Print(modified_root);
    if (printed_json != nullptr) {
        // Write to output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file != nullptr) {
            fwrite(printed_json, strlen(printed_json), 1, out_file);
            assert_file_closed(&out_file);;
        }
        
        // Use cJSON_free to free the printed string
        cJSON_free(printed_json);
    }

    // Clean up: Delete the original and modified JSON trees
    cJSON_Delete(root);
    cJSON_Delete(modified_root);

    return 0;
}