// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
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

static cJSON *create_random_json(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateNull();
    }
    
    char *json_str = malloc(size + 1);
    if (!json_str) {
        return NULL;
    }
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *json = cJSON_Parse(json_str);
    free(json_str);
    
    if (!json) {
        return cJSON_CreateObject();
    }
    return json;
}

static void fuzz_is_functions(cJSON *item) {
    if (!item) return;
    
    cJSON_IsString(item);
    cJSON_IsTrue(item);
    cJSON_IsNull(item);
}

static void fuzz_duplicate_and_compare(cJSON *item) {
    if (!item) return;
    
    cJSON *duplicate1 = cJSON_Duplicate(item, 0);
    cJSON *duplicate2 = cJSON_Duplicate(item, 1);
    
    if (duplicate1) {
        cJSON_Compare(item, duplicate1, 0);
        cJSON_Compare(item, duplicate1, 1);
        cJSON_Delete(duplicate1);
    }
    
    if (duplicate2) {
        cJSON_Compare(item, duplicate2, 0);
        cJSON_Compare(item, duplicate2, 1);
        cJSON_Delete(duplicate2);
    }
    
    if (item->type == cJSON_Object) {
        cJSON_GetObjectItemCaseSensitive(item, "key");
        cJSON_GetObjectItemCaseSensitive(item, "test");
        cJSON_GetObjectItemCaseSensitive(item, "");
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *data, size_t size) {
    cJSON *json1 = create_random_json(data, size);
    if (!json1) {
        return 0;
    }
    
    fuzz_is_functions(json1);
    fuzz_duplicate_and_compare(json1);
    
    cJSON *json2 = create_random_json(data, size % 32);
    if (json2) {
        cJSON_Compare(json1, json2, size % 2);
        cJSON_Compare(json2, json1, size % 2);
        cJSON_Delete(json2);
    }
    
    cJSON_Delete(json1);
    return 0;
}