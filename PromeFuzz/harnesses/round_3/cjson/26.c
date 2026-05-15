// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static cJSON* create_test_item(const uint8_t* data, size_t size, int item_type) {
    if (size == 0) {
        return NULL;
    }

    switch (item_type % 7) {
        case 0:
            return cJSON_CreateFalse();
        case 1:
            return cJSON_CreateTrue();
        case 2:
            return cJSON_CreateNull();
        case 3: {
            double num = 0.0;
            if (size >= sizeof(double)) {
                memcpy(&num, data, sizeof(double));
            }
            return cJSON_CreateNumber(num);
        }
        case 4: {
            char* str = (char*)malloc(size + 1);
            if (!str) return NULL;
            memcpy(str, data, size);
            str[size] = '\0';
            cJSON* item = cJSON_CreateString(str);
            free(str);
            return item;
        }
        case 5: {
            cJSON* array = cJSON_CreateArray();
            if (array && size > 0) {
                cJSON_AddItemToArray(array, cJSON_CreateNumber(data[0]));
            }
            return array;
        }
        case 6: {
            cJSON* obj = cJSON_CreateObject();
            if (obj && size > 0) {
                cJSON_AddItemToObject(obj, "key", cJSON_CreateNumber(data[0]));
            }
            return obj;
        }
        default:
            return cJSON_CreateFalse();
    }
}

static cJSON* create_raw_item(const uint8_t* data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    char* raw_str = (char*)malloc(size + 1);
    if (!raw_str) return NULL;
    
    memcpy(raw_str, data, size);
    raw_str[size] = '\0';
    
    cJSON* item = cJSON_CreateRaw(raw_str);
    free(raw_str);
    return item;
}

int LLVMFuzzerTestOneInput_26(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use first byte to determine test scenario
    uint8_t scenario = Data[0];
    const uint8_t* fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    if (fuzz_size == 0) {
        fuzz_data = Data;
        fuzz_size = Size;
    }

    // Test cJSON_CreateFalse
    cJSON* false_item = cJSON_CreateFalse();
    if (false_item) {
        // Test cJSON_IsTrue on false item
        cJSON_IsTrue(false_item);
        
        // Test cJSON_IsString on false item
        cJSON_IsString(false_item);
        
        cJSON_Delete(false_item);
    }

    // Create test items based on fuzz data
    cJSON* item1 = create_test_item(fuzz_data, fuzz_size, scenario);
    cJSON* item2 = create_test_item(fuzz_data + (fuzz_size > 1 ? 1 : 0), 
                                   fuzz_size > 1 ? fuzz_size - 1 : fuzz_size, 
                                   scenario + 1);

    if (item1) {
        // Test cJSON_IsTrue
        cJSON_IsTrue(item1);
        
        // Test cJSON_IsString
        cJSON_IsString(item1);
        
        // Test cJSON_Duplicate with varying recursion
        cJSON* dup1 = cJSON_Duplicate(item1, scenario & 1);
        cJSON* dup2 = cJSON_Duplicate(item1, !(scenario & 1));
        
        if (dup1) {
            // Test cJSON_Compare
            if (item2) {
                cJSON_Compare(item1, item2, scenario & 1);
                cJSON_Compare(dup1, item2, !(scenario & 1));
            }
            
            // Compare with duplicate
            cJSON_Compare(item1, dup1, 1);
            cJSON_Compare(item1, dup1, 0);
            
            cJSON_Delete(dup1);
        }
        
        if (dup2) {
            cJSON_Delete(dup2);
        }
    }

    // Create and test raw item
    cJSON* raw_item = create_raw_item(fuzz_data, fuzz_size > 100 ? 100 : fuzz_size);
    if (raw_item) {
        // Test cJSON_IsRaw
        cJSON_IsRaw(raw_item);
        
        // Test cJSON_IsTrue on raw item
        cJSON_IsTrue(raw_item);
        
        // Test cJSON_IsString on raw item
        cJSON_IsString(raw_item);
        
        // Duplicate raw item
        cJSON* raw_dup = cJSON_Duplicate(raw_item, scenario & 1);
        if (raw_dup) {
            cJSON_Compare(raw_item, raw_dup, 1);
            cJSON_Delete(raw_dup);
        }
        
        cJSON_Delete(raw_item);
    }

    // Cleanup
    if (item1) {
        cJSON_Delete(item1);
    }
    if (item2) {
        cJSON_Delete(item2);
    }

    return 0;
}