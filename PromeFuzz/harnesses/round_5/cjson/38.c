// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static cJSON* create_test_array(const uint8_t *data, size_t size) {
    cJSON *array = cJSON_CreateArray();
    if (!array) return NULL;
    
    size_t i;
    for (i = 0; i < size && i < 10; i++) {
        cJSON *item = cJSON_CreateNumber(data[i]);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
    
    return array;
}

static cJSON* create_test_item(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateNull();
    }
    
    if (size % 3 == 0) {
        char *str = malloc(size + 1);
        if (!str) return cJSON_CreateNull();
        memcpy(str, data, size);
        str[size] = '\0';
        cJSON *item = cJSON_CreateString(str);
        free(str);
        return item;
    } else if (size % 3 == 1) {
        double num = 0;
        if (size >= sizeof(double)) {
            memcpy(&num, data, sizeof(double));
        } else {
            num = (double)size;
        }
        return cJSON_CreateNumber(num);
    } else {
        return cJSON_CreateBool(size % 2);
    }
}

int LLVMFuzzerTestOneInput_38(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    cJSON *array = create_test_array(data, size);
    if (!array) return 0;
    
    int index = (int)data[0];
    if (size > 1) {
        index = (index << 8) | data[1];
    }
    
    cJSON *new_item = create_test_item(data + (size > 2 ? 2 : 0), 
                                       size > 2 ? size - 2 : 0);
    
    cJSON_bool is_array = cJSON_IsArray(array);
    (void)is_array;
    
    cJSON *detached = cJSON_DetachItemFromArray(array, index);
    if (detached) {
        cJSON_Delete(detached);
    }
    
    if (size > 3) {
        int delete_index = (int)data[2];
        cJSON_DeleteItemFromArray(array, delete_index);
    }
    
    cJSON *get_item = cJSON_GetArrayItem(array, index);
    (void)get_item;
    
    if (new_item) {
        cJSON_ReplaceItemInArray(array, index, new_item);
        cJSON_InsertItemInArray(array, index, new_item);
    }
    
    cJSON_Delete(array);
    
    return 0;
}