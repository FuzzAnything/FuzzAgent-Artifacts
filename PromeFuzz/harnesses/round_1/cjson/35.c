// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_GetObjectItemCaseSensitive at cJSON.c:1988:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

static void fuzz_cjson_functions(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Create a dummy file for parsing if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    // Parse JSON from the dummy file
    fp = fopen("./dummy_file", "rb");
    if (!fp) return;
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char *buffer = (char*)malloc(file_size + 1);
    if (!buffer) {
        fclose(fp);
        return;
    }
    fread(buffer, 1, file_size, fp);
    buffer[file_size] = '\0';
    fclose(fp);
    
    cJSON *parsed = cJSON_Parse(buffer);
    free(buffer);
    
    if (!parsed) {
        // Test functions with NULL input
        cJSON_IsString(NULL);
        cJSON_IsInvalid(NULL);
        cJSON_IsTrue(NULL);
        cJSON_Duplicate(NULL, 0);
        cJSON_Duplicate(NULL, 1);
        cJSON_Compare(NULL, NULL, 0);
        cJSON_Compare(NULL, NULL, 1);
        return;
    }
    
    // Test cJSON_GetObjectItemCaseSensitive on the parsed object
    if (parsed->type == cJSON_Object) {
        // Try with various keys from the input data
        size_t key_len = size > 100 ? 100 : size;
        char *key = (char*)malloc(key_len + 1);
        if (key) {
            memcpy(key, data, key_len);
            key[key_len] = '\0';
            cJSON_GetObjectItemCaseSensitive(parsed, key);
            free(key);
        }
        
        // Also try with fixed keys
        cJSON_GetObjectItemCaseSensitive(parsed, "test");
        cJSON_GetObjectItemCaseSensitive(parsed, "");
    }
    
    // Create a simple stack for traversal
    #define MAX_STACK_SIZE 1000
    cJSON *stack[MAX_STACK_SIZE];
    int stack_top = 0;
    
    // Push the root
    stack[stack_top++] = parsed;
    
    while (stack_top > 0) {
        cJSON *current = stack[--stack_top];
        
        // Process current node
        cJSON_IsString(current);
        cJSON_IsInvalid(current);
        cJSON_IsTrue(current);
        
        // Test cJSON_GetObjectItemCaseSensitive on child objects
        if (current->type == cJSON_Object && current->child) {
            cJSON *child = current->child;
            while (child) {
                if (child->string) {
                    cJSON_GetObjectItemCaseSensitive(current, child->string);
                }
                child = child->next;
            }
        }
        
        // Test cJSON_Duplicate with both recursive and non-recursive modes
        cJSON *duplicate1 = cJSON_Duplicate(current, 0);
        cJSON *duplicate2 = cJSON_Duplicate(current, 1);
        
        // Test cJSON_Compare with various combinations
        cJSON_Compare(current, duplicate1, 0);
        cJSON_Compare(current, duplicate1, 1);
        cJSON_Compare(current, duplicate2, 0);
        cJSON_Compare(current, duplicate2, 1);
        cJSON_Compare(duplicate1, duplicate2, 0);
        cJSON_Compare(duplicate1, duplicate2, 1);
        cJSON_Compare(NULL, current, 0);
        cJSON_Compare(current, NULL, 1);
        
        // Clean up duplicates
        if (duplicate1) cJSON_Delete(duplicate1);
        if (duplicate2) cJSON_Delete(duplicate2);
        
        // Push children and next siblings to stack for traversal
        if (current->next && stack_top < MAX_STACK_SIZE) {
            stack[stack_top++] = current->next;
        }
        if (current->child && stack_top < MAX_STACK_SIZE) {
            stack[stack_top++] = current->child;
        }
    }
    
    // Test cJSON_Compare on the entire parsed tree with itself
    cJSON *duplicate_root = cJSON_Duplicate(parsed, 1);
    if (duplicate_root) {
        cJSON_Compare(parsed, duplicate_root, 0);
        cJSON_Compare(parsed, duplicate_root, 1);
        cJSON_Delete(duplicate_root);
    }
    
    // Clean up
    cJSON_Delete(parsed);
}

int LLVMFuzzerTestOneInput_35(const uint8_t *data, size_t size) {
    fuzz_cjson_functions(data, size);
    return 0;
}