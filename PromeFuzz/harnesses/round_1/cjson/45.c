// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_cJSON_operations(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    // Create initial array with random size (limited to avoid excessive memory)
    int array_size = (data[0] % 16) + 1;
    cJSON *array = cJSON_CreateArray();
    
    if (!array) return;
    
    // Populate array with some items
    for (int i = 0; i < array_size; i++) {
        if (i < size - 1) {
            cJSON *item = cJSON_CreateNumber(data[i + 1] % 256);
            if (item) {
                cJSON_AddItemToArray(array, item);
            }
        }
    }
    
    // Test cJSON_IsArray
    cJSON_bool is_array = cJSON_IsArray(array);
    
    // Test cJSON_GetArraySize
    int retrieved_size = cJSON_GetArraySize(array);
    
    // Test cJSON_GetArrayItem with various indices
    int test_index = (data[0] % (retrieved_size > 0 ? retrieved_size * 2 : 2)) - 1;
    cJSON *retrieved_item = cJSON_GetArrayItem(array, test_index);
    
    // Test cJSON_InsertItemInArray
    if (retrieved_size > 0) {
        int insert_index = (data[1] % (retrieved_size * 2 + 2)) - 1;
        cJSON *new_item = cJSON_CreateString("fuzz_insert");
        if (new_item) {
            cJSON_InsertItemInArray(array, insert_index, new_item);
        }
    }
    
    // Test cJSON_CreateIntArray
    if (size >= 8) {
        int int_count = (data[2] % 8) + 1;
        int *int_numbers = (int*)malloc(int_count * sizeof(int));
        if (int_numbers) {
            for (int i = 0; i < int_count && i + 3 < size; i++) {
                int_numbers[i] = data[i + 3];
            }
            cJSON *int_array = cJSON_CreateIntArray(int_numbers, int_count);
            if (int_array) {
                cJSON_Delete(int_array);
            }
            free(int_numbers);
        }
    }
    
    // Test cJSON_CreateDoubleArray
    if (size >= 16) {
        int double_count = (data[3] % 4) + 1;
        double *double_numbers = (double*)malloc(double_count * sizeof(double));
        if (double_numbers) {
            for (int i = 0; i < double_count && i * 2 + 4 < size; i++) {
                // Create simple double from two bytes
                double_numbers[i] = (double)((data[i * 2 + 4] << 8) | data[i * 2 + 5]) / 100.0;
            }
            cJSON *double_array = cJSON_CreateDoubleArray(double_numbers, double_count);
            if (double_array) {
                cJSON_Delete(double_array);
            }
            free(double_numbers);
        }
    }
    
    // Cleanup
    cJSON_Delete(array);
}

int LLVMFuzzerTestOneInput_45(const uint8_t *data, size_t size) {
    // Initialize cJSON internal structures if needed
    // (cJSON doesn't require explicit initialization)
    
    fuzz_cJSON_operations(data, size);
    
    return 0;
}