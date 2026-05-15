// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
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

static cJSON* create_random_cjson(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    
    switch (data[0] % 7) {
        case 0:
            return cJSON_CreateNull();
        case 1:
            return cJSON_CreateTrue();
        case 2:
            return cJSON_CreateFalse();
        case 3:
            if (size > 1) {
                char *str = (char*)malloc(size);
                if (!str) return NULL;
                memcpy(str, data, size);
                str[size-1] = '\0';
                cJSON *item = cJSON_CreateString(str);
                free(str);
                return item;
            }
            return cJSON_CreateString("");
        case 4:
            if (size > 1) {
                return cJSON_CreateNumber((double)(data[0] * 256 + data[1]) / 100.0);
            }
            return cJSON_CreateNumber(0);
        case 5: {
            cJSON *array = cJSON_CreateArray();
            if (array && size > 2) {
                for (size_t i = 1; i < size && i < 5; i++) {
                    cJSON_AddItemToArray(array, cJSON_CreateNumber(data[i]));
                }
            }
            return array;
        }
        case 6: {
            cJSON *obj = cJSON_CreateObject();
            if (obj && size > 4) {
                char key1[10], key2[10];
                snprintf(key1, sizeof(key1), "k%d", data[1]);
                snprintf(key2, sizeof(key2), "k%d", data[2]);
                cJSON_AddItemToObject(obj, key1, cJSON_CreateNumber(data[3]));
                cJSON_AddItemToObject(obj, key2, cJSON_CreateNumber(data[4]));
            }
            return obj;
        }
        default:
            return NULL;
    }
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test cJSON_CreateFalse
    cJSON *false_item = cJSON_CreateFalse();
    if (false_item) {
        // Test cJSON_IsTrue on false item
        cJSON_IsTrue(false_item);
        
        // Test cJSON_IsInvalid on false item
        cJSON_IsInvalid(false_item);
        
        // Test cJSON_IsString on false item
        cJSON_IsString(false_item);
        
        cJSON_Delete(false_item);
    }
    
    // Create random cJSON objects from fuzzer input
    cJSON *item1 = create_random_cjson(Data, Size);
    cJSON *item2 = NULL;
    
    if (Size > 1) {
        item2 = create_random_cjson(Data + 1, Size - 1);
    }
    
    // Test cJSON_Duplicate with various recurse values
    if (item1) {
        cJSON *dup1 = cJSON_Duplicate(item1, Data[0] % 2);
        if (dup1) {
            cJSON_Delete(dup1);
        }
    }
    
    if (item2) {
        cJSON *dup2 = cJSON_Duplicate(item2, (Data[0] + 1) % 2);
        if (dup2) {
            cJSON_Delete(dup2);
        }
    }
    
    // Test cJSON_Compare with various combinations
    if (item1 && item2) {
        // Compare with case sensitivity based on input
        cJSON_Compare(item1, item2, Data[0] % 2);
        
        // Compare item with itself
        cJSON_Compare(item1, item1, (Data[0] + 1) % 2);
        
        // Compare item with its duplicate
        cJSON *dup = cJSON_Duplicate(item1, 1);
        if (dup) {
            cJSON_Compare(item1, dup, Data[0] % 2);
            cJSON_Delete(dup);
        }
    }
    
    // Test cJSON_IsInvalid with NULL and valid items
    cJSON_IsInvalid(NULL);
    if (item1) {
        cJSON_IsInvalid(item1);
    }
    
    // Test cJSON_IsString with various items
    cJSON_IsString(NULL);
    if (item1) {
        cJSON_IsString(item1);
    }
    
    // Test cJSON_IsTrue with various items
    cJSON_IsTrue(NULL);
    if (item1) {
        cJSON_IsTrue(item1);
    }
    
    // Cleanup
    if (item1) cJSON_Delete(item1);
    if (item2) cJSON_Delete(item2);
    
    return 0;
}