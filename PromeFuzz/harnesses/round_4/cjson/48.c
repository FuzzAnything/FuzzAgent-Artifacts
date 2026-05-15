// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
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

static int read_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) <= size) {
        int val;
        memcpy(&val, data + *offset, sizeof(int));
        *offset += sizeof(int);
        return val;
    }
    return 0;
}

static char *read_string(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return NULL;
    
    size_t max_len = size - *offset;
    size_t len = data[*offset] % (max_len + 1);
    if (len == 0) {
        (*offset)++;
        char *str = malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    char *str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data + *offset + 1, len);
    str[len] = '\0';
    *offset += 1 + len;
    return str;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    size_t offset = 0;
    
    // Test 1: cJSON_GetArraySize with various inputs
    {
        // Test with NULL
        cJSON_GetArraySize(NULL);
        
        // Create an array and test
        cJSON *array = cJSON_CreateArray();
        if (array) {
            cJSON_GetArraySize(array);
            
            // Add some items to the array
            for (int i = 0; i < 5 && offset < Size; i++) {
                int val = read_int(Data, Size, &offset);
                cJSON *item = cJSON_CreateNumber(val);
                if (item) {
                    cJSON_AddItemToArray(array, item);
                }
            }
            cJSON_GetArraySize(array);
            cJSON_Delete(array);
        }
    }
    
    // Test 2: cJSON_CreateIntArray
    {
        int count = (Data[offset % Size] % 20) + 1; // 1-20
        offset = (offset + 1) % Size;
        
        int *numbers = malloc(count * sizeof(int));
        if (numbers) {
            for (int i = 0; i < count; i++) {
                numbers[i] = read_int(Data, Size, &offset);
            }
            
            cJSON *int_array = cJSON_CreateIntArray(numbers, count);
            if (int_array) {
                // Test cJSON_IsArray on the created array
                cJSON_IsArray(int_array);
                
                // Test cJSON_GetArrayItem on the array
                for (int i = -5; i < count + 5; i++) {
                    cJSON_GetArrayItem(int_array, i);
                }
                
                // Test cJSON_GetArraySize on the array
                cJSON_GetArraySize(int_array);
                
                cJSON_Delete(int_array);
            }
            free(numbers);
        }
    }
    
    // Test 3: cJSON_IsArray with various inputs
    {
        cJSON_IsArray(NULL);
        
        cJSON *null_json = NULL;
        cJSON_IsArray(null_json);
        
        cJSON *array = cJSON_CreateArray();
        if (array) {
            cJSON_IsArray(array);
            cJSON_Delete(array);
        }
        
        cJSON *object = cJSON_CreateObject();
        if (object) {
            cJSON_IsArray(object);
            cJSON_Delete(object);
        }
        
        cJSON *number = cJSON_CreateNumber(42);
        if (number) {
            cJSON_IsArray(number);
            cJSON_Delete(number);
        }
    }
    
    // Test 4: cJSON_CreateStringArray
    {
        int str_count = (Data[(offset + 1) % Size] % 10) + 1; // 1-10
        offset = (offset + 1) % Size;
        
        char **strings = malloc(str_count * sizeof(char *));
        if (strings) {
            int valid_strings = 0;
            for (int i = 0; i < str_count && offset < Size; i++) {
                strings[i] = read_string(Data, Size, &offset);
                if (strings[i]) {
                    valid_strings++;
                } else {
                    strings[i] = "";
                }
            }
            
            cJSON *str_array = cJSON_CreateStringArray((const char *const *)strings, valid_strings);
            if (str_array) {
                // Test array operations on the string array
                cJSON_IsArray(str_array);
                cJSON_GetArraySize(str_array);
                
                for (int i = -3; i < valid_strings + 3; i++) {
                    cJSON_GetArrayItem(str_array, i);
                }
                
                cJSON_Delete(str_array);
            }
            
            // Free allocated strings
            for (int i = 0; i < str_count; i++) {
                if (strings[i] && strings[i][0] != '\0') {
                    free(strings[i]);
                }
            }
            free(strings);
        }
    }
    
    // Test 5: cJSON_GetArrayItem with various scenarios
    {
        // Create an array with mixed types
        cJSON *array = cJSON_CreateArray();
        if (array) {
            // Add various items
            cJSON_AddItemToArray(array, cJSON_CreateNumber(1));
            cJSON_AddItemToArray(array, cJSON_CreateString("test"));
            cJSON_AddItemToArray(array, cJSON_CreateBool(1));
            cJSON_AddItemToArray(array, cJSON_CreateNull());
            
            // Test valid and invalid indices
            for (int i = -10; i < 20; i++) {
                cJSON_GetArrayItem(array, i);
            }
            
            // Test with NULL array
            cJSON_GetArrayItem(NULL, 0);
            
            cJSON_Delete(array);
        }
    }
    
    // Test 6: cJSON_InsertItemInArray
    {
        cJSON *array = cJSON_CreateArray();
        if (array) {
            // Add some initial items
            for (int i = 0; i < 3; i++) {
                cJSON_AddItemToArray(array, cJSON_CreateNumber(i));
            }
            
            // Test insertions at various positions
            cJSON *new_item = cJSON_CreateString("inserted");
            if (new_item) {
                // Test valid indices
                cJSON_InsertItemInArray(array, 0, cJSON_Duplicate(new_item, 1));
                cJSON_InsertItemInArray(array, 2, cJSON_Duplicate(new_item, 1));
                cJSON_InsertItemInArray(array, 10, cJSON_Duplicate(new_item, 1)); // Should append
                
                // Test negative index
                cJSON_InsertItemInArray(array, -1, cJSON_Duplicate(new_item, 1));
                
                // Test with NULL array
                cJSON_InsertItemInArray(NULL, 0, cJSON_Duplicate(new_item, 1));
                
                // Test with NULL item
                cJSON_InsertItemInArray(array, 0, NULL);
                
                cJSON_Delete(new_item);
            }
            
            cJSON_Delete(array);
        }
    }
    
    // Additional test: Combined operations
    {
        // Create an array using CreateIntArray
        int small_count = 3;
        int small_numbers[] = {1, 2, 3};
        cJSON *array = cJSON_CreateIntArray(small_numbers, small_count);
        
        if (array) {
            // Verify it's an array
            if (cJSON_IsArray(array)) {
                // Get size
                int size = cJSON_GetArraySize(array);
                
                // Access all items
                for (int i = 0; i < size; i++) {
                    cJSON_GetArrayItem(array, i);
                }
                
                // Insert a new item
                cJSON *new_num = cJSON_CreateNumber(99);
                if (new_num) {
                    cJSON_InsertItemInArray(array, 1, new_num);
                    
                    // Get new size
                    cJSON_GetArraySize(array);
                    
                    // Access items again
                    for (int i = 0; i < cJSON_GetArraySize(array); i++) {
                        cJSON_GetArrayItem(array, i);
                    }
                }
            }
            cJSON_Delete(array);
        }
    }
    
    return 0;
}