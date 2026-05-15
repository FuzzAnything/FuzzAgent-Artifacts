// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void test_array_functions(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Create a dummy array for testing
    cJSON *array = cJSON_CreateArray();
    if (!array) return;
    
    // Add some items to the array using fuzz data
    int num_items = (size > 10) ? 10 : (int)size;
    for (int i = 0; i < num_items; i++) {
        cJSON *item = cJSON_CreateNumber(data[i % size]);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
    
    // Test cJSON_IsArray
    cJSON_bool is_array = cJSON_IsArray(array);
    (void)is_array;
    
    // Test cJSON_GetArraySize
    int array_size = cJSON_GetArraySize(array);
    
    // Test cJSON_GetArrayItem with various indices
    if (array_size > 0) {
        for (int i = 0; i < 3; i++) {
            int index = (data[i % size] % (array_size * 2)) - array_size/2;
            cJSON *item = cJSON_GetArrayItem(array, index);
            (void)item;
        }
    }
    
    // Test cJSON_ReplaceItemInArray
    if (array_size > 0) {
        int replace_index = data[0] % (array_size * 2);
        cJSON *new_item = cJSON_CreateString("replacement");
        if (new_item) {
            cJSON_ReplaceItemInArray(array, replace_index, new_item);
        }
    }
    
    // Test cJSON_InsertItemInArray
    if (array_size > 0) {
        int insert_index;
        if (size > 1) {
            insert_index = data[1] % (array_size * 2);
        } else {
            insert_index = data[0] % (array_size * 2);
        }
        cJSON *insert_item = cJSON_CreateBool((size > 2 ? data[2] : data[0]) % 2);
        if (insert_item) {
            cJSON_InsertItemInArray(array, insert_index, insert_item);
        }
    }
    
    // Test cJSON_CreateIntArray
    if (size >= 4) {
        int count = (int)(data[0] % 8) + 1;
        if (count * sizeof(int) <= size) {
            int *numbers = (int*)malloc(count * sizeof(int));
            if (numbers) {
                for (int i = 0; i < count; i++) {
                    numbers[i] = (int)data[i * sizeof(int) % size];
                }
                cJSON *int_array = cJSON_CreateIntArray(numbers, count);
                if (int_array) {
                    // Test array functions on the new int array
                    cJSON_IsArray(int_array);
                    cJSON_GetArraySize(int_array);
                    cJSON_Delete(int_array);
                }
                free(numbers);
            }
        }
    }
    
    // Cleanup
    cJSON_Delete(array);
}

int LLVMFuzzerTestOneInput_31(const uint8_t *data, size_t size) {
    // Test with valid array
    test_array_functions(data, size);
    
    // Test with NULL array
    cJSON_IsArray(NULL);
    cJSON_GetArrayItem(NULL, 0);
    cJSON_GetArraySize(NULL);
    cJSON_ReplaceItemInArray(NULL, 0, NULL);
    cJSON_InsertItemInArray(NULL, 0, NULL);
    
    // Test cJSON_CreateIntArray with edge cases
    if (size > 0) {
        // Test with zero count
        cJSON *empty_array = cJSON_CreateIntArray(NULL, 0);
        if (empty_array) {
            cJSON_Delete(empty_array);
        }
        
        // Test with negative count (should handle gracefully)
        int dummy = 42;
        cJSON *neg_array = cJSON_CreateIntArray(&dummy, -1);
        if (neg_array) {
            cJSON_Delete(neg_array);
        }
    }
    
    return 0;
}