// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddFalseToObject at cJSON.c:2174:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

/* Helper to create a simple JSON object from fuzz input if possible, or a default one */
static cJSON *create_test_object(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateObject();
    }
    
    /* Try to parse input as JSON to create a complex object */
    cJSON *obj = cJSON_ParseWithLength((const char *)data, size);
    if (obj && cJSON_IsObject(obj)) {
        return obj;
    }
    
    /* If parsing failed or not an object, clean up and create a new empty object */
    if (obj) {
        cJSON_Delete(obj);
    }
    return cJSON_CreateObject();
}

/* Helper to create a new item from fuzz input */
static cJSON *create_test_item(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateNumber(0);
    }
    
    /* Try to parse input as JSON */
    cJSON *item = cJSON_ParseWithLength((const char *)data, size);
    if (item) {
        return item;
    }
    
    /* Fallback to creating a number */
    return cJSON_CreateNumber(0);
}

/* Helper to extract a key string from fuzz input */
static char *create_test_key(const uint8_t *data, size_t size) {
    if (size == 0) {
        char *key = malloc(2);
        if (key) {
            key[0] = 'k';
            key[1] = '\0';
        }
        return key;
    }
    
    /* Ensure null termination */
    char *key = malloc(size + 1);
    if (!key) return NULL;
    
    memcpy(key, data, size);
    key[size] = '\0';
    
    return key;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    /* Split input into parts for different parameters */
    size_t part_size = Size / 4;
    const uint8_t *obj_data = Data;
    const uint8_t *item_data = Data + part_size;
    const uint8_t *key_data = Data + 2 * part_size;
    const uint8_t *extra_data = Data + 3 * part_size;
    
    size_t obj_len = part_size;
    size_t item_len = part_size;
    size_t key_len = part_size;
    size_t extra_len = Size - 3 * part_size;
    
    /* Create test object */
    cJSON *obj = create_test_object(obj_data, obj_len);
    if (!obj) {
        return 0;
    }
    
    /* Create test key */
    char *key = create_test_key(key_data, key_len);
    if (!key) {
        cJSON_Delete(obj);
        return 0;
    }
    
    /* Create test item */
    cJSON *new_item = create_test_item(item_data, item_len);
    if (!new_item) {
        free(key);
        cJSON_Delete(obj);
        return 0;
    }
    
    /* Test cJSON_AddObjectToObject */
    cJSON *nested_obj = cJSON_AddObjectToObject(obj, key);
    if (nested_obj) {
        /* Add some content to nested object */
        cJSON_AddNumberToObject(nested_obj, "nested_key", 42);
    }
    
    /* Test cJSON_AddBoolToObject */
    cJSON_bool bool_val = (extra_len > 0 && extra_data[0] % 2 == 0) ? cJSON_True : cJSON_False;
    cJSON *bool_item = cJSON_AddBoolToObject(obj, "bool_key", bool_val);
    if (bool_item) {
        /* Successfully added */
    }
    
    /* Test cJSON_AddFalseToObject */
    cJSON *false_item = cJSON_AddFalseToObject(obj, "false_key");
    if (false_item) {
        /* Successfully added */
    }
    
    /* Test cJSON_ReplaceItemInObjectCaseSensitive */
    cJSON *replacement_item = create_test_item(extra_data, extra_len);
    if (replacement_item) {
        cJSON_ReplaceItemInObjectCaseSensitive(obj, "bool_key", replacement_item);
        /* replacement_item ownership transferred to obj on success, so do not delete here */
    } else {
        /* If creation failed, we don't have an item to replace with, but we must clean up new_item */
        cJSON_Delete(new_item);
        new_item = NULL;
    }
    
    /* Test cJSON_ReplaceItemInObject */
    cJSON *replacement_item2 = create_test_item(extra_data, extra_len);
    if (replacement_item2) {
        cJSON_ReplaceItemInObject(obj, "bool_key", replacement_item2);
        /* replacement_item2 ownership transferred to obj on success */
    } else {
        /* If creation failed, ensure new_item is cleaned up if it wasn't used above */
        if (new_item) {
            cJSON_Delete(new_item);
            new_item = NULL;
        }
    }
    
    /* Test cJSON_DeleteItemFromObject */
    cJSON_DeleteItemFromObject(obj, "false_key");
    
    /* Cleanup remaining resources */
    free(key);
    
    /* If new_item was not successfully replaced into the object, delete it manually */
    if (new_item) {
        cJSON_Delete(new_item);
    }
    
    cJSON_Delete(obj);
    
    return 0;
}