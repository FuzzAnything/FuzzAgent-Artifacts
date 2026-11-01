// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_InitHooks at cJSON.c:209:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static void *custom_malloc(size_t sz) {
    return malloc(sz);
}

static void custom_free(void *ptr) {
    free(ptr);
}

static cJSON_Hooks custom_hooks = {
    .malloc_fn = custom_malloc,
    .free_fn = custom_free
};

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Step 1: cJSON_CreateString
    // Use the input data as a string, ensuring it's null-terminated if possible
    char *str1 = NULL;
    if (Size > 0) {
        char *temp = malloc(Size + 1);
        if (temp) {
            memcpy(temp, Data, Size);
            temp[Size] = '\0';
            str1 = cJSON_CreateString(temp);
            free(temp);
        }
    }

    // Step 2: cJSON_CreateArray
    cJSON *array = cJSON_CreateArray();
    if (!array) {
        if (str1) cJSON_Delete(str1);
        return 0;
    }

    // Step 3: cJSON_CreateString (second)
    char *str2 = NULL;
    if (Size > 0) {
        char *temp = malloc(Size + 1);
        if (temp) {
            memcpy(temp, Data, Size);
            temp[Size] = '\0';
            str2 = cJSON_CreateString(temp);
            free(temp);
        }
    }

    // Step 4: cJSON_CreateString (third)
    char *str3 = NULL;
    if (Size > 0) {
        char *temp = malloc(Size + 1);
        if (temp) {
            memcpy(temp, Data, Size);
            temp[Size] = '\0';
            str3 = cJSON_CreateString(temp);
            free(temp);
        }
    }

    // Step 5: cJSON_CreateString (fourth)
    char *str4 = NULL;
    if (Size > 0) {
        char *temp = malloc(Size + 1);
        if (temp) {
            memcpy(temp, Data, Size);
            temp[Size] = '\0';
            str4 = cJSON_CreateString(temp);
            free(temp);
        }
    }

    // Step 6: cJSON_InitHooks
    cJSON_InitHooks(&custom_hooks);

    // Step 7: cJSON_Delete (on str1)
    if (str1) {
        cJSON_Delete(str1);
        str1 = NULL;
    }

    // Step 8: cJSON_GetArraySize
    int size = cJSON_GetArraySize(array);
    (void)size; // Suppress unused variable warning

    // Step 9: cJSON_AddItemToArray (add str2)
    if (str2) {
        cJSON_AddItemToArray(array, str2);
        str2 = NULL; // Ownership transferred
    }

    // Step 10: cJSON_AddItemToArray (add str3)
    if (str3) {
        cJSON_AddItemToArray(array, str3);
        str3 = NULL; // Ownership transferred
    }

    // Step 11: cJSON_AddItemToObject
    // We need an object to add to. Let's create one.
    cJSON *obj = cJSON_CreateObject();
    if (obj && str4) {
        cJSON_AddItemToObject(obj, "key", str4);
        str4 = NULL; // Ownership transferred
    }

    // Cleanup
    if (array) cJSON_Delete(array);
    if (obj) cJSON_Delete(obj);
    if (str2) cJSON_Delete(str2);
    if (str3) cJSON_Delete(str3);
    if (str4) cJSON_Delete(str4);

    return 0;
}