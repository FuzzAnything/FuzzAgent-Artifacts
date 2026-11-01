// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
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

static void sanitize_input(uint8_t *data, size_t size, uint8_t *out_data, size_t out_size) {
    if (size == 0 || out_size == 0) {
        out_data[0] = '\0';
        return;
    }
    size_t copy_size = (size < out_size - 1) ? size : (out_size - 1);
    memcpy(out_data, data, copy_size);
    out_data[copy_size] = '\0';
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    char key[256];
    char raw[256];

    // Derive key and raw string from input data
    // Use modulo to ensure we don't overflow buffers
    size_t key_len = (Size % 255) + 1;
    size_t raw_len = (Size % 255) + 1;

    sanitize_input((uint8_t *)Data, Size, (uint8_t *)key, sizeof(key));
    sanitize_input((uint8_t *)Data, Size, (uint8_t *)raw, sizeof(raw));

    // Attempt to add raw string to object
    cJSON_AddRawToObject(obj, key, raw);

    // Cleanup
    cJSON_Delete(obj);

    return 0;
}