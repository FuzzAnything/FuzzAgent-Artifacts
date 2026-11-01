// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void safe_cleanup(cJSON *root) {
    if (root) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a base object to operate on
    cJSON *obj = cJSON_CreateObject();
    if (!obj) {
        return 0;
    }

    // Extract potential key and value strings from input
    // We treat the input as a sequence of potential keys and values
    // to diversify the state of the object.
    
    // Initialize some random data to ensure we have something to work with
    // if the input is too short for complex operations.
    const char *default_key = "default_key";
    const char *default_val = "default_val";

    // Attempt to add items based on input data to create a state
    // We use modulo to keep indices within bounds
    for (size_t i = 0; i < Size; i += 4) {
        if (i + 1 > Size) break;
        
        // Create a temporary key string from input
        size_t key_len = 0;
        while (i + key_len < Size && Data[i + key_len] != '\0' && key_len < 64) {
            key_len++;
        }
        if (key_len == 0) key_len = 1; // Ensure at least 1 byte

        char *key = (char *)malloc(key_len + 1);
        if (!key) break;
        memcpy(key, Data + i, key_len);
        key[key_len] = '\0';

        // Randomly choose an operation to perform on the object
        int op = Data[i] % 6;

        switch (op) {
            case 0: {
                // cJSON_AddBoolToObject
                // Ensure we have a second byte for the boolean value
                if (i + 1 < Size) {
                    cJSON_AddBoolToObject(obj, key, (Data[i + 1] & 1));
                } else {
                    cJSON_AddBoolToObject(obj, key, 0);
                }
                break;
            }
            case 1: {
                // cJSON_AddObjectToObject
                cJSON_AddObjectToObject(obj, key);
                break;
            }
            case 2: {
                // Check existence (cJSON_HasObjectItem)
                cJSON_HasObjectItem(obj, key);
                break;
            }
            case 3: {
                // cJSON_DeleteItemFromObject
                cJSON_DeleteItemFromObject(obj, key);
                break;
            }
            case 4: {
                // cJSON_ReplaceItemInObjectCaseSensitive
                // Need a new item to replace with
                cJSON *new_item = cJSON_CreateString("replaced_value");
                if (new_item) {
                    cJSON_bool res = cJSON_ReplaceItemInObjectCaseSensitive(obj, key, new_item);
                    if (!res) {
                        cJSON_Delete(new_item);
                    }
                }
                break;
            }
            case 5: {
                // cJSON_ReplaceItemInObject
                cJSON *new_item = cJSON_CreateNumber(12345);
                if (new_item) {
                    cJSON_bool res = cJSON_ReplaceItemInObject(obj, key, new_item);
                    if (!res) {
                        cJSON_Delete(new_item);
                    }
                }
                break;
            }
        }
        free(key);
    }

    // Perform specific targeted tests with the constructed object
    // 1. Test DeleteItemFromObject
    cJSON_DeleteItemFromObject(obj, "test_delete");

    // 2. Test AddBoolToObject
    cJSON_AddBoolToObject(obj, "test_bool", 1);

    // 3. Test AddObjectToObject
    cJSON *nested = cJSON_AddObjectToObject(obj, "test_nested");
    if (nested) {
        cJSON_AddNumberToObject(nested, "inner_num", 42);
    }

    // 4. Test HasObjectItem
    cJSON_HasObjectItem(obj, "test_bool");
    cJSON_HasObjectItem(obj, "non_existent");

    // 5. Test ReplaceItemInObjectCaseSensitive
    cJSON *replace_item = cJSON_CreateString("new_value");
    if (replace_item) {
        cJSON_bool res = cJSON_ReplaceItemInObjectCaseSensitive(obj, "test_bool", replace_item);
        if (!res) {
            cJSON_Delete(replace_item);
        }
    }

    // 6. Test ReplaceItemInObject
    cJSON *replace_item2 = cJSON_CreateNumber(999);
    if (replace_item2) {
        cJSON_bool res = cJSON_ReplaceItemInObject(obj, "test_bool", replace_item2);
        if (!res) {
            cJSON_Delete(replace_item2);
        }
    }

    // Cleanup
    safe_cleanup(obj);

    return 0;
}