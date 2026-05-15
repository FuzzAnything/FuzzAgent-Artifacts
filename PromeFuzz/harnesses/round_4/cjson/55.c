// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    // Create a dummy file for any potential file operations
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }

    // Use fuzzer data to create keys and values
    // Ensure we have at least some data to work with
    if (Size < 4) {
        return 0;
    }

    // Create strings from fuzzer data (with null termination)
    size_t str_len = Size / 4;
    if (str_len > 100) str_len = 100; // Limit string length
    
    char *key1 = malloc(str_len + 1);
    char *key2 = malloc(str_len + 1);
    char *key3 = malloc(str_len + 1);
    char *string_val = malloc(str_len + 1);
    
    if (!key1 || !key2 || !key3 || !string_val) {
        free(key1);
        free(key2);
        free(key3);
        free(string_val);
        return 0;
    }
    
    // Copy data to strings with null termination
    memcpy(key1, Data, str_len);
    key1[str_len] = '\0';
    
    memcpy(key2, Data + str_len, str_len);
    key2[str_len] = '\0';
    
    memcpy(key3, Data + (2 * str_len), str_len);
    key3[str_len] = '\0';
    
    memcpy(string_val, Data + (3 * str_len), str_len);
    string_val[str_len] = '\0';
    
    // Generate a number from the data
    double number_val = 0.0;
    if (Size >= 8) {
        // Use first 8 bytes to create a double
        uint64_t int_val = 0;
        memcpy(&int_val, Data, sizeof(uint64_t));
        number_val = (double)int_val / 1000.0;
    }

    // Test cJSON_CreateObject
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        free(key1);
        free(key2);
        free(key3);
        free(string_val);
        return 0;
    }

    // Test cJSON_AddStringToObject
    cJSON *string_item = cJSON_AddStringToObject(root, key1, string_val);
    
    // Test cJSON_AddNumberToObject
    cJSON *number_item = cJSON_AddNumberToObject(root, key2, number_val);
    
    // Test cJSON_AddObjectToObject
    cJSON *nested_object = cJSON_AddObjectToObject(root, key3);
    
    // Test cJSON_IsObject on various objects
    cJSON_bool is_root_object = cJSON_IsObject(root);
    cJSON_bool is_string_item_object = cJSON_IsObject(string_item);
    cJSON_bool is_number_item_object = cJSON_IsObject(number_item);
    cJSON_bool is_nested_object_object = cJSON_IsObject(nested_object);
    
    // Test cJSON_HasObjectItem for various keys
    cJSON_bool has_key1 = cJSON_HasObjectItem(root, key1);
    cJSON_bool has_key2 = cJSON_HasObjectItem(root, key2);
    cJSON_bool has_key3 = cJSON_HasObjectItem(root, key3);
    cJSON_bool has_nonexistent = cJSON_HasObjectItem(root, "nonexistent_key");
    
    // Add more items to nested object to explore more states
    if (nested_object) {
        cJSON_AddStringToObject(nested_object, "nested_key", "nested_value");
        cJSON_AddNumberToObject(nested_object, "nested_number", 42.5);
        
        // Create another nested object
        cJSON *double_nested = cJSON_AddObjectToObject(nested_object, "double_nested");
        if (double_nested) {
            cJSON_AddStringToObject(double_nested, "deep_key", "deep_value");
        }
    }
    
    // Test with NULL inputs
    cJSON_AddStringToObject(NULL, "test", "value");
    cJSON_AddNumberToObject(NULL, "test", 1.0);
    cJSON_AddObjectToObject(NULL, "test");
    cJSON_IsObject(NULL);
    cJSON_HasObjectItem(NULL, "test");
    cJSON_HasObjectItem(root, NULL);
    
    // Test with empty string key
    cJSON_AddStringToObject(root, "", "empty_key_value");
    
    // Clean up
    cJSON_Delete(root);
    free(key1);
    free(key2);
    free(key3);
    free(string_val);
    
    return 0;
}