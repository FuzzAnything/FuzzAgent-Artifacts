// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddArrayToObject at cJSON.c:2246:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Step 1: Create the root object
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }

    // Step 2: Add a string to the object
    // Use input data as key and value, ensuring null termination
    const char *key_str = (const char *)Data;
    const char *val_str = (const char *)(Data + 1);
    
    // Ensure we don't read past bounds for key
    size_t key_len = strnlen(key_str, Size);
    if (key_len >= Size) key_len = Size - 1;
    
    // Create a safe copy for the key to ensure null termination
    char safe_key[256];
    size_t safe_key_len = (key_len < 255) ? key_len : 255;
    memcpy(safe_key, key_str, safe_key_len);
    safe_key[safe_key_len] = '\0';

    // Create a safe copy for the value
    char safe_val[256];
    size_t val_len = strnlen(val_str, Size - key_len - 1);
    if (val_len >= 255) val_len = 254;
    memcpy(safe_val, val_str, val_len);
    safe_val[val_len] = '\0';

    cJSON_AddStringToObject(root, safe_key, safe_val);

    // Step 3: Add an array to the object
    cJSON *array = cJSON_AddArrayToObject(root, "array_key");
    if (array == NULL) {
        cJSON_Delete(root);
        return 0;
    }

    // Step 4: Create a nested object
    cJSON *nested_obj = cJSON_CreateObject();
    if (nested_obj == NULL) {
        cJSON_Delete(root);
        return 0;
    }

    // Step 5 & 6: Add two numbers to the nested object
    // Use input data to generate numbers
    double num1 = 0.0;
    double num2 = 0.0;
    
    if (Size > 0) num1 = (double)Data[0] / 10.0;
    if (Size > 1) num2 = (double)Data[1] / 10.0;

    cJSON_AddNumberToObject(nested_obj, "num1", num1);
    cJSON_AddNumberToObject(nested_obj, "num2", num2);

    // Step 7: Add the nested object to the array
    cJSON_AddItemToArray(array, nested_obj);

    // Step 8: Print the JSON object
    char *json_str = cJSON_Print(root);
    if (json_str != NULL) {
        // The string is heap allocated, but we will let cJSON_Delete handle the tree.
        // However, cJSON_Print allocates a separate string that must be freed.
        // Note: cJSON_Delete does NOT free the string returned by cJSON_Print.
        free(json_str);
    }

    // Step 9: Delete the root object (which cleans up the entire tree)
    cJSON_Delete(root);

    return 0;
}