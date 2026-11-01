// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
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

static void sanitize_input(uint8_t *data, size_t size, char **out_str, size_t *out_len) {
    if (size == 0) {
        *out_str = NULL;
        *out_len = 0;
        return;
    }

    // Find the first null terminator to limit string length
    size_t len = size;
    for (size_t i = 0; i < size; i++) {
        if (data[i] == '\0') {
            len = i;
            break;
        }
    }

    if (len == 0) {
        *out_str = NULL;
        *out_len = 0;
        return;
    }

    *out_str = (char *)malloc(len + 1);
    if (*out_str == NULL) {
        *out_len = 0;
        return;
    }
    memcpy(*out_str, data, len);
    (*out_str)[len] = '\0';
    *out_len = len;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Split input into two parts: one for the key, one for the raw value
    // Use at least 1 byte for key and 1 byte for raw
    size_t key_len = Size / 2;
    if (key_len < 1) key_len = 1;
    size_t raw_len = Size - key_len;
    if (raw_len < 1) raw_len = 1;

    const uint8_t *key_data = Data;
    const uint8_t *raw_data = Data + key_len;

    char *key_str = NULL;
    size_t key_str_len = 0;
    sanitize_input((uint8_t *)key_data, key_len, &key_str, &key_str_len);

    char *raw_str = NULL;
    size_t raw_str_len = 0;
    sanitize_input((uint8_t *)raw_data, raw_len, &raw_str, &raw_str_len);

    // Step 1: Create Object
    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        // Allocation failure, nothing to do
        free(key_str);
        free(raw_str);
        return 0;
    }

    // Step 2: Add Raw to Object
    // Only proceed if we have valid strings
    if (key_str != NULL && raw_str != NULL) {
        cJSON_AddRawToObject(obj, key_str, raw_str);
    }

    // Step 3: Delete Object
    cJSON_Delete(obj);

    // Cleanup
    free(key_str);
    free(raw_str);

    return 0;
}