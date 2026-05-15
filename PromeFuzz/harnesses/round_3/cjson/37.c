// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_cjson_operations(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    // Use first byte to determine array size (max 64)
    int array_size = (data[0] % 64) + 1;
    // Use second byte to determine which index to use
    int target_index = data[1];
    // Use third byte for count parameter
    int count = data[2] % 32;
    // Use fourth byte for operation mode
    int mode = data[3] % 8;
    
    // Create integer array for testing
    int *numbers = NULL;
    if (count > 0) {
        numbers = (int*)malloc(count * sizeof(int));
        if (!numbers) return;
        
        // Fill with deterministic values from fuzzer data
        for (int i = 0; i < count && (size_t)(4 + i) < size; i++) {
            numbers[i] = data[4 + i] * 1000;
        }
    }
    
    // Create arrays for testing
    cJSON *array1 = cJSON_CreateArray();
    cJSON *array2 = cJSON_CreateIntArray(numbers, count);
    
    if (!array1 || !array2) {
        goto cleanup;
    }
    
    // Fill array1 with some items
    for (int i = 0; i < array_size; i++) {
        cJSON *item = cJSON_CreateNumber(i * 100);
        if (item) {
            cJSON_AddItemToArray(array1, item);
        }
    }
    
    // Perform operations based on mode
    switch (mode) {
        case 0: {
            // Test cJSON_InsertItemInArray
            cJSON *new_item = cJSON_CreateString("fuzz_item");
            if (new_item) {
                cJSON_InsertItemInArray(array1, target_index, new_item);
            }
            break;
        }
        case 1: {
            // Test cJSON_IsArray
            cJSON_bool is_arr1 = cJSON_IsArray(array1);
            cJSON_bool is_arr2 = cJSON_IsArray(array2);
            cJSON_bool is_null = cJSON_IsArray(NULL);
            (void)is_arr1;
            (void)is_arr2;
            (void)is_null;
            break;
        }
        case 2: {
            // Test cJSON_GetArrayItem with various indices
            for (int i = -5; i < array_size + 5; i++) {
                cJSON *item = cJSON_GetArrayItem(array1, i);
                (void)item;
            }
            break;
        }
        case 3: {
            // Test cJSON_GetArraySize
            int size1 = cJSON_GetArraySize(array1);
            int size2 = cJSON_GetArraySize(array2);
            int size_null = cJSON_GetArraySize(NULL);
            (void)size1;
            (void)size2;
            (void)size_null;
            break;
        }
        case 4: {
            // Test insertion with negative index
            cJSON *neg_item = cJSON_CreateBool(1);
            if (neg_item) {
                cJSON_InsertItemInArray(array1, -target_index, neg_item);
            }
            break;
        }
        case 5: {
            // Test insertion beyond array length
            cJSON *beyond_item = cJSON_CreateString("beyond");
            if (beyond_item) {
                cJSON_InsertItemInArray(array1, array_size + 100, beyond_item);
            }
            break;
        }
        case 6: {
            // Test with empty array
            cJSON *empty_array = cJSON_CreateArray();
            if (empty_array) {
                cJSON_InsertItemInArray(empty_array, 0, cJSON_CreateNull());
                cJSON_Delete(empty_array);
            }
            break;
        }
        case 7: {
            // Test mixed operations
            cJSON *mixed_array = cJSON_CreateArray();
            if (mixed_array) {
                for (int i = 0; i < 10; i++) {
                    cJSON *item = cJSON_CreateNumber(i);
                    if (item) {
                        cJSON_InsertItemInArray(mixed_array, i % 3, item);
                    }
                }
                int mixed_size = cJSON_GetArraySize(mixed_array);
                for (int i = 0; i < mixed_size; i++) {
                    cJSON_GetArrayItem(mixed_array, i);
                }
                cJSON_Delete(mixed_array);
            }
            break;
        }
    }
    
cleanup:
    if (array1) cJSON_Delete(array1);
    if (array2) cJSON_Delete(array2);
    if (numbers) free(numbers);
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    // Initialize with dummy data if too small
    if (Size == 0) {
        uint8_t dummy[4] = {10, 0, 5, 1};
        fuzz_cjson_operations(dummy, 4);
        return 0;
    }
    
    fuzz_cjson_operations(Data, Size);
    return 0;
}