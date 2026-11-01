// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "cJSON.h"

static void prepare_string_from_fuzz(const uint8_t *data, size_t size, char **out_str, size_t *out_len) {
    if (size == 0) {
        *out_str = NULL;
        *out_len = 0;
        return;
    }
    
    // Allocate buffer with space for null terminator
    char *buf = (char *)malloc(size + 1);
    if (!buf) {
        *out_str = NULL;
        *out_len = 0;
        return;
    }
    
    // Copy data
    memcpy(buf, data, size);
    // Ensure null termination
    buf[size] = '\0';
    
    *out_str = buf;
    *out_len = size;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Split input: first half for string, second half for number generation
    size_t split_point = Size / 2;
    const uint8_t *string_data = Data;
    size_t string_size = split_point;
    const uint8_t *number_data = Data + split_point;
    size_t number_size = Size - split_point;
    
    // Prepare string for cJSON_CreateString
    char *str_buf = NULL;
    size_t str_len = 0;
    prepare_string_from_fuzz(string_data, string_size, &str_buf, &str_len);
    
    cJSON *string_item = cJSON_CreateString(str_buf);
    if (str_buf) {
        free(str_buf);
    }
    
    if (!string_item) {
        return 0;
    }
    
    // Generate number from binary data safely
    double num_val = 0.0;
    if (number_size >= 8) {
        memcpy(&num_val, number_data, 8);
    } else if (number_size > 0) {
        // Create a small double from available bytes safely
        // Use a fixed-size buffer to prevent stack overflow
        uint8_t temp_buf[8] = {0};
        size_t copy_len = (number_size < 8) ? number_size : 8;
        memcpy(temp_buf, number_data, copy_len);
        
        // Interpret as a smaller integer type to avoid overflow
        if (copy_len == 1) {
            num_val = (double)(int8_t)temp_buf[0];
        } else if (copy_len == 2) {
            int16_t val16;
            memcpy(&val16, temp_buf, 2);
            num_val = (double)val16;
        } else if (copy_len == 4) {
            int32_t val32;
            memcpy(&val32, temp_buf, 4);
            num_val = (double)val32;
        } else {
            // For 5-7 bytes, just use the first 4 bytes
            int32_t val32;
            memcpy(&val32, temp_buf, 4);
            num_val = (double)val32;
        }
    }
    
    cJSON *number_item = cJSON_CreateNumber(num_val);
    
    // Perform multiple GetNumberValue calls as required
    double val1 = cJSON_GetNumberValue(number_item);
    double val2 = cJSON_GetNumberValue(number_item);
    double val3 = cJSON_GetNumberValue(number_item);
    
    // Also test GetNumberValue on string item (should return NAN)
    double val4 = cJSON_GetNumberValue(string_item);
    
    // Cleanup
    cJSON_Delete(number_item);
    cJSON_Delete(string_item);
    
    return 0;
}