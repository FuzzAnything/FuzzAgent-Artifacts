// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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
#include <stdio.h>

static void write_to_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create object
    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    // Prepare name and raw strings from fuzzer input
    // Split input into two parts for name and raw value
    size_t name_len = Size / 2;
    size_t raw_len = Size - name_len;
    
    char *name = (char *)malloc(name_len + 1);
    char *raw = (char *)malloc(raw_len + 1);
    
    if (name == NULL || raw == NULL) {
        free(name);
        free(raw);
        cJSON_Delete(obj);
        return 0;
    }
    
    memcpy(name, Data, name_len);
    name[name_len] = '\0';
    memcpy(raw, Data + name_len, raw_len);
    raw[raw_len] = '\0';

    // Add raw to object
    cJSON *raw_item = cJSON_AddRawToObject(obj, name, raw);
    
    // Clean up
    free(name);
    free(raw);
    cJSON_Delete(obj);
    
    return 0;
}