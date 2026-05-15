// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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

static cJSON *create_test_item_from_data(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    uint8_t type = data[0] % 7;
    
    switch (type) {
        case 0:
            return cJSON_CreateNull();
        case 1:
            return cJSON_CreateTrue();
        case 2:
            return cJSON_CreateFalse();
        case 3:
            if (size > 1) {
                char *str = (char *)malloc(size);
                if (str) {
                    memcpy(str, data, size - 1);
                    str[size - 1] = '\0';
                    cJSON *item = cJSON_CreateString(str);
                    free(str);
                    return item;
                }
            }
            return cJSON_CreateString("test");
        case 4:
            if (size > 1) {
                double num = (double)data[1];
                return cJSON_CreateNumber(num);
            }
            return cJSON_CreateNumber(42.0);
        case 5:
            if (size > 1) {
                char *raw = (char *)malloc(size);
                if (raw) {
                    memcpy(raw, data, size - 1);
                    raw[size - 1] = '\0';
                    cJSON *item = cJSON_CreateRaw(raw);
                    free(raw);
                    return item;
                }
            }
            return cJSON_CreateRaw("{\"test\":123}");
        case 6:
            return cJSON_CreateArray();
        default:
            return cJSON_CreateObject();
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    uint8_t selector = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    switch (selector % 6) {
        case 0: {  // Test cJSON_IsTrue
            cJSON *item = create_test_item_from_data(fuzz_data, fuzz_size);
            cJSON_IsTrue(item);
            cJSON_Delete(item);
            break;
        }
        
        case 1: {  // Test cJSON_CreateFalse
            cJSON *false_item = cJSON_CreateFalse();
            if (false_item) {
                cJSON_IsTrue(false_item);
                cJSON_Delete(false_item);
            }
            break;
        }
        
        case 2: {  // Test cJSON_Duplicate
            cJSON *item = create_test_item_from_data(fuzz_data, fuzz_size);
            if (item) {
                cJSON_bool recurse = (fuzz_size > 0) ? (fuzz_data[0] % 2) : 0;
                cJSON *duplicate = cJSON_Duplicate(item, recurse);
                if (duplicate) {
                    cJSON_Delete(duplicate);
                }
                cJSON_Delete(item);
            }
            break;
        }
        
        case 3: {  // Test cJSON_IsRaw
            cJSON *item = create_test_item_from_data(fuzz_data, fuzz_size);
            cJSON_IsRaw(item);
            cJSON_Delete(item);
            break;
        }
        
        case 4: {  // Test cJSON_IsString
            cJSON *item = create_test_item_from_data(fuzz_data, fuzz_size);
            cJSON_IsString(item);
            cJSON_Delete(item);
            break;
        }
        
        case 5: {  // Test cJSON_Compare
            cJSON *item1 = create_test_item_from_data(fuzz_data, fuzz_size);
            cJSON *item2 = NULL;
            
            if (fuzz_size > 1) {
                item2 = create_test_item_from_data(fuzz_data + 1, fuzz_size - 1);
            } else {
                item2 = create_test_item_from_data(fuzz_data, fuzz_size);
            }
            
            if (item1 && item2) {
                cJSON_bool case_sensitive = (fuzz_size > 0) ? (fuzz_data[0] % 2) : 0;
                cJSON_Compare(item1, item2, case_sensitive);
            }
            
            if (item1) cJSON_Delete(item1);
            if (item2) cJSON_Delete(item2);
            break;
        }
    }
    
    return 0;
}