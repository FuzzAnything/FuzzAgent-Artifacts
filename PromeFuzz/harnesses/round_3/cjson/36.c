// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddObjectToObject at cJSON.c:2234:22 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
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

static char* create_string_from_data(const uint8_t* data, size_t size, size_t max_len) {
    if (size == 0) {
        char* str = malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    size_t len = size < max_len ? size : max_len;
    char* str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

static double create_double_from_data(const uint8_t* data, size_t size) {
    if (size < sizeof(double)) {
        return 0.0;
    }
    
    double value;
    memcpy(&value, data, sizeof(double));
    return value;
}

int LLVMFuzzerTestOneInput_36(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to determine fuzzing mode
    uint8_t mode = Data[0] % 8;
    const uint8_t* remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    // Create test strings from fuzzer data
    size_t key_len = remaining_size / 3;
    size_t str_len = remaining_size / 3;
    
    char* key_name = create_string_from_data(remaining_data, key_len, 256);
    char* string_value = create_string_from_data(remaining_data + key_len, str_len, 256);
    double number_value = create_double_from_data(remaining_data + key_len + str_len, 
                                                 remaining_size - key_len - str_len);
    
    // Test different scenarios based on mode
    switch (mode) {
        case 0: {
            // Test basic object creation and adding items
            cJSON* root = cJSON_CreateObject();
            if (root) {
                cJSON_AddNumberToObject(root, key_name, number_value);
                cJSON_AddStringToObject(root, key_name, string_value);
                cJSON_AddObjectToObject(root, key_name);
                
                // Verify object type
                cJSON_IsObject(root);
                
                // Check if item exists
                cJSON_HasObjectItem(root, key_name);
                
                cJSON_Delete(root);
            }
            break;
        }
        
        case 1: {
            // Test with NULL object
            cJSON_AddNumberToObject(NULL, key_name, number_value);
            cJSON_AddStringToObject(NULL, key_name, string_value);
            cJSON_AddObjectToObject(NULL, key_name);
            break;
        }
        
        case 2: {
            // Test with empty strings
            cJSON* root = cJSON_CreateObject();
            if (root) {
                cJSON_AddNumberToObject(root, "", number_value);
                cJSON_AddStringToObject(root, "", "");
                cJSON_AddObjectToObject(root, "");
                
                cJSON_IsObject(root);
                cJSON_HasObjectItem(root, "");
                
                cJSON_Delete(root);
            }
            break;
        }
        
        case 3: {
            // Test with special number values
            cJSON* root = cJSON_CreateObject();
            if (root) {
                cJSON_AddNumberToObject(root, key_name, 0.0);
                cJSON_AddNumberToObject(root, key_name, -number_value);
                cJSON_AddNumberToObject(root, key_name, number_value * 1000);
                
                cJSON_Delete(root);
            }
            break;
        }
        
        case 4: {
            // Test nested objects
            cJSON* root = cJSON_CreateObject();
            if (root) {
                cJSON* child = cJSON_AddObjectToObject(root, key_name);
                if (child) {
                    cJSON_AddNumberToObject(child, "nested_num", number_value);
                    cJSON_AddStringToObject(child, "nested_str", string_value);
                    
                    cJSON_IsObject(child);
                    cJSON_HasObjectItem(child, "nested_num");
                }
                
                cJSON_Delete(root);
            }
            break;
        }
        
        case 5: {
            // Test multiple additions
            cJSON* root = cJSON_CreateObject();
            if (root) {
                for (int i = 0; i < 5 && i < key_len; i++) {
                    char temp_key[2] = {key_name[i % key_len], '\0'};
                    cJSON_AddNumberToObject(root, temp_key, number_value + i);
                    cJSON_AddStringToObject(root, temp_key, string_value);
                    
                    cJSON_HasObjectItem(root, temp_key);
                }
                
                cJSON_Delete(root);
            }
            break;
        }
        
        case 6: {
            // Test IsObject with various inputs
            cJSON* root = cJSON_CreateObject();
            cJSON_IsObject(root);
            cJSON_IsObject(NULL);
            
            if (root) {
                cJSON* num_item = cJSON_AddNumberToObject(root, key_name, number_value);
                cJSON_IsObject(num_item);  // Should return false for number item
                
                cJSON* str_item = cJSON_AddStringToObject(root, "test", "value");
                cJSON_IsObject(str_item);  // Should return false for string item
                
                cJSON_Delete(root);
            }
            break;
        }
        
        case 7: {
            // Test HasObjectItem with various inputs
            cJSON* root = cJSON_CreateObject();
            if (root) {
                cJSON_AddNumberToObject(root, "key1", 1.0);
                cJSON_AddStringToObject(root, "key2", "value");
                
                cJSON_HasObjectItem(root, key_name);
                cJSON_HasObjectItem(root, "");
                cJSON_HasObjectItem(NULL, key_name);
                cJSON_HasObjectItem(root, NULL);
                
                cJSON_Delete(root);
            }
            break;
        }
    }
    
    // Cleanup
    free(key_name);
    free(string_value);
    
    return 0;
}