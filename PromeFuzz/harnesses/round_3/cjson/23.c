// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <math.h>

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double) + 1) {
        return 0;
    }

    // Extract a double from the first 8 bytes
    double number;
    memcpy(&number, Data, sizeof(double));
    
    // Extract a string for object names (ensure null-terminated)
    size_t name_len = Size - sizeof(double);
    char *name = (char *)malloc(name_len + 1);
    if (!name) return 0;
    memcpy(name, Data + sizeof(double), name_len);
    name[name_len] = '\0';

    // Test cJSON_CreateNumber
    cJSON *num_obj = cJSON_CreateNumber(number);
    if (num_obj) {
        // Test cJSON_IsNumber on the created number object
        cJSON_bool is_num = cJSON_IsNumber(num_obj);
        (void)is_num; // Use result to avoid unused variable warning
        
        // Test cJSON_GetNumberValue on the number object
        double retrieved = cJSON_GetNumberValue(num_obj);
        (void)retrieved;
        
        // Test cJSON_SetNumberHelper on the number object
        double new_val = number * 2.0; // Different value
        double set_result = cJSON_SetNumberHelper(num_obj, new_val);
        (void)set_result;
        
        // Test cJSON_GetNumberValue again after setting
        retrieved = cJSON_GetNumberValue(num_obj);
        (void)retrieved;
        
        cJSON_Delete(num_obj);
    }

    // Test cJSON_AddNumberToObject and cJSON_AddTrueToObject
    cJSON *object = cJSON_CreateObject();
    if (object) {
        // Add number to object
        cJSON *added_num = cJSON_AddNumberToObject(object, name, number);
        (void)added_num;
        
        // Add true to object with modified name
        char modified_name[256];
        snprintf(modified_name, sizeof(modified_name), "%s_true", name);
        cJSON *added_true = cJSON_AddTrueToObject(object, modified_name);
        (void)added_true;
        
        // Test cJSON_IsNumber on the object's child
        cJSON *child = cJSON_GetObjectItem(object, name);
        if (child) {
            cJSON_bool child_is_num = cJSON_IsNumber(child);
            (void)child_is_num;
            
            // Test cJSON_GetNumberValue on the child
            double child_val = cJSON_GetNumberValue(child);
            (void)child_val;
            
            // Test cJSON_SetNumberHelper on the child
            double another_val = number / 2.0;
            double set_child_result = cJSON_SetNumberHelper(child, another_val);
            (void)set_child_result;
        }
        
        cJSON_Delete(object);
    }

    // Test edge cases with NULL inputs
    cJSON_SetNumberHelper(NULL, number);
    cJSON_AddNumberToObject(NULL, name, number);
    cJSON_AddTrueToObject(NULL, name);
    cJSON_GetNumberValue(NULL);
    cJSON_IsNumber(NULL);

    // Test with non-number cJSON object
    cJSON *string_obj = cJSON_CreateString("test");
    if (string_obj) {
        cJSON_bool string_is_num = cJSON_IsNumber(string_obj);
        (void)string_is_num;
        
        double string_as_num = cJSON_GetNumberValue(string_obj);
        (void)string_as_num;
        
        // This should work but the object type won't change
        double set_string_result = cJSON_SetNumberHelper(string_obj, number);
        (void)set_string_result;
        
        cJSON_Delete(string_obj);
    }

    free(name);
    return 0;
}