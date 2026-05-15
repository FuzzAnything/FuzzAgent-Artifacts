// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON *create_test_item_from_data(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    cJSON *item = NULL;
    switch (data[0] % 7) {
        case 0:
            item = cJSON_CreateNull();
            break;
        case 1:
            item = cJSON_CreateTrue();
            break;
        case 2:
            item = cJSON_CreateFalse();
            break;
        case 3:
            if (size > 1) {
                char *str = (char*)malloc(size);
                if (str) {
                    memcpy(str, data, size);
                    str[size-1] = '\0';
                    item = cJSON_CreateString(str);
                    free(str);
                }
            }
            break;
        case 4:
            if (size > 1) {
                double num = 0.0;
                for (size_t i = 0; i < (size < 8 ? size : 8); i++) {
                    num += (double)data[i] / (1 << (8 * i));
                }
                item = cJSON_CreateNumber(num);
            }
            break;
        case 5:
            item = cJSON_CreateArray();
            if (item && size > 1) {
                for (size_t i = 1; i < (size < 5 ? size : 5); i++) {
                    cJSON_AddItemToArray(item, cJSON_CreateNumber(data[i]));
                }
            }
            break;
        case 6:
            item = cJSON_CreateObject();
            if (item && size > 1) {
                for (size_t i = 1; i < (size < 5 ? size : 5); i++) {
                    char key[2] = {(char)('a' + data[i] % 26), '\0'};
                    cJSON_AddItemToObject(item, key, cJSON_CreateNumber(data[i]));
                }
            }
            break;
    }
    
    return item;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *data, size_t size) {
    if (size < 1) {
        return 0;
    }
    
    cJSON *test_item = create_test_item_from_data(data, size);
    
    if (test_item) {
        cJSON_bool recurse = (data[0] & 0x01) ? 1 : 0;
        cJSON *duplicate = cJSON_Duplicate(test_item, recurse);
        
        if (duplicate) {
            cJSON_bool case_sensitive = (data[0] & 0x02) ? 1 : 0;
            cJSON_Compare(test_item, duplicate, case_sensitive);
            
            cJSON_IsInvalid(duplicate);
            cJSON_IsString(duplicate);
            cJSON_IsTrue(duplicate);
            
            cJSON_Delete(duplicate);
        }
        
        cJSON_IsInvalid(test_item);
        cJSON_IsString(test_item);
        cJSON_IsTrue(test_item);
        
        cJSON_Delete(test_item);
    }
    
    cJSON *false_item = cJSON_CreateFalse();
    if (false_item) {
        cJSON_IsInvalid(false_item);
        cJSON_IsString(false_item);
        cJSON_IsTrue(false_item);
        cJSON_Delete(false_item);
    }
    
    return 0;
}