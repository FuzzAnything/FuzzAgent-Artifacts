// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DetachItemFromArray at cJSON.c:2294:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON *create_test_array_from_input(const uint8_t *data, size_t size) {
    cJSON *root = cJSON_CreateArray();
    if (!root) {
        return NULL;
    }

    if (size == 0) {
        return root;
    }

    // Parse input bytes as potential JSON strings or numbers to add to the array
    // We create a buffer that is null-terminated to attempt JSON parsing or string creation
    char *buffer = (char *)malloc(size + 1);
    if (!buffer) {
        cJSON_Delete(root);
        return NULL;
    }
    memcpy(buffer, data, size);
    buffer[size] = '\0';

    // Try to parse the whole buffer as a JSON array or object first
    cJSON *parsed = cJSON_Parse(buffer);
    if (parsed) {
        if (parsed->type == cJSON_Array) {
            // If it's an array, we can merge its children into our root or just use it
            // To keep it simple, let's just return the parsed array if valid, 
            // but we need to ensure we don't leak the root we created.
            cJSON_Delete(root);
            free(buffer);
            return parsed;
        } else if (parsed->type == cJSON_Object) {
            // If it's an object, add it as a single item to our root array
            cJSON_AddItemToArray(root, parsed);
            free(buffer);
            return root;
        } else {
            // If it's a primitive, add it to the array
            cJSON_AddItemToArray(root, parsed);
            free(buffer);
            return root;
        }
    }

    // If parsing fails, treat the buffer as a raw string and add it
    cJSON *item = cJSON_CreateString(buffer);
    if (item) {
        cJSON_AddItemToArray(root, item);
    }
    
    free(buffer);
    return root;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare the environment
    // Create a base array from the fuzzing input
    cJSON *array = create_test_array_from_input(Data, Size);
    
    if (!array) {
        return 0;
    }

    // Ensure the root is actually an array for the target functions
    if (array->type != cJSON_Array) {
        cJSON_Delete(array);
        return 0;
    }

    // Derive an index for testing from the first byte of input
    int index = Data[0] % 100;
    
    // Also test negative indices and out-of-bounds
    int negative_index = -(index + 1);
    int large_index = 1000000;

    // 2. Invoke target functions diversely

    // Test cJSON_GetArraySize
    int size = cJSON_GetArraySize(array);
    (void)size; // Suppress unused warning

    // Test cJSON_GetArrayItem with valid, negative, and large indices
    cJSON *item_valid = cJSON_GetArrayItem(array, index);
    cJSON *item_neg = cJSON_GetArrayItem(array, negative_index);
    cJSON *item_large = cJSON_GetArrayItem(array, large_index);
    // No cleanup needed for GetArrayItem as it returns a pointer, not ownership

    // Test cJSON_DetachItemFromArray
    // We need a fresh array or ensure the item exists. 
    // Let's try to detach. If it returns NULL, it's safe.
    cJSON *detached = cJSON_DetachItemFromArray(array, index);
    if (detached) {
        // If detached, we own it now. We can delete it or re-add it.
        // Let's try re-adding it via reference to test AddItemReferenceToArray
        cJSON_AddItemReferenceToArray(array, detached);
        // Now delete the detached item (which is now referenced in array)
        // Note: Deleting a referenced item that is still in an array might be tricky depending on implementation
        // But cJSON_Delete usually handles the chain. 
        // To be safe for fuzzing, let's just delete it if it's not in the array anymore?
        // Actually, if we added it back, it's in the array. 
        // Let's just delete it directly to see how the library handles it.
        cJSON_Delete(detached);
    }

    // Test cJSON_DeleteItemFromArray
    // We need to ensure there is an item to delete.
    // If the array was modified by detach, we might need to add something back.
    if (cJSON_GetArraySize(array) > 0) {
        cJSON_DeleteItemFromArray(array, 0);
    }

    // Test cJSON_AddItemToArray and cJSON_AddItemReferenceToArray
    // Create a new item to add
    cJSON *new_item = cJSON_CreateNumber(123.45);
    if (new_item) {
        // Add by ownership
        cJSON_AddItemToArray(array, new_item);
        
        // Create another item for reference
        cJSON *ref_item = cJSON_CreateString("fuzz_ref");
        if (ref_item) {
            // Add by reference
            cJSON_AddItemReferenceToArray(array, ref_item);
            // ref_item is now part of the array. We should not delete it manually here
            // unless we detach it first, to avoid double free or corruption.
            // Let's leave it in the array for the final cleanup of 'array'.
        }
    }

    // 3. Cleanup
    cJSON_Delete(array);

    return 0;
}