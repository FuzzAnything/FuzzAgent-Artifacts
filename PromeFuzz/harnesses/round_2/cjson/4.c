// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
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

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Use fuzzer data to generate strings and numbers
    // Ensure we have at least some data for string operations
    size_t usable_size = Size > 100 ? 100 : Size;
    
    // Create first object
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }

    // Create keys and values from fuzzer data
    char key1[50];
    char value1[50];
    char key2[50];
    char key3[50];
    char key4[50];
    
    // Generate strings from fuzzer data (ensuring null termination)
    size_t key1_len = (Data[0] % 20) + 1;
    if (key1_len > 49) key1_len = 49;
    if (key1_len > usable_size - 1) key1_len = usable_size > 1 ? usable_size - 1 : 1;
    memcpy(key1, Data, key1_len);
    key1[key1_len] = '\0';
    
    size_t value1_len = (Data[1] % 20) + 1;
    if (value1_len > 49) value1_len = 49;
    size_t value1_offset = key1_len;
    if (value1_offset + value1_len > usable_size) {
        if (value1_offset < usable_size) {
            value1_len = usable_size - value1_offset;
        } else {
            value1_len = 1;
        }
    }
    memcpy(value1, Data + value1_offset, value1_len);
    value1[value1_len] = '\0';
    
    // Add string to object
    cJSON_AddStringToObject(root, key1, value1);
    
    // Generate second key
    size_t key2_offset = value1_offset + value1_len;
    size_t key2_len = (Data[2] % 20) + 1;
    if (key2_len > 49) key2_len = 49;
    if (key2_offset + key2_len > usable_size) {
        if (key2_offset < usable_size) {
            key2_len = usable_size - key2_offset;
        } else {
            key2_len = 1;
        }
    }
    memcpy(key2, Data + key2_offset, key2_len);
    key2[key2_len] = '\0';
    
    // Add array to object
    cJSON_AddArrayToObject(root, key2);
    
    // Create second object
    cJSON *child = cJSON_CreateObject();
    if (child == NULL) {
        cJSON_Delete(root);
        return 0;
    }
    
    // Generate third key
    size_t key3_offset = key2_offset + key2_len;
    size_t key3_len = (Data[3] % 20) + 1;
    if (key3_len > 49) key3_len = 49;
    if (key3_offset + key3_len > usable_size) {
        if (key3_offset < usable_size) {
            key3_len = usable_size - key3_offset;
        } else {
            key3_len = 1;
        }
    }
    memcpy(key3, Data + key3_offset, key3_len);
    key3[key3_len] = '\0';
    
    // Generate first number from fuzzer data
    double number1 = 0.0;
    if (usable_size >= 8) {
        // Use bytes 4-11 for first double
        uint64_t num_bytes;
        memcpy(&num_bytes, Data + 4, sizeof(uint64_t));
        number1 = (double)num_bytes / 1000.0;
    } else {
        number1 = (double)Data[4];
    }
    
    // Add number to child object
    cJSON_AddNumberToObject(child, key3, number1);
    
    // Generate fourth key
    size_t key4_offset = key3_offset + key3_len;
    size_t key4_len = (Data[12] % 20) + 1;
    if (key4_len > 49) key4_len = 49;
    if (key4_offset + key4_len > usable_size) {
        if (key4_offset < usable_size) {
            key4_len = usable_size - key4_offset;
        } else {
            key4_len = 1;
        }
    }
    memcpy(key4, Data + key4_offset, key4_len);
    key4[key4_len] = '\0';
    
    // Generate second number from fuzzer data
    double number2 = 0.0;
    if (usable_size >= 20) {
        // Use bytes 13-20 for second double
        uint64_t num_bytes2;
        memcpy(&num_bytes2, Data + 13, sizeof(uint64_t));
        number2 = (double)num_bytes2 / 1000.0;
    } else if (usable_size > 13) {
        number2 = (double)Data[13];
    }
    
    // Add second number to child object
    cJSON_AddNumberToObject(child, key4, number2);
    
    // Get the array we added earlier
    cJSON *array = cJSON_GetObjectItem(root, key2);
    if (array != NULL) {
        // Add child object to array
        cJSON_AddItemToArray(array, child);
    } else {
        // If array wasn't found, clean up child
        cJSON_Delete(child);
    }
    
    // Print the JSON
    char *printed = cJSON_Print(root);
    if (printed != NULL) {
        free(printed);
    }
    
    // Clean up
    cJSON_Delete(root);
    
    return 0;
}