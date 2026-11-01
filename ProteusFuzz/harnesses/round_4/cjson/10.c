// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    int *numbers = NULL;
    int count = 0;

    if (Size > 0) {
        count = Data[0] % 100; 
        if (count > 0 && Size >= (size_t)(count + 1)) {
            numbers = (int *)malloc(count * sizeof(int));
            if (numbers) {
                for (int i = 0; i < count; i++) {
                    numbers[i] = Data[i + 1];
                }
            }
        }
    }

    cJSON *array = cJSON_CreateIntArray(numbers, count);
    if (numbers) {
        free(numbers);
    }

    if (array) {
        cJSON *item = cJSON_GetArrayItem(array, count > 0 ? (int)(Data[0] % count) : 0);
        (void)item;

        if (!cJSON_AddItemToObject(root, "int_array", array)) {
            cJSON_Delete(array);
        }
    }

    cJSON *obj1 = cJSON_CreateObject();
    if (obj1) {
        const char *str_val = (const char *)Data;
        size_t str_len = Size;
        if (str_len > 0 && Data[str_len - 1] == 0) {
            str_len--;
        }
        if (str_len > 0) {
            char *safe_str = (char *)malloc(str_len + 1);
            if (safe_str) {
                memcpy(safe_str, Data, str_len);
                safe_str[str_len] = '\0';
                cJSON *str_item = cJSON_CreateString(safe_str);
                if (str_item) {
                    if (!cJSON_AddItemToObject(obj1, "string_key", str_item)) {
                        cJSON_Delete(str_item);
                    }
                }
                free(safe_str);
            }
        }

        if (!cJSON_AddItemToObject(root, "nested_obj", obj1)) {
            cJSON_Delete(obj1);
            obj1 = NULL;
        }
    }

    cJSON *obj2 = cJSON_CreateObject();
    if (obj2) {
        const char *str_val2 = (const char *)Data;
        size_t str_len2 = Size;
        if (str_len2 > 0 && Data[str_len2 - 1] == 0) {
            str_len2--;
        }
        if (str_len2 > 0) {
            char *safe_str2 = (char *)malloc(str_len2 + 1);
            if (safe_str2) {
                memcpy(safe_str2, Data, str_len2);
                safe_str2[str_len2] = '\0';
                cJSON *str_item2 = cJSON_CreateString(safe_str2);
                if (str_item2) {
                    if (!cJSON_AddItemToObject(obj2, "string_key2", str_item2)) {
                        cJSON_Delete(str_item2);
                    }
                }
                free(safe_str2);
            }
        }

        if (!cJSON_AddItemToObject(root, "nested_obj2", obj2)) {
            cJSON_Delete(obj2);
            obj2 = NULL;
        }
    }

    cJSON_Delete(root);

    return 0;
}