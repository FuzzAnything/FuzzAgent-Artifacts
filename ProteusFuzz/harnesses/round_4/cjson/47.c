// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_DetachItemViaPointer at cJSON.c:2258:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_CreateObjectReference at cJSON.c:2560:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_CreateObjectReference at cJSON.c:2560:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cJSON *create_random_cjson(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }

    // Create a buffer with a null terminator
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
        return NULL;
    }
    memcpy(json_str, data, size);
    json_str[size] = '\0';

    cJSON *item = cJSON_Parse(json_str);
    free(json_str);

    if (!item) {
        // If parsing fails, try to create a simple valid object manually
        // to ensure we have something to test with, using parts of the fuzz data
        cJSON *obj = cJSON_CreateObject();
        if (!obj) return NULL;
        
        if (size > 0) {
            char key[32];
            size_t key_len = (size < 31) ? size : 31;
            memcpy(key, data, key_len);
            key[key_len] = '\0';
            cJSON_AddNumberToObject(obj, key, (double)data[0]);
        }
        return obj;
    }
    return item;
}

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create a base JSON object from input
    cJSON *base = create_random_cjson(Data, Size);
    if (!base) {
        return 0;
    }

    // Ensure base is an object or array for the tests
    if (!cJSON_IsObject(base) && !cJSON_IsArray(base)) {
        // Wrap in an object if it's a primitive
        cJSON *wrapper = cJSON_CreateObject();
        if (wrapper) {
            cJSON_AddItemToObject(wrapper, "root", base);
            base = wrapper;
        } else {
            cleanup_cjson(base);
            return 0;
        }
    }

    // Create a child item to use for reference tests
    cJSON *child = cJSON_CreateNumber(123.0);
    if (!child) {
        cleanup_cjson(base);
        return 0;
    }

    // Add child to base if base is an object or array
    if (cJSON_IsObject(base)) {
        cJSON_AddItemToObject(base, "child", child);
    } else if (cJSON_IsArray(base)) {
        cJSON_AddItemToArray(base, child);
    }

    // --- Test 1: cJSON_DeleteItemFromArray ---
    // We need an array to test this. Let's create one or use existing.
    cJSON *test_array = cJSON_CreateArray();
    if (test_array) {
        cJSON *item1 = cJSON_CreateNumber(1);
        cJSON *item2 = cJSON_CreateNumber(2);
        cJSON *item3 = cJSON_CreateNumber(3);
        if (item1 && item2 && item3) {
            cJSON_AddItemToArray(test_array, item1);
            cJSON_AddItemToArray(test_array, item2);
            cJSON_AddItemToArray(test_array, item3);
            
            // Delete valid index
            cJSON_DeleteItemFromArray(test_array, 1);
            
            // Delete out of bounds (safe)
            cJSON_DeleteItemFromArray(test_array, 100);
            
            // Delete on NULL array (safe)
            cJSON_DeleteItemFromArray(NULL, 0);
            
            // Delete on non-array (undefined behavior in some versions, but we test safety)
            // We won't pass a non-array here to avoid crashing if implementation assumes array type
        }
        cleanup_cjson(test_array);
    }

    // --- Test 2: cJSON_AddItemReferenceToArray ---
    cJSON *ref_array = cJSON_CreateArray();
    if (ref_array) {
        cJSON *ref_item = cJSON_CreateString("test_ref");
        if (ref_item) {
            cJSON_AddItemReferenceToArray(ref_array, ref_item);
            // Try adding NULL item
            cJSON_AddItemReferenceToArray(ref_array, NULL);
            // Try adding to NULL array
            cJSON_AddItemReferenceToArray(NULL, ref_item);
            cleanup_cjson(ref_item); // Clean up the original, ref_array holds a reference
        }
        cleanup_cjson(ref_array);
    }

    // --- Test 3: cJSON_AddItemReferenceToObject ---
    cJSON *ref_obj = cJSON_CreateObject();
    if (ref_obj) {
        cJSON *ref_obj_item = cJSON_CreateNumber(42);
        if (ref_obj_item) {
            cJSON_AddItemReferenceToObject(ref_obj, "key_ref", ref_obj_item);
            // Try NULL object
            cJSON_AddItemReferenceToObject(NULL, "key", ref_obj_item);
            // Try NULL string
            cJSON_AddItemReferenceToObject(ref_obj, NULL, ref_obj_item);
            // Try NULL item
            cJSON_AddItemReferenceToObject(ref_obj, "key2", NULL);
            cleanup_cjson(ref_obj_item);
        }
        cleanup_cjson(ref_obj);
    }

    // --- Test 4: cJSON_DetachItemViaPointer ---
    cJSON *detach_parent = cJSON_CreateArray();
    if (detach_parent) {
        cJSON *detach_child = cJSON_CreateString("detach_me");
        if (detach_child) {
            cJSON_AddItemToArray(detach_parent, detach_child);
            
            // Detach valid item
            cJSON *detached = cJSON_DetachItemViaPointer(detach_parent, detach_child);
            if (detached) {
                // detached is now owned by us, free it
                cleanup_cjson(detached);
            }
            
            // Detach from NULL parent
            cJSON_DetachItemViaPointer(NULL, detach_child);
            
            // Detach NULL item
            cJSON_DetachItemViaPointer(detach_parent, NULL);
            
            // Detach item not in parent (create a new one)
            cJSON *orphan = cJSON_CreateNumber(99);
            cJSON_DetachItemViaPointer(detach_parent, orphan);
            cleanup_cjson(orphan);
        }
        cleanup_cjson(detach_parent);
    }

    // --- Test 5: cJSON_CreateObjectReference ---
    cJSON *ref_source = cJSON_CreateObject();
    if (ref_source) {
        cJSON_AddNumberToObject(ref_source, "num", 10);
        
        cJSON *ref_copy = cJSON_CreateObjectReference(ref_source);
        if (ref_copy) {
            // Verify it's a reference
            // Deleting the reference should not free the source
            cleanup_cjson(ref_copy);
            // Source should still be valid
            if (cJSON_GetObjectItem(ref_source, "num")) {
                // Success
            }
        }
        // Test with NULL
        cJSON_CreateObjectReference(NULL);
        cleanup_cjson(ref_source);
    }

    // --- Test 6: cJSON_DeleteItemFromObject ---
    cJSON *del_obj = cJSON_CreateObject();
    if (del_obj) {
        cJSON_AddNumberToObject(del_obj, "to_delete", 5);
        cJSON_AddNumberToObject(del_obj, "keep", 6);
        
        // Delete existing
        cJSON_DeleteItemFromObject(del_obj, "to_delete");
        
        // Delete non-existing (safe)
        cJSON_DeleteItemFromObject(del_obj, "non_existent");
        
        // Delete with NULL string (safe)
        cJSON_DeleteItemFromObject(del_obj, NULL);
        
        // Delete from NULL object (safe)
        cJSON_DeleteItemFromObject(NULL, "key");
        
        cleanup_cjson(del_obj);
    }

    // Cleanup main base
    cleanup_cjson(base);

    return 0;
}