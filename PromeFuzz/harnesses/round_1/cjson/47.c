// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
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

static double extract_double(const uint8_t *data, size_t size) {
    double value = 0.0;
    if (size >= sizeof(double)) {
        memcpy(&value, data, sizeof(double));
    } else if (size > 0) {
        int64_t temp = 0;
        memcpy(&temp, data, size);
        value = (double)temp;
    }
    return value;
}

static char *extract_string(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    char *str = (char *)malloc(size + 1);
    if (!str) return NULL;
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to decide which test path to take
    uint8_t selector = Data[0] % 6;
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    switch (selector) {
        case 0: { // Test cJSON_CreateNumber
            double num = extract_double(remaining_data, remaining_size);
            cJSON *num_item = cJSON_CreateNumber(num);
            if (num_item) {
                cJSON_Delete(num_item);
            }
            break;
        }
        
        case 1: { // Test cJSON_AddTrueToObject
            cJSON *object = cJSON_CreateObject();
            if (object) {
                char *name = extract_string(remaining_data, remaining_size);
                if (name) {
                    cJSON *true_item = cJSON_AddTrueToObject(object, name);
                    (void)true_item; // Use result to avoid unused variable warning
                    free(name);
                }
                cJSON_Delete(object);
            }
            break;
        }
        
        case 2: { // Test cJSON_SetNumberHelper
            double num = extract_double(remaining_data, remaining_size);
            cJSON *num_item = cJSON_CreateNumber(0.0);
            if (num_item) {
                double result = cJSON_SetNumberHelper(num_item, num);
                (void)result; // Use result to avoid unused variable warning
                cJSON_Delete(num_item);
            }
            break;
        }
        
        case 3: { // Test cJSON_IsNumber
            // Create different types of items
            cJSON *items[3];
            items[0] = cJSON_CreateNumber(extract_double(remaining_data, remaining_size));
            items[1] = cJSON_CreateString("test");
            items[2] = cJSON_CreateObject();
            
            for (int i = 0; i < 3; i++) {
                if (items[i]) {
                    cJSON_bool is_num = cJSON_IsNumber(items[i]);
                    (void)is_num; // Use result to avoid unused variable warning
                    cJSON_Delete(items[i]);
                }
            }
            break;
        }
        
        case 4: { // Test cJSON_GetNumberValue
            cJSON *num_item = cJSON_CreateNumber(extract_double(remaining_data, remaining_size));
            if (num_item) {
                double value = cJSON_GetNumberValue(num_item);
                (void)value; // Use result to avoid unused variable warning
                cJSON_Delete(num_item);
            }
            
            // Also test with non-number item
            cJSON *string_item = cJSON_CreateString("not a number");
            if (string_item) {
                double value = cJSON_GetNumberValue(string_item);
                (void)value; // Use result to avoid unused variable warning
                cJSON_Delete(string_item);
            }
            break;
        }
        
        case 5: { // Test cJSON_AddNumberToObject
            cJSON *object = cJSON_CreateObject();
            if (object) {
                // Use first half for name, second half for number
                size_t name_size = remaining_size / 2;
                size_t num_data_size = remaining_size - name_size;
                
                char *name = extract_string(remaining_data, name_size);
                double num = extract_double(remaining_data + name_size, num_data_size);
                
                if (name) {
                    cJSON *num_item = cJSON_AddNumberToObject(object, name, num);
                    (void)num_item; // Use result to avoid unused variable warning
                    free(name);
                }
                cJSON_Delete(object);
            }
            break;
        }
    }
    
    return 0;
}