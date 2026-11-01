// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

/* Helper function to create a simple cJSON array with some items */
static cJSON *create_test_array(size_t size) {
    cJSON *arr = cJSON_CreateArray();
    if (!arr) return NULL;

    for (size_t i = 0; i < size; i++) {
        cJSON *item = cJSON_CreateNumber((double)i);
        if (!item) {
            cJSON_Delete(arr);
            return NULL;
        }
        cJSON_AddItemToArray(arr, item);
    }
    return arr;
}

/* Helper function to create a standalone cJSON item */
static cJSON *create_test_item(int type, const char *str_val, double num_val) {
    cJSON *item = NULL;
    if (type == cJSON_Number) {
        item = cJSON_CreateNumber(num_val);
    } else if (type == cJSON_String && str_val) {
        item = cJSON_CreateString(str_val);
    } else if (type == cJSON_Array) {
        item = cJSON_CreateArray();
    } else if (type == cJSON_Object) {
        item = cJSON_CreateObject();
    } else {
        item = cJSON_CreateNull();
    }
    return item;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    /* Use first byte to determine operation mode */
    int op_mode = Data[0] % 6;
    /* Use second byte to determine array size (0-10) */
    int array_size = (Data[1] % 11);
    /* Use third byte (if available) for index */
    int index = (Size > 2) ? (int)(Data[2] % 20) : 0;
    /* Use fourth byte (if available) for item type */
    int item_type = (Size > 3) ? (int)(Data[3] % 6) : 0;
    /* Use fifth byte (if available) for number value */
    double num_val = (Size > 4) ? (double)(Data[4] % 100) : 0.0;
    /* Use sixth byte (if available) for string length */
    int str_len = (Size > 5) ? (int)(Data[5] % 10) : 0;

    cJSON *arr = NULL;
    cJSON *item = NULL;
    cJSON *new_item = NULL;
    cJSON *detached = NULL;
    cJSON_bool res_bool;
    int res_int;
    cJSON *res_ptr;
    cJSON *temp_item = NULL;
    cJSON *target = NULL;

    /* Create base array */
    arr = create_test_array(array_size);
    if (!arr && array_size > 0) {
        /* If creation failed but size > 0, try with size 0 */
        arr = cJSON_CreateArray();
        if (!arr) return 0;
    }

    /* Create item for reference/insertion/replacement */
    item = create_test_item(item_type, "test_string", num_val);
    new_item = create_test_item((item_type + 1) % 6, "new_string", num_val + 1.0);

    switch (op_mode) {
        case 0: {
            /* Test cJSON_AddItemReferenceToArray */
            if (arr && item) {
                res_bool = cJSON_AddItemReferenceToArray(arr, item);
                /* Check return value */
                (void)res_bool;
            }
            break;
        }
        case 1: {
            /* Test cJSON_GetArraySize */
            if (arr) {
                res_int = cJSON_GetArraySize(arr);
                (void)res_int;
            }
            break;
        }
        case 2: {
            /* Test cJSON_DetachItemFromArray */
            if (arr) {
                /* Try various indices: negative, valid, out of bounds */
                int test_indices[] = {-1, 0, index, array_size, array_size + 1};
                for (int i = 0; i < 5; i++) {
                    detached = cJSON_DetachItemFromArray(arr, test_indices[i]);
                    if (detached) {
                        cJSON_Delete(detached);
                    }
                }
            }
            break;
        }
        case 3: {
            /* Test cJSON_ReplaceItemViaPointer */
            if (arr && item && new_item) {
                /* Try to replace first child if exists */
                target = arr->child;
                if (target) {
                    res_bool = cJSON_ReplaceItemViaPointer(arr, target, new_item);
                    /* new_item is now owned by arr, do not delete it later */
                    new_item = NULL; 
                    (void)res_bool;
                } else {
                    /* If no child, try with NULL pointers to test safety */
                    res_bool = cJSON_ReplaceItemViaPointer(NULL, target, new_item);
                    res_bool = cJSON_ReplaceItemViaPointer(arr, NULL, new_item);
                    res_bool = cJSON_ReplaceItemViaPointer(arr, target, NULL);
                }
            }
            break;
        }
        case 4: {
            /* Test cJSON_ReplaceItemInArray */
            if (arr && new_item) {
                /* Try various indices */
                int test_indices[] = {-1, 0, index, array_size, array_size + 1};
                for (int i = 0; i < 5; i++) {
                    temp_item = create_test_item(item_type, "replace_str", num_val);
                    res_bool = cJSON_ReplaceItemInArray(arr, test_indices[i], temp_item);
                    if (!res_bool && temp_item) {
                        cJSON_Delete(temp_item);
                        temp_item = NULL;
                    } else if (res_bool) {
                        /* temp_item is now owned by arr */
                        temp_item = NULL;
                    }
                }
            }
            break;
        }
        case 5: {
            /* Test cJSON_InsertItemInArray */
            if (arr && new_item) {
                /* Try various indices */
                int test_indices[] = {-1, 0, index, array_size, array_size + 1};
                for (int i = 0; i < 5; i++) {
                    temp_item = create_test_item(item_type, "insert_str", num_val);
                    res_bool = cJSON_InsertItemInArray(arr, test_indices[i], temp_item);
                    if (!res_bool && temp_item) {
                        cJSON_Delete(temp_item);
                        temp_item = NULL;
                    } else if (res_bool) {
                        /* temp_item is now owned by arr */
                        temp_item = NULL;
                    }
                }
            }
            break;
        }
        default:
            break;
    }

    /* Cleanup */
    if (arr) {
        cJSON_Delete(arr);
        arr = NULL;
    }
    /* item might have been added to arr via reference or replaced, 
       but cJSON_Delete(arr) handles children. 
       If item was added via reference, it is not freed by arr delete, 
       but we must be careful not to double free if it was detached or replaced.
       However, in case 0, we added reference. In case 3, 4, 5 we might have moved ownership.
       To be safe, we only delete item if it wasn't successfully moved into arr or detached.
       Since tracking ownership precisely is complex, and cJSON_Delete is safe for NULL,
       we rely on the fact that if item was added to arr, it's part of the tree.
       But cJSON_AddItemReferenceToArray does NOT take ownership.
       So if we did case 0, item is still ours.
       If we did case 3, 4, 5 and succeeded, item (or temp_item) is owned by arr.
       We set new_item/temp_item to NULL on success to avoid double free.
       'item' is only used in case 0 and 3. In case 3, we didn't move 'item', we moved 'new_item'.
       So 'item' is always ours unless we did something else.
       Let's just delete item if it's not NULL.
    */
    if (item) {
        cJSON_Delete(item);
    }
    if (new_item) {
        cJSON_Delete(new_item);
    }
    if (temp_item) {
        cJSON_Delete(temp_item);
    }

    return 0;
}