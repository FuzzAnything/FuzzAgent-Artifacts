// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_ReplaceItemInObject at cJSON.c:2450:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

static void safe_delete(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Step 1: Prepare environment
    // Create the root object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // We need to generate two numbers based on input data
    double num1 = 0.0;
    double num2 = 0.0;

    // Extract doubles from input if possible
    if (Size >= 8) {
        memcpy(&num1, Data, sizeof(double));
        if (Size >= 16) {
            memcpy(&num2, Data + 8, sizeof(double));
        } else {
            num2 = num1;
        }
    } else if (Size >= 4) {
        int32_t int_val;
        memcpy(&int_val, Data, sizeof(int32_t));
        num1 = (double)int_val;
        num2 = (double)int_val;
    } else {
        num1 = (double)Data[0];
        num2 = (double)Data[0];
    }

    // Invoke: cJSON_CreateNumber (first)
    cJSON *num_node1 = cJSON_CreateNumber(num1);
    if (!num_node1) {
        safe_delete(root);
        return 0;
    }

    // Invoke: cJSON_CreateNumber (second)
    cJSON *num_node2 = cJSON_CreateNumber(num2);
    if (!num_node2) {
        safe_delete(num_node1);
        safe_delete(root);
        return 0;
    }

    // Generate a key string from input
    char key[32];
    size_t key_len = (Size > 31) ? 31 : Size;
    if (key_len == 0) {
        key[0] = 'k';
        key[1] = '\0';
    } else {
        memcpy(key, Data, key_len);
        key[key_len] = '\0';
        // Ensure null termination and valid chars (simple sanitization)
        for (size_t i = 0; i < key_len; i++) {
            if (key[i] < 32 || key[i] > 126) {
                key[i] = 'x';
            }
        }
    }

    // Invoke: cJSON_AddItemToObject
    // Add the first number to the object
    if (!cJSON_AddItemToObject(root, key, num_node1)) {
        // If add fails (e.g., key exists or invalid object), we still clean up
        safe_delete(num_node1);
        safe_delete(num_node2);
        safe_delete(root);
        return 0;
    }

    // Invoke: cJSON_ReplaceItemInObject
    // Replace the item we just added with the second number
    // IMPORTANT: cJSON_ReplaceItemInObject internally calls cJSON_Delete on the OLD item (num_node1)
    // before replacing it with the new item (num_node2).
    // Therefore, num_node1 is already freed by the library if the replacement succeeds.
    // We must NOT call safe_delete(num_node1) after a successful replacement.
    
    if (cJSON_ReplaceItemInObject(root, key, num_node2)) {
        // Success: 
        // 1. The old item (num_node1) was automatically deleted by cJSON_ReplaceItemInObject.
        // 2. The new item (num_node2) is now owned by 'root'.
        // We do nothing with num_node1 or num_node2 here.
    } else {
        // Failure: 
        // 1. The old item (num_node1) is still in 'root'.
        // 2. The new item (num_node2) was NOT added and is orphaned.
        // We must free num_node2 manually.
        safe_delete(num_node2);
    }

    // Invoke: cJSON_Delete
    // Clean up the root object (which holds num_node1 if replace failed, or num_node2 if succeeded)
    safe_delete(root);

    return 0;
}