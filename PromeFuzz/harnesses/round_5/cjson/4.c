// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Create root objects
    cJSON *root_obj = cJSON_CreateObject();
    cJSON *root_arr = cJSON_CreateArray();
    
    if (!root_obj || !root_arr) {
        if (root_obj) cJSON_Delete(root_obj);
        if (root_arr) cJSON_Delete(root_arr);
        return 0;
    }
    
    // Create some items to work with
    cJSON *item1 = cJSON_CreateString("test1");
    cJSON *item2 = cJSON_CreateNumber(42);
    cJSON *item3 = cJSON_CreateObject();
    cJSON *item4 = cJSON_CreateArray();
    cJSON *item5 = cJSON_CreateString("test5");
    
    // Create string keys from fuzzer data
    char key1[32] = {0};
    char key2[32] = {0};
    char key3[32] = {0};
    char key4[32] = {0};
    char key5[32] = {0};
    
    size_t key_len = Size > 32 ? 32 : Size;
    if (Size > 0) {
        memcpy(key1, Data, key_len);
        if (Size > key_len) memcpy(key2, Data + key_len, key_len);
        if (Size > 2*key_len) memcpy(key3, Data + 2*key_len, key_len);
        if (Size > 3*key_len) memcpy(key4, Data + 3*key_len, key_len);
        if (Size > 4*key_len) memcpy(key5, Data + 4*key_len, key_len);
    } else {
        strcpy(key1, "key1");
        strcpy(key2, "key2");
        strcpy(key3, "key3");
        strcpy(key4, "key4");
        strcpy(key5, "key5");
    }
    
    // Ensure null termination
    key1[31] = '\0';
    key2[31] = '\0';
    key3[31] = '\0';
    key4[31] = '\0';
    key5[31] = '\0';
    
    // 1-3: cJSON_AddItemToObject
    cJSON_AddItemToObject(root_obj, key1, item1);
    cJSON_AddItemToObject(root_obj, key2, item2);
    cJSON_AddItemToObject(root_obj, key3, item3);
    
    // 4-5: cJSON_AddItemToObjectCS (using const strings)
    cJSON_AddItemToObjectCS(root_obj, "const_key1", item4);
    cJSON_AddItemToObjectCS(root_obj, "const_key2", item5);
    
    // Create more items for reference operations
    cJSON *ref_item1 = cJSON_CreateString("ref1");
    cJSON *ref_item2 = cJSON_CreateNumber(99);
    cJSON *ref_item3 = cJSON_CreateBool(1);
    
    // Add items to array for reference operations
    cJSON_AddItemToArray(root_arr, ref_item1);
    cJSON_AddItemToArray(root_arr, ref_item2);
    
    // 6-7: cJSON_AddItemReferenceToArray
    cJSON_AddItemReferenceToArray(root_arr, ref_item1);
    cJSON_AddItemReferenceToArray(root_arr, ref_item2);
    
    // 8-10: cJSON_AddItemReferenceToObject
    cJSON_AddItemReferenceToObject(root_obj, "ref_key1", ref_item1);
    cJSON_AddItemReferenceToObject(root_obj, "ref_key2", ref_item2);
    cJSON_AddItemReferenceToObject(root_obj, "ref_key3", ref_item3);
    
    // 11: cJSON_DeleteItemFromArray
    if (cJSON_GetArraySize(root_arr) > 0) {
        cJSON_DeleteItemFromArray(root_arr, 0);
    }
    
    // 12-13: cJSON_DeleteItemFromObject
    cJSON_DeleteItemFromObject(root_obj, key1);
    cJSON_DeleteItemFromObject(root_obj, key2);
    
    // 14-15: cJSON_DeleteItemFromObjectCaseSensitive
    cJSON_DeleteItemFromObjectCaseSensitive(root_obj, "const_key1");
    cJSON_DeleteItemFromObjectCaseSensitive(root_obj, "const_key2");
    
    // 16: cJSON_Minify
    // Create a mutable string from fuzzer data for minify
    char *minify_str = malloc(Size + 1);
    if (minify_str) {
        if (Size > 0) {
            memcpy(minify_str, Data, Size);
        }
        minify_str[Size] = '\0';
        cJSON_Minify(minify_str);
        free(minify_str);
    }
    
    // 17-19: cJSON_Delete
    cJSON_Delete(root_obj);
    cJSON_Delete(root_arr);
    
    // Create and delete an extra item for the third delete call
    cJSON *extra_item = cJSON_CreateNull();
    cJSON_Delete(extra_item);
    
    return 0;
}