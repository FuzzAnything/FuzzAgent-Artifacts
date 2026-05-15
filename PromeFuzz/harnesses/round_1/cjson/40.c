// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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

static cJSON* create_test_array(void) {
    cJSON* array = cJSON_CreateArray();
    if (!array) return NULL;
    
    for (int i = 0; i < 5; i++) {
        cJSON* item = cJSON_CreateNumber(i);
        if (item) {
            cJSON_AddItemToArray(array, item);
        }
    }
    return array;
}

static cJSON* create_test_object(void) {
    cJSON* obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    cJSON_AddItemToObject(obj, "key1", cJSON_CreateString("value1"));
    cJSON_AddItemToObject(obj, "key2", cJSON_CreateNumber(42));
    cJSON_AddItemToObject(obj, "key3", cJSON_CreateBool(1));
    
    return obj;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to determine operation
    uint8_t op = Data[0] % 6;
    
    // Use second byte for index if needed
    int idx = (Size > 1) ? (Data[1] % 10) : 0;
    
    // Test cJSON_DeleteItemFromArray
    if (op == 0) {
        cJSON* array = create_test_array();
        if (array) {
            int array_size = cJSON_GetArraySize(array);
            if (array_size > 0) {
                int which = idx % array_size;
                cJSON_DeleteItemFromArray(array, which);
            }
            cJSON_Delete(array);
        }
    }
    
    // Test cJSON_ReplaceItemViaPointer
    else if (op == 1) {
        cJSON* parent = create_test_array();
        if (parent) {
            int array_size = cJSON_GetArraySize(parent);
            if (array_size > 0) {
                int which = idx % array_size;
                cJSON* item = cJSON_GetArrayItem(parent, which);
                cJSON* replacement = cJSON_CreateString("replacement");
                if (item && replacement) {
                    cJSON_ReplaceItemViaPointer(parent, item, replacement);
                }
                if (!replacement) {
                    // Cleanup if creation failed
                    cJSON_Delete(replacement);
                }
            }
            cJSON_Delete(parent);
        }
    }
    
    // Test cJSON_DetachItemFromArray
    else if (op == 2) {
        cJSON* array = create_test_array();
        if (array) {
            int array_size = cJSON_GetArraySize(array);
            if (array_size > 0) {
                int which = idx % array_size;
                cJSON* detached = cJSON_DetachItemFromArray(array, which);
                if (detached) {
                    cJSON_Delete(detached);
                }
            }
            cJSON_Delete(array);
        }
    }
    
    // Test cJSON_ReplaceItemInArray
    else if (op == 3) {
        cJSON* array = create_test_array();
        if (array) {
            int array_size = cJSON_GetArraySize(array);
            if (array_size > 0) {
                int which = idx % array_size;
                cJSON* newitem = cJSON_CreateBool(0);
                if (newitem) {
                    cJSON_ReplaceItemInArray(array, which, newitem);
                }
            }
            cJSON_Delete(array);
        }
    }
    
    // Test cJSON_GetArraySize
    else if (op == 4) {
        cJSON* array = create_test_array();
        if (array) {
            int size = cJSON_GetArraySize(array);
            (void)size; // Use variable to avoid unused warning
            cJSON_Delete(array);
        }
        
        cJSON* obj = create_test_object();
        if (obj) {
            int size = cJSON_GetArraySize(obj);
            (void)size;
            cJSON_Delete(obj);
        }
        
        // Test with NULL
        cJSON_GetArraySize(NULL);
    }
    
    // Test cJSON_CreateFloatArray
    else if (op == 5) {
        // Create float array from fuzzer data
        if (Size > 2) {
            int count = (Size - 2) / sizeof(float);
            if (count > 0) {
                float* numbers = (float*)malloc(count * sizeof(float));
                if (numbers) {
                    // Copy data as floats (may be misaligned, but that's OK for fuzzing)
                    memcpy(numbers, Data + 2, count * sizeof(float));
                    
                    cJSON* float_array = cJSON_CreateFloatArray(numbers, count);
                    if (float_array) {
                        cJSON_Delete(float_array);
                    }
                    
                    // Also test with negative count
                    cJSON_CreateFloatArray(numbers, -1);
                    
                    // Test with NULL numbers
                    cJSON_CreateFloatArray(NULL, count);
                    
                    free(numbers);
                }
            }
        }
    }
    
    return 0;
}