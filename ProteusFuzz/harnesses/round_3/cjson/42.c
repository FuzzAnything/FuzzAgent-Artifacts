// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void cleanup_json_tree(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

static cJSON *create_random_item(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateBool(0);
    }

    // Use data to decide type
    uint8_t type_choice = data[0] % 4;
    cJSON *item = NULL;

    switch (type_choice) {
        case 0: // Bool
            // Ensure size is at least 2 to safely access data[1]
            if (size >= 2) {
                item = cJSON_CreateBool(data[1] % 2);
            } else {
                item = cJSON_CreateBool(0);
            }
            break;
        case 1: // Number
            if (size >= 2) {
                int val = (data[1] << 8) | data[0];
                item = cJSON_CreateNumber((double)val);
            } else {
                item = cJSON_CreateNumber(0.0);
            }
            break;
        case 2: // String
            if (size >= 2) {
                size_t len = data[1] % 100;
                if (len > size - 1) len = size - 1;
                if (len > 0) {
                    char *str = (char *)malloc(len + 1);
                    if (str) {
                        memcpy(str, data, len);
                        str[len] = '\0';
                        item = cJSON_CreateString(str);
                        free(str);
                    }
                }
            }
            if (item == NULL) {
                item = cJSON_CreateString("default");
            }
            break;
        case 3: // Object
            item = cJSON_CreateObject();
            break;
    }

    return item;
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cJSON *root_obj = cJSON_CreateObject();
    cJSON *root_arr = cJSON_CreateArray();
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *dup_item = NULL;
    char key[128] = {0};

    // Ensure basic validity of root objects
    if (!root_obj || !root_arr) {
        cleanup_json_tree(root_obj);
        cleanup_json_tree(root_arr);
        return 0;
    }

    // Generate keys from input data
    size_t key_len = Size > 10 ? 10 : Size;
    if (key_len > 0) {
        memcpy(key, Data, key_len);
        key[key_len] = '\0';
    } else {
        strcpy(key, "fuzz_key");
    }

    // Create items to manipulate
    item1 = create_random_item(Data, Size);
    
    // Ensure we have enough data for item2
    if (Size > 1) {
        item2 = create_random_item(Data + 1, Size - 1);
    } else {
        item2 = cJSON_CreateBool(0);
    }

    if (!item1 || !item2) {
        // If creation fails, try with defaults to ensure we test the API logic
        if (!item1) item1 = cJSON_CreateBool(0);
        if (!item2) item2 = cJSON_CreateBool(1);
    }

    // Test cJSON_AddItemToObject
    cJSON_AddItemToObject(root_obj, key, item1);

    // Test cJSON_AddItemToArray
    cJSON_AddItemToArray(root_arr, item2);

    // Test cJSON_Duplicate
    // Try with recurse = true (data[0] % 2)
    cJSON_bool recurse = (Data[0] % 2) == 1;
    dup_item = cJSON_Duplicate(item1, recurse);

    // Test cJSON_AddItemReferenceToObject
    // Add reference of item2 to root_obj under a new key
    char ref_key[128] = {0};
    snprintf(ref_key, sizeof(ref_key), "ref_key_%d", Data[0] % 100);
    cJSON_AddItemReferenceToObject(root_obj, ref_key, item2);

    // Test cJSON_AddItemReferenceToArray
    // Add reference of dup_item (if exists) or item1 to root_arr
    cJSON *ref_target = (dup_item != NULL) ? dup_item : item1;
    cJSON_AddItemReferenceToArray(root_arr, ref_target);

    // Test cJSON_CreateBool directly with fuzz data
    cJSON_bool bool_val = (Data[0] % 2) == 1;
    cJSON *bool_item = cJSON_CreateBool(bool_val);
    if (bool_item) {
        cJSON_AddItemToObject(root_obj, "bool_test", bool_item);
    }

    // Cleanup
    cleanup_json_tree(root_obj);
    cleanup_json_tree(root_arr);
    // Note: item1, item2, dup_item are either owned by root_obj/root_arr or 
    // are references. cJSON_Delete handles references correctly by not freeing
    // the referenced item if it's still owned elsewhere, but since we deleted
    // the roots, we must be careful.
    // However, item1 was added to root_obj (ownership transferred).
    // item2 was added to root_arr (ownership transferred) AND referenced in root_obj.
    // dup_item was created by Duplicate (new ownership) and referenced in root_arr.
    // bool_item was added to root_obj.
    // Since root_obj and root_arr are deleted, all children are freed.
    // We should NOT free item1, item2, dup_item, bool_item manually here to avoid double free.
    // The only exception is if they were NOT added successfully, but the API logic 
    // generally transfers ownership on success. If it fails, the item remains owned by caller.
    // To be safe against partial failures in fuzzing:
    // We rely on cJSON_Delete(root) to clean up the tree. If an item was not added, it leaks in this specific
    // iteration, which is acceptable for fuzzing (memory leak vs crash).

    return 0;
}