// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure we have at least 2 bytes to split for string and number
    if (Size < 2) {
        return 0;
    }

    // Create a null-terminated string from the input data
    // We use the first half of the data for the string
    size_t str_len = Size / 2;
    if (str_len == 0) str_len = 1;
    
    char *str_buf = malloc(str_len + 1);
    if (!str_buf) return 0;
    
    memcpy(str_buf, Data, str_len);
    str_buf[str_len] = '\0';

    // Create a string object
    cJSON *str_obj = cJSON_CreateString(str_buf);
    free(str_buf);

    if (!str_obj) {
        return 0;
    }

    // Extract a double from the remaining data for the number
    // We interpret the second half as a double if possible, otherwise use a default
    double num_val = 0.0;
    if (Size - str_len >= sizeof(double)) {
        double temp;
        memcpy(&temp, Data + str_len, sizeof(double));
        num_val = temp;
    } else if (Size - str_len > 0) {
        // Fallback: use a simple integer cast if not enough bytes for double
        num_val = (double)Data[str_len];
    }

    // Create a number object
    cJSON *num_obj = cJSON_CreateNumber(num_val);
    
    if (!num_obj) {
        cJSON_Delete(str_obj);
        return 0;
    }

    // Invoke cJSON_GetNumberValue multiple times as requested
    // Note: str_obj is a string, so this should return NAN
    double val1 = cJSON_GetNumberValue(str_obj);
    
    // num_obj is a number, so this should return the value
    double val2 = cJSON_GetNumberValue(num_obj);
    
    // Call again on num_obj
    double val3 = cJSON_GetNumberValue(num_obj);

    // Perform cleanup
    cJSON_Delete(str_obj);
    cJSON_Delete(num_obj);

    return 0;
}