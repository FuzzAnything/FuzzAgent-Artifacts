// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static cJSON *create_random_object(const uint8_t *data, size_t size) {
    cJSON *root = cJSON_CreateObject();
    if (!root) return NULL;

    size_t offset = 0;
    while (offset < size) {
        char key[256];
        char val[256];
        int key_len = (data[offset] % 255) + 1;
        offset++;
        if (offset + key_len > size) break;
        memcpy(key, data + offset, key_len);
        key[key_len] = '\0';
        offset += key_len;

        int val_len = (offset < size) ? (data[offset] % 255) + 1 : 1;
        if (offset < size) offset++;
        if (offset + val_len > size) val_len = size - offset;
        if (val_len > 255) val_len = 255;
        memcpy(val, data + offset, val_len);
        val[val_len] = '\0';
        offset += val_len;

        cJSON *item = cJSON_CreateString(val);
        if (item) {
            cJSON_AddItemToObject(root, key, item);
        }
    }
    return root;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cJSON *obj = create_random_object(Data, Size);
    if (!obj) return 0;

    // Ensure we have a key to operate on for some tests
    if (!obj->child) {
        cJSON *dummy = cJSON_CreateString("test");
        if (dummy) cJSON_AddItemToObject(obj, "test", dummy);
    }

    // Extract key string for operations
    const char *test_key = NULL;
    if (obj->child) {
        test_key = obj->child->string;
    } else {
        test_key = "nonexistent";
    }

    // Generate a random key from input data for case-sensitive tests
    char random_key[64];
    int rkey_len = (Size > 0) ? (Data[0] % 63) + 1 : 1;
    if (rkey_len > 63) rkey_len = 63;
    
    // Safety check: ensure we don't read past the buffer
    if (Size > 1) {
        // Ensure we don't copy more bytes than available
        if (rkey_len > Size - 1) {
            rkey_len = (int)(Size - 1);
        }
        if (rkey_len > 0) {
            memcpy(random_key, Data + 1, rkey_len);
            random_key[rkey_len] = '\0';
        } else {
            strcpy(random_key, "random");
        }
    } else {
        strcpy(random_key, "random");
    }

    // Test 1: cJSON_DeleteItemFromObjectCaseSensitive
    cJSON_DeleteItemFromObjectCaseSensitive(obj, random_key);

    // Rebuild object if it became empty or corrupted for next tests
    if (!obj->child) {
        cJSON *dummy = cJSON_CreateString("recovered");
        if (dummy) cJSON_AddItemToObject(obj, "recovered", dummy);
    }

    // Test 2: cJSON_AddItemToObject
    cJSON *new_item = cJSON_CreateNumber(123.456);
    if (new_item) {
        cJSON_AddItemToObject(obj, "new_key", new_item);
    }

    // Test 3: cJSON_ReplaceItemInObject
    cJSON *replace_item = cJSON_CreateString("replaced_value");
    if (replace_item && obj->child) {
        cJSON_ReplaceItemInObject(obj, obj->child->string, replace_item);
    }

    // Test 4: cJSON_DetachItemFromObject
    cJSON *detached = NULL;
    if (obj->child) {
        detached = cJSON_DetachItemFromObject(obj, obj->child->string);
    }
    if (detached) {
        cJSON_Delete(detached);
    }

    // Re-add for next test
    if (!obj->child) {
        cJSON *dummy = cJSON_CreateString("for_detach_case");
        if (dummy) cJSON_AddItemToObject(obj, "for_detach_case", dummy);
    }

    // Test 5: cJSON_DetachItemFromObjectCaseSensitive
    cJSON *detached_cs = NULL;
    if (obj->child) {
        detached_cs = cJSON_DetachItemFromObjectCaseSensitive(obj, obj->child->string);
    }
    if (detached_cs) {
        cJSON_Delete(detached_cs);
    }

    // Re-add for final test
    if (!obj->child) {
        cJSON *dummy = cJSON_CreateString("for_delete");
        if (dummy) cJSON_AddItemToObject(obj, "for_delete", dummy);
    }

    // Test 6: cJSON_DeleteItemFromObject
    if (obj->child) {
        cJSON_DeleteItemFromObject(obj, obj->child->string);
    }

    // Cleanup
    cJSON_Delete(obj);

    return 0;
}