// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static void fuzz_operations(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Create a root object to work with
    cJSON *root = cJSON_CreateObject();
    if (!root) return;
    
    // Create some initial items in the object
    cJSON_AddStringToObject(root, "key1", "value1");
    cJSON_AddNullToObject(root, "key2");
    cJSON_AddRawToObject(root, "key3", "{\"nested\": true}");
    
    // Use fuzzer data to generate keys
    // Ensure we have at least 1 byte for key length
    size_t key_len = data[0] % 32 + 1;  // Key length 1-32 bytes
    if (key_len > size - 1) key_len = size - 1;
    
    char *key = malloc(key_len + 1);
    if (!key) {
        cJSON_Delete(root);
        return;
    }
    memcpy(key, data + 1, key_len);
    key[key_len] = '\0';
    
    // Determine which operation to perform based on fuzzer input
    int op = data[0] % 6;
    
    switch (op) {
        case 0: {
            // cJSON_DetachItemFromObjectCaseSensitive
            cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, key);
            if (detached) {
                cJSON_Delete(detached);
            }
            break;
        }
        case 1: {
            // cJSON_AddStringToObject
            size_t str_len = key_len > 0 ? key_len : 1;
            char *str_val = malloc(str_len + 1);
            if (str_val) {
                memcpy(str_val, key, str_len);
                str_val[str_len] = '\0';
                cJSON *added = cJSON_AddStringToObject(root, key, str_val);
                free(str_val);
                // Don't delete added item - it's owned by root
            }
            break;
        }
        case 2: {
            // cJSON_DeleteItemFromObjectCaseSensitive
            cJSON_DeleteItemFromObjectCaseSensitive(root, key);
            break;
        }
        case 3: {
            // cJSON_DeleteItemFromObject
            cJSON_DeleteItemFromObject(root, key);
            break;
        }
        case 4: {
            // cJSON_AddNullToObject
            cJSON_AddNullToObject(root, key);
            break;
        }
        case 5: {
            // cJSON_AddRawToObject
            size_t raw_len = key_len > 0 ? key_len : 1;
            char *raw_val = malloc(raw_len + 1);
            if (raw_val) {
                memcpy(raw_val, key, raw_len);
                raw_val[raw_len] = '\0';
                cJSON_AddRawToObject(root, key, raw_val);
                free(raw_val);
            }
            break;
        }
    }
    
    // Cleanup
    free(key);
    
    // Create another object and perform operations with different keys
    cJSON *obj2 = cJSON_CreateObject();
    if (obj2) {
        // Use different part of fuzzer data for second key
        size_t key2_offset = (key_len + 1) % size;
        size_t key2_len = data[key2_offset] % 16 + 1;
        if (key2_len > size - key2_offset - 1) {
            key2_len = size - key2_offset - 1;
        }
        
        if (key2_len > 0) {
            char *key2 = malloc(key2_len + 1);
            if (key2) {
                memcpy(key2, data + key2_offset + 1, key2_len);
                key2[key2_len] = '\0';
                
                // Try all operations on second object
                cJSON_AddStringToObject(obj2, key2, "test");
                cJSON_DetachItemFromObjectCaseSensitive(obj2, key2);
                cJSON_AddNullToObject(obj2, key2);
                cJSON_DeleteItemFromObjectCaseSensitive(obj2, key2);
                cJSON_AddRawToObject(obj2, key2, "[1,2,3]");
                cJSON_DeleteItemFromObject(obj2, key2);
                
                free(key2);
            }
        }
        cJSON_Delete(obj2);
    }
    
    // Finally delete the root object
    cJSON_Delete(root);
}

int LLVMFuzzerTestOneInput_29(const uint8_t *data, size_t size) {
    // First pass: normal operations
    fuzz_operations(data, size);
    
    // Second pass: test with empty/null keys
    cJSON *empty_test = cJSON_CreateObject();
    if (empty_test) {
        cJSON_AddStringToObject(empty_test, "", "empty");
        cJSON_DetachItemFromObjectCaseSensitive(empty_test, "");
        cJSON_DeleteItemFromObjectCaseSensitive(empty_test, "");
        cJSON_DeleteItemFromObject(empty_test, "");
        cJSON_AddNullToObject(empty_test, "");
        cJSON_AddRawToObject(empty_test, "", "null");
        cJSON_Delete(empty_test);
    }
    
    // Third pass: test with duplicate keys
    cJSON *dup_test = cJSON_CreateObject();
    if (dup_test) {
        cJSON_AddStringToObject(dup_test, "dup", "first");
        cJSON_AddStringToObject(dup_test, "dup", "second");
        cJSON_DetachItemFromObjectCaseSensitive(dup_test, "dup");
        cJSON_DeleteItemFromObject(dup_test, "dup");
        cJSON_Delete(dup_test);
    }
    
    return 0;
}