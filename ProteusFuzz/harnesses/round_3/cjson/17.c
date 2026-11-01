// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    // Use input data to generate key names and boolean values
    // Ensure key names are null-terminated strings
    char key1[64];
    char key2[64];
    int key1_len = (Data[0] % 63) + 1;
    int key2_len = (Data[1] % 63) + 1;

    // Check if we have enough data for keys and boolean values
    // We need: 2 bytes for lengths, key1_len bytes for key1, key2_len bytes for key2, 2 bytes for booleans
    if (Size < 2 + key1_len + key2_len + 2) {
        cJSON_Delete(obj);
        return 0;
    }

    // Copy data for key1
    for (int i = 0; i < key1_len; i++) {
        key1[i] = (char)(Data[2 + i] & 0x7F) | 0x20; // Ensure printable ASCII
        if (key1[i] == '\0') {
            key1[i] = 'a';
        }
    }
    key1[key1_len] = '\0';

    // Copy data for key2
    for (int i = 0; i < key2_len; i++) {
        key2[i] = (char)(Data[2 + key1_len + i] & 0x7F) | 0x20; // Ensure printable ASCII
        if (key2[i] == '\0') {
            key2[i] = 'b';
        }
    }
    key2[key2_len] = '\0';

    // Determine boolean values from input
    cJSON_bool bool1 = (Data[2 + key1_len + key2_len] % 2);
    cJSON_bool bool2 = (Data[2 + key1_len + key2_len + 1] % 2);

    // Invoke target functions in order
    cJSON_AddBoolToObject(obj, key1, bool1);
    cJSON_AddBoolToObject(obj, key2, bool2);

    // Cleanup
    cJSON_Delete(obj);

    return 0;
}