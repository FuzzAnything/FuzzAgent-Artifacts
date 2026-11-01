// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddFalseToObject at cJSON.c:2174:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

/* Fuzz driver for cjson: cJSON_CreateObject -> cJSON_AddFalseToObject -> cJSON_Delete */

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    cJSON *root = NULL;
    cJSON *item = NULL;
    char key[256];
    size_t key_len;

    /* Step 1: Prepare environment */
    if (Size == 0) {
        return 0;
    }

    /* Create a random key from input data */
    key_len = Size < sizeof(key) - 1 ? Size : sizeof(key) - 1;
    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    /* Step 2: Invoke target functions in order */
    root = cJSON_CreateObject();
    if (root == NULL) {
        return 0; /* Allocation failed, nothing to delete */
    }

    item = cJSON_AddFalseToObject(root, key);
    /* item may be NULL if key already exists or object is invalid, which is fine for fuzzing */

    /* Step 3: Cleanup */
    cJSON_Delete(root);

    return 0;
}