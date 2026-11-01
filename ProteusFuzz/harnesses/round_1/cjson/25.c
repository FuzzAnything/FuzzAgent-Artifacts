// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

static void cleanup(cJSON *root) {
    if (root) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // Use input data to generate keys and raw strings safely
    char key[256];
    char raw[256];
    
    // Limit key length to avoid overflow
    size_t key_len = (Size < 255) ? Size : 255;
    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    // Generate a raw string from a different part of the input if available
    size_t raw_len = (Size > 10 && Size < 255) ? Size - 10 : (Size > 10 ? 245 : 0);
    if (raw_len > 0) {
        memcpy(raw, Data + (Size > 10 ? 10 : 0), raw_len);
        raw[raw_len] = '\0';
    } else {
        raw[0] = '\0';
    }

    // Test cJSON_AddArrayToObject
    cJSON *arr = cJSON_AddArrayToObject(root, key);
    if (arr) {
        // Create a number item and add it to the array to exercise more logic
        cJSON *num_item = cJSON_CreateNumber(42.0);
        if (num_item) {
            cJSON_AddItemToArray(arr, num_item);
        }
    }

    // Test cJSON_AddNumberToObject
    double num = 0.0;
    if (Size > 0) {
        num = (double)(Data[0] & 0xFF) / 255.0;
    }
    cJSON_AddNumberToObject(root, key, num);

    // Test cJSON_AddRawToObject
    cJSON_AddRawToObject(root, key, raw);

    // Test cJSON_AddTrueToObject
    cJSON_AddTrueToObject(root, key);

    // Test cJSON_DetachItemFromObject
    cJSON *detached = cJSON_DetachItemFromObject(root, key);
    if (detached) {
        // Re-attach or delete to ensure no leaks
        cJSON_AddItemToObject(root, key, detached);
    }

    // Cleanup
    cleanup(root);

    return 0;
}