// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cJSON.h"

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test cJSON_CreateNumber
    double num_val = 0.0;
    if (Size >= 8) {
        memcpy(&num_val, Data, 8);
    } else if (Size > 0) {
        num_val = (double)Data[0];
    }
    
    cJSON *number_node = cJSON_CreateNumber(num_val);
    if (number_node != NULL) {
        cJSON_bool is_num = cJSON_IsNumber(number_node);
        if (is_num) {
            double retrieved = cJSON_GetNumberValue(number_node);
            (void)retrieved;
        }
        cleanup_cjson(number_node);
    }

    // Test cJSON_CreateObject and cJSON_AddNumberToObject
    cJSON *obj = cJSON_CreateObject();
    if (obj != NULL) {
        // Extract a key from input if possible
        const char *key = "test_key";
        if (Size > 10) {
            char *temp_key = (char *)malloc(Size);
            if (temp_key != NULL) {
                memcpy(temp_key, Data, Size);
                temp_key[Size - 1] = '\0'; // Ensure null termination
                key = temp_key;
            }
        }

        double add_num = 0.0;
        if (Size >= 8) {
            memcpy(&add_num, Data, 8);
        } else if (Size > 0) {
            add_num = (double)Data[0];
        }

        cJSON *added_item = cJSON_AddNumberToObject(obj, key, add_num);
        if (added_item != NULL) {
            cJSON_bool is_num = cJSON_IsNumber(added_item);
            if (is_num) {
                double val = cJSON_GetNumberValue(added_item);
                (void)val;
            }
        }
        
        // Test cJSON_SetNumberHelper on the object's child if it exists
        if (obj->child != NULL && cJSON_IsNumber(obj->child)) {
            double new_val = 0.0;
            if (Size >= 8) {
                memcpy(&new_val, Data + (Size >= 16 ? 8 : 0), 8);
            } else {
                new_val = 1.0;
            }
            cJSON_SetNumberHelper(obj->child, new_val);
        }

        cleanup_cjson(obj);
    }

    // Test cJSON_SetNumberHelper with NULL
    double null_result = cJSON_SetNumberHelper(NULL, 123.456);
    (void)null_result;

    // Test cJSON_IsNumber and cJSON_GetNumberValue with NULL
    cJSON_bool null_is_num = cJSON_IsNumber(NULL);
    double null_get_val = cJSON_GetNumberValue(NULL);
    (void)null_is_num;
    (void)null_get_val;

    return 0;
}