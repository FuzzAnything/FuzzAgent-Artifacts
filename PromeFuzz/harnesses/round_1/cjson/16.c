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
#include <math.h>

static void dummy_function(void) {
    // This is just to avoid unused function warnings if needed
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string from fuzz data
    char *str = NULL;
    if (Size > 0) {
        str = (char *)malloc(Size + 1);
        if (!str) return 0;
        memcpy(str, Data, Size);
        str[Size] = '\0';
    } else {
        // If no data, use empty string
        str = (char *)malloc(1);
        if (!str) return 0;
        str[0] = '\0';
    }
    
    // 1. cJSON_CreateString
    cJSON *string_item = cJSON_CreateString(str);
    
    // 2. cJSON_CreateNumber - use part of fuzz data as number
    double num_value = 0.0;
    if (Size >= sizeof(double)) {
        memcpy(&num_value, Data, sizeof(double));
    } else if (Size > 0) {
        // Use first byte as small number
        num_value = (double)Data[0];
    }
    cJSON *number_item = cJSON_CreateNumber(num_value);
    
    // 3. First cJSON_GetNumberValue
    double val1 = cJSON_GetNumberValue(number_item);
    (void)val1; // Use void to avoid unused variable warning
    
    // 4. Second cJSON_GetNumberValue
    double val2 = cJSON_GetNumberValue(number_item);
    (void)val2;
    
    // 5. Third cJSON_GetNumberValue
    double val3 = cJSON_GetNumberValue(number_item);
    (void)val3;
    
    // 6. cJSON_Delete for string item
    cJSON_Delete(string_item);
    
    // 7. cJSON_Delete for number item
    cJSON_Delete(number_item);
    
    free(str);
    return 0;
}