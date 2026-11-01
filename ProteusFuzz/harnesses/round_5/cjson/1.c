// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
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
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create a buffer for JSON string with null terminator
    char *json_str = (char *)malloc(Size + 1);
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    // Parse the JSON input
    cJSON *root = cJSON_Parse(json_str);
    free(json_str);

    if (!root) {
        // If parsing fails, try to create a minimal valid object to test logic paths
        root = cJSON_CreateObject();
        if (!root) {
            return 0;
        }
        cJSON_AddStringToObject(root, "key1", "value1");
        cJSON_AddTrueToObject(root, "key2");
    }

    // 1. cJSON_GetObjectItemCaseSensitive
    cJSON *item1 = cJSON_GetObjectItemCaseSensitive(root, "key1");
    
    // 2. cJSON_IsString
    cJSON_IsString(item1);

    // 3. cJSON_GetObjectItemCaseSensitive
    cJSON *item2 = cJSON_GetObjectItemCaseSensitive(root, "key2");

    // 4. cJSON_IsTrue
    cJSON_IsTrue(item2);

    // 5. cJSON_GetObjectItemCaseSensitive
    cJSON *item3 = cJSON_GetObjectItemCaseSensitive(root, "key3"); // Likely NULL

    // 6. cJSON_GetObjectItemCaseSensitive
    cJSON *item4 = cJSON_GetObjectItemCaseSensitive(root, "key1");

    // 7. cJSON_Duplicate
    cJSON *dup = cJSON_Duplicate(item4, 1); // Deep copy

    // 8. cJSON_GetObjectItemCaseSensitive
    cJSON *item5 = cJSON_GetObjectItemCaseSensitive(dup, "key1");

    // 9. cJSON_GetObjectItemCaseSensitive
    cJSON *item6 = cJSON_GetObjectItemCaseSensitive(dup, "key2");

    // 10. cJSON_Compare
    cJSON_Compare(item4, item5, 1); // Case sensitive comparison
    cJSON_Compare(item4, item6, 0); // Case insensitive comparison

    // 11. cJSON_Delete
    cJSON_Delete(root);
    cJSON_Delete(dup);

    return 0;
}