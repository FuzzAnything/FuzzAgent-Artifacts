// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_IsArray at cJSON.c:3042:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_IsBool at cJSON.c:3003:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *item = NULL;
    cJSON *item2 = NULL;
    cJSON_bool case_sensitive = (Data[0] % 2);

    // Ensure the input data is null-terminated for cJSON_Parse
    // We create a copy to safely add the null terminator
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Attempt to parse the input data as JSON
    item = cJSON_Parse(input_str);
    
    // Free the temporary buffer
    free(input_str);
    
    // If parsing fails, test type checks on NULL or invalid inputs
    if (!item) {
        // Test with NULL
        cJSON_IsArray(NULL);
        cJSON_IsTrue(NULL);
        cJSON_IsNumber(NULL);
        cJSON_IsBool(NULL);
        cJSON_IsString(NULL);
        cJSON_Compare(NULL, NULL, case_sensitive);
        
        // Create a simple valid item to test against NULL
        cJSON *temp = cJSON_CreateNull();
        if (temp) {
            cJSON_IsArray(temp);
            cJSON_IsTrue(temp);
            cJSON_IsNumber(temp);
            cJSON_IsBool(temp);
            cJSON_IsString(temp);
            cJSON_Compare(temp, NULL, case_sensitive);
            cJSON_Compare(NULL, temp, case_sensitive);
            cleanup_cjson(temp);
        }
        return 0;
    }

    // Test type checking functions on the parsed item
    cJSON_IsArray(item);
    cJSON_IsTrue(item);
    cJSON_IsNumber(item);
    cJSON_IsBool(item);
    cJSON_IsString(item);

    // Create a duplicate for comparison tests
    item2 = cJSON_Duplicate(item, 1);
    
    if (item2) {
        // Test comparison with itself
        cJSON_Compare(item, item, case_sensitive);
        
        // Test comparison with duplicate
        cJSON_Compare(item, item2, case_sensitive);
        cJSON_Compare(item2, item, case_sensitive);
        
        // Test comparison with NULL
        cJSON_Compare(item, NULL, case_sensitive);
        cJSON_Compare(NULL, item, case_sensitive);
        
        // Test comparison with NULLs
        cJSON_Compare(NULL, NULL, case_sensitive);
        
        // Cleanup duplicate
        cJSON_Delete(item2);
    }

    // Cleanup original item
    cleanup_cjson(item);

    return 0;
}