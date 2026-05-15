// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsString at cJSON.c:3032:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsRaw at cJSON.c:3062:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_IsTrue at cJSON.c:2992:26 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Duplicate at cJSON.c:2784:23 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Compare at cJSON.c:3072:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateFalse at cJSON.c:2483:23 in cJSON.h
// cJSON_CreateTrue at cJSON.c:2472:23 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
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

static void test_functions(cJSON *item1, cJSON *item2, cJSON_bool case_sensitive) {
    // Test cJSON_IsString
    cJSON_IsString(item1);
    cJSON_IsString(item2);
    cJSON_IsString(NULL);
    
    // Test cJSON_IsRaw
    cJSON_IsRaw(item1);
    cJSON_IsRaw(item2);
    cJSON_IsRaw(NULL);
    
    // Test cJSON_IsTrue
    cJSON_IsTrue(item1);
    cJSON_IsTrue(item2);
    cJSON_IsTrue(NULL);
    
    // Test cJSON_Duplicate
    cJSON *dup1 = cJSON_Duplicate(item1, case_sensitive & 1);
    cJSON *dup2 = cJSON_Duplicate(item2, (case_sensitive >> 1) & 1);
    cJSON_Duplicate(NULL, 0);
    
    // Test cJSON_Compare
    cJSON_Compare(item1, item2, case_sensitive);
    cJSON_Compare(item1, dup1, case_sensitive);
    cJSON_Compare(item2, dup2, case_sensitive);
    cJSON_Compare(NULL, NULL, case_sensitive);
    cJSON_Compare(item1, NULL, case_sensitive);
    cJSON_Compare(NULL, item2, case_sensitive);
    
    // Clean up duplicates
    if (dup1) cJSON_Delete(dup1);
    if (dup2) cJSON_Delete(dup2);
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    // Create dummy file if needed (not required for these functions)
    // FILE *fp = fopen("./dummy_file", "wb");
    // if (fp) {
    //     fwrite(Data, 1, Size, fp);
    //     fclose(fp);
    // }
    
    // Use first byte to determine case sensitivity for comparisons
    cJSON_bool case_sensitive = Size > 0 ? Data[0] : 0;
    
    // Create various cJSON objects to test
    cJSON *false_item = cJSON_CreateFalse();
    if (!false_item) return 0;
    
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *number_item = cJSON_CreateNumber(42.5);
    cJSON *string_item = cJSON_CreateString("test");
    
    // Create raw JSON string if we have enough data
    cJSON *raw_item = NULL;
    if (Size > 1) {
        char *raw_string = (char*)malloc(Size);
        if (raw_string) {
            memcpy(raw_string, Data, Size);
            raw_item = cJSON_CreateRaw(raw_string);
            free(raw_string);
        }
    }
    
    // Try to parse input as JSON
    cJSON *parsed_item = NULL;
    if (Size > 0) {
        char *json_string = (char*)malloc(Size + 1);
        if (json_string) {
            memcpy(json_string, Data, Size);
            json_string[Size] = '\0';
            parsed_item = cJSON_Parse(json_string);
            free(json_string);
        }
    }
    
    // Test functions with various combinations
    test_functions(false_item, true_item, case_sensitive);
    test_functions(false_item, null_item, case_sensitive);
    test_functions(false_item, number_item, case_sensitive);
    test_functions(false_item, string_item, case_sensitive);
    test_functions(false_item, raw_item, case_sensitive);
    test_functions(false_item, parsed_item, case_sensitive);
    
    // Clean up
    cJSON_Delete(false_item);
    if (true_item) cJSON_Delete(true_item);
    if (null_item) cJSON_Delete(null_item);
    if (number_item) cJSON_Delete(number_item);
    if (string_item) cJSON_Delete(string_item);
    if (raw_item) cJSON_Delete(raw_item);
    if (parsed_item) cJSON_Delete(parsed_item);
    
    return 0;
}