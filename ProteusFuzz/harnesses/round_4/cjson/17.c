// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

static const char *get_string_from_input(const uint8_t *Data, size_t Size, size_t offset, size_t *out_len) {
    if (offset >= Size) {
        return NULL;
    }
    size_t len = 0;
    while (offset + len < Size && Data[offset + len] != 0 && len < 1024) {
        len++;
    }
    if (len == 0) {
        return NULL;
    }
    *out_len = len;
    char *str = (char *)malloc(len + 1);
    if (!str) {
        return NULL;
    }
    memcpy(str, Data + offset, len);
    str[len] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    cJSON *obj = cJSON_CreateObject();
    if (!obj) {
        return 0;
    }

    size_t key1_len, key2_len;
    const char *key1 = get_string_from_input(Data, Size, 0, &key1_len);
    const char *key2 = get_string_from_input(Data, Size, key1_len + 1, &key2_len);

    if (!key1 || !key2) {
        cJSON_Delete(obj);
        if (key1) free((void *)key1);
        if (key2) free((void *)key2);
        return 0;
    }

    // Ensure we have enough data to read val1 and val2 safely
    size_t offset_val1 = key1_len + 1;
    size_t offset_val2 = key1_len + 2;
    
    if (Size <= offset_val2) {
        cJSON_Delete(obj);
        free((void *)key1);
        free((void *)key2);
        return 0;
    }

    cJSON_bool val1 = (Data[offset_val1] & 0x01) ? 1 : 0;
    cJSON_bool val2 = (Data[offset_val2] & 0x01) ? 1 : 0;

    cJSON_AddBoolToObject(obj, key1, val1);
    cJSON_AddBoolToObject(obj, key2, val2);

    cJSON_Delete(obj);

    free((void *)key1);
    free((void *)key2);

    return 0;
}