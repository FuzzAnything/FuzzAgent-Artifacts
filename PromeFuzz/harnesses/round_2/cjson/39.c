// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_IsObject at cJSON.c:3052:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItem at cJSON.c:1983:23 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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

static void test_functions(cJSON *json) {
    if (!json) return;
    
    // Test cJSON_IsObject
    cJSON_IsObject(json);
    
    // Test cJSON_GetArrayItem if json is an array
    if (cJSON_IsArray(json)) {
        int array_size = cJSON_GetArraySize(json);
        for (int i = -5; i < array_size + 5; i++) {
            cJSON_GetArrayItem(json, i);
        }
    }
    
    // Test object-related functions if json is an object
    if (cJSON_IsObject(json)) {
        // Test with various keys
        const char *test_keys[] = {"key", "name", "value", "test", "item", NULL};
        for (int i = 0; test_keys[i] != NULL; i++) {
            cJSON_GetObjectItemCaseSensitive(json, test_keys[i]);
            cJSON_GetObjectItem(json, test_keys[i]);
            cJSON_HasObjectItem(json, test_keys[i]);
        }
        
        // Test with empty string
        cJSON_GetObjectItemCaseSensitive(json, "");
        cJSON_GetObjectItem(json, "");
        cJSON_HasObjectItem(json, "");
    }
    
    // Test cJSON_Compare with various combinations
    cJSON_Compare(json, json, 1);  // Compare with itself, case sensitive
    cJSON_Compare(json, json, 0);  // Compare with itself, case insensitive
    
    // Compare with NULL
    cJSON_Compare(json, NULL, 1);
    cJSON_Compare(NULL, json, 1);
    cJSON_Compare(NULL, NULL, 1);
    
    // Create a duplicate for comparison
    cJSON *duplicate = cJSON_Duplicate(json, 1);
    if (duplicate) {
        cJSON_Compare(json, duplicate, 1);
        cJSON_Compare(json, duplicate, 0);
        cJSON_Delete(duplicate);
    }
    
    // Test with children
    for (cJSON *child = json->child; child != NULL; child = child->next) {
        test_functions(child);
    }
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    // Write data to file if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Parse JSON from the file
    fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;
    
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char *buffer = (char *)malloc(file_size + 1);
    if (!buffer) {
        fclose(fp);
        return 0;
    }
    
    size_t read_size = fread(buffer, 1, file_size, fp);
    buffer[read_size] = '\0';
    fclose(fp);
    
    // Parse JSON
    cJSON *json = cJSON_Parse(buffer);
    free(buffer);
    
    if (json) {
        test_functions(json);
        cJSON_Delete(json);
    }
    
    // Also test with direct parsing from memory
    char *data_copy = (char *)malloc(Size + 1);
    if (data_copy) {
        memcpy(data_copy, Data, Size);
        data_copy[Size] = '\0';
        
        cJSON *json2 = cJSON_Parse(data_copy);
        if (json2) {
            test_functions(json2);
            cJSON_Delete(json2);
        }
        free(data_copy);
    }
    
    return 0;
}