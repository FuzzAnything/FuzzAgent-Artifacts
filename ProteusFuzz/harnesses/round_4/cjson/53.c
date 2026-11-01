// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void cleanup_cjson(cJSON *root) {
    if (root) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create a base object to manipulate
    cJSON *obj = cJSON_CreateObject();
    if (!obj) {
        return 0;
    }

    // Create some items to add/replace/delete
    cJSON *item1 = cJSON_CreateNumber(1.0);
    cJSON *item2 = cJSON_CreateString("test");
    cJSON *item3 = cJSON_CreateArray();
    cJSON *item4 = cJSON_CreateBool(1);

    if (!item1 || !item2 || !item3 || !item4) {
        cleanup_cjson(obj);
        // Only delete items that were successfully created
        if (item1) cJSON_Delete(item1);
        if (item2) cJSON_Delete(item2);
        if (item3) cJSON_Delete(item3);
        if (item4) cJSON_Delete(item4);
        return 0;
    }

    // Extract key and value strings from input data if possible
    // We use a fixed key for safety, but vary operations based on input
    const char *key1 = "key1";
    const char *key2 = "key2";
    const char *key3 = "key3";
    const char *key4 = "key4";
    const char *key5 = "key5";

    // Operation selection based on input data
    int op = Data[0] % 6;
    int seed = Data[1];

    cJSON_bool result = cJSON_False;

    // Track which items have been added to the object to avoid double-free
    int item1_added = 0;
    int item2_added = 0;
    int item3_added = 0;
    int item4_added = 0;

    switch (op) {
        case 0: {
            // Test cJSON_AddItemReferenceToObject
            // Ensure we have an item to reference first
            if (cJSON_AddItemToObject(obj, key1, item1)) {
                item1_added = 1;
                result = cJSON_AddItemReferenceToObject(obj, key2, item1);
            }
            break;
        }
        case 1: {
            // Test cJSON_AddItemToObjectCS
            // Note: This marks the key as const. 
            // We pass a static string literal for safety in fuzzing context.
            if (cJSON_AddItemToObjectCS(obj, key3, item2)) {
                item2_added = 1;
            }
            result = cJSON_True;
            break;
        }
        case 2: {
            // Test cJSON_DeleteItemFromObject
            // Add something first to ensure we have something to delete
            if (cJSON_AddItemToObject(obj, key4, item3)) {
                item3_added = 1;
            }
            cJSON_DeleteItemFromObject(obj, key4);
            // Try deleting non-existent
            cJSON_DeleteItemFromObject(obj, "nonexistent");
            result = cJSON_True; // Void return, just check no crash
            break;
        }
        case 3: {
            // Test cJSON_ReplaceItemInObjectCaseSensitive
            if (cJSON_AddItemToObject(obj, key5, item1)) {
                item1_added = 1;
            }
            // Replace item1 with item4. item4 becomes owned by obj.
            if (cJSON_ReplaceItemInObjectCaseSensitive(obj, key5, item4)) {
                item4_added = 1;
            }
            result = cJSON_True;
            break;
        }
        case 4: {
            // Test cJSON_AddItemToObject
            if (cJSON_AddItemToObject(obj, key1, item1)) {
                item1_added = 1;
            }
            // Try adding duplicate (might fail depending on config, but shouldn't crash)
            // Note: If this succeeds, item2 is now owned by obj.
            if (cJSON_AddItemToObject(obj, key1, item2)) {
                item2_added = 1;
            }
            break;
        }
        case 5: {
            // Test cJSON_HasObjectItem
            if (cJSON_AddItemToObject(obj, key2, item2)) {
                item2_added = 1;
            }
            result = cJSON_HasObjectItem(obj, key2);
            // Check non-existent
            cJSON_bool not_found = cJSON_HasObjectItem(obj, "missing");
            (void)not_found;
            break;
        }
    }

    (void)result; // Suppress unused variable warning
    (void)seed;   // Suppress unused variable warning

    // Cleanup
    // When items are added to an object, cJSON takes ownership.
    // We must NOT delete items that were successfully added to 'obj'.
    // If they were not added, we delete them manually.
    
    if (!item1_added) cJSON_Delete(item1);
    if (!item2_added) cJSON_Delete(item2);
    if (!item3_added) cJSON_Delete(item3);
    if (!item4_added) cJSON_Delete(item4);

    // Delete the root object, which will free all children it owns
    cleanup_cjson(obj);

    return 0;
}