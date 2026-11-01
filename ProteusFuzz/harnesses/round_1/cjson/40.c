// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_CreateObjectReference at cJSON.c:2560:23 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void cleanup_json(cJSON *json) {
    if (json) {
        cJSON_Delete(json);
    }
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input data is null-terminated for cJSON_Parse
    // We create a buffer with Size + 1 to hold the null terminator
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Parse input as JSON to create a base structure
    cJSON *root = cJSON_Parse(input_str);
    free(input_str);

    if (!root) {
        // Try creating a simple object if parsing fails
        root = cJSON_CreateObject();
        if (!root) {
            return 0;
        }
    }

    // Create some auxiliary items for testing references and detachments
    cJSON *item1 = cJSON_CreateString("test_string");
    cJSON *item2 = cJSON_CreateNumber(123.45);
    cJSON *array = cJSON_CreateArray();
    cJSON *obj = cJSON_CreateObject();

    if (!item1 || !item2 || !array || !obj) {
        cleanup_json(root);
        if (item1) cJSON_Delete(item1);
        if (item2) cJSON_Delete(item2);
        if (array) cJSON_Delete(array);
        if (obj) cJSON_Delete(obj);
        return 0;
    }

    // Ensure we have a key string to use for object operations
    // Use a safe subset of input data or a default if input is weird
    // Fixed: Allocate key_buf with size + 1 to safely add null terminator
    char *key_buf = (char *)malloc(Size + 1);
    if (!key_buf) {
        cleanup_json(root);
        cJSON_Delete(item1);
        cJSON_Delete(item2);
        cJSON_Delete(array);
        cJSON_Delete(obj);
        return 0;
    }
    
    size_t key_len = (Size < 64) ? Size : 64;
    memcpy(key_buf, Data, key_len);
    key_buf[key_len] = '\0';
    
    // Sanitize key to avoid null bytes in the middle
    for (size_t i = 0; i < key_len; i++) {
        if (key_buf[i] == '\0') {
            key_buf[i] = ' '; 
        }
    }
    const char *key = key_buf;

    // 1. Test cJSON_AddItemToObject
    // Add item1 to root under key
    cJSON_AddItemToObject(root, key, item1);
    
    // 2. Test cJSON_AddItemReferenceToArray
    // Add reference of item2 to array
    cJSON_AddItemReferenceToArray(array, item2);
    
    // 3. Test cJSON_AddItemReferenceToObject
    // Add reference of item2 to obj under key
    cJSON_AddItemReferenceToObject(obj, key, item2);

    // 4. Test cJSON_CreateObjectReference
    // Create a reference to the array
    cJSON *ref_array = cJSON_CreateObjectReference(array);
    
    // 5. Test cJSON_DetachItemViaPointer
    // Try to detach item2 from array (it was added by reference, so it should still be there)
    cJSON *detached = cJSON_DetachItemViaPointer(array, item2);
    if (detached) {
        // Detached successfully, do nothing with it here, it will be cleaned up later
    }

    // 6. Test cJSON_DeleteItemFromObject
    // Try to delete item from root using key
    cJSON_DeleteItemFromObject(root, key);

    // Additional state exploration:
    // Add item2 back to root to test deletion again
    cJSON_AddItemToObject(root, key, item2);
    cJSON_DeleteItemFromObject(root, key);

    // Cleanup
    cleanup_json(root);
    // item1 was added to root, so it's deleted with root
    // item2 was added to root, so it's deleted with root
    // array was not added to root, delete it
    cleanup_json(array);
    // obj was not added to root, delete it
    cleanup_json(obj);
    
    // ref_array points to array, but array is deleted. 
    // cJSON_CreateObjectReference creates a non-owning reference.
    // Deleting the reference wrapper should be safe.
    if (ref_array) {
        cJSON_Delete(ref_array);
    }
    
    free(key_buf);

    return 0;
}