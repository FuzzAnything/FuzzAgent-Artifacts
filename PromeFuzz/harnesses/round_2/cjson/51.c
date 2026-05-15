// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_GetArrayItem at cJSON.c:1941:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
// cJSON_GetArraySize at cJSON.c:1899:19 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static int read_int(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) return 0;
    int val;
    memcpy(&val, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    return val;
}

static double read_double(const uint8_t **data, size_t *size) {
    if (*size < sizeof(double)) return 0.0;
    double val;
    memcpy(&val, *data, sizeof(double));
    *data += sizeof(double);
    *size -= sizeof(double);
    return val;
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    const uint8_t *ptr = Data;
    size_t remaining = Size;

    // Read counts for arrays
    int int_count = read_int(&ptr, &remaining);
    int double_count = read_int(&ptr, &remaining);
    
    // Limit counts to reasonable values to avoid excessive memory usage
    if (int_count < 0) int_count = 0;
    if (int_count > 100) int_count = 100;
    if (double_count < 0) double_count = 0;
    if (double_count > 100) double_count = 100;

    // Allocate and populate integer array
    int *int_array = NULL;
    if (int_count > 0) {
        int_array = (int*)malloc(int_count * sizeof(int));
        if (int_array) {
            for (int i = 0; i < int_count && remaining >= sizeof(int); i++) {
                int_array[i] = read_int(&ptr, &remaining);
            }
        }
    }

    // Allocate and populate double array
    double *double_array = NULL;
    if (double_count > 0) {
        double_array = (double*)malloc(double_count * sizeof(double));
        if (double_array) {
            for (int i = 0; i < double_count && remaining >= sizeof(double); i++) {
                double_array[i] = read_double(&ptr, &remaining);
            }
        }
    }

    // Test cJSON_CreateIntArray
    cJSON *int_json_array = NULL;
    if (int_array) {
        int_json_array = cJSON_CreateIntArray(int_array, int_count);
    }

    // Test cJSON_CreateDoubleArray
    cJSON *double_json_array = NULL;
    if (double_array) {
        double_json_array = cJSON_CreateDoubleArray(double_array, double_count);
    }

    // Test cJSON_CreateArray
    cJSON *empty_array = cJSON_CreateArray();

    // Test cJSON_GetArraySize on all arrays (including NULL)
    if (int_json_array) {
        int int_array_size = cJSON_GetArraySize(int_json_array);
        (void)int_array_size;
    }
    if (double_json_array) {
        int double_array_size = cJSON_GetArraySize(double_json_array);
        (void)double_array_size;
    }
    if (empty_array) {
        int empty_array_size = cJSON_GetArraySize(empty_array);
        (void)empty_array_size;
    }
    // Test with NULL
    cJSON_GetArraySize(NULL);

    // Test cJSON_GetArrayItem on all arrays
    if (int_json_array) {
        for (int i = -2; i < int_count + 2; i++) {
            cJSON *item = cJSON_GetArrayItem(int_json_array, i);
            (void)item;
        }
    }
    if (double_json_array) {
        for (int i = -2; i < double_count + 2; i++) {
            cJSON *item = cJSON_GetArrayItem(double_json_array, i);
            (void)item;
        }
    }
    if (empty_array) {
        for (int i = -2; i < 5; i++) {
            cJSON *item = cJSON_GetArrayItem(empty_array, i);
            (void)item;
        }
    }
    // Test with NULL array
    cJSON_GetArrayItem(NULL, 0);

    // Test cJSON_InsertItemInArray
    if (empty_array && int_json_array) {
        // Insert items from int array into empty array
        for (int i = 0; i < int_count && i < 10; i++) {
            cJSON *new_item = cJSON_CreateNumber(int_array[i]);
            if (new_item) {
                // Generate insertion index from remaining data
                int insert_index = 0;
                if (remaining >= sizeof(int)) {
                    insert_index = read_int(&ptr, &remaining);
                }
                cJSON_InsertItemInArray(empty_array, insert_index, new_item);
            }
        }
    }

    // Test cJSON_InsertItemInArray with NULL array
    if (int_json_array) {
        cJSON *dummy_item = cJSON_CreateNumber(42);
        if (dummy_item) {
            cJSON_InsertItemInArray(NULL, 0, dummy_item);
            cJSON_Delete(dummy_item);
        }
    }

    // Test cJSON_InsertItemInArray with NULL item
    if (empty_array) {
        cJSON_InsertItemInArray(empty_array, 0, NULL);
    }

    // Cleanup
    if (int_json_array) cJSON_Delete(int_json_array);
    if (double_json_array) cJSON_Delete(double_json_array);
    if (empty_array) cJSON_Delete(empty_array);
    free(int_array);
    free(double_array);

    return 0;
}