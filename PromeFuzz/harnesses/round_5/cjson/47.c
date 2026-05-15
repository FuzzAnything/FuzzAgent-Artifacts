// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
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

static void fuzz_cjson_operations(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    /* Create an empty array */
    cJSON *array = cJSON_CreateArray();
    if (!array) return;
    
    /* Add some items to the array using CreateStringArray */
    const char *strings[] = {"test1", "test2", "test3", "test4", "test5"};
    cJSON *string_array = cJSON_CreateStringArray(strings, 5);
    if (string_array) {
        /* Add the string array as an element */
        cJSON_AddItemToArray(array, string_array);
    }
    
    /* Add some individual items */
    for (int i = 0; i < 5 && i < (int)size; i++) {
        cJSON_AddItemToArray(array, cJSON_CreateNumber(data[i]));
    }
    
    /* Get array size */
    int array_size = cJSON_GetArraySize(array);
    
    /* Perform operations based on input data */
    if (array_size > 0) {
        /* Generate index from data, ensuring it's within bounds */
        int idx = data[0] % (array_size + 2);  /* +2 to test out-of-bounds */
        
        /* Test GetArrayItem */
        cJSON *item = cJSON_GetArrayItem(array, idx);
        (void)item;  /* Use result to avoid unused variable warning */
        
        /* Test DetachItemFromArray */
        cJSON *detached = cJSON_DetachItemFromArray(array, idx);
        if (detached) {
            cJSON_Delete(detached);
        }
        
        /* Reset array for DeleteItemFromArray test */
        cJSON_Delete(array);
        array = cJSON_CreateArray();
        if (array) {
            /* Re-add some items */
            for (int i = 0; i < 3 && i < (int)size; i++) {
                cJSON_AddItemToArray(array, cJSON_CreateNumber(data[i]));
            }
            
            array_size = cJSON_GetArraySize(array);
            if (array_size > 0) {
                idx = data[1] % (array_size + 2);
                /* Test DeleteItemFromArray */
                cJSON_DeleteItemFromArray(array, idx);
            }
        }
    }
    
    /* Cleanup */
    if (array) {
        cJSON_Delete(array);
    }
    
    /* Test CreateStringArray with various inputs */
    if (size > 10) {
        /* Create string array from input data chunks */
        const char *input_strings[3];
        char str1[4], str2[4], str3[4];
        
        /* Copy chunks from input data */
        size_t copy_len = size > 12 ? 3 : size/4;
        if (copy_len > 3) copy_len = 3;
        
        memcpy(str1, data, copy_len);
        memcpy(str2, data + copy_len, copy_len);
        memcpy(str3, data + 2*copy_len, copy_len);
        str1[3] = str2[3] = str3[3] = '\0';
        
        input_strings[0] = str1;
        input_strings[1] = str2;
        input_strings[2] = str3;
        
        cJSON *str_array = cJSON_CreateStringArray(input_strings, 3);
        if (str_array) {
            cJSON_Delete(str_array);
        }
    }
}

int LLVMFuzzerTestOneInput_47(const uint8_t *data, size_t size) {
    /* Initialize and perform fuzzing operations */
    fuzz_cjson_operations(data, size);
    
    /* Additional test with empty array */
    cJSON *empty_array = cJSON_CreateArray();
    if (empty_array) {
        /* Test GetArraySize on empty array */
        int empty_size = cJSON_GetArraySize(empty_array);
        (void)empty_size;
        
        /* Test GetArrayItem with invalid index */
        cJSON *null_item = cJSON_GetArrayItem(empty_array, 0);
        (void)null_item;
        
        /* Test DetachItemFromArray with invalid index */
        cJSON *null_detached = cJSON_DetachItemFromArray(empty_array, -1);
        (void)null_detached;
        
        cJSON_Delete(empty_array);
    }
    
    /* Test with NULL array for GetArraySize */
    int null_size = cJSON_GetArraySize(NULL);
    (void)null_size;
    
    return 0;
}