// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static void cleanup(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    // Derive key names and boolean values from input data
    char key1[64] = {0};
    char key2[64] = {0};
    cJSON_bool bool1 = (Data[0] % 2);
    cJSON_bool bool2 = (Size % 2);

    // Generate safe key strings from input
    size_t keyLen1 = (Size > 10) ? 10 : Size;
    size_t keyLen2 = (Size > 20) ? 10 : (Size > 10 ? Size - 10 : Size);
    
    if (keyLen1 > 63) keyLen1 = 63;
    if (keyLen2 > 63) keyLen2 = 63;

    memcpy(key1, Data, keyLen1);
    key1[keyLen1] = '\0';
    
    size_t offset = keyLen1;
    if (offset < Size && keyLen2 > 0) {
        memcpy(key2, Data + offset, keyLen2);
        key2[keyLen2] = '\0';
    } else {
        strcpy(key2, "default_key");
    }

    // Invoke target functions in order
    cJSON *item1 = cJSON_AddBoolToObject(obj, key1, bool1);
    cJSON *item2 = cJSON_AddBoolToObject(obj, key2, bool2);

    // Cleanup
    cleanup(obj);

    return 0;
}