// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON* create_random_item(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset >= size) return NULL;
    
    uint8_t type = data[*offset] % 6;
    (*offset)++;
    
    switch (type) {
        case 0: return cJSON_CreateNull();
        case 1: return cJSON_CreateTrue();
        case 2: return cJSON_CreateFalse();
        case 3: {
            if (*offset + sizeof(int) <= size) {
                int value;
                memcpy(&value, data + *offset, sizeof(int));
                *offset += sizeof(int);
                return cJSON_CreateNumber(value);
            }
            return cJSON_CreateNumber(0);
        }
        case 4: {
            if (*offset + 1 <= size) {
                size_t str_len = data[*offset] % 32 + 1;
                (*offset)++;
                if (*offset + str_len <= size) {
                    char* str = (char*)malloc(str_len + 1);
                    if (str) {
                        memcpy(str, data + *offset, str_len);
                        str[str_len] = '\0';
                        cJSON* item = cJSON_CreateString(str);
                        free(str);
                        *offset += str_len;
                        return item;
                    }
                }
            }
            return cJSON_CreateString("test");
        }
        case 5: {
            cJSON* arr = cJSON_CreateArray();
            if (arr && *offset + 1 <= size) {
                size_t item_count = data[*offset] % 5;
                (*offset)++;
                for (size_t i = 0; i < item_count && *offset < size; i++) {
                    cJSON* subitem = create_random_item(data, size, offset);
                    if (subitem) {
                        cJSON_AddItemToArray(arr, subitem);
                    }
                }
            }
            return arr;
        }
        default: return cJSON_CreateNull();
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    size_t offset = 0;
    
    // Create array
    cJSON* array = cJSON_CreateArray();
    if (!array) return 0;
    
    // Add some items to array
    size_t initial_items = Data[offset] % 10 + 1;
    offset = (offset + 1) % Size;
    
    for (size_t i = 0; i < initial_items && offset < Size; i++) {
        cJSON* item = create_random_item(Data, Size, &offset);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
    
    // Get array size
    int array_size = cJSON_GetArraySize(array);
    
    // Perform operations based on fuzzing input
    while (offset < Size) {
        uint8_t operation = Data[offset] % 6;
        offset = (offset + 1) % Size;
        
        if (array_size == 0 && operation != 0) {
            // Only allow adding items when array is empty
            operation = 0;
        }
        
        switch (operation) {
            case 0: { // Add item
                cJSON* new_item = create_random_item(Data, Size, &offset);
                if (new_item) {
                    cJSON_AddItemToArray(array, new_item);
                    array_size++;
                }
                break;
            }
            case 1: { // Delete item
                if (array_size > 0 && offset < Size) {
                    int index = Data[offset] % array_size;
                    offset = (offset + 1) % Size;
                    cJSON_DeleteItemFromArray(array, index);
                    array_size--;
                }
                break;
            }
            case 2: { // Get item
                if (array_size > 0 && offset < Size) {
                    int index = Data[offset] % array_size;
                    offset = (offset + 1) % Size;
                    cJSON_GetArrayItem(array, index);
                }
                break;
            }
            case 3: { // Detach item
                if (array_size > 0 && offset < Size) {
                    int index = Data[offset] % array_size;
                    offset = (offset + 1) % Size;
                    cJSON* detached = cJSON_DetachItemFromArray(array, index);
                    if (detached) {
                        array_size--;
                        cJSON_Delete(detached);
                    }
                }
                break;
            }
            case 4: { // Get array size
                cJSON_GetArraySize(array);
                break;
            }
            case 5: { // Mixed operations
                if (array_size > 0 && offset + 2 <= Size) {
                    int index1 = Data[offset] % array_size;
                    offset = (offset + 1) % Size;
                    int index2 = Data[offset] % array_size;
                    offset = (offset + 1) % Size;
                    
                    // Get and delete in sequence
                    cJSON* item = cJSON_GetArrayItem(array, index1);
                    if (item && index2 < array_size) {
                        cJSON_DeleteItemFromArray(array, index2);
                        array_size--;
                    }
                }
                break;
            }
        }
        
        // Update array size
        array_size = cJSON_GetArraySize(array);
    }
    
    // Clean up
    cJSON_Delete(array);
    
    return 0;
}