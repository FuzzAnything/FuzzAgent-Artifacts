// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static void test_cJSON_CreateStringArray(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    int count = data[0] % 16;
    if (count <= 0) return;
    
    const char **strings = (const char **)malloc(count * sizeof(char *));
    if (!strings) return;
    
    size_t data_offset = 1;
    for (int i = 0; i < count; i++) {
        if (data_offset >= size) {
            strings[i] = NULL;
            continue;
        }
        
        int str_len = data[data_offset] % 32;
        if (str_len == 0) str_len = 1;
        
        data_offset++;
        if (data_offset + str_len > size) {
            str_len = size - data_offset;
        }
        
        if (str_len > 0) {
            char *str = (char *)malloc(str_len + 1);
            if (str) {
                memcpy(str, data + data_offset, str_len);
                str[str_len] = '\0';
                strings[i] = str;
                data_offset += str_len;
            } else {
                strings[i] = NULL;
            }
        } else {
            strings[i] = NULL;
        }
    }
    
    cJSON *array = cJSON_CreateStringArray(strings, count);
    
    if (array) {
        int array_size = cJSON_GetArraySize(array);
        for (int i = 0; i < array_size; i++) {
            cJSON_GetArrayItem(array, i);
        }
        
        if (array_size > 0) {
            int delete_index = data[0] % array_size;
            cJSON_DeleteItemFromArray(array, delete_index);
        }
        
        cJSON_Delete(array);
    }
    
    for (int i = 0; i < count; i++) {
        free((void *)strings[i]);
    }
    free(strings);
}

static void test_cJSON_CreateIntArray(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    int count = data[0] % 32;
    if (count <= 0) return;
    
    int *numbers = (int *)malloc(count * sizeof(int));
    if (!numbers) return;
    
    for (int i = 0; i < count; i++) {
        if (i * 4 + 4 <= size) {
            numbers[i] = *((int *)(data + i * 4));
        } else {
            numbers[i] = 0;
        }
    }
    
    cJSON *array = cJSON_CreateIntArray(numbers, count);
    
    if (array) {
        int array_size = cJSON_GetArraySize(array);
        for (int i = 0; i < array_size; i++) {
            cJSON_GetArrayItem(array, i);
        }
        
        if (array_size > 0) {
            int delete_index = data[0] % array_size;
            cJSON_DeleteItemFromArray(array, delete_index);
        }
        
        cJSON_Delete(array);
    }
    
    free(numbers);
}

static void test_cJSON_CreateArray(const uint8_t *data, size_t size) {
    cJSON *array = cJSON_CreateArray();
    
    if (array) {
        int array_size = cJSON_GetArraySize(array);
        
        if (size > 0 && array_size == 0) {
            int delete_index = data[0] % 10;
            cJSON_DeleteItemFromArray(array, delete_index);
        }
        
        cJSON_Delete(array);
    }
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    test_cJSON_CreateStringArray(Data, Size);
    test_cJSON_CreateIntArray(Data, Size);
    test_cJSON_CreateArray(Data, Size);
    
    return 0;
}