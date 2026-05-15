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
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from fuzzer input
    char *str = (char *)malloc(Size + 1);
    if (!str) {
        return 0;
    }
    memcpy(str, Data, Size);
    str[Size] = '\0';

    // Create cJSON string node
    cJSON *string_item = cJSON_CreateString(str);
    
    // Create cJSON number node using first 8 bytes as double if possible
    double num = 0.0;
    if (Size >= sizeof(double)) {
        memcpy(&num, Data, sizeof(double));
    } else {
        memcpy(&num, Data, Size);
    }
    cJSON *number_item = cJSON_CreateNumber(num);
    
    // Get number value three times as specified
    if (number_item) {
        double val1 = cJSON_GetNumberValue(number_item);
        double val2 = cJSON_GetNumberValue(number_item);
        double val3 = cJSON_GetNumberValue(number_item);
        (void)val1; (void)val2; (void)val3; // Avoid unused variable warnings
    }
    
    // Clean up
    if (string_item) {
        cJSON_Delete(string_item);
    }
    if (number_item) {
        cJSON_Delete(number_item);
    }
    
    free(str);
    return 0;
}