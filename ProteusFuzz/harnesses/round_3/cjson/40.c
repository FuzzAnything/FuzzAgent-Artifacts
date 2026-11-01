// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static cJSON *create_random_array(size_t size) {
    cJSON *array = cJSON_CreateArray();
    if (!array) {
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        cJSON *item = cJSON_CreateNumber((double)i);
        if (!item) {
            cJSON_Delete(array);
            return NULL;
        }
        cJSON_AddItemToArray(array, item);
    }
    return array;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    int op = Data[0] % 6;
    int index = (int)(Data[1]) - 10; 
    int array_size = (int)(Data[2]) % 10 + 1;
    int insert_index = (int)(Data[3]) % (array_size + 5) - 2;

    cJSON *array = create_random_array(array_size);
    cJSON *new_item = cJSON_CreateNumber(42.0);
    cJSON *detached_item = NULL;

    if (!array || !new_item) {
        cJSON_Delete(array);
        cJSON_Delete(new_item);
        return 0;
    }

    switch (op) {
        case 0: // cJSON_ReplaceItemInArray
            cJSON_ReplaceItemInArray(array, index, new_item);
            new_item = NULL; // Ownership transferred to array
            break;
        case 1: // cJSON_DetachItemFromArray
            detached_item = cJSON_DetachItemFromArray(array, index);
            break;
        case 2: // cJSON_ReplaceItemViaPointer
            if (array && array->child) {
                cJSON *target = array->child;
                cJSON_ReplaceItemViaPointer(array, target, new_item);
                new_item = NULL; // Ownership transferred
            }
            break;
        case 3: // cJSON_GetArraySize
            (void)cJSON_GetArraySize(array);
            break;
        case 4: // cJSON_InsertItemInArray
            cJSON_InsertItemInArray(array, insert_index, new_item);
            new_item = NULL; // Ownership transferred
            break;
        case 5: // cJSON_AddItemReferenceToArray
            cJSON_AddItemReferenceToArray(array, new_item);
            new_item = NULL; // Reference added, but we don't delete the original here as it might be used elsewhere in logic, 
                             // however for this simple driver, we treat it as consumed by the array to avoid double free if array holds it.
                             // Actually, AddItemReferenceToArray does NOT take ownership. 
                             // But if we delete new_item here, and the array still holds a reference, it's fine as long as we don't delete the array's child separately.
                             // However, cJSON_Delete(array) will delete all children. If new_item is a child of array, deleting new_item explicitly is a double free.
                             // So we must set new_item to NULL to prevent explicit deletion if it was added.
            break;
    }

    // Cleanup
    cJSON_Delete(array);
    if (new_item) {
        cJSON_Delete(new_item);
    }
    if (detached_item) {
        cJSON_Delete(detached_item);
    }

    return 0;
}