// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    // Create a key string from the input data
    size_t key_len = Size / 2;
    if (key_len == 0) key_len = 1;
    if (key_len > 256) key_len = 256;

    char *key = (char *)malloc(key_len + 1);
    if (key == NULL) {
        cJSON_Delete(obj);
        return 0;
    }
    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    // Create a raw string from the remaining input data
    size_t raw_len = Size - key_len;
    if (raw_len == 0) raw_len = 1;
    if (raw_len > 256) raw_len = 256;

    char *raw = (char *)malloc(raw_len + 1);
    if (raw == NULL) {
        free(key);
        cJSON_Delete(obj);
        return 0;
    }
    memcpy(raw, Data + key_len, raw_len);
    raw[raw_len] = '\0';

    // Invoke the target API sequence
    cJSON_AddRawToObject(obj, key, raw);

    // Cleanup
    free(key);
    free(raw);
    cJSON_Delete(obj);

    return 0;
}