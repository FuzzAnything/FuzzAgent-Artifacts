// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    // Create various cJSON items for testing
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *true_item = cJSON_CreateTrue();
    
    // Create string item from fuzzer data
    char *string_data = NULL;
    if (Size > 0) {
        string_data = (char*)malloc(Size + 1);
        if (string_data) {
            memcpy(string_data, Data, Size);
            string_data[Size] = '\0';
        }
    }
    
    cJSON *string_item = NULL;
    if (string_data) {
        string_item = cJSON_CreateString(string_data);
    }
    
    // Create raw item
    cJSON *raw_item = NULL;
    if (string_data) {
        raw_item = cJSON_CreateRaw(string_data);
    }
    
    // Create number item
    cJSON *number_item = cJSON_CreateNumber(42.5);
    
    // Create array item
    cJSON *array_item = cJSON_CreateArray();
    if (array_item) {
        cJSON_AddItemToArray(array_item, cJSON_CreateNumber(1));
        cJSON_AddItemToArray(array_item, cJSON_CreateString("test"));
    }
    
    // Create object item
    cJSON *object_item = cJSON_CreateObject();
    if (object_item) {
        cJSON_AddItemToObject(object_item, "key1", cJSON_CreateString("value1"));
        cJSON_AddItemToObject(object_item, "key2", cJSON_CreateNumber(2));
    }
    
    // Test cJSON_IsTrue on various items
    (void)cJSON_IsTrue(false_item);
    (void)cJSON_IsTrue(true_item);
    (void)cJSON_IsTrue(string_item);
    (void)cJSON_IsTrue(raw_item);
    (void)cJSON_IsTrue(number_item);
    (void)cJSON_IsTrue(array_item);
    (void)cJSON_IsTrue(object_item);
    (void)cJSON_IsTrue(NULL);
    
    // Test cJSON_CreateFalse (already created above, verify it's not NULL)
    if (false_item) {
        // Use the false_item in other tests
        (void)cJSON_IsTrue(false_item);
    }
    
    // Test cJSON_Duplicate on various items with different recurse values
    cJSON *dup_false = cJSON_Duplicate(false_item, 0);
    cJSON *dup_false_recursive = cJSON_Duplicate(false_item, 1);
    
    cJSON *dup_string = cJSON_Duplicate(string_item, 0);
    cJSON *dup_string_recursive = cJSON_Duplicate(string_item, 1);
    
    cJSON *dup_array = cJSON_Duplicate(array_item, 0);
    cJSON *dup_array_recursive = cJSON_Duplicate(array_item, 1);
    
    cJSON *dup_object = cJSON_Duplicate(object_item, 0);
    cJSON *dup_object_recursive = cJSON_Duplicate(object_item, 1);
    
    // Test with NULL input
    cJSON *dup_null = cJSON_Duplicate(NULL, 0);
    cJSON *dup_null_recursive = cJSON_Duplicate(NULL, 1);
    
    // Test cJSON_IsRaw on various items
    (void)cJSON_IsRaw(false_item);
    (void)cJSON_IsRaw(true_item);
    (void)cJSON_IsRaw(string_item);
    (void)cJSON_IsRaw(raw_item);
    (void)cJSON_IsRaw(number_item);
    (void)cJSON_IsRaw(array_item);
    (void)cJSON_IsRaw(object_item);
    (void)cJSON_IsRaw(NULL);
    
    // Test cJSON_IsString on various items
    (void)cJSON_IsString(false_item);
    (void)cJSON_IsString(true_item);
    (void)cJSON_IsString(string_item);
    (void)cJSON_IsString(raw_item);
    (void)cJSON_IsString(number_item);
    (void)cJSON_IsString(array_item);
    (void)cJSON_IsString(object_item);
    (void)cJSON_IsString(NULL);
    
    // Test cJSON_Compare with various combinations
    // Compare items with themselves
    (void)cJSON_Compare(false_item, false_item, 0);
    (void)cJSON_Compare(false_item, false_item, 1);
    
    (void)cJSON_Compare(string_item, string_item, 0);
    (void)cJSON_Compare(string_item, string_item, 1);
    
    (void)cJSON_Compare(array_item, array_item, 0);
    (void)cJSON_Compare(array_item, array_item, 1);
    
    (void)cJSON_Compare(object_item, object_item, 0);
    (void)cJSON_Compare(object_item, object_item, 1);
    
    // Compare different items
    (void)cJSON_Compare(false_item, true_item, 0);
    (void)cJSON_Compare(false_item, string_item, 1);
    (void)cJSON_Compare(string_item, raw_item, 0);
    (void)cJSON_Compare(array_item, object_item, 1);
    
    // Compare with duplicates
    if (dup_false) {
        (void)cJSON_Compare(false_item, dup_false, 0);
        (void)cJSON_Compare(false_item, dup_false, 1);
    }
    
    if (dup_array && dup_array_recursive) {
        (void)cJSON_Compare(dup_array, dup_array_recursive, 0);
        (void)cJSON_Compare(array_item, dup_array_recursive, 1);
    }
    
    if (dup_object && dup_object_recursive) {
        (void)cJSON_Compare(dup_object, dup_object_recursive, 0);
        (void)cJSON_Compare(object_item, dup_object_recursive, 1);
    }
    
    // Test with NULL inputs
    (void)cJSON_Compare(NULL, false_item, 0);
    (void)cJSON_Compare(string_item, NULL, 1);
    (void)cJSON_Compare(NULL, NULL, 0);
    
    // Clean up
    cJSON_Delete(false_item);
    cJSON_Delete(true_item);
    cJSON_Delete(string_item);
    cJSON_Delete(raw_item);
    cJSON_Delete(number_item);
    cJSON_Delete(array_item);
    cJSON_Delete(object_item);
    
    cJSON_Delete(dup_false);
    cJSON_Delete(dup_false_recursive);
    cJSON_Delete(dup_string);
    cJSON_Delete(dup_string_recursive);
    cJSON_Delete(dup_array);
    cJSON_Delete(dup_array_recursive);
    cJSON_Delete(dup_object);
    cJSON_Delete(dup_object_recursive);
    cJSON_Delete(dup_null);
    cJSON_Delete(dup_null_recursive);
    
    free(string_data);
    
    return 0;
}