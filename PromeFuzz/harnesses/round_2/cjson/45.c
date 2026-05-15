// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
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

static cJSON* create_test_item(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    
    cJSON *item = NULL;
    unsigned char selector = data[0] % 8;
    
    switch (selector) {
        case 0:
            item = cJSON_CreateNull();
            break;
        case 1:
            item = cJSON_CreateTrue();
            break;
        case 2:
            item = cJSON_CreateFalse();
            break;
        case 3: {
            int num = (size > 1) ? (int)data[1] : 0;
            item = cJSON_CreateNumber(num);
            break;
        }
        case 4: {
            char *str = (size > 1) ? (char*)malloc(size) : NULL;
            if (str && size > 1) {
                memcpy(str, data + 1, size - 1);
                str[size - 1] = '\0';
                item = cJSON_CreateString(str);
                free(str);
            } else {
                item = cJSON_CreateString("test");
            }
            break;
        }
        case 5: {
            char *raw = (size > 1) ? (char*)malloc(size) : NULL;
            if (raw && size > 1) {
                memcpy(raw, data + 1, size - 1);
                raw[size - 1] = '\0';
                item = cJSON_CreateRaw(raw);
                free(raw);
            } else {
                item = cJSON_CreateRaw("{}");
            }
            break;
        }
        case 6:
            item = cJSON_CreateArray();
            if (item && size > 1) {
                for (size_t i = 1; i < size && i < 5; i++) {
                    cJSON_AddItemToArray(item, cJSON_CreateNumber(data[i]));
                }
            }
            break;
        case 7:
            item = cJSON_CreateObject();
            if (item && size > 1) {
                char key[16];
                for (size_t i = 1; i < size && i < 5; i++) {
                    snprintf(key, sizeof(key), "key%zu", i);
                    cJSON_AddItemToObject(item, key, cJSON_CreateNumber(data[i]));
                }
            }
            break;
    }
    
    return item;
}

static void test_functions(cJSON *item) {
    if (!item) return;
    
    cJSON_IsRaw(item);
    cJSON_IsString(item);
    cJSON_IsTrue(item);
    
    cJSON *dup = cJSON_Duplicate(item, 0);
    if (dup) {
        cJSON_Compare(item, dup, 1);
        cJSON_Compare(item, dup, 0);
        cJSON_Delete(dup);
    }
    
    if (item->type == cJSON_Object) {
        cJSON_GetObjectItemCaseSensitive(item, "key1");
        cJSON_GetObjectItemCaseSensitive(item, "nonexistent");
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    cJSON *parsed = NULL;
    if (size > 0) {
        char *json_str = (char*)malloc(size + 1);
        if (!json_str) return 0;
        memcpy(json_str, data, size);
        json_str[size] = '\0';
        
        parsed = cJSON_Parse(json_str);
        free(json_str);
    }
    
    cJSON *created = create_test_item(data, size);
    
    if (parsed) {
        test_functions(parsed);
        
        cJSON *dup_parsed = cJSON_Duplicate(parsed, 1);
        if (dup_parsed) {
            test_functions(dup_parsed);
            cJSON_Delete(dup_parsed);
        }
        
        cJSON_Delete(parsed);
    }
    
    if (created) {
        test_functions(created);
        cJSON_Delete(created);
    }
    
    cJSON *null_item = NULL;
    cJSON_IsRaw(null_item);
    cJSON_IsString(null_item);
    cJSON_IsTrue(null_item);
    cJSON_Compare(null_item, NULL, 1);
    cJSON_Duplicate(NULL, 0);
    
    return 0;
}