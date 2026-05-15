// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
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

static char* create_string_from_data(const uint8_t* data, size_t size, size_t max_len) {
    if (size == 0) {
        char* str = (char*)malloc(2);
        if (str) {
            str[0] = 'a';
            str[1] = '\0';
        }
        return str;
    }
    
    size_t len = size < max_len ? size : max_len;
    char* str = (char*)malloc(len + 1);
    if (!str) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        str[i] = data[i] == 0 ? 'a' : data[i];
    }
    str[len] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Create strings from fuzzer data
    char* str1 = create_string_from_data(Data, Size, 50);
    char* str2 = create_string_from_data(Data + (Size > 50 ? 50 : 0), 
                                        Size > 50 ? Size - 50 : 0, 50);
    char* str3 = create_string_from_data(Data + (Size > 100 ? 100 : 0), 
                                        Size > 100 ? Size - 100 : 0, 50);
    char* str4 = create_string_from_data(Data + (Size > 150 ? 150 : 0), 
                                        Size > 150 ? Size - 150 : 0, 50);
    char* str5 = create_string_from_data(Data + (Size > 200 ? 200 : 0), 
                                        Size > 200 ? Size - 200 : 0, 50);
    
    if (!str1 || !str2 || !str3 || !str4 || !str5) {
        free(str1);
        free(str2);
        free(str3);
        free(str4);
        free(str5);
        return 0;
    }
    
    // 1. Create root object
    cJSON* root = cJSON_CreateObject();
    if (!root) {
        free(str1);
        free(str2);
        free(str3);
        free(str4);
        free(str5);
        return 0;
    }
    
    // 2. Add string to object
    cJSON* string_item = cJSON_AddStringToObject(root, str1, str2);
    
    // 3. Add array to object
    cJSON* array_item = cJSON_AddArrayToObject(root, str3);
    
    // 4. Create another object
    cJSON* child_obj = cJSON_CreateObject();
    
    // 5. Add number to child object
    double num1 = Size > 0 ? (double)Data[0] : 0.0;
    cJSON* num_item1 = cJSON_AddNumberToObject(child_obj, str4, num1);
    
    // 6. Add another number to child object
    double num2 = Size > 1 ? (double)Data[1] : 1.0;
    cJSON* num_item2 = cJSON_AddNumberToObject(child_obj, str5, num2);
    
    // 7. Add child object to array
    if (array_item && child_obj) {
        cJSON_AddItemToArray(array_item, child_obj);
    } else {
        // Clean up child_obj if not added to array
        if (child_obj) {
            cJSON_Delete(child_obj);
        }
    }
    
    // 8. Print the JSON
    char* printed = cJSON_Print(root);
    if (printed) {
        free(printed);
    }
    
    // 9. Clean up
    cJSON_Delete(root);
    
    // Free allocated strings
    free(str1);
    free(str2);
    free(str3);
    free(str4);
    free(str5);
    
    return 0;
}