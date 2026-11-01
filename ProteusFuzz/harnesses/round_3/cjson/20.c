// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }

    /* Use input data to generate a key name safely */
    size_t key_len = (Size % 256) + 1;
    if (key_len > Size) {
        key_len = Size;
    }

    char *key = (char *)malloc(key_len + 1);
    if (key == NULL) {
        cJSON_Delete(root);
        return 0;
    }

    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    cJSON *child = cJSON_AddObjectToObject(root, key);
    if (child != NULL) {
        /* Optionally add more nested structures to explore state */
        if (Size > key_len) {
            size_t sub_key_len = (Size - key_len) % 256 + 1;
            if (sub_key_len > (Size - key_len)) {
                sub_key_len = (Size - key_len);
            }
            char *sub_key = (char *)malloc(sub_key_len + 1);
            if (sub_key != NULL) {
                memcpy(sub_key, Data + key_len, sub_key_len);
                sub_key[sub_key_len] = '\0';
                cJSON *sub_child = cJSON_AddObjectToObject(child, sub_key);
                if (sub_child != NULL) {
                    /* Add a string value to further exercise the tree */
                    cJSON_AddStringToObject(sub_child, "test", "fuzz_value");
                }
                free(sub_key);
            }
        }
    }

    free(key);
    cJSON_Delete(root);

    return 0;
}