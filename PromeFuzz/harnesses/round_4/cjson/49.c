// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cJSON.h"

static void fuzz_cJSON_GetArraySize(const cJSON *array) {
    if (array == NULL) {
        /* NULL input should return 0 */
        cJSON_GetArraySize(NULL);
    } else {
        /* Get size of valid array */
        cJSON_GetArraySize(array);
    }
}

static void fuzz_cJSON_CreateIntArray(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    /* Create integer array from fuzzer data */
    int count = (int)(size / sizeof(int));
    if (count > 100) count = 100; /* Limit to reasonable size */
    
    int *numbers = (int*)data;
    
    /* Test with valid parameters */
    cJSON *int_array = cJSON_CreateIntArray(numbers, count);
    if (int_array != NULL) {
        /* Test array operations */
        int array_size = cJSON_GetArraySize(int_array);
        for (int i = 0; i < array_size && i < 10; i++) {
            cJSON_GetArrayItem(int_array, i);
            cJSON_GetArrayItem(int_array, -i); /* Test negative index */
        }
        cJSON_Delete(int_array);
    }
    
    /* Test edge cases */
    cJSON_CreateIntArray(NULL, 0);
    cJSON_CreateIntArray(numbers, 0);
    cJSON_CreateIntArray(NULL, count);
    cJSON_CreateIntArray(numbers, -1); /* Invalid count */
}

static void fuzz_cJSON_CreateFloatArray(const uint8_t *data, size_t size) {
    if (size < sizeof(float)) return;
    
    /* Create float array from fuzzer data */
    int count = (int)(size / sizeof(float));
    if (count > 100) count = 100; /* Limit to reasonable size */
    
    float *numbers = (float*)data;
    
    /* Test with valid parameters */
    cJSON *float_array = cJSON_CreateFloatArray(numbers, count);
    if (float_array != NULL) {
        /* Test array operations */
        int array_size = cJSON_GetArraySize(float_array);
        for (int i = 0; i < array_size && i < 10; i++) {
            cJSON_GetArrayItem(float_array, i);
        }
        cJSON_Delete(float_array);
    }
    
    /* Test edge cases */
    cJSON_CreateFloatArray(NULL, 0);
    cJSON_CreateFloatArray(numbers, 0);
    cJSON_CreateFloatArray(NULL, count);
    cJSON_CreateFloatArray(numbers, -1); /* Invalid count */
}

static void fuzz_cJSON_GetArrayItem(const cJSON *array) {
    if (array == NULL) {
        cJSON_GetArrayItem(NULL, 0);
        return;
    }
    
    int size = cJSON_GetArraySize(array);
    
    /* Test valid indices */
    for (int i = 0; i < size && i < 20; i++) {
        cJSON_GetArrayItem(array, i);
    }
    
    /* Test boundary and invalid indices */
    cJSON_GetArrayItem(array, -1);
    cJSON_GetArrayItem(array, size);
    cJSON_GetArrayItem(array, size + 1);
    cJSON_GetArrayItem(array, -size);
}

static void fuzz_cJSON_ReplaceItemInArray(cJSON *array, const uint8_t *data, size_t size) {
    if (array == NULL) {
        cJSON_ReplaceItemInArray(NULL, 0, NULL);
        return;
    }
    
    int array_size = cJSON_GetArraySize(array);
    
    /* Create replacement items from fuzzer data */
    cJSON *replacement = NULL;
    if (size > 0) {
        /* Use first byte to determine replacement type */
        uint8_t type = data[0] % 4;
        switch (type) {
            case 0:
                replacement = cJSON_CreateNull();
                break;
            case 1:
                replacement = cJSON_CreateTrue();
                break;
            case 2:
                replacement = cJSON_CreateFalse();
                break;
            case 3:
                replacement = cJSON_CreateNumber((double)(data[0]));
                break;
        }
    }
    
    /* Test various indices */
    for (int i = -2; i < array_size + 2; i++) {
        cJSON_ReplaceItemInArray(array, i, replacement);
    }
    
    /* Test with NULL replacement */
    for (int i = -2; i < array_size + 2; i++) {
        cJSON_ReplaceItemInArray(array, i, NULL);
    }
    
    if (replacement != NULL) {
        cJSON_Delete(replacement);
    }
}

static void fuzz_cJSON_CreateArray(void) {
    cJSON *array = cJSON_CreateArray();
    if (array != NULL) {
        /* Test operations on empty array */
        fuzz_cJSON_GetArraySize(array);
        fuzz_cJSON_GetArrayItem(array);
        
        /* Add some items to test replacement */
        for (int i = 0; i < 5; i++) {
            cJSON *item = cJSON_CreateNumber(i);
            if (item != NULL) {
                cJSON_AddItemToArray(array, item);
            }
        }
        
        /* Test replacement on populated array */
        uint8_t dummy_data[] = {1, 2, 3};
        fuzz_cJSON_ReplaceItemInArray(array, dummy_data, sizeof(dummy_data));
        
        cJSON_Delete(array);
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *data, size_t size) {
    if (size == 0) {
        /* Test with empty input */
        fuzz_cJSON_GetArraySize(NULL);
        fuzz_cJSON_CreateArray();
        return 0;
    }
    
    /* Use first byte to determine which functions to test */
    uint8_t selector = data[0];
    data++;
    size--;
    
    /* Test cJSON_CreateIntArray */
    if ((selector & 0x01) && size >= sizeof(int)) {
        fuzz_cJSON_CreateIntArray(data, size);
    }
    
    /* Test cJSON_CreateFloatArray */
    if ((selector & 0x02) && size >= sizeof(float)) {
        fuzz_cJSON_CreateFloatArray(data, size);
    }
    
    /* Test cJSON_CreateArray and related functions */
    if (selector & 0x04) {
        fuzz_cJSON_CreateArray();
    }
    
    /* Create a test array for GetArraySize and GetArrayItem */
    cJSON *test_array = cJSON_CreateArray();
    if (test_array != NULL) {
        /* Add some items to the array */
        for (int i = 0; i < 10 && i < (int)(size / 2); i++) {
            cJSON *item = cJSON_CreateNumber(data[i % size]);
            if (item != NULL) {
                cJSON_AddItemToArray(test_array, item);
            }
        }
        
        /* Test GetArraySize */
        if (selector & 0x08) {
            fuzz_cJSON_GetArraySize(test_array);
            fuzz_cJSON_GetArraySize(NULL);
        }
        
        /* Test GetArrayItem */
        if (selector & 0x10) {
            fuzz_cJSON_GetArrayItem(test_array);
            fuzz_cJSON_GetArrayItem(NULL);
        }
        
        /* Test ReplaceItemInArray */
        if (selector & 0x20) {
            fuzz_cJSON_ReplaceItemInArray(test_array, data, size);
            fuzz_cJSON_ReplaceItemInArray(NULL, data, size);
        }
        
        cJSON_Delete(test_array);
    }
    
    return 0;
}