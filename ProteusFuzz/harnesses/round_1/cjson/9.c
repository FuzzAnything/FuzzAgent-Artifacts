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
#include <cJSON.h>

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for cJSON_Parse
    char *json_str = (char *)malloc(Size + 1);
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    cJSON *root = cJSON_Parse(json_str);
    free(json_str);

    // Step 2: cJSON_GetErrorPtr (safe to call even if parse succeeded, though info is only useful on failure)
    const char *error_ptr = cJSON_GetErrorPtr();
    (void)error_ptr; // Use the variable to suppress unused warning

    if (!root) {
        return 0;
    }

    // Step 3: First cJSON_GetObjectItemCaseSensitive
    cJSON *item1 = cJSON_GetObjectItemCaseSensitive(root, "key1");

    // Step 4: cJSON_IsString
    if (item1) {
        cJSON_IsString(item1);
    }

    // Step 5: Second cJSON_GetObjectItemCaseSensitive
    cJSON *item2 = cJSON_GetObjectItemCaseSensitive(root, "key2");

    // Step 6: Third cJSON_GetObjectItemCaseSensitive
    cJSON *item3 = cJSON_GetObjectItemCaseSensitive(root, "key3");

    // Step 7: Fourth cJSON_GetObjectItemCaseSensitive
    cJSON *item4 = cJSON_GetObjectItemCaseSensitive(root, "key4");

    // Step 8: First cJSON_IsNumber
    if (item4) {
        cJSON_IsNumber(item4);
    }

    // Step 9: Second cJSON_IsNumber
    if (item3) {
        cJSON_IsNumber(item3);
    }

    // Step 10: cJSON_Delete
    cJSON_Delete(root);

    return 0;
}