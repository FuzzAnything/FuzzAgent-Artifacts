// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
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

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test 1: cJSON_CreateArray
    cJSON *array1 = cJSON_CreateArray();
    if (array1 != NULL) {
        cJSON *item = cJSON_CreateNumber(123);
        if (item != NULL) {
            cJSON_AddItemToArray(array1, item);
        }
        cleanup_cjson(array1);
    }

    // Test 2: cJSON_CreateIntArray
    if (Size >= 4) {
        int count = Data[0] % 100; 
        if (count > 0 && Size >= (size_t)(count * sizeof(int) + 1)) {
            int *int_arr = (int *)malloc(count * sizeof(int));
            if (int_arr != NULL) {
                for (int i = 0; i < count; i++) {
                    int_arr[i] = Data[i + 1];
                }
                cJSON *array2 = cJSON_CreateIntArray(int_arr, count);
                cleanup_cjson(array2);
                free(int_arr);
            }
        }
    }

    // Test 3: cJSON_CreateDoubleArray
    if (Size >= 8) {
        int count = Data[0] % 50;
        if (count > 0 && Size >= (size_t)(count * sizeof(double) + 1)) {
            double *double_arr = (double *)malloc(count * sizeof(double));
            if (double_arr != NULL) {
                for (int i = 0; i < count; i++) {
                    double_arr[i] = (double)Data[i + 1];
                }
                cJSON *array3 = cJSON_CreateDoubleArray(double_arr, count);
                cleanup_cjson(array3);
                free(double_arr);
            }
        }
    }

    // Test 4: cJSON_CreateFloatArray
    if (Size >= 4) {
        int count = Data[0] % 50;
        if (count > 0 && Size >= (size_t)(count * sizeof(float) + 1)) {
            float *float_arr = (float *)malloc(count * sizeof(float));
            if (float_arr != NULL) {
                for (int i = 0; i < count; i++) {
                    float_arr[i] = (float)Data[i + 1];
                }
                cJSON *array4 = cJSON_CreateFloatArray(float_arr, count);
                cleanup_cjson(array4);
                free(float_arr);
            }
        }
    }

    // Test 5: cJSON_CreateStringArray
    // We need to ensure strings are null-terminated and valid pointers
    if (Size >= 2) {
        int count = Data[0] % 20;
        if (count > 0) {
            size_t remaining_size = Size - 1;
            if (remaining_size == 0) {
                return 0;
            }
            
            char **string_arr = (char **)malloc(count * sizeof(char *));
            if (string_arr != NULL) {
                size_t total_used = 0;
                int valid_strings = 0;
                
                for (int i = 0; i < count; i++) {
                    // Calculate length for this string
                    size_t len = (remaining_size - total_used) / (count - i);
                    if (len == 0) len = 1;
                    
                    if (total_used + len > remaining_size) {
                        len = remaining_size - total_used;
                        if (len == 0) break;
                    }

                    string_arr[i] = (char *)malloc(len + 1);
                    if (string_arr[i] != NULL) {
                        memcpy(string_arr[i], Data + 1 + total_used, len);
                        string_arr[i][len] = '\0'; // Ensure null termination
                        total_used += len;
                        valid_strings++;
                    } else {
                        // Allocation failed, clean up and stop
                        for (int j = 0; j < valid_strings; j++) {
                            free(string_arr[j]);
                        }
                        free(string_arr);
                        return 0;
                    }
                }
                
                if (valid_strings > 0) {
                    cJSON *array5 = cJSON_CreateStringArray((const char *const *)string_arr, valid_strings);
                    cleanup_cjson(array5);
                }
                
                for (int i = 0; i < valid_strings; i++) {
                    free(string_arr[i]);
                }
                free(string_arr);
            }
        }
    }

    // Test 6: cJSON_InsertItemInArray
    if (Size >= 2) {
        cJSON *array6 = cJSON_CreateArray();
        if (array6 != NULL) {
            int index = Data[0] % 10;
            cJSON *new_item = cJSON_CreateNumber(Data[1]);
            if (new_item != NULL) {
                cJSON_InsertItemInArray(array6, index, new_item);
            }
            cleanup_cjson(array6);
        }
    }

    return 0;
}