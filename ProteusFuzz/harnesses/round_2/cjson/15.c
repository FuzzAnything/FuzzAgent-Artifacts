// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Prepare environment: Create string and number nodes */
    /* Use the input data as a string, ensuring null termination */
    char *str_buf = NULL;
    cJSON *str_node = NULL;
    cJSON *num_node = NULL;

    /* Create a null-terminated string from input */
    if (Size > 0) {
        str_buf = (char *)malloc(Size + 1);
        if (str_buf) {
            memcpy(str_buf, Data, Size);
            str_buf[Size] = '\0';
            str_node = cJSON_CreateString(str_buf);
            free(str_buf);
        }
    }

    /* Create a number node using the first byte as a double value */
    double num_val = (double)(int8_t)Data[0];
    num_node = cJSON_CreateNumber(num_val);

    /* Invoke target functions in order */
    /* 1. cJSON_CreateString (already done above) */
    /* 2. cJSON_CreateNumber (already done above) */
    
    /* 3. cJSON_GetNumberValue on str_node (should return NAN) */
    double val1 = cJSON_GetNumberValue(str_node);
    (void)val1; /* Suppress unused variable warning */

    /* 4. cJSON_GetNumberValue on num_node (should return the number) */
    double val2 = cJSON_GetNumberValue(num_node);
    (void)val2;

    /* 5. cJSON_GetNumberValue on NULL (should return NAN) */
    double val3 = cJSON_GetNumberValue(NULL);
    (void)val3;

    /* Cleanup: Delete nodes */
    /* 6. cJSON_Delete on str_node */
    if (str_node) {
        cJSON_Delete(str_node);
    }

    /* 7. cJSON_Delete on num_node */
    if (num_node) {
        cJSON_Delete(num_node);
    }

    return 0;
}