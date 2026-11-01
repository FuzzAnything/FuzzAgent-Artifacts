// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "cJSON.h"

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Data == NULL || Size == 0) {
        return 0;
    }

    // Test cJSON_CreateFalse
    cJSON *falseNode = cJSON_CreateFalse();
    if (falseNode != NULL) {
        cJSON_IsFalse(falseNode);
        cJSON_IsBool(falseNode);
        cJSON_IsTrue(falseNode);
        cleanup_cjson(falseNode);
    }

    // Test cJSON_CreateTrue
    cJSON *trueNode = cJSON_CreateTrue();
    if (trueNode != NULL) {
        cJSON_IsTrue(trueNode);
        cJSON_IsBool(trueNode);
        cJSON_IsFalse(trueNode);
        cleanup_cjson(trueNode);
    }

    // Test cJSON_CreateBool with varying inputs derived from Data
    for (size_t i = 0; i < Size; i++) {
        cJSON_bool boolVal = (Data[i] % 2 == 0) ? cJSON_False : cJSON_True;
        cJSON *boolNode = cJSON_CreateBool(boolVal);
        if (boolNode != NULL) {
            cJSON_IsBool(boolNode);
            if (boolVal == cJSON_True) {
                cJSON_IsTrue(boolNode);
            } else {
                cJSON_IsFalse(boolNode);
            }
            cleanup_cjson(boolNode);
        }
    }

    // Test edge cases with NULL
    cJSON_IsTrue(NULL);
    cJSON_IsFalse(NULL);
    cJSON_IsBool(NULL);

    return 0;
}