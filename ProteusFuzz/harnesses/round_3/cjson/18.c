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

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    // Create a null-terminated string from the input data for the key
    // Ensure the key is not empty and has a reasonable length
    size_t key_len = Size;
    if (key_len > 1024) {
        key_len = 1024;
    }
    
    char *key = (char *)malloc(key_len + 1);
    if (key == NULL) {
        cJSON_Delete(obj);
        return 0;
    }
    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    // Create a null-terminated string for the raw JSON value
    // We use the same data or a subset to simulate raw JSON
    char *raw = (char *)malloc(Size + 1);
    if (raw == NULL) {
        free(key);
        cJSON_Delete(obj);
        return 0;
    }
    memcpy(raw, Data, Size);
    raw[Size] = '\0';

    // Invoke the target function sequence
    cJSON_AddRawToObject(obj, key, raw);

    // Cleanup
    free(key);
    free(raw);
    cJSON_Delete(obj);

    return 0;
}