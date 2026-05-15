// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON* create_dummy_array(size_t size) {
    cJSON* array = cJSON_CreateArray();
    if (!array) return NULL;
    
    for (size_t i = 0; i < size; i++) {
        cJSON* item = cJSON_CreateNumber((double)i);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
    return array;
}

static cJSON* create_dummy_object(const char* key) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    cJSON_AddNumberToObject(obj, key, 42.0);
    return obj;
}

static float* create_float_array_from_data(const uint8_t* data, size_t size, int* count) {
    if (size < sizeof(float)) {
        *count = 0;
        return NULL;
    }
    
    *count = (int)(size / sizeof(float));
    if (*count > 100) *count = 100; // Limit to avoid excessive memory
    
    float* float_array = (float*)malloc(*count * sizeof(float));
    if (!float_array) {
        *count = 0;
        return NULL;
    }
    
    for (int i = 0; i < *count; i++) {
        float value;
        memcpy(&value, data + i * sizeof(float), sizeof(float));
        float_array[i] = value;
    }
    
    return float_array;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create test arrays
    size_t array_size = (size_t)(Data[0] % 10) + 1;
    cJSON* test_array = create_dummy_array(array_size);
    if (!test_array) return 0;
    
    // Test cJSON_GetArraySize
    int array_size_result = cJSON_GetArraySize(test_array);
    
    // Test cJSON_GetArrayItem with various indices
    for (int i = -2; i < array_size_result + 2; i++) {
        cJSON* item = cJSON_GetArrayItem(test_array, i);
        (void)item; // Use result to avoid unused variable warning
    }
    
    // Test cJSON_CreateFloatArray
    int float_count = 0;
    float* float_array = create_float_array_from_data(Data, Size, &float_count);
    if (float_array && float_count > 0) {
        cJSON* float_json_array = cJSON_CreateFloatArray(float_array, float_count);
        if (float_json_array) {
            // Test operations on the float array
            int float_array_size = cJSON_GetArraySize(float_json_array);
            for (int i = 0; i < float_array_size && i < 5; i++) {
                cJSON_GetArrayItem(float_json_array, i);
            }
            cJSON_Delete(float_json_array);
        }
        free(float_array);
    }
    
    // Test cJSON_ReplaceItemInArray
    if (array_size_result > 0) {
        int replace_index = (int)(Data[0] % (array_size_result + 3)) - 1; // Include invalid indices
        cJSON* new_item = cJSON_CreateString("replacement");
        if (new_item) {
            cJSON_bool replace_result = cJSON_ReplaceItemInArray(test_array, replace_index, new_item);
            (void)replace_result;
        }
    }
    
    // Test cJSON_ReplaceItemViaPointer
    if (array_size_result > 0) {
        int pointer_index = (int)(Data[0] % array_size_result);
        cJSON* old_item = cJSON_GetArrayItem(test_array, pointer_index);
        if (old_item) {
            cJSON* replacement = cJSON_CreateBool(cJSON_True);
            if (replacement) {
                cJSON_bool via_pointer_result = cJSON_ReplaceItemViaPointer(test_array, old_item, replacement);
                (void)via_pointer_result;
            }
        }
    }
    
    // Test cJSON_DeleteItemFromArray
    if (array_size_result > 0) {
        int delete_index = (int)(Data[0] % (array_size_result + 2)) - 1; // Include invalid indices
        cJSON_DeleteItemFromArray(test_array, delete_index);
        
        // Update array size after deletion
        array_size_result = cJSON_GetArraySize(test_array);
    }
    
    // Additional edge case testing
    // Test with NULL array
    cJSON_GetArraySize(NULL);
    cJSON_GetArrayItem(NULL, 0);
    
    // Test with negative count for cJSON_CreateFloatArray
    cJSON* null_float_array = cJSON_CreateFloatArray(NULL, -1);
    (void)null_float_array;
    
    // Test replacement with NULL parameters
    if (array_size_result > 0) {
        cJSON* item = cJSON_GetArrayItem(test_array, 0);
        if (item) {
            cJSON_ReplaceItemViaPointer(test_array, item, NULL);
            cJSON_ReplaceItemViaPointer(NULL, item, item);
            cJSON_ReplaceItemViaPointer(test_array, NULL, item);
        }
    }
    
    // Cleanup
    cJSON_Delete(test_array);
    
    return 0;
}