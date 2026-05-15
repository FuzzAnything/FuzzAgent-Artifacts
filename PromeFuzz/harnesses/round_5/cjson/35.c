// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static cJSON* create_test_item(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateNull();
    }
    
    switch (data[0] % 7) {
        case 0:
            return cJSON_CreateNull();
        case 1:
            return cJSON_CreateTrue();
        case 2:
            return cJSON_CreateFalse();
        case 3:
            return cJSON_CreateNumber((double)(size));
        case 4: {
            char *str = (char*)malloc(size + 1);
            if (str) {
                memcpy(str, data, size);
                str[size] = '\0';
                cJSON *item = cJSON_CreateString(str);
                free(str);
                return item;
            }
            return cJSON_CreateString("");
        }
        case 5: {
            cJSON *array = cJSON_CreateArray();
            if (array && size > 1) {
                cJSON_AddItemToArray(array, cJSON_CreateNumber(data[1]));
                cJSON_AddItemToArray(array, cJSON_CreateString("test"));
            }
            return array;
        }
        case 6: {
            cJSON *obj = cJSON_CreateObject();
            if (obj && size > 1) {
                cJSON_AddItemToObject(obj, "key1", cJSON_CreateNumber(data[1]));
                cJSON_AddItemToObject(obj, "key2", cJSON_CreateString("value"));
            }
            return obj;
        }
        default:
            return cJSON_CreateNull();
    }
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    cJSON *item1 = create_test_item(Data, Size);
    cJSON *item2 = NULL;
    
    if (Size > 1) {
        item2 = create_test_item(Data + 1, Size - 1);
    } else {
        item2 = cJSON_CreateNull();
    }
    
    cJSON_bool case_sensitive = (Size > 2) ? (Data[2] % 2) : 0;
    
    cJSON_IsNull(item1);
    cJSON_IsNull(item2);
    
    cJSON_IsTrue(item1);
    cJSON_IsTrue(item2);
    
    cJSON *false_item = cJSON_CreateFalse();
    if (false_item) {
        cJSON_IsTrue(false_item);
        cJSON_Delete(false_item);
    }
    
    cJSON_IsString(item1);
    cJSON_IsString(item2);
    
    cJSON_Compare(item1, item2, case_sensitive);
    cJSON_Compare(item1, item1, case_sensitive);
    cJSON_Compare(item2, item2, case_sensitive);
    
    cJSON *dup1 = cJSON_Duplicate(item1, (Size > 3) ? (Data[3] % 2) : 1);
    if (dup1) {
        cJSON_Compare(item1, dup1, case_sensitive);
        cJSON_Delete(dup1);
    }
    
    cJSON *dup2 = cJSON_Duplicate(item2, (Size > 4) ? (Data[4] % 2) : 0);
    if (dup2) {
        cJSON_Compare(item2, dup2, case_sensitive);
        cJSON_Delete(dup2);
    }
    
    cJSON_Delete(item1);
    cJSON_Delete(item2);
    
    return 0;
}