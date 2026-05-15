// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateStringReference at cJSON.c:2548:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_SetValuestring at cJSON.c:435:21 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Step 1: Parse JSON from input data
    char *json_string = (char *)malloc(Size + 1);
    if (!json_string) return 0;
    memcpy(json_string, Data, Size);
    json_string[Size] = '\0';
    
    cJSON *parsed = cJSON_Parse(json_string);
    free(json_string);
    
    // Step 2: Create string items
    cJSON *string1 = cJSON_CreateString("test_string");
    cJSON *string_ref = cJSON_CreateStringReference("reference_string");
    
    // Step 3: Add items to object if parsed successfully and is an object
    if (parsed && parsed->type == cJSON_Object) {
        // Add first string item
        cJSON_AddItemToObject(parsed, "key1", string1);
        // Add second string reference item
        cJSON_AddItemToObject(parsed, "key2", string_ref);
        
        // Step 4: Set valuestring and get object items multiple times
        cJSON_SetValuestring(cJSON_GetObjectItem(parsed, "key1"), "new_value1");
        
        cJSON_GetObjectItem(parsed, "key1");
        cJSON_GetObjectItem(parsed, "key2");
        
        cJSON_SetValuestring(cJSON_GetObjectItem(parsed, "key2"), "new_value2");
        
        cJSON_GetObjectItem(parsed, "key1");
        cJSON_GetObjectItem(parsed, "key2");
        
        cJSON_SetValuestring(cJSON_GetObjectItem(parsed, "key1"), "new_value3");
        
        cJSON_GetObjectItem(parsed, "key1");
        cJSON_GetObjectItem(parsed, "key2");
    } else {
        // Clean up created items if not added to object
        if (string1) cJSON_Delete(string1);
        if (string_ref) cJSON_Delete(string_ref);
    }
    
    // Final cleanup
    if (parsed) cJSON_Delete(parsed);
    
    return 0;
}