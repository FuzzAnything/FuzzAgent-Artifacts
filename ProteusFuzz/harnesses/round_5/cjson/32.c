// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

static cJSON *create_random_cjson(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }

    // Create a buffer with a null terminator if needed
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
        return NULL;
    }
    memcpy(json_str, data, size);
    json_str[size] = '\0';

    cJSON *item = cJSON_Parse(json_str);
    free(json_str);
    return item;
}

static cJSON *create_manual_cjson(int type) {
    cJSON *item = cJSON_CreateNull();
    if (!item) {
        return NULL;
    }

    // Override the type manually for testing edge cases
    item->type = type;
    return item;
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test with parsed JSON
    cJSON *parsed_item = create_random_cjson(Data, Size);
    if (parsed_item) {
        cJSON_IsObject(parsed_item);
        cJSON_IsFalse(parsed_item);
        cJSON_IsRaw(parsed_item);
        cJSON_IsNull(parsed_item);
        cJSON_IsInvalid(parsed_item);
        cJSON_IsBool(parsed_item);
        cleanup_cjson(parsed_item);
    }

    // Test with NULL pointer
    cJSON_IsObject(NULL);
    cJSON_IsFalse(NULL);
    cJSON_IsRaw(NULL);
    cJSON_IsNull(NULL);
    cJSON_IsInvalid(NULL);
    cJSON_IsBool(NULL);

    // Test with manually created items of various types
    int types[] = {
        cJSON_Object,
        cJSON_Array,
        cJSON_String,
        cJSON_Number,
        cJSON_True,
        cJSON_False,
        cJSON_NULL,
        cJSON_Raw,
        cJSON_Invalid
    };
    int num_types = sizeof(types) / sizeof(types[0]);

    for (int i = 0; i < num_types; i++) {
        cJSON *manual_item = create_manual_cjson(types[i]);
        if (manual_item) {
            cJSON_IsObject(manual_item);
            cJSON_IsFalse(manual_item);
            cJSON_IsRaw(manual_item);
            cJSON_IsNull(manual_item);
            cJSON_IsInvalid(manual_item);
            cJSON_IsBool(manual_item);
            cleanup_cjson(manual_item);
        }
    }

    // Test with invalid type values
    int invalid_types[] = {-1, 100, 1000, 0};
    int num_invalid = sizeof(invalid_types) / sizeof(invalid_types[0]);

    for (int i = 0; i < num_invalid; i++) {
        cJSON *invalid_item = create_manual_cjson(invalid_types[i]);
        if (invalid_item) {
            cJSON_IsObject(invalid_item);
            cJSON_IsFalse(invalid_item);
            cJSON_IsRaw(invalid_item);
            cJSON_IsNull(invalid_item);
            cJSON_IsInvalid(invalid_item);
            cJSON_IsBool(invalid_item);
            cleanup_cjson(invalid_item);
        }
    }

    return 0;
}