// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

static void cleanup_json(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test 1: cJSON_CreateArray
    cJSON *array1 = cJSON_CreateArray();
    if (array1 != NULL) {
        // Try to add a dummy item to ensure it's a valid array
        cJSON *dummy = cJSON_CreateNumber(0);
        if (dummy != NULL) {
            cJSON_AddItemToArray(array1, dummy);
        }
        cleanup_json(array1);
    }

    // Test 2: cJSON_CreateIntArray
    if (Size >= 2) {
        int count = Data[0] % 100; // Limit count to avoid huge allocations
        int *int_arr = (int *)malloc(count * sizeof(int));
        if (int_arr != NULL) {
            for (int i = 0; i < count; i++) {
                int_arr[i] = (i < Size - 1) ? Data[i + 1] : 0;
            }
            cJSON *array2 = cJSON_CreateIntArray(int_arr, count);
            cleanup_json(array2);
            free(int_arr);
        }
    }

    // Test 3: cJSON_CreateDoubleArray
    if (Size >= 2) {
        int count = Data[0] % 100;
        double *double_arr = (double *)malloc(count * sizeof(double));
        if (double_arr != NULL) {
            for (int i = 0; i < count; i++) {
                double_arr[i] = (i < Size - 1) ? (double)Data[i + 1] : 0.0;
            }
            cJSON *array3 = cJSON_CreateDoubleArray(double_arr, count);
            cleanup_json(array3);
            free(double_arr);
        }
    }

    // Test 4: cJSON_CreateFloatArray
    if (Size >= 2) {
        int count = Data[0] % 100;
        float *float_arr = (float *)malloc(count * sizeof(float));
        if (float_arr != NULL) {
            for (int i = 0; i < count; i++) {
                float_arr[i] = (i < Size - 1) ? (float)Data[i + 1] : 0.0f;
            }
            cJSON *array4 = cJSON_CreateFloatArray(float_arr, count);
            cleanup_json(array4);
            free(float_arr);
        }
    }

    // Test 5: cJSON_CreateStringArray
    if (Size >= 2) {
        int count = Data[0] % 50; // Limit string count
        char **str_arr = (char **)malloc(count * sizeof(char *));
        if (str_arr != NULL) {
            for (int i = 0; i < count; i++) {
                int len = (i < Size - 1) ? (Data[i + 1] % 20) + 1 : 1;
                str_arr[i] = (char *)malloc(len + 1);
                if (str_arr[i] != NULL) {
                    for (int j = 0; j < len; j++) {
                        str_arr[i][j] = (char)Data[(i + 1) % Size];
                    }
                    str_arr[i][len] = '\0';
                } else {
                    // Cleanup partial allocation
                    for (int k = 0; k < i; k++) {
                        free(str_arr[k]);
                    }
                    free(str_arr);
                    str_arr = NULL;
                    break;
                }
            }
            if (str_arr != NULL) {
                cJSON *array5 = cJSON_CreateStringArray((const char *const *)str_arr, count);
                cleanup_json(array5);
                for (int i = 0; i < count; i++) {
                    free(str_arr[i]);
                }
                free(str_arr);
            }
        }
    }

    // Test 6: cJSON_InsertItemInArray
    if (Size >= 3) {
        cJSON *array6 = cJSON_CreateArray();
        if (array6 != NULL) {
            // Create some items to insert
            cJSON *item1 = cJSON_CreateNumber(10);
            cJSON *item2 = cJSON_CreateNumber(20);
            cJSON *item3 = cJSON_CreateNumber(30);

            // Add initial items
            if (item1) cJSON_AddItemToArray(array6, item1);
            if (item2) cJSON_AddItemToArray(array6, item2);

            // Try inserting at various positions
            int pos = Data[2] % 10;
            if (item3) {
                cJSON_InsertItemInArray(array6, pos, item3);
            }

            // Try invalid insertions
            cJSON *null_item = NULL;
            cJSON_InsertItemInArray(array6, -1, null_item);
            cJSON_InsertItemInArray(NULL, 0, item1);

            cleanup_json(array6);
        }
    }

    return 0;
}