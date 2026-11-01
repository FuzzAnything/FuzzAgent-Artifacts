// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateStringReference at cJSON.c:2548:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

static cJSON *safe_get(cJSON *obj, const char *key) {
    if (!obj) return NULL;
    return cJSON_GetObjectItem(obj, key);
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for parsing
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
        // Parsing failed, try to create a minimal valid object to proceed with the sequence
        root = cJSON_CreateObject();
        if (!root) {
            return 0;
        }
    }

    // 2. cJSON_CreateString
    // Create a safe, null-terminated string buffer to avoid passing raw fuzzer data
    // which might contain embedded nulls or be too short for expected string operations.
    char safe_str[256];
    size_t copy_len = (Size < 255) ? Size : 255;
    memcpy(safe_str, Data, copy_len);
    safe_str[copy_len] = '\0';

    cJSON *item_str = cJSON_CreateString(safe_str);
    
    // 3. cJSON_CreateStringReference
    cJSON *item_ref = cJSON_CreateStringReference(safe_str);

    // 4. cJSON_AddItemToObject (First addition)
    // Add the created string item
    if (item_str) {
        cJSON_AddItemToObject(root, "key_string", item_str);
    }

    // 5. cJSON_AddItemToObject (Second addition)
    // Add the reference item
    if (item_ref) {
        cJSON_AddItemToObject(root, "key_ref", item_ref);
    }

    // 6. cJSON_SetValuestring
    // Try to update the string item
    cJSON *target1 = safe_get(root, "key_string");
    if (target1) {
        // Create a new value to set, ensuring it's null-terminated
        char new_val[64];
        size_t len = (Size < 63) ? Size : 63;
        memcpy(new_val, Data, len);
        new_val[len] = '\0';
        cJSON_SetValuestring(target1, new_val);
    }

    // 7. cJSON_GetObjectItem
    cJSON *check1 = safe_get(root, "key_string");

    // 8. cJSON_GetObjectItem
    cJSON *check2 = safe_get(root, "key_ref");

    // 9. cJSON_SetValuestring
    // Try to update again (might fail if type is reference or null, which is expected behavior)
    if (check1) {
        cJSON_SetValuestring(check1, "updated_value");
    }

    // 10. cJSON_GetObjectItem
    cJSON *check3 = safe_get(root, "key_string");

    // 11. cJSON_GetObjectItem
    cJSON *check4 = safe_get(root, "key_ref");

    // 12. cJSON_SetValuestring
    if (check3) {
        cJSON_SetValuestring(check3, "final_value");
    }

    // 13. cJSON_GetObjectItem
    cJSON *check5 = safe_get(root, "key_string");

    // 14. cJSON_GetObjectItem
    cJSON *check6 = safe_get(root, "key_ref");

    // 15. cJSON_SetValuestring
    if (check5) {
        cJSON_SetValuestring(check5, "last_update");
    }

    // Cleanup
    cJSON_Delete(root);

    return 0;
}