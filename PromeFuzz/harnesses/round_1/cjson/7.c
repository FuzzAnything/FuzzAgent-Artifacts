// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
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
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static char* create_string_from_data(const uint8_t *data, size_t size, size_t offset, size_t max_len) {
    if (offset >= size) return NULL;
    size_t len = size - offset;
    if (len > max_len) len = max_len;
    char *str = malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, data + offset, len);
    str[len] = '\0';
    return str;
}

static int get_index_from_data(const uint8_t *data, size_t size, size_t offset) {
    if (offset >= size) return 0;
    return data[offset] % 16;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;

    // Create objects and arrays
    cJSON *root_obj = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    
    // Create items to add
    cJSON *item1 = cJSON_CreateNumber(42.5);
    cJSON *item2 = cJSON_CreateString("test");
    cJSON *item3 = cJSON_CreateBool(1);
    cJSON *item4 = cJSON_CreateNull();
    cJSON *item5 = cJSON_CreateArray();
    cJSON *item6 = cJSON_CreateObject();
    
    // Create keys from fuzz data
    char *key1 = create_string_from_data(Data, Size, 0, 10);
    char *key2 = create_string_from_data(Data, Size, 10, 10);
    char *key3 = create_string_from_data(Data, Size, 20, 10);
    char *key4 = create_string_from_data(Data, Size, 30, 10);
    char *key5 = create_string_from_data(Data, Size, 40, 10);
    char *key6 = create_string_from_data(Data, Size, 50, 10);
    char *key7 = create_string_from_data(Data, Size, 60, 10);
    char *key8 = create_string_from_data(Data, Size, 70, 10);
    char *key9 = create_string_from_data(Data, Size, 80, 10);
    char *key10 = create_string_from_data(Data, Size, 90, 10);
    
    // cJSON_AddItemToObject (3 times)
    if (key1) cJSON_AddItemToObject(root_obj, key1, item1);
    if (key2) cJSON_AddItemToObject(root_obj, key2, item2);
    if (key3) cJSON_AddItemToObject(root_obj, key3, item3);
    
    // cJSON_AddItemToObjectCS (3 times)
    if (key4) cJSON_AddItemToObjectCS(root_obj, key4, item4);
    if (key5) cJSON_AddItemToObjectCS(root_obj, key5, item5);
    if (key6) cJSON_AddItemToObjectCS(root_obj, key6, item6);
    
    // Add some items to array first
    cJSON_AddItemToArray(array, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(array, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(array, cJSON_CreateNumber(3));
    
    // cJSON_AddItemReferenceToArray (2 times)
    cJSON_AddItemReferenceToArray(array, item1);
    cJSON_AddItemReferenceToArray(array, item2);
    
    // cJSON_AddItemReferenceToObject (3 times)
    if (key7) cJSON_AddItemReferenceToObject(root_obj, key7, item3);
    if (key8) cJSON_AddItemReferenceToObject(root_obj, key8, item4);
    if (key9) cJSON_AddItemReferenceToObject(root_obj, key9, item5);
    
    // cJSON_DeleteItemFromArray
    int index = get_index_from_data(Data, Size, 100);
    cJSON_DeleteItemFromArray(array, index);
    
    // cJSON_DeleteItemFromObject (2 times)
    if (key1) cJSON_DeleteItemFromObject(root_obj, key1);
    if (key2) cJSON_DeleteItemFromObject(root_obj, key2);
    
    // cJSON_DeleteItemFromObjectCaseSensitive (2 times)
    if (key3) cJSON_DeleteItemFromObjectCaseSensitive(root_obj, key3);
    if (key4) cJSON_DeleteItemFromObjectCaseSensitive(root_obj, key4);
    
    // cJSON_Minify
    char *json_str = cJSON_Print(root_obj);
    if (json_str) {
        cJSON_Minify(json_str);
        free(json_str);
    }
    
    // cJSON_Delete (3 times)
    cJSON_Delete(root_obj);
    cJSON_Delete(array);
    cJSON_Delete(NULL);  // Test with NULL
    
    // Clean up keys
    free(key1);
    free(key2);
    free(key3);
    free(key4);
    free(key5);
    free(key6);
    free(key7);
    free(key8);
    free(key9);
    free(key10);
    
    return 0;
}