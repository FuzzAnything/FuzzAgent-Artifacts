// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
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

static void fuzz_cJSON_GetArraySize(const uint8_t *data, size_t size) {
    // Test with NULL input
    cJSON_GetArraySize(NULL);
    
    // Create an array from fuzzing data if possible
    if (size > 0) {
        char *json_str = (char *)malloc(size + 1);
        if (json_str) {
            memcpy(json_str, data, size);
            json_str[size] = '\0';
            
            cJSON *parsed = cJSON_Parse(json_str);
            if (parsed) {
                cJSON_GetArraySize(parsed);
                cJSON_Delete(parsed);
            }
            free(json_str);
        }
    }
}

static void fuzz_cJSON_CreateIntArray(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Use first byte as count (limited to avoid excessive memory)
    int count = data[0] % 100;
    if (count <= 0) count = 1;
    
    // Allocate integer array
    int *numbers = (int *)malloc(count * sizeof(int));
    if (!numbers) return;
    
    // Fill with fuzzing data
    for (int i = 0; i < count; i++) {
        if (i * sizeof(int) < size) {
            memcpy(&numbers[i], data + (i * sizeof(int)) % size, sizeof(int));
        } else {
            numbers[i] = i;
        }
    }
    
    cJSON *array = cJSON_CreateIntArray(numbers, count);
    if (array) {
        cJSON_Delete(array);
    }
    
    free(numbers);
}

static void fuzz_cJSON_IsArray(const uint8_t *data, size_t size) {
    // Test with NULL input
    cJSON_IsArray(NULL);
    
    if (size > 0) {
        char *json_str = (char *)malloc(size + 1);
        if (json_str) {
            memcpy(json_str, data, size);
            json_str[size] = '\0';
            
            cJSON *parsed = cJSON_Parse(json_str);
            if (parsed) {
                cJSON_IsArray(parsed);
                cJSON_Delete(parsed);
            }
            free(json_str);
        }
    }
    
    // Also test with explicitly created arrays
    cJSON *array = cJSON_CreateArray();
    if (array) {
        cJSON_IsArray(array);
        cJSON_Delete(array);
    }
    
    cJSON *number = cJSON_CreateNumber(42);
    if (number) {
        cJSON_IsArray(number);
        cJSON_Delete(number);
    }
}

static void fuzz_cJSON_CreateDoubleArray(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Use first byte as count (limited to avoid excessive memory)
    int count = data[0] % 100;
    if (count <= 0) count = 1;
    
    // Allocate double array
    double *numbers = (double *)malloc(count * sizeof(double));
    if (!numbers) return;
    
    // Fill with fuzzing data
    for (int i = 0; i < count; i++) {
        if (i * sizeof(double) < size) {
            memcpy(&numbers[i], data + (i * sizeof(double)) % size, sizeof(double));
        } else {
            numbers[i] = i * 1.5;
        }
    }
    
    cJSON *array = cJSON_CreateDoubleArray(numbers, count);
    if (array) {
        cJSON_Delete(array);
    }
    
    free(numbers);
}

static void fuzz_cJSON_GetArrayItem(const uint8_t *data, size_t size) {
    // Test with NULL inputs
    cJSON_GetArrayItem(NULL, 0);
    
    // Create test arrays
    cJSON *array = cJSON_CreateArray();
    if (!array) return;
    
    // Add some items to the array
    for (int i = 0; i < 10 && i * sizeof(int) < size; i++) {
        int value;
        memcpy(&value, data + (i * sizeof(int)) % size, sizeof(int));
        cJSON_AddItemToArray(array, cJSON_CreateNumber(value));
    }
    
    // Test various indices (including out of bounds)
    for (int i = -5; i < 15; i++) {
        cJSON_GetArrayItem(array, i);
    }
    
    // Test with actual array from parsing
    if (size > 0) {
        char *json_str = (char *)malloc(size + 1);
        if (json_str) {
            memcpy(json_str, data, size);
            json_str[size] = '\0';
            
            cJSON *parsed = cJSON_Parse(json_str);
            if (parsed && cJSON_IsArray(parsed)) {
                for (int i = -2; i < 10; i++) {
                    cJSON_GetArrayItem(parsed, i);
                }
                cJSON_Delete(parsed);
            }
            free(json_str);
        }
    }
    
    cJSON_Delete(array);
}

static void fuzz_cJSON_InsertItemInArray(const uint8_t *data, size_t size) {
    // Test with NULL inputs
    cJSON_InsertItemInArray(NULL, 0, NULL);
    
    // Create test array
    cJSON *array = cJSON_CreateArray();
    if (!array) return;
    
    // Add initial items
    for (int i = 0; i < 5; i++) {
        cJSON_AddItemToArray(array, cJSON_CreateNumber(i));
    }
    
    // Create new items to insert
    cJSON *new_items[3];
    for (int i = 0; i < 3; i++) {
        if (i * sizeof(int) < size) {
            int value;
            memcpy(&value, data + (i * sizeof(int)) % size, sizeof(int));
            new_items[i] = cJSON_CreateNumber(value);
        } else {
            new_items[i] = cJSON_CreateNumber(i * 10);
        }
    }
    
    // Test various insertion positions
    for (int i = -3; i < 10; i++) {
        for (int j = 0; j < 3; j++) {
            if (new_items[j]) {
                // Create a copy since the original gets consumed
                cJSON *copy = cJSON_Duplicate(new_items[j], 1);
                if (copy) {
                    cJSON_InsertItemInArray(array, i, copy);
                }
            }
        }
    }
    
    // Clean up new items
    for (int i = 0; i < 3; i++) {
        if (new_items[i]) {
            cJSON_Delete(new_items[i]);
        }
    }
    
    cJSON_Delete(array);
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    // Initialize cJSON if needed (some versions might need initialization)
    
    // Fuzz each API function
    fuzz_cJSON_GetArraySize(Data, Size);
    fuzz_cJSON_CreateIntArray(Data, Size);
    fuzz_cJSON_IsArray(Data, Size);
    fuzz_cJSON_CreateDoubleArray(Data, Size);
    fuzz_cJSON_GetArrayItem(Data, Size);
    fuzz_cJSON_InsertItemInArray(Data, Size);
    
    return 0;
}