// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateStringReference at cJSON.c:2548:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void safe_cleanup(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure null termination for parsing
    char *json_str = (char *)malloc(Size + 1);
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    // 1. cJSON_Parse
    cJSON *root = cJSON_Parse(json_str);
    free(json_str);

    if (!root) {
        // If parsing fails, try to create a minimal object to test the rest of the chain
        root = cJSON_CreateObject();
        if (!root) {
            return 0;
        }
    }

    // 2. cJSON_CreateString
    // Create a safe, null-terminated string from input or a default
    char safe_str[256];
    size_t str_len = (Size < 255) ? Size : 255;
    memcpy(safe_str, Data, str_len);
    safe_str[str_len] = '\0';
    
    cJSON *str_node = cJSON_CreateString(safe_str);
    if (!str_node) {
        safe_cleanup(root);
        return 0;
    }

    // 3. cJSON_CreateStringReference
    // Create a reference to a static buffer to ensure it's not freed
    static char static_ref[] = "static_reference";
    cJSON *ref_node = cJSON_CreateStringReference(static_ref);
    if (!ref_node) {
        safe_cleanup(root);
        cJSON_Delete(str_node);
        return 0;
    }

    // 4. cJSON_AddItemToObject (First)
    if (!cJSON_AddItemToObject(root, "key1", str_node)) {
        safe_cleanup(root);
        cJSON_Delete(str_node);
        cJSON_Delete(ref_node);
        return 0;
    }

    // 5. cJSON_AddItemToObject (Second)
    if (!cJSON_AddItemToObject(root, "key2", ref_node)) {
        safe_cleanup(root);
        cJSON_Delete(str_node);
        cJSON_Delete(ref_node);
        return 0;
    }

    // 6. cJSON_SetValuestring
    // Try to set value on the string node we just added (key1)
    cJSON *item1 = cJSON_GetObjectItem(root, "key1");
    if (item1 && item1->type == cJSON_String) {
        // Ensure the new value is null-terminated and safe
        char new_val_buf[256];
        size_t val_len = (Size < 255) ? Size : 255;
        memcpy(new_val_buf, Data, val_len);
        new_val_buf[val_len] = '\0';
        cJSON_SetValuestring(item1, new_val_buf);
    }

    // 7. cJSON_GetObjectItem
    cJSON *get1 = cJSON_GetObjectItem(root, "key1");

    // 8. cJSON_GetObjectItem
    cJSON *get2 = cJSON_GetObjectItem(root, "key2");

    // 9. cJSON_SetValuestring
    // Try to set value on get1 if it's a string
    if (get1 && get1->type == cJSON_String) {
        const char *val9 = "updated_val_9";
        cJSON_SetValuestring(get1, val9);
    }

    // 10. cJSON_GetObjectItem
    cJSON *get3 = cJSON_GetObjectItem(root, "key1");

    // 11. cJSON_GetObjectItem
    cJSON *get4 = cJSON_GetObjectItem(root, "key2");

    // 12. cJSON_SetValuestring
    // Try to set value on get3
    if (get3 && get3->type == cJSON_String) {
        const char *val12 = "updated_val_12";
        cJSON_SetValuestring(get3, val12);
    }

    // 13. cJSON_GetObjectItem
    cJSON *get5 = cJSON_GetObjectItem(root, "key1");

    // 14. cJSON_GetObjectItem
    cJSON *get6 = cJSON_GetObjectItem(root, "key2");

    // 15. cJSON_SetValuestring
    // Try to set value on get5
    if (get5 && get5->type == cJSON_String) {
        const char *val15 = "updated_val_15";
        cJSON_SetValuestring(get5, val15);
    }

    // 16. cJSON_Delete
    safe_cleanup(root);

    return 0;
}