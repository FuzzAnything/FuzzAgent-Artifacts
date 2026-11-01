// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static cJSON *parse_json_from_input(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
        return NULL;
    }
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *item = cJSON_Parse(json_str);
    free(json_str);
    
    return item;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *root = parse_json_from_input(Data, Size);
    if (!root) {
        return 0;
    }

    // Step 1: cJSON_GetObjectItemCaseSensitive
    cJSON *item1 = cJSON_GetObjectItemCaseSensitive(root, "key1");
    
    // Step 2: cJSON_IsString
    if (item1) {
        cJSON_IsString(item1);
    }
    
    // Step 3: cJSON_GetObjectItemCaseSensitive
    cJSON *item2 = cJSON_GetObjectItemCaseSensitive(root, "key2");
    
    // Step 4: cJSON_IsTrue
    if (item2) {
        cJSON_IsTrue(item2);
    }
    
    // Step 5: cJSON_GetObjectItemCaseSensitive
    cJSON *item3 = cJSON_GetObjectItemCaseSensitive(root, "key3");
    
    // Step 6: cJSON_GetObjectItemCaseSensitive
    cJSON *item4 = cJSON_GetObjectItemCaseSensitive(root, "key4");
    
    // Step 7: cJSON_Duplicate
    cJSON *dup_item = NULL;
    if (item4) {
        dup_item = cJSON_Duplicate(item4, 1); // Deep copy
    } else if (item3) {
        dup_item = cJSON_Duplicate(item3, 1);
    } else if (item2) {
        dup_item = cJSON_Duplicate(item2, 1);
    } else if (item1) {
        dup_item = cJSON_Duplicate(item1, 1);
    } else {
        dup_item = cJSON_Duplicate(root, 1);
    }
    
    // Step 8: cJSON_GetObjectItemCaseSensitive
    cJSON *item5 = NULL;
    if (dup_item) {
        item5 = cJSON_GetObjectItemCaseSensitive(dup_item, "nested_key");
    }
    
    // Step 9: cJSON_GetObjectItemCaseSensitive
    cJSON *item6 = NULL;
    if (item5) {
        item6 = cJSON_GetObjectItemCaseSensitive(item5, "deep_key");
    }
    
    // Step 10: cJSON_Compare
    cJSON_bool compare_result = cJSON_Compare(root, dup_item, 1);
    (void)compare_result;
    
    // Step 11: cJSON_Delete
    cJSON_Delete(dup_item);
    cJSON_Delete(root);
    
    return 0;
}