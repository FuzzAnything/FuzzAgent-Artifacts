// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void cleanup_json_tree(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a root object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // Create some items to add
    cJSON *item1 = cJSON_CreateNumber(1);
    cJSON *item2 = cJSON_CreateNumber(2);
    cJSON *item3 = cJSON_CreateString("test");
    cJSON *item4 = cJSON_CreateArray();
    cJSON *item5 = cJSON_CreateBool(1);
    cJSON *item6 = cJSON_CreateNull();
    cJSON *item7 = cJSON_CreateNumber(3.14);
    cJSON *item8 = cJSON_CreateString("hello");
    cJSON *item9 = cJSON_CreateBool(0);
    cJSON *item10 = cJSON_CreateNull();
    cJSON *item11 = cJSON_CreateNumber(100);
    cJSON *item12 = cJSON_CreateString("world");
    cJSON *item13 = cJSON_CreateBool(1);

    // 1. cJSON_AddItemToObject
    if (item1) cJSON_AddItemToObject(root, "key1", item1);
    // 2. cJSON_AddItemToObject
    if (item2) cJSON_AddItemToObject(root, "key2", item2);
    // 3. cJSON_AddItemToObjectCS
    if (item3) cJSON_AddItemToObjectCS(root, "key3", item3);
    // 4. cJSON_AddItemToObjectCS
    if (item4) cJSON_AddItemToObjectCS(root, "key4", item4);
    // 5. cJSON_AddItemToObjectCS
    if (item5) cJSON_AddItemToObjectCS(root, "key5", item5);

    // Prepare array for reference tests
    cJSON *arr = cJSON_CreateArray();
    if (arr) {
        // 6. cJSON_AddItemReferenceToArray
        if (item6) cJSON_AddItemReferenceToArray(arr, item6);
        // 7. cJSON_AddItemReferenceToArray
        if (item7) cJSON_AddItemReferenceToArray(arr, item7);
        cJSON_AddItemToObject(root, "array_ref", arr);
    }

    // 8. cJSON_AddItemReferenceToObject
    if (item8) cJSON_AddItemReferenceToObject(root, "ref_key1", item8);
    // 9. cJSON_AddItemReferenceToObject
    if (item9) cJSON_AddItemReferenceToObject(root, "ref_key2", item9);
    // 10. cJSON_AddItemReferenceToObject
    if (item10) cJSON_AddItemReferenceToObject(root, "ref_key3", item10);

    // Add more items to array for deletion test
    if (arr) {
        cJSON *temp1 = cJSON_CreateNumber(99);
        cJSON *temp2 = cJSON_CreateNumber(88);
        if (temp1) cJSON_AddItemToArray(arr, temp1);
        if (temp2) cJSON_AddItemToArray(arr, temp2);
    }

    // 11. cJSON_DeleteItemFromArray
    if (arr) cJSON_DeleteItemFromArray(arr, 0);

    // 12. cJSON_DeleteItemFromObject
    cJSON_DeleteItemFromObject(root, "key1");
    // 13. cJSON_DeleteItemFromObject
    cJSON_DeleteItemFromObject(root, "key2");
    // 14. cJSON_DeleteItemFromObjectCaseSensitive
    cJSON_DeleteItemFromObjectCaseSensitive(root, "key3");
    // 15. cJSON_DeleteItemFromObjectCaseSensitive
    cJSON_DeleteItemFromObjectCaseSensitive(root, "ref_key1");

    // 16. cJSON_Minify
    // Generate string first
    char *json_str = cJSON_Print(root);
    if (json_str) {
        // Ensure we have a writable buffer (cJSON_Print returns malloc'd memory)
        cJSON_Minify(json_str);
        free(json_str);
    }

    // 17. cJSON_Delete
    // Note: We need to be careful here. item1, item2, etc. might have been added to root.
    // If we delete root, it deletes children.
    // However, some items were added by reference (item6, item7, item8, item9, item10).
    // Deleting root will delete the references, but not the original items if they are not owned.
    // But wait, cJSON_AddItemReferenceToObject adds a reference. The original item is NOT moved.
    // So if we delete root, the references are removed, but the original items (item6..item10) still exist.
    // We must delete the original items manually if they were not moved.
    // However, the task says to call cJSON_Delete 3 times at the end.
    // Let's assume the caller wants to clean up the root and potentially some loose items.
    
    // Clean up root
    cJSON_Delete(root);

    // Clean up items that were added by reference and thus not freed by root deletion
    // item6, item7, item8, item9, item10 were added by reference.
    // item1, item2, item3, item4, item5 were added by value (moved).
    // item11, item12, item13 were never added.
    
    // The prompt asks for 3 calls to cJSON_Delete.
    // Let's delete the remaining loose items.
    if (item6) cJSON_Delete(item6);
    if (item7) cJSON_Delete(item7);
    // item8, item9, item10 are also loose, but we only need 3 calls total for the sequence.
    // The sequence is: ... -> cJSON_Delete -> cJSON_Delete -> cJSON_Delete
    // We already did one on root. We need two more.
    if (item8) cJSON_Delete(item8);
    if (item9) cJSON_Delete(item9);
    
    // Clean up any remaining items that were not used or not freed
    if (item10) cJSON_Delete(item10);
    if (item11) cJSON_Delete(item11);
    if (item12) cJSON_Delete(item12);
    if (item13) cJSON_Delete(item13);

    return 0;
}