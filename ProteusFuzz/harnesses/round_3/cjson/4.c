// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateStringReference at cJSON.c:2548:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static void safe_cleanup(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

static cJSON *get_item_safe(cJSON *obj, const char *key) {
    if (!obj) return NULL;
    if (!(obj->type & cJSON_Object)) return NULL;
    return cJSON_GetObjectItem(obj, key);
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Ensure null termination for string parsing
    char *input_buf = (char *)malloc(Size + 1);
    if (!input_buf) {
        return 0;
    }
    memcpy(input_buf, Data, Size);
    input_buf[Size] = '\0';

    // 1. cJSON_Parse
    cJSON *root = cJSON_Parse(input_buf);
    if (!root) {
        free(input_buf);
        return 0;
    }

    // 2. cJSON_CreateString
    // Use a portion of the input as the string content
    size_t str_len = (Size > 10) ? 10 : Size;
    char *str_content = (char *)malloc(str_len + 1);
    if (!str_content) {
        safe_cleanup(root);
        free(input_buf);
        return 0;
    }
    memcpy(str_content, input_buf, str_len);
    str_content[str_len] = '\0';

    cJSON *item_str = cJSON_CreateString(str_content);
    free(str_content);

    // 3. cJSON_CreateStringReference
    // Create a reference to a static or heap string that persists
    // Using a static buffer for reference safety
    static char ref_buffer[256];
    size_t ref_len = (Size > 255) ? 255 : Size;
    memcpy(ref_buffer, input_buf, ref_len);
    ref_buffer[ref_len] = '\0';
    
    cJSON *item_ref = cJSON_CreateStringReference(ref_buffer);

    // 4. cJSON_AddItemToObject (first addition)
    // Ensure root is an object before adding
    if (root->type & cJSON_Object) {
        cJSON_AddItemToObject(root, "key1", item_str);
        // item_str is now owned by root, do not clean up separately
        item_str = NULL; 
    } else {
        // If root is not an object, we must clean up the items we created
        safe_cleanup(item_str);
        item_str = NULL;
    }

    // 5. cJSON_AddItemToObject (second addition)
    if (root->type & cJSON_Object) {
        cJSON_AddItemToObject(root, "key2", item_ref);
        // item_ref is now owned by root
        item_ref = NULL;
    } else {
        // If root is not an object, we must clean up the items we created
        safe_cleanup(item_ref);
        item_ref = NULL;
    }

    // 6. cJSON_SetValuestring
    cJSON *target1 = get_item_safe(root, "key1");
    if (target1) {
        char *new_val1 = (char *)malloc(Size + 1);
        if (new_val1) {
            memcpy(new_val1, input_buf, Size);
            new_val1[Size] = '\0';
            cJSON_SetValuestring(target1, new_val1);
            free(new_val1);
        }
    }

    // 7. cJSON_GetObjectItem
    cJSON *item1 = cJSON_GetObjectItem(root, "key1");

    // 8. cJSON_GetObjectItem
    cJSON *item2 = cJSON_GetObjectItem(root, "key2");

    // 9. cJSON_SetValuestring
    if (item1) {
        char *new_val2 = "modified_value";
        cJSON_SetValuestring(item1, new_val2);
    }

    // 10. cJSON_GetObjectItem
    cJSON *item3 = cJSON_GetObjectItem(root, "key1");

    // 11. cJSON_GetObjectItem
    cJSON *item4 = cJSON_GetObjectItem(root, "key2");

    // 12. cJSON_SetValuestring
    if (item4) {
        // Ensure we don't try to set a reference string if it's flagged
        if (!(item4->type & cJSON_IsReference)) {
            char *new_val3 = "another_mod";
            cJSON_SetValuestring(item4, new_val3);
        }
    }

    // 13. cJSON_GetObjectItem
    cJSON *item5 = cJSON_GetObjectItem(root, "key1");

    // 14. cJSON_GetObjectItem
    cJSON *item6 = cJSON_GetObjectItem(root, "key2");

    // 15. cJSON_Delete
    safe_cleanup(root);

    free(input_buf);
    return 0;
}