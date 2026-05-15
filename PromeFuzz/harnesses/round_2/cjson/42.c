// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include "cJSON.h"

static double get_double_from_data(const uint8_t *data, size_t size) {
    if (size >= sizeof(double)) {
        double result;
        memcpy(&result, data, sizeof(double));
        return result;
    }
    return (double)size;
}

static char *create_string_from_data(const uint8_t *data, size_t size, size_t max_len) {
    if (size == 0) {
        return strdup("default");
    }
    
    size_t len = size < max_len ? size : max_len;
    char *str = (char*)malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data, len);
    str[len] = '\0';
    
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '\0') {
            str[i] = 'a';
        }
    }
    
    return str;
}

static cJSON *create_test_object(const uint8_t *data, size_t size) {
    cJSON *obj = cJSON_CreateObject();
    if (!obj) return NULL;
    
    if (size > 0) {
        char *name = create_string_from_data(data, size, 16);
        if (name) {
            cJSON_AddNumberToObject(obj, name, get_double_from_data(data + 1, size - 1));
            cJSON_AddTrueToObject(obj, name);
            free(name);
        }
    }
    
    return obj;
}

int LLVMFuzzerTestOneInput_42(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    double test_num = get_double_from_data(data, size);
    
    cJSON *number_item = cJSON_CreateNumber(test_num);
    if (number_item) {
        cJSON_bool is_num = cJSON_IsNumber(number_item);
        (void)is_num;
        
        double retrieved = cJSON_GetNumberValue(number_item);
        (void)retrieved;
        
        double set_result = cJSON_SetNumberHelper(number_item, test_num * 2.0);
        (void)set_result;
        
        cJSON_Delete(number_item);
    }
    
    cJSON *null_item = NULL;
    cJSON_bool is_null_num = cJSON_IsNumber(null_item);
    (void)is_null_num;
    
    double null_retrieved = cJSON_GetNumberValue(null_item);
    (void)null_retrieved;
    
    double null_set = cJSON_SetNumberHelper(null_item, test_num);
    (void)null_set;
    
    cJSON *test_object = create_test_object(data, size);
    if (test_object) {
        cJSON_Delete(test_object);
    }
    
    cJSON *non_number_item = cJSON_CreateString("test");
    if (non_number_item) {
        cJSON_bool is_non_num = cJSON_IsNumber(non_number_item);
        (void)is_non_num;
        
        double non_num_retrieved = cJSON_GetNumberValue(non_number_item);
        (void)non_num_retrieved;
        
        cJSON_Delete(non_number_item);
    }
    
    cJSON *array_item = cJSON_CreateArray();
    if (array_item) {
        cJSON_AddNumberToObject(array_item, "invalid", test_num);
        cJSON_Delete(array_item);
    }
    
    cJSON *empty_object = cJSON_CreateObject();
    if (empty_object) {
        char *name = create_string_from_data(data, size, 8);
        if (name) {
            cJSON_AddNumberToObject(empty_object, name, test_num);
            cJSON_AddTrueToObject(empty_object, name);
            free(name);
        }
        cJSON_Delete(empty_object);
    }
    
    return 0;
}