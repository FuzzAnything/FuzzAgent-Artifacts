// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Split input into name and raw value parts
    size_t name_len = Data[0] % (Size - 1);
    if (name_len == 0) name_len = 1;
    
    size_t raw_len = Size - 1 - name_len;
    if (raw_len == 0) {
        return 0;
    }

    // Allocate and copy name string
    char *name = (char *)malloc(name_len + 1);
    if (!name) {
        return 0;
    }
    memcpy(name, Data + 1, name_len);
    name[name_len] = '\0';

    // Allocate and copy raw value string
    char *raw = (char *)malloc(raw_len + 1);
    if (!raw) {
        free(name);
        return 0;
    }
    memcpy(raw, Data + 1 + name_len, raw_len);
    raw[raw_len] = '\0';

    // Create JSON object
    cJSON *object = cJSON_CreateObject();
    if (!object) {
        free(name);
        free(raw);
        return 0;
    }

    // Add raw value to object
    cJSON *added_item = cJSON_AddRawToObject(object, name, raw);
    
    // Clean up
    cJSON_Delete(object);
    free(name);
    free(raw);

    return 0;
}