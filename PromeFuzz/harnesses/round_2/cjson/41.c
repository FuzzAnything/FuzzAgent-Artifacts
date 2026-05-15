// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <float.h>
#include <limits.h>
#include <math.h>

static double get_double_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(double) > size) {
        *offset = size;
        return 0.0;
    }
    double value;
    memcpy(&value, data + *offset, sizeof(double));
    *offset += sizeof(double);
    return value;
}

static cJSON_bool get_bool_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) {
        *offset = size;
        return 0;
    }
    cJSON_bool value = data[*offset] & 1;
    (*offset)++;
    return value;
}

static char* get_string_from_data(const uint8_t *data, size_t size, size_t *offset, size_t max_len) {
    if (*offset >= size) {
        *offset = size;
        return strdup("");
    }
    
    size_t remaining = size - *offset;
    size_t str_len = remaining < max_len ? remaining : max_len;
    
    char* str = malloc(str_len + 1);
    if (!str) {
        *offset = size;
        return strdup("");
    }
    
    memcpy(str, data + *offset, str_len);
    str[str_len] = '\0';
    *offset += str_len;
    
    return str;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    size_t offset = 0;
    
    // Test cJSON_CreateNumber and cJSON_IsNumber
    double num1 = get_double_from_data(Data, Size, &offset);
    cJSON* number_item = cJSON_CreateNumber(num1);
    if (number_item) {
        cJSON_IsNumber(number_item);
        cJSON_Delete(number_item);
    }
    
    // Test with special double values
    double special_values[] = {
        NAN,
        INFINITY,
        -INFINITY,
        0.0,
        -0.0,
        DBL_MAX,
        DBL_MIN,
        (double)INT_MAX + 1,
        (double)INT_MIN - 1
    };
    
    for (size_t i = 0; i < sizeof(special_values)/sizeof(special_values[0]); i++) {
        cJSON* special_item = cJSON_CreateNumber(special_values[i]);
        if (special_item) {
            cJSON_IsNumber(special_item);
            cJSON_Delete(special_item);
        }
    }
    
    // Test cJSON_IsNumber with NULL and non-number types
    cJSON_IsNumber(NULL);
    
    cJSON* bool_item = cJSON_CreateBool(get_bool_from_data(Data, Size, &offset));
    if (bool_item) {
        cJSON_IsNumber(bool_item);
        cJSON_Delete(bool_item);
    }
    
    cJSON* string_item = cJSON_CreateString("test");
    if (string_item) {
        cJSON_IsNumber(string_item);
        cJSON_Delete(string_item);
    }
    
    cJSON* null_item = cJSON_CreateNull();
    if (null_item) {
        cJSON_IsNumber(null_item);
        cJSON_Delete(null_item);
    }
    
    // Test cJSON_AddBoolToObject and cJSON_AddNumberToObject
    cJSON* object = cJSON_CreateObject();
    if (object) {
        char* name1 = get_string_from_data(Data, Size, &offset, 100);
        cJSON_bool bool_val = get_bool_from_data(Data, Size, &offset);
        cJSON_AddBoolToObject(object, name1, bool_val);
        free(name1);
        
        char* name2 = get_string_from_data(Data, Size, &offset, 100);
        double num2 = get_double_from_data(Data, Size, &offset);
        cJSON_AddNumberToObject(object, name2, num2);
        free(name2);
        
        // Test with empty string name
        cJSON_AddBoolToObject(object, "", bool_val);
        cJSON_AddNumberToObject(object, "", num2);
        
        // Test with NULL object
        cJSON_AddBoolToObject(NULL, "test", 1);
        cJSON_AddNumberToObject(NULL, "test", 1.0);
        
        cJSON_Delete(object);
    }
    
    // Test cJSON_SetNumberHelper
    cJSON* number_to_set = cJSON_CreateNumber(0.0);
    if (number_to_set) {
        double num3 = get_double_from_data(Data, Size, &offset);
        cJSON_SetNumberHelper(number_to_set, num3);
        
        // Test with NULL
        cJSON_SetNumberHelper(NULL, num3);
        
        // Test with non-number item
        cJSON* array = cJSON_CreateArray();
        if (array) {
            cJSON_SetNumberHelper(array, num3);
            cJSON_Delete(array);
        }
        
        cJSON_Delete(number_to_set);
    }
    
    // Test cJSON_GetNumberValue
    cJSON* number_for_get = cJSON_CreateNumber(get_double_from_data(Data, Size, &offset));
    if (number_for_get) {
        cJSON_GetNumberValue(number_for_get);
        cJSON_Delete(number_for_get);
    }
    
    // Test cJSON_GetNumberValue with non-number types
    cJSON_GetNumberValue(NULL);
    
    cJSON* bool_for_get = cJSON_CreateBool(get_bool_from_data(Data, Size, &offset));
    if (bool_for_get) {
        cJSON_GetNumberValue(bool_for_get);
        cJSON_Delete(bool_for_get);
    }
    
    cJSON* string_for_get = cJSON_CreateString("not a number");
    if (string_for_get) {
        cJSON_GetNumberValue(string_for_get);
        cJSON_Delete(string_for_get);
    }
    
    // Test combination: create object, add items, modify numbers, retrieve values
    cJSON* complex_object = cJSON_CreateObject();
    if (complex_object) {
        // Add multiple items
        for (int i = 0; i < 5 && offset < Size; i++) {
            char name[20];
            snprintf(name, sizeof(name), "item%d", i);
            
            if (i % 2 == 0) {
                double val = get_double_from_data(Data, Size, &offset);
                cJSON_AddNumberToObject(complex_object, name, val);
            } else {
                cJSON_bool val = get_bool_from_data(Data, Size, &offset);
                cJSON_AddBoolToObject(complex_object, name, val);
            }
        }
        
        // Get and modify numbers
        cJSON* item = cJSON_GetObjectItem(complex_object, "item0");
        if (item && cJSON_IsNumber(item)) {
            double old_val = cJSON_GetNumberValue(item);
            cJSON_SetNumberHelper(item, old_val * 2.0);
            cJSON_IsNumber(item);
        }
        
        cJSON_Delete(complex_object);
    }
    
    return 0;
}