// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON* create_random_item(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return NULL;
    
    uint8_t type = data[*offset] % 6;
    (*offset)++;
    
    switch (type) {
        case 0:
            return cJSON_CreateNull();
        case 1:
            if (*offset + 4 <= size) {
                int value = *(int*)(data + *offset);
                *offset += 4;
                return cJSON_CreateNumber(value);
            }
            break;
        case 2:
            if (*offset + 8 <= size) {
                double value = *(double*)(data + *offset);
                *offset += 8;
                return cJSON_CreateNumber(value);
            }
            break;
        case 3:
            if (*offset + 1 <= size) {
                size_t str_len = data[*offset] % 32;
                (*offset)++;
                if (*offset + str_len <= size && str_len > 0) {
                    char *str = (char*)malloc(str_len + 1);
                    if (str) {
                        memcpy(str, data + *offset, str_len);
                        str[str_len] = '\0';
                        *offset += str_len;
                        cJSON *item = cJSON_CreateString(str);
                        free(str);
                        return item;
                    }
                }
            }
            break;
        case 4:
            return cJSON_CreateArray();
        case 5:
            return cJSON_CreateObject();
    }
    return cJSON_CreateNull();
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    size_t offset = 0;
    
    // Create test arrays
    cJSON *array1 = cJSON_CreateArray();
    cJSON *array2 = cJSON_CreateArray();
    
    if (!array1 || !array2) {
        if (array1) cJSON_Delete(array1);
        if (array2) cJSON_Delete(array2);
        return 0;
    }
    
    // Test cJSON_IsArray
    cJSON_bool is_array1 = cJSON_IsArray(array1);
    cJSON_bool is_array2 = cJSON_IsArray(array2);
    cJSON_bool is_array_null = cJSON_IsArray(NULL);
    
    // Add some initial items to arrays
    for (int i = 0; i < 5 && offset < Size; i++) {
        cJSON *item = create_random_item(Data, Size, &offset);
        if (item) {
            cJSON_AddItemToArray(array1, item);
        }
    }
    
    // Test cJSON_GetArraySize
    int size1 = cJSON_GetArraySize(array1);
    int size2 = cJSON_GetArraySize(array2);
    int size_null = cJSON_GetArraySize(NULL);
    
    // Test cJSON_GetArrayItem with various indices
    for (int i = -2; i < size1 + 3; i++) {
        cJSON *item = cJSON_GetArrayItem(array1, i);
        (void)item; // Use item to avoid unused variable warning
    }
    
    // Test cJSON_InsertItemInArray
    for (int i = 0; i < 10 && offset + 1 < Size; i++) {
        cJSON *new_item = create_random_item(Data, Size, &offset);
        if (new_item) {
            int which = Data[offset] % (size1 + 5);
            offset = (offset + 1) % Size;
            
            cJSON_bool insert_result = cJSON_InsertItemInArray(array1, which, new_item);
            (void)insert_result;
            
            // Update size after insertion
            size1 = cJSON_GetArraySize(array1);
        }
    }
    
    // Test cJSON_CreateIntArray
    if (offset + 4 <= Size) {
        int count = *(int*)(Data + offset) % 20;
        offset = (offset + 4) % Size;
        
        if (count > 0 && offset + count * 4 <= Size) {
            int *int_numbers = (int*)malloc(count * sizeof(int));
            if (int_numbers) {
                for (int i = 0; i < count; i++) {
                    int_numbers[i] = *(int*)(Data + offset);
                    offset = (offset + 4) % Size;
                }
                cJSON *int_array = cJSON_CreateIntArray(int_numbers, count);
                if (int_array) {
                    // Test array functions on the new array
                    int int_array_size = cJSON_GetArraySize(int_array);
                    cJSON_bool is_int_array = cJSON_IsArray(int_array);
                    for (int i = 0; i < int_array_size + 2; i++) {
                        cJSON *item = cJSON_GetArrayItem(int_array, i);
                        (void)item;
                    }
                    cJSON_Delete(int_array);
                }
                free(int_numbers);
            }
        }
    }
    
    // Test cJSON_CreateDoubleArray
    if (offset + 4 <= Size) {
        int count = *(int*)(Data + offset) % 20;
        offset = (offset + 4) % Size;
        
        if (count > 0 && offset + count * 8 <= Size) {
            double *double_numbers = (double*)malloc(count * sizeof(double));
            if (double_numbers) {
                for (int i = 0; i < count; i++) {
                    double_numbers[i] = *(double*)(Data + offset);
                    offset = (offset + 8) % Size;
                }
                cJSON *double_array = cJSON_CreateDoubleArray(double_numbers, count);
                if (double_array) {
                    // Test array functions on the new array
                    int double_array_size = cJSON_GetArraySize(double_array);
                    cJSON_bool is_double_array = cJSON_IsArray(double_array);
                    for (int i = 0; i < double_array_size + 2; i++) {
                        cJSON *item = cJSON_GetArrayItem(double_array, i);
                        (void)item;
                    }
                    
                    // Test insertion in the double array
                    if (offset + 1 < Size) {
                        cJSON *new_item = cJSON_CreateNumber(3.14);
                        if (new_item) {
                            int which = Data[offset] % (double_array_size + 3);
                            offset = (offset + 1) % Size;
                            cJSON_InsertItemInArray(double_array, which, new_item);
                        }
                    }
                    cJSON_Delete(double_array);
                }
                free(double_numbers);
            }
        }
    }
    
    // Cleanup
    cJSON_Delete(array1);
    cJSON_Delete(array2);
    
    return 0;
}