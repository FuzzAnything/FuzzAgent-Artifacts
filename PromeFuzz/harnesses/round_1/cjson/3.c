// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "cJSON.h"

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Parse input as JSON
    char *json_string = (char *)malloc(Size + 1);
    if (!json_string) {
        return 0;
    }
    memcpy(json_string, Data, Size);
    json_string[Size] = '\0';

    cJSON *parsed = cJSON_Parse(json_string);
    free(json_string);

    if (!parsed) {
        return 0;
    }

    // Create some dummy keys for object lookups
    const char *keys[] = {"key1", "key2", "key3", "key4", "key5", "key6"};
    
    // First cJSON_GetObjectItemCaseSensitive
    cJSON *item1 = cJSON_GetObjectItemCaseSensitive(parsed, keys[0]);
    
    // cJSON_IsString
    if (item1) {
        cJSON_IsString(item1);
    }
    
    // Second cJSON_GetObjectItemCaseSensitive
    cJSON *item2 = cJSON_GetObjectItemCaseSensitive(parsed, keys[1]);
    
    // cJSON_IsTrue
    if (item2) {
        cJSON_IsTrue(item2);
    }
    
    // Third cJSON_GetObjectItemCaseSensitive
    cJSON *item3 = cJSON_GetObjectItemCaseSensitive(parsed, keys[2]);
    
    // Fourth cJSON_GetObjectItemCaseSensitive
    cJSON *item4 = cJSON_GetObjectItemCaseSensitive(parsed, keys[3]);
    
    // cJSON_Duplicate
    cJSON *duplicate = NULL;
    if (item3) {
        duplicate = cJSON_Duplicate(item3, 1); // recurse = true
    }
    
    // Fifth cJSON_GetObjectItemCaseSensitive
    cJSON *item5 = cJSON_GetObjectItemCaseSensitive(parsed, keys[4]);
    
    // Sixth cJSON_GetObjectItemCaseSensitive
    cJSON *item6 = cJSON_GetObjectItemCaseSensitive(parsed, keys[5]);
    
    // cJSON_Compare
    if (item5 && item6) {
        cJSON_Compare(item5, item6, 1); // case_sensitive = true
    }
    
    // Cleanup
    cJSON_Delete(parsed);
    if (duplicate) {
        cJSON_Delete(duplicate);
    }
    
    return 0;
}