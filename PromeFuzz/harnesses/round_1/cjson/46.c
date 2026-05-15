// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
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

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

static char* create_null_terminated_string(const uint8_t *data, size_t size, size_t max_len) {
    if (size == 0) {
        char* str = (char*)malloc(2);
        if (str) {
            str[0] = 'a';
            str[1] = '\0';
        }
        return str;
    }
    
    size_t len = size > max_len ? max_len : size;
    char* str = (char*)malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

static double extract_double(const uint8_t *data, size_t size) {
    if (size >= sizeof(double)) {
        double value;
        memcpy(&value, data, sizeof(double));
        return value;
    } else if (size >= sizeof(float)) {
        float value;
        memcpy(&value, data, sizeof(float));
        return (double)value;
    } else if (size >= sizeof(int)) {
        int value;
        memcpy(&value, data, size < sizeof(int) ? size : sizeof(int));
        return (double)value;
    }
    return (double)(data[0] % 256);
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Extract values from fuzzer input
    double num1 = extract_double(Data, Size);
    double num2 = extract_double(Data + (Size > 8 ? 8 : 0), Size > 8 ? Size - 8 : Size);
    
    // Create null-terminated string for object names
    char* name1 = create_null_terminated_string(Data, Size, 64);
    char* name2 = create_null_terminated_string(Data + (Size > 16 ? 16 : 0), 
                                               Size > 16 ? Size - 16 : Size, 64);
    if (!name1 || !name2) {
        free(name1);
        free(name2);
        return 0;
    }
    
    // Test 1: cJSON_CreateNumber
    cJSON *number_item = cJSON_CreateNumber(num1);
    if (number_item) {
        // Test 4: cJSON_IsNumber
        cJSON_bool is_number = cJSON_IsNumber(number_item);
        (void)is_number; // Use result to avoid unused variable warning
        
        // Test 5: cJSON_GetNumberValue
        double retrieved_value = cJSON_GetNumberValue(number_item);
        (void)retrieved_value; // Use result
        
        // Test 3: cJSON_SetNumberHelper
        double set_result = cJSON_SetNumberHelper(number_item, num2);
        (void)set_result; // Use result
        
        // Test with NULL input for cJSON_SetNumberHelper
        if (Size % 2 == 0) {
            cJSON_SetNumberHelper(NULL, num1);
        }
        
        // Clean up number item
        cJSON_Delete(number_item);
    }
    
    // Test 2: cJSON_AddTrueToObject and Test 6: cJSON_AddNumberToObject
    cJSON *root_object = cJSON_CreateObject();
    if (root_object) {
        // Add true boolean
        cJSON *true_item = cJSON_AddTrueToObject(root_object, name1);
        (void)true_item; // Use result
        
        // Add number
        cJSON *added_number = cJSON_AddNumberToObject(root_object, name2, num1);
        (void)added_number; // Use result
        
        // Test with invalid object
        if (Size % 3 == 0) {
            cJSON_AddTrueToObject(NULL, name1);
            cJSON_AddNumberToObject(NULL, name2, num1);
        }
        
        // Test with empty name
        if (Size % 5 == 0) {
            cJSON_AddTrueToObject(root_object, "");
            cJSON_AddNumberToObject(root_object, "", num2);
        }
        
        // Clean up
        cJSON_Delete(root_object);
    }
    
    // Test edge cases for cJSON_IsNumber and cJSON_GetNumberValue
    cJSON *null_item = NULL;
    cJSON *false_item = cJSON_CreateFalse();
    if (false_item) {
        cJSON_bool is_false_number = cJSON_IsNumber(false_item);
        (void)is_false_number;
        double false_value = cJSON_GetNumberValue(false_item);
        (void)false_value;
        cJSON_Delete(false_item);
    }
    
    // Test with string item
    cJSON *string_item = cJSON_CreateString("test");
    if (string_item) {
        cJSON_bool is_string_number = cJSON_IsNumber(string_item);
        (void)is_string_number;
        double string_value = cJSON_GetNumberValue(string_item);
        (void)string_value;
        cJSON_Delete(string_item);
    }
    
    // Test cJSON_IsNumber with NULL
    cJSON_bool null_is_number = cJSON_IsNumber(null_item);
    (void)null_is_number;
    
    // Test cJSON_GetNumberValue with NULL
    double null_value = cJSON_GetNumberValue(null_item);
    (void)null_value;
    
    // Test with special double values
    if (Size % 7 == 0) {
        cJSON *special_number = cJSON_CreateNumber(NAN);
        if (special_number) {
            cJSON_Delete(special_number);
        }
        
        special_number = cJSON_CreateNumber(INFINITY);
        if (special_number) {
            cJSON_Delete(special_number);
        }
        
        special_number = cJSON_CreateNumber(-INFINITY);
        if (special_number) {
            cJSON_Delete(special_number);
        }
    }
    
    // Clean up allocated strings
    free(name1);
    free(name2);
    
    return 0;
}