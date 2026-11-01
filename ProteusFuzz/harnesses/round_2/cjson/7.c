// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_InitHooks at cJSON.c:209:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

/* Custom memory hooks for testing */
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

    /* Ensure the input data is treated as a null-terminated string */
    /* Create a safe buffer with a guaranteed null terminator */
    char *safe_str = (char *)malloc(Size + 1);
    if (safe_str == NULL) {
        return 0;
    }
    memcpy(safe_str, Data, Size);
    safe_str[Size] = '\0';

    /* Step 1: cJSON_CreateString */
    cJSON *str1 = cJSON_CreateString(safe_str);
    if (str1 == NULL) {
        free(safe_str);
        return 0;
    }

    /* Step 2: cJSON_CreateArray */
    cJSON *arr = cJSON_CreateArray();
    if (arr == NULL) {
        cJSON_Delete(str1);
        free(safe_str);
        return 0;
    }

    /* Step 3: cJSON_CreateString */
    cJSON *str2 = cJSON_CreateString(safe_str);
    if (str2 == NULL) {
        cJSON_Delete(str1);
        cJSON_Delete(arr);
        free(safe_str);
        return 0;
    }

    /* Step 4: cJSON_CreateString */
    cJSON *str3 = cJSON_CreateString(safe_str);
    if (str3 == NULL) {
        cJSON_Delete(str1);
        cJSON_Delete(str2);
        cJSON_Delete(arr);
        free(safe_str);
        return 0;
    }

    /* Step 5: cJSON_CreateString */
    cJSON *str4 = cJSON_CreateString(safe_str);
    if (str4 == NULL) {
        cJSON_Delete(str1);
        cJSON_Delete(str2);
        cJSON_Delete(str3);
        cJSON_Delete(arr);
        free(safe_str);
        return 0;
    }

    /* Step 6: cJSON_InitHooks */
    cJSON_InitHooks(&custom_hooks);

    /* Step 7: cJSON_Delete (on str1 to test deletion before adding to array) */
    cJSON_Delete(str1);

    /* Step 8: cJSON_GetArraySize */
    int size = cJSON_GetArraySize(arr);
    (void)size; /* Suppress unused variable warning */

    /* Step 9: cJSON_AddItemToArray */
    cJSON_bool res1 = cJSON_AddItemToArray(arr, str2);
    if (!res1) {
        cJSON_Delete(str2);
        cJSON_Delete(str3);
        cJSON_Delete(str4);
        cJSON_Delete(arr);
        free(safe_str);
        return 0;
    }

    /* Step 10: cJSON_AddItemToArray */
    cJSON_bool res2 = cJSON_AddItemToArray(arr, str3);
    if (!res2) {
        cJSON_Delete(arr); /* This will also delete str2 and str3 */
        cJSON_Delete(str4);
        free(safe_str);
        return 0;
    }

    /* Step 11: cJSON_AddItemToObject */
    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        cJSON_Delete(arr);
        cJSON_Delete(str4);
        free(safe_str);
        return 0;
    }

    cJSON_bool res3 = cJSON_AddItemToObject(obj, "test_key", str4);
    if (!res3) {
        cJSON_Delete(str4);
        cJSON_Delete(arr);
        cJSON_Delete(obj);
        free(safe_str);
        return 0;
    }

    /* Cleanup */
    cJSON_Delete(arr);
    cJSON_Delete(obj);
    free(safe_str);

    return 0;
}