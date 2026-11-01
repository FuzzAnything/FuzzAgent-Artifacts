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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Prepare environment: Create string from input data
    // Ensure null termination for string creation
    char *str_buf = (char *)malloc(Size + 1);
    if (!str_buf) {
        return 0;
    }
    memcpy(str_buf, Data, Size);
    str_buf[Size] = '\0';

    cJSON *string_item = cJSON_CreateString(str_buf);
    free(str_buf);

    if (!string_item) {
        return 0;
    }

    // Create number from input data (interpreting first 8 bytes as double if possible)
    double num_val = 0.0;
    if (Size >= 8) {
        memcpy(&num_val, Data, 8);
    } else {
        // Fallback to a simple integer derived from input
        num_val = (double)Data[0];
    }

    cJSON *number_item = cJSON_CreateNumber(num_val);
    if (!number_item) {
        cJSON_Delete(string_item);
        return 0;
    }

    // Invoke cJSON_GetNumberValue three times as requested
    // First on string_item (expected to return NAN)
    double val1 = cJSON_GetNumberValue(string_item);
    (void)val1; // Suppress unused variable warning

    // Second on number_item (expected to return the number)
    double val2 = cJSON_GetNumberValue(number_item);
    (void)val2;

    // Third on number_item again
    double val3 = cJSON_GetNumberValue(number_item);
    (void)val3;

    // Cleanup: Delete items in reverse order of creation logic (though independent)
    // Order specified: cJSON_Delete -> cJSON_Delete
    cJSON_Delete(string_item);
    cJSON_Delete(number_item);

    return 0;
}