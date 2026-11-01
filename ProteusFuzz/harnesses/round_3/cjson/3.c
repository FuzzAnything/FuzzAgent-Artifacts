// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

static void safe_cleanup(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for cJSON_Parse
    // We create a buffer with size + 1 to add the null terminator safely
    char *buffer = (char *)malloc(Size + 1);
    if (!buffer) {
        return 0;
    }
    
    memcpy(buffer, Data, Size);
    buffer[Size] = '\0';

    cJSON *root = cJSON_Parse(buffer);
    free(buffer);

    if (!root) {
        return 0;
    }

    // Ensure root is an object to safely call GetObjectItemCaseSensitive
    if (!cJSON_IsObject(root)) {
        cJSON_Delete(root);
        return 0;
    }

    // 1. cJSON_GetObjectItemCaseSensitive
    cJSON *item1 = cJSON_GetObjectItemCaseSensitive(root, "key1");
    
    // 2. cJSON_IsString
    if (item1) {
        cJSON_IsString(item1);
    }

    // 3. cJSON_GetObjectItemCaseSensitive
    cJSON *item2 = cJSON_GetObjectItemCaseSensitive(root, "key2");

    // 4. cJSON_IsTrue
    if (item2) {
        cJSON_IsTrue(item2);
    }

    // 5. cJSON_GetObjectItemCaseSensitive
    cJSON *item3 = cJSON_GetObjectItemCaseSensitive(root, "key3");

    // 6. cJSON_GetObjectItemCaseSensitive
    cJSON *item4 = cJSON_GetObjectItemCaseSensitive(root, "key4");

    // 7. cJSON_Duplicate
    // We duplicate item3 if it exists, otherwise item1, otherwise root
    cJSON *dup = NULL;
    if (item3) {
        dup = cJSON_Duplicate(item3, 1);
    } else if (item1) {
        dup = cJSON_Duplicate(item1, 1);
    } else {
        dup = cJSON_Duplicate(root, 1);
    }

    // 8. cJSON_GetObjectItemCaseSensitive
    cJSON *item5 = NULL;
    if (dup && cJSON_IsObject(dup)) {
        item5 = cJSON_GetObjectItemCaseSensitive(dup, "key5");
    }

    // 9. cJSON_GetObjectItemCaseSensitive
    cJSON *item6 = NULL;
    if (dup && cJSON_IsObject(dup)) {
        item6 = cJSON_GetObjectItemCaseSensitive(dup, "key6");
    }

    // 10. cJSON_Compare
    // Compare dup with root or one of the items found
    if (dup) {
        cJSON_Compare(dup, root, 1);
        if (item1) {
            cJSON_Compare(dup, item1, 1);
        }
        if (item5) {
            cJSON_Compare(item5, item6, 1);
        }
    }

    // 11. cJSON_Delete
    // Clean up the duplicate first
    safe_cleanup(dup);
    // Clean up the original root
    safe_cleanup(root);

    return 0;
}