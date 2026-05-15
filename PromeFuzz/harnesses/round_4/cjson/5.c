// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_cleanup(void) {
    // Cleanup function for any temporary resources
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // Early exit if input is too small to be useful
    if (Size < 1) return 0;

    // Initialize dummy file if needed (not required for these functions)
    // FILE *fp = fopen("./dummy_file", "wb");
    // if (fp) {
    //     fwrite(Data, 1, Size, fp);
    //     fclose(fp);
    // }

    // Create a root object and array for testing
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    if (!root || !array) {
        if (root) cJSON_Delete(root);
        if (array) cJSON_Delete(array);
        return 0;
    }

    // Create some items to add
    cJSON *item1 = cJSON_CreateNumber(42.0);
    cJSON *item2 = cJSON_CreateString("test");
    cJSON *item3 = cJSON_CreateBool(1);
    cJSON *item4 = cJSON_CreateNull();
    cJSON *item5 = cJSON_CreateObject();
    cJSON *item6 = cJSON_CreateArray();

    // Use fuzzer data to generate keys and indices
    // We'll use the first part of Data for string keys, and bytes for indices
    size_t key_len = Size > 100 ? 100 : Size; // Use up to 100 bytes for keys
    char *key1 = (char*)malloc(key_len + 1);
    char *key2 = (char*)malloc(key_len + 1);
    char *key3 = (char*)malloc(key_len + 1);
    char *key4 = (char*)malloc(key_len + 1);
    char *key5 = (char*)malloc(key_len + 1);
    if (!key1 || !key2 || !key3 || !key4 || !key5) {
        free(key1); free(key2); free(key3); free(key4); free(key5);
        cJSON_Delete(item1); cJSON_Delete(item2); cJSON_Delete(item3);
        cJSON_Delete(item4); cJSON_Delete(item5); cJSON_Delete(item6);
        cJSON_Delete(root); cJSON_Delete(array);
        return 0;
    }
    memcpy(key1, Data, key_len); key1[key_len] = '\0';
    memcpy(key2, Data, key_len); key2[key_len] = '\0';
    memcpy(key3, Data, key_len); key3[key_len] = '\0';
    memcpy(key4, Data, key_len); key4[key_len] = '\0';
    memcpy(key5, Data, key_len); key5[key_len] = '\0';

    // Determine an index for array operations (use first byte modulo a small number)
    int index = (Data[0] % 10);

    // 1-3: cJSON_AddItemToObject (3 times)
    cJSON_AddItemToObject(root, key1, cJSON_Duplicate(item1, 1));
    cJSON_AddItemToObject(root, key2, cJSON_Duplicate(item2, 1));
    cJSON_AddItemToObject(root, key3, cJSON_Duplicate(item3, 1));

    // 4-6: cJSON_AddItemToObjectCS (3 times)
    cJSON_AddItemToObjectCS(root, key4, cJSON_Duplicate(item4, 1));
    cJSON_AddItemToObjectCS(root, key5, cJSON_Duplicate(item5, 1));
    cJSON_AddItemToObjectCS(root, "literal_key", cJSON_Duplicate(item6, 1));

    // 7-8: cJSON_AddItemReferenceToArray (2 times)
    cJSON_AddItemReferenceToArray(array, item1);
    cJSON_AddItemReferenceToArray(array, item2);

    // 9-11: cJSON_AddItemReferenceToObject (3 times)
    cJSON_AddItemReferenceToObject(root, "ref_key1", item3);
    cJSON_AddItemReferenceToObject(root, "ref_key2", item4);
    cJSON_AddItemReferenceToObject(root, "ref_key3", item5);

    // 12: cJSON_DeleteItemFromArray
    if (cJSON_GetArraySize(array) > 0) {
        cJSON_DeleteItemFromArray(array, index % cJSON_GetArraySize(array));
    }

    // 13-14: cJSON_DeleteItemFromObject (2 times)
    cJSON_DeleteItemFromObject(root, key1);
    cJSON_DeleteItemFromObject(root, key2);

    // 15-16: cJSON_DeleteItemFromObjectCaseSensitive (2 times)
    cJSON_DeleteItemFromObjectCaseSensitive(root, key3);
    cJSON_DeleteItemFromObjectCaseSensitive(root, key4);

    // 17: cJSON_Minify
    // Create a mutable string from part of the input
    size_t json_len = Size > 200 ? 200 : Size;
    char *json_str = (char*)malloc(json_len + 1);
    if (json_str) {
        memcpy(json_str, Data, json_len);
        json_str[json_len] = '\0';
        cJSON_Minify(json_str);
        free(json_str);
    }

    // 18-20: cJSON_Delete (3 times)
    cJSON_Delete(root);
    cJSON_Delete(array);
    cJSON_Delete(cJSON_CreateNull()); // Delete a dummy item

    // Cleanup
    free(key1); free(key2); free(key3); free(key4); free(key5);
    // Note: item1, item2, item3, item4, item5, item6 are either owned by root/array or already deleted
    // They should not be deleted separately to avoid double-free

    dummy_cleanup();
    return 0;
}