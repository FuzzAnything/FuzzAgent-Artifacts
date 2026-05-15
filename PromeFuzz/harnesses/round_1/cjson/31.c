// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ReplaceItemInArray at cJSON.c:2417:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static cJSON *create_test_item(uint8_t type_byte) {
    switch (type_byte % 4) {
        case 0:
            return cJSON_CreateNumber((double)type_byte);
        case 1:
            return cJSON_CreateString("test");
        case 2:
            return cJSON_CreateArray();
        case 3:
            return cJSON_CreateObject();
        default:
            return cJSON_CreateNull();
    }
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    cJSON *root = cJSON_CreateArray();
    if (!root) return 0;
    
    size_t initial_items = (Data[0] % 10) + 1;
    for (size_t i = 0; i < initial_items && i < Size - 1; i++) {
        cJSON *item = create_test_item(Data[i + 1]);
        if (item) {
            cJSON_AddItemToArray(root, item);
        }
    }
    
    if (cJSON_IsArray(root)) {
        int array_size = cJSON_GetArraySize(root);
        
        if (array_size > 0) {
            size_t data_index = (initial_items < Size - 1) ? initial_items + 1 : 1;
            
            while (data_index + 1 < Size) {
                uint8_t op = Data[data_index] % 6;
                uint8_t index_byte = Data[data_index + 1];
                int index = index_byte % (array_size + 2);
                
                switch (op) {
                    case 0: {
                        cJSON_DeleteItemFromArray(root, index);
                        break;
                    }
                    case 1: {
                        cJSON *new_item = create_test_item(index_byte);
                        if (new_item) {
                            cJSON_InsertItemInArray(root, index, new_item);
                        }
                        break;
                    }
                    case 2: {
                        int current_size = cJSON_GetArraySize(root);
                        (void)current_size;
                        break;
                    }
                    case 3: {
                        cJSON *detached = cJSON_DetachItemFromArray(root, index);
                        if (detached) {
                            cJSON_Delete(detached);
                        }
                        break;
                    }
                    case 4: {
                        cJSON *replacement = create_test_item(index_byte);
                        if (replacement) {
                            cJSON_ReplaceItemInArray(root, index, replacement);
                        }
                        break;
                    }
                    case 5: {
                        cJSON_bool is_array = cJSON_IsArray(root);
                        (void)is_array;
                        break;
                    }
                }
                
                data_index += 2;
                array_size = cJSON_GetArraySize(root);
                
                if (array_size == 0) {
                    cJSON *new_item = create_test_item(Data[data_index % Size]);
                    if (new_item) {
                        cJSON_AddItemToArray(root, new_item);
                        array_size = 1;
                    }
                }
            }
        }
    }
    
    cJSON_Delete(root);
    return 0;
}