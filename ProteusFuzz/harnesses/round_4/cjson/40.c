// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

/* Helper to create a simple cJSON item from fuzz data */
static cJSON *create_item_from_data(const uint8_t *data, size_t size) {
    if (size < 2) {
        return cJSON_CreateNull();
    }
    
    /* Use first byte to determine type */
    uint8_t type_hint = data[0] % 5;
    cJSON *item = NULL;
    
    switch (type_hint) {
        case 0:
            item = cJSON_CreateNull();
            break;
        case 1:
            item = cJSON_CreateBool(data[1] % 2);
            break;
        case 2: {
            double val = 0.0;
            if (size >= 5) {
                int32_t iv;
                memcpy(&iv, data + 1, sizeof(iv));
                val = (double)iv;
            }
            item = cJSON_CreateNumber(val);
            break;
        }
        case 3: {
            /* Create string, ensure null termination */
            size_t str_len = (size > 1) ? (size - 1) : 1;
            if (str_len > 1000) str_len = 1000;
            char *str = (char *)malloc(str_len + 1);
            if (str) {
                memcpy(str, data + 1, str_len);
                str[str_len] = '\0';
                item = cJSON_CreateString(str);
                free(str);
            }
            break;
        }
        case 4:
            item = cJSON_CreateArray();
            break;
    }
    
    return item;
}

/* Helper to create an array with some items */
static cJSON *create_array_from_data(const uint8_t *data, size_t size) {
    cJSON *array = cJSON_CreateArray();
    if (!array) return NULL;
    
    /* Add a few items based on data */
    int count = (size > 0) ? (data[0] % 10) + 1 : 1;
    for (int i = 0; i < count; i++) {
        /* Ensure we have enough data for the offset */
        if (size <= 1 + i) break;
        
        cJSON *item = create_item_from_data(data + 1 + i, size - 1 - i);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
    
    return array;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 5) {
        return 0;
    }

    /* Create test structures */
    cJSON *array = create_array_from_data(Data, Size);
    if (!array) {
        return 0;
    }
    
    cJSON *item_ref = NULL;
    cJSON *item_replace = NULL;
    cJSON *replacement = NULL;
    cJSON *insert_item = NULL;
    
    if (Size > 1) {
        item_ref = create_item_from_data(Data + 1, Size - 1);
    }
    if (Size > 2) {
        item_replace = create_item_from_data(Data + 2, Size - 2);
    }
    if (Size > 3) {
        replacement = create_item_from_data(Data + 3, Size - 3);
    }
    if (Size > 4) {
        insert_item = create_item_from_data(Data + 4, Size - 4);
    }
    
    /* Test 1: cJSON_AddItemReferenceToArray */
    cJSON *array_ref = cJSON_CreateArray();
    if (array_ref && item_ref) {
        cJSON_AddItemReferenceToArray(array_ref, item_ref);
    }
    
    /* Test 2: cJSON_ReplaceItemInArray */
    int index = Data[0] % 10;
    if (array && item_replace) {
        cJSON_ReplaceItemInArray(array, index, item_replace);
        cJSON_ReplaceItemInArray(array, -1, item_replace); /* Test negative index */
    }
    
    /* Test 3: cJSON_ReplaceItemViaPointer */
    if (array && array->child) {
        cJSON *first_child = array->child;
        if (replacement) {
            cJSON_ReplaceItemViaPointer(array, first_child, replacement);
        }
        /* Test with NULL pointers */
        cJSON_ReplaceItemViaPointer(NULL, first_child, replacement);
        cJSON_ReplaceItemViaPointer(array, NULL, replacement);
        cJSON_ReplaceItemViaPointer(array, first_child, NULL);
    }
    
    /* Test 4: cJSON_DetachItemFromArray */
    int detach_index = Data[1] % 10;
    cJSON *detached = cJSON_DetachItemFromArray(array, detach_index);
    cJSON *detached_neg = cJSON_DetachItemFromArray(array, -1);
    
    /* Test 5: cJSON_GetArraySize */
    int size = cJSON_GetArraySize(array);
    int size_null = cJSON_GetArraySize(NULL);
    
    /* Test 6: cJSON_InsertItemInArray */
    int insert_index = Data[2] % 10;
    
    if (array && insert_item) {
        cJSON_InsertItemInArray(array, insert_index, insert_item);
        cJSON_InsertItemInArray(array, -1, insert_item); /* Test negative index */
        cJSON_InsertItemInArray(NULL, insert_index, insert_item); /* Test NULL array */
        cJSON_InsertItemInArray(array, insert_index, NULL); /* Test NULL item */
    }
    
    /* Cleanup */
    /* Note: cJSON_Delete recursively deletes children. 
       Items added via reference or detach are managed separately.
       However, cJSON_AddItemReferenceToArray does not take ownership.
       cJSON_InsertItemInArray takes ownership.
       cJSON_ReplaceItemInArray takes ownership of the new item.
       cJSON_DetachItemFromArray removes ownership from array.
    */
    
    cJSON_Delete(array);
    cJSON_Delete(array_ref);
    
    /* Detached items are no longer part of array, so we must delete them */
    if (detached) cJSON_Delete(detached);
    if (detached_neg) cJSON_Delete(detached_neg);
    
    /* Items that were added by reference (item_ref) are still owned by the caller */
    if (item_ref) cJSON_Delete(item_ref);
    
    /* item_replace was passed to ReplaceItemInArray which takes ownership, 
       so we should NOT delete it here if it was successfully added. 
       However, if the operation failed or index was out of bounds, it might not be owned.
       To be safe and avoid double free, we only delete items we know we own.
       Since ReplaceItemInArray takes ownership on success, we skip deleting item_replace 
       if we assume it was added. But to be robust against failures where ownership wasn't taken:
       Actually, cJSON_ReplaceItemInArray documentation says it takes ownership of newitem.
       If it fails (e.g. index < 0), it returns false and does NOT take ownership.
       In our test, we call it with -1, so it likely didn't take ownership in that specific call.
       But the first call with valid index likely did.
       This is tricky. The safest approach in fuzzing is to NOT delete items that were passed 
       to functions that might take ownership, UNLESS we are sure they weren't taken.
       Given the complexity, let's rely on the fact that if they were added, they are part of 'array' 
       which we already deleted. If they weren't added, we delete them.
       But we can't easily know if they were added.
       
       Better strategy: Do not pass items we create to functions that take ownership unless we 
       are prepared to not delete them. Or, create copies.
       
       Let's change the logic: We will NOT delete item_replace, replacement, insert_item 
       if they were passed to functions that take ownership.
       Since we don't track success perfectly, and to avoid UAF, we will skip deleting 
       items that were passed to ownership-taking functions.
       
       However, the error was UAF on insert_item. This means insert_item was added to array, 
       array was deleted (freeing insert_item), and then we tried to delete insert_item again.
       
       Fix: Do not delete items that were passed to ownership-taking functions.
       We will assume they are owned by the array if the function was called.
       We will only delete items that were NOT passed to ownership-taking functions.
       
       item_ref: Passed to AddItemReferenceToArray -> Does NOT take ownership. Safe to delete.
       item_replace: Passed to ReplaceItemInArray -> Takes ownership. Do NOT delete.
       replacement: Passed to ReplaceItemViaPointer -> Takes ownership. Do NOT delete.
       insert_item: Passed to InsertItemInArray -> Takes ownership. Do NOT delete.
    */
    
    /* item_ref is safe to delete as AddItemReferenceToArray does not take ownership */
    /* item_replace, replacement, insert_item are NOT deleted here as they may be owned by 'array' */
    
    return 0;
}