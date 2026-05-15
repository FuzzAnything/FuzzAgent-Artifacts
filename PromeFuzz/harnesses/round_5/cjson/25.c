// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsInvalid at cJSON.c:2972:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void test_functions(const cJSON *item1, const cJSON *item2, const uint8_t *data, size_t size) {
    // Use data to influence behavior
    cJSON_bool recurse = (size > 0) ? (data[0] & 1) : 0;
    cJSON_bool case_sensitive = (size > 0) ? ((data[0] >> 1) & 1) : 0;
    
    // Test cJSON_IsTrue
    (void)cJSON_IsTrue(item1);
    (void)cJSON_IsTrue(item2);
    (void)cJSON_IsTrue(NULL);
    
    // Test cJSON_IsInvalid
    (void)cJSON_IsInvalid(item1);
    (void)cJSON_IsInvalid(item2);
    (void)cJSON_IsInvalid(NULL);
    
    // Test cJSON_CreateFalse
    cJSON *false_item = cJSON_CreateFalse();
    if (false_item != NULL) {
        (void)cJSON_IsTrue(false_item);
        (void)cJSON_IsInvalid(false_item);
        cJSON_Delete(false_item);
    }
    
    // Test cJSON_Duplicate
    cJSON *dup = cJSON_Duplicate(item1, recurse);
    if (dup != NULL) {
        (void)cJSON_IsString(dup);
        (void)cJSON_IsInvalid(dup);
        cJSON_Delete(dup);
    }
    
    // Test cJSON_IsString
    (void)cJSON_IsString(item1);
    (void)cJSON_IsString(item2);
    (void)cJSON_IsString(NULL);
    
    // Test cJSON_Compare
    (void)cJSON_Compare(item1, item2, case_sensitive);
    (void)cJSON_Compare(item1, NULL, case_sensitive);
    (void)cJSON_Compare(NULL, item2, case_sensitive);
    (void)cJSON_Compare(NULL, NULL, case_sensitive);
}

int LLVMFuzzerTestOneInput_25(const uint8_t *data, size_t size) {
    // Create dummy file if needed (not required for these functions)
    // FILE *fp = fopen("./dummy_file", "wb");
    // if (fp) {
    //     fwrite(data, 1, size, fp);
    //     fclose(fp);
    // }
    
    // Create various cJSON items for testing
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    
    // Try to parse input as JSON for item1
    if (size > 0) {
        // Ensure null-terminated string for parsing
        char *json_str = (char *)malloc(size + 1);
        if (json_str) {
            memcpy(json_str, data, size);
            json_str[size] = '\0';
            item1 = cJSON_Parse(json_str);
            free(json_str);
        }
    }
    
    // Create item2 with different types based on input
    if (size > 0) {
        uint8_t type_selector = (size > 1) ? data[1] : data[0];
        
        switch (type_selector % 6) {
            case 0:
                item2 = cJSON_CreateNull();
                break;
            case 1:
                item2 = cJSON_CreateTrue();
                break;
            case 2:
                item2 = cJSON_CreateFalse();
                break;
            case 3:
                item2 = cJSON_CreateNumber(42.5);
                break;
            case 4:
                item2 = cJSON_CreateString("test");
                break;
            case 5:
                item2 = cJSON_CreateArray();
                if (item2 && size > 2) {
                    for (size_t i = 0; i < size % 10; i++) {
                        cJSON_AddItemToArray(item2, cJSON_CreateNumber(i));
                    }
                }
                break;
        }
    } else {
        item2 = cJSON_CreateNull();
    }
    
    // Test all target functions
    test_functions(item1, item2, data, size);
    
    // Cleanup
    if (item1) cJSON_Delete(item1);
    if (item2) cJSON_Delete(item2);
    
    return 0;
}