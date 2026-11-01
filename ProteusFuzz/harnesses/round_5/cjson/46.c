// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
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

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *root = NULL;
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON_bool case_sensitive = (Data[0] & 1) ? 1 : 0;

    // Parse JSON from input data using length-aware function to avoid strlen overflow
    root = cJSON_ParseWithLength((const char *)Data, Size);

    if (root) {
        // Test cJSON_IsArray
        cJSON_IsArray(root);
        if (root->child) cJSON_IsArray(root->child);
        if (root->next) cJSON_IsArray(root->next);

        // Test cJSON_IsTrue
        cJSON_IsTrue(root);
        if (root->child) cJSON_IsTrue(root->child);
        if (root->next) cJSON_IsTrue(root->next);

        // Test cJSON_IsNumber
        cJSON_IsNumber(root);
        if (root->child) cJSON_IsNumber(root->child);
        if (root->next) cJSON_IsNumber(root->next);

        // Test cJSON_IsBool
        cJSON_IsBool(root);
        if (root->child) cJSON_IsBool(root->child);
        if (root->next) cJSON_IsBool(root->next);

        // Test cJSON_IsString
        cJSON_IsString(root);
        if (root->child) cJSON_IsString(root->child);
        if (root->next) cJSON_IsString(root->next);

        // Create a duplicate for comparison tests
        item1 = cJSON_Duplicate(root, 1);
        item2 = cJSON_Duplicate(root, 1);

        if (item1 && item2) {
            // Test cJSON_Compare with same items
            cJSON_Compare(item1, item2, case_sensitive);
            
            // Modify one item slightly to test inequality
            if (item1->child) {
                item1->child->type = cJSON_Number;
                item1->child->valuedouble = 42.0;
            }
            cJSON_Compare(item1, item2, case_sensitive);
        }

        // Cleanup duplicates
        cleanup_cjson(item1);
        cleanup_cjson(item2);
    }

    // Cleanup root
    cleanup_cjson(root);

    return 0;
}