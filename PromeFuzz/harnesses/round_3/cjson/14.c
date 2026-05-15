// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Extract two double values from fuzz data
    double num1, num2;
    if (Size >= sizeof(double) * 2) {
        memcpy(&num1, Data, sizeof(double));
        memcpy(&num2, Data + sizeof(double), sizeof(double));
    } else if (Size >= sizeof(double)) {
        memcpy(&num1, Data, sizeof(double));
        num2 = 0.0;
    } else {
        // Use the first byte as integer values if not enough data for doubles
        num1 = (double)Data[0];
        num2 = (double)(Size > 1 ? Data[1] : 0);
    }

    // Create number nodes
    cJSON *num_item1 = cJSON_CreateNumber(num1);
    if (num_item1 == NULL) {
        return 0;
    }

    cJSON *num_item2 = cJSON_CreateNumber(num2);
    if (num_item2 == NULL) {
        cJSON_Delete(num_item1);
        return 0;
    }

    // Create an object to add items to
    cJSON *object = cJSON_CreateObject();
    if (object == NULL) {
        cJSON_Delete(num_item1);
        cJSON_Delete(num_item2);
        return 0;
    }

    // Generate key from fuzz data
    char key[256];
    size_t key_len = Size > 16 ? 16 : Size;
    if (key_len > 0) {
        memcpy(key, Data, key_len);
        key[key_len] = '\0';
    } else {
        strcpy(key, "key");
    }

    // Add first number to object
    cJSON_AddItemToObject(object, key, num_item1);

    // Replace the item with second number
    cJSON_ReplaceItemInObject(object, key, num_item2);

    // Clean up
    cJSON_Delete(object);

    return 0;
}