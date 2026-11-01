// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

/* Helper to create a cJSON number item from a double */
static cJSON *create_number_item(double val) {
    return cJSON_CreateNumber(val);
}

/* Helper to create a cJSON string item from a C string */
static cJSON *create_string_item(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    return cJSON_CreateString(str);
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* Use input data to drive various test scenarios */
    uint8_t mode = Data[0] % 6;
    int count = (Size > 1) ? (Data[1] % 100) : 5;
    int insert_index = (Size > 2) ? (int)(Data[2] % 200) - 50 : 0; /* Can be negative */

    cJSON *array = NULL;
    cJSON *item = NULL;
    cJSON *result = NULL;

    switch (mode) {
        case 0: {
            /* Test cJSON_CreateArray and cJSON_InsertItemInArray */
            array = cJSON_CreateArray();
            if (array == NULL) {
                break;
            }

            /* Create a few items to insert, ensuring we don't read out of bounds */
            int max_items = (Size > 1) ? (int)(Size - 1) : 0;
            if (count > max_items) {
                count = max_items;
            }
            
            for (int i = 0; i < count && i < 10; i++) {
                double val = (double)(Data[1 + i] % 100);
                item = create_number_item(val);
                if (item == NULL) {
                    break;
                }
                cJSON_InsertItemInArray(array, insert_index, item);
            }
            break;
        }
        case 1: {
            /* Test cJSON_CreateIntArray */
            /* Ensure we have enough data for the count */
            int available = (Size > 1) ? (int)(Size - 1) : 0;
            if (count > available) {
                count = available;
            }
            if (count <= 0) {
                break;
            }

            int *int_arr = (int *)malloc(count * sizeof(int));
            if (int_arr == NULL) {
                break;
            }
            for (int i = 0; i < count; i++) {
                int_arr[i] = (int)(Data[1 + i] % 1000) - 500;
            }
            result = cJSON_CreateIntArray(int_arr, count);
            free(int_arr);
            break;
        }
        case 2: {
            /* Test cJSON_CreateDoubleArray */
            int available = (Size > 1) ? (int)(Size - 1) : 0;
            if (count > available) {
                count = available;
            }
            if (count <= 0) {
                break;
            }

            double *dbl_arr = (double *)malloc(count * sizeof(double));
            if (dbl_arr == NULL) {
                break;
            }
            for (int i = 0; i < count; i++) {
                dbl_arr[i] = (double)(Data[1 + i] % 100) / 10.0;
            }
            result = cJSON_CreateDoubleArray(dbl_arr, count);
            free(dbl_arr);
            break;
        }
        case 3: {
            /* Test cJSON_CreateFloatArray */
            int available = (Size > 1) ? (int)(Size - 1) : 0;
            if (count > available) {
                count = available;
            }
            if (count <= 0) {
                break;
            }

            float *flt_arr = (float *)malloc(count * sizeof(float));
            if (flt_arr == NULL) {
                break;
            }
            for (int i = 0; i < count; i++) {
                flt_arr[i] = (float)(Data[1 + i] % 100) / 10.0f;
            }
            result = cJSON_CreateFloatArray(flt_arr, count);
            free(flt_arr);
            break;
        }
        case 4: {
            /* Test cJSON_CreateStringArray */
            /* Create a small fixed set of strings to avoid complex memory management from fuzz data */
            const char *strings[] = {"hello", "world", "test", "fuzz", "data"};
            int str_count = (count > 5) ? 5 : count;
            if (str_count < 0) str_count = 0;
            result = cJSON_CreateStringArray(strings, str_count);
            break;
        }
        case 5: {
            /* Test cJSON_InsertItemInArray with edge cases */
            array = cJSON_CreateArray();
            if (array == NULL) {
                break;
            }
            
            /* Add some initial items */
            for (int i = 0; i < 5; i++) {
                item = create_number_item((double)i);
                if (item) {
                    cJSON_AddItemToArray(array, item);
                }
            }
            
            /* Try inserting at various positions including negative and beyond length */
            item = create_number_item(999.0);
            if (item) {
                cJSON_InsertItemInArray(array, insert_index, item);
            }
            
            /* Try inserting NULL */
            cJSON_InsertItemInArray(array, 0, NULL);
            
            /* Try inserting into NULL array */
            cJSON_InsertItemInArray(NULL, 0, item);
            break;
        }
    }

    /* Cleanup */
    if (array != NULL) {
        cJSON_Delete(array);
    }
    if (result != NULL) {
        cJSON_Delete(result);
    }

    return 0;
}