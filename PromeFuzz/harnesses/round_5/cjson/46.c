// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
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

static void create_and_populate_array(cJSON **array, const uint8_t *data, size_t size) {
    *array = cJSON_CreateArray();
    if (*array == NULL) return;
    
    size_t max_items = size > 100 ? 100 : size;
    for (size_t i = 0; i < max_items; i++) {
        char str[2] = {data[i % size], '\0'};
        cJSON *item = cJSON_CreateString(str);
        if (item) {
            cJSON_AddItemToArray(*array, item);
        }
    }
}

static void create_string_array_test(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    size_t count = data[0] % 10;
    if (count == 0) count = 1;
    
    const char **strings = malloc(count * sizeof(char*));
    if (!strings) return;
    
    for (size_t i = 0; i < count; i++) {
        char *str = malloc(2);
        if (str) {
            str[0] = data[(i + 1) % size];
            str[1] = '\0';
            strings[i] = str;
        } else {
            strings[i] = "";
        }
    }
    
    cJSON *array = cJSON_CreateStringArray(strings, count);
    if (array) {
        cJSON_Delete(array);
    }
    
    for (size_t i = 0; i < count; i++) {
        if (strings[i] && strings[i][0] != '\0') {
            free((void*)strings[i]);
        }
    }
    free(strings);
}

int LLVMFuzzerTestOneInput_46(const uint8_t *data, size_t size) {
    if (size < 2) return 0;
    
    cJSON *array = NULL;
    
    create_and_populate_array(&array, data, size);
    if (!array) return 0;
    
    int array_size = cJSON_GetArraySize(array);
    
    if (array_size > 0) {
        int index1 = data[0] % (array_size + 2);
        int index2 = data[1] % (array_size + 2);
        
        cJSON_DeleteItemFromArray(array, index1 - 1);
        
        cJSON *detached = cJSON_DetachItemFromArray(array, index2 - 1);
        if (detached) {
            cJSON_Delete(detached);
        }
        
        int new_index = (data[0] + data[1]) % (cJSON_GetArraySize(array) + 2);
        cJSON *item = cJSON_GetArrayItem(array, new_index - 1);
        (void)item;
    }
    
    cJSON_Delete(array);
    
    create_string_array_test(data, size);
    
    cJSON *empty_array = cJSON_CreateArray();
    if (empty_array) {
        cJSON_GetArraySize(empty_array);
        cJSON_DeleteItemFromArray(empty_array, 0);
        cJSON_DetachItemFromArray(empty_array, 0);
        cJSON_GetArrayItem(empty_array, 0);
        cJSON_Delete(empty_array);
    }
    
    return 0;
}