// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
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

static void cleanup_detached_item(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

static cJSON* create_test_object(void) {
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) return NULL;
    
    cJSON_AddStringToObject(root, "key1", "value1");
    cJSON_AddNumberToObject(root, "key2", 42);
    cJSON_AddBoolToObject(root, "key3", 1);
    cJSON_AddNullToObject(root, "key4");
    
    cJSON *array = cJSON_CreateArray();
    if (array != NULL) {
        cJSON_AddItemToArray(array, cJSON_CreateString("array_item1"));
        cJSON_AddItemToArray(array, cJSON_CreateNumber(100));
        cJSON_AddItemToObject(root, "array_key", array);
    }
    
    return root;
}

static char* extract_string(const uint8_t *data, size_t size, size_t max_len) {
    if (size == 0) return NULL;
    
    size_t len = size;
    if (len > max_len) len = max_len;
    
    char *str = (char*)malloc(len + 1);
    if (str == NULL) return NULL;
    
    memcpy(str, data, len);
    str[len] = '\0';
    
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '\0') {
            str[i] = 'a';
        }
    }
    
    return str;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    cJSON *root = create_test_object();
    if (root == NULL) return 0;
    
    size_t max_key_len = 32;
    char *key = extract_string(Data, Size, max_key_len);
    if (key == NULL) {
        cJSON_Delete(root);
        return 0;
    }
    
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0: {
            cJSON *item = cJSON_GetObjectItem(root, "key1");
            if (item != NULL) {
                cJSON *detached = cJSON_DetachItemViaPointer(root, item);
                cleanup_detached_item(detached);
            }
            break;
        }
        case 1: {
            cJSON *detached = cJSON_DetachItemFromObject(root, key);
            cleanup_detached_item(detached);
            break;
        }
        case 2: {
            cJSON_DeleteItemFromObjectCaseSensitive(root, key);
            break;
        }
        case 3: {
            cJSON_DeleteItemFromObject(root, key);
            break;
        }
        case 4: {
            cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, key);
            cleanup_detached_item(detached);
            break;
        }
        case 5: {
            cJSON *true_item = cJSON_CreateTrue();
            if (true_item != NULL) {
                cJSON_AddItemToObject(root, "new_true_key", true_item);
                
                cJSON *detached = cJSON_DetachItemFromObject(root, "new_true_key");
                cleanup_detached_item(detached);
            }
            break;
        }
    }
    
    free(key);
    cJSON_Delete(root);
    
    return 0;
}