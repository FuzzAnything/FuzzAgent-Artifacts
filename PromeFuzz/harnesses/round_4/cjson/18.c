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

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    // Create a JSON object
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }

    // Extract two boolean values from the fuzzer input
    cJSON_bool bool1 = 0;
    cJSON_bool bool2 = 0;
    
    if (Size >= 1) {
        bool1 = (Data[0] & 1) ? 1 : 0;
    }
    if (Size >= 2) {
        bool2 = (Data[1] & 1) ? 1 : 0;
    }

    // Extract two names from the fuzzer input
    char name1[256] = {0};
    char name2[256] = {0};
    
    // Generate name1 from the first part of input
    size_t name1_len = Size / 2;
    if (name1_len > 255) name1_len = 255;
    if (name1_len > 0) {
        memcpy(name1, Data, name1_len);
        name1[name1_len] = '\0';
    } else {
        strcpy(name1, "bool1");
    }
    
    // Generate name2 from the second part of input
    size_t name2_len = Size - name1_len;
    if (name2_len > 255) name2_len = 255;
    if (name2_len > 0) {
        memcpy(name2, Data + name1_len, name2_len);
        name2[name2_len] = '\0';
    } else {
        strcpy(name2, "bool2");
    }

    // Add first boolean to object
    cJSON *item1 = cJSON_AddBoolToObject(root, name1, bool1);
    
    // Add second boolean to object
    cJSON *item2 = cJSON_AddBoolToObject(root, name2, bool2);

    // Clean up
    cJSON_Delete(root);
    
    return 0;
}