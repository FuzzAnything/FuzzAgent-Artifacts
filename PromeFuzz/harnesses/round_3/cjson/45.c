// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static void fuzz_cJSON_InsertItemInArray(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    cJSON *array = cJSON_CreateArray();
    if (!array) return;
    
    // Create some items to insert
    cJSON *item1 = cJSON_CreateNumber((double)data[0]);
    cJSON *item2 = cJSON_CreateNumber((double)data[1 % size]);
    
    // First add an item to have non-empty array
    cJSON_AddItemToArray(array, item1);
    
    // Determine index from fuzz data
    int index = (int)data[0];
    
    // Insert second item at calculated index
    cJSON_InsertItemInArray(array, index, item2);
    
    // Cleanup
    cJSON_Delete(array);
}

static void fuzz_cJSON_CreateIntArray(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Determine count from fuzz data (limit to reasonable size)
    int count = (int)(data[0] % 32);
    if (count == 0) count = 1;
    
    // Create integer array from fuzz data
    int *int_array = (int*)malloc(count * sizeof(int));
    if (!int_array) return;
    
    for (int i = 0; i < count; i++) {
        int_array[i] = (int)data[i % size];
    }
    
    cJSON *json_array = cJSON_CreateIntArray(int_array, count);
    if (json_array) {
        cJSON_Delete(json_array);
    }
    
    free(int_array);
}

static void fuzz_cJSON_CreateArray(void) {
    cJSON *array = cJSON_CreateArray();
    if (array) {
        // Add some items to test the array
        cJSON_AddItemToArray(array, cJSON_CreateNumber(42));
        cJSON_AddItemToArray(array, cJSON_CreateString("test"));
        
        // Get array size
        int array_size = cJSON_GetArraySize(array);
        
        // Try to get items at various indices
        for (int i = -1; i <= array_size + 1; i++) {
            cJSON *item = cJSON_GetArrayItem(array, i);
            (void)item; // Use item to avoid unused variable warning
        }
        
        cJSON_Delete(array);
    }
}

static void fuzz_cJSON_GetArrayItem(const uint8_t *data, size_t size) {
    cJSON *array = cJSON_CreateArray();
    if (!array) return;
    
    // Add some items to the array
    for (size_t i = 0; i < size && i < 10; i++) {
        cJSON_AddItemToArray(array, cJSON_CreateNumber((double)data[i]));
    }
    
    // Try to get items at various indices based on fuzz data
    for (size_t i = 0; i < size && i < 20; i++) {
        int index = (int)data[i] - 128; // Create both positive and negative indices
        cJSON *item = cJSON_GetArrayItem(array, index);
        (void)item; // Use item to avoid unused variable warning
    }
    
    cJSON_Delete(array);
}

static void fuzz_cJSON_GetArraySize(const uint8_t *data, size_t size) {
    cJSON *array = cJSON_CreateArray();
    if (!array) return;
    
    // Add variable number of items based on fuzz data
    size_t item_count = data[0] % 20;
    for (size_t i = 0; i < item_count; i++) {
        cJSON_AddItemToArray(array, cJSON_CreateNumber((double)i));
    }
    
    // Get array size
    int array_size = cJSON_GetArraySize(array);
    (void)array_size; // Use to avoid unused variable warning
    
    // Test with NULL array
    cJSON_GetArraySize(NULL);
    
    cJSON_Delete(array);
}

static void fuzz_cJSON_CreateDoubleArray(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Determine count from fuzz data (limit to reasonable size)
    int count = (int)(data[0] % 32);
    if (count == 0) count = 1;
    
    // Create double array from fuzz data
    double *double_array = (double*)malloc(count * sizeof(double));
    if (!double_array) return;
    
    for (int i = 0; i < count; i++) {
        // Use fuzz data to create doubles (simple conversion)
        double_array[i] = (double)data[i % size];
    }
    
    cJSON *json_array = cJSON_CreateDoubleArray(double_array, count);
    if (json_array) {
        cJSON_Delete(json_array);
    }
    
    free(double_array);
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Seed random based on fuzz data for diversity
    srand(Data[0]);
    
    // Call all target functions with the fuzz data
    fuzz_cJSON_InsertItemInArray(Data, Size);
    fuzz_cJSON_CreateIntArray(Data, Size);
    fuzz_cJSON_CreateArray();
    fuzz_cJSON_GetArrayItem(Data, Size);
    fuzz_cJSON_GetArraySize(Data, Size);
    fuzz_cJSON_CreateDoubleArray(Data, Size);
    
    return 0;
}