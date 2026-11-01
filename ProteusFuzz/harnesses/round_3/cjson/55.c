#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

/* Helper to safely delete cJSON items */
static void safe_cleanup(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

/* Helper to create a cJSON item with a specific type and valuestring for edge cases */
static cJSON *create_raw_string_item(const char *str) {
    cJSON *item = cJSON_CreateString(str);
    if (item) {
        item->type = cJSON_Raw;
    }
    return item;
}

/* Helper to create an invalid type item */
static cJSON *create_invalid_type_item(void) {
    cJSON *item = cJSON_CreateString("dummy");
    if (item) {
        /* Set an invalid type to trigger default cases */
        item->type = -1; 
    }
    return item;
}

/* Helper to create a string item with NULL valuestring */
static cJSON *create_null_string_item(void) {
    cJSON *item = cJSON_CreateString("test");
    if (item) {
        /* Manually set valuestring to NULL to trigger line 3119 */
        item->valuestring = NULL;
        /* free(NULL) is safe, so cleanup is fine */
    }
    return item;
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure null termination for parsing */
    char *buffer = (char *)malloc(Size + 1);
    if (!buffer) {
        return 0;
    }
    memcpy(buffer, Data, Size);
    buffer[Size] = '\0';

    cJSON *root = cJSON_Parse(buffer);
    free(buffer);

    if (!root) {
        return 0;
    }

    /* Test Case 1: NULL pointer (Line 3074) */
    cJSON_Compare(NULL, root, 1);
    cJSON_Compare(root, NULL, 1);

    /* Test Case 2: Identical pointers (Line 3097) */
    cJSON_Compare(root, root, 1);

    /* Test Case 3: Mismatched types (Line 3074) */
    cJSON *num = cJSON_CreateNumber(123);
    cJSON *str = cJSON_CreateString("hello");
    if (num && str) {
        cJSON_Compare(num, str, 1);
    }

    /* Test Case 4: cJSON_Raw type (Line 3087) */
    cJSON *raw1 = create_raw_string_item("raw_data");
    cJSON *raw2 = create_raw_string_item("raw_data");
    if (raw1 && raw2) {
        cJSON_Compare(raw1, raw2, 1);
    }

    /* Test Case 5: Invalid type (Lines 3092, 3192) */
    cJSON *invalid1 = create_invalid_type_item();
    cJSON *invalid2 = create_invalid_type_item();
    if (invalid1 && invalid2) {
        cJSON_Compare(invalid1, invalid2, 1);
    }

    /* Test Case 6: NULL valuestring (Lines 3118-3119) */
    cJSON *null_str1 = create_null_string_item();
    cJSON *null_str2 = create_null_string_item();
    if (null_str1 && null_str2) {
        cJSON_Compare(null_str1, null_str2, 1);
    }

    /* Test Case 7: Array comparison loop (Line 3135) */
    cJSON *arr1 = cJSON_CreateArray();
    cJSON *arr2 = cJSON_CreateArray();
    if (arr1 && arr2) {
        cJSON_AddItemToArray(arr1, cJSON_CreateNumber(1));
        cJSON_AddItemToArray(arr2, cJSON_CreateNumber(1));
        cJSON_Compare(arr1, arr2, 1);
    }

    /* Test Case 8: Object comparison loop (Line 3183) */
    cJSON *obj1 = cJSON_CreateObject();
    cJSON *obj2 = cJSON_CreateObject();
    if (obj1 && obj2) {
        cJSON_AddItemToObject(obj1, "key", cJSON_CreateString("val"));
        cJSON_AddItemToObject(obj2, "key", cJSON_CreateString("val"));
        cJSON_Compare(obj1, obj2, 1);
    }

    /* Cleanup */
    safe_cleanup(root);
    safe_cleanup(num);
    safe_cleanup(str);
    safe_cleanup(raw1);
    safe_cleanup(raw2);
    safe_cleanup(invalid1);
    safe_cleanup(invalid2);
    safe_cleanup(null_str1);
    safe_cleanup(null_str2);
    safe_cleanup(arr1);
    safe_cleanup(arr2);
    safe_cleanup(obj1);
    safe_cleanup(obj2);

    return 0;
}