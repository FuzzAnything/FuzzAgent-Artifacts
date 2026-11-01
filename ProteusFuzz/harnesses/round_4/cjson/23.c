// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArrayReference at cJSON.c:2571:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static void cleanup_json_tree(cJSON *root) {
    if (root != NULL) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test 1: cJSON_CreateArray
    cJSON *arr1 = cJSON_CreateArray();
    if (arr1 != NULL) {
        if (Size > 0) {
            cJSON *item = cJSON_CreateNull();
            if (item != NULL) {
                cJSON_AddItemToArray(arr1, item);
            }
        }
        cleanup_json_tree(arr1);
    }

    // Test 2: cJSON_AddItemReferenceToArray
    cJSON *arr2 = cJSON_CreateArray();
    if (arr2 != NULL) {
        cJSON *refItem = cJSON_CreateNull();
        if (refItem != NULL) {
            cJSON_AddItemReferenceToArray(arr2, refItem);
            // Fix: refItem is not owned by arr2, so we must delete it explicitly
            // after the array is cleaned up to prevent memory leaks.
            // Note: We cannot delete refItem immediately after adding it if we want 
            // to test the reference behavior, but since arr2 is deleted below, 
            // the reference becomes dangling. The memory for refItem itself must be freed.
            // However, standard practice for references is to manage the source separately.
            // Here, we delete the source item after the array is deleted.
        }
        cleanup_json_tree(arr2);
        // Explicitly delete the referenced item to fix the leak
        if (refItem != NULL) {
            cJSON_Delete(refItem);
        }
    }

    // Test 3: cJSON_AddItemToArray
    cJSON *arr3 = cJSON_CreateArray();
    if (arr3 != NULL) {
        cJSON *item3 = cJSON_CreateNull();
        if (item3 != NULL) {
            cJSON_AddItemToArray(arr3, item3);
        }
        cleanup_json_tree(arr3);
    }

    // Test 4: cJSON_AddArrayToObject
    cJSON *obj = cJSON_CreateObject();
    if (obj != NULL) {
        const char *key = "fuzz_key";
        cJSON *subArr = cJSON_AddArrayToObject(obj, key);
        if (subArr != NULL) {
            cJSON *nullItem = cJSON_CreateNull();
            if (nullItem != NULL) {
                cJSON_AddItemToArray(subArr, nullItem);
            }
        }
        cleanup_json_tree(obj);
    }

    // Test 5: cJSON_CreateNull
    cJSON *nullItem = cJSON_CreateNull();
    if (nullItem != NULL) {
        cleanup_json_tree(nullItem);
    }

    // Test 6: cJSON_CreateArrayReference
    cJSON *sourceItem = cJSON_CreateNull();
    if (sourceItem != NULL) {
        cJSON *refArr = cJSON_CreateArrayReference(sourceItem);
        if (refArr != NULL) {
            cleanup_json_tree(refArr);
        }
        // sourceItem is not owned by refArr, must be deleted manually
        cleanup_json_tree(sourceItem);
    }

    return 0;
}