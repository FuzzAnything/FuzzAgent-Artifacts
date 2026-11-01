// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

static void generate_key(const uint8_t *data, size_t size, char *out, size_t out_size) {
    if (size == 0) {
        out[0] = '\0';
        return;
    }
    // Ensure we don't read more than available data or write more than buffer size
    size_t max_len = out_size - 1;
    size_t len = (size < max_len) ? size : max_len;
    
    // Copy data safely
    memcpy(out, data, len);
    out[len] = '\0';
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    char key1[256];
    char key2[256];
    
    // Generate keys from input data safely
    generate_key(Data, Size, key1, sizeof(key1));
    
    // For the second key, use a different offset or the same data if Size is small
    if (Size > 1) {
        generate_key(Data + 1, Size - 1, key2, sizeof(key2));
    } else {
        // If only 1 byte, reuse it or use a default
        key2[0] = Data[0];
        key2[1] = '\0';
    }

    // Derive boolean values from input bytes safely
    cJSON_bool bool1 = (Data[0] & 0x01) ? 1 : 0;
    cJSON_bool bool2 = (Size > 1 && (Data[1] & 0x01)) ? 1 : 0;

    cJSON_AddBoolToObject(obj, key1, bool1);
    cJSON_AddBoolToObject(obj, key2, bool2);

    cJSON_Delete(obj);

    return 0;
}