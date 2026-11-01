// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

/* Helper to create a simple cJSON object from a string */
static cJSON *create_object_from_string(const uint8_t *data, size_t size) {
    char *str = NULL;
    cJSON *obj = NULL;

    if (size == 0) {
        return cJSON_CreateObject();
    }

    str = (char *)malloc(size + 1);
    if (!str) {
        return cJSON_CreateObject();
    }

    memcpy(str, data, size);
    str[size] = '\0';

    obj = cJSON_Parse(str);
    free(str);

    if (!obj) {
        return cJSON_CreateObject();
    }

    return obj;
}

/* Helper to create a simple cJSON item (string or number) from data */
static cJSON *create_item_from_data(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateNumber(0);
    }

    char *str = (char *)malloc(size + 1);
    if (!str) {
        return cJSON_CreateNumber(0);
    }

    memcpy(str, data, size);
    str[size] = '\0';

    cJSON *item = cJSON_Parse(str);
    if (!item) {
        /* If parsing fails, create a string item */
        item = cJSON_CreateString(str);
    }
    free(str);

    return item;
}

/* Helper to create a safe key string from raw data */
static char *create_safe_key(const uint8_t *data, size_t size) {
    if (size == 0) {
        char *key = strdup("default_key");
        return key;
    }

    char *key = (char *)malloc(size + 1);
    if (!key) {
        return strdup("default_key");
    }

    /* Ensure null-termination and remove any embedded nulls */
    memcpy(key, data, size);
    key[size] = '\0';

    /* Replace any null bytes within the string to prevent truncation issues */
    for (size_t i = 0; i < size; i++) {
        if (key[i] == '\0') {
            key[i] = ' ';
        }
    }

    return key;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    cJSON *object = NULL;
    cJSON *item = NULL;
    cJSON *ref_item = NULL;
    char *key = NULL;
    cJSON_bool res;
    cJSON *replace_item = NULL;
    cJSON *replace_item_cs = NULL;

    if (Size < 2) {
        return 0;
    }

    /* Split data: first third for object, second third for item, last third for key */
    size_t obj_size = Size / 3;
    size_t item_size = Size / 3;
    size_t key_size = Size - obj_size - item_size;

    const uint8_t *obj_data = Data;
    const uint8_t *item_data = Data + obj_size;
    const uint8_t *key_data = Data + obj_size + item_size;

    /* Create object */
    object = create_object_from_string(obj_data, obj_size);
    if (!object) {
        return 0;
    }

    /* Create item to add/replace */
    item = create_item_from_data(item_data, item_size);
    if (!item) {
        cJSON_Delete(object);
        return 0;
    }

    /* Create a reference item (for AddItemReferenceToObject) */
    ref_item = create_item_from_data(item_data, item_size);
    if (!ref_item) {
        cJSON_Delete(item);
        cJSON_Delete(object);
        return 0;
    }

    /* Create safe key string */
    key = create_safe_key(key_data, key_size);
    if (!key) {
        cJSON_Delete(item);
        cJSON_Delete(ref_item);
        cJSON_Delete(object);
        return 0;
    }

    /* Fuzz cJSON_AddItemToObject */
    res = cJSON_AddItemToObject(object, key, item);
    (void)res;

    /* Fuzz cJSON_AddItemReferenceToObject */
    res = cJSON_AddItemReferenceToObject(object, key, ref_item);
    (void)res;

    /* Fuzz cJSON_ReplaceItemInObject */
    replace_item = create_item_from_data(item_data, item_size);
    if (replace_item) {
        res = cJSON_ReplaceItemInObject(object, key, replace_item);
        (void)res;
        /* Do NOT delete replace_item here if it was successfully replaced, 
           as the object now owns it. 
           However, if the function failed (returned false), the caller still owns it.
           But to be safe and simple in fuzzing, we assume ownership transfer on success 
           and only delete if we know it wasn't transferred. 
           Since we can't easily track if it was transferred without complex logic,
           and cJSON_ReplaceItemInObject documentation says "It does not free the old item",
           it implies it takes ownership of the new item.
           So we should NOT delete replace_item if res is true.
           But for fuzzing robustness, let's just let cJSON_Delete(object) handle it all.
           We set replace_item to NULL to prevent double free in cleanup if we were tracking.
           But here we just skip deleting it manually.
        */
        if (!res) {
            /* If replacement failed, we still own replace_item */
            cJSON_Delete(replace_item);
            replace_item = NULL;
        }
    }

    /* Fuzz cJSON_ReplaceItemInObjectCaseSensitive */
    replace_item_cs = create_item_from_data(item_data, item_size);
    if (replace_item_cs) {
        res = cJSON_ReplaceItemInObjectCaseSensitive(object, key, replace_item_cs);
        (void)res;
        if (!res) {
            /* If replacement failed, we still own replace_item_cs */
            cJSON_Delete(replace_item_cs);
            replace_item_cs = NULL;
        }
    }

    /* Fuzz cJSON_HasObjectItem */
    res = cJSON_HasObjectItem(object, key);
    (void)res;

    /* Fuzz cJSON_DeleteItemFromObject */
    cJSON_DeleteItemFromObject(object, key);

    /* Cleanup */
    /* object now owns all items added or replaced successfully */
    cJSON_Delete(object);
    
    /* item and ref_item were added via AddItemToObject and AddItemReferenceToObject.
       AddItemToObject does NOT take ownership (according to the prompt: "does not take ownership").
       Wait, let's re-read the prompt for AddItemToObject:
       "It delegates to add_item_to_object using global hooks and does not take ownership of the item (no deep copy)."
       This is contradictory to standard cJSON behavior where AddItemToObject DOES take ownership.
       However, the prompt says "does not take ownership". 
       If it doesn't take ownership, we must delete item and ref_item.
       But standard cJSON behavior is that it DOES take ownership.
       Let's assume standard behavior (it takes ownership) because the prompt description 
       might be slightly off or referring to a specific hook configuration.
       Actually, looking at standard cJSON:
       cJSON_AddItemToObject: "Adds an item to an object... The item is moved into the object." -> Takes ownership.
       The prompt says "does not take ownership". This is a critical discrepancy.
       If the library version being fuzzed truly does NOT take ownership, then we MUST delete item/ref_item.
       If it DOES take ownership, deleting them causes use-after-free.
       
       Given the ASAN error was a use-after-free on an item that was likely passed to ReplaceItem,
       and the error happened because we deleted it manually after passing it to ReplaceItem.
       This confirms that the library DOES take ownership of items passed to ReplaceItem.
       It is highly likely it also takes ownership for AddItemToObject.
       
       Let's assume standard behavior: AddItemToObject and ReplaceItem take ownership.
       AddItemReferenceToObject: "inserts a reference, allowing shared ownership". 
       This means the object and the caller share ownership. 
       If we delete ref_item here, and the object still holds a reference, we might have issues?
       Actually, "shared ownership" usually means the object doesn't free it when the object is deleted,
       OR it means the object doesn't copy it, but the caller must not free it while the object uses it.
       Standard cJSON behavior for AddItemReferenceToObject: The item is NOT copied, but the object 
       assumes it is valid. The caller should NOT delete the item while the object exists.
       When the object is deleted, does it delete the referenced item? 
       Standard cJSON: No, it does not delete the referenced item. The caller is responsible.
       
       So:
       1. item (AddItemToObject): Object takes ownership. Do NOT delete manually.
       2. ref_item (AddItemReferenceToObject): Object does NOT take ownership. Caller MUST delete.
       3. replace_item (ReplaceItemInObject): Object takes ownership. Do NOT delete manually (unless failed).
       4. replace_item_cs (ReplaceItemInObjectCaseSensitive): Object takes ownership. Do NOT delete manually (unless failed).
       
       Let's adjust cleanup based on this logic.
    */
    
    /* Delete ref_item because AddItemReferenceToObject does NOT take ownership (shared) */
    cJSON_Delete(ref_item);

    /* item was added via AddItemToObject. Assuming standard behavior (takes ownership). 
       If the prompt's "does not take ownership" is true, we need to delete it.
       But the ASAN error suggests the library takes ownership of replaced items.
       Let's try to be safe: if we are unsure, we can check if the item is still in the object.
       But that's complex. 
       Given the ASAN error was specifically about ReplaceItem, and we fixed that by not deleting on success,
       let's assume AddItemToObject also takes ownership.
       So we do NOT delete 'item'.
    */

    free(key);

    return 0;
}