// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
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

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test cJSON_CreateArray
    cJSON *arr = cJSON_CreateArray();
    if (arr != NULL) {
        cleanup_cjson(arr);
    }

    // Test cJSON_CreateIntArray
    if (Size >= 2) {
        int count = (Data[0] % 10) + 1;
        // Ensure we don't read past the buffer
        if (count > (int)(Size - 1)) {
            count = (int)(Size - 1);
        }
        if (count > 0) {
            int *int_nums = (int *)malloc(count * sizeof(int));
            if (int_nums != NULL) {
                for (int i = 0; i < count; i++) {
                    int_nums[i] = Data[1 + i] * 100 + (i % 10);
                }
                cJSON *int_arr = cJSON_CreateIntArray(int_nums, count);
                cleanup_cjson(int_arr);
                free(int_nums);
            }
        }
    }

    // Test cJSON_CreateDoubleArray
    if (Size >= 2) {
        int count = (Data[0] % 10) + 1;
        if (count > (int)(Size - 1)) {
            count = (int)(Size - 1);
        }
        if (count > 0) {
            double *dbl_nums = (double *)malloc(count * sizeof(double));
            if (dbl_nums != NULL) {
                for (int i = 0; i < count; i++) {
                    dbl_nums[i] = (double)Data[1 + i] + (i * 0.5);
                }
                cJSON *dbl_arr = cJSON_CreateDoubleArray(dbl_nums, count);
                cleanup_cjson(dbl_arr);
                free(dbl_nums);
            }
        }
    }

    // Test cJSON_CreateFloatArray
    if (Size >= 2) {
        int count = (Data[0] % 10) + 1;
        if (count > (int)(Size - 1)) {
            count = (int)(Size - 1);
        }
        if (count > 0) {
            float *flt_nums = (float *)malloc(count * sizeof(float));
            if (flt_nums != NULL) {
                for (int i = 0; i < count; i++) {
                    flt_nums[i] = (float)Data[1 + i] + (i * 0.1f);
                }
                cJSON *flt_arr = cJSON_CreateFloatArray(flt_nums, count);
                cleanup_cjson(flt_arr);
                free(flt_nums);
            }
        }
    }

    // Test cJSON_CreateStringArray
    if (Size >= 2) {
        int count = (Data[0] % 10) + 1;
        if (count > (int)(Size - 1)) {
            count = (int)(Size - 1);
        }
        if (count > 0) {
            // Create a safe array of strings using the input data
            // We need to ensure strings are null-terminated for cJSON
            // Since Data might not contain null terminators, we create a temporary buffer
            // or use a fixed length string approach. Here we just pass pointers to Data
            // but limit length to avoid reading past end.
            // Note: cJSON_CreateStringArray expects null-terminated strings.
            // To be safe, we will not use raw pointers to Data unless we know they are terminated.
            // Instead, we create a small array of valid C strings for testing.
            const char *dummy_strings[] = {"str1", "str2", "str3", "str4", "str5", 
                                           "str6", "str7", "str8", "str9", "str10"};
            
            cJSON *str_arr = cJSON_CreateStringArray(dummy_strings, count);
            cleanup_cjson(str_arr);
        }
    }

    // Test cJSON_InsertItemInArray
    if (Size >= 3) {
        cJSON *arr2 = cJSON_CreateArray();
        if (arr2 != NULL) {
            int which = (Data[0] % 5) - 2; // -2 to 2
            cJSON *item1 = cJSON_CreateNumber(123);
            cJSON *item2 = cJSON_CreateNumber(456);

            if (item1 != NULL && item2 != NULL) {
                cJSON_InsertItemInArray(arr2, which, item1);
                cJSON_InsertItemInArray(arr2, which + 1, item2);
            } else {
                cleanup_cjson(item1);
                cleanup_cjson(item2);
            }
            cleanup_cjson(arr2);
        }
    }

    return 0;
}