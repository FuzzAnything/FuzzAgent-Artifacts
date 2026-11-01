// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare data for cJSON_CreateIntArray
    int *int_array = NULL;
    int count = 0;
    
    // Determine count safely from input
    if (Size >= sizeof(int)) {
        count = Data[0];
        if (count < 0) count = 0;
        if (count > 100) count = 100; // Limit to prevent excessive allocation
    }
    
    if (count > 0) {
        int_array = (int *)malloc(count * sizeof(int));
        if (!int_array) {
            return 0;
        }
        // Fill with data from input, ensuring valid range
        for (int i = 0; i < count; i++) {
            int_array[i] = (int)Data[i % Size];
        }
    }

    // Step 1: cJSON_CreateObject
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        if (int_array) free(int_array);
        return 0;
    }

    // Step 2: cJSON_CreateIntArray
    cJSON *int_array_json = cJSON_CreateIntArray(int_array, count);
    if (int_array) {
        free(int_array);
    }

    // Step 3: cJSON_GetArrayItem
    // Use input data to determine index, but ensure safety
    int index = (Size > 0) ? (int)Data[0] : 0;
    if (index < 0) index = 0;
    if (index > 100) index = 100;
    
    cJSON *item = cJSON_GetArrayItem(int_array_json, index);
    (void)item; // Use the result to avoid unused variable warning

    // Step 4: cJSON_AddItemToObject (Add int array)
    cJSON_AddItemToObject(root, "int_array", int_array_json);

    // Step 5: cJSON_CreateObject (Nested object 1)
    cJSON *nested_obj1 = cJSON_CreateObject();
    if (!nested_obj1) {
        cJSON_Delete(root);
        return 0;
    }

    // Step 6: cJSON_CreateString
    // Create a safe string from input
    char *str_buf = NULL;
    if (Size > 0) {
        size_t len = (Size < 256) ? Size : 255;
        str_buf = (char *)malloc(len + 1);
        if (str_buf) {
            memcpy(str_buf, Data, len);
            str_buf[len] = '\0';
        }
    } else {
        str_buf = (char *)malloc(1);
        if (str_buf) str_buf[0] = '\0';
    }

    cJSON *str_json = NULL;
    if (str_buf) {
        str_json = cJSON_CreateString(str_buf);
        free(str_buf);
    } else {
        str_json = cJSON_CreateString("default");
    }

    // Step 7: cJSON_AddItemToObject (Add string to nested_obj1)
    if (str_json) {
        cJSON_AddItemToObject(nested_obj1, "key1", str_json);
    }

    // Step 8: cJSON_CreateObject (Nested object 2)
    cJSON *nested_obj2 = cJSON_CreateObject();
    if (!nested_obj2) {
        cJSON_Delete(root);
        return 0;
    }

    // Step 9: cJSON_CreateString (Another string)
    cJSON *str_json2 = NULL;
    if (Size > 1) {
        size_t len = (Size - 1 < 256) ? (Size - 1) : 255;
        str_buf = (char *)malloc(len + 1);
        if (str_buf) {
            memcpy(str_buf, Data + 1, len);
            str_buf[len] = '\0';
            str_json2 = cJSON_CreateString(str_buf);
            free(str_buf);
        }
    }
    if (!str_json2) {
        str_json2 = cJSON_CreateString("default2");
    }

    // Step 10: cJSON_AddItemToObject (Add string to nested_obj2)
    if (str_json2) {
        cJSON_AddItemToObject(nested_obj2, "key2", str_json2);
    }

    // Add nested objects to root
    cJSON_AddItemToObject(root, "nested1", nested_obj1);
    cJSON_AddItemToObject(root, "nested2", nested_obj2);

    // Step 11, 12, 13: cJSON_Delete
    // Delete the root which will recursively delete all children
    cJSON_Delete(root);

    return 0;
}