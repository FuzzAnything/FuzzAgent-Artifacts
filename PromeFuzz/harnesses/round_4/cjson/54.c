// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
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

static void fuzz_cJSON_GetArraySize(const cJSON *array) {
    // Test with valid array and NULL input
    int size = cJSON_GetArraySize(array);
    (void)size; // Use result to avoid unused variable warning
}

static void fuzz_cJSON_CreateIntArray(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    // Use first 4 bytes to determine array count
    int count = *(const int*)data;
    if (count < 0) count = 0;
    if (count > 1000) count = 1000; // Limit to avoid excessive memory usage
    
    // Create integer array from remaining data
    size_t max_elements = (size - sizeof(int)) / sizeof(int);
    if (max_elements < (size_t)count) return;
    
    const int *numbers = (const int*)(data + sizeof(int));
    cJSON *array = cJSON_CreateIntArray(numbers, count);
    if (array) {
        // Test other array functions on the created array
        fuzz_cJSON_GetArraySize(array);
        cJSON_IsArray(array);
        
        if (count > 0) {
            // Test getting array items
            for (int i = 0; i < count && i < 10; i++) {
                cJSON_GetArrayItem(array, i);
            }
        }
        
        cJSON_Delete(array);
    }
}

static void fuzz_cJSON_IsArray(const cJSON *item) {
    cJSON_bool is_array = cJSON_IsArray(item);
    (void)is_array; // Use result to avoid unused variable warning
}

static void fuzz_cJSON_CreateDoubleArray(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    // Use first 4 bytes to determine array count
    int count = *(const int*)data;
    if (count < 0) count = 0;
    if (count > 1000) count = 1000; // Limit to avoid excessive memory usage
    
    // Create double array from remaining data
    size_t max_elements = (size - sizeof(int)) / sizeof(double);
    if (max_elements < (size_t)count) return;
    
    const double *numbers = (const double*)(data + sizeof(int));
    cJSON *array = cJSON_CreateDoubleArray(numbers, count);
    if (array) {
        // Test other array functions on the created array
        fuzz_cJSON_GetArraySize(array);
        cJSON_IsArray(array);
        
        if (count > 0) {
            // Test getting array items
            for (int i = 0; i < count && i < 10; i++) {
                cJSON_GetArrayItem(array, i);
            }
        }
        
        cJSON_Delete(array);
    }
}

static void fuzz_cJSON_GetArrayItem(const cJSON *array, const uint8_t *data, size_t size) {
    if (!array) return;
    
    int array_size = cJSON_GetArraySize(array);
    if (array_size <= 0) return;
    
    // Use data to generate indices
    for (size_t i = 0; i < size && i < 10; i++) {
        int index = data[i] % (array_size + 5); // Include out-of-bounds indices
        cJSON *item = cJSON_GetArrayItem(array, index);
        (void)item; // Use result to avoid unused variable warning
    }
}

static void fuzz_cJSON_InsertItemInArray(cJSON *array, const uint8_t *data, size_t size) {
    if (!array) return;
    
    int array_size = cJSON_GetArraySize(array);
    
    // Create some items to insert
    cJSON *items[5];
    for (int i = 0; i < 5; i++) {
        items[i] = cJSON_CreateNumber(i);
    }
    
    // Use data to generate insertion indices
    for (size_t i = 0; i < size && i < 10; i++) {
        int which = data[i] % (array_size + 10); // Include out-of-bounds indices
        int item_idx = (i < 5) ? i : 0;
        cJSON_InsertItemInArray(array, which, items[item_idx]);
        
        // Update array size after insertion
        array_size = cJSON_GetArraySize(array);
    }
    
    // Cleanup - note: inserted items are now owned by the array
    for (int i = 0; i < 5; i++) {
        if (items[i] && items[i]->child == NULL && items[i]->next == NULL) {
            // Item wasn't inserted, delete it
            cJSON_Delete(items[i]);
        }
    }
}

static cJSON *parse_json_from_data(const uint8_t *data, size_t size) {
    // Create a null-terminated string from the data
    char *json_str = malloc(size + 1);
    if (!json_str) return NULL;
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *parsed = cJSON_Parse(json_str);
    free(json_str);
    return parsed;
}

int LLVMFuzzerTestOneInput_54(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    // Seed random number generator with data for deterministic behavior
    srand(*(unsigned int*)data);
    
    // Test 1: Parse JSON and test array functions on the result
    cJSON *parsed = parse_json_from_data(data, size);
    if (parsed) {
        fuzz_cJSON_GetArraySize(parsed);
        fuzz_cJSON_IsArray(parsed);
        fuzz_cJSON_GetArrayItem(parsed, data, size);
        fuzz_cJSON_InsertItemInArray(parsed, data, size);
        cJSON_Delete(parsed);
    }
    
    // Test 2: Test with NULL input
    fuzz_cJSON_GetArraySize(NULL);
    fuzz_cJSON_IsArray(NULL);
    cJSON_GetArrayItem(NULL, 0);
    cJSON_InsertItemInArray(NULL, 0, NULL);
    
    // Test 3: Create arrays from raw data
    fuzz_cJSON_CreateIntArray(data, size);
    fuzz_cJSON_CreateDoubleArray(data, size);
    
    // Test 4: Create various array types and test functions on them
    cJSON *simple_array = cJSON_CreateArray();
    if (simple_array) {
        // Add some items to the array
        for (int i = 0; i < 5; i++) {
            cJSON *item = cJSON_CreateNumber(i);
            cJSON_AddItemToArray(simple_array, item);
        }
        
        fuzz_cJSON_GetArraySize(simple_array);
        fuzz_cJSON_IsArray(simple_array);
        fuzz_cJSON_GetArrayItem(simple_array, data, size);
        fuzz_cJSON_InsertItemInArray(simple_array, data, size);
        
        cJSON_Delete(simple_array);
    }
    
    // Test 5: Test with empty array
    cJSON *empty_array = cJSON_CreateArray();
    if (empty_array) {
        fuzz_cJSON_GetArraySize(empty_array);
        fuzz_cJSON_IsArray(empty_array);
        fuzz_cJSON_GetArrayItem(empty_array, data, size);
        fuzz_cJSON_InsertItemInArray(empty_array, data, size);
        cJSON_Delete(empty_array);
    }
    
    return 0;
}