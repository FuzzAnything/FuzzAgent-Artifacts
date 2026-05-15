// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
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

static void fuzz_cjson_array_functions(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    /* Create test integer array from fuzzer data */
    int *int_array = malloc(sizeof(int) * (size - 1));
    if (!int_array) return;
    
    for (size_t i = 0; i < size - 1; i++) {
        int_array[i] = (int)data[i];
    }
    
    /* Create cJSON array using the integer array */
    int count = (int)(size - 1);
    if (count < 0) {
        free(int_array);
        return;
    }
    
    cJSON *array = cJSON_CreateIntArray(int_array, count);
    free(int_array);
    
    if (!array) return;
    
    /* Test cJSON_IsArray */
    cJSON_bool is_array = cJSON_IsArray(array);
    (void)is_array; /* Use result to avoid unused variable warning */
    
    /* Test cJSON_GetArraySize */
    int array_size = cJSON_GetArraySize(array);
    
    /* Test cJSON_GetArrayItem with various indices */
    if (array_size > 0) {
        /* Use fuzzer data to generate indices */
        for (size_t i = 0; i < size && i < 10; i++) {
            int idx = (int)(data[i] % (array_size + 5)); /* Include out-of-bounds cases */
            cJSON *item = cJSON_GetArrayItem(array, idx);
            (void)item; /* Use result */
        }
    }
    
    /* Test cJSON_ReplaceItemInArray */
    if (array_size > 0) {
        /* Create new items for replacement */
        cJSON *new_item = cJSON_CreateNumber((double)(data[0] % 256));
        
        /* Try to replace at various positions */
        for (size_t i = 0; i < size && i < 5; i++) {
            int idx = (int)(data[i] % (array_size + 10)); /* Include invalid indices */
            cJSON_bool replace_result = cJSON_ReplaceItemInArray(array, idx, new_item);
            (void)replace_result;
            
            /* If replacement succeeded, create a new item for next iteration */
            if (replace_result) {
                /* The old new_item is now part of the array, so create a new one */
                new_item = cJSON_CreateNumber((double)(data[(i + 1) % size] % 256));
            }
        }
        
        /* Clean up the last created item if it wasn't used */
        if (new_item) {
            cJSON_Delete(new_item);
        }
    }
    
    /* Test cJSON_InsertItemInArray */
    if (array_size > 0) {
        /* Create items for insertion */
        cJSON *insert_item = cJSON_CreateString("fuzz_insert");
        
        /* Try to insert at various positions */
        for (size_t i = 0; i < size && i < 5; i++) {
            int idx = (int)(data[i] % (array_size * 2 + 5)); /* Include beyond array length */
            cJSON_bool insert_result = cJSON_InsertItemInArray(array, idx, insert_item);
            (void)insert_result;
            
            /* If insertion succeeded, update array size and create new item */
            if (insert_result) {
                array_size = cJSON_GetArraySize(array);
                /* The old insert_item is now part of the array, so create a new one */
                insert_item = cJSON_CreateNumber((double)(data[(i + 2) % size] % 256));
            }
        }
        
        /* Clean up the last created item if it wasn't used */
        if (insert_item) {
            cJSON_Delete(insert_item);
        }
    }
    
    /* Test with non-array items */
    cJSON *non_array = cJSON_CreateObject();
    if (non_array) {
        cJSON_bool non_array_result = cJSON_IsArray(non_array);
        (void)non_array_result;
        
        int non_array_size = cJSON_GetArraySize(non_array);
        (void)non_array_size;
        
        cJSON_Delete(non_array);
    }
    
    /* Test with NULL input */
    cJSON_IsArray(NULL);
    cJSON_GetArraySize(NULL);
    cJSON_GetArrayItem(NULL, 0);
    
    /* Clean up */
    cJSON_Delete(array);
}

int LLVMFuzzerTestOneInput_43(const uint8_t *data, size_t size) {
    fuzz_cjson_array_functions(data, size);
    return 0;
}