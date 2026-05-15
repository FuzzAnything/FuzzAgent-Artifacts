// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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

static char* create_string_from_data(const uint8_t* data, size_t size) {
    if (size == 0) return NULL;
    
    char* str = (char*)malloc(size + 1);
    if (!str) return NULL;
    
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

static double create_double_from_data(const uint8_t* data, size_t size) {
    if (size < sizeof(double)) {
        // Use available bytes to create a simple double
        double result = 0.0;
        for (size_t i = 0; i < size && i < sizeof(double); i++) {
            ((unsigned char*)&result)[i % sizeof(double)] = data[i];
        }
        return result;
    }
    
    double result;
    memcpy(&result, data, sizeof(double));
    return result;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create test objects
    cJSON* object = cJSON_CreateObject();
    cJSON* number_item = cJSON_CreateNumber(0.0);
    
    if (!object || !number_item) {
        if (object) cJSON_Delete(object);
        if (number_item) cJSON_Delete(number_item);
        return 0;
    }
    
    // Extract name and number from fuzzer input
    size_t name_len = Size / 2;
    size_t num_offset = name_len;
    size_t num_len = Size - name_len;
    
    char* name = create_string_from_data(Data, name_len);
    double number = create_double_from_data(Data + num_offset, num_len);
    
    // Test cJSON_AddNumberToObject
    cJSON* added_number = cJSON_AddNumberToObject(object, name, number);
    
    // Test cJSON_AddTrueToObject
    cJSON* added_true = cJSON_AddTrueToObject(object, name);
    
    // Test cJSON_SetNumberHelper
    double set_result = cJSON_SetNumberHelper(number_item, number);
    (void)set_result; // Use result to avoid unused variable warning
    
    // Test cJSON_IsNumber
    cJSON_bool is_num1 = cJSON_IsNumber(number_item);
    cJSON_bool is_num2 = cJSON_IsNumber(object);
    (void)is_num1;
    (void)is_num2;
    
    // Test cJSON_GetNumberValue
    double get_num1 = cJSON_GetNumberValue(number_item);
    double get_num2 = cJSON_GetNumberValue(object);
    (void)get_num1;
    (void)get_num2;
    
    // Test cJSON_CreateNumber with various inputs
    cJSON* created_num1 = cJSON_CreateNumber(number);
    cJSON* created_num2 = cJSON_CreateNumber(NAN);
    cJSON* created_num3 = cJSON_CreateNumber(INFINITY);
    cJSON* created_num4 = cJSON_CreateNumber(-INFINITY);
    
    // Cleanup
    if (name) free(name);
    if (object) cJSON_Delete(object);
    if (number_item) cJSON_Delete(number_item);
    if (created_num1) cJSON_Delete(created_num1);
    if (created_num2) cJSON_Delete(created_num2);
    if (created_num3) cJSON_Delete(created_num3);
    if (created_num4) cJSON_Delete(created_num4);
    
    return 0;
}