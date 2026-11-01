// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is treated as a valid C-string by finding the first null terminator
    // or using the whole buffer if no null terminator exists within bounds.
    // cJSON_CreateString expects a null-terminated string.
    size_t str_len = Size;
    const char *str_start = (const char *)Data;
    
    // Find the first null byte to ensure we pass a valid C-string
    for (size_t i = 0; i < Size; i++) {
        if (Data[i] == 0) {
            str_len = i;
            break;
        }
    }

    // Create a safe null-terminated string buffer if the input wasn't null-terminated
    char *safe_str = NULL;
    if (str_len < Size) {
        // Input already has a null terminator within bounds
        safe_str = (char *)str_start;
    } else {
        // No null terminator found, allocate a buffer and copy the data
        safe_str = (char *)malloc(Size + 1);
        if (!safe_str) {
            return 0;
        }
        memcpy(safe_str, Data, Size);
        safe_str[Size] = '\0';
    }

    // Step 1: Prepare environment and invoke target functions in specified order
    
    // 1. cJSON_CreateString
    cJSON *item1 = cJSON_CreateString(safe_str);
    if (!item1) {
        if (safe_str != (char *)str_start) free(safe_str);
        return 0;
    }

    // 2. cJSON_CreateArray
    cJSON *array = cJSON_CreateArray();
    if (!array) {
        cJSON_Delete(item1);
        if (safe_str != (char *)str_start) free(safe_str);
        return 0;
    }

    // 3. cJSON_CreateString
    cJSON *item2 = cJSON_CreateString(safe_str);
    if (!item2) {
        cJSON_Delete(item1);
        cJSON_Delete(array);
        if (safe_str != (char *)str_start) free(safe_str);
        return 0;
    }

    // 4. cJSON_CreateString
    cJSON *item3 = cJSON_CreateString(safe_str);
    if (!item3) {
        cJSON_Delete(item1);
        cJSON_Delete(item2);
        cJSON_Delete(array);
        if (safe_str != (char *)str_start) free(safe_str);
        return 0;
    }

    // 5. cJSON_CreateString
    cJSON *item4 = cJSON_CreateString(safe_str);
    if (!item4) {
        cJSON_Delete(item1);
        cJSON_Delete(item2);
        cJSON_Delete(item3);
        cJSON_Delete(array);
        if (safe_str != (char *)str_start) free(safe_str);
        return 0;
    }

    // 6. cJSON_InitHooks
    // Using NULL to reset to defaults, as per documentation
    cJSON_InitHooks(NULL);

    // 7. cJSON_Delete
    // Delete item1 as part of the sequence
    cJSON_Delete(item1);

    // 8. cJSON_GetArraySize
    int size = cJSON_GetArraySize(array);
    (void)size; // Suppress unused warning

    // 9. cJSON_AddItemToArray
    cJSON_AddItemToArray(array, item2);

    // 10. cJSON_AddItemToArray
    cJSON_AddItemToArray(array, item3);

    // 11. cJSON_AddItemToObject
    // We need an object to add to. Create a temporary object.
    cJSON *dummy_obj = cJSON_CreateObject();
    if (dummy_obj) {
        cJSON_AddItemToObject(dummy_obj, "key", item4);
        cJSON_Delete(dummy_obj);
    } else {
        // If object creation fails, we still need to clean up item4
        cJSON_Delete(item4);
    }

    // Cleanup remaining items
    // item1 was already deleted.
    // item2, item3 are owned by 'array'.
    // item4 was added to dummy_obj and dummy_obj was deleted.
    
    // Final cleanup of the main array
    cJSON_Delete(array);

    // Free the temporary buffer if we allocated one
    if (safe_str != (char *)str_start) {
        free(safe_str);
    }

    return 0;
}