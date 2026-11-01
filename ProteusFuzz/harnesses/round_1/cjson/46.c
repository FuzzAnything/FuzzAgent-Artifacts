// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObjectReference at cJSON.c:2560:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObjectReference at cJSON.c:2560:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

/* Helper to create a simple object with a key-value pair for testing */
static cJSON *create_test_object(const char *key, const char *value) {
    cJSON *obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    cJSON *val = cJSON_CreateString(value);
    if (!val) {
        cJSON_Delete(obj);
        return NULL;
    }
    
    cJSON_AddItemToObject(obj, key, val);
    return obj;
}

/* Helper to create a simple array with one item */
static cJSON *create_test_array(const char *value) {
    cJSON *arr = cJSON_CreateArray();
    if (!arr) return NULL;
    
    cJSON *item = cJSON_CreateString(value);
    if (!item) {
        cJSON_Delete(arr);
        return NULL;
    }
    
    cJSON_AddItemToArray(arr, item);
    return arr;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Use input data to generate keys and values */
    char key[256];
    char value[256];
    
    /* Ensure we don't read past the buffer */
    size_t key_len = (Size > 255) ? 255 : Size;
    size_t val_len = (Size > 255) ? 255 : Size;
    
    /* Ensure null termination */
    memcpy(key, Data, key_len);
    key[key_len] = '\0';
    
    /* Create a different value by shifting or modifying data if possible */
    if (Size > 1) {
        /* Check if we have enough data for val_len from Data+1 */
        size_t available = Size - 1;
        if (val_len > available) {
            val_len = available;
        }
        memcpy(value, Data + 1, val_len);
        value[val_len] = '\0';
    } else {
        strcpy(value, "default_value");
    }

    /* Test Case 1: cJSON_DeleteItemFromObject */
    {
        cJSON *obj = create_test_object(key, value);
        if (obj) {
            cJSON_DeleteItemFromObject(obj, key);
            cJSON_DeleteItemFromObject(obj, "non_existent_key");
            cJSON_Delete(obj);
        }
    }

    /* Test Case 2: cJSON_AddItemToObjectCS */
    {
        cJSON *obj = cJSON_CreateObject();
        if (obj) {
            cJSON *item = cJSON_CreateString(value);
            if (item) {
                cJSON_AddItemToObjectCS(obj, key, item);
                cJSON_Delete(obj);
            }
        }
    }

    /* Test Case 3: cJSON_AddItemReferenceToObject */
    {
        cJSON *obj = cJSON_CreateObject();
        if (obj) {
            cJSON *source = cJSON_CreateString(value);
            if (source) {
                cJSON_AddItemReferenceToObject(obj, key, source);
                cJSON_Delete(obj); /* This should not free source */
                cJSON_Delete(source);
            }
        }
    }

    /* Test Case 4: cJSON_DetachItemViaPointer */
    {
        cJSON *arr = create_test_array(value);
        if (arr && arr->child) {
            cJSON *detached = cJSON_DetachItemViaPointer(arr, arr->child);
            if (detached) {
                cJSON_Delete(detached);
            }
            cJSON_Delete(arr);
        }
        
        /* Test with object */
        cJSON *obj = create_test_object(key, value);
        if (obj && obj->child) {
            cJSON *detached = cJSON_DetachItemViaPointer(obj, obj->child);
            if (detached) {
                cJSON_Delete(detached);
            }
            cJSON_Delete(obj);
        }
    }

    /* Test Case 5: cJSON_AddItemToObject */
    {
        cJSON *obj = cJSON_CreateObject();
        if (obj) {
            cJSON *item = cJSON_CreateString(value);
            if (item) {
                cJSON_AddItemToObject(obj, key, item);
                cJSON_Delete(obj);
            }
        }
    }

    /* Test Case 6: cJSON_CreateObjectReference */
    {
        cJSON *original = create_test_object(key, value);
        if (original) {
            cJSON *ref = cJSON_CreateObjectReference(original);
            if (ref) {
                cJSON_Delete(ref); /* Should not free original */
            }
            cJSON_Delete(original);
        }
        
        /* Test with array */
        cJSON *arr = create_test_array(value);
        if (arr) {
            cJSON *ref = cJSON_CreateObjectReference(arr);
            if (ref) {
                cJSON_Delete(ref);
            }
            cJSON_Delete(arr);
        }
    }

    return 0;
}