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

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated string from fuzzer input
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
    (void)error_ptr; // Use variable to avoid unused warning
    
    if (root != NULL) {
        // 3. cJSON_GetObjectItemCaseSensitive
        cJSON *item1 = cJSON_GetObjectItemCaseSensitive(root, "key1");
        
        // 4. cJSON_IsString
        cJSON_bool is_string = cJSON_IsString(item1);
        (void)is_string;
        
        // 5. cJSON_GetObjectItemCaseSensitive
        cJSON *item2 = cJSON_GetObjectItemCaseSensitive(root, "key2");
        
        // 6. cJSON_GetObjectItemCaseSensitive
        cJSON *item3 = cJSON_GetObjectItemCaseSensitive(root, "key3");
        
        // 7. cJSON_GetObjectItemCaseSensitive
        cJSON *item4 = cJSON_GetObjectItemCaseSensitive(root, "key4");
        
        // 8. cJSON_IsNumber
        cJSON_bool is_number1 = cJSON_IsNumber(item2);
        (void)is_number1;
        
        // 9. cJSON_IsNumber
        cJSON_bool is_number2 = cJSON_IsNumber(item3);
        (void)is_number2;
        
        // 10. cJSON_Delete
        cJSON_Delete(root);
    } else {
        // Even if parsing failed, we might still have an error pointer
        // The error pointer is already obtained above
    }

    free(json_string);
    return 0;
}