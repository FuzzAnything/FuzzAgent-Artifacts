// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
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
        case 0: // Number (integer)
            if (*offset + sizeof(int) <= size) {
                int value;
                memcpy(&value, data + *offset, sizeof(int));
                *offset += sizeof(int);
                return cJSON_CreateNumber(value);
            }
            return cJSON_CreateNumber(0);
            
        case 1: // Number (double)
            if (*offset + sizeof(double) <= size) {
                double value;
                memcpy(&value, data + *offset, sizeof(double));
                *offset += sizeof(double);
                return cJSON_CreateNumber(value);
            }
            return cJSON_CreateNumber(0.0);
            
        case 2: // String
            if (*offset + 1 <= size) {
                size_t str_len = data[*offset] % 64;
                (*offset)++;
                if (*offset + str_len <= size && str_len > 0) {
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
            return cJSON_CreateString("");
            
        case 3: // Array
            return cJSON_CreateArray();
            
        case 4: // Object
            return cJSON_CreateObject();
            
        case 5: // Boolean
            if (*offset < size) {
                cJSON_bool value = data[*offset] % 2;
                (*offset)++;
                return cJSON_CreateBool(value);
            }
            return cJSON_CreateBool(0);
            
        default:
            return cJSON_CreateNull();
    }
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    size_t offset = 0;
    
    // Create an array to work with
    cJSON* array = cJSON_CreateArray();
    if (!array) return 0;
    
    // Initialize array with some items
    size_t initial_items = Data[offset] % 16;
    offset = (offset + 1) % Size;
    
    for (size_t i = 0; i < initial_items; i++) {
        cJSON* item = create_random_item(Data, Size, &offset);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
        if (offset >= Size) offset = 0;
    }
    
    // Perform various operations based on fuzzer input
    while (offset < Size) {
        uint8_t operation = Data[offset] % 8;
        offset = (offset + 1) % Size;
        
        switch (operation) {
            case 0: { // cJSON_InsertItemInArray
                if (offset + 1 < Size) {
                    int which = Data[offset] % (cJSON_GetArraySize(array) + 2);
                    offset = (offset + 1) % Size;
                    
                    cJSON* newitem = create_random_item(Data, Size, &offset);
                    if (newitem) {
                        cJSON_InsertItemInArray(array, which, newitem);
                    }
                }
                break;
            }
            
            case 1: { // cJSON_IsArray
                cJSON_IsArray(array);
                // Also test with NULL
                cJSON_IsArray(NULL);
                break;
            }
            
            case 2: { // cJSON_AddItemToArray
                cJSON* item = create_random_item(Data, Size, &offset);
                if (item) {
                    cJSON_AddItemToArray(array, item);
                }
                break;
            }
            
            case 3: { // cJSON_DeleteItemFromArray
                int current_size = cJSON_GetArraySize(array);
                if (current_size > 0 && offset < Size) {
                    int which = Data[offset] % current_size;
                    offset = (offset + 1) % Size;
                    cJSON_DeleteItemFromArray(array, which);
                }
                break;
            }
            
            case 4: { // cJSON_DetachItemFromArray
                int current_size = cJSON_GetArraySize(array);
                if (current_size > 0 && offset < Size) {
                    int which = Data[offset] % current_size;
                    offset = (offset + 1) % Size;
                    cJSON* detached = cJSON_DetachItemFromArray(array, which);
                    if (detached) {
                        cJSON_Delete(detached);
                    }
                }
                // Test with negative index
                cJSON_DetachItemFromArray(array, -1);
                break;
            }
            
            case 5: { // cJSON_GetArraySize
                cJSON_GetArraySize(array);
                // Test with NULL
                cJSON_GetArraySize(NULL);
                break;
            }
            
            case 6: { // Mixed operations
                // Add an item
                cJSON* item1 = create_random_item(Data, Size, &offset);
                if (item1) {
                    cJSON_AddItemToArray(array, item1);
                }
                
                // Then immediately delete it if array has items
                int current_size = cJSON_GetArraySize(array);
                if (current_size > 0 && offset < Size) {
                    int which = Data[offset] % current_size;
                    offset = (offset + 1) % Size;
                    cJSON_DeleteItemFromArray(array, which);
                }
                break;
            }
            
            case 7: { // Test array validation
                // Create a non-array item and test cJSON_IsArray on it
                cJSON* non_array = cJSON_CreateNumber(42);
                cJSON_IsArray(non_array);
                cJSON_Delete(non_array);
                
                // Test array operations on non-array (should be safe due to cJSON_IsArray check)
                if (cJSON_IsArray(array)) {
                    // Only perform array operations if it's actually an array
                    int current_size = cJSON_GetArraySize(array);
                    if (current_size > 0 && offset < Size) {
                        int which = Data[offset] % current_size;
                        offset = (offset + 1) % Size;
                        cJSON* detached = cJSON_DetachItemFromArray(array, which);
                        if (detached) {
                            // Reinsert the detached item
                            cJSON_InsertItemInArray(array, which % (current_size + 1), detached);
                        }
                    }
                }
                break;
            }
        }
        
        // Prevent infinite loop
        if (offset == 0) offset = 1;
    }
    
    // Clean up
    cJSON_Delete(array);
    
    return 0;
}