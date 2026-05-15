// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
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

static cJSON* create_test_array_from_data(const uint8_t *data, size_t size) {
    cJSON *array = cJSON_CreateArray();
    if (!array) return NULL;
    
    size_t num_items = size > 0 ? data[0] % 10 : 0;
    for (size_t i = 0; i < num_items && i < size; i++) {
        cJSON *item = cJSON_CreateNumber(data[i % size]);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
    return array;
}

static cJSON* create_test_object_from_data(const uint8_t *data, size_t size) {
    cJSON *obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    size_t num_items = size > 0 ? data[0] % 5 : 0;
    for (size_t i = 0; i < num_items && i < size; i++) {
        char key[16];
        snprintf(key, sizeof(key), "key%zu", i);
        cJSON *item = cJSON_CreateNumber(data[i % size]);
        if (item) {
            cJSON_AddItemToObject(obj, key, item);
        }
    }
    return obj;
}

static float* create_float_array_from_data(const uint8_t *data, size_t size, int *count) {
    if (size == 0) {
        *count = 0;
        return NULL;
    }
    
    *count = data[0] % 20;
    if (*count <= 0) {
        return NULL;
    }
    
    float *float_array = (float*)malloc(*count * sizeof(float));
    if (!float_array) {
        *count = 0;
        return NULL;
    }
    
    for (int i = 0; i < *count && i < (int)size; i++) {
        float_array[i] = (float)data[i % size];
    }
    return float_array;
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    uint8_t selector = Data[0];
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    switch (selector % 6) {
        case 0: {  // Test cJSON_GetArraySize
            cJSON *array = create_test_array_from_data(remaining_data, remaining_size);
            if (array) {
                int array_size = cJSON_GetArraySize(array);
                (void)array_size;  // Use result to avoid optimization
                cJSON_Delete(array);
            }
            
            cJSON *obj = create_test_object_from_data(remaining_data, remaining_size);
            if (obj) {
                int obj_size = cJSON_GetArraySize(obj);
                (void)obj_size;  // Use result to avoid optimization
                cJSON_Delete(obj);
            }
            
            // Test with NULL
            cJSON_GetArraySize(NULL);
            break;
        }
        
        case 1: {  // Test cJSON_DeleteItemFromArray
            cJSON *array = create_test_array_from_data(remaining_data, remaining_size);
            if (array) {
                int array_size = cJSON_GetArraySize(array);
                if (array_size > 0) {
                    int index_to_delete = remaining_size > 0 ? remaining_data[0] % array_size : 0;
                    cJSON_DeleteItemFromArray(array, index_to_delete);
                }
                cJSON_Delete(array);
            }
            break;
        }
        
        case 2: {  // Test cJSON_CreateFloatArray
            int count = 0;
            float *float_array = create_float_array_from_data(remaining_data, remaining_size, &count);
            cJSON *json_array = cJSON_CreateFloatArray(float_array, count);
            if (json_array) {
                cJSON_Delete(json_array);
            }
            free(float_array);
            
            // Test edge cases
            cJSON_CreateFloatArray(NULL, 0);
            cJSON_CreateFloatArray(NULL, -1);
            cJSON_CreateFloatArray(float_array, -1);
            break;
        }
        
        case 3: {  // Test cJSON_AddItemToArray
            cJSON *array = cJSON_CreateArray();
            if (array) {
                cJSON *item = cJSON_CreateNumber(42.0);
                if (item) {
                    cJSON_AddItemToArray(array, item);
                }
                
                // Add more items based on data
                size_t num_items = remaining_size > 0 ? remaining_data[0] % 10 : 0;
                for (size_t i = 0; i < num_items && i < remaining_size; i++) {
                    cJSON *new_item = cJSON_CreateNumber(remaining_data[i]);
                    if (new_item) {
                        cJSON_AddItemToArray(array, new_item);
                    }
                }
                cJSON_Delete(array);
            }
            break;
        }
        
        case 4: {  // Test cJSON_ReplaceItemInArray
            cJSON *array = create_test_array_from_data(remaining_data, remaining_size);
            if (array) {
                int array_size = cJSON_GetArraySize(array);
                if (array_size > 0) {
                    int index_to_replace = remaining_size > 0 ? remaining_data[0] % array_size : 0;
                    cJSON *new_item = cJSON_CreateString("replacement");
                    if (new_item) {
                        cJSON_ReplaceItemInArray(array, index_to_replace, new_item);
                    }
                }
                cJSON_Delete(array);
            }
            break;
        }
        
        case 5: {  // Test cJSON_ReplaceItemViaPointer
            cJSON *parent = create_test_array_from_data(remaining_data, remaining_size);
            if (parent && parent->child) {
                cJSON *item_to_replace = parent->child;
                cJSON *replacement = cJSON_CreateBool(1);
                if (replacement) {
                    cJSON_ReplaceItemViaPointer(parent, item_to_replace, replacement);
                }
                cJSON_Delete(parent);
            } else if (parent) {
                cJSON_Delete(parent);
            }
            
            // Test with object
            cJSON *obj = cJSON_CreateObject();
            if (obj) {
                cJSON_AddItemToObject(obj, "key1", cJSON_CreateNumber(1));
                cJSON_AddItemToObject(obj, "key2", cJSON_CreateNumber(2));
                
                cJSON *item = cJSON_GetObjectItem(obj, "key1");
                if (item) {
                    cJSON *replacement = cJSON_CreateString("replaced");
                    if (replacement) {
                        cJSON_ReplaceItemViaPointer(obj, item, replacement);
                    }
                }
                cJSON_Delete(obj);
            }
            break;
        }
    }
    
    return 0;
}