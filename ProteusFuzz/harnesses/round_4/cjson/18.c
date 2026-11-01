// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
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

    // Prepare a null-terminated string from the input data
    // We limit the size to prevent excessive memory allocation
    size_t max_len = 1024;
    if (Size > max_len) {
        Size = max_len;
    }

    char *raw_str = (char *)malloc(Size + 1);
    if (raw_str == NULL) {
        cJSON_Delete(obj);
        return 0;
    }

    memcpy(raw_str, Data, Size);
    raw_str[Size] = '\0';

    // Use a static key name to avoid issues with keys containing nulls or special chars
    // The fuzzing focus is on the 'raw' JSON value content
    const char *key = "fuzz_key";

    // Invoke the target API sequence
    cJSON_AddRawToObject(obj, key, raw_str);

    // Cleanup
    free(raw_str);
    cJSON_Delete(obj);

    return 0;
}