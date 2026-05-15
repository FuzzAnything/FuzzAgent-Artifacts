// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsFalse at cJSON.c:2982:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_cjson_apis(const uint8_t *data, size_t size) {
    // Use first byte to determine test variations
    if (size == 0) return;
    
    uint8_t test_type = data[0] % 8;
    data++;
    size--;
    
    // Create basic boolean items
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *true_item = cJSON_CreateTrue();
    
    if (false_item) {
        // Test cJSON_IsFalse
        cJSON_bool is_false = cJSON_IsFalse(false_item);
        
        // Test cJSON_IsBool
        cJSON_bool is_bool = cJSON_IsBool(false_item);
        
        // Test cJSON_IsTrue (should return false)
        cJSON_bool is_true = cJSON_IsTrue(false_item);
        
        // Clean up
        cJSON_Delete(false_item);
    }
    
    if (true_item) {
        // Test cJSON_IsTrue
        cJSON_bool is_true = cJSON_IsTrue(true_item);
        
        // Test cJSON_IsBool
        cJSON_bool is_bool = cJSON_IsBool(true_item);
        
        // Test cJSON_IsFalse (should return false)
        cJSON_bool is_false = cJSON_IsFalse(true_item);
        
        // Clean up
        cJSON_Delete(true_item);
    }
    
    // Create various cJSON items for comparison testing
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    
    // Use fuzzer data to create different types of items
    if (size > 0) {
        // Create items based on test_type
        switch (test_type) {
            case 0:
                item1 = cJSON_CreateFalse();
                item2 = cJSON_CreateFalse();
                break;
            case 1:
                item1 = cJSON_CreateTrue();
                item2 = cJSON_CreateTrue();
                break;
            case 2:
                item1 = cJSON_CreateFalse();
                item2 = cJSON_CreateTrue();
                break;
            case 3:
                item1 = cJSON_CreateNull();
                item2 = cJSON_CreateNull();
                break;
            case 4:
                item1 = cJSON_CreateNumber(42.5);
                item2 = cJSON_CreateNumber(42.5);
                break;
            case 5:
                item1 = cJSON_CreateString("test");
                item2 = cJSON_CreateString("test");
                break;
            case 6:
                item1 = cJSON_CreateArray();
                item2 = cJSON_CreateArray();
                break;
            case 7:
                item1 = cJSON_CreateObject();
                item2 = cJSON_CreateObject();
                break;
        }
        
        // Test cJSON_Compare with different case sensitivity
        if (item1 && item2) {
            cJSON_bool case_sensitive = (size > 1) ? (data[1] % 2) : 1;
            cJSON_bool compare_result = cJSON_Compare(item1, item2, case_sensitive);
            
            // Also test with NULL inputs
            cJSON_Compare(NULL, item2, case_sensitive);
            cJSON_Compare(item1, NULL, case_sensitive);
            cJSON_Compare(NULL, NULL, case_sensitive);
        }
        
        // Test cJSON_AddBoolToObject
        if (test_type == 7 && item1) {  // item1 is an object
            // Create a name from fuzzer data if available
            char name[32] = {0};
            if (size > 2) {
                size_t name_len = (size - 2 > 31) ? 31 : size - 2;
                memcpy(name, data + 2, name_len);
                name[name_len] = '\0';
            } else {
                strcpy(name, "fuzz");
            }
            
            // Add boolean to object
            cJSON_bool bool_val = (size > 1) ? (data[1] % 2) : 1;
            cJSON *added_item = cJSON_AddBoolToObject(item1, name, bool_val);
            
            // Test the added item if successful
            if (added_item) {
                cJSON_IsBool(added_item);
                cJSON_IsTrue(added_item);
                cJSON_IsFalse(added_item);
            }
        }
        
        // Clean up
        if (item1) cJSON_Delete(item1);
        if (item2) cJSON_Delete(item2);
    }
    
    // Additional test: Create and test boolean items with fuzzer-controlled values
    if (size > 0) {
        cJSON *dynamic_false = cJSON_CreateFalse();
        cJSON *dynamic_true = cJSON_CreateTrue();
        
        if (dynamic_false && dynamic_true) {
            // Use fuzzer data to determine which comparisons to perform
            for (size_t i = 0; i < (size < 10 ? size : 10); i++) {
                uint8_t op = data[i % size] % 4;
                cJSON_bool case_sens = data[i % size] % 2;
                
                switch (op) {
                    case 0:
                        cJSON_Compare(dynamic_false, dynamic_false, case_sens);
                        break;
                    case 1:
                        cJSON_Compare(dynamic_true, dynamic_true, case_sens);
                        break;
                    case 2:
                        cJSON_Compare(dynamic_false, dynamic_true, case_sens);
                        break;
                    case 3:
                        cJSON_Compare(dynamic_true, dynamic_false, case_sens);
                        break;
                }
            }
        }
        
        if (dynamic_false) cJSON_Delete(dynamic_false);
        if (dynamic_true) cJSON_Delete(dynamic_true);
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    // Call the main fuzzing function
    fuzz_cjson_apis(Data, Size);
    
    return 0;
}