// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateStringReference at cJSON.c:2548:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static char* create_null_terminated_string(const uint8_t* data, size_t size, size_t max_len) {
    if (size == 0) {
        char* str = (char*)malloc(2);
        if (str) {
            str[0] = 'a';
            str[1] = '\0';
        }
        return str;
    }
    
    size_t len = size > max_len ? max_len : size;
    char* str = (char*)malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Create null-terminated strings from fuzzer input
    char* parse_str = create_null_terminated_string(Data, Size, 1024);
    if (!parse_str) return 0;
    
    // 1. cJSON_Parse
    cJSON* parsed = cJSON_Parse(parse_str);
    
    // Create more strings for other operations
    char* str1 = create_null_terminated_string(Data, Size, 256);
    char* str2 = create_null_terminated_string(Data + (Size > 256 ? 256 : 0), 
                                              Size > 256 ? Size - 256 : 0, 256);
    char* str3 = create_null_terminated_string(Data + (Size > 512 ? 512 : 0),
                                              Size > 512 ? Size - 512 : 0, 256);
    
    if (!str1 || !str2 || !str3) {
        free(parse_str);
        free(str1);
        free(str2);
        free(str3);
        cJSON_Delete(parsed);
        return 0;
    }
    
    // 2. cJSON_CreateString
    cJSON* string_item = cJSON_CreateString(str1);
    
    // 3. cJSON_CreateStringReference
    cJSON* string_ref = cJSON_CreateStringReference(str2);
    
    // Create an object if parsed is not an object or NULL
    cJSON* obj = parsed;
    if (!obj || obj->type != cJSON_Object) {
        obj = cJSON_CreateObject();
    }
    
    // 4. cJSON_AddItemToObject
    if (obj && string_item) {
        cJSON_AddItemToObject(obj, "key1", string_item);
    }
    
    // 5. cJSON_AddItemToObject
    if (obj && string_ref) {
        cJSON_AddItemToObject(obj, "key2", string_ref);
    }
    
    // 6. cJSON_SetValuestring
    if (string_item) {
        cJSON_SetValuestring(string_item, str3);
    }
    
    // 7. cJSON_GetObjectItem
    cJSON* item1 = NULL;
    if (obj) {
        item1 = cJSON_GetObjectItem(obj, "key1");
    }
    
    // 8. cJSON_GetObjectItem
    cJSON* item2 = NULL;
    if (obj) {
        item2 = cJSON_GetObjectItem(obj, "key2");
    }
    
    // 9. cJSON_SetValuestring
    if (item1) {
        cJSON_SetValuestring(item1, str1);
    }
    
    // 10. cJSON_GetObjectItem
    cJSON* item3 = NULL;
    if (obj) {
        item3 = cJSON_GetObjectItem(obj, "key1");
    }
    
    // 11. cJSON_GetObjectItem
    cJSON* item4 = NULL;
    if (obj) {
        item4 = cJSON_GetObjectItem(obj, "key2");
    }
    
    // 12. cJSON_SetValuestring
    if (item4) {
        cJSON_SetValuestring(item4, str2);
    }
    
    // 13. cJSON_GetObjectItem
    cJSON* item5 = NULL;
    if (obj) {
        item5 = cJSON_GetObjectItem(obj, "key1");
    }
    
    // 14. cJSON_GetObjectItem
    cJSON* item6 = NULL;
    if (obj) {
        item6 = cJSON_GetObjectItem(obj, "key2");
    }
    
    // 15. cJSON_Delete
    if (parsed != obj) {
        // If we created a new object, delete it separately
        cJSON_Delete(obj);
    }
    cJSON_Delete(parsed);
    
    // Cleanup
    free(parse_str);
    free(str1);
    free(str2);
    free(str3);
    
    return 0;
}