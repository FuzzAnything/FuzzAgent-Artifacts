// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_InitHooks at cJSON.c:209:20 in cJSON.h
// cJSON_InitHooks at cJSON.c:209:20 in cJSON.h
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
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Use fuzzer input to create strings for cJSON_CreateString calls
    char *input_str = NULL;
    if (Size > 0) {
        input_str = (char*)malloc(Size + 1);
        if (!input_str) return 0;
        memcpy(input_str, Data, Size);
        input_str[Size] = '\0';
    } else {
        input_str = strdup("");
    }

    // 1. cJSON_CreateString
    cJSON *string1 = cJSON_CreateString(input_str);
    
    // 2. cJSON_CreateArray
    cJSON *array = cJSON_CreateArray();
    
    // 3. cJSON_CreateString
    cJSON *string2 = cJSON_CreateString(input_str);
    
    // 4. cJSON_CreateString
    cJSON *string3 = cJSON_CreateString(input_str);
    
    // 5. cJSON_CreateString
    cJSON *string4 = cJSON_CreateString(input_str);
    
    // 6. cJSON_InitHooks
    cJSON_Hooks hooks;
    if (Size > 0) {
        // Use fuzzer data to decide hook configuration
        uint8_t hook_selector = Data[0] % 3;
        switch (hook_selector) {
            case 0:
                cJSON_InitHooks(NULL);
                break;
            case 1:
                hooks.malloc_fn = malloc;
                hooks.free_fn = free;
                cJSON_InitHooks(&hooks);
                break;
            case 2:
                // Leave hooks uninitialized (use defaults)
                break;
        }
    } else {
        cJSON_InitHooks(NULL);
    }
    
    // 7. cJSON_Delete
    if (string1) {
        cJSON_Delete(string1);
    }
    
    // 8. cJSON_GetArraySize
    int array_size = 0;
    if (array) {
        array_size = cJSON_GetArraySize(array);
    }
    
    // 9. cJSON_AddItemToArray (first call)
    if (array && string2) {
        cJSON_AddItemToArray(array, string2);
    } else if (string2) {
        cJSON_Delete(string2);
    }
    
    // 10. cJSON_AddItemToArray (second call)
    if (array && string3) {
        cJSON_AddItemToArray(array, string3);
    } else if (string3) {
        cJSON_Delete(string3);
    }
    
    // 11. cJSON_AddItemToObject
    // First create an object since we need one for this function
    cJSON *object = cJSON_CreateObject();
    if (object && string4) {
        // Use a fixed key name for the object
        cJSON_AddItemToObject(object, "fuzz_key", string4);
    } else if (string4) {
        cJSON_Delete(string4);
    }
    
    // Cleanup remaining items
    if (array) {
        cJSON_Delete(array);
    }
    if (object) {
        cJSON_Delete(object);
    }
    
    free(input_str);
    return 0;
}