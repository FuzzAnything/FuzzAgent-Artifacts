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

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string from fuzzer input for cJSON_CreateString
    char *str = NULL;
    if (Size > 0) {
        str = (char *)malloc(Size + 1);
        if (str) {
            memcpy(str, Data, Size);
            str[Size] = '\0';
        }
    }
    
    // Create string item
    cJSON *string_item = cJSON_CreateString(str ? str : "");
    
    // Create number item - use first 8 bytes of input as double if available
    double num = 0.0;
    if (Size >= sizeof(double)) {
        memcpy(&num, Data, sizeof(double));
    }
    cJSON *number_item = cJSON_CreateNumber(num);
    
    // Call cJSON_GetNumberValue three times on the number item
    if (number_item) {
        double val1 = cJSON_GetNumberValue(number_item);
        double val2 = cJSON_GetNumberValue(number_item);
        double val3 = cJSON_GetNumberValue(number_item);
        (void)val1; (void)val2; (void)val3; // Avoid unused variable warnings
    }
    
    // Clean up - delete both items
    if (string_item) {
        cJSON_Delete(string_item);
    }
    if (number_item) {
        cJSON_Delete(number_item);
    }
    
    if (str) {
        free(str);
    }
    
    return 0;
}