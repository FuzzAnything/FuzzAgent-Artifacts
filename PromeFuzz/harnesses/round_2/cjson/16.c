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

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string from fuzzing input for cJSON_CreateString
    char *string_data = NULL;
    if (Size > 0) {
        string_data = (char *)malloc(Size + 1);
        if (string_data == NULL) {
            return 0;
        }
        memcpy(string_data, Data, Size);
        string_data[Size] = '\0';
    } else {
        string_data = (char *)malloc(1);
        if (string_data == NULL) {
            return 0;
        }
        string_data[0] = '\0';
    }
    
    // Create a double value from fuzzing input for cJSON_CreateNumber
    double num_value = 0.0;
    if (Size >= sizeof(double)) {
        memcpy(&num_value, Data, sizeof(double));
    } else if (Size > 0) {
        // Use available bytes to create a simple number
        uint64_t temp = 0;
        memcpy(&temp, Data, Size);
        num_value = (double)temp;
    }
    
    // 1. cJSON_CreateString
    cJSON *string_item = cJSON_CreateString(string_data);
    
    // 2. cJSON_CreateNumber
    cJSON *number_item = cJSON_CreateNumber(num_value);
    
    // 3. First cJSON_GetNumberValue (on string item - should return NaN)
    double val1 = cJSON_GetNumberValue(string_item);
    
    // 4. Second cJSON_GetNumberValue (on number item)
    double val2 = cJSON_GetNumberValue(number_item);
    
    // 5. Third cJSON_GetNumberValue (on NULL item)
    double val3 = cJSON_GetNumberValue(NULL);
    
    // 6. First cJSON_Delete (string item)
    cJSON_Delete(string_item);
    
    // 7. Second cJSON_Delete (number item)
    cJSON_Delete(number_item);
    
    // Clean up
    free(string_data);
    
    // Use the values to avoid compiler optimizations
    volatile double dummy = val1 + val2 + val3;
    (void)dummy;
    
    return 0;
}