// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
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

static void test_boolean_functions(void) {
    cJSON *false_item = cJSON_CreateFalse();
    if (false_item != NULL) {
        // Test boolean checking functions
        cJSON_bool is_false = cJSON_IsFalse(false_item);
        cJSON_bool is_bool = cJSON_IsBool(false_item);
        cJSON_bool is_true = cJSON_IsTrue(false_item);
        
        // Create a true item for comparison
        cJSON *true_item = cJSON_CreateTrue();
        if (true_item != NULL) {
            // Test comparison
            cJSON_bool compare_same = cJSON_Compare(false_item, false_item, 1);
            cJSON_bool compare_diff = cJSON_Compare(false_item, true_item, 1);
            cJSON_bool compare_diff_case = cJSON_Compare(false_item, true_item, 0);
            
            cJSON_Delete(true_item);
        }
        
        cJSON_Delete(false_item);
    }
    
    // Test with NULL inputs
    cJSON_bool null_is_true = cJSON_IsTrue(NULL);
    cJSON_bool null_is_bool = cJSON_IsBool(NULL);
    cJSON_bool null_is_false = cJSON_IsFalse(NULL);
    cJSON_bool null_compare = cJSON_Compare(NULL, NULL, 1);
}

static void test_add_bool_to_object(const uint8_t *data, size_t size) {
    cJSON *object = cJSON_CreateObject();
    if (object != NULL) {
        // Create a name from fuzzer data
        char *name = NULL;
        if (size > 0) {
            // Limit name length to avoid excessive memory usage
            size_t name_len = size > 100 ? 100 : size;
            name = (char*)malloc(name_len + 1);
            if (name != NULL) {
                memcpy(name, data, name_len);
                name[name_len] = '\0';
                
                // Test adding both true and false
                cJSON *added_true = cJSON_AddBoolToObject(object, name, 1);
                cJSON *added_false = cJSON_AddBoolToObject(object, name, 0);
                
                free(name);
            }
        } else {
            // Test with empty name
            cJSON *added = cJSON_AddBoolToObject(object, "", 1);
        }
        
        cJSON_Delete(object);
    }
}

static void test_comparisons(const uint8_t *data, size_t size) {
    // Create various boolean items for comparison
    cJSON *false1 = cJSON_CreateFalse();
    cJSON *false2 = cJSON_CreateFalse();
    cJSON *true1 = cJSON_CreateTrue();
    
    if (false1 != NULL && false2 != NULL && true1 != NULL) {
        // Test case-sensitive comparisons
        cJSON_bool comp1 = cJSON_Compare(false1, false2, 1);
        cJSON_bool comp2 = cJSON_Compare(false1, true1, 1);
        cJSON_bool comp3 = cJSON_Compare(true1, true1, 1);
        
        // Test case-insensitive comparisons (shouldn't matter for booleans)
        cJSON_bool comp4 = cJSON_Compare(false1, false2, 0);
        cJSON_bool comp5 = cJSON_Compare(false1, true1, 0);
        
        // Test with NULL
        cJSON_bool comp6 = cJSON_Compare(false1, NULL, 1);
        cJSON_bool comp7 = cJSON_Compare(NULL, true1, 1);
        cJSON_bool comp8 = cJSON_Compare(NULL, NULL, 1);
    }
    
    if (false1 != NULL) cJSON_Delete(false1);
    if (false2 != NULL) cJSON_Delete(false2);
    if (true1 != NULL) cJSON_Delete(true1);
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    // Test basic boolean functions
    test_boolean_functions();
    
    // Test adding booleans to objects with fuzzer data
    test_add_bool_to_object(Data, Size);
    
    // Test comparisons
    test_comparisons(Data, Size);
    
    // Additional test: create nested structure with booleans
    cJSON *root = cJSON_CreateObject();
    if (root != NULL) {
        cJSON *array = cJSON_CreateArray();
        if (array != NULL) {
            cJSON_AddItemToArray(array, cJSON_CreateFalse());
            cJSON_AddItemToArray(array, cJSON_CreateTrue());
            cJSON_AddItemToObject(root, "booleans", array);
            
            // Test boolean checks on array items
            cJSON *first_item = cJSON_GetArrayItem(array, 0);
            if (first_item != NULL) {
                cJSON_bool is_bool1 = cJSON_IsBool(first_item);
                cJSON_bool is_false1 = cJSON_IsFalse(first_item);
            }
            
            cJSON *second_item = cJSON_GetArrayItem(array, 1);
            if (second_item != NULL) {
                cJSON_bool is_bool2 = cJSON_IsBool(second_item);
                cJSON_bool is_true2 = cJSON_IsTrue(second_item);
            }
        }
        
        cJSON_Delete(root);
    }
    
    return 0;
}