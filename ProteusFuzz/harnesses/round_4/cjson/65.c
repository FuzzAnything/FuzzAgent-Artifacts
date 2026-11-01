// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
// cJSON_CreateDoubleArray at cJSON.c:2701:23 in cJSON.h
// cJSON_CreateFloatArray at cJSON.c:2661:23 in cJSON.h
// cJSON_CreateStringArray at cJSON.c:2741:23 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_InsertItemInArray at cJSON.c:2334:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateIntArray at cJSON.c:2621:23 in cJSON.h
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

/* Helper to safely delete cJSON structures */
static void safe_delete(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

/* Helper to generate random integers from input data */
static int get_int_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 4 > size) {
        return 0;
    }
    int val = *(const int*)(data + *offset);
    *offset += 4;
    return val;
}

/* Helper to generate random doubles from input data */
static double get_double_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 8 > size) {
        return 0.0;
    }
    double val = *(const double*)(data + *offset);
    *offset += 8;
    return val;
}

/* Helper to generate random floats from input data */
static float get_float_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 4 > size) {
        return 0.0f;
    }
    float val = *(const float*)(data + *offset);
    *offset += 4;
    return val;
}

/* Helper to get a string from data (null-terminated or fixed length) */
static char *get_string_from_data(const uint8_t *data, size_t size, size_t *offset, size_t max_len) {
    if (*offset >= size) {
        return NULL;
    }
    size_t remaining = size - *offset;
    if (remaining > max_len) {
        remaining = max_len;
    }
    char *str = (char *)malloc(remaining + 1);
    if (!str) return NULL;
    memcpy(str, data + *offset, remaining);
    str[remaining] = '\0';
    *offset += remaining;
    return str;
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    size_t offset = 0;
    cJSON *array = NULL;
    cJSON *item = NULL;
    cJSON *target_array = NULL;
    cJSON_bool result;
    int count;
    int which;

    /* Test 1: cJSON_CreateArray */
    array = cJSON_CreateArray();
    safe_delete(array);

    /* Test 2: cJSON_CreateIntArray */
    if (offset + 4 <= Size) {
        count = get_int_from_data(Data, Size, &offset);
        if (count > 0 && count < 1000) {
            int *int_buf = (int *)malloc(count * sizeof(int));
            if (int_buf) {
                for (int i = 0; i < count; i++) {
                    int_buf[i] = get_int_from_data(Data, Size, &offset);
                }
                array = cJSON_CreateIntArray(int_buf, count);
                safe_delete(array);
                free(int_buf);
            }
        }
    }

    /* Test 3: cJSON_CreateDoubleArray */
    if (offset + 4 <= Size) {
        count = get_int_from_data(Data, Size, &offset);
        if (count > 0 && count < 1000) {
            double *double_buf = (double *)malloc(count * sizeof(double));
            if (double_buf) {
                for (int i = 0; i < count; i++) {
                    double_buf[i] = get_double_from_data(Data, Size, &offset);
                }
                array = cJSON_CreateDoubleArray(double_buf, count);
                safe_delete(array);
                free(double_buf);
            }
        }
    }

    /* Test 4: cJSON_CreateFloatArray */
    if (offset + 4 <= Size) {
        count = get_int_from_data(Data, Size, &offset);
        if (count > 0 && count < 1000) {
            float *float_buf = (float *)malloc(count * sizeof(float));
            if (float_buf) {
                for (int i = 0; i < count; i++) {
                    float_buf[i] = get_float_from_data(Data, Size, &offset);
                }
                array = cJSON_CreateFloatArray(float_buf, count);
                safe_delete(array);
                free(float_buf);
            }
        }
    }

    /* Test 5: cJSON_CreateStringArray */
    if (offset + 4 <= Size) {
        count = get_int_from_data(Data, Size, &offset);
        if (count > 0 && count < 100) {
            char **str_buf = (char **)malloc(count * sizeof(char *));
            if (str_buf) {
                for (int i = 0; i < count; i++) {
                    str_buf[i] = get_string_from_data(Data, Size, &offset, 100);
                }
                array = cJSON_CreateStringArray((const char *const *)str_buf, count);
                safe_delete(array);
                for (int i = 0; i < count; i++) {
                    free(str_buf[i]);
                }
                free(str_buf);
            }
        }
    }

    /* Test 6: cJSON_InsertItemInArray */
    /* Create a base array to insert into */
    target_array = cJSON_CreateArray();
    if (target_array) {
        /* Create some items to insert */
        for (int i = 0; i < 5 && offset < Size; i++) {
            item = cJSON_CreateNumber((double)get_int_from_data(Data, Size, &offset));
            if (item) {
                which = get_int_from_data(Data, Size, &offset);
                result = cJSON_InsertItemInArray(target_array, which, item);
                /* If insertion failed, item might still be owned by caller, delete it */
                if (!result) {
                    safe_delete(item);
                }
            }
        }
        safe_delete(target_array);
    }

    /* Edge cases: negative counts, NULL pointers */
    array = cJSON_CreateIntArray(NULL, -1);
    safe_delete(array);
    array = cJSON_CreateDoubleArray(NULL, -1);
    safe_delete(array);
    array = cJSON_CreateFloatArray(NULL, -1);
    safe_delete(array);
    array = cJSON_CreateStringArray(NULL, -1);
    safe_delete(array);

    target_array = cJSON_CreateArray();
    if (target_array) {
        cJSON_InsertItemInArray(target_array, -1, NULL);
        cJSON_InsertItemInArray(target_array, 0, NULL);
        safe_delete(target_array);
    }

    return 0;
}