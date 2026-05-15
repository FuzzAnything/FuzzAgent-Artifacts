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
    // Ensure null termination for the JSON string
    char *json_string = (char *)malloc(Size + 1);
    if (json_string == NULL) {
        return 0;
    }
    memcpy(json_string, Data, Size);
    json_string[Size] = '\0';

    // 1. cJSON_Parse
    cJSON *root = cJSON_Parse(json_string);
    
    // 2. cJSON_GetErrorPtr
    const char *error_ptr = cJSON_GetErrorPtr();
    
    if (root != NULL) {
        // 3. cJSON_GetObjectItemCaseSensitive
        cJSON *item1 = cJSON_GetObjectItemCaseSensitive(root, "key1");
        
        // 4. cJSON_IsString
        cJSON_bool is_string = cJSON_IsString(item1);
        
        // 5. cJSON_GetObjectItemCaseSensitive
        cJSON *item2 = cJSON_GetObjectItemCaseSensitive(root, "key2");
        
        // 6. cJSON_GetObjectItemCaseSensitive
        cJSON *item3 = cJSON_GetObjectItemCaseSensitive(root, "key3");
        
        // 7. cJSON_GetObjectItemCaseSensitive
        cJSON *item4 = cJSON_GetObjectItemCaseSensitive(root, "key4");
        
        // 8. cJSON_IsNumber
        cJSON_bool is_number1 = cJSON_IsNumber(item3);
        
        // 9. cJSON_IsNumber
        cJSON_bool is_number2 = cJSON_IsNumber(item4);
    }
    
    // 10. cJSON_Delete
    cJSON_Delete(root);
    
    free(json_string);
    return 0;
}