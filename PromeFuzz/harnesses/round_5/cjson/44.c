// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_operations(cJSON *array, const uint8_t *data, size_t size) {
    if (!array || size < 1) return;
    
    int array_size = cJSON_GetArraySize(array);
    
    // Use first byte to determine operation type
    uint8_t op_type = data[0] % 6;
    size_t data_index = 1;
    
    switch (op_type) {
        case 0: { // cJSON_DeleteItemFromArray
            if (array_size > 0) {
                int index = (size > data_index) ? (data[data_index] % array_size) : 0;
                cJSON_DeleteItemFromArray(array, index);
            }
            break;
        }
        case 1: { // cJSON_AddItemToArray
            cJSON *new_item = cJSON_CreateNumber((size > data_index) ? (double)data[data_index] : 0.0);
            if (new_item) {
                cJSON_AddItemToArray(array, new_item);
            }
            break;
        }
        case 2: { // cJSON_ReplaceItemInArray
            if (array_size > 0 && size > data_index + 1) {
                int index = data[data_index] % array_size;
                cJSON *new_item = cJSON_CreateNumber((double)data[data_index + 1]);
                if (new_item) {
                    cJSON_ReplaceItemInArray(array, index, new_item);
                }
            }
            break;
        }
        case 3: { // cJSON_ReplaceItemViaPointer
            if (array_size > 0 && size > data_index + 1) {
                int index = data[data_index] % array_size;
                cJSON *item = cJSON_GetArrayItem(array, index);
                if (item) {
                    cJSON *replacement = cJSON_CreateNumber((double)data[data_index + 1]);
                    if (replacement) {
                        cJSON_ReplaceItemViaPointer(array, item, replacement);
                    }
                }
            }
            break;
        }
        case 4: { // cJSON_CreateFloatArray test
            if (size > data_index + 1) {
                int count = data[data_index] % 10; // Limit to reasonable size
                if (count > 0 && size >= data_index + 1 + (size_t)count) {
                    float *numbers = (float*)malloc(count * sizeof(float));
                    if (numbers) {
                        for (int i = 0; i < count; i++) {
                            numbers[i] = (float)data[data_index + 1 + i];
                        }
                        cJSON *float_array = cJSON_CreateFloatArray(numbers, count);
                        if (float_array) {
                            cJSON_AddItemToArray(array, float_array);
                        }
                        free(numbers);
                    }
                }
            }
            break;
        }
        case 5: { // cJSON_GetArraySize stress test
            // Create nested arrays to stress GetArraySize
            int depth = (size > data_index) ? (data[data_index] % 5) : 1;
            cJSON *current = array;
            for (int i = 0; i < depth; i++) {
                cJSON *nested = cJSON_CreateArray();
                if (nested) {
                    cJSON_AddItemToArray(current, nested);
                    current = nested;
                } else {
                    break;
                }
            }
            // Get sizes at different levels
            cJSON_GetArraySize(array);
            if (array->child) {
                cJSON_GetArraySize(array->child);
            }
            break;
        }
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a base array for testing
    cJSON *test_array = cJSON_CreateArray();
    if (!test_array) return 0;
    
    // Populate initial array with some items
    int initial_items = (Data[0] % 10) + 1;
    for (int i = 0; i < initial_items && i < (int)Size - 1; i++) {
        cJSON *item = cJSON_CreateNumber((double)Data[i + 1]);
        if (item) {
            cJSON_AddItemToArray(test_array, item);
        }
    }
    
    // Perform fuzzing operations
    size_t offset = (initial_items < (int)Size - 1) ? (size_t)initial_items + 1 : 1;
    while (offset < Size) {
        size_t chunk_size = (Size - offset < 10) ? Size - offset : 10;
        fuzz_operations(test_array, Data + offset, chunk_size);
        offset += chunk_size;
    }
    
    // Clean up
    cJSON_Delete(test_array);
    
    return 0;
}