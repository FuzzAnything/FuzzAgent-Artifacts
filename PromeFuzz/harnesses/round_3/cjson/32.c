// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
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

static float read_float(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + sizeof(float) > size) {
        return 0.0f;
    }
    float value;
    memcpy(&value, data + *offset, sizeof(float));
    *offset += sizeof(float);
    return value;
}

static int read_int(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + sizeof(int) > size) {
        return 0;
    }
    int value;
    memcpy(&value, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return value;
}

static cJSON *create_random_item(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset >= size) {
        return NULL;
    }
    
    uint8_t type = data[*offset] % 7;
    (*offset)++;
    
    switch (type) {
        case 0:
            return cJSON_CreateNull();
        case 1: {
            float num = read_float(data, offset, size);
            return cJSON_CreateNumber(num);
        }
        case 2: {
            int str_len = data[*offset] % 32;
            if (*offset + str_len > size) {
                return cJSON_CreateString("");
            }
            char *str = (char *)malloc(str_len + 1);
            if (!str) return cJSON_CreateString("");
            memcpy(str, data + *offset, str_len);
            str[str_len] = '\0';
            (*offset) += str_len;
            cJSON *item = cJSON_CreateString(str);
            free(str);
            return item;
        }
        case 3:
            return cJSON_CreateArray();
        case 4:
            return cJSON_CreateObject();
        case 5:
            return cJSON_CreateTrue();
        case 6:
            return cJSON_CreateFalse();
        default:
            return cJSON_CreateNull();
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    size_t offset = 0;
    
    // Create initial array with some items
    cJSON *array = cJSON_CreateArray();
    if (!array) return 0;
    
    // Add some initial items to the array
    int initial_count = Data[offset] % 8;
    offset++;
    
    for (int i = 0; i < initial_count && offset < Size; i++) {
        cJSON *item = create_random_item(Data, &offset, Size);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
    
    // Test cJSON_GetArraySize
    int array_size = cJSON_GetArraySize(array);
    
    // Test cJSON_GetArrayItem with various indices
    for (int i = 0; i < array_size + 2; i++) {
        cJSON *item = cJSON_GetArrayItem(array, i);
        (void)item; // Use item to avoid unused variable warning
    }
    
    // Test cJSON_CreateFloatArray
    if (offset + sizeof(int) < Size) {
        int float_count = read_int(Data, &offset, Size) % 16;
        if (float_count < 0) float_count = 0;
        
        float *float_array = NULL;
        if (float_count > 0) {
            float_array = (float *)malloc(float_count * sizeof(float));
            if (float_array) {
                for (int i = 0; i < float_count && offset < Size; i++) {
                    float_array[i] = read_float(Data, &offset, Size);
                }
            }
        }
        
        cJSON *float_json_array = cJSON_CreateFloatArray(float_array, float_count);
        if (float_json_array) {
            cJSON_Delete(float_json_array);
        }
        free(float_array);
    }
    
    // Test cJSON_ReplaceItemInArray
    if (array_size > 0 && offset < Size) {
        int replace_index = read_int(Data, &offset, Size);
        cJSON *new_item = create_random_item(Data, &offset, Size);
        if (new_item) {
            cJSON_bool replace_result = cJSON_ReplaceItemInArray(array, replace_index, new_item);
            (void)replace_result;
            // If replacement failed, we need to clean up new_item
            if (!replace_result) {
                cJSON_Delete(new_item);
            }
        }
    }
    
    // Test cJSON_ReplaceItemViaPointer
    if (array_size > 0 && offset < Size) {
        int item_index = read_int(Data, &offset, Size) % array_size;
        cJSON *item_to_replace = cJSON_GetArrayItem(array, item_index);
        cJSON *replacement = create_random_item(Data, &offset, Size);
        
        if (item_to_replace && replacement) {
            cJSON_bool replace_result = cJSON_ReplaceItemViaPointer(array, item_to_replace, replacement);
            (void)replace_result;
            if (!replace_result) {
                cJSON_Delete(replacement);
            }
        } else if (replacement) {
            cJSON_Delete(replacement);
        }
    }
    
    // Test cJSON_DeleteItemFromArray
    if (array_size > 0 && offset < Size) {
        int delete_index = read_int(Data, &offset, Size);
        cJSON_DeleteItemFromArray(array, delete_index);
    }
    
    // Clean up
    cJSON_Delete(array);
    
    return 0;
}