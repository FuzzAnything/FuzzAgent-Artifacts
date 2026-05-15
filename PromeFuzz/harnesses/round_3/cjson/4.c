// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateStringReference at cJSON.c:2548:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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
    // No-op cleanup function
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Ensure null-terminated string for parsing
    char *json_str = (char *)malloc(Size + 1);
    if (!json_str) return 0;
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';
    
    // 1. cJSON_Parse
    cJSON *parsed = cJSON_Parse(json_str);
    free(json_str);
    
    if (!parsed) {
        // If parsing fails, we can still test other functions with new objects
        parsed = cJSON_CreateObject();
        if (!parsed) return 0;
    }
    
    // 2. cJSON_CreateString
    cJSON *string1 = cJSON_CreateString("test_string1");
    if (!string1) {
        cJSON_Delete(parsed);
        return 0;
    }
    
    // 3. cJSON_CreateStringReference
    static const char ref_str[] = "reference_string";
    cJSON *string_ref = cJSON_CreateStringReference(ref_str);
    if (!string_ref) {
        cJSON_Delete(string1);
        cJSON_Delete(parsed);
        return 0;
    }
    
    // 4. cJSON_AddItemToObject
    cJSON_AddItemToObject(parsed, "key1", string1);
    
    // 5. cJSON_AddItemToObject
    cJSON_AddItemToObject(parsed, "key2", string_ref);
    
    // 6. cJSON_SetValuestring
    cJSON_SetValuestring(string1, "modified_string1");
    
    // 7. cJSON_GetObjectItem
    cJSON *item1 = cJSON_GetObjectItem(parsed, "key1");
    
    // 8. cJSON_GetObjectItem
    cJSON *item2 = cJSON_GetObjectItem(parsed, "key2");
    
    // 9. cJSON_SetValuestring
    if (item1 && item1->type == cJSON_String) {
        cJSON_SetValuestring(item1, "modified_string2");
    }
    
    // 10. cJSON_GetObjectItem
    cJSON *item3 = cJSON_GetObjectItem(parsed, "key1");
    
    // 11. cJSON_GetObjectItem
    cJSON *item4 = cJSON_GetObjectItem(parsed, "key2");
    
    // 12. cJSON_SetValuestring
    if (item3 && item3->type == cJSON_String) {
        cJSON_SetValuestring(item3, "modified_string3");
    }
    
    // 13. cJSON_GetObjectItem
    cJSON *item5 = cJSON_GetObjectItem(parsed, "key1");
    
    // 14. cJSON_GetObjectItem
    cJSON *item6 = cJSON_GetObjectItem(parsed, "key2");
    
    // Create additional objects to test more states
    cJSON *extra_obj = cJSON_CreateObject();
    if (extra_obj) {
        cJSON *extra_string = cJSON_CreateString("extra_test");
        if (extra_string) {
            cJSON_AddItemToObject(extra_obj, "extra_key", extra_string);
            cJSON_SetValuestring(extra_string, "extra_modified");
            cJSON_GetObjectItem(extra_obj, "extra_key");
            cJSON_Delete(extra_obj);
        } else {
            cJSON_Delete(extra_obj);
        }
    }
    
    // Test with NULL inputs
    cJSON_GetObjectItem(NULL, "dummy");
    cJSON_SetValuestring(NULL, "dummy");
    cJSON_AddItemToObject(NULL, "dummy", NULL);
    
    // 15. cJSON_Delete
    cJSON_Delete(parsed);
    
    dummy_cleanup();
    return 0;
}