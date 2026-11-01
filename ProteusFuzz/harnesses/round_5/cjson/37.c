// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cJSON.h"

static cJSON *create_fuzzed_object(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateObject();
    }

    // Try to parse the input as a JSON object
    cJSON *root = cJSON_ParseWithLength((const char *)data, size);
    if (root != NULL && root->type & cJSON_Object) {
        return root;
    }

    // If parsing fails or it's not an object, create a new empty object
    if (root) {
        cJSON_Delete(root);
    }
    return cJSON_CreateObject();
}

static void cleanup_object(cJSON *obj) {
    if (obj) {
        cJSON_Delete(obj);
    }
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a base object to work with
    cJSON *obj = create_fuzzed_object(Data, Size);
    if (!obj) {
        return 0;
    }

    // Extract potential keys and values from the input data
    // We'll use the input data to generate keys and items dynamically
    
    // Create a new item to add/replace
    cJSON *new_item = NULL;
    
    // Generate a key string from the input (ensure it's null-terminated)
    char key_buffer[256];
    size_t key_len = (Size < 255) ? Size : 255;
    memcpy(key_buffer, Data, key_len);
    key_buffer[key_len] = '\0';
    
    // Create a new item based on the first byte of data
    uint8_t type_byte = Data[0] % 10;
    
    switch (type_byte) {
        case 0:
            if (Size > 1) {
                new_item = cJSON_CreateNumber((double)(Data[1] % 1000));
            } else {
                new_item = cJSON_CreateNumber(0);
            }
            break;
        case 1:
            new_item = cJSON_CreateString(key_buffer);
            break;
        case 2:
            new_item = cJSON_CreateArray();
            break;
        case 3:
            new_item = cJSON_CreateObject();
            break;
        case 4:
            new_item = cJSON_CreateTrue();
            break;
        case 5:
            new_item = cJSON_CreateFalse();
            break;
        case 6:
            new_item = cJSON_CreateNull();
            break;
        case 7:
            // Create a nested object
            new_item = cJSON_CreateObject();
            if (new_item) {
                cJSON_AddNumberToObject(new_item, "nested_val", (Size > 2) ? (double)(Data[2] % 100) : 0);
            }
            break;
        case 8:
            // Create an array with some items
            new_item = cJSON_CreateArray();
            if (new_item) {
                cJSON_AddItemToArray(new_item, cJSON_CreateNumber(1));
                cJSON_AddItemToArray(new_item, cJSON_CreateNumber(2));
            }
            break;
        case 9:
            // Create a string from input data, ensuring null termination
            if (Size > 1) {
                char *str_data = malloc(Size);
                if (str_data) {
                    memcpy(str_data, Data + 1, Size - 1);
                    str_data[Size - 1] = '\0';
                    new_item = cJSON_CreateString(str_data);
                    free(str_data);
                }
            }
            if (!new_item) {
                new_item = cJSON_CreateString("default");
            }
            break;
    }

    // Test cJSON_HasObjectItem
    cJSON_bool has_item = cJSON_HasObjectItem(obj, key_buffer);
    (void)has_item; // Use the result to avoid unused variable warning

    // Test cJSON_AddBoolToObject
    cJSON_bool bool_val = (Size > 0) ? ((Data[0] % 2) != 0) : 0;
    cJSON *added_bool = cJSON_AddBoolToObject(obj, key_buffer, bool_val);
    (void)added_bool;

    // Test cJSON_AddItemToObject
    if (new_item) {
        cJSON_bool add_result = cJSON_AddItemToObject(obj, key_buffer, new_item);
        (void)add_result;
        
        // If addition failed, new_item is still owned by us
        if (!add_result) {
            cJSON_Delete(new_item);
            new_item = NULL;
        }
    }

    // Create another item for replacement tests
    cJSON *replace_item = cJSON_CreateNumber((Size > 0) ? (double)(Data[Size-1] % 500) : 0);
    
    // Test cJSON_ReplaceItemInObject
    cJSON_bool replace_result = cJSON_ReplaceItemInObject(obj, key_buffer, replace_item);
    (void)replace_result;
    
    // If replacement failed, we own replace_item
    if (!replace_result) {
        cJSON_Delete(replace_item);
        replace_item = NULL;
    }

    // Test cJSON_ReplaceItemInObjectCaseSensitive
    cJSON *replace_item_case = cJSON_CreateString("replacement_value");
    cJSON_bool replace_case_result = cJSON_ReplaceItemInObjectCaseSensitive(obj, key_buffer, replace_item_case);
    (void)replace_case_result;
    
    if (!replace_case_result) {
        cJSON_Delete(replace_item_case);
        replace_item_case = NULL;
    }

    // Test cJSON_DeleteItemFromObject
    cJSON_DeleteItemFromObject(obj, key_buffer);

    // Clean up
    cleanup_object(obj);
    
    return 0;
}