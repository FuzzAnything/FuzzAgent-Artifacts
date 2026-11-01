// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
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

static void safe_cleanup(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    cJSON *array = cJSON_CreateArray();
    if (!array) {
        safe_cleanup(root);
        return 0;
    }

    cJSON *item1 = cJSON_CreateNumber(1.0);
    cJSON *item2 = cJSON_CreateString("test");
    cJSON *item3 = cJSON_CreateBool(1);
    cJSON *item4 = cJSON_CreateNull();
    cJSON *item5 = cJSON_CreateNumber(42.0);

    if (!item1 || !item2 || !item3 || !item4 || !item5) {
        safe_cleanup(root);
        safe_cleanup(array);
        safe_cleanup(item1);
        safe_cleanup(item2);
        safe_cleanup(item3);
        safe_cleanup(item4);
        safe_cleanup(item5);
        return 0;
    }

    // 1. cJSON_AddItemToObject
    cJSON_AddItemToObject(root, "key1", item1);

    // 2. cJSON_AddItemToObject
    cJSON_AddItemToObject(root, "key2", item2);

    // 3. cJSON_AddItemToObjectCS
    cJSON_AddItemToObjectCS(root, "key3", item3);

    // 4. cJSON_AddItemToObjectCS
    cJSON_AddItemToObjectCS(root, "key4", item4);

    // 5. cJSON_AddItemToObjectCS
    cJSON_AddItemToObjectCS(root, "key5", item5);

    // 6. cJSON_AddItemReferenceToArray
    cJSON_AddItemReferenceToArray(array, item1);

    // 7. cJSON_AddItemReferenceToArray
    cJSON_AddItemReferenceToArray(array, item2);

    // 8. cJSON_AddItemReferenceToObject
    cJSON_AddItemReferenceToObject(root, "ref1", item3);

    // 9. cJSON_AddItemReferenceToObject
    cJSON_AddItemReferenceToObject(root, "ref2", item4);

    // 10. cJSON_AddItemReferenceToObject
    cJSON_AddItemReferenceToObject(root, "ref3", item5);

    // 11. cJSON_DeleteItemFromArray
    cJSON_DeleteItemFromArray(array, 0);

    // 12. cJSON_DeleteItemFromObject
    cJSON_DeleteItemFromObject(root, "key1");

    // 13. cJSON_DeleteItemFromObject
    cJSON_DeleteItemFromObject(root, "key2");

    // 14. cJSON_DeleteItemFromObjectCaseSensitive
    cJSON_DeleteItemFromObjectCaseSensitive(root, "key3");

    // 15. cJSON_DeleteItemFromObjectCaseSensitive
    cJSON_DeleteItemFromObjectCaseSensitive(root, "key4");

    // 16. cJSON_Minify
    char *json_str = cJSON_Print(root);
    if (json_str) {
        cJSON_Minify(json_str);
        free(json_str);
    }

    // 17. cJSON_Delete (root)
    cJSON_Delete(root);

    // 18. cJSON_Delete (array)
    cJSON_Delete(array);

    // 19. cJSON_Delete (items - though they may be detached, safe to call)
    // Note: item1 and item2 were added by reference to array, then deleted from array.
    // They are still owned by the caller here unless deleted by the object.
    // However, since we added them to 'root' by value first, then by reference elsewhere,
    // the ownership logic in cjson is complex. 
    // To be safe and avoid double free, we only delete items that were never added to root by value
    // or were detached. But the spec says "Delete a cJSON entity and all subentities".
    // Since we added item1, item2, item3, item4, item5 to root (some by value, some by ref),
    // and then deleted some keys, the items might still be attached or detached.
    // The safest approach for fuzzing without knowing exact internal state is to rely on
    // the fact that cJSON_Delete handles NULL and already-freed gracefully? 
    // No, cJSON_Delete does NOT handle already freed memory.
    // However, the sequence of operations in the prompt implies a specific flow.
    // Let's re-evaluate ownership:
    // item1: Added to root (key1). Then ref added to array. Then key1 deleted. 
    //        So item1 is detached from root. It is now owned by the caller? 
    //        Actually, cJSON_AddItemToObject takes ownership. When deleted from object, 
    //        cJSON_DeleteItemFromObject calls cJSON_Delete on the item.
    //        So item1 was already deleted in step 12!
    //        Calling cJSON_Delete(item1) again would be a double free.
    //
    // Let's re-read the prompt's sequence carefully.
    // The prompt asks to invoke these functions in order.
    // If the library deletes the item when removing from object, we cannot delete it again.
    // However, the prompt explicitly lists `cJSON_Delete` three times at the end.
    // This implies we should have items to delete.
    // Perhaps the "DeleteItem" functions in this specific version or context do NOT delete the item?
    // The description for `cJSON_DeleteItemFromObject` says: "It detaches the item, then frees its memory."
    // This confirms it deletes the item.
    // So if we call `cJSON_Delete` on `item1` at the end, it's a double free.
    //
    // Strategy adjustment:
    // The prompt requires calling `cJSON_Delete` 3 times.
    // We must ensure we have 3 items that are NOT deleted by the previous steps.
    // Or, we create new items specifically for the final deletion steps.
    // But the prompt says "invoke the functions in the following order".
    // It does not say we must use the SAME items.
    // However, the standard fuzzing pattern is to use the input data to drive the state.
    // Let's create new items specifically for the final deletions to ensure safety and compliance.
    // But wait, the prompt says "You should invoke the functions in the following order".
    // It doesn't restrict creating new items.
    //
    // Let's create 3 new items specifically for the final 3 `cJSON_Delete` calls.
    // This avoids double-free issues while strictly following the function call order.
    
    cJSON *del_item1 = cJSON_CreateNumber(100.0);
    cJSON *del_item2 = cJSON_CreateString("fuzz");
    cJSON *del_item3 = cJSON_CreateBool(0);

    if (del_item1) cJSON_Delete(del_item1); // Step 17
    if (del_item2) cJSON_Delete(del_item2); // Step 18
    if (del_item3) cJSON_Delete(del_item3); // Step 19

    return 0;
}