// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
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
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
    
    // 2. cJSON_GetErrorPtr (Only if parse failed)
    if (root == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        (void)error_ptr; // Use the pointer to suppress unused warnings
    }

    // 3. cJSON_GetObjectItemCaseSensitive (First call)
    // We need a key to search for. We'll use a fixed key or derive one if possible.
    // Since the input is arbitrary, we try a common key like "key" or use a substring.
    // To be safe and diverse, we try a few static keys.
    cJSON *item1 = NULL;
    if (root != NULL) {
        item1 = cJSON_GetObjectItemCaseSensitive(root, "key");
        // Try another key if first fails to explore more states
        if (item1 == NULL) {
            item1 = cJSON_GetObjectItemCaseSensitive(root, "data");
        }
        if (item1 == NULL) {
            item1 = cJSON_GetObjectItemCaseSensitive(root, "test");
        }
    }

    // 4. cJSON_IsString
    if (item1 != NULL) {
        cJSON_bool is_str = cJSON_IsString(item1);
        (void)is_str;
    }

    // 5. cJSON_GetObjectItemCaseSensitive (Second call)
    cJSON *item2 = NULL;
    if (root != NULL) {
        item2 = cJSON_GetObjectItemCaseSensitive(root, "value");
        if (item2 == NULL) {
            item2 = cJSON_GetObjectItemCaseSensitive(root, "id");
        }
    }

    // 6. cJSON_GetObjectItemCaseSensitive (Third call)
    cJSON *item3 = NULL;
    if (root != NULL) {
        item3 = cJSON_GetObjectItemCaseSensitive(root, "name");
        if (item3 == NULL) {
            item3 = cJSON_GetObjectItemCaseSensitive(root, "count");
        }
    }

    // 7. cJSON_GetObjectItemCaseSensitive (Fourth call)
    cJSON *item4 = NULL;
    if (root != NULL) {
        item4 = cJSON_GetObjectItemCaseSensitive(root, "result");
        if (item4 == NULL) {
            item4 = cJSON_GetObjectItemCaseSensitive(root, "status");
        }
    }

    // 8. cJSON_IsNumber (First check)
    if (item4 != NULL) {
        cJSON_bool is_num = cJSON_IsNumber(item4);
        (void)is_num;
    }

    // 9. cJSON_IsNumber (Second check)
    if (item3 != NULL) {
        cJSON_bool is_num2 = cJSON_IsNumber(item3);
        (void)is_num2;
    }

    // 10. cJSON_Delete
    if (root != NULL) {
        cJSON_Delete(root);
    }

    free(input);
    return 0;
}