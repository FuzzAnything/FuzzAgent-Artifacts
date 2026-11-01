#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

static void cleanup(cJSON *root) {
    if (root) cJSON_Delete(root);
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cJSON *obj = cJSON_CreateObject();
    if (!obj) return 0;

    // 1. Test NULL pointers (Line 2087)
    cJSON_AddStringToObject(NULL, "key", "val");
    cJSON_AddStringToObject(obj, NULL, "val");
    cJSON_AddStringToObject(obj, "key", NULL);

    // 2. Test valid path (Line 2100 - cJSON_strdup)
    // If malloc fails in cJSON_strdup, line 2100 is hit.
    cJSON_AddStringToObject(obj, "key1", "value1");

    // 3. Test object == item (Line 2087)
    // We need to pass 'obj' as the item. cJSON_AddStringToObject creates a new item.
    // We must use cJSON_AddItemToObject.
    cJSON_AddItemToObject(obj, "self", obj);

    // 4. Test existing string (Line 2108)
    // Create an item, add it (sets string), then add it again.
    cJSON *item = cJSON_CreateString("test");
    if (item) {
        // First add: item->string is NULL initially.
        // After add_item_to_object, item->string becomes "key1".
        cJSON_AddItemToObject(obj, "key1", item);
        
        // Second add: item->string is "key1" (not NULL).
        // This triggers line 2108: if (!(item->type & cJSON_StringIsConst) && (item->string != NULL))
        cJSON_AddItemToObject(obj, "key2", item);
    }

    cleanup(obj);
    return 0;
}