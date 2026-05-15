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
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "cJSON.h"

static double fuzz_double_from_data(const uint8_t *data, size_t size) {
    if (size >= sizeof(double)) {
        double result;
        memcpy(&result, data, sizeof(double));
        return result;
    } else if (size > 0) {
        return (double)data[0];
    }
    return 0.0;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create null-terminated string from fuzzer input for cJSON_CreateString
    char *string_data = (char *)malloc(Size + 1);
    if (!string_data) {
        return 0;
    }
    memcpy(string_data, Data, Size);
    string_data[Size] = '\0';
    
    // Create double from fuzzer input for cJSON_CreateNumber
    double num = fuzz_double_from_data(Data, Size);
    
    // 1. cJSON_CreateString
    cJSON *string_item = cJSON_CreateString(string_data);
    
    // 2. cJSON_CreateNumber
    cJSON *number_item = cJSON_CreateNumber(num);
    
    // 3. cJSON_GetNumberValue (on string item - should return NaN)
    if (string_item) {
        double val1 = cJSON_GetNumberValue(string_item);
        (void)val1; // Use value to avoid unused variable warning
    }
    
    // 4. cJSON_GetNumberValue (on number item)
    if (number_item) {
        double val2 = cJSON_GetNumberValue(number_item);
        (void)val2; // Use value to avoid unused variable warning
    }
    
    // 5. cJSON_GetNumberValue (on number item again)
    if (number_item) {
        double val3 = cJSON_GetNumberValue(number_item);
        (void)val3; // Use value to avoid unused variable warning
    }
    
    // 6. cJSON_Delete (string item)
    if (string_item) {
        cJSON_Delete(string_item);
    }
    
    // 7. cJSON_Delete (number item)
    if (number_item) {
        cJSON_Delete(number_item);
    }
    
    free(string_data);
    return 0;
}