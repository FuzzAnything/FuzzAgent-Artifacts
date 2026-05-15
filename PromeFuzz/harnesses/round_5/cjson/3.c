// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateStringReference at cJSON.c:2548:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
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
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Create null-terminated string from fuzzer input for parsing
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) return 0;
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // 1. Parse JSON input
    cJSON *parsed_json = cJSON_Parse(input_str);
    
    // Create static strings for string reference
    static const char static_str1[] = "static_string_1";
    static const char static_str2[] = "static_string_2";
    
    // 2. Create regular string
    cJSON *string_item = cJSON_CreateString("test_string");
    
    // 3. Create string reference
    cJSON *string_ref = cJSON_CreateStringReference(static_str1);
    
    // Create an object if parsed_json is not an object or is NULL
    cJSON *root_object = parsed_json;
    if (!root_object || root_object->type != cJSON_Object) {
        if (root_object) cJSON_Delete(root_object);
        root_object = cJSON_CreateObject();
    }
    
    // 4. Add string item to object
    if (string_item && root_object) {
        cJSON_AddItemToObject(root_object, "key1", string_item);
    }
    
    // 5. Add string reference to object
    if (string_ref && root_object) {
        cJSON_AddItemToObject(root_object, "key2", string_ref);
    }
    
    // 6. Set value string on the first string item
    if (string_item) {
        cJSON_SetValuestring(string_item, "new_value_1");
    }
    
    // 7. Get object item (key1)
    cJSON *item1 = NULL;
    if (root_object) {
        item1 = cJSON_GetObjectItem(root_object, "key1");
    }
    
    // 8. Get object item (key2)
    cJSON *item2 = NULL;
    if (root_object) {
        item2 = cJSON_GetObjectItem(root_object, "key2");
    }
    
    // 9. Set value string on item1 if it exists and is a string
    if (item1 && item1->type == cJSON_String) {
        cJSON_SetValuestring(item1, "new_value_2");
    }
    
    // 10. Get object item (key1) again
    if (root_object) {
        cJSON_GetObjectItem(root_object, "key1");
    }
    
    // 11. Get object item (key2) again
    if (root_object) {
        cJSON_GetObjectItem(root_object, "key2");
    }
    
    // 12. Set value string on item2 if it exists and is a string
    if (item2 && item2->type == cJSON_String) {
        cJSON_SetValuestring(item2, "new_value_3");
    }
    
    // 13. Get object item (key1) third time
    if (root_object) {
        cJSON_GetObjectItem(root_object, "key1");
    }
    
    // 14. Get object item (key2) third time
    if (root_object) {
        cJSON_GetObjectItem(root_object, "key2");
    }
    
    // 15. Clean up - delete the root object
    if (root_object) {
        cJSON_Delete(root_object);
    } else if (parsed_json) {
        cJSON_Delete(parsed_json);
    }
    
    free(input_str);
    return 0;
}