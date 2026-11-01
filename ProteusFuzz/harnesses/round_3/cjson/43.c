// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_SetNumberHelper at cJSON.c:411:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_IsNumber at cJSON.c:3022:26 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
// cJSON_GetNumberValue at cJSON.c:109:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Helper function to create a random double from fuzzing input */
static double get_random_double(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(double) > size) {
        return 0.0;
    }
    double val;
    memcpy(&val, data + *offset, sizeof(double));
    *offset += sizeof(double);
    return val;
}

/* Helper function to create a random string from fuzzing input */
static char *get_random_string(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) {
        return NULL;
    }
    size_t len = data[*offset];
    (*offset)++;
    if (len == 0 || *offset + len > size) {
        return NULL;
    }
    char *str = (char *)malloc(len + 1);
    if (!str) {
        return NULL;
    }
    memcpy(str, data + *offset, len);
    str[len] = '\0';
    *offset += len;
    return str;
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cJSON *obj = NULL;
    cJSON *num_item = NULL;
    cJSON *created_obj = NULL;
    cJSON *added_item = NULL;
    double num_val = 0.0;
    char *key_str = NULL;
    size_t offset = 0;

    /* Test cJSON_IsNumber with NULL */
    cJSON_IsNumber(NULL);

    /* Test cJSON_CreateNumber */
    if (offset < Size) {
        num_val = get_random_double(Data, Size, &offset);
        num_item = cJSON_CreateNumber(num_val);
    }

    /* Test cJSON_IsNumber on created number */
    if (num_item) {
        cJSON_IsNumber(num_item);
    }

    /* Test cJSON_CreateObject */
    created_obj = cJSON_CreateObject();

    /* Test cJSON_AddNumberToObject */
    if (created_obj && offset < Size) {
        key_str = get_random_string(Data, Size, &offset);
        if (key_str) {
            num_val = get_random_double(Data, Size, &offset);
            added_item = cJSON_AddNumberToObject(created_obj, key_str, num_val);
            free(key_str);
        }
    }

    /* Test cJSON_GetNumberValue */
    if (num_item) {
        cJSON_GetNumberValue(num_item);
    }
    if (added_item) {
        cJSON_GetNumberValue(added_item);
    }
    /* Test with non-number item (object) */
    if (created_obj) {
        cJSON_GetNumberValue(created_obj);
    }
    /* Test with NULL */
    cJSON_GetNumberValue(NULL);

    /* Test cJSON_SetNumberHelper */
    if (num_item) {
        double new_val = get_random_double(Data, Size, &offset);
        cJSON_SetNumberHelper(num_item, new_val);
    }
    /* Test with NULL */
    cJSON_SetNumberHelper(NULL, 0.0);

    /* Cleanup */
    if (num_item) {
        cJSON_Delete(num_item);
    }
    if (created_obj) {
        cJSON_Delete(created_obj);
    }

    return 0;
}