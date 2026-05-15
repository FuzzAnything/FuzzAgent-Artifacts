// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
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

static cJSON *create_random_item(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return NULL;
    
    uint8_t type = data[*offset] % 6;
    (*offset)++;
    
    switch (type) {
        case 0:  // Null
            return cJSON_CreateNull();
        case 1:  // Boolean
            return cJSON_CreateBool((*offset < size) ? (data[*offset] & 1) : 0);
        case 2: { // Number (int)
            int value = 0;
            if (*offset + sizeof(int) <= size) {
                memcpy(&value, data + *offset, sizeof(int));
                *offset += sizeof(int);
            }
            return cJSON_CreateNumber(value);
        }
        case 3: { // Number (double)
            double value = 0.0;
            if (*offset + sizeof(double) <= size) {
                memcpy(&value, data + *offset, sizeof(double));
                *offset += sizeof(double);
            }
            return cJSON_CreateNumber(value);
        }
        case 4: { // String
            size_t str_len = 0;
            if (*offset < size) {
                str_len = data[*offset] % 32;
                (*offset)++;
            }
            
            if (str_len == 0 || *offset + str_len > size) {
                return cJSON_CreateString("");
            }
            
            char *str = (char*)malloc(str_len + 1);
            if (!str) return NULL;
            
            memcpy(str, data + *offset, str_len);
            str[str_len] = '\0';
            *offset += str_len;
            
            cJSON *item = cJSON_CreateString(str);
            free(str);
            return item;
        }
        case 5:  // Array
            return cJSON_CreateArray();
        default:
            return cJSON_CreateNull();
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    size_t offset = 0;
    
    // Test 1: Create and manipulate a basic array
    cJSON *array1 = cJSON_CreateArray();
    if (!array1) return 0;
    
    // Insert some items into the array
    for (int i = 0; i < 10 && offset < Size; i++) {
        cJSON *item = create_random_item(Data, Size, &offset);
        if (item) {
            int index = (offset < Size) ? Data[offset] % 10 : 0;
            offset = (offset + 1) % Size;
            cJSON_InsertItemInArray(array1, index, item);
        }
    }
    
    // Test GetArraySize and GetArrayItem
    int array1_size = cJSON_GetArraySize(array1);
    for (int i = 0; i < array1_size; i++) {
        cJSON *item = cJSON_GetArrayItem(array1, i);
        (void)item; // Use item to avoid unused variable warning
    }
    
    // Test out-of-bounds access
    cJSON *out_of_bounds = cJSON_GetArrayItem(array1, array1_size + 1);
    (void)out_of_bounds;
    
    cJSON_Delete(array1);
    
    // Test 2: Create double array
    if (offset + sizeof(double) * 2 <= Size) {
        size_t double_count = Data[offset] % 10;
        offset = (offset + 1) % Size;
        
        if (double_count > 0 && offset + sizeof(double) * double_count <= Size) {
            double *doubles = (double*)malloc(sizeof(double) * double_count);
            if (doubles) {
                for (size_t i = 0; i < double_count; i++) {
                    memcpy(&doubles[i], Data + offset + i * sizeof(double), sizeof(double));
                }
                offset += double_count * sizeof(double);
                
                cJSON *double_array = cJSON_CreateDoubleArray(doubles, double_count);
                if (double_array) {
                    int double_array_size = cJSON_GetArraySize(double_array);
                    for (int i = 0; i < double_array_size; i++) {
                        cJSON *item = cJSON_GetArrayItem(double_array, i);
                        (void)item;
                    }
                    cJSON_Delete(double_array);
                }
                free(doubles);
            }
        }
    }
    
    // Test 3: Create integer array
    if (offset + sizeof(int) * 2 <= Size) {
        size_t int_count = Data[offset] % 10;
        offset = (offset + 1) % Size;
        
        if (int_count > 0 && offset + sizeof(int) * int_count <= Size) {
            int *ints = (int*)malloc(sizeof(int) * int_count);
            if (ints) {
                for (size_t i = 0; i < int_count; i++) {
                    memcpy(&ints[i], Data + offset + i * sizeof(int), sizeof(int));
                }
                offset += int_count * sizeof(int);
                
                cJSON *int_array = cJSON_CreateIntArray(ints, int_count);
                if (int_array) {
                    int int_array_size = cJSON_GetArraySize(int_array);
                    for (int i = 0; i < int_array_size; i++) {
                        cJSON *item = cJSON_GetArrayItem(int_array, i);
                        (void)item;
                    }
                    cJSON_Delete(int_array);
                }
                free(ints);
            }
        }
    }
    
    // Test 4: Create another array with more complex operations
    cJSON *array2 = cJSON_CreateArray();
    if (array2) {
        // Insert items at various positions
        for (int i = 0; i < 5 && offset < Size; i++) {
            cJSON *item = create_random_item(Data, Size, &offset);
            if (item) {
                // Use data to determine insertion index
                int index = (offset < Size) ? Data[offset] % 20 : i;
                offset = (offset + 1) % Size;
                cJSON_InsertItemInArray(array2, index, item);
            }
        }
        
        // Test edge cases for GetArrayItem
        cJSON_GetArrayItem(array2, -1);  // Negative index
        cJSON_GetArrayItem(array2, 100); // Large index
        cJSON_GetArrayItem(NULL, 0);     // NULL array
        
        // Test GetArraySize with NULL
        cJSON_GetArraySize(NULL);
        
        cJSON_Delete(array2);
    }
    
    // Test 5: Test InsertItemInArray with NULL parameters
    cJSON_InsertItemInArray(NULL, 0, NULL);
    
    cJSON *dummy_item = cJSON_CreateNull();
    if (dummy_item) {
        cJSON_InsertItemInArray(NULL, 0, dummy_item);
        cJSON_Delete(dummy_item);
    }
    
    return 0;
}