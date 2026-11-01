// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    cJSON *root = NULL;
    cJSON *array = NULL;
    cJSON *obj1 = NULL;
    cJSON *str1 = NULL;
    cJSON *obj2 = NULL;
    cJSON *str2 = NULL;
    cJSON *item = NULL;
    int *int_array = NULL;
    int count = 0;
    int index = 0;

    if (Size < 2) {
        return 0;
    }

    // Parse input to determine array size and index
    count = Data[0];
    index = Data[1];

    // Limit count to prevent excessive allocation and ensure we have enough data
    if (count > 1000) {
        count = 1000;
    }
    
    // Ensure we have enough data bytes for the array
    // We need 2 bytes for count/index + count bytes for the array
    if (Size < 2 + (size_t)count) {
        count = (int)(Size - 2);
        if (count < 0) {
            count = 0;
        }
    }

    // Allocate and populate int array from input
    if (count > 0) {
        int_array = (int *)malloc(count * sizeof(int));
        if (!int_array) {
            return 0;
        }
        for (int i = 0; i < count; i++) {
            int_array[i] = (int)Data[2 + i];
        }
    }

    // 1. cJSON_CreateObject
    root = cJSON_CreateObject();
    if (!root) {
        free(int_array);
        return 0;
    }

    // 2. cJSON_CreateIntArray
    array = cJSON_CreateIntArray(int_array, count);
    if (!array) {
        cJSON_Delete(root);
        free(int_array);
        return 0;
    }

    // 3. cJSON_GetArrayItem
    if (index >= 0 && index < count) {
        item = cJSON_GetArrayItem(array, index);
        // item is read-only, no cleanup needed for the item itself
    }

    // 4. cJSON_AddItemToObject (add array to root)
    if (!cJSON_AddItemToObject(root, "array", array)) {
        cJSON_Delete(root);
        cJSON_Delete(array);
        free(int_array);
        return 0;
    }
    // Note: array ownership is transferred to root, so we should not delete it separately

    // 5. cJSON_CreateObject
    obj1 = cJSON_CreateObject();
    if (!obj1) {
        cJSON_Delete(root);
        free(int_array);
        return 0;
    }

    // 6. cJSON_CreateString
    if (Size > 10) {
        // Allocate Size - 10 + 1 for the null terminator
        size_t str_len = Size - 10;
        char *str_val = (char *)malloc(str_len + 1);
        if (str_val) {
            memcpy(str_val, &Data[10], str_len);
            str_val[str_len] = '\0';
            str1 = cJSON_CreateString(str_val);
            free(str_val);
        }
    } else {
        str1 = cJSON_CreateString("default");
    }

    if (!str1) {
        cJSON_Delete(obj1);
        cJSON_Delete(root);
        free(int_array);
        return 0;
    }

    // 7. cJSON_AddItemToObject (add string to obj1)
    if (!cJSON_AddItemToObject(obj1, "string1", str1)) {
        cJSON_Delete(obj1);
        cJSON_Delete(root);
        free(int_array);
        return 0;
    }

    // 8. cJSON_CreateObject
    obj2 = cJSON_CreateObject();
    if (!obj2) {
        cJSON_Delete(obj1);
        cJSON_Delete(root);
        free(int_array);
        return 0;
    }

    // 9. cJSON_CreateString
    if (Size > 20) {
        // Allocate Size - 20 + 1 for the null terminator
        size_t str_len2 = Size - 20;
        char *str_val2 = (char *)malloc(str_len2 + 1);
        if (str_val2) {
            memcpy(str_val2, &Data[20], str_len2);
            str_val2[str_len2] = '\0';
            str2 = cJSON_CreateString(str_val2);
            free(str_val2);
        }
    } else {
        str2 = cJSON_CreateString("default2");
    }

    if (!str2) {
        cJSON_Delete(obj2);
        cJSON_Delete(obj1);
        cJSON_Delete(root);
        free(int_array);
        return 0;
    }

    // 10. cJSON_AddItemToObject (add string to obj2)
    if (!cJSON_AddItemToObject(obj2, "string2", str2)) {
        cJSON_Delete(obj2);
        cJSON_Delete(obj1);
        cJSON_Delete(root);
        free(int_array);
        return 0;
    }

    // Add obj1 and obj2 to root
    if (!cJSON_AddItemToObject(root, "obj1", obj1)) {
        cJSON_Delete(obj1);
        cJSON_Delete(obj2);
        cJSON_Delete(root);
        free(int_array);
        return 0;
    }

    if (!cJSON_AddItemToObject(root, "obj2", obj2)) {
        cJSON_Delete(obj2);
        cJSON_Delete(root);
        free(int_array);
        return 0;
    }

    // 11. cJSON_Delete (root) - this will recursively delete everything
    cJSON_Delete(root);

    // 12. cJSON_Delete (obj1) - already deleted via root, set to NULL to avoid double free
    obj1 = NULL;
    cJSON_Delete(obj1);

    // 13. cJSON_Delete (obj2) - already deleted via root, set to NULL to avoid double free
    obj2 = NULL;
    cJSON_Delete(obj2);

    free(int_array);
    return 0;
}