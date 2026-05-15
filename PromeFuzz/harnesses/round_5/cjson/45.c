// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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

static void fuzz_cJSON_IsArray(const uint8_t *data, size_t size) {
    if (size < sizeof(cJSON)) return;
    
    cJSON item;
    memset(&item, 0, sizeof(cJSON));
    
    item.type = (int)(data[0] % 8);
    cJSON_IsArray(&item);
    
    cJSON_IsArray(NULL);
}

static void fuzz_cJSON_CreateDoubleArray(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    int count = (int)(data[0] % 100);
    if (count <= 0) return;
    
    double *numbers = (double *)malloc(count * sizeof(double));
    if (!numbers) return;
    
    for (int i = 0; i < count; i++) {
        double val = 0.0;
        size_t offset = i * sizeof(double);
        if (offset + sizeof(double) <= size) {
            memcpy(&val, data + offset, sizeof(double));
        }
        numbers[i] = val;
    }
    
    cJSON *array = cJSON_CreateDoubleArray(numbers, count);
    if (array) {
        cJSON_Delete(array);
    }
    
    free(numbers);
    
    cJSON_CreateDoubleArray(NULL, 0);
    cJSON_CreateDoubleArray(NULL, count);
}

static void fuzz_cJSON_GetArrayItem(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    int count = (int)(data[0] % 50);
    if (count <= 0) return;
    
    double *numbers = (double *)malloc(count * sizeof(double));
    if (!numbers) return;
    
    for (int i = 0; i < count; i++) {
        numbers[i] = (double)i;
    }
    
    cJSON *array = cJSON_CreateDoubleArray(numbers, count);
    if (array) {
        int index = (int)(data[1] % (count * 2));
        cJSON_GetArrayItem(array, index);
        
        cJSON_GetArrayItem(array, -1);
        cJSON_GetArrayItem(array, count);
        cJSON_GetArrayItem(array, count + 1);
        
        cJSON_Delete(array);
    }
    
    free(numbers);
    
    cJSON_GetArrayItem(NULL, 0);
}

static void fuzz_cJSON_CreateIntArray(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    int count = (int)(data[0] % 100);
    if (count <= 0) return;
    
    int *numbers = (int *)malloc(count * sizeof(int));
    if (!numbers) return;
    
    for (int i = 0; i < count; i++) {
        int val = 0;
        size_t offset = i * sizeof(int);
        if (offset + sizeof(int) <= size) {
            memcpy(&val, data + offset, sizeof(int));
        }
        numbers[i] = val;
    }
    
    cJSON *array = cJSON_CreateIntArray(numbers, count);
    if (array) {
        cJSON_Delete(array);
    }
    
    free(numbers);
    
    cJSON_CreateIntArray(NULL, 0);
    cJSON_CreateIntArray(NULL, count);
}

static void fuzz_cJSON_InsertItemInArray(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    int count = (int)(data[0] % 50);
    if (count <= 0) return;
    
    int *numbers = (int *)malloc(count * sizeof(int));
    if (!numbers) return;
    
    for (int i = 0; i < count; i++) {
        numbers[i] = i;
    }
    
    cJSON *array = cJSON_CreateIntArray(numbers, count);
    if (array) {
        int which = (int)(data[1] % (count * 2 + 5));
        cJSON *newitem = cJSON_CreateNumber((double)(data[2] % 256));
        
        cJSON_InsertItemInArray(array, which, newitem);
        
        cJSON_InsertItemInArray(array, -1, cJSON_CreateNull());
        cJSON_InsertItemInArray(array, count + 10, cJSON_CreateTrue());
        
        cJSON_Delete(array);
    } else {
        cJSON_InsertItemInArray(NULL, 0, NULL);
    }
    
    free(numbers);
}

static void fuzz_cJSON_GetArraySize(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    int count = (int)(data[0] % 50);
    if (count <= 0) return;
    
    int *numbers = (int *)malloc(count * sizeof(int));
    if (!numbers) return;
    
    for (int i = 0; i < count; i++) {
        numbers[i] = i;
    }
    
    cJSON *array = cJSON_CreateIntArray(numbers, count);
    if (array) {
        cJSON_GetArraySize(array);
        cJSON_Delete(array);
    }
    
    free(numbers);
    
    cJSON_GetArraySize(NULL);
    
    cJSON obj;
    memset(&obj, 0, sizeof(cJSON));
    obj.type = cJSON_Object;
    cJSON_GetArraySize(&obj);
}

int LLVMFuzzerTestOneInput_45(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    fuzz_cJSON_IsArray(data, size);
    fuzz_cJSON_CreateDoubleArray(data, size);
    fuzz_cJSON_GetArrayItem(data, size);
    fuzz_cJSON_CreateIntArray(data, size);
    fuzz_cJSON_InsertItemInArray(data, size);
    fuzz_cJSON_GetArraySize(data, size);
    
    return 0;
}