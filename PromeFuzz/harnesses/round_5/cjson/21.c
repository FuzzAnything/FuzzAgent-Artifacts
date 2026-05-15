// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
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

#define MAX_KEY_LEN 256

static char *create_key_from_data(const uint8_t *data, size_t size, size_t *consumed) {
    if (size == 0) {
        *consumed = 0;
        return NULL;
    }
    
    size_t key_len = size > MAX_KEY_LEN ? MAX_KEY_LEN : size;
    char *key = (char *)malloc(key_len + 1);
    if (!key) {
        *consumed = 0;
        return NULL;
    }
    
    memcpy(key, data, key_len);
    key[key_len] = '\0';
    *consumed = key_len;
    return key;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }
    
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }
    
    size_t consumed1 = 0;
    char *key1 = create_key_from_data(Data, Size, &consumed1);
    
    size_t remaining = Size - consumed1;
    const uint8_t *remaining_data = Data + consumed1;
    
    size_t consumed2 = 0;
    char *key2 = NULL;
    
    if (remaining > 0) {
        key2 = create_key_from_data(remaining_data, remaining, &consumed2);
    }
    
    cJSON_bool bool1 = (Data[0] & 1) ? 1 : 0;
    cJSON_bool bool2 = (Size > 1) ? ((Data[1] & 1) ? 1 : 0) : 0;
    
    if (key1) {
        cJSON_AddBoolToObject(root, key1, bool1);
        free(key1);
    }
    
    if (key2) {
        cJSON_AddBoolToObject(root, key2, bool2);
        free(key2);
    }
    
    cJSON_Delete(root);
    
    return 0;
}