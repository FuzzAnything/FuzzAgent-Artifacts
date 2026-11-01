// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Extract potential double values from input
    double num1 = 0.0;
    double num2 = 0.0;
    double num3 = 0.0;
    
    if (Size >= sizeof(double)) {
        memcpy(&num1, Data, sizeof(double));
    }
    if (Size >= sizeof(double) * 2) {
        memcpy(&num2, Data + sizeof(double), sizeof(double));
    }
    if (Size >= sizeof(double) * 3) {
        memcpy(&num3, Data + sizeof(double) * 2, sizeof(double));
    }

    // Extract potential key name (null-terminated string)
    char key[256] = {0};
    size_t key_len = (Size > 255) ? 255 : Size;
    if (Size > 0) {
        memcpy(key, Data, key_len);
        key[key_len] = '\0';
    }

    // Test cJSON_CreateNumber
    cJSON *num_node = cJSON_CreateNumber(num1);
    if (num_node != NULL) {
        // Test cJSON_IsNumber
        cJSON_IsNumber(num_node);
        
        // Test cJSON_GetNumberValue
        double val = cJSON_GetNumberValue(num_node);
        (void)val; // Suppress unused warning
        
        // Test cJSON_SetNumberHelper
        double set_val = cJSON_SetNumberHelper(num_node, num2);
        (void)set_val;
        
        cleanup_cjson(num_node);
    }

    // Test cJSON_CreateObject and cJSON_AddNumberToObject
    cJSON *obj = cJSON_CreateObject();
    if (obj != NULL) {
        // Add a number to the object
        cJSON *added_num = cJSON_AddNumberToObject(obj, key, num1);
        if (added_num != NULL) {
            cJSON_IsNumber(added_num);
            double retrieved = cJSON_GetNumberValue(added_num);
            (void)retrieved;
            
            // Try to set number helper on the added item
            cJSON_SetNumberHelper(added_num, num3);
        }
        
        // Try adding another number with same key (should fail or overwrite depending on implementation details)
        cJSON_AddNumberToObject(obj, key, num2);
        
        // Add number with different key if possible
        char alt_key[256];
        memcpy(alt_key, key, sizeof(alt_key));
        alt_key[0] = 'x'; // Ensure different key
        cJSON_AddNumberToObject(obj, alt_key, num3);
        
        cleanup_cjson(obj);
    }

    // Test cJSON_SetNumberHelper on NULL
    double null_result = cJSON_SetNumberHelper(NULL, num1);
    (void)null_result;

    // Test cJSON_GetNumberValue on non-number
    cJSON *str_node = cJSON_CreateString("test");
    if (str_node != NULL) {
        double str_val = cJSON_GetNumberValue(str_node);
        (void)str_val;
        cJSON_IsNumber(str_node);
        cleanup_cjson(str_node);
    }

    return 0;
}