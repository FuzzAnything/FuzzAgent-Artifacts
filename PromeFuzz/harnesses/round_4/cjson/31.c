// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON* create_test_item(int type) {
    switch (type % 4) {
        case 0:
            return cJSON_CreateNumber(42.5);
        case 1:
            return cJSON_CreateString("test");
        case 2:
            return cJSON_CreateBool(1);
        case 3:
            return cJSON_CreateArray();
    }
    return cJSON_CreateNull();
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Create an initial array with some items
    cJSON *array = cJSON_CreateArray();
    if (!array) return 0;

    // Add initial items to array
    for (int i = 0; i < 3 && i < Size; i++) {
        cJSON *item = create_test_item(Data[i]);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }

    int array_size = cJSON_GetArraySize(array);
    if (array_size == 0) {
        cJSON_Delete(array);
        return 0;
    }

    // Use fuzzer data to determine which operations to perform
    size_t data_index = 0;
    while (data_index + 1 < Size) {
        uint8_t op = Data[data_index++] % 6;
        uint8_t idx_data = Data[data_index++];
        
        // Calculate index based on array size
        int which = (idx_data % (array_size * 2 + 2)) - 1; // Range: -1 to (2*array_size)
        
        switch (op) {
            case 0: {
                // cJSON_DetachItemFromArray
                cJSON *detached = cJSON_DetachItemFromArray(array, which);
                if (detached) {
                    // Can reuse detached item later
                    cJSON_Delete(detached);
                }
                break;
            }
            case 1: {
                // cJSON_DeleteItemFromArray
                cJSON_DeleteItemFromArray(array, which);
                break;
            }
            case 2: {
                // cJSON_AddItemToArray
                cJSON *new_item = create_test_item(idx_data);
                if (new_item) {
                    cJSON_AddItemToArray(array, new_item);
                }
                break;
            }
            case 3: {
                // cJSON_ReplaceItemInArray
                cJSON *replacement = create_test_item(idx_data);
                if (replacement) {
                    cJSON_ReplaceItemInArray(array, which, replacement);
                }
                break;
            }
            case 4: {
                // cJSON_ReplaceItemViaPointer
                if (array_size > 0) {
                    int valid_idx = abs(which) % array_size;
                    cJSON *item = cJSON_GetArrayItem(array, valid_idx);
                    if (item) {
                        cJSON *replacement = create_test_item(idx_data);
                        if (replacement) {
                            cJSON_ReplaceItemViaPointer(array, item, replacement);
                        }
                    }
                }
                break;
            }
            case 5: {
                // cJSON_InsertItemInArray
                cJSON *new_item = create_test_item(idx_data);
                if (new_item) {
                    cJSON_InsertItemInArray(array, which, new_item);
                }
                break;
            }
        }
        
        // Update array size for next iteration
        array_size = cJSON_GetArraySize(array);
        if (array_size == 0 && data_index + 1 < Size) {
            // Add a new item to keep array non-empty for further operations
            cJSON *item = create_test_item(Data[data_index++]);
            if (item) {
                cJSON_AddItemToArray(array, item);
                array_size = 1;
            }
        }
    }

    // Clean up
    cJSON_Delete(array);
    
    return 0;
}