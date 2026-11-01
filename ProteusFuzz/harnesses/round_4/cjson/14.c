// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void safe_create_string_from_input(const uint8_t *data, size_t size, cJSON **out_item) {
    if (size == 0 || data == NULL) {
        *out_item = NULL;
        return;
    }

    // Ensure null termination for string creation
    // We create a temporary buffer to ensure safety
    char *temp_str = (char *)malloc(size + 1);
    if (temp_str == NULL) {
        *out_item = NULL;
        return;
    }

    memcpy(temp_str, data, size);
    temp_str[size] = '\0';

    *out_item = cJSON_CreateString(temp_str);
    free(temp_str);
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    cJSON *str_node = NULL;
    cJSON *num_node = NULL;
    double num_val;

    // 1. Prepare environment and invoke cJSON_CreateString
    // We use the fuzzed data as the string content
    safe_create_string_from_input(Data, Size, &str_node);

    // 2. Invoke cJSON_CreateNumber
    // We derive a double from the input data to ensure diversity
    if (Size >= 8) {
        // Use first 8 bytes as a double representation if possible, or just cast
        // To avoid invalid doubles, we can use a simpler approach:
        // Use the first byte as an integer and scale it, or just use a fixed logic
        // Let's try to interpret bytes as a double if size allows, otherwise use a simple cast
        double d_val = 0.0;
        if (Size >= 8) {
            memcpy(&d_val, Data, 8);
        } else {
            // Fallback: create a double from available bytes
            if (Size > 0) d_val = (double)(Data[0]);
            if (Size > 1) d_val += (double)(Data[1]) / 100.0;
        }
        num_node = cJSON_CreateNumber(d_val);
    } else {
        // If size is small, create a number from a simple calculation
        double d_val = (Size > 0) ? (double)Data[0] : 0.0;
        num_node = cJSON_CreateNumber(d_val);
    }

    // 3. Invoke cJSON_GetNumberValue (First call on str_node - expected NAN)
    if (str_node != NULL) {
        num_val = cJSON_GetNumberValue(str_node);
        (void)num_val; // Suppress unused variable warning
    }

    // 4. Invoke cJSON_GetNumberValue (Second call on num_node - expected valid double)
    if (num_node != NULL) {
        num_val = cJSON_GetNumberValue(num_node);
        (void)num_val;
    }

    // 5. Invoke cJSON_GetNumberValue (Third call on str_node again)
    if (str_node != NULL) {
        num_val = cJSON_GetNumberValue(str_node);
        (void)num_val;
    }

    // 6. Cleanup: Invoke cJSON_Delete on str_node
    if (str_node != NULL) {
        cJSON_Delete(str_node);
    }

    // 7. Cleanup: Invoke cJSON_Delete on num_node
    if (num_node != NULL) {
        cJSON_Delete(num_node);
    }

    return 0;
}