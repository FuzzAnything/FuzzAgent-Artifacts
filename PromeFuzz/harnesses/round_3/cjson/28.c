// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_DetachItemFromObjectCaseSensitive at cJSON.c:2316:23 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_cJSON_DeleteItemFromObjectCaseSensitive(cJSON *object, const char *key) {
    if (object && key) {
        cJSON_DeleteItemFromObjectCaseSensitive(object, key);
    }
}

static void fuzz_cJSON_AddNumberToObject(cJSON *object, const char *name, double number) {
    if (object && name) {
        cJSON *result = cJSON_AddNumberToObject(object, name, number);
        (void)result; // Use result to avoid unused variable warning
    }
}

static cJSON *fuzz_cJSON_DetachItemFromObject(cJSON *object, const char *key) {
    if (object && key) {
        return cJSON_DetachItemFromObject(object, key);
    }
    return NULL;
}

static cJSON *fuzz_cJSON_DetachItemFromObjectCaseSensitive(cJSON *object, const char *key) {
    if (object && key) {
        return cJSON_DetachItemFromObjectCaseSensitive(object, key);
    }
    return NULL;
}

static void fuzz_cJSON_AddRawToObject(cJSON *object, const char *name, const char *raw) {
    if (object && name && raw) {
        cJSON *result = cJSON_AddRawToObject(object, name, raw);
        (void)result; // Use result to avoid unused variable warning
    }
}

static void fuzz_cJSON_AddStringToObject(cJSON *object, const char *name, const char *string) {
    if (object && name && string) {
        cJSON *result = cJSON_AddStringToObject(object, name, string);
        (void)result; // Use result to avoid unused variable warning
    }
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a dummy JSON object to work with
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // Use the first byte to determine which function to test
    uint8_t function_selector = Data[0] % 6;
    
    // Extract a key from the data (limited to 100 bytes for safety)
    char key[101] = {0};
    size_t key_len = (Size - 1) > 100 ? 100 : (Size - 1);
    if (key_len > 0) {
        memcpy(key, Data + 1, key_len);
        key[key_len] = '\0';
    } else {
        strcpy(key, "default_key");
    }

    // Extract a value string from remaining data (if any)
    char value[101] = {0};
    size_t value_start = 1 + key_len;
    if (value_start < Size) {
        size_t value_len = (Size - value_start) > 100 ? 100 : (Size - value_start);
        memcpy(value, Data + value_start, value_len);
        value[value_len] = '\0';
    } else {
        strcpy(value, "default_value");
    }

    // Extract a number from data (use first 8 bytes if available)
    double number = 0.0;
    if (Size >= 9) {
        memcpy(&number, Data + 1, sizeof(double));
    } else {
        number = (double)(Data[0]);
    }

    switch (function_selector) {
        case 0:
            // First add an item, then delete it
            cJSON_AddNumberToObject(root, key, number);
            fuzz_cJSON_DeleteItemFromObjectCaseSensitive(root, key);
            break;
            
        case 1:
            fuzz_cJSON_AddNumberToObject(root, key, number);
            break;
            
        case 2: {
            // First add an item, then detach it
            cJSON_AddStringToObject(root, key, value);
            cJSON *detached = fuzz_cJSON_DetachItemFromObject(root, key);
            if (detached) {
                cJSON_Delete(detached);
            }
            break;
        }
            
        case 3: {
            // First add an item, then detach it case sensitively
            cJSON_AddRawToObject(root, key, value);
            cJSON *detached = fuzz_cJSON_DetachItemFromObjectCaseSensitive(root, key);
            if (detached) {
                cJSON_Delete(detached);
            }
            break;
        }
            
        case 4:
            fuzz_cJSON_AddRawToObject(root, key, value);
            break;
            
        case 5:
            fuzz_cJSON_AddStringToObject(root, key, value);
            break;
    }

    // Clean up
    cJSON_Delete(root);
    
    return 0;
}