// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

/* Helper to create a simple number item */
static cJSON *create_number_item(double val) {
    cJSON *item = cJSON_CreateNumber(val);
    return item;
}

/* Helper to create a simple string item */
static cJSON *create_string_item(const char *str) {
    cJSON *item = cJSON_CreateString(str);
    return item;
}

/* Helper to create an array and populate it with some items */
static cJSON *create_test_array(size_t count) {
    cJSON *arr = cJSON_CreateArray();
    if (!arr) return NULL;

    for (size_t i = 0; i < count; i++) {
        cJSON *item = create_number_item((double)i);
        if (!item) {
            cJSON_Delete(arr);
            return NULL;
        }
        cJSON_AddItemToArray(arr, item);
    }
    return arr;
}

/* Helper to create an object and populate it with some items */
static cJSON *create_test_object(size_t count) {
    cJSON *obj = cJSON_CreateObject();
    if (!obj) return NULL;

    for (size_t i = 0; i < count; i++) {
        char key[32];
        snprintf(key, sizeof(key), "key_%zu", i);
        cJSON *item = create_number_item((double)i);
        if (!item) {
            cJSON_Delete(obj);
            return NULL;
        }
        cJSON_AddItemToObject(obj, key, item);
    }
    return obj;
}

/* Helper to create a nested structure */
static cJSON *create_nested_structure(void) {
    cJSON *root = cJSON_CreateObject();
    if (!root) return NULL;

    cJSON *arr = cJSON_CreateArray();
    if (!arr) {
        cJSON_Delete(root);
        return NULL;
    }
    for (int i = 0; i < 5; i++) {
        cJSON *num = cJSON_CreateNumber(i);
        if (!num) {
            cJSON_Delete(arr);
            cJSON_Delete(root);
            return NULL;
        }
        cJSON_AddItemToArray(arr, num);
    }
    cJSON_AddItemToObject(root, "array", arr);

    cJSON *obj = cJSON_CreateObject();
    if (!obj) {
        cJSON_Delete(root);
        return NULL;
    }
    cJSON *str = cJSON_CreateString("test");
    if (!str) {
        cJSON_Delete(obj);
        cJSON_Delete(root);
        return NULL;
    }
    cJSON_AddItemToObject(obj, "value", str);
    cJSON_AddItemToObject(root, "object", obj);

    return root;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Use data to determine test scenarios safely */
    uint8_t mode = Data[0] % 10;
    
    /* Ensure we have enough data before accessing indices */
    int index = 0;
    int count = 1;
    
    if (Size >= 3) {
        index = (int)(Data[1] * 255 + Data[2]) % 1000 - 500; /* Range: -500 to 499 */
    }
    if (Size >= 5) {
        count = (int)(Data[3] * 255 + Data[4]) % 50 + 1;    /* Range: 1 to 50 */
    }

    cJSON *array = NULL;
    cJSON *item = NULL;
    cJSON *newitem = NULL;
    cJSON *detached = NULL;
    cJSON_bool result;

    switch (mode) {
        case 0: {
            /* Test cJSON_GetArrayItem */
            array = create_test_array(count);
            if (array) {
                cJSON *retrieved = cJSON_GetArrayItem(array, index);
                /* No specific action, just ensure no crash */
                (void)retrieved;
                cJSON_Delete(array);
            }
            break;
        }
        case 1: {
            /* Test cJSON_AddItemReferenceToArray */
            array = create_test_array(count);
            if (array) {
                item = create_number_item(123.0);
                if (item) {
                    result = cJSON_AddItemReferenceToArray(array, item);
                    /* Clean up: array owns the reference now. 
                     * We must not delete 'item' separately because it's now part of the array.
                     * Deleting the array will free the item.
                     */
                    cJSON_Delete(array);
                    /* item is freed as part of array, so we set it to NULL to avoid double free logic */
                    item = NULL; 
                }
            }
            break;
        }
        case 2: {
            /* Test cJSON_ReplaceItemInArray */
            array = create_test_array(count);
            if (array) {
                newitem = create_number_item(999.0);
                if (newitem) {
                    result = cJSON_ReplaceItemInArray(array, index, newitem);
                    /* If successful, newitem is now part of array. If failed, newitem is orphaned.
                     * We need to handle both cases to avoid leaks or double frees.
                     */
                    if (!result) {
                        cJSON_Delete(newitem);
                    }
                    cJSON_Delete(array);
                }
            }
            break;
        }
        case 3: {
            /* Test cJSON_GetArraySize */
            array = create_test_array(count);
            if (array) {
                int size = cJSON_GetArraySize(array);
                (void)size;
                cJSON_Delete(array);
            }
            break;
        }
        case 4: {
            /* Test cJSON_ReplaceItemViaPointer */
            array = create_test_array(count);
            if (array && count > 0) {
                cJSON *target = cJSON_GetArrayItem(array, 0);
                if (target) {
                    newitem = create_number_item(777.0);
                    if (newitem) {
                        result = cJSON_ReplaceItemViaPointer(array, target, newitem);
                        if (!result) {
                            cJSON_Delete(newitem);
                        }
                        cJSON_Delete(array);
                    }
                } else {
                    cJSON_Delete(array);
                }
            }
            break;
        }
        case 5: {
            /* Test cJSON_DetachItemFromArray */
            array = create_test_array(count);
            if (array) {
                detached = cJSON_DetachItemFromArray(array, index);
                /* If detached is not NULL, it is removed from array and must be freed manually */
                if (detached) {
                    cJSON_Delete(detached);
                }
                cJSON_Delete(array);
            }
            break;
        }
        case 6: {
            /* Test with NULL inputs */
            cJSON_GetArrayItem(NULL, 0);
            cJSON_AddItemReferenceToArray(NULL, NULL);
            cJSON_ReplaceItemInArray(NULL, 0, NULL);
            cJSON_GetArraySize(NULL);
            cJSON_ReplaceItemViaPointer(NULL, NULL, NULL);
            cJSON_DetachItemFromArray(NULL, 0);
            break;
        }
        case 7: {
            /* Test with negative index */
            array = create_test_array(count);
            if (array) {
                cJSON_GetArrayItem(array, -1);
                cJSON_ReplaceItemInArray(array, -1, NULL);
                cJSON_DetachItemFromArray(array, -1);
                cJSON_Delete(array);
            }
            break;
        }
        case 8: {
            /* Test with invalid index (out of bounds) */
            array = create_test_array(count);
            if (array) {
                int huge_index = 1000000;
                cJSON_GetArrayItem(array, huge_index);
                cJSON_ReplaceItemInArray(array, huge_index, NULL);
                cJSON_DetachItemFromArray(array, huge_index);
                cJSON_Delete(array);
            }
            break;
        }
        case 9: {
            /* Test with nested structures */
            cJSON *root = create_nested_structure();
            if (root) {
                cJSON *arr = cJSON_GetObjectItem(root, "array");
                if (arr) {
                    cJSON_GetArrayItem(arr, index);
                    cJSON_GetArraySize(arr);
                }
                cJSON_Delete(root);
            }
            break;
        }
        default:
            break;
    }

    return 0;
}