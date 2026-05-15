// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static void populate_array(cJSON *array, const uint8_t *data, size_t size) {
    if (!array || size == 0) return;
    
    for (size_t i = 0; i < size && i < 10; i++) {
        cJSON *item = cJSON_CreateNumber(data[i]);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Test cJSON_CreateArray */
    cJSON *array = cJSON_CreateArray();
    if (!array) return 0;
    
    /* Test cJSON_CreateIntArray */
    int int_count = Size > 0 ? (Data[0] % 16) + 1 : 1;
    if (int_count > (int)Size) int_count = Size;
    
    int *int_numbers = malloc(int_count * sizeof(int));
    if (int_numbers) {
        for (int i = 0; i < int_count; i++) {
            int_numbers[i] = (i < (int)Size) ? Data[i] : 0;
        }
        cJSON *int_array = cJSON_CreateIntArray(int_numbers, int_count);
        if (int_array) {
            /* Test cJSON_GetArraySize */
            int array_size = cJSON_GetArraySize(int_array);
            (void)array_size;
            
            /* Test cJSON_GetArrayItem */
            for (int i = 0; i < array_size && i < 5; i++) {
                cJSON *item = cJSON_GetArrayItem(int_array, i);
                (void)item;
            }
            
            /* Test cJSON_InsertItemInArray */
            if (array_size > 0) {
                cJSON *new_item = cJSON_CreateNumber(42);
                if (new_item) {
                    int insert_pos = (Data[0] % (array_size + 2));
                    cJSON_InsertItemInArray(int_array, insert_pos, new_item);
                }
            }
            
            cJSON_Delete(int_array);
        }
        free(int_numbers);
    }
    
    /* Test cJSON_CreateDoubleArray */
    int double_count = Size > 1 ? (Data[1] % 16) + 1 : 1;
    if (double_count > (int)Size) double_count = Size;
    
    double *double_numbers = malloc(double_count * sizeof(double));
    if (double_numbers) {
        for (int i = 0; i < double_count; i++) {
            double_numbers[i] = (i < (int)Size) ? (double)Data[i] : 0.0;
        }
        cJSON *double_array = cJSON_CreateDoubleArray(double_numbers, double_count);
        if (double_array) {
            /* Test array operations on double array */
            int dbl_array_size = cJSON_GetArraySize(double_array);
            (void)dbl_array_size;
            
            for (int i = 0; i < dbl_array_size && i < 5; i++) {
                cJSON *item = cJSON_GetArrayItem(double_array, i);
                (void)item;
            }
            
            cJSON_Delete(double_array);
        }
        free(double_numbers);
    }
    
    /* Populate the main array with some items */
    populate_array(array, Data, Size);
    
    /* Test array operations on the main array */
    int main_array_size = cJSON_GetArraySize(array);
    (void)main_array_size;
    
    /* Test cJSON_GetArrayItem with various indices */
    for (int i = 0; i < main_array_size && i < 10; i++) {
        cJSON *item = cJSON_GetArrayItem(array, i);
        (void)item;
    }
    
    /* Test cJSON_InsertItemInArray with various positions */
    if (main_array_size > 0) {
        for (int i = 0; i < 3 && i < (int)Size; i++) {
            cJSON *new_item = cJSON_CreateNumber(Data[i % Size]);
            if (new_item) {
                int insert_pos = (Data[i % Size] % (main_array_size + 2));
                cJSON_InsertItemInArray(array, insert_pos, new_item);
                main_array_size = cJSON_GetArraySize(array);
            }
        }
    }
    
    /* Test edge cases for cJSON_GetArrayItem */
    if (array) {
        cJSON *item = cJSON_GetArrayItem(array, -1);
        (void)item;
        item = cJSON_GetArrayItem(array, main_array_size);
        (void)item;
        item = cJSON_GetArrayItem(array, main_array_size + 1);
        (void)item;
    }
    
    /* Test cJSON_GetArraySize with NULL */
    int null_size = cJSON_GetArraySize(NULL);
    (void)null_size;
    
    /* Cleanup */
    cJSON_Delete(array);
    
    return 0;
}