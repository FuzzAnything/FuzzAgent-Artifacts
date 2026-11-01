// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static cJSON *create_test_array(const uint8_t *data, size_t size) {
    cJSON *array = cJSON_CreateArray();
    if (!array) {
        return NULL;
    }

    // Limit the number of items to prevent excessive memory usage or stack overflow
    size_t limit = (size > 1000) ? 1000 : size;
    for (size_t i = 0; i < limit; i++) {
        cJSON *item = cJSON_CreateNumber((double)data[i]);
        if (!item) {
            cJSON_Delete(array);
            return NULL;
        }
        if (!cJSON_AddItemToArray(array, item)) {
            cJSON_Delete(item);
            cJSON_Delete(array);
            return NULL;
        }
    }
    return array;
}

static cJSON *create_random_item(const uint8_t *data, size_t size, size_t offset) {
    if (offset + 1 >= size) {
        // Not enough data to read offset + 1, return a simple number
        return cJSON_CreateNumber(0.0);
    }
    
    uint8_t type = data[offset];
    cJSON *item = NULL;

    switch (type % 5) {
        case 0:
            item = cJSON_CreateNumber((double)(data[offset + 1] & 0xFF));
            break;
        case 1:
            item = cJSON_CreateString("fuzz_string");
            break;
        case 2:
            item = cJSON_CreateTrue();
            break;
        case 3:
            item = cJSON_CreateFalse();
            break;
        case 4:
            item = cJSON_CreateNull();
            break;
        default:
            item = cJSON_CreateNumber(0.0);
            break;
    }
    return item;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    int index = 0;
    if (Size > 1) {
        index = (int)(Data[1] % 100);
    }
    if (Size > 2) {
        index = (int)(Data[2] % 200);
    }
    if (Size > 3) {
        index = (int)(Data[3] % 500);
    }

    // Create initial array
    cJSON *array = create_test_array(Data, Size);
    if (!array) {
        return 0;
    }

    // Test cJSON_GetArraySize
    int size = cJSON_GetArraySize(array);
    (void)size; // Use result to avoid unused warning

    // Test cJSON_GetArrayItem with various indices
    cJSON *item = cJSON_GetArrayItem(array, index);
    if (item) {
        // Do something with item to avoid unused warning
        (void)item->type;
    }

    // Test cJSON_DetachItemFromArray
    cJSON *detached = cJSON_DetachItemFromArray(array, index);
    if (detached) {
        // Reuse detached item or delete it
        cJSON_Delete(detached);
    }

    // Create a new item to insert/replace
    // Ensure we have enough data for offset 4 and 5
    size_t offset1 = 4;
    if (Size <= offset1) offset1 = 0;
    cJSON *newitem = create_random_item(Data, Size, offset1);
    if (!newitem) {
        cJSON_Delete(array);
        return 0;
    }

    // Test cJSON_InsertItemInArray
    cJSON_InsertItemInArray(array, index, newitem);

    // Create another item for replacement
    size_t offset2 = 5;
    if (Size <= offset2) offset2 = 0;
    cJSON *replaceitem = create_random_item(Data, Size, offset2);
    if (!replaceitem) {
        cJSON_Delete(array);
        cJSON_Delete(newitem);
        return 0;
    }

    // Test cJSON_ReplaceItemInArray
    cJSON_ReplaceItemInArray(array, index, replaceitem);

    // Test cJSON_ReplaceItemViaPointer
    if (array && array->child) {
        cJSON *target = cJSON_GetArrayItem(array, 0);
        if (target) {
            size_t offset3 = 6;
            if (Size <= offset3) offset3 = 0;
            cJSON *swapitem = create_random_item(Data, Size, offset3);
            if (swapitem) {
                if (!cJSON_ReplaceItemViaPointer(array, target, swapitem)) {
                    cJSON_Delete(swapitem);
                }
            }
        }
    }

    // Cleanup
    cJSON_Delete(array);

    return 0;
}