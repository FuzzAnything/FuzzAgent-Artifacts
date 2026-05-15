// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON *create_random_item(const uint8_t *data, size_t size) {
    if (size == 0) return cJSON_CreateNull();
    
    switch (data[0] % 6) {
        case 0: return cJSON_CreateNull();
        case 1: return cJSON_CreateBool(data[1] % 2);
        case 2: return cJSON_CreateNumber((double)(data[1] * 256 + data[2 % size]));
        case 3: {
            char str_buf[256];
            size_t str_len = size < 255 ? size : 255;
            memcpy(str_buf, data, str_len);
            str_buf[str_len] = '\0';
            return cJSON_CreateString(str_buf);
        }
        case 4: return cJSON_CreateArray();
        case 5: return cJSON_CreateObject();
        default: return cJSON_CreateNull();
    }
}

static char *extract_string(const uint8_t *data, size_t size, size_t max_len) {
    if (size == 0) {
        char *str = malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    size_t len = size < max_len ? size : max_len;
    char *str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;
    
    size_t str_len = Size < 100 ? Size : 100;
    char *key1 = extract_string(Data, str_len, 50);
    char *key2 = extract_string(Data + (str_len/2), Size - (str_len/2), 50);
    
    if (!key1 || !key2) {
        free(key1);
        free(key2);
        cJSON_Delete(root);
        return 0;
    }
    
    cJSON *item1 = create_random_item(Data, Size);
    cJSON *item2 = create_random_item(Data + (Size/2), Size - (Size/2));
    cJSON *item3 = create_random_item(Data + (Size/3), Size - (Size/3));
    
    if (item1 && item2 && item3) {
        cJSON_AddItemToObject(root, key1, item1);
        cJSON_AddItemToObject(root, key2, item2);
        
        cJSON_DeleteItemFromObject(root, key1);
        
        cJSON_AddItemToObject(root, "newkey", item3);
        
        cJSON_ReplaceItemInObjectCaseSensitive(root, key2, create_random_item(Data, Size/2));
        cJSON_ReplaceItemInObject(root, "newkey", create_random_item(Data + (Size/4), Size - (Size/4)));
        
        cJSON_AddItemToObjectCS(root, "const_key", create_random_item(Data, Size/3));
        
        cJSON *new_obj = cJSON_AddObjectToObject(root, "nested_object");
        if (new_obj) {
            cJSON_AddItemToObject(new_obj, "inner_key", create_random_item(Data + (Size/5), Size - (Size/5)));
        }
    }
    
    free(key1);
    free(key2);
    cJSON_Delete(root);
    
    return 0;
}