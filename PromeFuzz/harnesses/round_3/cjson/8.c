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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // Create null-terminated string from fuzzer input
    char *json_string = (char *)malloc(Size + 1);
    if (!json_string) return 0;
    memcpy(json_string, Data, Size);
    json_string[Size] = '\0';

    // Step 1: Parse JSON
    cJSON *root = cJSON_Parse(json_string);
    
    // Step 2: Get error pointer (even if parse succeeded)
    const char *error_ptr = cJSON_GetErrorPtr();
    
    if (root) {
        // Step 3: Get first object item
        cJSON *item1 = cJSON_GetObjectItemCaseSensitive(root, "key1");
        
        // Step 4: Check if it's a string
        cJSON_bool is_string1 = cJSON_IsString(item1);
        
        // Step 5: Get second object item
        cJSON *item2 = cJSON_GetObjectItemCaseSensitive(root, "key2");
        
        // Step 6: Get third object item
        cJSON *item3 = cJSON_GetObjectItemCaseSensitive(root, "key3");
        
        // Step 7: Get fourth object item
        cJSON *item4 = cJSON_GetObjectItemCaseSensitive(root, "key4");
        
        // Step 8: Check if third item is number
        cJSON_bool is_number3 = cJSON_IsNumber(item3);
        
        // Step 9: Check if fourth item is number
        cJSON_bool is_number4 = cJSON_IsNumber(item4);
        
        // Step 10: Clean up
        cJSON_Delete(root);
    }
    
    free(json_string);
    return 0;
}