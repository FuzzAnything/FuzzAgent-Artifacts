// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
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
    if (size < 2) return;
    
    // Determine count of strings (1-10)
    int count = (data[0] % 10) + 1;
    if (count * 2 > (int)size - 1) return;
    
    const char **strings = malloc(count * sizeof(char *));
    if (!strings) return;
    
    size_t offset = 1;
    for (int i = 0; i < count; i++) {
        if (offset >= size) {
            strings[i] = NULL;
            continue;
        }
        
        // Each string gets up to 10 bytes
        int str_len = data[offset] % 10 + 1;
        offset++;
        
        if (offset + str_len > size) {
            str_len = size - offset;
        }
        
        if (str_len > 0) {
            char *str = malloc(str_len + 1);
            if (str) {
                memcpy(str, data + offset, str_len);
                str[str_len] = '\0';
                strings[i] = str;
                offset += str_len;
            } else {
                strings[i] = NULL;
            }
        } else {
            strings[i] = NULL;
        }
    }
    
    cJSON *array = cJSON_CreateStringArray(strings, count);
    
    // Clean up allocated strings
    for (int i = 0; i < count; i++) {
        if (strings[i]) free((void *)strings[i]);
    }
    free(strings);
    
    if (array) {
        cJSON_Delete(array);
    }
}

static void fuzz_cJSON_CreateIntArray(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Determine count of integers (1-20)
    int count = (data[0] % 20) + 1;
    if (count * sizeof(int) > size - 1) return;
    
    int *numbers = malloc(count * sizeof(int));
    if (!numbers) return;
    
    // Use data bytes to fill integer array
    for (int i = 0; i < count; i++) {
        if (1 + i * sizeof(int) + sizeof(int) <= size) {
            memcpy(&numbers[i], data + 1 + i * sizeof(int), sizeof(int));
        } else {
            numbers[i] = 0;
        }
    }
    
    cJSON *array = cJSON_CreateIntArray(numbers, count);
    free(numbers);
    
    if (array) {
        cJSON_Delete(array);
    }
}

static void fuzz_cJSON_InsertItemInArray(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Create an array first
    cJSON *array = cJSON_CreateArray();
    if (!array) return;
    
    // Add some initial items
    for (int i = 0; i < 5 && i * 2 < (int)size; i++) {
        cJSON *item = cJSON_CreateNumber(data[i] % 100);
        cJSON_AddItemToArray(array, item);
    }
    
    // Determine insertion index
    int which = data[0] % 20;  // 0-19
    
    // Create new item to insert
    cJSON *newitem = NULL;
    switch (data[1] % 4) {
        case 0:
            newitem = cJSON_CreateNumber(data[2] % 100);
            break;
        case 1:
            newitem = cJSON_CreateString("fuzz_string");
            break;
        case 2:
            newitem = cJSON_CreateBool(data[2] % 2);
            break;
        case 3:
            newitem = cJSON_CreateNull();
            break;
    }
    
    if (newitem) {
        cJSON_InsertItemInArray(array, which, newitem);
    }
    
    cJSON_Delete(array);
}

static void fuzz_cJSON_IsArray(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    cJSON *item = NULL;
    
    switch (data[0] % 5) {
        case 0:
            item = cJSON_CreateArray();
            break;
        case 1:
            item = cJSON_CreateObject();
            break;
        case 2:
            item = cJSON_CreateString("test");
            break;
        case 3:
            item = cJSON_CreateNumber(42);
            break;
        case 4:
            item = NULL;
            break;
    }
    
    if (item) {
        cJSON_bool is_array = cJSON_IsArray(item);
        (void)is_array;  // Use result to avoid unused variable warning
        cJSON_Delete(item);
    } else {
        cJSON_bool is_array = cJSON_IsArray(NULL);
        (void)is_array;
    }
}

static void fuzz_cJSON_GetArrayItem(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Create an array with some items
    cJSON *array = cJSON_CreateArray();
    if (!array) return;
    
    // Add 10 items to the array
    for (int i = 0; i < 10; i++) {
        char str[20];
        snprintf(str, sizeof(str), "item_%d", i);
        cJSON_AddItemToArray(array, cJSON_CreateString(str));
    }
    
    // Determine index to retrieve (0-15, some out of bounds)
    int index = data[0] % 16;
    
    cJSON *item = cJSON_GetArrayItem(array, index);
    (void)item;  // Use result to avoid unused variable warning
    
    cJSON_Delete(array);
}

static void fuzz_cJSON_GetArraySize(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    cJSON *container = NULL;
    
    switch (data[0] % 4) {
        case 0: {
            // Create an array
            container = cJSON_CreateArray();
            if (container) {
                // Add some items
                for (int i = 0; i < 5 && i * 2 < (int)size; i++) {
                    cJSON_AddItemToArray(container, cJSON_CreateNumber(data[i] % 100));
                }
            }
            break;
        }
        case 1: {
            // Create an object
            container = cJSON_CreateObject();
            if (container) {
                // Add some items
                for (int i = 0; i < 3 && i * 2 < (int)size; i++) {
                    char key[10];
                    snprintf(key, sizeof(key), "key%d", i);
                    cJSON_AddItemToObject(container, key, cJSON_CreateNumber(data[i] % 100));
                }
            }
            break;
        }
        case 2:
            // Create a non-array/object item
            container = cJSON_CreateString("test");
            break;
        case 3:
            // NULL case
            container = NULL;
            break;
    }
    
    int arr_size = cJSON_GetArraySize(container);
    (void)arr_size;  // Use result to avoid unused variable warning
    
    if (container) {
        cJSON_Delete(container);
    }
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Use first byte to determine which function(s) to test
    uint8_t selector = Data[0];
    
    // Test all functions with different probabilities
    if (selector % 6 == 0) {
        fuzz_cJSON_CreateStringArray(Data, Size);
    }
    if (selector % 5 == 0) {
        fuzz_cJSON_CreateIntArray(Data, Size);
    }
    if (selector % 4 == 0) {
        fuzz_cJSON_InsertItemInArray(Data, Size);
    }
    if (selector % 3 == 0) {
        fuzz_cJSON_IsArray(Data, Size);
    }
    if (selector % 2 == 0) {
        fuzz_cJSON_GetArrayItem(Data, Size);
    }
    
    // Always test GetArraySize
    fuzz_cJSON_GetArraySize(Data, Size);
    
    return 0;
}