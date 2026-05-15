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

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Create a dummy JSON string from the fuzzer input
    char *json_string = malloc(Size + 1);
    if (json_string == NULL) return 0;
    memcpy(json_string, Data, Size);
    json_string[Size] = '\0';

    // Parse the JSON string
    cJSON *parsed_json = cJSON_Parse(json_string);
    free(json_string);
    if (parsed_json == NULL) return 0;

    // Define some dummy keys for object lookups
    const char *key1 = "key1";
    const char *key2 = "key2";
    const char *key3 = "key3";
    const char *key4 = "key4";

    // Call functions in the specified order
    cJSON *item1 = cJSON_GetObjectItemCaseSensitive(parsed_json, key1);
    cJSON_bool is_string = cJSON_IsString(item1);
    
    cJSON *item2 = cJSON_GetObjectItemCaseSensitive(parsed_json, key2);
    cJSON_bool is_true = cJSON_IsTrue(item2);
    
    cJSON *item3 = cJSON_GetObjectItemCaseSensitive(parsed_json, key3);
    cJSON *item4 = cJSON_GetObjectItemCaseSensitive(parsed_json, key4);
    
    cJSON *duplicated = cJSON_Duplicate(item4, 0);
    
    cJSON *item5 = cJSON_GetObjectItemCaseSensitive(parsed_json, key1);
    cJSON *item6 = cJSON_GetObjectItemCaseSensitive(parsed_json, key2);
    
    cJSON_bool compare_result = cJSON_Compare(item5, item6, 1);
    
    // Cleanup
    cJSON_Delete(duplicated);
    cJSON_Delete(parsed_json);
    
    return 0;
}

#ifdef __cplusplus
}
#endif