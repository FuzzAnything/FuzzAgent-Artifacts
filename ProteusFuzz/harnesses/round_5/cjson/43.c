// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
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

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    double num;
    char key[256];
    cJSON *obj = NULL;
    cJSON *num_node = NULL;
    cJSON *added_node = NULL;
    cJSON_bool is_num;
    double retrieved_val;
    double set_val;

    // Extract data for number
    if (Size >= sizeof(double)) {
        memcpy(&num, Data, sizeof(double));
    } else {
        num = 0.0;
    }

    // Extract data for key (null-terminated)
    size_t key_len = (Size > 255) ? 255 : Size;
    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    // Test 1: cJSON_CreateNumber
    num_node = cJSON_CreateNumber(num);
    if (num_node != NULL) {
        // Verify it is a number
        is_num = cJSON_IsNumber(num_node);
        (void)is_num; // Use the result to avoid unused warning

        // Retrieve value
        retrieved_val = cJSON_GetNumberValue(num_node);
        (void)retrieved_val;

        // Cleanup
        cleanup_cjson(num_node);
        num_node = NULL;
    }

    // Test 2: cJSON_CreateObject and cJSON_AddNumberToObject
    obj = cJSON_CreateObject();
    if (obj != NULL) {
        added_node = cJSON_AddNumberToObject(obj, key, num);
        if (added_node != NULL) {
            is_num = cJSON_IsNumber(added_node);
            (void)is_num;

            retrieved_val = cJSON_GetNumberValue(added_node);
            (void)retrieved_val;
        }
        // Cleanup object which cleans up children
        cleanup_cjson(obj);
        obj = NULL;
    }

    // Test 3: cJSON_SetNumberHelper on a new number node
    num_node = cJSON_CreateNumber(0.0);
    if (num_node != NULL) {
        set_val = cJSON_SetNumberHelper(num_node, num);
        (void)set_val;
        
        // Verify after setting
        is_num = cJSON_IsNumber(num_node);
        (void)is_num;
        
        retrieved_val = cJSON_GetNumberValue(num_node);
        (void)retrieved_val;

        cleanup_cjson(num_node);
        num_node = NULL;
    }

    // Test 4: Edge cases with NULL
    is_num = cJSON_IsNumber(NULL);
    (void)is_num;

    retrieved_val = cJSON_GetNumberValue(NULL);
    (void)retrieved_val;

    set_val = cJSON_SetNumberHelper(NULL, num);
    (void)set_val;

    // Test 5: Attempt to add to NULL object
    added_node = cJSON_AddNumberToObject(NULL, key, num);
    (void)added_node;

    // Test 6: Create number with extreme values if fuzzing allows
    // The fuzzer input is already used for 'num', so extreme values are covered if input provides them.
    
    return 0;
}