// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON* create_random_array(const uint8_t* data, size_t size, int use_double) {
    if (size < 2) return NULL;
    
    int count = data[0] % 16 + 1;  // 1-16 elements
    if (count * sizeof(double) > size - 1) {
        count = (size - 1) / sizeof(double);
        if (count <= 0) return NULL;
    }
    
    if (use_double) {
        double* numbers = (double*)malloc(count * sizeof(double));
        if (!numbers) return NULL;
        
        for (int i = 0; i < count; i++) {
            double val;
            memcpy(&val, data + 1 + i * sizeof(double), sizeof(double));
            numbers[i] = val;
        }
        
        cJSON* arr = cJSON_CreateDoubleArray(numbers, count);
        free(numbers);
        return arr;
    } else {
        int* numbers = (int*)malloc(count * sizeof(int));
        if (!numbers) return NULL;
        
        for (int i = 0; i < count; i++) {
            int val;
            memcpy(&val, data + 1 + i * sizeof(int), sizeof(int));
            numbers[i] = val;
        }
        
        cJSON* arr = cJSON_CreateIntArray(numbers, count);
        free(numbers);
        return arr;
    }
}

static void test_array_operations(cJSON* array, const uint8_t* data, size_t size) {
    if (!array) return;
    
    // Test cJSON_IsArray
    cJSON_IsArray(array);
    
    // Test cJSON_GetArraySize
    int array_size = cJSON_GetArraySize(array);
    
    // Test cJSON_GetArrayItem with various indices
    if (array_size > 0) {
        for (int i = 0; i < array_size; i++) {
            cJSON_GetArrayItem(array, i);
        }
        
        // Test out-of-bounds access
        cJSON_GetArrayItem(array, -1);
        cJSON_GetArrayItem(array, array_size);
        cJSON_GetArrayItem(array, array_size + 1);
    }
    
    // Test cJSON_InsertItemInArray
    if (size > 0) {
        cJSON* new_item = cJSON_CreateNumber(data[0] % 100);
        if (new_item) {
            int insert_index = data[0] % (array_size + 2);  // Allow beyond array length
            cJSON_InsertItemInArray(array, insert_index, new_item);
            // Note: new_item is now owned by array, don't delete separately
        }
    }
}

int LLVMFuzzerTestOneInput_40(const uint8_t* data, size_t size) {
    // Test with NULL inputs
    cJSON_IsArray(NULL);
    cJSON_GetArrayItem(NULL, 0);
    cJSON_GetArraySize(NULL);
    cJSON_InsertItemInArray(NULL, 0, NULL);
    
    // Create and test double array
    cJSON* double_array = create_random_array(data, size, 1);
    test_array_operations(double_array, data, size);
    cJSON_Delete(double_array);
    
    // Create and test int array
    cJSON* int_array = create_random_array(data, size, 0);
    test_array_operations(int_array, data, size);
    cJSON_Delete(int_array);
    
    // Test cJSON_IsArray with non-array types
    cJSON* number = cJSON_CreateNumber(42.0);
    cJSON_IsArray(number);
    cJSON_Delete(number);
    
    cJSON* string = cJSON_CreateString("test");
    cJSON_IsArray(string);
    cJSON_Delete(string);
    
    cJSON* object = cJSON_CreateObject();
    cJSON_IsArray(object);
    cJSON_Delete(object);
    
    // Test cJSON_CreateDoubleArray with edge cases
    cJSON_CreateDoubleArray(NULL, 0);
    cJSON_CreateDoubleArray(NULL, 5);
    
    double empty_double[] = {};
    cJSON_CreateDoubleArray(empty_double, 0);
    
    // Test cJSON_CreateIntArray with edge cases
    cJSON_CreateIntArray(NULL, 0);
    cJSON_CreateIntArray(NULL, 5);
    
    int empty_int[] = {};
    cJSON_CreateIntArray(empty_int, 0);
    
    // Test with negative count
    double some_doubles[] = {1.0, 2.0, 3.0};
    cJSON_CreateDoubleArray(some_doubles, -1);
    
    int some_ints[] = {1, 2, 3};
    cJSON_CreateIntArray(some_ints, -1);
    
    return 0;
}