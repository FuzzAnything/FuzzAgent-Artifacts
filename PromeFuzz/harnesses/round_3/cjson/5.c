// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_DeleteItemFromArray at cJSON.c:2304:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObject at cJSON.c:2323:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_DeleteItemFromObjectCaseSensitive at cJSON.c:2328:20 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
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
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    create_dummy_file(Data, Size);
    
    // Create objects and items for testing
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *item1 = cJSON_CreateString("test1");
    cJSON *item2 = cJSON_CreateNumber(42.0);
    cJSON *item3 = cJSON_CreateBool(1);
    cJSON *item4 = cJSON_CreateString("test4");
    cJSON *item5 = cJSON_CreateNumber(3.14);
    cJSON *item6 = cJSON_CreateBool(0);
    cJSON *item7 = cJSON_CreateString("test7");
    cJSON *item8 = cJSON_CreateNumber(100.0);
    cJSON *item9 = cJSON_CreateString("test9");
    cJSON *item10 = cJSON_CreateNumber(200.0);
    
    // Create reference items
    cJSON *ref1 = cJSON_CreateString("ref1");
    cJSON *ref2 = cJSON_CreateNumber(99.0);
    cJSON *ref3 = cJSON_CreateBool(1);
    cJSON *ref4 = cJSON_CreateString("ref4");
    cJSON *ref5 = cJSON_CreateNumber(77.0);
    
    // Create strings from fuzzer data
    char *key1 = malloc(20);
    char *key2 = malloc(20);
    char *key3 = malloc(20);
    char *key4 = malloc(20);
    char *key5 = malloc(20);
    char *key6 = malloc(20);
    char *key7 = malloc(20);
    char *key8 = malloc(20);
    
    if (key1 && key2 && key3 && key4 && key5 && key6 && key7 && key8) {
        size_t copy_size = Size > 19 ? 19 : Size;
        memcpy(key1, Data, copy_size);
        key1[copy_size] = '\0';
        
        memcpy(key2, Data + (copy_size % Size), copy_size);
        key2[copy_size] = '\0';
        
        memcpy(key3, Data + (2 * copy_size % Size), copy_size);
        key3[copy_size] = '\0';
        
        memcpy(key4, Data + (3 * copy_size % Size), copy_size);
        key4[copy_size] = '\0';
        
        memcpy(key5, Data + (4 * copy_size % Size), copy_size);
        key5[copy_size] = '\0';
        
        memcpy(key6, Data + (5 * copy_size % Size), copy_size);
        key6[copy_size] = '\0';
        
        memcpy(key7, Data + (6 * copy_size % Size), copy_size);
        key7[copy_size] = '\0';
        
        memcpy(key8, Data + (7 * copy_size % Size), copy_size);
        key8[copy_size] = '\0';
        
        // Add some items to array first
        cJSON_AddItemToArray(array, cJSON_CreateString("array_item1"));
        cJSON_AddItemToArray(array, cJSON_CreateNumber(1.0));
        cJSON_AddItemToArray(array, cJSON_CreateBool(1));
        
        // Function calls in specified order
        cJSON_AddItemToObject(root, key1, item1);
        cJSON_AddItemToObject(root, key2, item2);
        cJSON_AddItemToObjectCS(root, "const_key1", item3);
        cJSON_AddItemToObjectCS(root, "const_key2", item4);
        cJSON_AddItemToObjectCS(root, "const_key3", item5);
        cJSON_AddItemReferenceToArray(array, ref1);
        cJSON_AddItemReferenceToArray(array, ref2);
        cJSON_AddItemReferenceToObject(root, key3, ref3);
        cJSON_AddItemReferenceToObject(root, key4, ref4);
        cJSON_AddItemReferenceToObject(root, key5, ref5);
        cJSON_DeleteItemFromArray(array, 0);
        cJSON_DeleteItemFromObject(root, key1);
        cJSON_DeleteItemFromObject(root, key2);
        cJSON_DeleteItemFromObjectCaseSensitive(root, "const_key1");
        cJSON_DeleteItemFromObjectCaseSensitive(root, "const_key2");
        
        // Create a mutable string for minify
        char *json_str = malloc(Size + 1);
        if (json_str) {
            memcpy(json_str, Data, Size);
            json_str[Size] = '\0';
            cJSON_Minify(json_str);
            free(json_str);
        }
        
        cJSON_Delete(root);
        cJSON_Delete(array);
        cJSON_Delete(item6);  // item6 wasn't added to anything
        cJSON_Delete(item7);  // item7 wasn't added to anything
        cJSON_Delete(item8);  // item8 wasn't added to anything
        cJSON_Delete(item9);  // item9 wasn't added to anything
        cJSON_Delete(item10); // item10 wasn't added to anything
        
        // Note: ref1, ref2, ref3, ref4, ref5 were added as references
        // and will be deleted when their parents are deleted
        
        free(key1);
        free(key2);
        free(key3);
        free(key4);
        free(key5);
        free(key6);
        free(key7);
        free(key8);
    } else {
        // Cleanup if allocation failed
        cJSON_Delete(root);
        cJSON_Delete(array);
        cJSON_Delete(item1);
        cJSON_Delete(item2);
        cJSON_Delete(item3);
        cJSON_Delete(item4);
        cJSON_Delete(item5);
        cJSON_Delete(item6);
        cJSON_Delete(item7);
        cJSON_Delete(item8);
        cJSON_Delete(item9);
        cJSON_Delete(item10);
        cJSON_Delete(ref1);
        cJSON_Delete(ref2);
        cJSON_Delete(ref3);
        cJSON_Delete(ref4);
        cJSON_Delete(ref5);
        
        free(key1);
        free(key2);
        free(key3);
        free(key4);
        free(key5);
        free(key6);
        free(key7);
        free(key8);
    }
    
    cleanup_dummy_file();
    return 0;
}