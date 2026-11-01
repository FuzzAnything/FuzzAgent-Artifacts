// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObjectReference at cJSON.c:2560:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

/* Helper to create a simple JSON object from fuzz data */
static cJSON *create_object_from_data(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateObject();
    }
    
    /* Create a null-terminated string from input data */
    char *str = (char *)malloc(size + 1);
    if (!str) return NULL;
    
    memcpy(str, data, size);
    str[size] = '\0';
    
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        free(str);
        return NULL;
    }
    
    /* Add the string as a value */
    cJSON *val = cJSON_CreateString(str);
    if (val) {
        cJSON_AddItemToObject(root, "fuzz_data", val);
    }
    
    free(str);
    return root;
}

/* Helper to create a simple JSON array from fuzz data */
static cJSON *create_array_from_data(const uint8_t *data, size_t size) {
    cJSON *root = cJSON_CreateArray();
    if (!root) return NULL;
    
    for (size_t i = 0; i < size; i++) {
        cJSON *num = cJSON_CreateNumber((double)data[i]);
        if (num) {
            cJSON_AddItemToArray(root, num);
        }
    }
    
    return root;
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Create test objects */
    cJSON *obj1 = create_object_from_data(Data, Size);
    cJSON *obj2 = create_object_from_data(Data, Size);
    cJSON *arr1 = create_array_from_data(Data, Size);
    
    if (!obj1 || !obj2 || !arr1) {
        cJSON_Delete(obj1);
        cJSON_Delete(obj2);
        cJSON_Delete(arr1);
        return 0;
    }

    /* Test 1: cJSON_CreateObjectReference */
    cJSON *ref = cJSON_CreateObjectReference(obj1);
    if (ref) {
        /* Try to delete reference - should not free obj1 */
        cJSON_Delete(ref);
    }

    /* Test 2: cJSON_AddItemToObject */
    cJSON *item1 = cJSON_CreateString("test_key");
    if (item1) {
        cJSON_AddItemToObject(obj1, "key1", item1);
    }
    
    /* Test 3: cJSON_AddItemReferenceToObject */
    cJSON *item2 = cJSON_CreateNumber(123);
    if (item2) {
        cJSON_AddItemReferenceToObject(obj2, "ref_key", item2);
    }

    /* Test 4: cJSON_DetachItemViaPointer */
    if (obj1->child) {
        cJSON *detached = cJSON_DetachItemViaPointer(obj1, obj1->child);
        if (detached) {
            /* Reattach or delete as needed */
            cJSON_Delete(detached);
        }
    }

    /* Test 5: cJSON_AddItemToObjectCS */
    cJSON *item3 = cJSON_CreateBool(1);
    if (item3) {
        cJSON_AddItemToObjectCS(obj1, "const_key", item3);
    }

    /* Test 6: cJSON_DeleteItemFromObject */
    cJSON_DeleteItemFromObject(obj1, "key1");
    cJSON_DeleteItemFromObject(obj2, "ref_key");

    /* Cleanup */
    cJSON_Delete(obj1);
    cJSON_Delete(obj2);
    cJSON_Delete(arr1);

    return 0;
}