// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ReplaceItemViaPointer at cJSON.c:2368:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static cJSON* create_test_array(void) {
    cJSON* array = cJSON_CreateArray();
    if (!array) return NULL;
    
    // Add some items to the array
    cJSON_AddItemToArray(array, cJSON_CreateString("item1"));
    cJSON_AddItemToArray(array, cJSON_CreateNumber(42));
    cJSON_AddItemToArray(array, cJSON_CreateBool(1));
    cJSON_AddItemToArray(array, cJSON_CreateNull());
    cJSON_AddItemToArray(array, cJSON_CreateObject());
    
    return array;
}

static cJSON* create_test_item(const uint8_t* data, size_t size) {
    if (size == 0) {
        return cJSON_CreateNull();
    }
    
    // Use first byte to determine type
    uint8_t type = data[0] % 5;
    
    switch (type) {
        case 0:
            return cJSON_CreateString("test_string");
        case 1:
            return cJSON_CreateNumber(3.14159);
        case 2:
            return cJSON_CreateBool(data[1] % 2);
        case 3:
            return cJSON_CreateNull();
        case 4: {
            cJSON* obj = cJSON_CreateObject();
            if (obj && size > 2) {
                cJSON_AddItemToObject(obj, "key", cJSON_CreateNumber(data[2]));
            }
            return obj;
        }
        default:
            return cJSON_CreateNull();
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t* Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Create test array
    cJSON* array = create_test_array();
    if (!array) return 0;
    
    // Use first byte to determine which function to test
    uint8_t func_selector = Data[0] % 6;
    
    // Use second byte for index (mod array size + 2 to test out-of-bounds)
    int array_size = cJSON_GetArraySize(array);
    int index = (int)Data[1];
    if (array_size > 0) {
        index = index % (array_size + 2);
    }
    
    switch (func_selector) {
        case 0: {
            // Test cJSON_DeleteItemFromArray
            cJSON_DeleteItemFromArray(array, index);
            break;
        }
        
        case 1: {
            // Test cJSON_DetachItemFromArray
            cJSON* detached = cJSON_DetachItemFromArray(array, index);
            if (detached) {
                cJSON_Delete(detached);
            }
            break;
        }
        
        case 2: {
            // Test cJSON_ReplaceItemInArray
            cJSON* newitem = create_test_item(Data + 2, Size > 2 ? Size - 2 : 0);
            if (newitem) {
                cJSON_ReplaceItemInArray(array, index, newitem);
            }
            break;
        }
        
        case 3: {
            // Test cJSON_ReplaceItemViaPointer
            if (array_size > 0 && index >= 0 && index < array_size) {
                cJSON* item = cJSON_GetArrayItem(array, index);
                if (item) {
                    cJSON* replacement = create_test_item(Data + 2, Size > 2 ? Size - 2 : 0);
                    if (replacement) {
                        cJSON_ReplaceItemViaPointer(array, item, replacement);
                    }
                }
            }
            break;
        }
        
        case 4: {
            // Test cJSON_InsertItemInArray
            cJSON* newitem = create_test_item(Data + 2, Size > 2 ? Size - 2 : 0);
            if (newitem) {
                cJSON_InsertItemInArray(array, index, newitem);
            }
            break;
        }
        
        case 5: {
            // Test cJSON_GetArraySize
            int size = cJSON_GetArraySize(array);
            (void)size; // Use variable to avoid unused warning
            break;
        }
    }
    
    // Clean up
    cJSON_Delete(array);
    
    return 0;
}