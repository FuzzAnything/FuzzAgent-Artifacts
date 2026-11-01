// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "cJSON.h"

static void cleanup_json_tree(cJSON *root) {
    if (root != NULL) {
        cJSON_Delete(root);
    }
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use the first byte to select the test case
    int test_case = Data[0] % 6;

    switch (test_case) {
        case 0: {
            // Test cJSON_CreateArray
            cJSON *arr = cJSON_CreateArray();
            if (arr != NULL) {
                // Optionally add some items to exercise the array further
                cJSON *item = cJSON_CreateNumber(123);
                if (item != NULL) {
                    cJSON_AddItemToArray(arr, item);
                }
                cleanup_json_tree(arr);
            }
            break;
        }
        case 1: {
            // Test cJSON_CreateDoubleArray
            if (Size < 2) break;
            int count = (Data[1] % 100) + 1;
            // Ensure we have enough data for the array of doubles
            if (Size < (size_t)(2 + count * sizeof(double))) break;

            double *doubles = (double *)(Data + 2);
            cJSON *arr = cJSON_CreateDoubleArray(doubles, count);
            cleanup_json_tree(arr);
            break;
        }
        case 2: {
            // Test cJSON_CreateFloatArray
            if (Size < 2) break;
            int count = (Data[1] % 100) + 1;
            // Ensure we have enough data for the array of floats
            if (Size < (size_t)(2 + count * sizeof(float))) break;

            float *floats = (float *)(Data + 2);
            cJSON *arr = cJSON_CreateFloatArray(floats, count);
            cleanup_json_tree(arr);
            break;
        }
        case 3: {
            // Test cJSON_InsertItemInArray
            if (Size < 4) break; // Need at least 4 bytes: test_case, index, val1, val2
            
            cJSON *arr = cJSON_CreateArray();
            if (arr == NULL) break;

            int insert_index = (int)(Data[1] % 10);
            int value = (int)(Data[2] * 256 + (Data[3] % 256));
            cJSON *new_item = cJSON_CreateNumber(value);
            
            if (new_item != NULL) {
                cJSON_InsertItemInArray(arr, insert_index, new_item);
            }
            
            cleanup_json_tree(arr);
            break;
        }
        case 4: {
            // Test cJSON_CreateStringArray
            if (Size < 2) break;
            int count = (Data[1] % 20) + 1;
            
            // We need to construct an array of string pointers from the input data
            // Since input is raw bytes, we will create temporary null-terminated strings
            char **strings = (char **)malloc(count * sizeof(char *));
            if (strings == NULL) break;

            // Initialize all pointers to NULL to prevent double-free or free(NULL) issues
            for (int i = 0; i < count; i++) {
                strings[i] = NULL;
            }

            size_t offset = 2;
            int valid_count = 0;
            
            for (int i = 0; i < count; i++) {
                if (offset >= Size) break;
                
                // Determine length from data, ensuring it's valid and within bounds
                int len = Data[offset] % 10 + 1;
                
                // Check if we have enough bytes for the string content
                if (offset + 1 + len > Size) {
                    // Not enough data, stop here
                    break;
                }
                
                strings[i] = (char *)malloc(len + 1);
                if (strings[i] == NULL) {
                    // Cleanup partial allocation
                    for (int j = 0; j < i; j++) {
                        if (strings[j] != NULL) {
                            free(strings[j]);
                            strings[j] = NULL;
                        }
                    }
                    free(strings);
                    strings = NULL;
                    break;
                }
                
                // Copy data and ensure null termination
                memcpy(strings[i], Data + offset + 1, len);
                strings[i][len] = '\0';
                
                offset += len + 1;
                valid_count = i + 1;
            }

            if (valid_count > 0 && strings != NULL) {
                cJSON *arr = cJSON_CreateStringArray((const char *const *)strings, valid_count);
                cleanup_json_tree(arr);
            }
            
            // Cleanup strings
            if (strings != NULL) {
                for (int i = 0; i < count; i++) {
                    if (strings[i] != NULL) {
                        free(strings[i]);
                        strings[i] = NULL;
                    }
                }
                free(strings);
            }
            break;
        }
        case 5: {
            // Test cJSON_CreateIntArray
            if (Size < 2) break;
            int count = (Data[1] % 100) + 1;
            // Ensure we have enough data for the array of ints
            if (Size < (size_t)(2 + count * sizeof(int))) break;

            int *ints = (int *)(Data + 2);
            cJSON *arr = cJSON_CreateIntArray(ints, count);
            cleanup_json_tree(arr);
            break;
        }
        default:
            break;
    }

    return 0;
}