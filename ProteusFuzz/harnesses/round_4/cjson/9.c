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
#include "cJSON.h"

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is a valid C-string by appending a null terminator if missing
    char *json_str = (char *)malloc(Size + 1);
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    // 1. cJSON_Parse
    cJSON *root = cJSON_Parse(json_str);

    // 2. cJSON_GetErrorPtr
    const char *error_ptr = cJSON_GetErrorPtr();
    (void)error_ptr;

    if (root) {
        // 3. cJSON_GetObjectItemCaseSensitive (First call)
        const char *key1 = (Size > 0) ? (const char *)Data : "key";
        cJSON *item1 = cJSON_GetObjectItemCaseSensitive(root, key1);

        // 4. cJSON_IsString
        if (item1) {
            cJSON_bool is_str = cJSON_IsString(item1);
            (void)is_str;
        }

        // 5. cJSON_GetObjectItemCaseSensitive (Second call)
        const char *key2 = (Size > 1) ? (const char *)(Data + 1) : "key2";
        cJSON *item2 = cJSON_GetObjectItemCaseSensitive(root, key2);

        // 6. cJSON_GetObjectItemCaseSensitive (Third call)
        const char *key3 = (Size > 2) ? (const char *)(Data + 2) : "key3";
        cJSON *item3 = cJSON_GetObjectItemCaseSensitive(root, key3);

        // 7. cJSON_GetObjectItemCaseSensitive (Fourth call)
        const char *key4 = (Size > 3) ? (const char *)(Data + 3) : "key4";
        cJSON *item4 = cJSON_GetObjectItemCaseSensitive(root, key4);

        // 8. cJSON_IsNumber
        if (item4) {
            cJSON_bool is_num1 = cJSON_IsNumber(item4);
            (void)is_num1;
        }

        // 9. cJSON_IsNumber (Second call)
        if (item3) {
            cJSON_bool is_num2 = cJSON_IsNumber(item3);
            (void)is_num2;
        }
    }

    // 10. cJSON_Delete
    if (root) {
        cJSON_Delete(root);
    }

    free(json_str);
    return 0;
}