// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
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

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    double num = 0.0;
    char key[256] = {0};
    cJSON *obj = NULL;
    cJSON *item = NULL;
    cJSON *added_item = NULL;
    cJSON *str_item = NULL;
    cJSON *bool_item = NULL;
    cJSON *null_item = NULL;

    // Helper to extract double from input safely
    if (Size >= sizeof(double)) {
        memcpy(&num, Data, sizeof(double));
    } else if (Size >= 2) {
        // Generate a pseudo-random double from small input
        num = (double)(Data[0] * 256 + Data[1]) / 100.0;
    } else if (Size == 1) {
        num = (double)Data[0] / 100.0;
    } else {
        num = 0.0;
    }

    // Helper to extract key string from input safely
    size_t offset = 0;
    if (Size >= sizeof(double)) {
        offset = sizeof(double);
    }
    
    size_t remaining = Size - offset;
    size_t key_len = (remaining > sizeof(key) - 1) ? sizeof(key) - 1 : remaining;
    
    if (key_len > 0) {
        memcpy(key, Data + offset, key_len);
        key[key_len] = '\0';
    } else {
        strcpy(key, "key");
    }

    // Test cJSON_CreateNumber
    item = cJSON_CreateNumber(num);
    if (item) {
        cJSON_bool is_num = cJSON_IsNumber(item);
        if (is_num) {
            double val = cJSON_GetNumberValue(item);
            (void)val; // Use value to avoid unused warning
        }
        cleanup_cjson(item);
    }

    // Test cJSON_CreateObject
    obj = cJSON_CreateObject();
    if (obj) {
        // Test cJSON_AddNumberToObject
        added_item = cJSON_AddNumberToObject(obj, key, num);
        if (added_item) {
            cJSON_bool is_num = cJSON_IsNumber(added_item);
            if (is_num) {
                double val = cJSON_GetNumberValue(added_item);
                (void)val;
            }
        }

        // Test cJSON_SetNumberHelper directly on an existing number item
        item = cJSON_CreateNumber(123.456);
        if (item) {
            double set_val = cJSON_SetNumberHelper(item, num);
            (void)set_val;
            cleanup_cjson(item);
        }

        // Test edge cases with special doubles
        item = cJSON_CreateNumber(NAN);
        cleanup_cjson(item);

        item = cJSON_CreateNumber(INFINITY);
        cleanup_cjson(item);

        item = cJSON_CreateNumber(-INFINITY);
        cleanup_cjson(item);

        // Test with very large numbers to trigger saturation
        item = cJSON_CreateNumber(1e308);
        cleanup_cjson(item);

        // Test cJSON_IsNumber on non-number types
        str_item = cJSON_CreateString("test");
        if (str_item) {
            cJSON_bool is_num = cJSON_IsNumber(str_item);
            (void)is_num;
            // Test cJSON_GetNumberValue on non-number types
            double val = cJSON_GetNumberValue(str_item);
            (void)val;
            cleanup_cjson(str_item);
        }

        bool_item = cJSON_CreateTrue();
        if (bool_item) {
            cJSON_bool is_num = cJSON_IsNumber(bool_item);
            (void)is_num;
            cleanup_cjson(bool_item);
        }

        null_item = cJSON_CreateNull();
        if (null_item) {
            cJSON_bool is_num = cJSON_IsNumber(null_item);
            (void)is_num;
            cleanup_cjson(null_item);
        }

        // Test cJSON_SetNumberHelper on NULL
        double null_set = cJSON_SetNumberHelper(NULL, num);
        (void)null_set;

        cleanup_cjson(obj);
    }

    return 0;
}