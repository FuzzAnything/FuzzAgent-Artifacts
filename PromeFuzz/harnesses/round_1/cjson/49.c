// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
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
#include <stdio.h>

static void fuzz_cjson_functions(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Create a dummy JSON string from fuzzer input
    char *json_str = (char*)malloc(size + 1);
    if (!json_str) return;
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';
    
    // Parse JSON
    cJSON *parsed = cJSON_Parse(json_str);
    free(json_str);
    
    if (!parsed) {
        // If parsing fails, create a simple object for testing
        parsed = cJSON_CreateObject();
        if (!parsed) return;
        
        cJSON_AddStringToObject(parsed, "key1", "value1");
        cJSON_AddNumberToObject(parsed, "key2", 42);
        cJSON_AddBoolToObject(parsed, "key3", 1);
        
        cJSON *array = cJSON_CreateArray();
        if (array) {
            cJSON_AddItemToArray(array, cJSON_CreateNumber(1));
            cJSON_AddItemToArray(array, cJSON_CreateNumber(2));
            cJSON_AddItemToArray(array, cJSON_CreateNumber(3));
            cJSON_AddItemToObject(parsed, "array", array);
        }
    }
    
    // Test cJSON_IsObject
    cJSON_bool is_obj = cJSON_IsObject(parsed);
    (void)is_obj; // Use result to avoid unused variable warning
    
    // Generate key from fuzzer data
    char key[256];
    size_t key_len = size % 256;
    if (key_len > 0) {
        memcpy(key, data, key_len);
        key[key_len] = '\0';
    } else {
        strcpy(key, "test_key");
    }
    
    // Test object functions
    cJSON *item1 = cJSON_GetObjectItemCaseSensitive(parsed, key);
    (void)item1;
    
    cJSON *item2 = cJSON_GetObjectItem(parsed, key);
    (void)item2;
    
    cJSON_bool has_item = cJSON_HasObjectItem(parsed, key);
    (void)has_item;
    
    // Test array functions
    cJSON *array_item = cJSON_GetArrayItem(parsed, (int)(size % 10));
    (void)array_item;
    
    // Create another JSON object for comparison
    cJSON *other_obj = cJSON_CreateObject();
    if (other_obj) {
        cJSON_AddStringToObject(other_obj, "key1", "value1");
        cJSON_AddNumberToObject(other_obj, "key2", 42);
        
        // Test cJSON_Compare with different case sensitivity settings
        cJSON_bool compare1 = cJSON_Compare(parsed, other_obj, 1);
        cJSON_bool compare2 = cJSON_Compare(parsed, other_obj, 0);
        (void)compare1;
        (void)compare2;
        
        cJSON_Delete(other_obj);
    }
    
    // Cleanup
    cJSON_Delete(parsed);
}

int LLVMFuzzerTestOneInput_49(const uint8_t *data, size_t size) {
    fuzz_cjson_functions(data, size);
    return 0;
}