// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

static void cleanup(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    cJSON *root = NULL;
    cJSON *child = NULL;
    char *safe_key = NULL;

    if (Size < 1) {
        return 0;
    }

    root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    /* Use the fuzz input to derive a key name */
    safe_key = (char *)malloc(Size + 1);
    if (!safe_key) {
        cleanup(root);
        return 0;
    }
    memcpy(safe_key, Data, Size);
    safe_key[Size] = '\0';

    /* Add a nested object to the root */
    child = cJSON_AddObjectToObject(root, safe_key);
    if (!child) {
        free(safe_key);
        cleanup(root);
        return 0;
    }

    /* Optionally add more data to the child object to explore more states */
    if (Size > 0) {
        cJSON_AddNumberToObject(child, "value", (double)Data[0]);
    }

    /* Cleanup: delete the root, which recursively deletes children */
    cJSON_Delete(root);
    free(safe_key);

    return 0;
}