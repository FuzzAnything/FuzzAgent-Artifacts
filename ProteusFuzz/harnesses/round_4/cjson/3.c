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
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input data is null-terminated for cJSON_Parse
    // We create a temporary buffer to hold the null-terminated string
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    cJSON *root = cJSON_Parse(input_str);
    free(input_str);

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
    // Duplicate item3 if it exists, otherwise duplicate root
    cJSON *dup = NULL;
    if (item3) {
        dup = cJSON_Duplicate(item3, 1);
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
    // Compare root with dup (if dup exists), else compare root with itself
    cJSON_bool compare_result = 0;
    if (dup) {
        compare_result = cJSON_Compare(root, dup, 1);
    } else {
        compare_result = cJSON_Compare(root, root, 1);
    }

    // Cleanup
    if (dup) {
        cJSON_Delete(dup);
    }
    cJSON_Delete(root);

    return 0;
}