// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Use fuzz data to create strings and numbers
    // We'll extract up to 5 strings from the input data for keys/values
    // and 2 numbers from the beginning of the data
    
    // Ensure we have enough data for at least some basic operations
    if (Size < 16) return 0;
    
    // Create null-terminated strings from the input data
    // We'll use different segments of the input for different strings
    size_t segment_size = Size / 5;
    if (segment_size == 0) segment_size = 1;
    
    // Create string buffers with null termination
    char key1[100] = {0};
    char str_val[100] = {0};
    char key2[100] = {0};
    char key3[100] = {0};
    char key4[100] = {0};
    
    // Copy data segments into string buffers
    size_t copy_len = segment_size < 99 ? segment_size : 99;
    
    if (Size >= segment_size) {
        memcpy(key1, Data, copy_len);
        key1[copy_len] = '\0';
    }
    
    if (Size >= 2*segment_size) {
        memcpy(str_val, Data + segment_size, copy_len);
        str_val[copy_len] = '\0';
    }
    
    if (Size >= 3*segment_size) {
        memcpy(key2, Data + 2*segment_size, copy_len);
        key2[copy_len] = '\0';
    }
    
    if (Size >= 4*segment_size) {
        memcpy(key3, Data + 3*segment_size, copy_len);
        key3[copy_len] = '\0';
    }
    
    if (Size >= 5*segment_size) {
        memcpy(key4, Data + 4*segment_size, copy_len);
        key4[copy_len] = '\0';
    }
    
    // Extract numbers from the beginning of the data
    double num1 = 0.0, num2 = 0.0;
    if (Size >= sizeof(double)) {
        memcpy(&num1, Data, sizeof(double));
    }
    if (Size >= 2*sizeof(double)) {
        memcpy(&num2, Data + sizeof(double), sizeof(double));
    }
    
    // 1. Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;
    
    // 2. Create string item
    cJSON *string_item = cJSON_CreateString(str_val);
    if (!string_item) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 3. Add string item to root object
    if (!cJSON_AddItemToObject(root, key1, string_item)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 4. Create array
    cJSON *array = cJSON_CreateArray();
    if (!array) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 5. Add array to root object
    if (!cJSON_AddItemToObject(root, key2, array)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 6. Create nested object
    cJSON *nested_obj = cJSON_CreateObject();
    if (!nested_obj) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 7. Add nested object to array
    if (!cJSON_AddItemToArray(array, nested_obj)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 8. Create first number
    cJSON *num_item1 = cJSON_CreateNumber(num1);
    if (!num_item1) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 9. Add first number to nested object
    if (!cJSON_AddItemToObject(nested_obj, key3, num_item1)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 10. Create second number
    cJSON *num_item2 = cJSON_CreateNumber(num2);
    if (!num_item2) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 11. Add second number to nested object
    if (!cJSON_AddItemToObject(nested_obj, key4, num_item2)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 12. Print the JSON structure
    char *printed = cJSON_Print(root);
    if (printed) {
        free(printed);
    }
    
    // 13. Clean up
    cJSON_Delete(root);
    
    return 0;
}