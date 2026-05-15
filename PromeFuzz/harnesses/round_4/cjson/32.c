// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_IsNull at cJSON.c:3012:26 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_AddFalseToObject at cJSON.c:2174:22 in cJSON.h
// cJSON_AddNullToObject at cJSON.c:2150:22 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cJSON.h"

static void test_cJSON_Duplicate(const cJSON *item) {
    if (item == NULL) {
        return;
    }
    
    // Test with recurse = 0
    cJSON *duplicate0 = cJSON_Duplicate(item, 0);
    if (duplicate0 != NULL) {
        cJSON_Delete(duplicate0);
    }
    
    // Test with recurse = 1
    cJSON *duplicate1 = cJSON_Duplicate(item, 1);
    if (duplicate1 != NULL) {
        cJSON_Delete(duplicate1);
    }
}

static void test_cJSON_Compare(const cJSON *a, const cJSON *b) {
    // Test with case_sensitive = 0
    cJSON_Compare(a, b, 0);
    
    // Test with case_sensitive = 1
    cJSON_Compare(a, b, 1);
    
    // Test with NULL inputs
    cJSON_Compare(NULL, b, 0);
    cJSON_Compare(a, NULL, 1);
    cJSON_Compare(NULL, NULL, 0);
}

static void test_cJSON_IsString(const cJSON *item) {
    cJSON_IsString(item);
    cJSON_IsString(NULL);
}

static void test_cJSON_GetObjectItemCaseSensitive(const cJSON *object) {
    if (object == NULL || object->type != cJSON_Object) {
        return;
    }
    
    // Try to get items with different keys
    cJSON_GetObjectItemCaseSensitive(object, "key");
    cJSON_GetObjectItemCaseSensitive(object, "name");
    cJSON_GetObjectItemCaseSensitive(object, "value");
    cJSON_GetObjectItemCaseSensitive(object, "");
    
    // Test with NULL object
    cJSON_GetObjectItemCaseSensitive(NULL, "key");
}

static void test_cJSON_IsTrue(const cJSON *item) {
    cJSON_IsTrue(item);
    cJSON_IsTrue(NULL);
}

static void test_cJSON_IsNull(const cJSON *item) {
    cJSON_IsNull(item);
    cJSON_IsNull(NULL);
}

static cJSON *create_test_json_from_data(const uint8_t *data, size_t size) {
    // Try to parse the input as JSON
    char *json_str = (char *)malloc(size + 1);
    if (json_str == NULL) {
        return NULL;
    }
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    cJSON *json = cJSON_Parse(json_str);
    free(json_str);
    
    // If parsing fails, create a simple JSON object
    if (json == NULL) {
        json = cJSON_CreateObject();
        if (json != NULL) {
            cJSON_AddStringToObject(json, "key", "value");
            cJSON_AddNumberToObject(json, "number", 42);
            cJSON_AddTrueToObject(json, "true");
            cJSON_AddFalseToObject(json, "false");
            cJSON_AddNullToObject(json, "null");
            
            // Add an array
            cJSON *array = cJSON_CreateArray();
            if (array != NULL) {
                cJSON_AddItemToArray(array, cJSON_CreateString("array_item1"));
                cJSON_AddItemToArray(array, cJSON_CreateNumber(123));
                cJSON_AddItemToObject(json, "array", array);
            }
        }
    }
    
    return json;
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create test JSON from input data
    cJSON *json1 = create_test_json_from_data(Data, Size);
    if (json1 == NULL) {
        return 0;
    }
    
    // Create another JSON for comparison
    cJSON *json2 = create_test_json_from_data(Data, Size > 1 ? Size - 1 : Size);
    
    // Test all target functions
    test_cJSON_Duplicate(json1);
    test_cJSON_Compare(json1, json2);
    test_cJSON_IsString(json1);
    test_cJSON_GetObjectItemCaseSensitive(json1);
    test_cJSON_IsTrue(json1);
    test_cJSON_IsNull(json1);
    
    // Also test on child items if they exist
    if (json1->child != NULL) {
        test_cJSON_Duplicate(json1->child);
        test_cJSON_IsString(json1->child);
        test_cJSON_IsTrue(json1->child);
        test_cJSON_IsNull(json1->child);
    }
    
    // Test on array items if json1 has an array
    cJSON *array = cJSON_GetObjectItemCaseSensitive(json1, "array");
    if (array != NULL && array->type == cJSON_Array && array->child != NULL) {
        test_cJSON_Duplicate(array->child);
        test_cJSON_IsString(array->child);
        test_cJSON_IsTrue(array->child);
        test_cJSON_IsNull(array->child);
    }
    
    // Clean up
    cJSON_Delete(json1);
    if (json2 != NULL) {
        cJSON_Delete(json2);
    }
    
    return 0;
}