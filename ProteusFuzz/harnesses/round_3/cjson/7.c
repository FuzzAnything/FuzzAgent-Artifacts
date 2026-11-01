// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_InitHooks at cJSON.c:209:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void *safe_malloc(size_t sz) {
    return malloc(sz);
}

static void safe_free(void *ptr) {
    free(ptr);
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. cJSON_CreateString
    // Ensure the input is null-terminated for string creation
    char *input_str = NULL;
    if (Size < 1024) {
        input_str = (char *)malloc(Size + 1);
        if (!input_str) return 0;
        memcpy(input_str, Data, Size);
        input_str[Size] = '\0';
    } else {
        // Truncate if too large
        input_str = (char *)malloc(1024);
        if (!input_str) return 0;
        memcpy(input_str, Data, 1023);
        input_str[1023] = '\0';
        Size = 1023;
    }

    cJSON *str1 = cJSON_CreateString(input_str);
    free(input_str);

    if (!str1) {
        return 0;
    }

    // 2. cJSON_CreateArray
    cJSON *arr = cJSON_CreateArray();
    if (!arr) {
        cJSON_Delete(str1);
        return 0;
    }

    // 3. cJSON_CreateString
    // FIX: Pass the null-terminated 'input_str' (re-allocated) or ensure Data is terminated.
    // Since 'input_str' was freed, we must create a new null-terminated buffer from Data.
    char *safe_data_str = NULL;
    if (Size < 1024) {
        safe_data_str = (char *)malloc(Size + 1);
        if (!safe_data_str) {
            cJSON_Delete(str1);
            cJSON_Delete(arr);
            return 0;
        }
        memcpy(safe_data_str, Data, Size);
        safe_data_str[Size] = '\0';
    } else {
        safe_data_str = (char *)malloc(1024);
        if (!safe_data_str) {
            cJSON_Delete(str1);
            cJSON_Delete(arr);
            return 0;
        }
        memcpy(safe_data_str, Data, 1023);
        safe_data_str[1023] = '\0';
    }

    cJSON *str2 = cJSON_CreateString(safe_data_str);
    free(safe_data_str);

    if (!str2) {
        cJSON_Delete(str1);
        cJSON_Delete(arr);
        return 0;
    }

    // 4. cJSON_CreateString
    cJSON *str3 = cJSON_CreateString("fuzz_key");
    if (!str3) {
        cJSON_Delete(str1);
        cJSON_Delete(str2);
        cJSON_Delete(arr);
        return 0;
    }

    // 5. cJSON_CreateString
    cJSON *str4 = cJSON_CreateString("fuzz_value");
    if (!str4) {
        cJSON_Delete(str1);
        cJSON_Delete(str2);
        cJSON_Delete(str3);
        cJSON_Delete(arr);
        return 0;
    }

    // 6. cJSON_InitHooks
    cJSON_Hooks hooks = {
        .malloc_fn = safe_malloc,
        .free_fn = safe_free
    };
    cJSON_InitHooks(&hooks);

    // 7. cJSON_Delete (Cleanup of temporary strings before adding to array to avoid ownership issues if logic changes)
    // Note: The prompt asks for specific order. We delete str1 here as per sequence.
    cJSON_Delete(str1);

    // 8. cJSON_GetArraySize
    int size = cJSON_GetArraySize(arr);
    (void)size; // Suppress unused warning

    // 9. cJSON_AddItemToArray
    // Add str2 to array
    cJSON_AddItemToArray(arr, str2);

    // 10. cJSON_AddItemToArray
    // Add str4 to array (str3 is a key, usually added to object, but we follow sequence)
    cJSON_AddItemToArray(arr, str4);

    // 11. cJSON_AddItemToObject
    // We need an object to add to. Let's create one dynamically since we deleted str1 (which was a string, not object).
    // To strictly follow the sequence without breaking logic, we create a new object here.
    cJSON *obj = cJSON_CreateObject();
    if (obj) {
        // str3 is "fuzz_key", str4 is already in array. We can't add str4 again easily without copying or reusing.
        // Let's use str3 as the key and create a new value or reuse str2 if not deleted.
        // Since str2 is now in 'arr', adding it to 'obj' would move it (cJSON behavior).
        // Let's create a new string for the value to be safe.
        cJSON *val = cJSON_CreateString("object_value");
        if (val) {
            cJSON_AddItemToObject(obj, "test_key", val);
            cJSON_Delete(obj); // Cleanup object
        } else {
            cJSON_Delete(obj);
        }
    }

    // Final cleanup
    cJSON_Delete(arr);
    cJSON_Delete(str3);
    
    return 0;
}