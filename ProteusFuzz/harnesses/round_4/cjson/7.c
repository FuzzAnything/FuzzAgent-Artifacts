// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_InitHooks at cJSON.c:209:20 in cJSON.h
// cJSON_InitHooks at cJSON.c:209:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

static char *safe_strdup_from_input(const uint8_t *data, size_t size, size_t max_len) {
    if (size == 0) return NULL;
    size_t len = (max_len < size) ? max_len : size;
    char *str = (char *)malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Step 1: cJSON_CreateString
    char *str1 = safe_strdup_from_input(Data, Size, Size);
    cJSON *item1 = cJSON_CreateString(str1 ? str1 : "");
    free(str1);

    // Step 2: cJSON_CreateArray
    cJSON *array = cJSON_CreateArray();
    if (!array) {
        if (item1) cJSON_Delete(item1);
        return 0;
    }

    // Step 3: cJSON_CreateString
    char *str2 = safe_strdup_from_input(Data, Size, 10);
    cJSON *item2 = cJSON_CreateString(str2 ? str2 : "");
    free(str2);

    // Step 4: cJSON_CreateString
    size_t offset3 = (Size > 10) ? 10 : 0;
    size_t len3 = (Size > 10) ? (Size - 10) : 0;
    if (len3 > 5) len3 = 5;
    char *str3 = safe_strdup_from_input(Data + offset3, len3, len3);
    cJSON *item3 = cJSON_CreateString(str3 ? str3 : "");
    free(str3);

    // Step 5: cJSON_CreateString
    size_t offset4 = (Size > 15) ? 15 : 0;
    size_t len4 = (Size > 15) ? (Size - 15) : 0;
    if (len4 > 3) len4 = 3;
    char *str4 = safe_strdup_from_input(Data + offset4, len4, len4);
    cJSON *item4 = cJSON_CreateString(str4 ? str4 : "");
    free(str4);

    // Step 6: cJSON_InitHooks
    cJSON_Hooks hooks = {0};
    if (Size > 20) {
        hooks.malloc_fn = malloc;
        hooks.free_fn = free;
        cJSON_InitHooks(&hooks);
    } else {
        cJSON_InitHooks(NULL);
    }

    // Step 8: cJSON_GetArraySize
    int size_before = cJSON_GetArraySize(array);
    (void)size_before;

    // Step 9: cJSON_AddItemToArray
    if (item1) {
        cJSON_AddItemToArray(array, item1);
        item1 = NULL;
    }

    // Step 10: cJSON_AddItemToArray
    if (item2) {
        cJSON_AddItemToArray(array, item2);
        item2 = NULL;
    }

    // Step 11: cJSON_AddItemToObject
    cJSON *obj = cJSON_CreateObject();
    if (obj && item3) {
        cJSON_AddItemToObject(obj, "key1", item3);
        item3 = NULL;
    }
    
    if (obj) {
        cJSON_AddItemToArray(array, obj);
        obj = NULL;
    }

    // Cleanup
    if (item4) cJSON_Delete(item4);
    if (array) cJSON_Delete(array);

    return 0;
}