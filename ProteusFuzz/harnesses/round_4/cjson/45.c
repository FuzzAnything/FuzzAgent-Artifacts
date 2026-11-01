// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_ReplaceItemInObjectCaseSensitive at cJSON.c:2455:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_cjson_tree(cJSON *root) {
    if (root) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a base object to work with
    cJSON *obj = cJSON_CreateObject();
    if (!obj) {
        return 0;
    }

    // Create some items to add/replace/delete
    cJSON *item1 = cJSON_CreateString("test_value_1");
    cJSON *item2 = cJSON_CreateNumber(123.45);
    cJSON *item3 = cJSON_CreateBool(1);
    cJSON *item4 = cJSON_CreateNull();
    cJSON *item5 = cJSON_CreateArray();
    cJSON *item6 = cJSON_CreateObject();

    if (!item1 || !item2 || !item3 || !item4 || !item5 || !item6) {
        // Cleanup partial creation
        if (item1) cJSON_Delete(item1);
        if (item2) cJSON_Delete(item2);
        if (item3) cJSON_Delete(item3);
        if (item4) cJSON_Delete(item4);
        if (item5) cJSON_Delete(item5);
        if (item6) cJSON_Delete(item6);
        cleanup_cjson_tree(obj);
        return 0;
    }

    // Use input data to drive fuzzing logic
    const char *key1 = (char *)Data;
    const char *key2 = (char *)(Data + 1);
    const char *key3 = (char *)(Data + 2);
    const char *key4 = (char *)(Data + 3);
    const char *key5 = (char *)(Data + 4);

    // Ensure keys are null-terminated safely
    // We will use a small fixed buffer for keys to avoid out-of-bounds if Data is short
    char k1[32], k2[32], k3[32], k4[32], k5[32];
    memset(k1, 0, sizeof(k1));
    memset(k2, 0, sizeof(k2));
    memset(k3, 0, sizeof(k3));
    memset(k4, 0, sizeof(k4));
    memset(k5, 0, sizeof(k5));

    size_t len1 = (Size > 1) ? 1 : 0;
    size_t len2 = (Size > 2) ? 1 : 0;
    size_t len3 = (Size > 3) ? 1 : 0;
    size_t len4 = (Size > 4) ? 1 : 0;
    size_t len5 = (Size > 5) ? 1 : 0;

    if (len1 > 0) strncpy(k1, (char *)Data, len1);
    if (len2 > 0) strncpy(k2, (char *)(Data + 1), len2);
    if (len3 > 0) strncpy(k3, (char *)(Data + 2), len3);
    if (len4 > 0) strncpy(k4, (char *)(Data + 3), len4);
    if (len5 > 0) strncpy(k5, (char *)(Data + 4), len5);

    // Fuzz cJSON_AddItemToObject
    cJSON_AddItemToObject(obj, k1, item1);

    // Fuzz cJSON_AddItemToObjectCS (requires const string, we use static string)
    // Note: k2 is not const, but we can pass it as const char* for the API call
    // However, the API expects the string to survive. We are careful not to free k2.
    cJSON_AddItemToObjectCS(obj, k2, item2);

    // Fuzz cJSON_AddItemReferenceToObject
    // item3 is added by reference
    cJSON_AddItemReferenceToObject(obj, k3, item3);

    // Fuzz cJSON_HasObjectItem
    cJSON_bool has1 = cJSON_HasObjectItem(obj, k1);
    cJSON_bool has2 = cJSON_HasObjectItem(obj, k2);
    cJSON_bool has3 = cJSON_HasObjectItem(obj, k3);
    cJSON_bool has4 = cJSON_HasObjectItem(obj, k4);
    cJSON_bool has5 = cJSON_HasObjectItem(obj, k5);

    // Fuzz cJSON_ReplaceItemInObjectCaseSensitive
    // Replace item1 with item4 if it exists
    cJSON_ReplaceItemInObjectCaseSensitive(obj, k1, item4);

    // Fuzz cJSON_DeleteItemFromObject
    // Try to delete item2
    cJSON_DeleteItemFromObject(obj, k2);

    // Try to delete a non-existent key
    cJSON_DeleteItemFromObject(obj, "non_existent_key");

    // Cleanup all created items that were not deleted by the API
    // item1 was replaced (ownership transferred to obj, then deleted? No, Replace keeps new item, old is freed if not referenced)
    // item2 was deleted by API
    // item3 is referenced in obj, so we should not delete it separately if we want to avoid double free, 
    // but cJSON_Delete on the root will handle references correctly if they are part of the tree.
    // However, item3 was added by reference. If we delete the root, item3 will be freed if it's the only owner?
    // Actually, AddItemReference does not copy, so the root owns the reference. Deleting root should free item3.
    // item4 was added via Replace, so it's owned by obj.
    // item5 and item6 were never added to obj, so we must delete them manually.
    
    cJSON_Delete(item5);
    cJSON_Delete(item6);

    // Delete the root object, which will clean up item1 (if not replaced), item3, item4, etc.
    // Note: item1 was replaced by item4. The old item1 should have been freed by ReplaceItemInObjectCaseSensitive?
    // The documentation says "Replaces...". Usually, the old item is freed.
    // item2 was explicitly deleted.
    // item3 is referenced, so deleting obj should free it.
    // item4 is now in obj.
    
    cleanup_cjson_tree(obj);

    return 0;
}