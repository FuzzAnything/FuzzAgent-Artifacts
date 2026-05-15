// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create object
    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    // Split input into name and raw value parts
    size_t name_len = Size / 2;
    size_t raw_len = Size - name_len;
    
    // Allocate and copy name string
    char *name = (char *)malloc(name_len + 1);
    if (name == NULL) {
        cJSON_Delete(obj);
        return 0;
    }
    memcpy(name, Data, name_len);
    name[name_len] = '\0';
    
    // Allocate and copy raw value string
    char *raw = (char *)malloc(raw_len + 1);
    if (raw == NULL) {
        free(name);
        cJSON_Delete(obj);
        return 0;
    }
    memcpy(raw, Data + name_len, raw_len);
    raw[raw_len] = '\0';

    // Add raw value to object
    cJSON *added_item = cJSON_AddRawToObject(obj, name, raw);
    
    // Clean up
    free(name);
    free(raw);
    cJSON_Delete(obj);
    
    return 0;
}