// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
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
    /* Test with valid array and NULL input */
    cJSON_GetArraySize(array);
    cJSON_GetArraySize(NULL);
}

static void fuzz_cJSON_CreateIntArray(const uint8_t *data, size_t size) {
    /* Derive count from input size, ensuring it's reasonable */
    int count = (size > 0) ? (data[0] % 256) : 0;
    if (count < 0) count = 0;
    
    /* Allocate and initialize integer array */
    int *numbers = NULL;
    if (count > 0) {
        numbers = (int*)malloc(count * sizeof(int));
        if (numbers) {
            for (int i = 0; i < count; i++) {
                numbers[i] = (i < size) ? data[i] : 0;
            }
        }
    }
    
    /* Create cJSON array and clean up */
    cJSON *array = cJSON_CreateIntArray(numbers, count);
    if (array) {
        cJSON_Delete(array);
    }
    
    free(numbers);
    
    /* Test edge cases */
    cJSON_CreateIntArray(NULL, 0);
    cJSON_CreateIntArray(NULL, count);
    if (numbers) {
        cJSON_CreateIntArray(numbers, -1);
    }
}

static void fuzz_cJSON_CreateFloatArray(const uint8_t *data, size_t size) {
    /* Derive count from input size */
    int count = (size > 0) ? (data[0] % 256) : 0;
    if (count < 0) count = 0;
    
    /* Allocate and initialize float array */
    float *numbers = NULL;
    if (count > 0) {
        numbers = (float*)malloc(count * sizeof(float));
        if (numbers) {
            for (int i = 0; i < count; i++) {
                numbers[i] = (i < size) ? (float)data[i] : 0.0f;
            }
        }
    }
    
    /* Create cJSON array and clean up */
    cJSON *array = cJSON_CreateFloatArray(numbers, count);
    if (array) {
        cJSON_Delete(array);
    }
    
    free(numbers);
    
    /* Test edge cases */
    cJSON_CreateFloatArray(NULL, 0);
    cJSON_CreateFloatArray(NULL, count);
    if (numbers) {
        cJSON_CreateFloatArray(numbers, -1);
    }
}

static void fuzz_cJSON_GetArrayItem(const cJSON *array, const uint8_t *data, size_t size) {
    if (!array) {
        cJSON_GetArrayItem(NULL, 0);
        return;
    }
    
    /* Test with various indices derived from input */
    int array_size = cJSON_GetArraySize(array);
    if (array_size > 0) {
        for (int i = 0; i < 5; i++) {
            int index = (i < size) ? (data[i] % (array_size * 2 + 1)) - (array_size / 2) : i;
            cJSON_GetArrayItem(array, index);
        }
    }
    
    /* Test edge cases */
    cJSON_GetArrayItem(array, -1);
    cJSON_GetArrayItem(array, array_size);
    cJSON_GetArrayItem(array, array_size + 1);
}

static void fuzz_cJSON_ReplaceItemInArray(cJSON *array, const uint8_t *data, size_t size) {
    if (!array) {
        cJSON *dummy = cJSON_CreateNumber(42);
        if (dummy) {
            cJSON_ReplaceItemInArray(NULL, 0, dummy);
            cJSON_Delete(dummy);
        }
        return;
    }
    
    int array_size = cJSON_GetArraySize(array);
    
    /* Create replacement items */
    cJSON *replacements[3];
    replacements[0] = cJSON_CreateNumber((size > 0) ? data[0] : 42);
    replacements[1] = cJSON_CreateString("test");
    replacements[2] = cJSON_CreateBool(1);
    
    /* Try replacements at various indices */
    for (int i = 0; i < 3; i++) {
        if (replacements[i]) {
            int index = (i < size) ? (data[i] % (array_size * 2 + 1)) - (array_size / 2) : i;
            cJSON_bool success = cJSON_ReplaceItemInArray(array, index, replacements[i]);
            /* If replacement failed, delete the replacement item */
            if (!success) {
                cJSON_Delete(replacements[i]);
            }
        }
    }
}

static void fuzz_cJSON_CreateArray(void) {
    cJSON *array = cJSON_CreateArray();
    if (array) {
        /* Add some items to make it non-empty */
        for (int i = 0; i < 3; i++) {
            cJSON_AddItemToArray(array, cJSON_CreateNumber(i));
        }
        cJSON_Delete(array);
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *data, size_t size) {
    /* Create test arrays for fuzzing */
    cJSON *int_array = NULL;
    cJSON *float_array = NULL;
    cJSON *empty_array = cJSON_CreateArray();
    cJSON *small_array = cJSON_CreateArray();
    
    /* Populate small_array with some items */
    if (small_array) {
        for (int i = 0; i < 5; i++) {
            cJSON_AddItemToArray(small_array, cJSON_CreateNumber(i));
        }
    }
    
    /* Create integer array for testing */
    if (size > 0) {
        int count = data[0] % 10;
        int *numbers = (int*)malloc(count * sizeof(int));
        if (numbers) {
            for (int i = 0; i < count; i++) {
                numbers[i] = (i < size) ? data[i] : i;
            }
            int_array = cJSON_CreateIntArray(numbers, count);
            free(numbers);
        }
    }
    
    /* Create float array for testing */
    if (size > 1) {
        int count = data[1] % 10;
        float *numbers = (float*)malloc(count * sizeof(float));
        if (numbers) {
            for (int i = 0; i < count; i++) {
                numbers[i] = (i < size) ? (float)data[i] : (float)i;
            }
            float_array = cJSON_CreateFloatArray(numbers, count);
            free(numbers);
        }
    }
    
    /* Fuzz each API function */
    fuzz_cJSON_GetArraySize(int_array);
    fuzz_cJSON_GetArraySize(float_array);
    fuzz_cJSON_GetArraySize(empty_array);
    fuzz_cJSON_GetArraySize(small_array);
    fuzz_cJSON_GetArraySize(NULL);
    
    fuzz_cJSON_CreateIntArray(data, size);
    fuzz_cJSON_CreateFloatArray(data, size);
    
    fuzz_cJSON_GetArrayItem(int_array, data, size);
    fuzz_cJSON_GetArrayItem(float_array, data, size);
    fuzz_cJSON_GetArrayItem(empty_array, data, size);
    fuzz_cJSON_GetArrayItem(small_array, data, size);
    fuzz_cJSON_GetArrayItem(NULL, data, size);
    
    fuzz_cJSON_ReplaceItemInArray(int_array, data, size);
    fuzz_cJSON_ReplaceItemInArray(float_array, data, size);
    fuzz_cJSON_ReplaceItemInArray(empty_array, data, size);
    fuzz_cJSON_ReplaceItemInArray(small_array, data, size);
    fuzz_cJSON_ReplaceItemInArray(NULL, data, size);
    
    fuzz_cJSON_CreateArray();
    
    /* Cleanup */
    if (int_array) cJSON_Delete(int_array);
    if (float_array) cJSON_Delete(float_array);
    if (empty_array) cJSON_Delete(empty_array);
    if (small_array) cJSON_Delete(small_array);
    
    return 0;
}