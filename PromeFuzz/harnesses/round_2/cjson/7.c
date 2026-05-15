// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
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
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateBool at cJSON.c:2494:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemToObjectCS at cJSON.c:2125:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToArray at cJSON.c:2130:26 in cJSON.h
// cJSON_AddItemReferenceToObject at cJSON.c:2140:26 in cJSON.h
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
#include <stdio.h>

static char* extract_string(const uint8_t* data, size_t size, size_t* offset, size_t max_len) {
    if (*offset >= size) return NULL;
    size_t len = (size - *offset) < max_len ? (size - *offset) : max_len;
    if (len == 0) return NULL;
    char* str = (char*)malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, data + *offset, len);
    str[len] = '\0';
    *offset += len;
    return str;
}

static int extract_int(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset + sizeof(int) > size) return 0;
    int val;
    memcpy(&val, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return val;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    size_t offset = 0;
    
    // Create objects and arrays
    cJSON* root_obj = cJSON_CreateObject();
    cJSON* array_obj = cJSON_CreateArray();
    cJSON* temp_obj = cJSON_CreateObject();
    
    if (!root_obj || !array_obj || !temp_obj) {
        if (root_obj) cJSON_Delete(root_obj);
        if (array_obj) cJSON_Delete(array_obj);
        if (temp_obj) cJSON_Delete(temp_obj);
        return 0;
    }
    
    // Create items to add
    cJSON* item1 = cJSON_CreateString("test1");
    cJSON* item2 = cJSON_CreateNumber(42);
    cJSON* item3 = cJSON_CreateBool(1);
    cJSON* item4 = cJSON_CreateNull();
    cJSON* item5 = cJSON_CreateString("test5");
    cJSON* item6 = cJSON_CreateNumber(3.14);
    cJSON* item7 = cJSON_CreateBool(0);
    cJSON* item8 = cJSON_CreateArray();
    cJSON* item9 = cJSON_CreateObject();
    cJSON* item10 = cJSON_CreateString("test10");
    
    // Extract strings for keys
    char* key1 = extract_string(Data, Size, &offset, 16);
    char* key2 = extract_string(Data, Size, &offset, 16);
    char* key3 = extract_string(Data, Size, &offset, 16);
    char* key4 = extract_string(Data, Size, &offset, 16);
    char* key5 = extract_string(Data, Size, &offset, 16);
    char* key6 = extract_string(Data, Size, &offset, 16);
    char* key7 = extract_string(Data, Size, &offset, 16);
    char* key8 = extract_string(Data, Size, &offset, 16);
    
    // 1-3: cJSON_AddItemToObject
    if (key1) cJSON_AddItemToObject(root_obj, key1, cJSON_Duplicate(item1, 1));
    if (key2) cJSON_AddItemToObject(root_obj, key2, cJSON_Duplicate(item2, 1));
    if (key3) cJSON_AddItemToObject(root_obj, key3, cJSON_Duplicate(item3, 1));
    
    // 4-6: cJSON_AddItemToObjectCS
    cJSON_AddItemToObjectCS(root_obj, "const_key1", cJSON_Duplicate(item4, 1));
    cJSON_AddItemToObjectCS(root_obj, "const_key2", cJSON_Duplicate(item5, 1));
    cJSON_AddItemToObjectCS(root_obj, "const_key3", cJSON_Duplicate(item6, 1));
    
    // Add items to array for reference operations
    cJSON_AddItemToArray(array_obj, cJSON_Duplicate(item7, 1));
    cJSON_AddItemToArray(array_obj, cJSON_Duplicate(item8, 1));
    cJSON_AddItemToArray(array_obj, cJSON_Duplicate(item9, 1));
    
    // 7-8: cJSON_AddItemReferenceToArray
    cJSON_AddItemReferenceToArray(array_obj, item10);
    cJSON_AddItemReferenceToArray(array_obj, temp_obj);
    
    // 9-11: cJSON_AddItemReferenceToObject
    if (key4) cJSON_AddItemReferenceToObject(root_obj, key4, item1);
    if (key5) cJSON_AddItemReferenceToObject(root_obj, key5, item2);
    if (key6) cJSON_AddItemReferenceToObject(root_obj, key6, item3);
    
    // Extract index for array deletion
    int array_index = extract_int(Data, Size, &offset) % 3;
    if (array_index < 0) array_index = 0;
    
    // 12: cJSON_DeleteItemFromArray
    cJSON_DeleteItemFromArray(array_obj, array_index);
    
    // 13-14: cJSON_DeleteItemFromObject
    if (key1) cJSON_DeleteItemFromObject(root_obj, key1);
    if (key2) cJSON_DeleteItemFromObject(root_obj, key2);
    
    // 15-16: cJSON_DeleteItemFromObjectCaseSensitive
    cJSON_DeleteItemFromObjectCaseSensitive(root_obj, "const_key1");
    cJSON_DeleteItemFromObjectCaseSensitive(root_obj, "const_key2");
    
    // 17: cJSON_Minify
    char* json_str = cJSON_Print(root_obj);
    if (json_str) {
        cJSON_Minify(json_str);
        free(json_str);
    }
    
    // 18-20: cJSON_Delete
    cJSON_Delete(root_obj);
    cJSON_Delete(array_obj);
    cJSON_Delete(temp_obj);
    
    // Clean up extracted strings
    if (key1) free(key1);
    if (key2) free(key2);
    if (key3) free(key3);
    if (key4) free(key4);
    if (key5) free(key5);
    if (key6) free(key6);
    if (key7) free(key7);
    if (key8) free(key8);
    
    // Clean up created items (except those added by reference)
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
    
    return 0;
}