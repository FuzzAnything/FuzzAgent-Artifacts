// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_cJSON_GetArraySize(const uint8_t *data, size_t size) {
    // Test with NULL input
    cJSON_GetArraySize(NULL);
    
    // Test with valid array created from data
    if (size > 0) {
        cJSON *json = cJSON_ParseWithLength((const char *)data, size);
        if (json) {
            cJSON_GetArraySize(json);
            cJSON_Delete(json);
        }
    }
}

static void fuzz_cJSON_CreateFloatArray(const uint8_t *data, size_t size) {
    // Test with NULL numbers pointer
    cJSON_CreateFloatArray(NULL, 0);
    cJSON_CreateFloatArray(NULL, 5);
    
    // Test with negative count
    if (size >= sizeof(float)) {
        float *numbers = (float *)data;
        cJSON_CreateFloatArray(numbers, -1);
    }
    
    // Test with valid float array
    if (size >= sizeof(float) * 10) {
        float *numbers = (float *)data;
        cJSON *array = cJSON_CreateFloatArray(numbers, 10);
        if (array) {
            cJSON_Delete(array);
        }
    }
    
    // Test with partial data
    if (size > 0 && size < sizeof(float) * 10) {
        int count = size / sizeof(float);
        if (count > 0) {
            float *numbers = (float *)data;
            cJSON *array = cJSON_CreateFloatArray(numbers, count);
            if (array) {
                cJSON_Delete(array);
            }
        }
    }
}

static void fuzz_cJSON_ReplaceItemInArray(const uint8_t *data, size_t size) {
    // Test with NULL array
    cJSON_ReplaceItemInArray(NULL, 0, NULL);
    
    // Create test array
    cJSON *array = cJSON_CreateArray();
    if (!array) return;
    
    // Add some initial items
    for (int i = 0; i < 5; i++) {
        cJSON *item = cJSON_CreateNumber(i);
        cJSON_AddItemToArray(array, item);
    }
    
    // Test with various indices
    if (size > 0) {
        // Use first byte to determine index
        int which = data[0] % 10;  // 0-9
        
        // Create replacement item
        cJSON *newitem = cJSON_CreateString("replacement");
        if (newitem) {
            cJSON_ReplaceItemInArray(array, which, newitem);
        }
        
        // Test with negative index
        cJSON_ReplaceItemInArray(array, -1, cJSON_CreateNull());
        
        // Test with large index
        cJSON_ReplaceItemInArray(array, 100, cJSON_CreateNull());
        
        // Test with NULL newitem
        cJSON_ReplaceItemInArray(array, 0, NULL);
    }
    
    cJSON_Delete(array);
}

static void fuzz_cJSON_ReplaceItemViaPointer(const uint8_t *data, size_t size) {
    // Test with NULL inputs
    cJSON_ReplaceItemViaPointer(NULL, NULL, NULL);
    
    // Create parent array
    cJSON *parent = cJSON_CreateArray();
    if (!parent) return;
    
    // Add initial items
    for (int i = 0; i < 3; i++) {
        cJSON *item = cJSON_CreateNumber(i * 10);
        cJSON_AddItemToArray(parent, item);
    }
    
    // Get first child item
    cJSON *item = parent->child;
    
    if (item) {
        // Create replacement
        cJSON *replacement = cJSON_CreateString("replaced");
        if (replacement) {
            cJSON_ReplaceItemViaPointer(parent, item, replacement);
        }
        
        // Test with NULL parent
        cJSON_ReplaceItemViaPointer(NULL, item, cJSON_CreateNull());
        
        // Test with NULL item
        cJSON_ReplaceItemViaPointer(parent, NULL, cJSON_CreateNull());
        
        // Test with NULL replacement
        cJSON_ReplaceItemViaPointer(parent, parent->child, NULL);
    }
    
    cJSON_Delete(parent);
}

static void fuzz_cJSON_AddItemToArray(const uint8_t *data, size_t size) {
    // Test with NULL array
    cJSON_AddItemToArray(NULL, NULL);
    
    // Create array
    cJSON *array = cJSON_CreateArray();
    if (!array) return;
    
    // Add various items
    cJSON_AddItemToArray(array, cJSON_CreateNull());
    cJSON_AddItemToArray(array, cJSON_CreateTrue());
    cJSON_AddItemToArray(array, cJSON_CreateFalse());
    cJSON_AddItemToArray(array, cJSON_CreateBool(1));
    
    // Add number from data
    if (size >= sizeof(double)) {
        double value;
        memcpy(&value, data, sizeof(double));
        cJSON_AddItemToArray(array, cJSON_CreateNumber(value));
    }
    
    // Add string from data
    if (size > 0) {
        char *str = (char *)malloc(size + 1);
        if (str) {
            memcpy(str, data, size);
            str[size] = '\0';
            cJSON_AddItemToArray(array, cJSON_CreateString(str));
            free(str);
        }
    }
    
    // Test with NULL item
    cJSON_AddItemToArray(array, NULL);
    
    cJSON_Delete(array);
}

static void fuzz_cJSON_DeleteItemFromArray(const uint8_t *data, size_t size) {
    // Test with NULL array (should handle gracefully)
    cJSON_DeleteItemFromArray(NULL, 0);
    
    // Create array with items
    cJSON *array = cJSON_CreateArray();
    if (!array) return;
    
    // Add some items
    for (int i = 0; i < 10; i++) {
        cJSON *item = cJSON_CreateNumber(i);
        cJSON_AddItemToArray(array, item);
    }
    
    // Delete items at various indices
    if (size > 0) {
        // Use first few bytes to determine indices
        for (size_t i = 0; i < size && i < 5; i++) {
            int which = data[i] % 20;  // 0-19
            cJSON_DeleteItemFromArray(array, which);
        }
    }
    
    // Delete from empty array
    cJSON_DeleteItemFromArray(array, 0);
    
    // Delete with negative index
    cJSON_DeleteItemFromArray(array, -1);
    
    // Delete with large index
    cJSON_DeleteItemFromArray(array, 100);
    
    cJSON_Delete(array);
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    // Initialize cJSON if needed
    // Note: cJSON doesn't require explicit initialization
    
    // Fuzz each API function
    fuzz_cJSON_GetArraySize(Data, Size);
    fuzz_cJSON_CreateFloatArray(Data, Size);
    fuzz_cJSON_ReplaceItemInArray(Data, Size);
    fuzz_cJSON_ReplaceItemViaPointer(Data, Size);
    fuzz_cJSON_AddItemToArray(Data, Size);
    fuzz_cJSON_DeleteItemFromArray(Data, Size);
    
    return 0;
}