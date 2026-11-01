// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test with NULL pointer directly
    cJSON_IsInvalid(NULL);
    cJSON_IsRaw(NULL);
    cJSON_IsNull(NULL);
    cJSON_IsTrue(NULL);
    cJSON_IsFalse(NULL);
    cJSON_IsBool(NULL);

    // Create a cJSON item manually to test various types
    cJSON *item = cJSON_CreateNull();
    if (item != NULL) {
        // Test with valid null item
        cJSON_IsInvalid(item);
        cJSON_IsRaw(item);
        cJSON_IsNull(item);
        cJSON_IsTrue(item);
        cJSON_IsFalse(item);
        cJSON_IsBool(item);
        cleanup_cjson(item);
    }

    item = cJSON_CreateTrue();
    if (item != NULL) {
        cJSON_IsInvalid(item);
        cJSON_IsRaw(item);
        cJSON_IsNull(item);
        cJSON_IsTrue(item);
        cJSON_IsFalse(item);
        cJSON_IsBool(item);
        cleanup_cjson(item);
    }

    item = cJSON_CreateFalse();
    if (item != NULL) {
        cJSON_IsInvalid(item);
        cJSON_IsRaw(item);
        cJSON_IsNull(item);
        cJSON_IsTrue(item);
        cJSON_IsFalse(item);
        cJSON_IsBool(item);
        cleanup_cjson(item);
    }

    item = cJSON_CreateNumber(123.45);
    if (item != NULL) {
        cJSON_IsInvalid(item);
        cJSON_IsRaw(item);
        cJSON_IsNull(item);
        cJSON_IsTrue(item);
        cJSON_IsFalse(item);
        cJSON_IsBool(item);
        cleanup_cjson(item);
    }

    item = cJSON_CreateString("test");
    if (item != NULL) {
        cJSON_IsInvalid(item);
        cJSON_IsRaw(item);
        cJSON_IsNull(item);
        cJSON_IsTrue(item);
        cJSON_IsFalse(item);
        cJSON_IsBool(item);
        cleanup_cjson(item);
    }

    // Parse JSON from fuzz input
    if (Size > 0) {
        // Ensure null termination for parsing
        char *json_str = (char *)malloc(Size + 1);
        if (json_str != NULL) {
            memcpy(json_str, Data, Size);
            json_str[Size] = '\0';

            cJSON *parsed = cJSON_Parse(json_str);
            if (parsed != NULL) {
                cJSON_IsInvalid(parsed);
                cJSON_IsRaw(parsed);
                cJSON_IsNull(parsed);
                cJSON_IsTrue(parsed);
                cJSON_IsFalse(parsed);
                cJSON_IsBool(parsed);

                // Traverse children if any
                cJSON *child = parsed->child;
                while (child != NULL) {
                    cJSON_IsInvalid(child);
                    cJSON_IsRaw(child);
                    cJSON_IsNull(child);
                    cJSON_IsTrue(child);
                    cJSON_IsFalse(child);
                    cJSON_IsBool(child);
                    child = child->next;
                }

                cJSON_Delete(parsed);
            }
            free(json_str);
        }
    }

    // Create an invalid item manually by modifying type
    item = cJSON_CreateNull();
    if (item != NULL) {
        item->type = cJSON_Invalid;
        cJSON_IsInvalid(item);
        cJSON_IsRaw(item);
        cJSON_IsNull(item);
        cJSON_IsTrue(item);
        cJSON_IsFalse(item);
        cJSON_IsBool(item);
        cleanup_cjson(item);
    }

    // Create a raw item manually
    item = cJSON_CreateString("raw");
    if (item != NULL) {
        item->type = cJSON_Raw;
        cJSON_IsInvalid(item);
        cJSON_IsRaw(item);
        cJSON_IsNull(item);
        cJSON_IsTrue(item);
        cJSON_IsFalse(item);
        cJSON_IsBool(item);
        cleanup_cjson(item);
    }

    return 0;
}