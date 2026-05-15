// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
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

static void fuzz_cJSON_CreateStringArray(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    int count = data[0] % 16;
    if (count <= 0) return;
    
    const char **strings = malloc(count * sizeof(char*));
    if (!strings) return;
    
    size_t pos = 1;
    for (int i = 0; i < count; i++) {
        if (pos >= size) {
            strings[i] = "";
        } else {
            size_t len = data[pos] % 32;
            if (len == 0) len = 1;
            if (pos + len > size) len = size - pos;
            
            char *str = malloc(len + 1);
            if (str) {
                memcpy(str, data + pos, len);
                str[len] = '\0';
                strings[i] = str;
            } else {
                strings[i] = "";
            }
            pos += len;
        }
    }
    
    cJSON *array = cJSON_CreateStringArray(strings, count);
    
    if (array) {
        cJSON_IsArray(array);
        cJSON_GetArraySize(array);
        for (int i = 0; i < count * 2; i++) {
            cJSON_GetArrayItem(array, i);
        }
        cJSON_Delete(array);
    }
    
    for (int i = 0; i < count; i++) {
        if (strings[i] && strings[i][0] != '\0') {
            free((void*)strings[i]);
        }
    }
    free(strings);
}

static void fuzz_cJSON_CreateIntArray(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    int count = data[0] % 16;
    if (count <= 0) return;
    
    int *numbers = malloc(count * sizeof(int));
    if (!numbers) return;
    
    for (int i = 0; i < count; i++) {
        if (i * 4 + 4 <= size) {
            numbers[i] = *(int*)(data + i * 4);
        } else {
            numbers[i] = i;
        }
    }
    
    cJSON *array = cJSON_CreateIntArray(numbers, count);
    
    if (array) {
        cJSON_IsArray(array);
        cJSON_GetArraySize(array);
        for (int i = 0; i < count * 2; i++) {
            cJSON_GetArrayItem(array, i);
        }
        
        if (count > 0) {
            cJSON *new_item = cJSON_CreateNumber(data[1]);
            if (new_item) {
                int insert_pos = data[2] % (count + 2);
                cJSON_InsertItemInArray(array, insert_pos, new_item);
            }
        }
        
        cJSON_Delete(array);
    }
    
    free(numbers);
}

static void fuzz_cJSON_InsertItemInArray(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    int array_size = data[0] % 8;
    cJSON *array = cJSON_CreateArray();
    
    for (int i = 0; i < array_size; i++) {
        cJSON *item = cJSON_CreateNumber(i);
        cJSON_AddItemToArray(array, item);
    }
    
    if (array) {
        cJSON *new_item = cJSON_CreateString("test");
        if (new_item) {
            int insert_pos = data[1] % (array_size + 3);
            cJSON_InsertItemInArray(array, insert_pos, new_item);
        }
        
        cJSON_IsArray(array);
        cJSON_GetArraySize(array);
        for (int i = 0; i < array_size * 2; i++) {
            cJSON_GetArrayItem(array, i);
        }
        
        cJSON_Delete(array);
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    uint8_t mode = data[0] % 3;
    
    switch (mode) {
        case 0:
            fuzz_cJSON_CreateStringArray(data, size);
            break;
        case 1:
            fuzz_cJSON_CreateIntArray(data, size);
            break;
        case 2:
            fuzz_cJSON_InsertItemInArray(data, size);
            break;
    }
    
    return 0;
}