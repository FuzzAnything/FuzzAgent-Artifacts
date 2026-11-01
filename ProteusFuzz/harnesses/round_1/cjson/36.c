// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <float.h>
#include <math.h>
#include <limits.h>

static void cleanup_json(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const double *numbers = NULL;
    int count = 0;
    double num_val = 0.0;
    char *key_name = NULL;
    cJSON *obj = NULL;
    cJSON *arr = NULL;
    cJSON *item = NULL;
    cJSON_bool is_num;
    double retrieved_val;

    // Extract count from first byte, ensure it's reasonable
    count = (int)Data[0];
    if (count < 0) count = 0;
    if (count > 1000) count = 1000; // Limit array size

    // Extract number value from next 8 bytes if available
    if (Size >= 9) {
        memcpy(&num_val, Data + 1, sizeof(double));
    } else {
        num_val = (double)Data[0];
    }

    // Create a double array if we have enough data
    if (Size >= 1 + (size_t)count * sizeof(double)) {
        numbers = (const double *)(Data + 1);
    } else if (count > 0) {
        // Fallback: create a small array from available data
        int available = (Size - 1) / sizeof(double);
        if (available > 0) {
            double *temp = (double *)malloc(available * sizeof(double));
            if (temp) {
                memcpy(temp, Data + 1, available * sizeof(double));
                numbers = temp;
                count = available;
            }
        }
    }

    // Test cJSON_CreateDoubleArray
    arr = cJSON_CreateDoubleArray(numbers, count);
    if (arr != NULL) {
        // Verify array contents
        cJSON *child = arr->child;
        int i = 0;
        while (child != NULL && i < count) {
            if (cJSON_IsNumber(child)) {
                double val = cJSON_GetNumberValue(child);
                // Just access to ensure no crash
                (void)val;
            }
            child = child->next;
            i++;
        }
        cleanup_json(arr);
    }

    // Test cJSON_CreateNumber
    item = cJSON_CreateNumber(num_val);
    if (item != NULL) {
        is_num = cJSON_IsNumber(item);
        retrieved_val = cJSON_GetNumberValue(item);
        // Test setting number
        cJSON_SetNumberHelper(item, num_val * 2.0);
        cleanup_json(item);
    }

    // Test cJSON_AddNumberToObject
    obj = cJSON_CreateObject();
    if (obj != NULL) {
        // Generate a key name from input data
        int key_len = (Size > 2) ? Data[2] : 5;
        if (key_len > 100) key_len = 100;
        // Ensure we don't read past the buffer
        if (key_len > (int)(Size - 3)) {
            key_len = (int)(Size - 3);
        }
        if (key_len < 0) key_len = 0;
        
        // Allocate enough space for the key plus null terminator
        key_name = (char *)malloc(key_len + 1);
        if (key_name != NULL) {
            if (key_len > 0) {
                memcpy(key_name, Data + 3, key_len);
                key_name[key_len] = '\0';
            } else {
                // Ensure we have enough space for "test_key" (8 chars + null)
                if (key_len + 1 < 9) {
                    // Reallocate if the default key is larger than allocated space
                    char *new_key = (char *)realloc(key_name, 9);
                    if (new_key) {
                        key_name = new_key;
                        strcpy(key_name, "test_key");
                    } else {
                        free(key_name);
                        key_name = NULL;
                    }
                } else {
                    strcpy(key_name, "test_key");
                }
            }
            
            if (key_name != NULL) {
                item = cJSON_AddNumberToObject(obj, key_name, num_val);
                if (item != NULL) {
                    is_num = cJSON_IsNumber(item);
                    retrieved_val = cJSON_GetNumberValue(item);
                }
                
                // Try adding with same key (should fail or overwrite depending on implementation)
                item = cJSON_AddNumberToObject(obj, key_name, num_val * 10.0);
            }
            
            if (key_name != NULL) {
                free(key_name);
            }
        }
        cleanup_json(obj);
    }

    // Test cJSON_IsNumber and cJSON_GetNumberValue with NULL
    is_num = cJSON_IsNumber(NULL);
    retrieved_val = cJSON_GetNumberValue(NULL);

    // Test cJSON_SetNumberHelper with NULL
    double ret = cJSON_SetNumberHelper(NULL, num_val);
    (void)ret;

    // Test with special float values
    double special_vals[] = {0.0, -0.0, DBL_MAX, -DBL_MAX, DBL_MIN, -DBL_MIN, INFINITY, -INFINITY, NAN};
    for (int i = 0; i < 9; i++) {
        item = cJSON_CreateNumber(special_vals[i]);
        if (item != NULL) {
            is_num = cJSON_IsNumber(item);
            retrieved_val = cJSON_GetNumberValue(item);
            cleanup_json(item);
        }
    }

    // Test with integer boundary values
    double int_boundary_vals[] = {(double)INT_MAX, (double)INT_MIN, (double)INT_MAX + 1.0, (double)INT_MIN - 1.0};
    for (int i = 0; i < 4; i++) {
        item = cJSON_CreateNumber(int_boundary_vals[i]);
        if (item != NULL) {
            is_num = cJSON_IsNumber(item);
            retrieved_val = cJSON_GetNumberValue(item);
            cleanup_json(item);
        }
    }

    // Test cJSON_CreateDoubleArray with NULL pointer
    arr = cJSON_CreateDoubleArray(NULL, 0);
    if (arr != NULL) cleanup_json(arr);
    
    arr = cJSON_CreateDoubleArray(NULL, 5);
    if (arr != NULL) cleanup_json(arr);

    // Test cJSON_CreateDoubleArray with negative count
    arr = cJSON_CreateDoubleArray((double[]){1.0, 2.0}, -1);
    if (arr != NULL) cleanup_json(arr);

    return 0;
}