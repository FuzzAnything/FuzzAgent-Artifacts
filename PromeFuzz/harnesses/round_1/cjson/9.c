// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
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

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated string for parsing
    char *json_str = (char *)malloc(Size + 1);
    if (json_str == NULL) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    // Step 1: Parse JSON
    cJSON *parsed = cJSON_Parse(json_str);
    
    // Step 2: Get error pointer (regardless of parse success)
    const char *error_ptr = cJSON_GetErrorPtr();
    
    // Only proceed with object operations if parsing succeeded
    if (parsed != NULL) {
        // Step 3: Get first object item
        cJSON *item1 = cJSON_GetObjectItemCaseSensitive(parsed, "key1");
        
        // Step 4: Check if it's a string
        cJSON_bool is_string = cJSON_IsString(item1);
        
        // Steps 5-7: Get three more object items
        cJSON *item2 = cJSON_GetObjectItemCaseSensitive(parsed, "key2");
        cJSON *item3 = cJSON_GetObjectItemCaseSensitive(parsed, "key3");
        cJSON *item4 = cJSON_GetObjectItemCaseSensitive(parsed, "key4");
        
        // Steps 8-9: Check if items are numbers
        cJSON_bool is_num1 = cJSON_IsNumber(item2);
        cJSON_bool is_num2 = cJSON_IsNumber(item3);
        
        // Step 10: Clean up
        cJSON_Delete(parsed);
    }
    
    // Clean up allocated string
    free(json_str);
    
    return 0;
}