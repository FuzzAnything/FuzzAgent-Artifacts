// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static cJSON *create_test_object(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateObject();
    }
    
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
        return cJSON_CreateObject();
    }
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *root = cJSON_Parse(json_str);
    free(json_str);
    
    if (!root) {
        return cJSON_CreateObject();
    }
    
    return root;
}

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

static void safe_memcpy(char *dest, size_t dest_size, const uint8_t *src, size_t src_size, size_t count) {
    if (count > dest_size - 1) {
        count = dest_size - 1;
    }
    if (src_size < count) {
        count = src_size;
    }
    if (count > 0) {
        memcpy(dest, src, count);
    }
    dest[count] = '\0';
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    cJSON *obj = create_test_object(Data, Size);
    if (!obj) {
        return 0;
    }
    
    if (obj->type != cJSON_Object) {
        cJSON_Delete(obj);
        obj = cJSON_CreateObject();
        if (!obj) {
            return 0;
        }
    }
    
    size_t key_len = (Data[0] % 10) + 1;
    char key[11];
    safe_memcpy(key, sizeof(key), Data, Size, key_len);
    
    cJSON *arr = cJSON_AddArrayToObject(obj, key);
    if (arr) {
        if (Size > 1) {
            cJSON_AddNumberToObject(obj, "temp_num", Data[1]);
        }
        
        int which = (Data[0] % 5) - 2;
        cJSON_DeleteItemFromArray(arr, which);
        
        char detach_key[11];
        safe_memcpy(detach_key, sizeof(detach_key), Data, Size, key_len);
        
        cJSON *detached = cJSON_DetachItemFromObject(obj, detach_key);
        if (detached) {
            cJSON_AddItemToObject(obj, detach_key, detached);
        }
        
        char detach_cs_key[11];
        safe_memcpy(detach_cs_key, sizeof(detach_cs_key), Data, Size, key_len);
        
        cJSON *detached_cs = cJSON_DetachItemFromObjectCaseSensitive(obj, detach_cs_key);
        if (detached_cs) {
            cJSON_AddItemToObject(obj, detach_cs_key, detached_cs);
        }
    }
    
    char delete_key[11];
    safe_memcpy(delete_key, sizeof(delete_key), Data, Size, key_len);
    cJSON_DeleteItemFromObject(obj, delete_key);
    
    char delete_cs_key[11];
    safe_memcpy(delete_cs_key, sizeof(delete_cs_key), Data, Size, key_len);
    cJSON_DeleteItemFromObjectCaseSensitive(obj, delete_cs_key);
    
    cleanup_cjson(obj);
    
    return 0;
}