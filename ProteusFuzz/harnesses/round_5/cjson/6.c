// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
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
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    cJSON *array = cJSON_CreateArray();
    if (!array) {
        cJSON_Delete(root);
        return 0;
    }

    cJSON *item1 = cJSON_CreateNumber(1.0);
    cJSON *item2 = cJSON_CreateNumber(2.0);
    cJSON *item3 = cJSON_CreateString("test");
    cJSON *item4 = cJSON_CreateBool(1);
    cJSON *item5 = cJSON_CreateNull();
    cJSON *item6 = cJSON_CreateNumber(6.0);
    cJSON *item7 = cJSON_CreateNumber(7.0);
    cJSON *item8 = cJSON_CreateNumber(8.0);
    cJSON *item9 = cJSON_CreateNumber(9.0);
    cJSON *item10 = cJSON_CreateNumber(10.0);

    if (!item1 || !item2 || !item3 || !item4 || !item5 || !item6 || !item7 || !item8 || !item9 || !item10) {
        cJSON_Delete(root);
        cJSON_Delete(array);
        cJSON_Delete(item1); cJSON_Delete(item2); cJSON_Delete(item3); cJSON_Delete(item4); cJSON_Delete(item5);
        cJSON_Delete(item6); cJSON_Delete(item7); cJSON_Delete(item8); cJSON_Delete(item9); cJSON_Delete(item10);
        return 0;
    }

    // 1. cJSON_AddItemToObject
    // Ownership of item1 is transferred to root
    cJSON_AddItemToObject(root, "key1", item1);
    
    // 2. cJSON_AddItemToObject
    // Ownership of item2 is transferred to root
    cJSON_AddItemToObject(root, "key2", item2);
    
    // 3. cJSON_AddItemToObjectCS
    // Ownership of item3 is transferred to root
    cJSON_AddItemToObjectCS(root, "key3_const", item3);
    
    // 4. cJSON_AddItemToObjectCS
    // Ownership of item4 is transferred to root
    cJSON_AddItemToObjectCS(root, "key4_const", item4);
    
    // 5. cJSON_AddItemToObjectCS
    // Ownership of item5 is transferred to root
    cJSON_AddItemToObjectCS(root, "key5_const", item5);

    // 6. cJSON_AddItemReferenceToArray
    // Adds a reference to item6 in array. Ownership remains with item6 variable.
    cJSON_AddItemReferenceToArray(array, item6);
    
    // 7. cJSON_AddItemReferenceToArray
    // Adds a reference to item7 in array. Ownership remains with item7 variable.
    cJSON_AddItemReferenceToArray(array, item7);

    // 8. cJSON_AddItemReferenceToObject
    // Adds a reference to item8 in root. Ownership remains with item8 variable.
    cJSON_AddItemReferenceToObject(root, "ref1", item8);
    
    // 9. cJSON_AddItemReferenceToObject
    // Adds a reference to item9 in root. Ownership remains with item9 variable.
    cJSON_AddItemReferenceToObject(root, "ref2", item9);
    
    // 10. cJSON_AddItemReferenceToObject
    // Adds a reference to item10 in root. Ownership remains with item10 variable.
    cJSON_AddItemReferenceToObject(root, "ref3", item10);

    // Add array to root. Ownership of array is transferred to root.
    cJSON_AddItemToObject(root, "my_array", array);

    // 11. cJSON_DeleteItemFromArray
    // Deletes item at index 0 of array (which is a reference to item6).
    // This frees the reference node, but NOT item6 itself.
    cJSON_DeleteItemFromArray(array, 0);

    // 12. cJSON_DeleteItemFromObject
    // Deletes "key1" from root. This frees item1 (which was owned by root).
    cJSON_DeleteItemFromObject(root, "key1");
    
    // 13. cJSON_DeleteItemFromObject
    // Deletes "key2" from root. This frees item2 (which was owned by root).
    // item2 is now freed. We must not use it again.
    cJSON_DeleteItemFromObject(root, "key2");
    
    // 14. cJSON_DeleteItemFromObjectCaseSensitive
    // Deletes "key3_const" from root. This frees item3.
    cJSON_DeleteItemFromObjectCaseSensitive(root, "key3_const");
    
    // 15. cJSON_DeleteItemFromObjectCaseSensitive
    // Deletes "ref1" from root. This removes the reference to item8.
    // It does NOT free item8 itself.
    cJSON_DeleteItemFromObjectCaseSensitive(root, "ref1");

    // 16. cJSON_Minify
    char *json_str = cJSON_Print(root);
    if (json_str) {
        cJSON_Minify(json_str);
        free(json_str);
    }

    // 17. cJSON_Delete
    // Delete root. This will:
    // - Free "key4_const" (item4) and "key5_const" (item5)
    // - Free "my_array". Inside array, it finds a reference to item7.
    //   cJSON_Delete on the array child (reference) will NOT free item7.
    // - Free "ref2" (reference to item9) and "ref3" (reference to item10).
    //   It will NOT free item9 or item10.
    cJSON_Delete(root);

    // 18. cJSON_Delete
    // item6 was referenced in array, but the reference was deleted in step 11.
    // item6 was never owned by root or array. It is still valid.
    cJSON_Delete(item6);

    // 19. cJSON_Delete
    // item8 was referenced in root, but the reference was deleted in step 15.
    // item8 was never owned by root. It is still valid.
    cJSON_Delete(item8);

    // Cleanup remaining items that were added by reference and not deleted yet
    // item7 is still valid (reference in array was removed when array was deleted? 
    // Wait, array was deleted in step 17. The child of array was a reference to item7.
    // cJSON_Delete on array iterates children. If child is reference, it skips deletion of the child.
    // So item7 is still valid.
    cJSON_Delete(item7);
    
    // item9 and item10 were references in root. Root deleted them (the reference nodes), not the items.
    cJSON_Delete(item9);
    cJSON_Delete(item10);

    // item1, item2, item3, item4, item5 were deleted via root or direct deletion.
    // Do not delete them again.

    return 0;
}