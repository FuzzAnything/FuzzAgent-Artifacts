// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
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
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
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

static void fuzz_cjson_apis(cJSON *root) {
    if (!root) return;
    
    // First cJSON_GetObjectItemCaseSensitive
    cJSON *item1 = cJSON_GetObjectItemCaseSensitive(root, "key1");
    
    // cJSON_IsString
    cJSON_bool is_string = cJSON_IsString(item1);
    
    // Second cJSON_GetObjectItemCaseSensitive
    cJSON *item2 = cJSON_GetObjectItemCaseSensitive(root, "key2");
    
    // cJSON_IsTrue
    cJSON_bool is_true = cJSON_IsTrue(item2);
    
    // Third cJSON_GetObjectItemCaseSensitive
    cJSON *item3 = cJSON_GetObjectItemCaseSensitive(root, "key3");
    
    // Fourth cJSON_GetObjectItemCaseSensitive
    cJSON *item4 = cJSON_GetObjectItemCaseSensitive(root, "key4");
    
    // cJSON_Duplicate
    cJSON *duplicate = cJSON_Duplicate(item4, 1);
    
    // Fifth cJSON_GetObjectItemCaseSensitive
    cJSON *item5 = cJSON_GetObjectItemCaseSensitive(root, "key5");
    
    // Sixth cJSON_GetObjectItemCaseSensitive
    cJSON *item6 = cJSON_GetObjectItemCaseSensitive(root, "key6");
    
    // cJSON_Compare
    cJSON_bool compare_result = cJSON_Compare(item5, item6, 1);
    
    // Cleanup
    cJSON_Delete(duplicate);
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string from the fuzzer input
    char *json_string = (char *)malloc(Size + 1);
    if (!json_string) return 0;
    
    memcpy(json_string, Data, Size);
    json_string[Size] = '\0';
    
    // Parse the JSON string
    cJSON *root = cJSON_Parse(json_string);
    
    // Call the API functions
    fuzz_cjson_apis(root);
    
    // Cleanup
    cJSON_Delete(root);
    free(json_string);
    
    return 0;
}