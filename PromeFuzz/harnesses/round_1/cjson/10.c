// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_InitHooks at cJSON.c:209:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_InitHooks at cJSON.c:209:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
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

static char* create_string_from_input(const uint8_t *data, size_t size, size_t max_len) {
    if (size == 0) {
        char* str = (char*)malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    size_t len = size;
    if (len > max_len) len = max_len;
    
    char* str = (char*)malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Split input into parts for different strings
    size_t part_size = Size / 4;
    
    // Create strings from input data
    char* str1 = create_string_from_input(Data, part_size, 256);
    char* str2 = create_string_from_input(Data + part_size, part_size, 256);
    char* str3 = create_string_from_input(Data + 2*part_size, part_size, 256);
    char* str4 = create_string_from_input(Data + 3*part_size, Size - 3*part_size, 256);
    
    if (!str1 || !str2 || !str3 || !str4) {
        free(str1); free(str2); free(str3); free(str4);
        return 0;
    }
    
    // 1. cJSON_CreateString
    cJSON* string1 = cJSON_CreateString(str1);
    
    // 2. cJSON_CreateArray
    cJSON* array = cJSON_CreateArray();
    
    // 3. cJSON_CreateString
    cJSON* string2 = cJSON_CreateString(str2);
    
    // 4. cJSON_CreateString
    cJSON* string3 = cJSON_CreateString(str3);
    
    // 5. cJSON_CreateString
    cJSON* string4 = cJSON_CreateString(str4);
    
    // 6. cJSON_InitHooks
    cJSON_Hooks hooks = {NULL, NULL, NULL};
    cJSON_InitHooks(&hooks);
    
    // 7. cJSON_Delete
    if (string1) {
        cJSON_Delete(string1);
    }
    
    // 8. cJSON_GetArraySize
    if (array) {
        int array_size = cJSON_GetArraySize(array);
        (void)array_size; // Use result to avoid unused variable warning
    }
    
    // 9. cJSON_AddItemToArray
    if (array && string2) {
        cJSON_AddItemToArray(array, string2);
    } else if (string2) {
        cJSON_Delete(string2);
    }
    
    // 10. cJSON_AddItemToArray
    if (array && string3) {
        cJSON_AddItemToArray(array, string3);
    } else if (string3) {
        cJSON_Delete(string3);
    }
    
    // 11. cJSON_AddItemToObject
    // Create an object for the last function
    cJSON* object = cJSON_CreateObject();
    if (object && string4) {
        // Use a fixed key name since we don't have more input data
        cJSON_AddItemToObject(object, "key", string4);
    } else if (string4) {
        cJSON_Delete(string4);
    }
    
    // Cleanup
    if (array) cJSON_Delete(array);
    if (object) cJSON_Delete(object);
    
    // Reset hooks to default
    cJSON_InitHooks(NULL);
    
    // Free temporary strings
    free(str1);
    free(str2);
    free(str3);
    free(str4);
    
    return 0;
}