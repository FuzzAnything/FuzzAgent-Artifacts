// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static cJSON *create_fuzzed_object(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateObject();
    }
    
    char *str = (char *)malloc(size + 1);
    if (!str) return NULL;
    memcpy(str, data, size);
    str[size] = '\0';
    
    cJSON *obj = cJSON_Parse(str);
    free(str);
    
    if (!obj) {
        obj = cJSON_CreateObject();
    }
    
    return obj;
}

static cJSON *create_fuzzed_item(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateString("default");
    }
    
    char *str = (char *)malloc(size + 1);
    if (!str) return NULL;
    memcpy(str, data, size);
    str[size] = '\0';
    
    cJSON *item = cJSON_Parse(str);
    free(str);
    
    if (!item) {
        item = cJSON_CreateString("fallback_value");
        if (item) {
            char *key_str = strdup("fallback_key");
            if (key_str) {
                item->string = key_str;
            }
        }
    }
    
    return item;
}

static const char *get_fuzzed_key(const uint8_t *data, size_t size) {
    static char key_buffer[256];
    
    if (size == 0) {
        return "default_key";
    }
    
    size_t len = (size < 255) ? size : 255;
    memcpy(key_buffer, data, len);
    key_buffer[len] = '\0';
    
    return key_buffer;
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    size_t part_size = Size / 4;
    const uint8_t *obj_data = Data;
    const uint8_t *item_data = Data + part_size;
    const uint8_t *key_data = Data + 2 * part_size;
    const uint8_t *new_item_data = Data + 3 * part_size;
    
    size_t obj_size = part_size;
    size_t item_size = part_size;
    size_t key_size = part_size;
    size_t new_item_size = Size - 3 * part_size;
    
    cJSON *obj = create_fuzzed_object(obj_data, obj_size);
    cJSON *item = create_fuzzed_item(item_data, item_size);
    cJSON *new_item = create_fuzzed_item(new_item_data, new_item_size);
    const char *key = get_fuzzed_key(key_data, key_size);
    
    if (!obj) {
        cJSON_Delete(item);
        cJSON_Delete(new_item);
        return 0;
    }
    
    // Test 1: cJSON_AddItemReferenceToObject
    cJSON *ref_item = create_fuzzed_item(item_data, item_size);
    if (ref_item) {
        cJSON_AddItemReferenceToObject(obj, key, ref_item);
        // ref_item is now referenced by obj, do not delete separately
    }
    
    // Test 2: cJSON_AddItemToObjectCS
    cJSON *cs_item = create_fuzzed_item(item_data, item_size);
    if (cs_item) {
        cJSON_AddItemToObjectCS(obj, key, cs_item);
    }
    
    // Test 3: cJSON_DeleteItemFromObject
    cJSON_DeleteItemFromObject(obj, key);
    
    // Test 4: cJSON_ReplaceItemInObjectCaseSensitive
    cJSON *replace_item = create_fuzzed_item(new_item_data, new_item_size);
    if (replace_item) {
        cJSON_ReplaceItemInObjectCaseSensitive(obj, key, replace_item);
    }
    
    // Test 5: cJSON_AddItemToObject
    cJSON *add_item = create_fuzzed_item(item_data, item_size);
    if (add_item) {
        cJSON_AddItemToObject(obj, key, add_item);
    }
    
    // Test 6: cJSON_HasObjectItem
    cJSON_bool has_item = cJSON_HasObjectItem(obj, key);
    (void)has_item;
    
    // Cleanup: Deleting obj will recursively delete all children including referenced items
    cJSON_Delete(obj);
    
    return 0;
}