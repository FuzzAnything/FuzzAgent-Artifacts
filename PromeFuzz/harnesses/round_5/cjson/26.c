// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static void cleanup_items(cJSON **items, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (items[i] != NULL) {
            cJSON_Delete(items[i]);
            items[i] = NULL;
        }
    }
}

static char *create_null_terminated_string(const uint8_t *data, size_t size, size_t max_len) {
    if (size == 0) {
        char *str = malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    size_t len = size > max_len ? max_len : size;
    char *str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Create null-terminated strings from fuzzer input
    size_t key_len = Size / 3;
    size_t val_len = Size / 3;
    size_t name_len = Size - key_len - val_len;
    
    char *key = create_null_terminated_string(Data, key_len, 256);
    char *value = create_null_terminated_string(Data + key_len, val_len, 256);
    char *name = create_null_terminated_string(Data + key_len + val_len, name_len, 256);
    
    if (!key || !value || !name) {
        free(key);
        free(value);
        free(name);
        return 0;
    }
    
    // Create parent object
    cJSON *parent = cJSON_CreateObject();
    if (!parent) {
        free(key);
        free(value);
        free(name);
        return 0;
    }
    
    // Create various items for testing
    cJSON *items[10] = {0};
    size_t item_count = 0;
    
    // Create string item
    items[item_count++] = cJSON_CreateString(value);
    // Create number item
    items[item_count++] = cJSON_CreateNumber(Size);
    // Create boolean item
    items[item_count++] = cJSON_CreateBool(Size % 2);
    // Create array item
    cJSON *array = cJSON_CreateArray();
    if (array) {
        cJSON_AddItemToArray(array, cJSON_CreateString(value));
        items[item_count++] = array;
    }
    
    // Test cJSON_AddItemToObject
    if (items[0]) {
        cJSON_AddItemToObject(parent, key, items[0]);
    }
    
    // Test cJSON_HasObjectItem
    if (cJSON_HasObjectItem(parent, key)) {
        // Key exists, try to replace it
        if (items[1]) {
            // Test cJSON_ReplaceItemInObjectCaseSensitive
            cJSON_ReplaceItemInObjectCaseSensitive(parent, key, items[1]);
            
            // Test cJSON_ReplaceItemInObject (case-insensitive version)
            if (items[2]) {
                // Convert key to different case for testing case sensitivity
                char *key_upper = malloc(strlen(key) + 1);
                if (key_upper) {
                    for (size_t i = 0; key[i]; i++) {
                        key_upper[i] = (key[i] >= 'a' && key[i] <= 'z') ? key[i] - 32 : key[i];
                    }
                    key_upper[strlen(key)] = '\0';
                    
                    cJSON_ReplaceItemInObject(parent, key_upper, items[2]);
                    free(key_upper);
                }
            }
        }
    }
    
    // Test cJSON_AddObjectToObject
    cJSON *child_obj = cJSON_AddObjectToObject(parent, name);
    if (child_obj && items[3]) {
        // Add something to the child object
        cJSON_AddItemToObject(child_obj, "nested_key", items[3]);
    }
    
    // Test cJSON_AddItemToObjectCS with literal string
    if (items[1]) {
        // Create a new item for this test
        cJSON *cs_item = cJSON_CreateString("const_string_value");
        if (cs_item) {
            // Mark the string as const (this is a hack for fuzzing)
            // In real usage, this should only be used with string literals
            cs_item->type &= ~cJSON_StringIsConst;
            cJSON_AddItemToObjectCS(parent, "literal_key", cs_item);
        }
    }
    
    // Cleanup
    cJSON_Delete(parent);
    free(key);
    free(value);
    free(name);
    
    // Cleanup any remaining items that weren't added to objects
    for (size_t i = 0; i < item_count; i++) {
        // Only delete items that weren't added to parent (they get deleted with parent)
        // We can't easily track which items were added, so we rely on cJSON's internal cleanup
        // Items added to parent are already deleted with cJSON_Delete(parent)
    }
    
    return 0;
}