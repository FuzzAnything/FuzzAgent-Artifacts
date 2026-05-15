// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
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

static double get_double_from_data(const uint8_t *data, size_t size, size_t offset) {
    double num = 0.0;
    if (size >= offset + sizeof(double)) {
        memcpy(&num, data + offset, sizeof(double));
    } else if (offset < size) {
        // Use available bytes to create a smaller number
        uint64_t temp = 0;
        size_t bytes_to_copy = size - offset;
        if (bytes_to_copy > sizeof(temp)) bytes_to_copy = sizeof(temp);
        memcpy(&temp, data + offset, bytes_to_copy);
        num = (double)temp;
    }
    return num;
}

static char *get_string_from_data(const uint8_t *data, size_t size, size_t offset, size_t max_len) {
    if (offset >= size) {
        char *str = malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    size_t remaining = size - offset;
    size_t len = remaining < max_len ? remaining : max_len;
    char *str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data + offset, len);
    str[len] = '\0';
    
    // Ensure null termination even if data contains null bytes
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '\0') {
            str[i] = ' ';
        }
    }
    
    return str;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create objects for testing
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;
    
    // Test cJSON_AddNumberToObject
    size_t name_offset = 0;
    char *name1 = get_string_from_data(Data, Size, name_offset, 50);
    double num1 = get_double_from_data(Data, Size, name_offset + (name1 ? strlen(name1) : 0));
    
    cJSON *num_item = cJSON_AddNumberToObject(root, name1, num1);
    
    // Test cJSON_IsNumber on the added item
    if (num_item) {
        cJSON_bool is_num = cJSON_IsNumber(num_item);
        (void)is_num; // Use result to avoid unused variable warning
    }
    
    // Test cJSON_SetNumberHelper
    if (num_item) {
        double num2 = get_double_from_data(Data, Size, Size/2);
        double result = cJSON_SetNumberHelper(num_item, num2);
        (void)result; // Use result
    }
    
    // Test cJSON_GetNumberValue
    if (num_item) {
        double extracted = cJSON_GetNumberValue(num_item);
        (void)extracted; // Use result
    }
    
    // Test cJSON_AddTrueToObject
    size_t name2_offset = Size/3;
    char *name2 = get_string_from_data(Data, Size, name2_offset, 50);
    cJSON *true_item = cJSON_AddTrueToObject(root, name2);
    (void)true_item; // Use result
    
    // Test cJSON_CreateNumber
    double num3 = get_double_from_data(Data, Size, Size/4);
    cJSON *num_node = cJSON_CreateNumber(num3);
    
    // Test cJSON_IsNumber and cJSON_GetNumberValue on created number
    if (num_node) {
        cJSON_bool is_num2 = cJSON_IsNumber(num_node);
        (void)is_num2;
        
        double extracted2 = cJSON_GetNumberValue(num_node);
        (void)extracted2;
        
        // Test cJSON_SetNumberHelper on created number
        double num4 = get_double_from_data(Data, Size, 3*Size/4);
        double result2 = cJSON_SetNumberHelper(num_node, num4);
        (void)result2;
        
        cJSON_Delete(num_node);
    }
    
    // Test edge cases
    // Test with NULL objects
    cJSON_AddNumberToObject(NULL, "test", 1.0);
    cJSON_IsNumber(NULL);
    cJSON_SetNumberHelper(NULL, 1.0);
    cJSON_GetNumberValue(NULL);
    cJSON_AddTrueToObject(NULL, "test");
    
    // Test with special double values
    cJSON *special_num = cJSON_CreateNumber(NAN);
    if (special_num) {
        cJSON_IsNumber(special_num);
        cJSON_GetNumberValue(special_num);
        cJSON_Delete(special_num);
    }
    
    special_num = cJSON_CreateNumber(INFINITY);
    if (special_num) {
        cJSON_IsNumber(special_num);
        cJSON_GetNumberValue(special_num);
        cJSON_Delete(special_num);
    }
    
    // Test with empty string name
    cJSON_AddNumberToObject(root, "", 1.0);
    cJSON_AddTrueToObject(root, "");
    
    // Cleanup
    cJSON_Delete(root);
    free(name1);
    free(name2);
    
    return 0;
}