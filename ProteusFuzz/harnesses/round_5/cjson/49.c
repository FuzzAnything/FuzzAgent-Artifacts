// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <math.h>

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Extract values from input for testing
    double num1 = 0.0;
    double num2 = 0.0;
    char key[256] = {0};
    
    // Initialize numbers from input data if available
    if (Size >= 8) {
        memcpy(&num1, Data, sizeof(double));
    }
    if (Size >= 16) {
        memcpy(&num2, Data + 8, sizeof(double));
    }
    
    // Create a key from input data safely
    size_t key_len = (Size > 255) ? 255 : Size;
    size_t offset = (Size > 16) ? 16 : 0;
    
    // Ensure we don't read past the buffer
    if (offset < Size) {
        size_t available = Size - offset;
        if (available > 255) {
            available = 255;
        }
        key_len = available;
        memcpy(key, Data + offset, key_len);
    } else {
        key_len = 0;
    }
    
    // Ensure null termination
    key[key_len] = '\0';
    
    // If key is empty, use a default
    if (key_len == 0) {
        strcpy(key, "default_key");
    }

    // Test 1: cJSON_CreateNumber
    cJSON *num_node = cJSON_CreateNumber(num1);
    if (num_node) {
        // Verify it's a number
        if (cJSON_IsNumber(num_node)) {
            double val = cJSON_GetNumberValue(num_node);
            (void)val; // Use the value to prevent unused warnings
        }
        // Test cJSON_SetNumberHelper
        cJSON_SetNumberHelper(num_node, num2);
        cleanup_cjson(num_node);
    }

    // Test 2: cJSON_CreateObject and cJSON_AddNumberToObject
    cJSON *obj = cJSON_CreateObject();
    if (obj) {
        // Try adding a number to the object
        cJSON *added = cJSON_AddNumberToObject(obj, key, num1);
        if (added) {
            // Verify the added item
            if (cJSON_IsNumber(added)) {
                double val = cJSON_GetNumberValue(added);
                (void)val;
            }
            // Try setting a new number value
            cJSON_SetNumberHelper(added, num2);
        }
        // Try adding another number with same key (should fail or overwrite depending on implementation)
        cJSON *added2 = cJSON_AddNumberToObject(obj, key, num2);
        if (added2) {
            if (cJSON_IsNumber(added2)) {
                double val = cJSON_GetNumberValue(added2);
                (void)val;
            }
        }
        cleanup_cjson(obj);
    }

    // Test 3: Edge cases with special double values
    cJSON *nan_node = cJSON_CreateNumber(NAN);
    if (nan_node) {
        if (cJSON_IsNumber(nan_node)) {
            double val = cJSON_GetNumberValue(nan_node);
            (void)val;
        }
        cleanup_cjson(nan_node);
    }

    cJSON *inf_node = cJSON_CreateNumber(INFINITY);
    if (inf_node) {
        if (cJSON_IsNumber(inf_node)) {
            double val = cJSON_GetNumberValue(inf_node);
            (void)val;
        }
        cleanup_cjson(inf_node);
    }

    cJSON *neg_inf_node = cJSON_CreateNumber(-INFINITY);
    if (neg_inf_node) {
        if (cJSON_IsNumber(neg_inf_node)) {
            double val = cJSON_GetNumberValue(neg_inf_node);
            (void)val;
        }
        cleanup_cjson(neg_inf_node);
    }

    // Test 4: Very large and very small numbers
    cJSON *large_node = cJSON_CreateNumber(1e308);
    if (large_node) {
        if (cJSON_IsNumber(large_node)) {
            double val = cJSON_GetNumberValue(large_node);
            (void)val;
        }
        cleanup_cjson(large_node);
    }

    cJSON *small_node = cJSON_CreateNumber(1e-308);
    if (small_node) {
        if (cJSON_IsNumber(small_node)) {
            double val = cJSON_GetNumberValue(small_node);
            (void)val;
        }
        cleanup_cjson(small_node);
    }

    // Test 5: Test cJSON_IsNumber with NULL
    cJSON_bool is_num_null = cJSON_IsNumber(NULL);
    (void)is_num_null;

    // Test 6: Test cJSON_GetNumberValue with NULL
    double val_null = cJSON_GetNumberValue(NULL);
    (void)val_null;

    // Test 7: Test cJSON_SetNumberHelper with NULL
    double set_null = cJSON_SetNumberHelper(NULL, num1);
    (void)set_null;

    return 0;
}