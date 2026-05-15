// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON* create_test_item(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateFalse();
    }
    
    // Use first byte to determine type
    uint8_t type = data[0] % 6;
    
    switch (type) {
        case 0:
            return cJSON_CreateFalse();
        case 1:
            return cJSON_CreateTrue();
        case 2: {
            // Create string with fuzzer data
            char *str = (char*)malloc(size + 1);
            if (!str) return NULL;
            memcpy(str, data, size);
            str[size] = '\0';
            cJSON *item = cJSON_CreateString(str);
            free(str);
            return item;
        }
        case 3: {
            // Create raw JSON string
            char *raw_str = (char*)malloc(size + 1);
            if (!raw_str) return NULL;
            memcpy(raw_str, data, size);
            raw_str[size] = '\0';
            cJSON *item = cJSON_CreateRaw(raw_str);
            free(raw_str);
            return item;
        }
        case 4: {
            // Create number
            double num = 0.0;
            if (size >= sizeof(double)) {
                memcpy(&num, data, sizeof(double));
            } else if (size > 0) {
                num = (double)data[0];
            }
            return cJSON_CreateNumber(num);
        }
        case 5:
            return cJSON_CreateNull();
        default:
            return cJSON_CreateFalse();
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test cJSON_CreateFalse
    cJSON *false_item = cJSON_CreateFalse();
    if (false_item) {
        cJSON_Delete(false_item);
    }
    
    // Create test items
    cJSON *item1 = create_test_item(Data, Size);
    if (!item1) return 0;
    
    // Test cJSON_IsTrue
    (void)cJSON_IsTrue(item1);
    
    // Test cJSON_IsRaw
    (void)cJSON_IsRaw(item1);
    
    // Test cJSON_IsString
    (void)cJSON_IsString(item1);
    
    // Test cJSON_Duplicate with different recurse values
    cJSON *dup1 = cJSON_Duplicate(item1, 0);
    cJSON *dup2 = cJSON_Duplicate(item1, 1);
    
    // Test cJSON_Compare with various combinations
    (void)cJSON_Compare(item1, item1, 0);  // case insensitive
    (void)cJSON_Compare(item1, item1, 1);  // case sensitive
    
    if (dup1) {
        (void)cJSON_Compare(item1, dup1, 0);
        (void)cJSON_Compare(item1, dup1, 1);
        cJSON_Delete(dup1);
    }
    
    if (dup2) {
        (void)cJSON_Compare(item1, dup2, 0);
        (void)cJSON_Compare(item1, dup2, 1);
        cJSON_Delete(dup2);
    }
    
    // Create another item for comparison
    cJSON *item2 = create_test_item(Data + (Size > 1 ? 1 : 0), Size > 1 ? Size - 1 : 1);
    if (item2) {
        (void)cJSON_Compare(item1, item2, 0);
        (void)cJSON_Compare(item1, item2, 1);
        cJSON_Delete(item2);
    }
    
    // Cleanup
    cJSON_Delete(item1);
    
    return 0;
}