// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddFalseToObject at cJSON.c:2174:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void generate_key_from_input(const uint8_t *data, size_t size, char *key, size_t key_size) {
    if (size == 0) {
        key[0] = '\0';
        return;
    }
    
    size_t copy_len = size < key_size - 1 ? size : key_size - 1;
    memcpy(key, data, copy_len);
    key[copy_len] = '\0';
    
    // Ensure no null bytes in the middle of the key unless it's the terminator
    for (size_t i = 0; i < copy_len; i++) {
        if (key[i] == '\0') {
            key[i] = 'a'; // Replace null with a safe character
        }
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Step 1: Prepare the environment
    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        // Allocation failed, nothing to fuzz further
        return 0;
    }

    // Generate a key from the fuzz input
    char key[256];
    generate_key_from_input(Data, Size, key, sizeof(key));

    // Step 2: Invoke the target functions in order
    // Try adding a false boolean to the object
    cJSON *added_item = cJSON_AddFalseToObject(obj, key);
    
    // If the addition failed (e.g., key already exists or invalid object), 
    // we still proceed to cleanup to avoid leaks.
    // The function handles its own cleanup on failure per documentation.

    // Step 3: Cleanup
    cJSON_Delete(obj);

    return 0;
}