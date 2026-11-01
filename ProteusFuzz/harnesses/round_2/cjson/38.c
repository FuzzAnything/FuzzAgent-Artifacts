// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

static cJSON *create_fuzzed_cjson(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    // Create a null-terminated string from fuzz data
    char *json_str = (char *)malloc(size + 1);
    if (json_str == NULL) {
        return NULL;
    }
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *item = cJSON_Parse(json_str);
    free(json_str);
    
    return item;
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Split input into two parts for comparing two JSON items
    size_t split_point = Size / 2;
    if (split_point == 0) {
        split_point = 1;
    }
    
    cJSON *item1 = create_fuzzed_cjson(Data, split_point);
    cJSON *item2 = create_fuzzed_cjson(Data + split_point, Size - split_point);
    
    // Test cJSON_IsString
    cJSON_IsString(item1);
    cJSON_IsString(item2);
    cJSON_IsString(NULL);
    
    // Test cJSON_Compare
    cJSON_Compare(item1, item2, 1); // case sensitive
    cJSON_Compare(item1, item2, 0); // case insensitive
    cJSON_Compare(NULL, item2, 1);
    cJSON_Compare(item1, NULL, 1);
    cJSON_Compare(NULL, NULL, 1);
    cJSON_Compare(item1, item1, 1); // compare with itself
    
    // Test cJSON_IsBool
    cJSON_IsBool(item1);
    cJSON_IsBool(item2);
    cJSON_IsBool(NULL);
    
    // Test cJSON_IsObject
    cJSON_IsObject(item1);
    cJSON_IsObject(item2);
    cJSON_IsObject(NULL);
    
    // Test cJSON_IsRaw
    cJSON_IsRaw(item1);
    cJSON_IsRaw(item2);
    cJSON_IsRaw(NULL);
    
    // Test cJSON_IsTrue
    cJSON_IsTrue(item1);
    cJSON_IsTrue(item2);
    cJSON_IsTrue(NULL);
    
    // Cleanup
    cleanup_cjson(item1);
    cleanup_cjson(item2);
    
    return 0;
}