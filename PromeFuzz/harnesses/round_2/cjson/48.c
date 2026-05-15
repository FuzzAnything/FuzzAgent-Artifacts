// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
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

static void fuzz_cJSON_GetArraySize(const uint8_t *data, size_t size) {
    // Test with NULL input
    cJSON_GetArraySize(NULL);
    
    // Test with valid JSON array
    if (size > 0) {
        char *json_string = malloc(size + 1);
        if (json_string) {
            memcpy(json_string, data, size);
            json_string[size] = '\0';
            
            cJSON *parsed = cJSON_Parse(json_string);
            if (parsed) {
                cJSON_GetArraySize(parsed);
                cJSON_Delete(parsed);
            }
            free(json_string);
        }
    }
}

static void fuzz_cJSON_CreateFloatArray(const uint8_t *data, size_t size) {
    // Test with NULL numbers or negative count
    cJSON_CreateFloatArray(NULL, -1);
    cJSON_CreateFloatArray(NULL, 0);
    
    // Test with valid float array
    if (size >= sizeof(float)) {
        size_t max_floats = size / sizeof(float);
        if (max_floats > 0) {
            float *float_array = (float *)data;
            int count = max_floats % 100; // Limit to reasonable size
            
            cJSON *array = cJSON_CreateFloatArray(float_array, count);
            if (array) {
                cJSON_Delete(array);
            }
        }
    }
}

static void fuzz_cJSON_ReplaceItemInArray(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Create a base array
    cJSON *array = cJSON_CreateArray();
    if (!array) return;
    
    // Add some items to the array
    for (int i = 0; i < 5 && i < (int)size; i++) {
        cJSON *item = cJSON_CreateNumber(data[i]);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
    
    // Create replacement item
    cJSON *newitem = cJSON_CreateString("replacement");
    
    // Test replacement at various indices
    int array_size = cJSON_GetArraySize(array);
    if (array_size > 0) {
        // Test valid index
        int valid_index = data[0] % array_size;
        cJSON_ReplaceItemInArray(array, valid_index, newitem);
        
        // Test invalid indices
        cJSON_ReplaceItemInArray(array, -1, cJSON_CreateNull());
        cJSON_ReplaceItemInArray(array, array_size + 1, cJSON_CreateNull());
        
        // Don't delete newitem if it was successfully added to array
        // (array takes ownership)
        if (cJSON_ReplaceItemInArray(array, valid_index, cJSON_CreateNumber(42))) {
            // newitem was replaced, need to delete it
            cJSON_Delete(newitem);
        }
    } else {
        cJSON_Delete(newitem);
    }
    
    cJSON_Delete(array);
}

static void fuzz_cJSON_ReplaceItemViaPointer(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Create parent array
    cJSON *parent = cJSON_CreateArray();
    if (!parent) return;
    
    // Add items to the array
    cJSON *items[3];
    for (int i = 0; i < 3 && i < (int)size; i++) {
        items[i] = cJSON_CreateNumber(data[i]);
        if (items[i]) {
            cJSON_AddItemToArray(parent, items[i]);
        }
    }
    
    // Create replacement
    cJSON *replacement = cJSON_CreateString("test");
    
    // Test replacement with various pointers
    if (cJSON_GetArraySize(parent) > 0) {
        // Test with valid item pointer
        cJSON_ReplaceItemViaPointer(parent, items[0], replacement);
        
        // Test with NULL pointers
        cJSON_ReplaceItemViaPointer(NULL, items[1], cJSON_CreateNull());
        cJSON_ReplaceItemViaPointer(parent, NULL, cJSON_CreateNull());
        cJSON_ReplaceItemViaPointer(parent, items[2], NULL);
        
        // Test with item not in parent
        cJSON *external_item = cJSON_CreateNumber(999);
        cJSON_ReplaceItemViaPointer(parent, external_item, cJSON_CreateNull());
        cJSON_Delete(external_item);
    } else {
        cJSON_Delete(replacement);
    }
    
    cJSON_Delete(parent);
}

static void fuzz_cJSON_AddItemToArray(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Create array
    cJSON *array = cJSON_CreateArray();
    if (!array) return;
    
    // Add various types of items
    for (size_t i = 0; i < size && i < 10; i++) {
        cJSON *item = NULL;
        
        switch (data[i] % 5) {
            case 0:
                item = cJSON_CreateNumber(data[i]);
                break;
            case 1:
                item = cJSON_CreateString("string");
                break;
            case 2:
                item = cJSON_CreateBool(data[i] % 2);
                break;
            case 3:
                item = cJSON_CreateNull();
                break;
            case 4:
                item = cJSON_CreateArray();
                break;
        }
        
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
    
    // Test adding to NULL array
    cJSON_AddItemToArray(NULL, cJSON_CreateNull());
    
    // Test adding NULL item
    cJSON_AddItemToArray(array, NULL);
    
    cJSON_Delete(array);
}

static void fuzz_cJSON_DeleteItemFromArray(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Create array with items
    cJSON *array = cJSON_CreateArray();
    if (!array) return;
    
    // Add items
    for (int i = 0; i < 10 && i < (int)size; i++) {
        cJSON *item = cJSON_CreateNumber(data[i]);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
    
    int array_size = cJSON_GetArraySize(array);
    
    // Delete items at various indices
    if (array_size > 0) {
        // Test valid index
        int valid_index = data[0] % array_size;
        cJSON_DeleteItemFromArray(array, valid_index);
        
        // Test edge cases
        cJSON_DeleteItemFromArray(array, 0);
        if (array_size > 1) {
            cJSON_DeleteItemFromArray(array, array_size - 2);
        }
        
        // Test invalid indices (should be handled by caller)
        // Note: The documentation says invalid input may cause undefined behavior
        // We'll still test some cases but be cautious
        if (size > 1) {
            // Use modulo to get potentially out-of-bounds indices
            int test_index = data[1];
            if (cJSON_GetArraySize(array) > 0) {
                test_index = test_index % (cJSON_GetArraySize(array) * 2);
                cJSON_DeleteItemFromArray(array, test_index);
            }
        }
    }
    
    // Test with NULL array
    cJSON_DeleteItemFromArray(NULL, 0);
    
    cJSON_Delete(array);
}

int LLVMFuzzerTestOneInput_48(const uint8_t *data, size_t size) {
    // Initialize cJSON if needed (no initialization required for basic cJSON)
    
    // Call each fuzz function
    fuzz_cJSON_GetArraySize(data, size);
    fuzz_cJSON_CreateFloatArray(data, size);
    fuzz_cJSON_ReplaceItemInArray(data, size);
    fuzz_cJSON_ReplaceItemViaPointer(data, size);
    fuzz_cJSON_AddItemToArray(data, size);
    fuzz_cJSON_DeleteItemFromArray(data, size);
    
    return 0;
}