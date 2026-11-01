// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test with NULL pointer directly for all functions
    cJSON_IsFalse(NULL);
    cJSON_IsObject(NULL);
    cJSON_IsRaw(NULL);
    cJSON_IsNull(NULL);
    cJSON_IsInvalid(NULL);
    cJSON_IsBool(NULL);

    // Parse the input data as JSON to create a cJSON structure
    cJSON *root = cJSON_ParseWithLength((const char *)Data, Size);

    if (root) {
        // Test the root node
        cJSON_IsFalse(root);
        cJSON_IsObject(root);
        cJSON_IsRaw(root);
        cJSON_IsNull(root);
        cJSON_IsInvalid(root);
        cJSON_IsBool(root);

        // Traverse the JSON tree to test all nodes
        cJSON *item = root;
        while (item) {
            cJSON_IsFalse(item);
            cJSON_IsObject(item);
            cJSON_IsRaw(item);
            cJSON_IsNull(item);
            cJSON_IsInvalid(item);
            cJSON_IsBool(item);

            // Move to next sibling
            item = item->next;
        }

        // Also traverse children if any
        item = root;
        while (item) {
            if (item->child) {
                cJSON *child = item->child;
                while (child) {
                    cJSON_IsFalse(child);
                    cJSON_IsObject(child);
                    cJSON_IsRaw(child);
                    cJSON_IsNull(child);
                    cJSON_IsInvalid(child);
                    cJSON_IsBool(child);
                    child = child->next;
                }
            }
            item = item->next;
        }

        // Cleanup the parsed JSON
        cJSON_Delete(root);
    }

    // Create manual cJSON structures to test edge cases
    cJSON *manual_obj = cJSON_CreateObject();
    if (manual_obj) {
        cJSON_IsFalse(manual_obj);
        cJSON_IsObject(manual_obj);
        cJSON_IsRaw(manual_obj);
        cJSON_IsNull(manual_obj);
        cJSON_IsInvalid(manual_obj);
        cJSON_IsBool(manual_obj);
        cJSON_Delete(manual_obj);
    }

    cJSON *manual_arr = cJSON_CreateArray();
    if (manual_arr) {
        cJSON_IsFalse(manual_arr);
        cJSON_IsObject(manual_arr);
        cJSON_IsRaw(manual_arr);
        cJSON_IsNull(manual_arr);
        cJSON_IsInvalid(manual_arr);
        cJSON_IsBool(manual_arr);
        cJSON_Delete(manual_arr);
    }

    cJSON *manual_str = cJSON_CreateString("test");
    if (manual_str) {
        cJSON_IsFalse(manual_str);
        cJSON_IsObject(manual_str);
        cJSON_IsRaw(manual_str);
        cJSON_IsNull(manual_str);
        cJSON_IsInvalid(manual_str);
        cJSON_IsBool(manual_str);
        cJSON_Delete(manual_str);
    }

    cJSON *manual_num = cJSON_CreateNumber(123);
    if (manual_num) {
        cJSON_IsFalse(manual_num);
        cJSON_IsObject(manual_num);
        cJSON_IsRaw(manual_num);
        cJSON_IsNull(manual_num);
        cJSON_IsInvalid(manual_num);
        cJSON_IsBool(manual_num);
        cJSON_Delete(manual_num);
    }

    cJSON *manual_true = cJSON_CreateTrue();
    if (manual_true) {
        cJSON_IsFalse(manual_true);
        cJSON_IsObject(manual_true);
        cJSON_IsRaw(manual_true);
        cJSON_IsNull(manual_true);
        cJSON_IsInvalid(manual_true);
        cJSON_IsBool(manual_true);
        cJSON_Delete(manual_true);
    }

    cJSON *manual_false = cJSON_CreateFalse();
    if (manual_false) {
        cJSON_IsFalse(manual_false);
        cJSON_IsObject(manual_false);
        cJSON_IsRaw(manual_false);
        cJSON_IsNull(manual_false);
        cJSON_IsInvalid(manual_false);
        cJSON_IsBool(manual_false);
        cJSON_Delete(manual_false);
    }

    cJSON *manual_null = cJSON_CreateNull();
    if (manual_null) {
        cJSON_IsFalse(manual_null);
        cJSON_IsObject(manual_null);
        cJSON_IsRaw(manual_null);
        cJSON_IsNull(manual_null);
        cJSON_IsInvalid(manual_null);
        cJSON_IsBool(manual_null);
        cJSON_Delete(manual_null);
    }

    // Test with raw string
    cJSON *manual_raw = cJSON_CreateRaw("raw");
    if (manual_raw) {
        cJSON_IsFalse(manual_raw);
        cJSON_IsObject(manual_raw);
        cJSON_IsRaw(manual_raw);
        cJSON_IsNull(manual_raw);
        cJSON_IsInvalid(manual_raw);
        cJSON_IsBool(manual_raw);
        cJSON_Delete(manual_raw);
    }

    // Test with invalid type manually
    cJSON *manual_invalid = cJSON_CreateNull();
    if (manual_invalid) {
        manual_invalid->type = cJSON_Invalid;
        cJSON_IsFalse(manual_invalid);
        cJSON_IsObject(manual_invalid);
        cJSON_IsRaw(manual_invalid);
        cJSON_IsNull(manual_invalid);
        cJSON_IsInvalid(manual_invalid);
        cJSON_IsBool(manual_invalid);
        cJSON_Delete(manual_invalid);
    }

    return 0;
}