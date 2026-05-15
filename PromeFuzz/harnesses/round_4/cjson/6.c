// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_InitHooks at cJSON.c:209:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
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

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string from fuzzer input for string creation
    char *str_buffer = NULL;
    if (Size > 0) {
        str_buffer = (char *)malloc(Size + 1);
        if (str_buffer) {
            memcpy(str_buffer, Data, Size);
            str_buffer[Size] = '\0';
        }
    } else {
        // If input is empty, use a default string
        str_buffer = strdup("fuzz");
    }

    // 1. cJSON_CreateString
    cJSON *string1 = cJSON_CreateString(str_buffer);
    
    // 2. cJSON_CreateArray
    cJSON *array = cJSON_CreateArray();
    
    // 3. cJSON_CreateString
    cJSON *string2 = cJSON_CreateString(str_buffer);
    
    // 4. cJSON_CreateString
    cJSON *string3 = cJSON_CreateString(str_buffer);
    
    // 5. cJSON_CreateString
    cJSON *string4 = cJSON_CreateString(str_buffer);
    
    // 6. cJSON_InitHooks
    cJSON_Hooks hooks = {NULL, NULL, NULL};
    cJSON_InitHooks(&hooks);
    
    // 7. cJSON_Delete
    cJSON_Delete(string1);
    
    // 8. cJSON_GetArraySize
    int array_size = cJSON_GetArraySize(array);
    
    // 9. cJSON_AddItemToArray
    cJSON_AddItemToArray(array, string2);
    
    // 10. cJSON_AddItemToArray
    cJSON_AddItemToArray(array, string3);
    
    // 11. cJSON_AddItemToObject
    // First create an object since we need one for this function
    cJSON *object = cJSON_CreateObject();
    if (object && string4) {
        cJSON_AddItemToObject(object, "key", string4);
    }
    
    // Cleanup
    if (object) {
        cJSON_Delete(object);
    }
    if (array) {
        cJSON_Delete(array);
    }
    if (str_buffer) {
        free(str_buffer);
    }
    
    return 0;
}