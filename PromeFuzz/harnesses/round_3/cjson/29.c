// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
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

static cJSON* create_test_object(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateNull();
    }
    
    // Use first byte to determine object type
    uint8_t type = data[0] % 8;
    
    switch (type) {
        case 0:
            return cJSON_CreateFalse();
        case 1:
            return cJSON_CreateTrue();
        case 2:
            return cJSON_CreateBool(data[1] % 2);
        case 3:
            if (size > 1) {
                char *str = (char*)malloc(size);
                if (str) {
                    memcpy(str, data, size - 1);
                    str[size - 1] = '\0';
                    cJSON *obj = cJSON_CreateString(str);
                    free(str);
                    return obj;
                }
            }
            return cJSON_CreateString("");
        case 4:
            if (size > 1) {
                return cJSON_CreateNumber((double)data[1]);
            }
            return cJSON_CreateNumber(0.0);
        case 5:
            return cJSON_CreateArray();
        case 6:
            return cJSON_CreateObject();
        case 7:
            return cJSON_CreateNull();
        default:
            return cJSON_CreateNull();
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test cJSON_CreateFalse
    cJSON *false_obj = cJSON_CreateFalse();
    if (false_obj) {
        // Test cJSON_IsTrue on false object
        cJSON_bool is_true = cJSON_IsTrue(false_obj);
        (void)is_true; // Use result to avoid unused variable warning
        
        // Test cJSON_IsInvalid on false object
        cJSON_bool is_invalid = cJSON_IsInvalid(false_obj);
        (void)is_invalid;
        
        cJSON_Delete(false_obj);
    }

    // Create test objects from fuzzer data
    cJSON *obj1 = create_test_object(Data, Size);
    if (!obj1) {
        return 0;
    }

    // Test cJSON_IsString
    cJSON_bool is_string = cJSON_IsString(obj1);
    (void)is_string;

    // Test cJSON_IsInvalid
    cJSON_bool is_invalid = cJSON_IsInvalid(obj1);
    (void)is_invalid;

    // Test cJSON_IsTrue
    cJSON_bool is_true = cJSON_IsTrue(obj1);
    (void)is_true;

    // Test cJSON_Duplicate with different recurse values
    cJSON_bool recurse = (Size > 1) ? (Data[1] % 2) : 0;
    cJSON *duplicate = cJSON_Duplicate(obj1, recurse);
    if (duplicate) {
        // Test cJSON_Compare with various case sensitivity settings
        cJSON_bool case_sensitive = (Size > 2) ? (Data[2] % 2) : 0;
        cJSON_bool compare_result = cJSON_Compare(obj1, duplicate, case_sensitive);
        (void)compare_result;
        
        // Also compare with NULL objects
        cJSON_Compare(NULL, duplicate, case_sensitive);
        cJSON_Compare(obj1, NULL, case_sensitive);
        cJSON_Compare(NULL, NULL, case_sensitive);
        
        cJSON_Delete(duplicate);
    }

    // Create second object for comparison tests
    cJSON *obj2 = NULL;
    if (Size > 3) {
        obj2 = create_test_object(Data + 3, Size - 3);
    } else {
        obj2 = create_test_object(Data, Size);
    }

    if (obj2) {
        // Test cJSON_Compare between two different objects
        cJSON_bool case_sensitive = (Size > 4) ? (Data[4] % 2) : 0;
        cJSON_bool compare_result = cJSON_Compare(obj1, obj2, case_sensitive);
        (void)compare_result;
        
        // Test cJSON_IsString on second object
        is_string = cJSON_IsString(obj2);
        (void)is_string;
        
        // Test cJSON_IsInvalid on second object
        is_invalid = cJSON_IsInvalid(obj2);
        (void)is_invalid;
        
        // Test cJSON_IsTrue on second object
        is_true = cJSON_IsTrue(obj2);
        (void)is_true;
        
        cJSON_Delete(obj2);
    }

    // Clean up
    cJSON_Delete(obj1);

    return 0;
}