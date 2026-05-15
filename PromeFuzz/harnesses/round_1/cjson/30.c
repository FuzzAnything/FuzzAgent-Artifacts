// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static cJSON* create_test_array_from_data(const uint8_t *data, size_t size) {
    if (size < 1) return cJSON_CreateArray();
    
    int count = data[0] % 32;
    int *numbers = malloc(count * sizeof(int));
    if (!numbers) return cJSON_CreateArray();
    
    for (int i = 0; i < count; i++) {
        numbers[i] = (i < size - 1) ? data[i + 1] : 0;
    }
    
    cJSON *array = cJSON_CreateIntArray(numbers, count);
    free(numbers);
    return array;
}

static cJSON* create_random_item(const uint8_t *data, size_t size, size_t offset) {
    if (offset >= size) return cJSON_CreateNull();
    
    switch (data[offset] % 7) {
        case 0: return cJSON_CreateNull();
        case 1: return cJSON_CreateTrue();
        case 2: return cJSON_CreateFalse();
        case 3: return cJSON_CreateNumber((double)(data[offset % size]));
        case 4: {
            char str[2] = {(char)data[offset % size], '\0'};
            return cJSON_CreateString(str);
        }
        case 5: return cJSON_CreateArray();
        case 6: return cJSON_CreateObject();
        default: return cJSON_CreateNull();
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *data, size_t size) {
    if (size < 2) return 0;
    
    cJSON *array = create_test_array_from_data(data, size);
    if (!array) return 0;
    
    if (!cJSON_IsArray(array)) {
        cJSON_Delete(array);
        return 0;
    }
    
    int array_size = cJSON_GetArraySize(array);
    
    size_t offset = (size_t)data[0] % (size > 1 ? size - 1 : 1);
    int which = (int)data[offset] % (array_size + 5);
    
    cJSON *newitem = create_random_item(data, size, (offset + 1) % size);
    
    cJSON_bool insert_result = cJSON_InsertItemInArray(array, which, newitem);
    if (!insert_result) {
        cJSON_Delete(newitem);
    }
    
    int new_array_size = cJSON_GetArraySize(array);
    
    if (new_array_size > 0) {
        int get_index = (int)data[(offset + 2) % size] % (new_array_size + 3);
        cJSON *item = cJSON_GetArrayItem(array, get_index);
        (void)item;
        
        if (new_array_size > 1) {
            int replace_index = (int)data[(offset + 3) % size] % new_array_size;
            cJSON *replace_item = create_random_item(data, size, (offset + 4) % size);
            cJSON_bool replace_result = cJSON_ReplaceItemInArray(array, replace_index, replace_item);
            if (!replace_result) {
                cJSON_Delete(replace_item);
            }
        }
    }
    
    cJSON_Delete(array);
    return 0;
}