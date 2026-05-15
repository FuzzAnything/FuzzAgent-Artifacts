// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
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
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
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

static void fuzz_cjson_operations(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return;
    
    // Create some items to work with
    cJSON *item1 = cJSON_CreateNumber(42.5);
    cJSON *item2 = cJSON_CreateString("test");
    cJSON *item3 = cJSON_CreateBool(1);
    cJSON *item4 = cJSON_CreateNull();
    
    if (!item1 || !item2 || !item3 || !item4) {
        cJSON_Delete(root);
        if (item1) cJSON_Delete(item1);
        if (item2) cJSON_Delete(item2);
        if (item3) cJSON_Delete(item3);
        if (item4) cJSON_Delete(item4);
        return;
    }
    
    // Extract key strings from fuzzing data
    // Use first part for key1, second part for key2
    size_t half_size = size / 2;
    char *key1 = (char*)malloc(half_size + 1);
    char *key2 = (char*)malloc(half_size + 1);
    
    if (!key1 || !key2) {
        free(key1);
        free(key2);
        cJSON_Delete(root);
        cJSON_Delete(item1);
        cJSON_Delete(item2);
        cJSON_Delete(item3);
        cJSON_Delete(item4);
        return;
    }
    
    memcpy(key1, data, half_size);
    memcpy(key2, data + half_size, size - half_size);
    key1[half_size] = '\0';
    key2[size - half_size] = '\0';
    
    // Test cJSON_AddItemToObject
    cJSON_AddItemToObject(root, key1, item1);
    
    // Test cJSON_AddItemToObjectCS (with const string literal)
    cJSON_AddItemToObjectCS(root, "const_key", item2);
    
    // Test cJSON_AddObjectToObject
    cJSON *nested_obj = cJSON_AddObjectToObject(root, key2);
    if (nested_obj) {
        cJSON_AddItemToObject(nested_obj, "nested_key", item3);
    }
    
    // Test cJSON_ReplaceItemInObject
    cJSON *new_item1 = cJSON_CreateNumber(99.9);
    if (new_item1) {
        cJSON_ReplaceItemInObject(root, key1, new_item1);
    }
    
    // Test cJSON_ReplaceItemInObjectCaseSensitive
    cJSON *new_item2 = cJSON_CreateString("replaced");
    if (new_item2) {
        cJSON_ReplaceItemInObjectCaseSensitive(root, "const_key", new_item2);
    }
    
    // Test cJSON_DeleteItemFromObject
    cJSON_DeleteItemFromObject(root, key1);
    cJSON_DeleteItemFromObject(root, "const_key");
    cJSON_DeleteItemFromObject(root, key2);
    
    // Cleanup
    free(key1);
    free(key2);
    cJSON_Delete(root);
    // item4 was not used, delete it
    cJSON_Delete(item4);
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    // Initialize with dummy data if too small
    if (Size == 0) {
        static const uint8_t dummy[] = {0x41, 0x42, 0x43, 0x44};
        fuzz_cjson_operations(dummy, sizeof(dummy));
        return 0;
    }
    
    fuzz_cjson_operations(Data, Size);
    return 0;
}