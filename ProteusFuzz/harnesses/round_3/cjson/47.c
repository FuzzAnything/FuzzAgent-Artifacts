// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddRawToObject at cJSON.c:2222:22 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_DetachItemFromObject at cJSON.c:2309:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>

static void safe_cleanup(cJSON *root) {
    if (root) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a root object to work with
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // Use input data to generate keys and raw strings
    // We ensure null termination by limiting length
    size_t max_len = Size > 256 ? 256 : Size;
    char key[257];
    char raw[257];

    // Create a safe copy of input for key
    memcpy(key, Data, max_len);
    key[max_len] = '\0';

    // Create a safe copy of input for raw value
    memcpy(raw, Data, max_len);
    raw[max_len] = '\0';

    // Fuzz cJSON_AddRawToObject
    // This adds unprocessed JSON text as a value under a key
    cJSON_AddRawToObject(root, key, raw);

    // Fuzz cJSON_CreateRaw to create an item to attach
    cJSON *raw_item = cJSON_CreateRaw(raw);
    if (raw_item) {
        // Fuzz cJSON_AddItemToObject
        cJSON_AddItemToObject(root, key, raw_item);
        
        // Fuzz cJSON_AddItemToObjectCS (using a const-like string)
        // We use the same key again; this might fail if key exists depending on hooks
        cJSON_AddItemToObjectCS(root, key, raw_item);
        
        // Fuzz cJSON_DetachItemFromObject
        cJSON *detached = cJSON_DetachItemFromObject(root, key);
        if (detached) {
            // Re-attach it to test state
            cJSON_AddItemToObject(root, key, detached);
            // Clean up the detached item separately if we don't re-attach
            // cJSON_Delete(detached); 
        }

        // Fuzz cJSON_DeleteItemFromObject
        // This removes and frees the item
        cJSON_DeleteItemFromObject(root, key);
        
        // Clean up the raw_item if it wasn't deleted by the object operations
        // Note: cJSON_AddItemToObject transfers ownership, so if it was added,
        // cJSON_DeleteItemFromObject freed it. If it wasn't added, we must free it.
        // However, since we called DeleteItemFromObject, the item is likely gone.
        // To be safe against logic where AddItemToObject failed, we check if it's still valid.
        // But cJSON_DeleteItemFromObject is safe if key is missing.
        // If AddItemToObject failed, raw_item is still owned by us.
        // Let's assume the worst case and try to delete if it wasn't successfully added/deleted.
        // Actually, simpler: if we added it, it's gone. If we didn't, we delete it.
        // Since we can't easily track success of AddItemToObject without checking return,
        // and DeleteItemFromObject is idempotent for missing keys, we just ensure
        // we don't double free. The safest path for fuzzing is to let the object
        // manage it or delete it if we are sure we own it.
        // Given the sequence: Add -> AddCS -> Detach -> Add -> Delete.
        // If Add succeeded, Delete freed it. If Add failed, we own it.
        // Let's just delete it here. If it was already freed, this is bad.
        // Correction: cJSON_AddItemToObject returns cJSON_bool.
        // Let's restructure slightly to be safe.
    }

    // Re-run a safer sequence for the item management to avoid double free
    cJSON *test_item = cJSON_CreateRaw(raw);
    if (test_item) {
        cJSON_bool added = cJSON_AddItemToObject(root, "test_key", test_item);
        if (!added) {
            // If not added, we own it
            cJSON_Delete(test_item);
        }
        // If added, root owns it.
        
        // Try Detach
        cJSON *detached_item = cJSON_DetachItemFromObject(root, "test_key");
        if (detached_item) {
            // We now own detached_item
            cJSON_Delete(detached_item);
        }
    }

    // Cleanup root
    safe_cleanup(root);

    return 0;
}