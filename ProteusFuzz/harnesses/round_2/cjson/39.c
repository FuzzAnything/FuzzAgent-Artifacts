// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_AddFalseToObject at cJSON.c:2174:22 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void free_cjson_tree(cJSON *item) {
    if (item == NULL) {
        return;
    }
    cJSON_Delete(item);
}

static cJSON *create_random_cjson(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    // Create a buffer with a null terminator
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

static cJSON *create_manual_cjson(const uint8_t *data, size_t size) {
    if (size < 2) {
        return NULL;
    }
    
    cJSON *item = cJSON_CreateObject();
    if (item == NULL) {
        return NULL;
    }
    
    // Randomly set type based on data
    int type_choice = data[0] % 6;
    
    switch (type_choice) {
        case 0:
            // String: Ensure we have enough data and create a safe null-terminated string
            if (size > 1) {
                // Find a null terminator or limit length to avoid overflow
                size_t str_len = 0;
                while (str_len < size - 1 && data[str_len + 1] != '\0') {
                    str_len++;
                }
                if (str_len > 0) {
                    char *safe_str = (char *)malloc(str_len + 1);
                    if (safe_str) {
                        memcpy(safe_str, data + 1, str_len);
                        safe_str[str_len] = '\0';
                        cJSON_AddStringToObject(item, "key", safe_str);
                        free(safe_str);
                    }
                }
            }
            break;
        case 1:
            // Number
            cJSON_AddNumberToObject(item, "key", (double)(data[1] * 256 + (size > 2 ? data[2] : 0)));
            break;
        case 2:
            // Null
            cJSON_AddNullToObject(item, "key");
            break;
        case 3:
            // True
            cJSON_AddTrueToObject(item, "key");
            break;
        case 4:
            // False
            cJSON_AddFalseToObject(item, "key");
            break;
        case 5:
            // Array
            cJSON *arr = cJSON_CreateArray();
            if (arr != NULL) {
                cJSON_AddItemToObject(item, "key", arr);
            }
            break;
    }
    
    return item;
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Test with parsed JSON
    cJSON *item1 = create_random_cjson(Data, Size);
    cJSON *item2 = create_random_cjson(Data, Size);
    cJSON *item3 = create_manual_cjson(Data, Size);
    
    // Test cJSON_IsString
    cJSON_IsString(item1);
    cJSON_IsString(item2);
    cJSON_IsString(item3);
    cJSON_IsString(NULL);
    
    // Test cJSON_IsNull
    cJSON_IsNull(item1);
    cJSON_IsNull(item2);
    cJSON_IsNull(item3);
    cJSON_IsNull(NULL);
    
    // Test cJSON_Compare
    cJSON_Compare(item1, item2, 1);
    cJSON_Compare(item1, item2, 0);
    cJSON_Compare(item1, item3, 1);
    cJSON_Compare(item1, NULL, 1);
    cJSON_Compare(NULL, item2, 0);
    cJSON_Compare(NULL, NULL, 1);
    
    // Test cJSON_IsObject
    cJSON_IsObject(item1);
    cJSON_IsObject(item2);
    cJSON_IsObject(item3);
    cJSON_IsObject(NULL);
    
    // Test cJSON_IsRaw
    cJSON_IsRaw(item1);
    cJSON_IsRaw(item2);
    cJSON_IsRaw(item3);
    cJSON_IsRaw(NULL);
    
    // Test cJSON_IsTrue
    cJSON_IsTrue(item1);
    cJSON_IsTrue(item2);
    cJSON_IsTrue(item3);
    cJSON_IsTrue(NULL);
    
    // Cleanup
    free_cjson_tree(item1);
    free_cjson_tree(item2);
    free_cjson_tree(item3);
    
    return 0;
}