// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateStringReference at cJSON.c:2548:23 in cJSON.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Step 1: Prepare null-terminated string for cJSON_Parse
    char *json_str = (char *)malloc(Size + 1);
    if (json_str == NULL) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';
    
    // Step 2: Parse JSON
    cJSON *parsed = cJSON_Parse(json_str);
    
    // Step 3: Create string items
    cJSON *string_item = cJSON_CreateString("test_string");
    cJSON *string_ref_item = cJSON_CreateStringReference("reference_string");
    
    // Step 4: Create an object to add items to
    cJSON *object = cJSON_CreateObject();
    
    // Step 5: Add items to object (first cJSON_AddItemToObject)
    if (object && string_item) {
        cJSON_AddItemToObject(object, "key1", string_item);
    }
    
    // Step 6: Add reference string to object (second cJSON_AddItemToObject)
    if (object && string_ref_item) {
        cJSON_AddItemToObject(object, "key2", string_ref_item);
    }
    
    // Step 7: Set valuestring on the string item
    if (string_item) {
        cJSON_SetValuestring(string_item, "new_value1");
    }
    
    // Step 8: Get object items (first pair)
    if (object) {
        cJSON_GetObjectItem(object, "key1");
        cJSON_GetObjectItem(object, "key2");
    }
    
    // Step 9: Set valuestring on the string item again
    if (string_item) {
        cJSON_SetValuestring(string_item, "new_value2");
    }
    
    // Step 10: Get object items (second pair)
    if (object) {
        cJSON_GetObjectItem(object, "key1");
        cJSON_GetObjectItem(object, "key2");
    }
    
    // Step 11: Set valuestring on the string item third time
    if (string_item) {
        cJSON_SetValuestring(string_item, "new_value3");
    }
    
    // Step 12: Get object items (third pair)
    if (object) {
        cJSON_GetObjectItem(object, "key1");
        cJSON_GetObjectItem(object, "key2");
    }
    
    // Step 13: Clean up
    if (parsed) {
        cJSON_Delete(parsed);
    }
    if (object) {
        cJSON_Delete(object);
    }
    // Note: string_ref_item is part of object, so it's deleted with object
    // string_item is also part of object, so it's deleted with object
    
    free(json_str);
    return 0;
}