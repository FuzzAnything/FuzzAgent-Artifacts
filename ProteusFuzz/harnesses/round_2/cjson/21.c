// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

static void cleanup_json(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    // Create a key from the input data, ensuring it is null-terminated
    char *key = NULL;
    if (Size > 0) {
        key = (char *)malloc(Size + 1);
        if (key == NULL) {
            cleanup_json(obj);
            return 0;
        }
        memcpy(key, Data, Size);
        key[Size] = '\0';
    } else {
        key = (char *)malloc(1);
        if (key == NULL) {
            cleanup_json(obj);
            return 0;
        }
        key[0] = '\0';
    }

    // Invoke cJSON_AddTrueToObject
    cJSON_AddTrueToObject(obj, key);

    // Cleanup
    free(key);
    cleanup_json(obj);

    return 0;
}