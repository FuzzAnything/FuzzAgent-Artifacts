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

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
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
    
    // 2. cJSON_GetErrorPtr (Safe to call regardless of parse success/failure in this context for fuzzing)
    // Note: The documentation says use after failure, but calling it generally is safe for fuzzing exploration
    const char *error_ptr = cJSON_GetErrorPtr();
    (void)error_ptr; // Suppress unused variable warning

    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *item3 = NULL;
    cJSON *item4 = NULL;

    if (root) {
        // 3. cJSON_GetObjectItemCaseSensitive (First call)
        // We use a static key for simplicity, or could derive from input if root is an object
        // To explore more states, we try to get a key that might exist
        item1 = cJSON_GetObjectItemCaseSensitive(root, "key");

        // 4. cJSON_IsString
        if (item1) {
            cJSON_IsString(item1);
        }

        // 5. cJSON_GetObjectItemCaseSensitive (Second call)
        item2 = cJSON_GetObjectItemCaseSensitive(root, "data");

        // 6. cJSON_GetObjectItemCaseSensitive (Third call)
        item3 = cJSON_GetObjectItemCaseSensitive(root, "value");

        // 7. cJSON_GetObjectItemCaseSensitive (Fourth call)
        item4 = cJSON_GetObjectItemCaseSensitive(root, "number");

        // 8. cJSON_IsNumber
        if (item3) {
            cJSON_IsNumber(item3);
        }

        // 9. cJSON_IsNumber
        if (item4) {
            cJSON_IsNumber(item4);
        }
    }

    // 10. cJSON_Delete
    if (root) {
        cJSON_Delete(root);
    }

    free(input);
    return 0;
}