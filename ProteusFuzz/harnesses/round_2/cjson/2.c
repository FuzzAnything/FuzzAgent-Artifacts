// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static void safe_cleanup(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Convert input to a null-terminated string for parsing
    char *json_str = (char *)malloc(Size + 1);
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    // Parse the JSON string
    cJSON *root = cJSON_Parse(json_str);
    free(json_str);

    if (!root) {
        return 0;
    }

    // Ensure we have an object to query
    if (!cJSON_IsObject(root)) {
        safe_cleanup(root);
        return 0;
    }

    // 1. cJSON_GetObjectItemCaseSensitive
    cJSON *item1 = cJSON_GetObjectItemCaseSensitive(root, "key1");

    // 2. cJSON_IsString
    cJSON_bool is_str = cJSON_IsString(item1);
    (void)is_str; // Suppress unused variable warning

    // 3. cJSON_GetObjectItemCaseSensitive
    cJSON *item2 = cJSON_GetObjectItemCaseSensitive(root, "key2");

    // 4. cJSON_IsTrue
    cJSON_bool is_true = cJSON_IsTrue(item2);
    (void)is_true; // Suppress unused variable warning

    // 5. cJSON_GetObjectItemCaseSensitive
    cJSON *item3 = cJSON_GetObjectItemCaseSensitive(root, "key3");

    // 6. cJSON_GetObjectItemCaseSensitive
    cJSON *item4 = cJSON_GetObjectItemCaseSensitive(root, "key4");

    // 7. cJSON_Duplicate
    // Duplicate item3 if it exists, otherwise duplicate root
    cJSON *dup_item = cJSON_Duplicate(item3 ? item3 : root, 1);

    // 8. cJSON_GetObjectItemCaseSensitive
    cJSON *item5 = cJSON_GetObjectItemCaseSensitive(root, "key5");

    // 9. cJSON_GetObjectItemCaseSensitive
    cJSON *item6 = cJSON_GetObjectItemCaseSensitive(root, "key6");

    // 10. cJSON_Compare
    // Compare root with duplicate if available, else compare item5 and item6
    cJSON_bool compare_result = cJSON_Compare(
        dup_item ? dup_item : root,
        dup_item ? dup_item : root,
        1 // case_sensitive
    );
    (void)compare_result;

    // 11. cJSON_Delete
    // Clean up the duplicate first
    if (dup_item) {
        cJSON_Delete(dup_item);
    }
    // Then clean up the root
    safe_cleanup(root);

    return 0;
}