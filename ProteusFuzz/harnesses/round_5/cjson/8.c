// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
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

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for cJSON_Parse
    char *input = (char *)malloc(Size + 1);
    if (!input) {
        return 0;
    }
    memcpy(input, Data, Size);
    input[Size] = '\0';

    // 1. cJSON_Parse
    cJSON *root = cJSON_Parse(input);

    // 2. cJSON_GetErrorPtr
    // Called regardless of success to exercise the path, though spec says use after failure.
    // Safe to call, returns NULL on success.
    const char *error_ptr = cJSON_GetErrorPtr();
    (void)error_ptr; // Suppress unused warning

    if (root == NULL) {
        // Parse failed, nothing to traverse
        free(input);
        return 0;
    }

    // 3. cJSON_GetObjectItemCaseSensitive (First call)
    cJSON *item1 = cJSON_GetObjectItemCaseSensitive(root, "key1");

    // 4. cJSON_IsString
    if (item1 != NULL) {
        cJSON_IsString(item1);
    }

    // 5. cJSON_GetObjectItemCaseSensitive (Second call)
    cJSON *item2 = cJSON_GetObjectItemCaseSensitive(root, "key2");

    // 6. cJSON_GetObjectItemCaseSensitive (Third call)
    cJSON *item3 = cJSON_GetObjectItemCaseSensitive(root, "key3");

    // 7. cJSON_GetObjectItemCaseSensitive (Fourth call)
    cJSON *item4 = cJSON_GetObjectItemCaseSensitive(root, "key4");

    // 8. cJSON_IsNumber
    if (item4 != NULL) {
        cJSON_IsNumber(item4);
    }

    // 9. cJSON_IsNumber (Second call on potentially different item or same)
    if (item2 != NULL) {
        cJSON_IsNumber(item2);
    }

    // 10. cJSON_Delete
    cJSON_Delete(root);

    free(input);
    return 0;
}