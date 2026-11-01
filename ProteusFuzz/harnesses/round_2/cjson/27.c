// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "cJSON.h"

static void cleanup_json(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test 1: cJSON_CreateNumber
    // Extract a double from the input buffer if possible
    double test_double = 0.0;
    if (Size >= sizeof(double)) {
        memcpy(&test_double, Data, sizeof(double));
    } else {
        // Fallback to simple integer cast if buffer is too small
        int32_t temp_int = 0;
        if (Size >= sizeof(int32_t)) {
            memcpy(&temp_int, Data, sizeof(int32_t));
        }
        test_double = (double)temp_int;
    }

    cJSON *num_node = cJSON_CreateNumber(test_double);
    if (num_node) {
        // Test 2: cJSON_IsNumber
        cJSON_IsNumber(num_node);
        
        // Test 4: cJSON_GetNumberValue
        double retrieved_val = cJSON_GetNumberValue(num_node);
        (void)retrieved_val; // Suppress unused variable warning
        
        // Test 3: cJSON_SetNumberHelper (modify the node)
        double new_val = test_double * 2.0;
        // Ensure we have enough data before reading for new_val
        if (Size >= (sizeof(double) * 2)) {
            memcpy(&new_val, Data + sizeof(double), sizeof(double));
        }
        cJSON_SetNumberHelper(num_node, new_val);
        
        // Verify it's still a number after modification
        cJSON_IsNumber(num_node);
        
        cleanup_json(num_node);
    }

    // Test 5: cJSON_CreateObject and cJSON_AddNumberToObject
    cJSON *obj = cJSON_CreateObject();
    if (obj) {
        // Generate a key from the input data
        char key[64];
        size_t key_len = (Size > 63) ? 63 : Size;
        if (key_len > 0) {
            memcpy(key, Data, key_len);
            key[key_len] = '\0';
        } else {
            strcpy(key, "fuzz_key");
        }

        // Attempt to add a number to the object
        double obj_val = 0.0;
        // Ensure we have enough data for the double value
        if (Size >= (key_len + sizeof(double))) {
            memcpy(&obj_val, Data + key_len, sizeof(double));
        }
        
        cJSON *added_item = cJSON_AddNumberToObject(obj, key, obj_val);
        if (added_item) {
            // Verify the added item
            cJSON_IsNumber(added_item);
            cJSON_GetNumberValue(added_item);
            
            // Try setting a new value on the added item
            double mod_val = obj_val + 1.0;
            cJSON_SetNumberHelper(added_item, mod_val);
        }
        
        cleanup_json(obj);
    }

    // Test edge cases with NULL pointers for safety checks
    cJSON_IsNumber(NULL);
    cJSON_GetNumberValue(NULL);
    double null_result = cJSON_SetNumberHelper(NULL, 1.0);
    (void)null_result;

    return 0;
}