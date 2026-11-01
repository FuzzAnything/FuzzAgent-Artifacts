// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "cJSON.h"

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Extract values from input data safely
    double num1 = 0.0;
    double num2 = 0.0;
    int op = 0;

    if (Size >= sizeof(double)) {
        memcpy(&num1, Data, sizeof(double));
    }
    if (Size >= sizeof(double) * 2) {
        memcpy(&num2, Data + sizeof(double), sizeof(double));
    }
    if (Size >= sizeof(double) * 2 + sizeof(int)) {
        memcpy(&op, Data + sizeof(double) * 2, sizeof(int));
    }

    // Generate a key string safely from input
    char key[256];
    size_t key_len = 0;
    
    // Ensure we don't read beyond buffer bounds
    if (Size > 0) {
        key_len = (Size > 255) ? 255 : Size;
        // Copy data starting from a safe offset if available, otherwise from start
        size_t offset = 0;
        if (Size > 255) {
            offset = Size - 255; // Read from the end to avoid overlap with double/int reads if possible
        }
        
        // Ensure offset + key_len does not exceed Size
        if (offset + key_len > Size) {
            key_len = Size - offset;
        }

        if (key_len > 0) {
            memcpy(key, Data + offset, key_len);
            key[key_len] = '\0';
        }
    }
    
    // Ensure key is not empty
    if (key_len == 0 || key[0] == '\0') {
        strcpy(key, "fuzz_key");
    }

    // Test cJSON_CreateObject
    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    // Test cJSON_AddNumberToObject
    cJSON *added_item = cJSON_AddNumberToObject(obj, key, num1);
    if (added_item != NULL) {
        // Verify it is a number
        if (cJSON_IsNumber(added_item)) {
            double val = cJSON_GetNumberValue(added_item);
            // Just use the value to ensure path is covered
            if (isnan(val)) {
                // Handle NaN case
            }
        }
    }

    // Test cJSON_CreateNumber
    cJSON *num_node = cJSON_CreateNumber(num2);
    if (num_node != NULL) {
        if (cJSON_IsNumber(num_node)) {
            double val = cJSON_GetNumberValue(num_node);
            (void)val;
        }
        cleanup_cjson(num_node);
    }

    // Test cJSON_SetNumberHelper directly
    if (obj != NULL) {
        cJSON *test_item = cJSON_CreateNumber(0.0);
        if (test_item != NULL) {
            double result = cJSON_SetNumberHelper(test_item, num1);
            (void)result;
            cleanup_cjson(test_item);
        }
    }

    // Cleanup main object
    cleanup_cjson(obj);

    return 0;
}