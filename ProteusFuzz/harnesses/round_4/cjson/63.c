// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test 1: cJSON_CreateArray
    cJSON *arr1 = cJSON_CreateArray();
    if (arr1 != NULL) {
        cleanup_cjson(arr1);
    }

    // Test 2: cJSON_CreateIntArray
    // Use input data as int array if size allows
    if (Size >= sizeof(int)) {
        int count = (Size / sizeof(int));
        if (count > 1000) count = 1000; // Limit to avoid excessive allocation
        const int *int_data = (const int *)Data;
        cJSON *arr2 = cJSON_CreateIntArray(int_data, count);
        if (arr2 != NULL) {
            cleanup_cjson(arr2);
        }
    }

    // Test 3: cJSON_CreateDoubleArray
    if (Size >= sizeof(double)) {
        int count = (Size / sizeof(double));
        if (count > 1000) count = 1000;
        const double *double_data = (const double *)Data;
        cJSON *arr3 = cJSON_CreateDoubleArray(double_data, count);
        if (arr3 != NULL) {
            cleanup_cjson(arr3);
        }
    }

    // Test 4: cJSON_CreateFloatArray
    if (Size >= sizeof(float)) {
        int count = (Size / sizeof(float));
        if (count > 1000) count = 1000;
        const float *float_data = (const float *)Data;
        cJSON *arr4 = cJSON_CreateFloatArray(float_data, count);
        if (arr4 != NULL) {
            cleanup_cjson(arr4);
        }
    }

    // Test 5: cJSON_CreateStringArray
    // Simulate a string array by splitting input into null-terminated strings
    if (Size >= 2) {
        int max_strings = 100;
        const char *strings[100];
        int string_count = 0;
        size_t offset = 0;
        
        // Create simple string array from input
        while (offset < Size && string_count < max_strings) {
            size_t len = 1;
            // Find next null or limit length
            while (offset + len < Size && Data[offset + len] != '\0' && len < 50) {
                len++;
            }
            if (len > 0) {
                char *str = (char *)malloc(len + 1);
                if (str) {
                    memcpy(str, Data + offset, len);
                    str[len] = '\0';
                    strings[string_count++] = str;
                    offset += len + 1;
                } else {
                    break;
                }
            } else {
                offset++;
            }
        }

        if (string_count > 0) {
            cJSON *arr5 = cJSON_CreateStringArray(strings, string_count);
            if (arr5 != NULL) {
                cleanup_cjson(arr5);
            }
            // Free allocated strings
            for (int i = 0; i < string_count; i++) {
                free((void *)strings[i]);
            }
        }
    }

    // Test 6: cJSON_InsertItemInArray
    cJSON *base_arr = cJSON_CreateArray();
    if (base_arr != NULL) {
        cJSON *item1 = cJSON_CreateNumber(1.0);
        cJSON *item2 = cJSON_CreateNumber(2.0);
        cJSON *item3 = cJSON_CreateString("test");
        
        // Insert at various positions
        // Note: cJSON_InsertItemInArray takes ownership of the item if successful.
        // We must NOT delete items that were successfully inserted into the array.
        // We only delete items if the insertion failed or if we are testing failure cases.
        
        if (item1) {
            if (!cJSON_InsertItemInArray(base_arr, 0, item1)) {
                // Insertion failed, item1 is still owned by us
                cleanup_cjson(item1);
            }
        }
        
        if (item2) {
            if (!cJSON_InsertItemInArray(base_arr, 1, item2)) {
                cleanup_cjson(item2);
            }
        }
        
        if (item3) {
            if (!cJSON_InsertItemInArray(base_arr, 2, item3)) {
                cleanup_cjson(item3);
            }
        }
        
        // Try inserting beyond bounds (should append)
        // Create a new item for this test
        cJSON *item4 = cJSON_CreateNumber(4.0);
        if (item4) {
            if (!cJSON_InsertItemInArray(base_arr, 100, item4)) {
                cleanup_cjson(item4);
            }
        }
        
        // Try invalid insertions
        // 1. Negative index
        cJSON *item5 = cJSON_CreateNumber(5.0);
        if (item5) {
            if (!cJSON_InsertItemInArray(base_arr, -1, item5)) {
                cleanup_cjson(item5);
            }
        }
        
        // 2. NULL item
        // cJSON_InsertItemInArray handles NULL item internally, returns false.
        // No cleanup needed for NULL.
        cJSON_InsertItemInArray(base_arr, 0, NULL);
        
        // Clean up the base array, which will delete all children
        cleanup_cjson(base_arr);
    }

    return 0;
}