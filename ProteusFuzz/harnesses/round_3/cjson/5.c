// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
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
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
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
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static cJSON *create_test_object(const uint8_t *data, size_t size) {
    cJSON *obj = cJSON_CreateObject();
    if (!obj) return NULL;

    if (size > 0) {
        char *str = (char *)malloc(size + 1);
        if (str) {
            memcpy(str, data, size);
            str[size] = '\0';
            cJSON_AddItemToObject(obj, "input_data", cJSON_CreateString(str));
            free(str);
        }
    }
    return obj;
}

static cJSON *create_test_array(const uint8_t *data, size_t size) {
    cJSON *arr = cJSON_CreateArray();
    if (!arr) return NULL;

    for (size_t i = 0; i < size && i < 100; i++) {
        cJSON *num = cJSON_CreateNumber((double)(data[i] % 256));
        if (num) {
            cJSON_AddItemToArray(arr, num);
        }
    }
    return arr;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cJSON *root_obj = create_test_object(Data, Size);
    cJSON *root_arr = create_test_array(Data, Size);
    
    // Create items that will be added to the structure
    // item1, item2, item3, item4, item5 are owned by the object keys
    cJSON *item1 = cJSON_CreateNumber(1.0);
    cJSON *item2 = cJSON_CreateNumber(2.0);
    cJSON *item3 = cJSON_CreateString("test");
    cJSON *item4 = cJSON_CreateNumber(4.0);
    cJSON *item5 = cJSON_CreateNumber(5.0);
    // item6 is a standalone item, not added to the tree initially
    cJSON *item6 = cJSON_CreateNumber(6.0);

    if (!root_obj || !root_arr || !item1 || !item2 || !item3 || !item4 || !item5 || !item6) {
        cJSON_Delete(root_obj);
        cJSON_Delete(root_arr);
        cJSON_Delete(item1);
        cJSON_Delete(item2);
        cJSON_Delete(item3);
        cJSON_Delete(item4);
        cJSON_Delete(item5);
        cJSON_Delete(item6);
        return 0;
    }

    // 1. cJSON_AddItemToObject
    cJSON_AddItemToObject(root_obj, "key1", item1);
    // 2. cJSON_AddItemToObject
    cJSON_AddItemToObject(root_obj, "key2", item2);
    // 3. cJSON_AddItemToObjectCS
    cJSON_AddItemToObjectCS(root_obj, "key3_const", item3);
    // 4. cJSON_AddItemToObjectCS
    cJSON_AddItemToObjectCS(root_obj, "key4_const", item4);
    // 5. cJSON_AddItemToObjectCS
    cJSON_AddItemToObjectCS(root_obj, "key5_const", item5);

    // 6. cJSON_AddItemReferenceToArray
    cJSON_AddItemReferenceToArray(root_arr, item1);
    // 7. cJSON_AddItemReferenceToArray
    cJSON_AddItemReferenceToArray(root_arr, item2);

    // FIX: Create separate items for references to avoid double-free/use-after-free.
    // We cannot reference item1, item2, or item3 because they will be deleted later.
    // We use item6 and create new items for the other references.
    cJSON *ref_item1 = cJSON_CreateNumber(10.0);
    cJSON *ref_item2 = cJSON_CreateNumber(20.0);
    cJSON *ref_item3 = cJSON_CreateString("ref_test");

    if (!ref_item1 || !ref_item2 || !ref_item3) {
        cJSON_Delete(root_obj);
        cJSON_Delete(root_arr);
        cJSON_Delete(item1); cJSON_Delete(item2); cJSON_Delete(item3);
        cJSON_Delete(item4); cJSON_Delete(item5); cJSON_Delete(item6);
        cJSON_Delete(ref_item1); cJSON_Delete(ref_item2); cJSON_Delete(ref_item3);
        return 0;
    }

    // 8. cJSON_AddItemReferenceToObject (Using safe reference items)
    cJSON_AddItemReferenceToObject(root_obj, "ref_key1", ref_item1);
    // 9. cJSON_AddItemReferenceToObject
    cJSON_AddItemReferenceToObject(root_obj, "ref_key2", ref_item2);
    // 10. cJSON_AddItemReferenceToObject
    cJSON_AddItemReferenceToObject(root_obj, "ref_key3", ref_item3);

    // 11. cJSON_DeleteItemFromArray
    cJSON_DeleteItemFromArray(root_arr, 0);
    
    // 12. cJSON_DeleteItemFromObject
    // Deletes "key1" (item1). Safe because ref_key1 points to ref_item1.
    cJSON_DeleteItemFromObject(root_obj, "key1");
    // 13. cJSON_DeleteItemFromObject
    // Deletes "key2" (item2). Safe because ref_key2 points to ref_item2.
    cJSON_DeleteItemFromObject(root_obj, "key2");
    
    // 14. cJSON_DeleteItemFromObjectCaseSensitive
    // Deletes "key3_const" (item3). Safe because ref_key3 points to ref_item3.
    cJSON_DeleteItemFromObjectCaseSensitive(root_obj, "key3_const");
    // 15. cJSON_DeleteItemFromObjectCaseSensitive
    // Deletes "key4_const" (item4). Safe.
    cJSON_DeleteItemFromObjectCaseSensitive(root_obj, "key4_const");

    // 16. cJSON_Minify
    char *json_str = cJSON_Print(root_obj);
    if (json_str) {
        cJSON_Minify(json_str);
        free(json_str);
    }

    // 17. cJSON_Delete
    // Delete the root object. This will recursively delete children.
    // item5 is still in the tree (key5_const) and will be freed here.
    // ref_item1, ref_item2, ref_item3 are references, so they are NOT freed here.
    cJSON_Delete(root_obj);
    
    // 18. cJSON_Delete
    cJSON_Delete(root_arr);
    
    // Clean up standalone items and reference items that were not owned by the tree
    cJSON_Delete(item6);
    cJSON_Delete(ref_item1);
    cJSON_Delete(ref_item2);
    cJSON_Delete(ref_item3);

    return 0;
}